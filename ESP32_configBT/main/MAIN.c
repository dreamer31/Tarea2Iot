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

#include "protocols.c"
#include "configurer.c"
#include "config.h"
#include "sockets.c"
#include "connect.c"

extern bool is_Aconnected;
extern bool letsConfig;
extern char *newConfig;


//Esta función permite darle valores hardcodeados a la ESP32 si uno no quiere repetir
//el proceso de configuración por bluetooth cada vez.
void hardcodeConfiguration(char status, char id_protocol, int time)
{
    Config config; // datos hardcodeados
    config.status = status;
    config.ID_Protocol = id_protocol;
    config.BMI270_Sampling = 12;
    config.BMI270_Acc_Sensibility = 34;
    config.BMI270_Gyro_Sensibility = 56;
    config.BME688_Sampling = 78;
    config.Discontinuous_Time = time;
    config.Port_TCP = 5000;
    config.Port_UDP = 5005;
    config.Host_Ip_Addr = 0xc0a801db; // 0xc0a80501;
    strcpy(config.Ssid, "ZTE");
    strcpy(config.Pass, "pepopepo");
    writeConfiguration(config);
}


#define RESET_FLAG 1 //1 para el primer flasheo, 0 luego

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Wait for bluetooth configuration

    //Esto debe estar activo en el primer flasheo, y luego debe flashearse nuevamente con esta linea comentada
    if (RESET_FLAG)
    {
        reset_flash_flag();}
    else
    {
        if (read_flash_flag() == 200)
        {
            //En caso de que ya se haya configurado antes, 
            //pasa diractamente a empezar a mandar datos con la configuración guardada
            printf("FLAG %i: ", read_flash_flag());
            printf("Using saved CONFIG\n");
        }
        else
        {
            //Si la flag no está seteada, espera a ser configurado por BLE
            printf("FLAG %i: Waiting for BLE configuration", read_flash_flag());
            recvBLEConfigClient();
            write_flash_flag();
    }

    //Lee la configuración guardada en NVS y la deja en un struct para facil uso.
    Config config = readConfiguration();
    printConfig(config);

    //Conecta a la Wifi
    ESP_ERROR_CHECK(custom_connect(config.Ssid, config.Pass));
    printf("FUNCIONA!\n\n");

    //Inicia el main loop para mandar datos. Detalles en sockets.c
    xTaskCreate(main_client, "tcp_client", 4096, NULL, 5, NULL);
}
}
