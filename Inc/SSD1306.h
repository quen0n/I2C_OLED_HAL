#include "stm32f4xx_hal.h"

#define ON 1
#define OFF 0

uint8_t OLED_init(I2C_HandleTypeDef *i2c, uint16_t address);
void OLED_setBrightness(uint8_t brightness);
void OLED_clear(void);
void OLED_fill(void);
void OLED_display(uint8_t state);
void OLED_inversion(uint8_t state);

static uint8_t __sendCommandWithData(uint8_t cmd, uint8_t data);
static uint8_t __sendCommand(uint8_t cmd);
static uint8_t __sendDataArray(uint8_t byte[], uint16_t size);
static uint8_t __sendData(uint8_t byte);
static uint8_t __transmitToDisplay(uint8_t buff[], uint16_t size);

static void __displayUpdate(void);



static uint8_t __getCharLenght(unsigned char byte);
void displayDraw(const uint8_t bitmap[], uint8_t lenght, uint8_t width, uint8_t x, uint8_t y);
void displayPrintCharXY(unsigned char byte, uint8_t x, uint8_t y);
void displayPrintTextXY(char *text, uint8_t x, uint8_t y);
void displayPrintText(char *text);


