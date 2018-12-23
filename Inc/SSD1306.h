#include "stm32f4xx_hal.h"

#define DISPLAY_LENGHT 128
#define DISPLAY_WIDTH 64

#define ON 1
#define OFF 0

uint8_t OLED_init(I2C_HandleTypeDef *i2c, uint16_t address);
void OLED_setBrightness(uint8_t brightness);
void OLED_clear(void);
void OLED_fill(void);
void OLED_display(uint8_t state);
void OLED_inversion(uint8_t state);
void OLED_goto(uint8_t x, uint8_t y);
void OLED_draw(const uint8_t bitmap[], uint8_t length, uint8_t width, uint8_t inversion, uint8_t autoinversion, uint8_t transparent);
void OLED_update(void);
void OLED_printChar(unsigned char byte, uint8_t inversion, uint8_t autoinversion, uint8_t transparent);
void OLED_print(char *text, uint8_t inversion, uint8_t autoinversion, uint8_t transparent);

static uint8_t __sendCommandWithData(uint8_t cmd, uint8_t data);
static uint8_t __sendCommand(uint8_t cmd);
static uint8_t __sendDataArray(uint8_t byte[], uint16_t size);
static uint8_t __sendData(uint8_t byte);
static uint8_t __transmitToDisplay(uint8_t buff[], uint16_t size);
static void __displayUpdate(void);
