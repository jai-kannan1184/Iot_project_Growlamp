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
}