//
// Created by Ivan Kishchenko on 05.06.2022.
//
#pragma once

#include <esp_wifi.h>
#include <cstring>

#include "service/Service.h"

LOG_COMPONENT_SETUP(wifi)

#define ESP_WIFI_SSID "ED7-6"
#define ESP_WIFI_PASS "1234554321"

struct WifiConnected : TEvent<Event_WIFI_CONNECTED> {
    explicit WifiConnected(const esp_netif_ip_info_t& info) : ip(info.ip), netmask(info.netmask), gw(info.gw) { }

    esp_ip4_addr_t ip;      /**< Interface IPV4 address */
    esp_ip4_addr_t netmask; /**< Interface IPV4 netmask */
    esp_ip4_addr_t gw;      /**< Interface IPV4 gateway address */
};

struct WifiDisconnected : TEvent<Event_WIFI_DISCONNECTED> {
};

class WifiService : public TService<Service_WIFI> {
    static void onEventHandler(void *self, esp_event_base_t group, int32_t id, void *args) {
        ((WifiService *) self)->onEventHandler(group, id, args);
    }

    void onEventHandler(esp_event_base_t group, int32_t id, void *args) {
        if (group == WIFI_EVENT && id == WIFI_EVENT_STA_START) {
            wifi::log::warning("STA started");
            esp_wifi_connect();
        } else if (group == WIFI_EVENT && id == WIFI_EVENT_STA_DISCONNECTED) {
            this->getRegistry().getEventBus().post(std::make_unique<WifiDisconnected>());

            auto* event = (wifi_event_sta_disconnected_t*) args;
            wifi::log::warning("STA connect to AP fail: %d, %s:%d, retry", event->reason, event->ssid, event->ssid_len);
            esp_wifi_connect();
        } else if (group == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
            auto *event = (ip_event_got_ip_t *) args;
            wifi::log::info("got ip: " IPSTR, IP2STR(&event->ip_info.ip));
            this->getRegistry().getEventBus().post(std::make_unique<WifiConnected>(event->ip_info));
        }
    }

public:
    explicit WifiService(Registry &registry) : TService(registry) {}

    void setup() override {
        ESP_ERROR_CHECK(esp_netif_init());

        esp_netif_create_default_wifi_sta();

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        ESP_ERROR_CHECK(esp_wifi_init(&cfg));

        wifi_config_t wifi_config = {
                .sta {
                        .threshold {
                                .authmode = WIFI_AUTH_WPA2_PSK,
                        }
                }
        };
        strncpy(reinterpret_cast<char *>(wifi_config.sta.ssid), ESP_WIFI_SSID, 32);
        strncpy(reinterpret_cast<char *>(wifi_config.sta.password), ESP_WIFI_PASS, 64);
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

        ESP_ERROR_CHECK(
                esp_event_handler_register(
                        WIFI_EVENT,
                        ESP_EVENT_ANY_ID,
                        &onEventHandler,
                        this
                )
        );
        ESP_ERROR_CHECK(
                esp_event_handler_register(
                        IP_EVENT,
                        IP_EVENT_STA_GOT_IP,
                        &onEventHandler,
                        this
                )
        );
        ESP_ERROR_CHECK(esp_wifi_start());

        TService::setup();
    }
};
