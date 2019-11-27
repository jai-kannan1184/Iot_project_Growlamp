#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "state_machine.h"
#include "esp_event_loop.h"
#include "states.h"
#include <driver/gpio.h>
#include<stdlib.h>
#include<driver/i2c.h>
#include "unistd.h"
#include "temperature_sensor.h"
#include "humidity_sensor.h"
#include "rgb_led_driver.h"

//defines

#define TAG "main.c"
#define ESP_INTR_FLAG_DEFAULT 0

static QueueHandle_t eventQueue;

//State machine

void stateMachineTask(void *pvParameters) {
   ESP_LOGI(TAG, "State machine started");
   createDeviceState();
   int event = 1;
   for (;;) {
       if (xQueueReceive(eventQueue, &event, portMAX_DELAY)) {
           ESP_LOGI(TAG, "Event received");
           if (event == 1) {
             triggerEvent();
             sleep(1);
             double cTemp, humidity;
             cTemp = tempSensor();
             humidity = humiditySensor(); 
             if(humidity < 45){
               setFirstColour(GREEN);
               if(humidity==45){
                 setFirstColour(YELLOW);
                }
             }
             else{
               setFirstColour(ORANGE);
             }
            if(cTemp <=25){
              setSecondColour(BLUE);
              if(cTemp >= 26 && cTemp <=27){
                setSecondColour(YELLOW);
              }
            } 
            else{
              setSecondColour(RED);
            }
           }
           if (event == 2) {
             //wifi
           }
       }
   }
   vTaskDelete(NULL);
  destroyDeviceState(); 
}

//Button 21 input for state machine

void IRAM_ATTR buttonHandler(void* arg) {
  int event = 1;
   xQueueSendFromISR(eventQueue, &event, NULL);
}

//wifi button

void IRAM_ATTR wifiHandler(void* arg) {
  int event = 2;
   xQueueSendFromISR(eventQueue, &event, NULL);
}

//Initialize hardware

void initialiseHardware(xQueueHandle* events) {
  ESP_LOGI(TAG, "Initialising hardware");
  eventQueue = *events;
  gpio_config_t ioConfig;
  ioConfig.intr_type = GPIO_PIN_INTR_DISABLE;
  ioConfig.pin_bit_mask = 1 << 26 | 1 << 27;
  ioConfig.mode = GPIO_MODE_INPUT_OUTPUT;
  ioConfig.pull_down_en = 0;
  ioConfig.pull_up_en = 0;
  gpio_config(&ioConfig);

  ioConfig.intr_type = GPIO_PIN_INTR_NEGEDGE;
  ioConfig.pin_bit_mask = 1 << 21;
  ioConfig.mode = GPIO_MODE_INPUT;
  ioConfig.pull_down_en = 1;
  ioConfig.pull_up_en = 0;
  gpio_config(&ioConfig);

   ioConfig.intr_type = GPIO_PIN_INTR_DISABLE;
   ioConfig.pin_bit_mask = (1ULL << 32)| (1ULL<<33);
   ioConfig.mode = GPIO_MODE_OUTPUT;
   ioConfig.pull_down_en = 0;
   ioConfig.pull_up_en = 0;
   gpio_config(&ioConfig);

  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  gpio_isr_handler_add(21, buttonHandler, (void*) 21);
  gpio_isr_handler_add(22, wifiHandler, (void*) 22);
}

//main

void app_main() {
  ESP_LOGI(TAG, "App main has started");
  eventQueue = xQueueCreate(10, sizeof(int));
  xTaskCreate(&stateMachineTask, "State Machine", 1024 * 5, NULL, 1, NULL);
  initialiseHardware(&eventQueue);
  initialiseRgbLeds();
  // Start config and initialise I2C bus
  i2c_config_t config;

  config.sda_io_num = 4;
  config.sda_pullup_en = GPIO_PULLUP_ENABLE;
  config.scl_io_num = 14;
  config.scl_pullup_en = GPIO_PULLUP_ENABLE;
  config.mode = I2C_MODE_MASTER;
  config.master.clk_speed = 100000;

  esp_err_t error = i2c_param_config(I2C_NUM_0, &config);
  if(error != ESP_OK){
    ESP_LOGI(TAG, "Failed to configure shared I2C bus: %s", esp_err_to_name(error));
    return;
  }
  error = i2c_set_timeout(I2C_NUM_0, 100000);
  if(error != ESP_OK){
    ESP_LOGI(TAG, "Failed to set timeout for I2C bus: %s", esp_err_to_name(error));
  }
  error = i2c_driver_install(I2C_NUM_0, config.mode, 512,512,0);
  if(error != ESP_OK){
    ESP_LOGI(TAG, "Failed to install driver for the I2C bus: %s", esp_err_to_name(error));
  }
  
}