#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"

#define STATUS 1
#define PROTOCOL 2
#define BMI_SAMPLE 3
#define BMI_ACC 4
#define BMI_GYRO 5
#define BME_SAMPLE 6
#define DISC_TIME 7
#define TCP_PORT 8
#define UDP_PORT 9
#define HOST_IP 10
#define SSID 11
#define PASS 12

int write_flash_flag()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    nvs_handle_t my_handle;
    err = nvs_open("Storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        // printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }
    else
    {
        err = nvs_set_i32(my_handle, "FLAG", 200);
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "i32 Failed!\n" : "Done\n");
        //  Close
        nvs_close(my_handle);
    }
    fflush(stdout);
    return 0;
}

int reset_flash_flag()
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    nvs_handle_t my_handle;
    err = nvs_open("Storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        // printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }
    else
    {
        err = nvs_set_i32(my_handle, "FLAG", 42);
        err = nvs_commit(my_handle);
        printf((err != ESP_OK) ? "i32 Failed!\n" : "Done\n");
        //  Close
        nvs_close(my_handle);
    }
    fflush(stdout);
    return 0;
}

int read_flash_flag()
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    int data;
    // Open
    // //printf("Opening NVS .. ");
    nvs_handle_t my_handle;
    err = nvs_open("Storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }
    else{
        err = nvs_get_i32(my_handle, "FLAG", &data);
        nvs_close(my_handle);
        switch (err)
        {
        case ESP_OK:
            nvs_close(my_handle);
            return data;
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            return -1;
            break;
        default:
            printf("Error (%d) reading!\n", err);
            return -1;
        }
    }
}
int Write_NVS_str(char *datastr, int key)
{
    // //printf("Saving str: %s, key: %i\n", datastr, key);

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    // //printf("Opening NVS .. ");
    nvs_handle_t my_handle;
    err = nvs_open("Storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        // printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }
    else
    {
        // //printf("Done\n");
        // // Write
        // //printf("Updating restart counter in NVS ... ");
        switch (key)
        {
        case 11:
            err = nvs_set_str(my_handle, "Ssid", datastr);
            break;
        case 12:
            err = nvs_set_str(my_handle, "Pass", datastr);
            break;
        default:
            // printf("ERROR key");
            break;
        }
        // //printf((err != ESP_OK) ? "Failed in NVS!\n" : "Done\n");
        // //printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        // //printf((err != ESP_OK) ? "STR Failed!\n" : "Done\n");
        // Close
        nvs_close(my_handle);
    }
    fflush(stdout);
    return 0;
}

int Write_NVS_int32(int32_t data, int key)
{
    // //printf("Saving i32: %i, key: %i\n", data, key);

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    // //printf("Opening NVS .. ");
    nvs_handle_t my_handle;
    err = nvs_open("Storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        // printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }
    else
    {
        // //printf("Done\n");
        // // Write
        // //printf("Updating restart counter in NVS ... ");
        switch (key)
        {
        case 3:
            err = nvs_set_i32(my_handle, "BMI270_Sampling", data);
            break;
        case 4:
            err = nvs_set_i32(my_handle, "BMI270_Acc", data);
            break;
        case 5:
            err = nvs_set_i32(my_handle, "BMI270_Gyro", data);
            break;
        case 6:
            err = nvs_set_i32(my_handle, "BME688_Sampling", data);
            break;
        case 7:
            err = nvs_set_i32(my_handle, "Disc_Time", data);
            break;
        case 8:
            err = nvs_set_i32(my_handle, "Port_TCP", data);
            break;
        case 9:
            err = nvs_set_i32(my_handle, "Port_UDP", data);
            break;
        case 10:
            err = nvs_set_i32(my_handle, "Host_Ip_Addr", data);
            break;
        default:
            printf("ERROR key");
            break;
        }
        // printf((err != ESP_OK) ? "Failed in NVS!\n" : "Done\n");
        //  //printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        // printf((err != ESP_OK) ? "i32 Failed!\n" : "Done\n");
        //  Close
        nvs_close(my_handle);
    }
    fflush(stdout);
    return 0;
}

int Write_NVS_int8(int8_t data8, int key)
{
    // printf("Saving i8: %i, key: %i\n", data8, key);

    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open
    // //printf("Opening NVS .. ");
    nvs_handle_t my_handle;
    err = nvs_open("Storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }
    else
    {
        // //printf("Done\n");
        // // Write
        // //printf("Updating restart counter in NVS ... ");
        switch (key)
        {
        case 1:
            err = nvs_set_i8(my_handle, "status", data8);
            break;
        case 2:
            err = nvs_set_i8(my_handle, "ID_Protocol", data8);
            break;
        default:
            printf("ERROR key");
            break;
        }
        // printf((err != ESP_OK) ? "Failed in NVS!\n" : "Done\n");
        //  //printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
        // printf((err != ESP_OK) ? "i8 Failed!\n" : "Done\n");
        //  Close
        nvs_close(my_handle);
    }
    fflush(stdout);
    return 0;
}

int Read_NVS(int32_t *data, int8_t *data8, char *datastr, int key)
{
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    ESP_ERROR_CHECK(err);

    // Open
    // //printf("\n");
    // //printf("Opening Non-Volatile Storage (NVS) handle... ");
    nvs_handle_t my_handle;
    err = nvs_open("Storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        // printf("Error (%d) opening NVS handle!\n", err);
        return -1;
    }
    else
    {
        // //printf("Done\n");

        // // Read
        // //printf("Reading from NVS ... ");
        size_t required_size = 21;
        switch (key)
        {
        case 1:
            err = nvs_get_i8(my_handle, "status", data8);
            break;
        case 2:
            err = nvs_get_i8(my_handle, "ID_Protocol", data8);
            break;
        case 3:
            err = nvs_get_i32(my_handle, "BMI270_Sampling", data);
            break;
        case 4:
            err = nvs_get_i32(my_handle, "BMI270_Acc", data);
            break;
        case 5:
            err = nvs_get_i32(my_handle, "BMI270_Gyro", data);
            break;
        case 6:
            err = nvs_get_i32(my_handle, "BME688_Sampling", data);
            break;
        case 7:
            err = nvs_get_i32(my_handle, "Disc_Time", data);
            break;
        case 8:
            err = nvs_get_i32(my_handle, "Port_TCP", data);
            break;
        case 9:
            err = nvs_get_i32(my_handle, "Port_UDP", data);
            break;
        case 10:
            err = nvs_get_i32(my_handle, "Host_Ip_Addr", data);
            break;
        case 11:
            err = nvs_get_str(my_handle, "Ssid", datastr, &required_size);
            break;
        case 12:
            err = nvs_get_str(my_handle, "Pass", datastr, &required_size);
            break;
        default:
            printf("ERROR key");
            break;
        }
        switch (err)
        {
        case ESP_OK:
            // //printf("Done\n");
            //  //printf("Value Data = %d\n", *data);
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            printf("The value is not initialized yet!\n");
            break;
        default:
            printf("Error (%d) reading!\n", err);
        }
        // //printf("Committing updates in NVS ... ");
        // printf((err != ESP_OK) ? "read Failed!: %i\n" : "Done\n", key);
        // Close
        nvs_close(my_handle);
    }
    fflush(stdout);
    return 0;
}
