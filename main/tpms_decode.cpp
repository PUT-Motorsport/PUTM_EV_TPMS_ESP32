/*
 * tpms_decode.cpp
 * Format: docs/ble_format_b.md (byte numbers here include the AC 00 prefix)
 */
#include "tpms_decode.hpp"

bool tpms_decode(const uint8_t* b, size_t len, TpmsMeasurement& out) {
    if (len != TPMS_MFG_LEN || b[0] != 0xAC || b[1] != 0x00)
        return false;

    // Pressure: 3.144 kPa per step
    uint32_t raw = b[3] + 256u * b[8];
    out.pressure_kpa_x10 = static_cast<uint16_t>((raw * 3144u + 50u) / 100u); // rounded

    out.temp_c      = static_cast<int16_t>(b[4]) - 55;
    out.status      = b[5];
    out.battery_raw = b[2];
    return true;
}
