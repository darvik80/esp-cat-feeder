//
// Created by Ivan Kishchenko on 04.06.2022.
//


#pragma once

#include <utility>
#include <string>
#include <esp_log.h>

namespace logging {
    const char *MAIN_TAG = "main";

    template<class... A>
    static void log(esp_log_level_t lvl, const char *tag, const char *str, A &&... args) {
        std::string fmt;
        switch (lvl) {
            case ESP_LOG_VERBOSE:
                fmt.append(LOG_COLOR_V +"V");
                break;
            case ESP_LOG_DEBUG:
                fmt.append(LOG_COLOR_D "D");
                break;
            case ESP_LOG_INFO:
                fmt.append(LOG_COLOR_I "I");
                break;
            case ESP_LOG_ERROR:
                fmt.append(LOG_COLOR_E "E");
                break;
            case ESP_LOG_WARN:
                fmt.append(LOG_COLOR_W "W");
                break;
            default:
                break;
        }
        fmt.append(" (%6u) [%8s]" LOG_RESET_COLOR ": ");
        fmt.append(str);
        fmt.append("\n");

        esp_log_write(lvl, tag, fmt.c_str(), esp_log_timestamp(), tag, std::forward<A>(args)...);
    }

    template<class... A>
    static void debug(const char *str, A &&... args) {
        log(ESP_LOG_DEBUG, MAIN_TAG, str, std::forward<A>(args)...);
    }

    template<class... A>
    static void info(const char *str, A &&... args) {
        log(ESP_LOG_INFO, MAIN_TAG, str, std::forward<A>(args)...);
    }

    template<class... A>
    static void warning(const char *str, A &&... args) {
        log(ESP_LOG_WARN, MAIN_TAG, str, std::forward<A>(args)...);
    }

    template<class... A>
    static void error(const char *str, A &&... args) {
        log(ESP_LOG_ERROR, MAIN_TAG, str, std::forward<A>(args)...);
    }
}

#define DECLARE_COMPONENT_LEVEL(COMPONENT, LEVEL, FNNAME)                   \
    template <class... A>                                                   \
    static void FNNAME(const char* fmt, A&&... args) {                      \
        logging::log(LEVEL, #COMPONENT, fmt, std::forward<A>(args)...);     \
    }

#define LOG_COMPONENT_SETUP(COMPONENT) namespace COMPONENT {        \
    namespace log {                                                 \
        DECLARE_COMPONENT_LEVEL(COMPONENT, ESP_LOG_DEBUG, debug)    \
        DECLARE_COMPONENT_LEVEL(COMPONENT, ESP_LOG_INFO, info)      \
        DECLARE_COMPONENT_LEVEL(COMPONENT, ESP_LOG_WARN, warning)   \
        DECLARE_COMPONENT_LEVEL(COMPONENT, ESP_LOG_ERROR, error)    \
    }                                                               \
}