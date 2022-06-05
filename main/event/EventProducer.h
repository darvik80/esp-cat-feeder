//
// Created by Ivan Kishchenko on 09.12.2021.
//

#pragma once

#include "Event.h"

#include <functional>

class EventProducer {
public:
    virtual void post(Event::Ptr &&msg) = 0;

    virtual void postISR(Event::Ptr &&msg) = 0;

    virtual bool schedule(uint32_t delay, bool repeat, const std::function<void()> &callback) = 0;
};
