/*
 * sensor_list.hpp
 */
#ifndef MAIN_SENSOR_LIST_HPP_
#define MAIN_SENSOR_LIST_HPP_

#include <cstdint>
#include "tpms_types.hpp"

// id = the 6 ID bytes as received (b[11..16]). Returns Wheel::unknown for foreign sensors.
Wheel sensor_find(const uint8_t id[6]);

// "FL", "FR", ... for logs
const char* wheel_name(Wheel w);

#endif /* MAIN_SENSOR_LIST_HPP_ */
