//
// Created by Kishchenko, Ivan on 11/12/21.
//

#pragma once

#include "logging/Logger.h"

#include <string_view>

#include "service/Service.h"

#include <mqtt_client.h>
#include <esp_tls.h>
#include <esp_crt_bundle.h>

LOG_COMPONENT_SETUP(mqtt);

struct MqttProperties {
    std::string clientId;
    std::string username;
    std::string password;
    std::string uri;
    const char *certDev;
    const char *rootCa;
};

struct MqttConnected : TEvent<Event_MQTT_CONNECTED> {
};


struct MqttDisconnected : TEvent<Event_MQTT_DISCONNECTED> {
};

#define MQTT_URI            "mqtts://mqtt.cloud.yandex.net:8883"
#define MQTT_CLIENT_ID      "esp32-c3"
#define MQTT_ACCESS_TOKEN   "esp32-dev-token"
#define MQTT_USERNAME       "are79hf9sme6issvd9hf"
#define MQTT_PASSWORD       "T6ZrIdAWKmX40h"

const char *cert_dev = nullptr;

const char *root_ca = "-----BEGIN CERTIFICATE-----\n"
                      "MIIFGTCCAwGgAwIBAgIQJMM7ZIy2SYxCBgK7WcFwnjANBgkqhkiG9w0BAQ0FADAf\n"
                      "MR0wGwYDVQQDExRZYW5kZXhJbnRlcm5hbFJvb3RDQTAeFw0xMzAyMTExMzQxNDNa\n"
                      "Fw0zMzAyMTExMzUxNDJaMB8xHTAbBgNVBAMTFFlhbmRleEludGVybmFsUm9vdENB\n"
                      "MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAgb4xoQjBQ7oEFk8EHVGy\n"
                      "1pDEmPWw0Wgw5nX9RM7LL2xQWyUuEq+Lf9Dgh+O725aZ9+SO2oEs47DHHt81/fne\n"
                      "5N6xOftRrCpy8hGtUR/A3bvjnQgjs+zdXvcO9cTuuzzPTFSts/iZATZsAruiepMx\n"
                      "SGj9S1fGwvYws/yiXWNoNBz4Tu1Tlp0g+5fp/ADjnxc6DqNk6w01mJRDbx+6rlBO\n"
                      "aIH2tQmJXDVoFdrhmBK9qOfjxWlIYGy83TnrvdXwi5mKTMtpEREMgyNLX75UjpvO\n"
                      "NkZgBvEXPQq+g91wBGsWIE2sYlguXiBniQgAJOyRuSdTxcJoG8tZkLDPRi5RouWY\n"
                      "gxXr13edn1TRDGco2hkdtSUBlajBMSvAq+H0hkslzWD/R+BXkn9dh0/DFnxVt4XU\n"
                      "5JbFyd/sKV/rF4Vygfw9ssh1ZIWdqkfZ2QXOZ2gH4AEeoN/9vEfUPwqPVzL0XEZK\n"
                      "r4s2WjU9mE5tHrVsQOZ80wnvYHYi2JHbl0hr5ghs4RIyJwx6LEEnj2tzMFec4f7o\n"
                      "dQeSsZpgRJmpvpAfRTxhIRjZBrKxnMytedAkUPguBQwjVCn7+EaKiJfpu42JG8Mm\n"
                      "+/dHi+Q9Tc+0tX5pKOIpQMlMxMHw8MfPmUjC3AAd9lsmCtuybYoeN2IRdbzzchJ8\n"
                      "l1ZuoI3gH7pcIeElfVSqSBkCAwEAAaNRME8wCwYDVR0PBAQDAgGGMA8GA1UdEwEB\n"
                      "/wQFMAMBAf8wHQYDVR0OBBYEFKu5xf+h7+ZTHTM5IoTRdtQ3Ti1qMBAGCSsGAQQB\n"
                      "gjcVAQQDAgEAMA0GCSqGSIb3DQEBDQUAA4ICAQAVpyJ1qLjqRLC34F1UXkC3vxpO\n"
                      "nV6WgzpzA+DUNog4Y6RhTnh0Bsir+I+FTl0zFCm7JpT/3NP9VjfEitMkHehmHhQK\n"
                      "c7cIBZSF62K477OTvLz+9ku2O/bGTtYv9fAvR4BmzFfyPDoAKOjJSghD1p/7El+1\n"
                      "eSjvcUBzLnBUtxO/iYXRNo7B3+1qo4F5Hz7rPRLI0UWW/0UAfVCO2fFtyF6C1iEY\n"
                      "/q0Ldbf3YIaMkf2WgGhnX9yH/8OiIij2r0LVNHS811apyycjep8y/NkG4q1Z9jEi\n"
                      "VEX3P6NEL8dWtXQlvlNGMcfDT3lmB+tS32CPEUwce/Ble646rukbERRwFfxXojpf\n"
                      "C6ium+LtJc7qnK6ygnYF4D6mz4H+3WaxJd1S1hGQxOb/3WVw63tZFnN62F6/nc5g\n"
                      "6T44Yb7ND6y3nVcygLpbQsws6HsjX65CoSjrrPn0YhKxNBscF7M7tLTW/5LK9uhk\n"
                      "yjRCkJ0YagpeLxfV1l1ZJZaTPZvY9+ylHnWHhzlq0FzcrooSSsp4i44DB2K7O2ID\n"
                      "87leymZkKUY6PMDa4GkDJx0dG4UXDhRETMf+NkYgtLJ+UIzMNskwVDcxO4kVL+Hi\n"
                      "Pj78bnC5yCw8P5YylR45LdxLzLO68unoXOyFz1etGXzszw8lJI9LNubYxk77mK8H\n"
                      "LpuQKbSbIERsmR+QqQ==\n"
                      "-----END CERTIFICATE-----";

class MqttService
        : public TService<Service_MQTT>,
          public TEventSubscriber<MqttService, WifiConnected, MqttConnected> {
    esp_mqtt_client_handle_t _client{};

    MqttProperties *_props{};
private:
    static void eventCallback(void *event_handler_arg, esp_event_base_t group, int32_t id, void *event_data) {
        ((MqttService *) event_handler_arg)->handleMqttEvent((esp_mqtt_event_handle_t) event_data);
    }

    void handleMqttEvent(esp_mqtt_event_handle_t event) {
        switch (event->event_id) {
            case MQTT_EVENT_CONNECTED: {
                mqtt::log::info("connected");
                getRegistry().getEventBus().post(std::make_unique<MqttConnected>());
                break;
                case MQTT_EVENT_DISCONNECTED:
                    mqtt::log::info("disconnected");
                break;
                case MQTT_EVENT_SUBSCRIBED:
                    mqtt::log::debug("subscribed");
                break;
                case MQTT_EVENT_PUBLISHED:
                    mqtt::log::debug("pub-ack: {}", event->msg_id);
                break;
                case MQTT_EVENT_ERROR:
                    mqtt::log::info("error");
                break;
                case MQTT_EVENT_DATA:
                    mqtt::log::debug("data: %s:%s", event->topic, std::string(event->data, event->data_len).c_str());
                break;
                default:
                    break;

            }
        }
    }

public:
    using TService<Service_MQTT>::TService;

    void setup() override {
        getRegistry().getEventBus().subscribe(this);

        //_props = getRegistry().getProperties().get<MqttProperties>(PROP_MQTT_PROPS);
        _props = new MqttProperties();
        _props->uri = MQTT_URI;
        _props->clientId = MQTT_CLIENT_ID;
        _props->username = MQTT_USERNAME;
        _props->password = MQTT_PASSWORD;
        _props->certDev = cert_dev;
        _props->rootCa = root_ca;

        mqtt::log::info(
                "user: %s, clientId: %s",
                _props->username.data(),
                _props->clientId.data()
        );
        mqtt::log::info(
                "server: %s",
                _props->uri.data()
        );

        if (_props->rootCa != nullptr) {
            ESP_ERROR_CHECK(esp_tls_init_global_ca_store());
            ESP_ERROR_CHECK(esp_tls_set_global_ca_store(
                    (const uint8_t *) _props->rootCa,
                    strlen(_props->rootCa) + 1)
            );
        }
    }

    void handleEvent(const WifiConnected &msg) {
        if (_client) {
            esp_mqtt_client_disconnect(_client);
            esp_mqtt_client_destroy(_client);
        }
        mqtt::log::info("handle wifi-conn, try connect to mqtt");
        const esp_mqtt_client_config_t config{
                .uri = _props->uri.c_str(),
                .client_id = _props->clientId.c_str(),
                .username = _props->username.c_str(),
                .password = _props->password.c_str(),
                .user_context = (void *) this,
                .cert_pem = _props->certDev,
                .use_global_ca_store = _props->rootCa != nullptr,
                .reconnect_timeout_ms = 10000
        };

        _client = esp_mqtt_client_init(&config);
        esp_mqtt_client_register_event(_client, MQTT_EVENT_ANY, eventCallback, this);
        esp_mqtt_client_start(_client);
    }

    void handleEvent(const MqttConnected &msg) {
        publish("$me/device/state", R"({"status": "active"})", 1);
    }

    void subscribe(std::string_view topic, uint8_t qos = 0) {
        mqtt::log::info("sub: %s:%d", topic.data(), qos);
        if (auto id = esp_mqtt_client_subscribe(_client, topic.data(), qos); id > 0) {
            mqtt::log::info("sub: %s:%d", topic.data(), id);
        } else {
            mqtt::log::error("sub failed: %s", topic.data());
        }
    }

    void publish(std::string_view topic, std::string_view data, uint8_t qos = 0) {
        mqtt::log::debug("pub: %s:%s", topic.data(), data.data());
        if (auto id = esp_mqtt_client_publish(_client, topic.data(), data.data(), (int) data.length(), qos, false); id > 0) {
            mqtt::log::debug("sent: %s:%d", topic.data(), id);
        } else {
            mqtt::log::error("send failed: %s:%d", topic.data(), id);
        }
    }
};