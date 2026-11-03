#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "ble_scan.hpp"

extern "C" void app_main(void)
{
	static BleScanner scanner;
	scanner.begin(nullptr);
    while (true) {
        printf("Hello from app_main!\n");
        sleep(1);
    }
}
