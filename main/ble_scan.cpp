/*
 * ble_scan.cpp
 *
 *  Created on: 3 lis 2026
 *      Author: User
 */
 
 
 #include "ble_scan.hpp"
 #include "esp_log.h"
 #include "tpms_decode.hpp"
 #include "sensor_list.hpp"
#include <cstdint>
#include <sys/types.h>

 static const char* TAG = "ble_scan";
 

 void BleScanner::begin(QueueHandle_t queue) {
     queue_ = queue;
     NimBLEDevice::init("");
     NimBLEScan* scan = NimBLEDevice::getScan();
     scan->setScanCallbacks(this, true); // true = report every broadcast, not only the first
     scan->setActiveScan(false);         // passive: just listen
     scan->setInterval(100);             // ms
     scan->setWindow(100);               // = interval, so it listens 100 % of the time
     scan->setMaxResults(0);             // don't store results, or RAM fills up
     scan->start(0, false);              // 0 = scan forever
 }
 
 bool isRepeat(const uint8_t* b, Wheel wheel) {
     static uint8_t  s_last[7][TPMS_MFG_LEN];
     static uint32_t s_last_ms[7];

     const uint8_t  w   = static_cast<uint8_t>(wheel);
     const uint32_t now = esp_timer_get_time() / 1000;   // ms since boot

     if (memcmp(s_last[w], b, TPMS_MFG_LEN) == 0 && (now - s_last_ms[w]) < 1000)
         return true;

     memcpy(s_last[w], b, TPMS_MFG_LEN);
     s_last_ms[w] = now;
     return false;
 }

 void BleScanner::onResult(const NimBLEAdvertisedDevice* dev) {
     if (!dev->haveManufacturerData()) return;
     std::string mfg = dev->getManufacturerData();
     if (mfg.size() != TPMS_MFG_LEN) return;
     const uint8_t* b = reinterpret_cast<const uint8_t*>(mfg.data());

     TpmsMeasurement m{};
     if (!tpms_decode(b, mfg.size(), m)) return;   // not a TPMS frame

     m.wheel = sensor_find(&b[TPMS_ID_OFFSET]);
     if (m.wheel == Wheel::unknown) {
         ESP_LOGD(TAG, "foreign sensor %02X%02X%02X ignored", b[16], b[15], b[14]);
         return;
     }
     m.rssi = static_cast<int8_t>(dev->getRSSI());
	 
	 if (isRepeat(b,  m.wheel)) return;

     // Debug 
     ESP_LOGI(TAG, "%-6s %3u.%u kPa (%.2f bar)  %3d C  status %02X%s  batt_raw %02X  rssi %d",
              wheel_name(m.wheel),
              m.pressure_kpa_x10 / 10, m.pressure_kpa_x10 % 10,
              m.pressure_kpa_x10 / 1000.0,
              m.temp_c,
              m.status, (m.status & 0x01) ? " LEAK" : "",
              m.battery_raw, m.rssi);

     // Raw bytes
     ESP_LOG_BUFFER_HEX_LEVEL(TAG, b, mfg.size(), ESP_LOG_DEBUG);

     xQueueSend(queue_, &m, 0);
 }

 void BleScanner::onScanEnd(const NimBLEScanResults&, int reason) {
     ESP_LOGW(TAG, "scan ended (%d), restarting", reason);
     NimBLEDevice::getScan()->start(0, false);
 }




