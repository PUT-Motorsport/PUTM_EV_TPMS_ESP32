/*
 * tpms_decode.hpp
 */
#ifndef MAIN_TPMS_DECODE_HPP_
#define MAIN_TPMS_DECODE_HPP_

#include <cstddef>
#include <cstdint>
#include "tpms_types.hpp"


constexpr size_t TPMS_MFG_LEN   = 17; //data frame length
constexpr size_t TPMS_ID_OFFSET = 11; // b[11..16] = sensor ID


bool tpms_decode(const uint8_t* b, size_t len, TpmsMeasurement& out);

#endif /* MAIN_TPMS_DECODE_HPP_ */
