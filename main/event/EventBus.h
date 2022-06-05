//
// Created by Ivan Kishchenko on 04.06.2022.
//


#pragma once

#include <esp_event_base.h>
#include <esp_event.h>
#include "EventLogger.h"
#include "EventSubscriber.h"
#include "EventProducer.h"
#include "system/Timer.h"

class EventBus : public EventSubscriber, public EventProducer {
    esp_event_base_t _id;
    std::vector<EventSubscriber *> _subscribers;
    std::array<Timer, 32> _timers;

    struct EventHolder {
        Event *ptr;
    };

    static void onEventHandler(void *self, esp_event_base_t event_base, int32_t id, void *args) {
        bus::log::info("event handle: %s, %d", event_base, id);
        switch (id) {
            case 0:
                ((EventBus *) self)->onSystemHandler(args);
                break;
            default:
                break;
        }
    }

    void onSystemHandler(void *args) {
        auto *holder = (EventHolder *) args;
        bus::log::info("handle system");
        if (holder && holder->ptr) {
            onEvent(*holder->ptr);
            delete holder->ptr;
        }
    }

public:
    explicit EventBus(esp_event_base_t id = "main") : _id(id) {
        ESP_ERROR_CHECK(esp_event_loop_create_default());
        // Register system event
        ESP_ERROR_CHECK(esp_event_handler_register(_id, ESP_EVENT_ANY_ID, onEventHandler, this));
    }

    void subscribe(EventSubscriber &subscriber) {
        _subscribers.emplace_back(&subscriber);
    }

    void subscribe(EventSubscriber* subscriber) {
        _subscribers.emplace_back(subscriber);
    }

    void onEvent(const Event &msg) override {
        bus::log::debug("on event: {}:{}", msg.getEventId(), _subscribers.size());
        for (const auto sub: _subscribers) {
            sub->onEvent(msg);
        }
    }

    void post(Event::Ptr &&event) override {
        EventHolder holder{event.release()};
        ESP_ERROR_CHECK(esp_event_post(_id, 0, &holder, sizeof(holder), portMAX_DELAY));
    }

    void postISR(Event::Ptr &&event) override {
        EventHolder holder{event.release()};
        ESP_ERROR_CHECK(esp_event_isr_post(_id, 0, &holder, sizeof(holder), nullptr));
    }

    bool schedule(uint32_t delay, bool repeat, const std::function<void()> &callback) override {
        for (auto &timer: _timers) {
            if (timer.isActive()) {
                continue;
            }

            timer.attach(delay, repeat, callback);
            return true;
        }

        return false;
    }

    ~EventBus() {
        ESP_ERROR_CHECK(esp_event_loop_delete_default());
    }
};

inline static bool schedule(EventProducer &pub, uint32_t delay, bool repeat, const std::function<void()> &callback) {
    return pub.schedule(delay, repeat, callback);
}

inline static bool schedule(EventProducer *pub, uint32_t delay, bool repeat, const std::function<void()> &callback) {
    return schedule(*pub, delay, repeat, callback);
}

inline static void sendMessage(EventProducer &pub, Event::Ptr &msg) {
    pub.post(std::move(msg));
}

inline static void sendMessage(EventProducer *pub, Event::Ptr &msg) {
    sendMessage(*pub, msg);
}
