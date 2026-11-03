/*
 * tpms_types.hpp
 * Data shared by ble_scan, tpms_decode, sensor_list and uart_comm.
 * Plain fields only: this struct is copied through a FreeRTOS queue.
 */
#ifndef MAIN_TPMS_TYPES_HPP_
#define MAIN_TPMS_TYPES_HPP_

#include <cstdint>

enum class Wheel : uint8_t { FL, FR, RL, RR, spare1, spare2, unknown };

struct TpmsMeasurement {
    Wheel    wheel;             // FL FR RL RR spare1 spare2
    uint16_t pressure_kpa_x10;  // gauge pressure in 0.1 kPa (1038 = 103.8 kPa)
    int16_t  temp_c;            // degC
    uint8_t  status;            // bit 0 = leak
    uint8_t  battery_raw;       
    int8_t   rssi;              // dBm, negative
};

#endif /* MAIN_TPMS_TYPES_HPP_ */
