    #include <stdlib.h>
    #include <esp_log.h>
    #include <freertos/FreeRTOS.h>
    #include <freertos/task.h>
    #include <freertos/queue.h>
    #include <driver/gpio.h>
    #define TAG "states.c"
    void firstState(void) {
           gpio_set_level(GPIO_NUM_33, 0);
           gpio_set_level(GPIO_NUM_32, 0);
           ESP_LOGI(TAG, "Off, Off");
    }

    void secondState(void) {
           gpio_set_level(GPIO_NUM_33, 1);
           gpio_set_level(GPIO_NUM_32, 0);
           ESP_LOGI(TAG, "On, Off");
    }

    void thirdState(void) {
           gpio_set_level(GPIO_NUM_33, 0);
           gpio_set_level(GPIO_NUM_32, 1);
           ESP_LOGI(TAG, "Off, On");
    }

    void fourthState(void) {
           gpio_set_level(GPIO_NUM_33, 1);
           gpio_set_level(GPIO_NUM_32, 1);
           ESP_LOGI(TAG, "On, On");
    }