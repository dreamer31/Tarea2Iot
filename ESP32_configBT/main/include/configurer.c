#include "config.h"
#include "nvs.c"
#include <stdio.h>
#include "esp_log.h"
/*
Funciones útiles para leer y utilizar la configuración.

-unpackConfig: Dado un stream de bytes con una configuración codificada, lo desempaca en un struct Config

-readConfiguration: Entrega un struct Config con las configuraciones guardadas en NVS.

-writeConfiguration: Dado un struct Config, guarda la configuración en NVS

-ipNumToStr: Dado un int32 que codifique una IP, entrega la conversión a string

*/




Config unpackConfig(char *buffer)
{
    Config c;
    // config buffer es de 76 bytes
    //  int8_t status;
    c.status = buffer[0];
    // int8_t ID_Protocol;
    c.ID_Protocol = buffer[1];
    // int32_t BMI270_Sampling;
    memcpy(&(c.BMI270_Sampling), &buffer[2], 4);
    // int32_t BMI270_Acc_Sensibility;
    memcpy(&(c.BMI270_Acc_Sensibility), &buffer[6], 4);
    // int32_t BMI270_Gyro_Sensibility;
    memcpy(&(c.BMI270_Gyro_Sensibility), &buffer[10], 4);
    // int32_t BME688_Sampling;
    memcpy(&(c.BME688_Sampling), &buffer[14], 4);
    // int32_t Discontinuous_Time;
    memcpy(&(c.Discontinuous_Time), &buffer[18], 4);
    // int32_t Port_TCP;
    memcpy(&(c.Port_TCP), &buffer[22], 4);
    // int32_t Port_UDP;
    memcpy(&(c.Port_UDP), &buffer[26], 4);
    // int32_t Host_Ip_Addr;
    memcpy(&(c.Host_Ip_Addr), &buffer[30], 4);
    // char* Ssid;
    memcpy(c.Ssid, &buffer[34], 11);
    // char* Pass;
    memcpy(c.Pass, &buffer[45], 11);
    return c;
}


Config readConfiguration()
{
    Config conf;
    int8_t e8 = 0;
    int32_t e32 = 0;
    char *estr = "";

    Read_NVS(&e32, &(conf.status), estr, STATUS);
    Read_NVS(&e32, &(conf.ID_Protocol), estr, PROTOCOL);
    Read_NVS(&(conf.BMI270_Sampling), &e8, estr, BMI_SAMPLE);
    Read_NVS(&(conf.BMI270_Acc_Sensibility), &e8, estr, BMI_ACC);
    Read_NVS(&(conf.BMI270_Gyro_Sensibility), &e8, estr, BMI_GYRO);
    Read_NVS(&(conf.BME688_Sampling), &e8, estr, BME_SAMPLE);
    Read_NVS(&(conf.Discontinuous_Time), &e8, estr, DISC_TIME);
    Read_NVS(&(conf.Port_TCP), &e8, estr, TCP_PORT);
    Read_NVS(&(conf.Port_UDP), &e8, estr, UDP_PORT);
    Read_NVS(&(conf.Host_Ip_Addr), &e8, estr, HOST_IP);
    Read_NVS(&e32, &e8, conf.Ssid, SSID);
    Read_NVS(&e32, &e8, conf.Pass, PASS);

    return conf;
}

int writeConfiguration(Config conf)
{
    Write_NVS_int8(conf.status, STATUS);
    Write_NVS_int8(conf.ID_Protocol, PROTOCOL);
    Write_NVS_int32(conf.BMI270_Sampling, BMI_SAMPLE);
    Write_NVS_int32(conf.BMI270_Acc_Sensibility, BMI_ACC);
    Write_NVS_int32(conf.BMI270_Gyro_Sensibility, BMI_GYRO);
    Write_NVS_int32(conf.BME688_Sampling, BME_SAMPLE);
    Write_NVS_int32(conf.Discontinuous_Time, DISC_TIME);
    Write_NVS_int32(conf.Port_TCP, TCP_PORT);
    Write_NVS_int32(conf.Port_UDP, UDP_PORT);
    Write_NVS_int32(conf.Host_Ip_Addr, HOST_IP);
    Write_NVS_str(conf.Ssid, SSID);
    Write_NVS_str(conf.Pass, PASS);

    return 0;
}

char *ipNumToStr(int32_t ip)
{
    ESP_LOGI("debug", "IP num: %i", ip);
    char *str = malloc(20);
    int32_t a = (ip & 0xff000000) >> 24;
    int32_t b = (ip & 0xff0000) >> 16;
    int32_t c = (ip & 0xff00) >> 8;
    int32_t d = (ip & 0xff);
    ESP_LOGI("debug", "IP componentes: %i, %i, %i, %i", a, b, c, d);

    sprintf(str, "%i.%i.%i.%i", a, b, c, d);
    ESP_LOGI("debug", "IP str: %s", str);

    return str;
}

void printConfig(Config c)
{
    char* ip = ipNumToStr(c.Host_Ip_Addr);
    printf("CONFIGURATION:\n  Status: %i\n  Protocol: %i\n  BMI270_SAMP: %i\n  BMI270_ACC: %i\n  BMI270_GYRO: %i\n  BME688_SAMP: %i\n  Disc_Time: %i\n  TCP: %i\n  UDP: %i\n  IP: %s\n  SSID: %s\n  PASS: %s\n",
           c.status, c.ID_Protocol, c.BMI270_Sampling, c.BMI270_Acc_Sensibility, c.BMI270_Gyro_Sensibility, c.BME688_Sampling, c.Discontinuous_Time, c.Port_TCP, c.Port_UDP, ip, c.Ssid, c.Pass);
    free(ip);
}