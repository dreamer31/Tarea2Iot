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
#include "wifi.c"
#include "sockets.c"
#include "connect.c"

extern bool is_Aconnected;
extern bool letsConfig;
extern char *newConfig;
// printf("Enabling timer wakeup, %lld\n", wakeup_time_sec);
// esp_sleep_enable_timer_wakeup(60*1000000);
// printf("goin to sleep for clk (60 seconds)...");
// esp_deep_sleep_start();
// static const char *TAG = "example";

// static void mode_UDP(void *pvParameters);
// static void mode_BLE_continua(void *pvParameters);
// static void mode_BLE_discontinua(void *pvParameters);

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

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */

    // Wait for bluetooth configuration
    reset_flash_flag();
    // hardcodeConfiguration(20, 0, 1);
    if (read_flash_flag() == 200)
    {
        printf("FLAG %i: ", read_flash_flag());
        printf("Using saved CONFIG\n");
    }
    else
    {
        //start in status 0 and wait for config
        printf("FLAG %i: ", read_flash_flag());
        printf("HARDCODING CONFIG\n");
        // hardcodeConfiguration(0, 0, 0);
        recvBLEConfigClient();
        write_flash_flag();
    }

    Config config = readConfiguration();
    printConfig(config);
    printf("ssid: %s, pass: %s\n", config.Ssid, config.Pass);

     ESP_ERROR_CHECK(custom_connect(config.Ssid, config.Pass));
    printf("FUNCIONA!\n\n");
    xTaskCreate(main_client, "tcp_client", 4096, NULL, 5, NULL);
}
