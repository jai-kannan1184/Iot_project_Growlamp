#ifndef RGB_LED_INTERFACE
#define RGB_LED_INTERFACE

#include "freertos/FreeRTOS.h"

//SPI bus pins
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLOCK 18

#define SPI_OUTPUT_PIN_SELECTION ((1 << PIN_NUM_MOSI) | (1 << PIN_NUM_CLOCK))

#define SPI_CLOCK_LOW_TIME 1/ portTICK_PERIOD_MS / 10
#define SPI_CLOCK_HIGH_TIME 1 / portTICK_PERIOD_MS / 10

void initialiseRgbLeds(void);
void writePacketToLeds();
void setFirstColour(uint8_t colour[3]);
void setSecondColour(uint8_t colour[3]);
void setThirdColour(uint8_t colour[3]);


static uint8_t RED[3]	= {0xFF,0x00, 0x00};
static uint8_t GREEN[3]	= {0x00,0xFF,0x00};
static uint8_t BLUE[3] = {0x00, 0x00, 0xFF};
static uint8_t NAVY[3] = {0x00, 0x00, 0x8B};
static uint8_t WHITE[3] = {0xFF, 0xFF, 0xFF};
static uint8_t MAGENTA[3] = {0xFF, 0x00, 0xFF};
static uint8_t CYAN[3] = {0x00, 0xFF, 0xFF};
static uint8_t YELLOW[3] = {0xFF, 0xFF, 0x00};
static uint8_t OFF[3] = {0x00,   0x00, 0x00};
static uint8_t VIOLET[3] = {0xEE, 0x82, 0xEE};
static uint8_t INDIGO[3] = {0x4B, 0x00, 0x82};
static uint8_t ORANGE[3] ={0xFF, 0xA5, 0x00};
#endif