#include  <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "./configure_wifi.h"
#include "freertos/FreeRTOS.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_wifi.h"

#include "nvs_flash.h"

#include "configure_wifi.h"

void app_main() {
    nvs_flash_init();
    esp_netif_init();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    // Init dummy AP to specify a channel and get WiFi hardware into
    // a mode where we can send the actual fake beacon frames.
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));

    ESP_ERROR_CHECK(esp32_deauther_configure_wifi(/* channel */ 1));

    ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    while (1)
    {
        printf("goshooo\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1000 milliseconds (1 second)
    }
}