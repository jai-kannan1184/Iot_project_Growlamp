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

    i2c_master_write_byte(handle,
    SI_7021_MEASURE_HUMIDITY, I2C_MASTER_ACK);

    i2c_master_stop(handle);
        esp_err_t error = i2c_master_cmd_begin(I2C_NUM_0, handle, SI_7021_TIMEOUT);
    i2c_cmd_link_delete(handle);

    if(error != ESP_OK){
        ESP_LOGI(TAG, "Failed to write humidity command: %s", esp_err_to_name(error));
    }
    uint8_t humMSB;
    uint8_t humLSB;
    handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    //bus read write
    i2c_master_write_byte(handle, 
    SI_7021_ADDRESS << 1 | I2C_MASTER_READ, I2C_MASTER_ACK);
    sleep(1);
    i2c_master_read_byte(handle, &humMSB, I2C_MASTER_ACK);
    i2c_master_read_byte(handle, &humLSB, I2C_MASTER_LAST_NACK);

    i2c_master_stop(handle);
    error = i2c_master_cmd_begin(I2C_NUM_0, handle, SI_7021_TIMEOUT);
    i2c_cmd_link_delete(handle);
    if(error != ESP_OK){
        ESP_LOGI(TAG,"Failer to read humidity: %s", esp_err_to_name(error));
    }
    //Humidity calculation
    double humidity = ((uint16_t)humMSB << 8) | (uint16_t)humLSB;
    humidity *= 125;
    humidity /= 65536;
    humidity -= 6;
    ESP_LOGI(TAG,"Humidity is: %.2f", humidity);
    return humidity;
}