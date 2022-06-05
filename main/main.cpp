/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"

#include "event/EventBus.h"

extern "C" {
    void app_main(void);
}

#include "service/Application.h"
#include "event/EventSubscriber.h"

class FeederApp
        : public TApplication<TRegistry<EventBus>>, public TEventSubscriber<FeederApp, WifiConnected> {
public:
    void setup() override {
        getRegistry().getEventBus().subscribe(this);
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
