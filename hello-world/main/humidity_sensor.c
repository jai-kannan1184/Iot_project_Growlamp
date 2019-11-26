#include "stdlib.h"
#include "esp_log.h"
#include "driver/i2c.h"
#include "unistd.h"

//defines for bus
#define TAG "i2c_bus"

//defines 
#define ESP_INTR_FLAG_DEFAULT 0
// Every click of button 21 will initiate a humidity reading
#define statemachine_TASK_PRIORITY (tskIDLE_PRIORITY  + 1)
//SI7021 chip temperature read commands and timeout
#define SI_7021_ADDRESS 0x40
#define SI_7021_MEASURE_HUMIDITY 0xE5
#define SI_7021_TIMEOUT 1000/portTICK_RATE_MS

double humiditySensor(){
    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);

    i2c_master_write_byte(handle, 
    SI_7021_ADDRESS << 1 | I2C_MASTER_WRITE, I2C_MASTER_ACK);

    
}