//
// Created by Kishchenko, Ivan on 11/11/21.
//

#pragma once

#include "logging/Logger.h"

#include "event/EventBus.h"

#include "service/Registry.h"
#include "service/wifi/WifiService.h"
#include "service/mqtt/MqttService.h"

template <class Reg>
class TApplication {
    Reg _reg;
public:
    Registry& getRegistry() {
        return _reg;
    }

    virtual void setup() {
        auto startTime = esp_timer_get_time();
        logging::info("starting...");

        getRegistry().template createSystem<WifiService>();
        getRegistry().template createSystem<MqttService>();

        for (auto service: getRegistry().getSystemServices()) {
            if (service) {
                logging::info("Setup %d", service->getServiceId());
                service->setup();
            }
        }
        for (auto service: getRegistry().getUserServices()) {
            if (service) {
                service->setup();
            }
        }

        getRegistry().getEventBus().schedule(100, true, [this]() {
            loop();
        });

        logging::info("started, %dms", esp_timer_get_time()-startTime);
    }

    virtual void loop() {
        for (auto service: getRegistry().getSystemServices()) {
            if (service) {
                service->loop();
            }
        }
        for (auto service: getRegistry().getUserServices()) {
            if (service) {
                service->loop();
            }
        }
    }
};