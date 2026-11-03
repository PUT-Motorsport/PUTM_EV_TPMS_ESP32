/*
 * sensor_list.cpp
 */
#include "sensor_list.hpp"
#include <cstddef>
#include <iterator>

struct SensorMapping {
    uint8_t id[6];
    Wheel   wheel;
};

static const SensorMapping KNOWN_SENSORS[] = {
    { {0x11, 0x11, 0x11, 0x18, 0xB6, 0x5A}, Wheel::FL },     // 5AB618
    { {0x11, 0x11, 0x11, 0x6B, 0xCB, 0x5B}, Wheel::FR },     // 5BCB6B
    { {0x11, 0x11, 0x11, 0x69, 0x2C, 0x5B}, Wheel::RL },     // 5B2C69
    { {0x11, 0x11, 0x11, 0x62, 0xD2, 0x5B}, Wheel::RR },     // 5BD262
    { {0x11, 0x11, 0x11, 0x72, 0xA4, 0x5B}, Wheel::spare1 }, // 5BA472
    { {0x11, 0x11, 0x11, 0x29, 0x9F, 0x5A}, Wheel::spare2 }, // 5A9F29
};

Wheel sensor_find(const uint8_t id[6]) {
    for (size_t i = 0; i < std::size(KNOWN_SENSORS); i++) {
        bool is_same = true;
        for (size_t j = 0; j < 6; j++) {
            if (id[j] != KNOWN_SENSORS[i].id[j]) {
                is_same = false;
                break;
            }
        }
        if (is_same) return KNOWN_SENSORS[i].wheel;
    }
    return Wheel::unknown;
}

const char* wheel_name(Wheel w) {
    switch (w) {
    case Wheel::FL:     return "FL";
    case Wheel::FR:     return "FR";
    case Wheel::RL:     return "RL";
    case Wheel::RR:     return "RR";
    case Wheel::spare1: return "SPARE1";
    case Wheel::spare2: return "SPARE2";
    default:            return "UNKNOWN";
    }
}
