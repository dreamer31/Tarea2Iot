#ifndef Paramet
#define Paramet

    typedef struct Config
    {

        int8_t status;
        int8_t ID_Protocol;
        int32_t BMI270_Sampling;
        int32_t BMI270_Acc_Sensibility; 
        int32_t BMI270_Gyro_Sensibility; 
        int32_t BME688_Sampling;
        int32_t Discontinuous_Time;
        int32_t Port_TCP;
        int32_t Port_UDP;
        int32_t Host_Ip_Addr;
        char Ssid[21];
        char Pass[21];
    }Config;

#endif
