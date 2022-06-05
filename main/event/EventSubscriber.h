//
// Created by Ivan Kishchenko on 07.11.2021.
//

#pragma once

#include "Event.h"
#include "EventLogger.h"

class EventSubscriber {
public:
    virtual void onEvent(const Event &event) = 0;
};

template<typename T, typename Event1 = void, typename Event2 = void, typename Event3 = void, typename Event4 = void>
class TEventSubscriber : public EventSubscriber {
public:
    void onEvent(const Event &event) override {
        switch (event.getEventId()) {
            case Event1::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event1 &>(event));
                break;
            case Event2::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event2 &>(event));
                break;
            case Event3::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event3 &>(event));
                break;
            case Event4::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event4 &>(event));
                break;
            default:
                break;
        }
    }
};

template<typename T, typename Event1, typename Event2, typename Event3>
class TEventSubscriber<T, Event1, Event2, Event3, void> : public EventSubscriber {
public:
    void onEvent(const Event &event) override {
        switch (event.getEventId()) {
            case Event1::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event1 &>(event));
                break;
            case Event2::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event2 &>(event));
                break;
            case Event3::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event3 &>(event));
                break;
            default:
                break;
        }
    }
};

template<typename T, typename Event1, typename Event2>
class TEventSubscriber<T, Event1, Event2, void, void> : public EventSubscriber {
public:
    void onEvent(const Event &event) override {
        switch (event.getEventId()) {
            case Event1::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event1 &>(event));
                break;
            case Event2::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Event2 &>(event));
                break;
            default:
                break;
        }
    }
};

template<typename T, typename Msg1>
class TEventSubscriber<T, Msg1, void, void, void> : public EventSubscriber {
public:
    void onEvent(const Event &event) override {
        switch (event.getEventId()) {
            case Msg1::ID:
                static_cast<T *>(this)->handleEvent(static_cast<const Msg1 &>(event));
                break;
            default:
                break;
        }
    }
};
