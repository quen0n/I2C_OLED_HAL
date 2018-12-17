#include "stm32f4xx_hal.h"

#define OVERLAP_OFF 0
#define OVERLAP_ON 1

uint8_t displayInit(I2C_HandleTypeDef *i2c, uint16_t address);
void displaySetBrightnes(uint8_t brightness);
void displayClear(void);
void displayDraw(const uint8_t bitmap[], uint8_t lenght, uint8_t width, uint8_t x, uint8_t y);
void displayPrintCharXY(unsigned char byte, uint8_t x, uint8_t y);
void displayPrintTextXY(char *text, uint8_t x, uint8_t y);
void displayPrintText(char *text);

static uint8_t __to_display_transmit(uint8_t buff[], uint8_t size);
static uint8_t __sendData(uint8_t byte);
static uint8_t __sendCommand(uint8_t cmd);
static uint8_t __sendCommandWithData(uint8_t cmd, uint8_t data);
static uint8_t __sendDataArray(uint8_t byte[], uint8_t size);
static void __setColumn(uint8_t x);
static void __setPage(uint8_t y);
static uint8_t __getCharLenght(unsigned char byte);
