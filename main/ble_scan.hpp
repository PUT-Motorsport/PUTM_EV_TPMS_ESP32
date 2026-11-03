/*
 * ble_scan.hpp
 *
 *  Created on: 3 lis 2026
 *      Author: User
 */

#ifndef MAIN_BLE_SCAN_HPP_
#define MAIN_BLE_SCAN_HPP_


#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <NimBLEDevice.h>
#include "tpms_types.hpp"

class BleScanner : public NimBLEScanCallbacks {
public:
    void begin(QueueHandle_t queue);   // init BLE and start scanning forever
private:
    void onResult(const NimBLEAdvertisedDevice* dev) override;
    void onScanEnd(const NimBLEScanResults& results, int reason) override;
    QueueHandle_t queue_ = nullptr;
};


#endif /* MAIN_BLE_SCAN_HPP_ */
