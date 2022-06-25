#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_sleep.h"
#include "protocol_examples_common.h"
#include "addr_from_stdin.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "BLE_server.c"

/*

Aqui está la mayor parte de lo que hace que todo funcione.

TCP_socket y UDP_socket crean lo sockets correspondientes
TCP_connect: conecta un socket TCP a un server TCP

TCP_send_frag/UDP_send_frag: Manda un mensaje fragmentado en paquietes de a 1000 bytes. (para el protocolo 5)
TCP_send/UDP_send: Manda un mensaje sin fragmentar. (para los protocolos 0 a 4)

UDP_client: Cliente UDP que manda datos constantemente a una IP (independientemente de si hay server abierto). (status 23)
TCP_client: Cliente TCP que busca una conexión y al encontrarla manda datos constantemente. Puede funcionar de forma discontinua con deep sleep. (status 21 y 22)

Ambos tipos de cliente espran a recibir un mensaje de OK luego de cada envío de datos. El mensaje de OK puede venir con instrucciones de cambiar de status y/o protocolo, 
en cuyo caso el cliente se cierra y vuelve a abrir con las nuevas configuraciones

TCP_config_client: Cliente TCP especial que espera a que le llegue una configuración por TCP (status 20)
recvBLEConfigClient: cliente BLE que espera a que le llegue una configuración por BLE (status 0)

main_client: cliente "general" que lee la configuración y abre el subcliente especifico que se adapte al status y protocolo guardado. 
Funciona en un loop infinito de forma que si un subcliente cierra, vuelve a leer la configuración y abre un
subcliente nuevo (util en caso de que la configuración cambie luego de un OK).

Los status 30 y 31 no fueron implementados.

*/




extern bool is_Aconnected;
extern bool letsConfig;
extern char *newConfig;

#define PACK_LEN 1000
static const char *TAG = "sockets";

static void printArray(const char *string, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("\\%02x", *string++);
    }
    printf("\n");
}

int TCP_socket()
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
    }
    return sock;
}
int UDP_socket()
{
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
    }
    else
    {
        struct timeval tv;
        tv.tv_sec = 2; /* Secs Timeout */
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        printf("Setting timeout\n");
    }

    return sock;
}

int TCP_connect(int sock, char *host_ip, int port)
{

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(host_ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
    if (err != 0)
    {
        ESP_LOGE(TAG, "Socket unable to connect: errno %d.", errno);
    }

    return err;
}

int TCP_send_frag(int sock, char status, char protocolo)
{
    //Parte el mensaje (payload) en trozos de 1000 btyes y los manda por separado, esperando un OK con cada trozo
    printf("Sending!\n");
    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo) - 1;
    char rx_buffer[128];

    for (int i = 0; i < payloadLen; i += PACK_LEN)
    {

        // Generamos el siguiente trozo
        int size = fmin(PACK_LEN, payloadLen - i);
        char *pack = malloc(size);
        memcpy(pack, &(payload[i]), size);

        //Enviamos el trozo
        int err = send(sock, pack, size, 0);
        free(pack);
        if (err < 0)
        {
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        }

        // wait for confirmation
        int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        // Error occurred during receiving
        if (len < 0)
        {
            //En caso de error abortamos
            ESP_LOGE(TAG, "recv failed: errno %d", errno);
            break;
        }
        else
        {
            rx_buffer[len] = 0;
            char OK_r = rx_buffer[0];
            if (!OK_r)
            {
                ESP_LOGE(TAG, "Server error in fragmented send.");
                free(payload);
                return -1;
            }
        }
    }
    //el último mensaje es solo un \0 para avisarle al server que terminamos
    int err = send(sock, "\0", 1, 0);

    free(payload);

    return err;
}

int UDP_send_frag(int sock, char status, char protocolo, char *ip, int port)
{
    printf("sending!\n");
    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo) - 1;

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    // mandamos los trozos constantemente sin esperar confirmación.
    for (int i = 0; i < payloadLen; i += PACK_LEN)
    {
        vTaskDelay(200 / portTICK_PERIOD_MS);
        printf("Pack starts at %i\n", i);
        // send Pack
        int size = fmin(PACK_LEN, payloadLen - i);
        char *pack = malloc(size);
        memcpy(pack, &(payload[i]), size);
        int err = sendto(sock, pack, size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        free(pack);
        if (err < 0)
        {
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        }

    }
    //mandamos la indicación del fin del mensaje
    int err = sendto(sock, "\0", 1, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)); // send(sock, "\0", 1, 0);

    free(payload);

    return err;
}

int TCP_send(int sock, char status, char protocolo)
{

    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo) - 1;
    // ESP_LOGI("DEBUG", "prot %i, length: %i", protocolo, payloadLen);

    printArray(payload, payloadLen);
    int err = send(sock, payload, payloadLen, 0);
    free(payload);
    if (err < 0)
    {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
    }
    return err;
}

int UDP_send(int sock, char status, char protocolo, char *ip, int port)
{
    char *payload = mensaje(protocolo, status);
    int payloadLen = messageLength(protocolo) - 1;
    printArray(payload, payloadLen);

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = inet_addr(ip);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    int err = sendto(sock, payload, payloadLen, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    free(payload);
    if (err < 0)
    {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
    }
    return err;
}


static void UDP_client()
{
    Config conf = readConfiguration();
    char *host_ip = ipNumToStr(conf.Host_Ip_Addr);
    char protocolo = conf.ID_Protocol;
    char status = conf.status;

    char rx_buffer[128];

    while (1)
    {
        int sock = UDP_socket();
        if (sock < 0)
        {
            if (sock != -1)
            {
                ESP_LOGE(TAG, "Error creating socket: Shutting down socket and restarting... Errno %i", sock);
                close(sock);
            }
            continue;
        }
        ESP_LOGI(TAG, "Socket created, sending to %s:%d", host_ip, conf.Port_UDP);
        while (1)
        {
            //Mandamos mensajes constantemente
            int err;
            if (protocolo == 5)//Para el protocolo 5 hay que mandar el mensaje fragmentado
            {
                err = UDP_send_frag(sock, status, protocolo, host_ip, conf.Port_UDP);
            }
            else
            {
                err = UDP_send(sock, status, protocolo, host_ip, conf.Port_UDP);
            }

            if (err < 0)
            {
                printf("err %i\n", err);
                close(sock);
                break;
            }

            //Esperamos recibir un OK luego de cada mensaje
            ESP_LOGI(TAG, "Sent data, waiting recv");
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0); //, NULL, NULL);
            // Error occurred during receiving
            if (len < 0)
            {
                //En caso de error cerramos todo y volvemos a intentar conectarnos
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                close(sock);
                return;
            }
            // Data received
            else
            {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string

                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                char OK_r = rx_buffer[0];
                char CHANGE_r = rx_buffer[1];
                char STATUS_r = rx_buffer[2];
                char PROTOCOL_r = rx_buffer[3];
                ESP_LOGI(TAG, "OK: %i, Change: %i, New status: %i, New protocol: %i", OK_r, CHANGE_r, STATUS_r, PROTOCOL_r);
                if (CHANGE_r)
                {   //En caso de que mandemos una instrucción de cambio, guardamos los nuevos status/protocolo y retornamos.
                    //Main client se encargará de elegir el cliente apropiado para enviar los nuevos mensajes
                    conf.status = STATUS_r;
                    conf.ID_Protocol = PROTOCOL_r;
                    writeConfiguration(conf);
                    close(sock);
                    return;
                }

            }

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        if (sock != -1)
        {
            ESP_LOGE(TAG, "End of Loop: Shutting down socket and restarting...");
            close(sock);
            
            vTaskDelay(1000 / portTICK_PERIOD_MS);

        }
    }
    free(host_ip);
}

static void TCP_client(int disc)
{
    Config conf = readConfiguration();
    char rx_buffer[128];
    char *host_ip = ipNumToStr(conf.Host_Ip_Addr);
    char protocolo = conf.ID_Protocol;
    char status = conf.status;
    printf("Status: %i, Protocol: %i\n", status, protocolo);

    while (1)
    {

        int sock = TCP_socket();
        if (sock < 0)
        {
            if (sock != -1)
            {
                ESP_LOGE(TAG, "Shutting down socket and restarting...");
                shutdown(sock, 0);
                close(sock);
            }
            continue;
        }

        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, conf.Port_TCP);
        //Intentamos constantemente conectarnos. En caso de timeout o error vuelve a intentarlo
        int err = TCP_connect(sock, host_ip, conf.Port_TCP);
        if (err != 0)
        {
            if (sock != -1)
            {
                ESP_LOGE(TAG, "Shutting down socket and restarting...");
                shutdown(sock, 0);
                close(sock);
            }
            continue;
        }

        ESP_LOGI(TAG, "Successfully connected");
        while (1)
        {
            //Una vez hecha la conexión mandamos mensajes constantemente
            //En caso de cualquier error, salimos del loop y volvemos a intentar conectarnos
            int err;
            if (protocolo == 5)//Para el protocolo 5 hay que mandar el mensaje fragmentado
            {
                printf("Sending fragmented\n");
                err = TCP_send_frag(sock, status, protocolo);
            }
            else
            {
                err = TCP_send(sock, status, protocolo);
            }

            if (err < 0)
            {
                printf("Error %i\n", err);
                break;
            }
            //espreamos el OK
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occurred during receiving
            if (len < 0)
            {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Data received
            else
            {
                rx_buffer[len] = 0; 

                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                char OK_r = rx_buffer[0];
                char CHANGE_r = rx_buffer[1];
                char STATUS_r = rx_buffer[2];
                char PROTOCOL_r = rx_buffer[3];
                ESP_LOGI(TAG, "OK: %i, Change: %i, New status: %i, New protocol: %i", OK_r, CHANGE_r, STATUS_r, PROTOCOL_r);

                if (CHANGE_r)//En caso ed recibir una instrucción de cambio, cambiamos.
                {
                    conf.status = STATUS_r;
                    conf.ID_Protocol = PROTOCOL_r;
                    writeConfiguration(conf);
                    shutdown(sock, 0);
                    close(sock);
                    return;
                }
                if (conf.Discontinuous_Time && disc)//Para el modo discontinuo, hay que apagar el wifi antes de hacer deep_sleep
                {
                    printf("Enabling timer wakeup, %i\n", conf.Discontinuous_Time);
                    esp_sleep_enable_timer_wakeup(conf.Discontinuous_Time * 60 * 1000000 - 5 * 1000000);//Se le restan 5 segundos al sleep porque apagar y restaurar el wifi toma tiempo
                    printf("goin to sleep...");
                    esp_wifi_stop();
                    esp_deep_sleep_start();
                    esp_wifi_restore();
                }
            }

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        if (sock != -1)
        {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    free(host_ip);
}

static void TCP_config_client()
{
    Config conf = readConfiguration();
    char rx_buffer[128];
    char *host_ip = ipNumToStr(conf.Host_Ip_Addr);

    printf("Waiting for TCP configuration\n");

    while (1)
    {

        int sock = TCP_socket();
        if (sock < 0)
        {
            if (sock != -1)
            {
                ESP_LOGE(TAG, "Shutting down socket and restarting...");
                shutdown(sock, 0);
                close(sock);
            }
            continue;
        }

        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, conf.Port_TCP);
        //Intentamos conectarnos hasta que funcione
        int err = TCP_connect(sock, host_ip, conf.Port_TCP);
        if (err != 0)
        {
            if (sock != -1)
            {
                ESP_LOGE(TAG, "Shutting down socket and restarting...");
                shutdown(sock, 0);
                close(sock);
            }
            continue;
        }

        ESP_LOGI(TAG, "Successfully connected");
        while (1)
        {
            //Esperamos a recibir una configuración
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occurred during receiving
            if (len < 0)
            {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Data received
            else
            {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string

                //Desempacamos la configuración recibida
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                Config recvConfig = unpackConfig(rx_buffer);
                printConfig(recvConfig);

                //devolvemos un OK
                int err = TCP_send(sock, 20, 0);
                if (err < 0)
                {
                    printf("Error %i\n", err);
                    break;
                }
                else{//En caso de que no haya error, guardamos la configuración y retornamos, pra que main_client haga su trabajo
                    writeConfiguration(recvConfig);
                    return;
                }
                
            }

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        if (sock != -1)
        {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    free(host_ip);
}

static void recvBLEConfigClient(){
    BLE_server_Init(500); //It was the default value in the example
        while (1)
        {
            static bool inicio = true;
            while (1)
            {
                vTaskDelay(1);
                if (is_Aconnected && inicio)
                {
                    printf("Connection established\n");
                    printf("gatts_if %d, conn_id: %d, handle %d\n", gl_profile_tab[PROFILE_A_APP_ID].gatts_if, gl_profile_tab[PROFILE_A_APP_ID].conn_id, gl_profile_tab[PROFILE_A_APP_ID].char_handle);
                    inicio = false;
                }

                if (is_Aconnected && letsConfig)
                {
                    tx_len = 2;
                    tx_data = (unsigned char *)malloc(sizeof(char) * 2);
                    tx_data[0] = 'O';
                    tx_data[1] = 'K';
                    ESP_ERROR_CHECK(BLE_send(tx_len, tx_data, false));
                    vTaskDelay(1000 / portTICK_PERIOD_MS);
                    letsConfig = false;
                    break;
                }
            }
            Config config1 = unpackConfig(newConfig);
            printConfig(config1);
            printf("Se leyo el pack config");
            writeConfiguration(config1);
            esp_bluedroid_disable();
            esp_bluedroid_deinit();
            esp_bt_controller_disable();
            esp_bt_controller_deinit();
            return;
        }
        
}

static void main_client(void *pvParameters)
{
    while (1)
    {
        //Leemos la configuración y abrimos el cliente correspondiente. 
        //Al cerrarse el cliente por cualquier razón repetimos, abriendo un cliente nuevo.
        Config conf = readConfiguration();
        int status = conf.status;
        printf("Starting client on Status %i, protocol %i\n", status, conf.ID_Protocol);
        switch (status)
        {
        case 0:
            esp_wifi_stop();
            recvBLEConfigClient();
            esp_wifi_restore();
            break;
        case 20:
            TCP_config_client();
            break;
        case 21:
            TCP_client(0);
            break;
        case 22:
            TCP_client(1);
            break;
        case 23:
            UDP_client();
            break;
        case 30:
            printf("Status %i not implemented\n", status);
            break;
        case 31:
            printf("Status %i not implemented\n", status);
            break;
        default:
            printf("Status %i does not exist\n", status);
            conf.status = 0;
            conf.ID_Protocol = 0;
            writeConfiguration(conf);
            break;
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}