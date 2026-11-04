#include <cstdint>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "ble_scan.hpp"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "portmacro.h"
#include "tpms_types.hpp"
#include "uart_comm.hpp"

extern "C" void app_main(void)
{
	
	QueueHandle_t dataQueue = xQueueCreate(10, sizeof(TpmsMeasurement));
		
		
	uartInit();
	
	static BleScanner scanner;
	scanner.begin(dataQueue);
	
	
    
	while (true) {
		
		TpmsMeasurement message;
        if(xQueueReceive(dataQueue, &message, portMAX_DELAY) == pdTRUE) {
			uartSend(message);
		}
		
		
    }
}
