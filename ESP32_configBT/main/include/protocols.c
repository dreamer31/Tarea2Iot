#include <math.h>
#include <stdlib.h>
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_log.h"

/*

Aqui generamos los 5 tipos de protocolos con sus datos.
Las timestamps en realidad siempre mandamos 0, y por comodidad 
guardamos la timestampo del tiempo de llegada en el servidor de la raspberry.


En general los "mensajes" los creamos copiando a la mala (con memcpy) la memoria de los datos en un char*.
No es muy elegante pero funciona.

Al final lo único que se usa fuera de este archivo es:

message: dado un protocolo, crea un mensaje (con header y datos) codificado en un array de bytes (char*).
messageLength: dado un protocolo, entrega el largo del mensaje correspondiente

*/




float floatrand(float min, float max){
    return min + (float)rand()/(float)(RAND_MAX/(max-min));
}


int16_t* acc_sensor_acc_x(){
    int16_t* arr = malloc(2000* sizeof(int16_t));
    for (int i =0; i <2000; i++){
        arr[i] = (int16_t)floatrand(-8000, 8000);
    }
    return arr;
}
int16_t* acc_sensor_acc_y(){
    int16_t* arr = malloc(2000* sizeof(int16_t));
    for (int i =0; i <2000; i++){
        arr[i] = (int16_t)floatrand(-8000, 8000);
    }
    return arr;
}
int16_t* acc_sensor_acc_z(){
    int16_t* arr = malloc(2000* sizeof(int16_t));
    for (int i =0; i <2000; i++){
        arr[i] = (int16_t)floatrand(-8000, 8000);
    }
    return arr;
}
char batt_sensor(){
    char n =(char) 1 + (rand() %100);
    return n;
}

float acc_kpi_amp_x(){
    return floatrand(0.0059, 0.12);
}
float acc_kpi_amp_y(){
    return floatrand(0.0041, 0.11);
}
float acc_kpi_amp_z(){
    return floatrand(0.008, 0.15);
}

float acc_kpi_frec_x(){
    return floatrand(29, 31);
}
float acc_kpi_frec_y(){
    return floatrand(59, 61);
}
float acc_kpi_frec_z(){
    return floatrand(89, 91);
}

float acc_kpi_RMS(){
    //((Amp_x)^2+ (Amp_y)^2+(Amp_z)^2)^1/2
    float n = pow(acc_kpi_amp_x(), 2) + pow(acc_kpi_amp_y(), 2) + pow(acc_kpi_amp_z(), 2);
    return pow(n, 0.5);
}

char THPC_sensor_temp(){
    char n =(char) 15 + (rand() %16);
    return n;
}

char THPC_sensor_hum(){
    char n =(char) 30 + (rand() %51);
    return n;
}

float THPC_sensor_pres(){
    return floatrand(1000,1200);
}

float THPC_sensor_co(){
    return floatrand(30,200);
}

unsigned short lengmsg[6] = {2, 6, 16, 20, 44, 12016};
unsigned short dataLength(char protocol){
    return lengmsg[ (unsigned int) protocol]-1;
}
unsigned short messageLength(char protocol){
    return 1+10+dataLength(protocol);
}


//Genera el header de un mensaje, con la MAC, el protocolo, status, y el largo del mensaje.
char* header(char protocol, char status){
	//ESP_LOGI("debug", "header");
	char* head = malloc(10);

	uint8_t* MACaddrs = malloc(6);
	esp_efuse_mac_get_default(MACaddrs);
	memcpy((void*) &(head[0]), (void*) MACaddrs, 6);
	head[6]= protocol;
    head[7]= status;
	unsigned short dataLen = dataLength(protocol);
	memcpy((void*) &(head[8]), (void*) &dataLen, 2);
	free(MACaddrs);
	return head;
}


char* dataprotocol0(){
    char* msg = malloc(dataLength(0));
    msg[0] = 1;
    return msg;
}

char* dataprotocol1(){
    
    //ESP_LOGI("debug", "dataprotocol0, antes");
    //srand((unsigned) time(NULL));
    char* msg = malloc(dataLength(1));
    float batt = batt_sensor();
    msg[0] = batt;
    long t = 0;
    //ESP_LOGI("debug", "dataprotocol0, en el lugar");
    memcpy((void*) &(msg[1]), (void*) &t, 4);
    //ESP_LOGI("debug", "dataprotocol0, despues");
    return msg;
}

char* dataprotocol2(){
    
    //ESP_LOGI("debug", "dataprotocol1");
    //srand((unsigned) time(NULL));
    char* msg = malloc(dataLength(2));

    float batt = batt_sensor();
    msg[0] = batt;


    int t = 0;
    memcpy((void*) &(msg[1]), (void*) &t, 4);

    char temp = THPC_sensor_temp();
    msg[5] = temp;


    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[6]), (void*) &press, 4);

    char hum = THPC_sensor_hum();
    msg[10] = hum;

    float co = THPC_sensor_co();
    memcpy((void*) &(msg[11]), (void*) &co, 4);

    return msg;
}

char* dataprotocol3(){
    
    //ESP_LOGI("debug", "dataprotocol2");
    //srand((unsigned) time(NULL));
    char* msg = malloc(dataLength(3));

    float batt = batt_sensor();
    msg[0] = batt;


    long t = 0;
    memcpy((void*) &(msg[1]), (void*) &t, 4);

    char temp = THPC_sensor_temp();
    msg[5] = temp;


    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[6]), (void*) &press, 4);

    char hum = THPC_sensor_hum();
    msg[10] = hum;

    float co = THPC_sensor_co();
    memcpy((void*) &(msg[11]), (void*) &co, 4);

    float rms = acc_kpi_RMS();
    memcpy((void*) &(msg[15]), (void*) &rms, 4);


    return msg;
}

char* dataprotocol4(){
    
    //ESP_LOGI("debug", "dataprotocol3");
    //srand((unsigned) time(NULL));
    char* msg = malloc(dataLength(4));

    float batt = batt_sensor();
    msg[0] = batt;


    long t = 0;
    memcpy((void*) &(msg[1]), (void*) &t, 4);

    char temp = THPC_sensor_temp();
    msg[5] = temp;


    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[6]), (void*) &press, 4);

    char hum = THPC_sensor_hum();
    msg[10] = hum;

    float co = THPC_sensor_co();
    memcpy((void*) &(msg[11]), (void*) &co, 4);

    float rms = acc_kpi_RMS();
    memcpy((void*) &(msg[15]), (void*) &rms, 4);


    float ampx = acc_kpi_amp_x();
    memcpy((void*) &(msg[19]), (void*) &ampx, 4);

    float frecx = acc_kpi_frec_x();
    memcpy((void*) &(msg[23]), (void*) &frecx, 4);


    float ampy = acc_kpi_amp_y();
    memcpy((void*) &(msg[27]), (void*) &ampy, 4);

    float frecy = acc_kpi_frec_y();
    memcpy((void*) &(msg[31]), (void*) &frecy, 4);


    float ampz = acc_kpi_amp_z();
    memcpy((void*) &(msg[35]), (void*) &ampz, 4);

    float frecz = acc_kpi_frec_z();
    memcpy((void*) &(msg[39]), (void*) &frecz, 4);

    return msg;
}

char* dataprotocol5(){
    
    //ESP_LOGI("debug", "dataprotocol4");
    //srand((unsigned) time(NULL));
    char* msg = malloc(dataLength(5));

    float batt = batt_sensor();
    msg[0] = batt;


    int t = (int) time(NULL);
    memcpy((void*) &(msg[1]), (void*) &t, 4);

    char temp = THPC_sensor_temp();
    msg[5] = temp;


    float press = THPC_sensor_pres();
    memcpy((void*) &(msg[6]), (void*) &press, 4);

    char hum = THPC_sensor_hum();
    msg[11] = hum;

    float co = THPC_sensor_co();
    memcpy((void*) &(msg[11]), (void*) &co, 4);

    int16_t* accx = acc_sensor_acc_x();
    memcpy((void*) &(msg[15]), (void*) accx, 2000*sizeof(int16_t));
    free(accx);

    int16_t* accy = acc_sensor_acc_y();
    memcpy((void*) &(msg[15+2000*sizeof(int16_t)]), (void*) accy, 2000*sizeof(int16_t));
    free(accy);

    int16_t* accz = acc_sensor_acc_z();
    memcpy((void*) &(msg[15+4000*sizeof(int16_t)]), (void*) accz, 2000*sizeof(int16_t));
    free(accz);
    return msg;
}



char* mensaje (char protocol, char status){
	//ESP_LOGI("debug", "mensaje");
	char* mnsj = malloc(messageLength(protocol));
	mnsj[messageLength(protocol)-1]= '\0';
	char* hdr = header(protocol, status);
	char* data;
	switch (protocol) {
		case 0:
			data = dataprotocol0();
			break;
		case 1:
			data = dataprotocol1();
			break;
		case 2:
			data = dataprotocol2();
			break;
		case 3:
			data = dataprotocol3();
			break;
        case 4:
			data = dataprotocol4();
			break;
        case 5:
			data = dataprotocol5();
			break;
		default:
			data = dataprotocol0();
			break;
	}
	memcpy((void*) mnsj, (void*) hdr, 10);
	memcpy((void*) &(mnsj[10]), (void*) data, dataLength(protocol));
	free(hdr);
	//ESP_LOGI("debug", );
	free(data);
	return mnsj;
}