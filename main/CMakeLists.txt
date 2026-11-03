# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS "main.cpp"         
   		 "ble_scan.cpp"
         "tpms_decode.cpp"
         "sensor_list.cpp"
         "uart_comm.cpp"
    INCLUDE_DIRS       "."
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES         esp_driver_uart esp_timer  
    PRIV_REQUIRES       # optional, list the private requirements
)
