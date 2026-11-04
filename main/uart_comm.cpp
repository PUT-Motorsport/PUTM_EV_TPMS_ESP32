/*
 * uart_comm.cpp
 *
 *  Created on: 3 lis 2026
 *      Author: User
 */

#include "driver/uart.h"
#include "hal/uart_types.h"
#include "soc/clk_tree_defs.h"
#include "tpms_types.hpp"
#include <cstdint>
#include "esp_err.h"
#include "uart_comm.hpp"





void uartInit(){
	ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, 256, 256, 0, nullptr, 0));

	const uart_port_t uart_num = UART_NUM_2;
	uart_config_t uart_config = {};          
	
	
	uart_config.baud_rate  = 115200;
	uart_config.data_bits  = UART_DATA_8_BITS;
	uart_config.parity     = UART_PARITY_DISABLE;
	uart_config.stop_bits  = UART_STOP_BITS_1;
	uart_config.flow_ctrl  = UART_HW_FLOWCTRL_DISABLE;
	uart_config.source_clk = UART_SCLK_DEFAULT;

	ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, 43, 44, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

	ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

}


static uint16_t crc16(const uint8_t* d, size_t n) {   // CRC-16/CCITT-FALSE
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < n; i++) {
        crc ^= d[i] << 8;
        for (int b = 0; b < 8; b++)
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

void uartSend(const TpmsMeasurement& m) {
    uint8_t f[14];
    f[0]  = 0xA5;
    f[1]  = 0x5A;
    f[2]  = 0x01;                                   
    f[3]  = 8;                                     
    f[4]  = static_cast<uint8_t>(m.wheel);
    f[5]  = m.pressure_kpa_x10 & 0xFF;              
    f[6]  = m.pressure_kpa_x10 >> 8;                
    f[7]  = static_cast<uint16_t>(m.temp_c) & 0xFF;
    f[8]  = static_cast<uint16_t>(m.temp_c) >> 8;
    f[9]  = m.status;
    f[10] = m.battery_raw;
    f[11] = static_cast<uint8_t>(m.rssi);
    uint16_t crc = crc16(&f[2], 10);                
    f[12] = crc & 0xFF;
    f[13] = crc >> 8;
    uart_write_bytes(UART_NUM_2, f, sizeof(f));
	
	ESP_LOGI("tpms_uart", "UART sent %d bytes:", sizeof(f));
	ESP_LOG_BUFFER_HEX("tpms_uart", f, sizeof(f));
}

