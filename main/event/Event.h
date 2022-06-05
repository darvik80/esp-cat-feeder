//
// Created by Ivan Kishchenko on 07.11.2021.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

typedef uint_least8_t EventId;

struct Event {
public:
    typedef std::unique_ptr<Event> Ptr;

    [[nodiscard]] virtual EventId getEventId() const = 0;

    virtual ~Event() = default;
};

template<EventId id>
struct TEvent : Event {
public:
    enum {
        ID = id
    };

    [[nodiscard]] EventId getEventId() const override {
        return ID;
    }
};
