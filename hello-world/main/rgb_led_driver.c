#include "stdlib.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rgb_led_interface.h"

//pin define for rgb led
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLOCK 18

//The 512 bits data packet for the led strip
static uint8_t packet[20] = {
    0x00, 0x00, 0x00, 0x00,     // This line tells the LEDs to get ready to change colour
    0xF9, 0x00, 0x00, 0x00,     // This is the RGB colour of the first LED
    0xF9, 0x00, 0x00, 0x00,     // This is the color of the second LED    
    0xF9, 0x00, 0x00, 0x00,     // This is the RGB colour of the third LED
    0xFF, 0xFF, 0xFF, 0xFF      // This ends the transmission
}

// packet passed to the led 
void writePacketToLeds(){
    uint8_t bit;
    for(int i = 0; i<16; i++){
        /* Bit shifting one place to the left after each iteration, this will eventually move the 
        comparator bit out of view and exit the loop after every bit of the byte is written*/
        for(bit = 0x80; bit; bit >>= 1){
            //Shift the MOSI line
            gpio_set_level(PIN_NUM_MOSI, (packet[i] & bit) ? 1:0);

            vTaskDelay(SPI_CLOCK_LOW_TIME);
            gpio_set_level(PIN_NUM_CLOCK, 1);

            vTaskDelay(SPI_CLOCK_HIGH_TIME);
            gpio_set_level(PIN_NUM_CLOCK,0);
        }
    }
}

//Init RGB LED
void initialiseRgbLeds(){
    gpio_config_t ioConfig;
    ioConfig.intr_type = GPIO_PIN_INTR_DISABLE;
    ioConfig.mode = GPIO_MODE_INPUT_OUTPUT;
    ioConfig.pin_bit_mask = SPI_OUTPUT_PIN_SELECTION;
    ioConfig.pull_down_en = 0;
    ioConfig.pull_up_en = 0;
    gpio_config(&ioConfig);
    writePacketToLeds();
}

//First led color: This is defined in the packet array defined above
void setFirstColour(uint8_t colour[3]){
    packet[5] = packet[0];
    packet[6] = packet[1];
    packet[7] = packet[2];
    writePacketToLeds();
}
//second led colour
void setSecondColour(uint8_t colour[3]){
    packet[9] = packet[0];
    packet[10] = packet[1];
    packet[11] = packet[2];
    writePacketToLeds();
}
//third led colour
void setThirdColour(uint8_t colour[3]){
    packet[13] = packet[0];
    packet[14] = packet[1];
    packet[15] = packet[2];
    writePacketToLeds();
}