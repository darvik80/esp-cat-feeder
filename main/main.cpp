/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <driver/gpio.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"

#include "led_strip.h"

#include "event/EventBus.h"

extern "C" {
    void app_main(void);
}

#include "service/Application.h"
#include "event/EventSubscriber.h"

#define BLINK_LED_GPIO GPIO_NUM_4
#define BLINK_LED_RMT_CHANNEL 1

class FeederApp
        : public TApplication<TRegistry<EventBus>>, public TEventSubscriber<FeederApp, WifiConnected> {
    led_strip_t *_strip{nullptr};
public:
    void setup() override {
        getRegistry().getEventBus().subscribe(this);

        gpio_reset_pin(BLINK_LED_GPIO);
        /* Set the GPIO as a push/pull output */
        gpio_set_direction(BLINK_LED_GPIO, GPIO_MODE_OUTPUT);

        _strip = led_strip_init(BLINK_LED_RMT_CHANNEL, BLINK_LED_GPIO, 1);
        _strip->clear(_strip, 50);

        _strip->set_pixel(_strip, 0, 255, 255, 255);
        _strip->refresh(_strip, 100);
        TApplication::setup();
    }

    void loop() override {
        TApplication::loop();
    }

    void handleEvent(const WifiConnected &msg) {
        logging::info(
                "onWifiConnected: " IPSTR "/" IPSTR ", gw: " IPSTR,
                IP2STR(&msg.ip),
                IP2STR(&msg.netmask),
                IP2STR(&msg.gw)
        );
    }
};

void app_main(void) {
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf(
            "This is %s chip with %d CPU core(s), WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : ""
    );

    printf("silicon revision %d, ", chip_info.revision);

    printf(
            "%uMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external"
    );

    printf("Minimum free heap size: %d bytes\n", esp_get_minimum_free_heap_size());

    FeederApp app;
    app.setup();

    for (;;) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
