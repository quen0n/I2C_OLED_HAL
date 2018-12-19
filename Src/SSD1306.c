#include "SSD1306.h"
#include <cstring>
I2C_HandleTypeDef *_i2c;
uint16_t _address;
uint8_t displayBuffer[8][128]; //8 страниц по 128 столбцов

const uint8_t VerdanaLatin10x12[96][21] = {
	{0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char  
	{0x03, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char !
	{0x05, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char "
	{0x09, 0x00, 0x00, 0x80, 0x00, 0x90, 0x03, 0xF8, 0x00, 0x96, 0x00, 0x90, 0x03, 0xF8, 0x00, 0x96, 0x00, 0x10, 0x00, 0x00, 0x00},  // Code for char #
	{0x07, 0x00, 0x00, 0x18, 0x02, 0x24, 0x02, 0xFF, 0x0F, 0x44, 0x02, 0x44, 0x02, 0x84, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char $
	{0x08, 0x00, 0x00, 0x0C, 0x02, 0x92, 0x01, 0x4C, 0x00, 0x20, 0x00, 0x90, 0x01, 0x4C, 0x02, 0x82, 0x01, 0x00, 0x00, 0x00, 0x00},  // Code for char %
	{0x09, 0x00, 0x00, 0xDC, 0x01, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x5C, 0x02, 0x80, 0x01, 0x60, 0x01, 0x00, 0x02, 0x00, 0x00},  // Code for char &
	{0x02, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char '
	{0x04, 0x00, 0x00, 0xF8, 0x01, 0x06, 0x06, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char (
	{0x04, 0x00, 0x00, 0x01, 0x08, 0x06, 0x06, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char )
	{0x06, 0x00, 0x00, 0x0A, 0x00, 0x04, 0x00, 0x1F, 0x00, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char *
	{0x08, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0xF8, 0x03, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char +
	{0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char ,
	{0x05, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char -
	{0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char .
	{0x05, 0x00, 0x08, 0x00, 0x07, 0xE0, 0x00, 0x1C, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char /
	{0x07, 0x00, 0x00, 0xFC, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 0
	{0x07, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x04, 0x02, 0xFE, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 1
	{0x07, 0x00, 0x00, 0x0C, 0x03, 0x82, 0x02, 0x42, 0x02, 0x42, 0x02, 0x22, 0x02, 0x1C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 2
	{0x07, 0x00, 0x00, 0x04, 0x01, 0x02, 0x02, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0xDC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 3
	{0x08, 0x00, 0x00, 0x60, 0x00, 0x50, 0x00, 0x48, 0x00, 0x44, 0x00, 0xFE, 0x03, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 4
	{0x07, 0x00, 0x00, 0x1E, 0x01, 0x12, 0x02, 0x12, 0x02, 0x12, 0x02, 0x12, 0x02, 0xE2, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 5
	{0x07, 0x00, 0x00, 0xF8, 0x01, 0x14, 0x02, 0x12, 0x02, 0x12, 0x02, 0x12, 0x02, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 6
	{0x07, 0x00, 0x00, 0x02, 0x00, 0x02, 0x02, 0x82, 0x01, 0x62, 0x00, 0x1A, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 7
	{0x07, 0x00, 0x00, 0xDC, 0x01, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0xDC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 8
	{0x07, 0x00, 0x00, 0x3C, 0x00, 0x42, 0x02, 0x42, 0x02, 0x42, 0x02, 0x42, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 9
	{0x03, 0x00, 0x00, 0x00, 0x00, 0x18, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char :
	{0x03, 0x00, 0x00, 0x00, 0x08, 0x18, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char ;
	{0x08, 0x00, 0x00, 0x40, 0x00, 0xA0, 0x00, 0xA0, 0x00, 0x10, 0x01, 0x10, 0x01, 0x08, 0x02, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00},  // Code for char <
	{0x08, 0x00, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char =
	{0x08, 0x00, 0x00, 0x08, 0x02, 0x08, 0x02, 0x10, 0x01, 0x10, 0x01, 0xA0, 0x00, 0xA0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char >
	{0x06, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0xC2, 0x02, 0x22, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char ?
	{0x0A, 0x00, 0x00, 0xF0, 0x00, 0x0C, 0x03, 0x04, 0x02, 0xF2, 0x04, 0x0A, 0x05, 0x0A, 0x05, 0xFA, 0x04, 0x04, 0x01, 0xF8, 0x00},  // Code for char @
	{0x08, 0x00, 0x03, 0xE0, 0x00, 0x98, 0x00, 0x86, 0x00, 0x86, 0x00, 0x98, 0x00, 0xE0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00},  // Code for char A
	{0x07, 0x00, 0x00, 0xFE, 0x03, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x3C, 0x02, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char B
	{0x08, 0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x01, 0x00, 0x00, 0x00, 0x00},  // Code for char C
	{0x08, 0x00, 0x00, 0xFE, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x01, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char D
	{0x07, 0x00, 0x00, 0xFE, 0x03, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char E
	{0x07, 0x00, 0x00, 0xFE, 0x03, 0x22, 0x00, 0x22, 0x00, 0x22, 0x00, 0x22, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char F
	{0x08, 0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x22, 0x02, 0x22, 0x02, 0xE4, 0x01, 0x00, 0x00, 0x00, 0x00},  // Code for char G
	{0x08, 0x00, 0x00, 0xFE, 0x03, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00},  // Code for char H
	{0x04, 0x00, 0x00, 0x02, 0x02, 0xFE, 0x03, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char I
	{0x04, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char J
	{0x07, 0x00, 0x00, 0xFE, 0x03, 0x20, 0x00, 0x50, 0x00, 0x88, 0x00, 0x04, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char K
	{0x07, 0x00, 0x00, 0xFE, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char L
	{0x09, 0x00, 0x00, 0xFE, 0x03, 0x06, 0x00, 0x38, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x38, 0x00, 0x06, 0x00, 0xFE, 0x03, 0x00, 0x00},  // Code for char M
	{0x08, 0x00, 0x00, 0xFE, 0x03, 0x06, 0x00, 0x18, 0x00, 0x20, 0x00, 0xC0, 0x00, 0x00, 0x03, 0xFE, 0x03, 0x00, 0x00, 0x00, 0x00},  // Code for char N
	{0x09, 0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0x01, 0xF8, 0x00, 0x00, 0x00},  // Code for char O
	{0x07, 0x00, 0x00, 0xFE, 0x03, 0x42, 0x00, 0x42, 0x00, 0x42, 0x00, 0x42, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char P
	{0x09, 0x00, 0x00, 0xF8, 0x00, 0x04, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x06, 0x04, 0x09, 0xF8, 0x08, 0x00, 0x00},  // Code for char Q
	{0x08, 0x00, 0x00, 0xFE, 0x03, 0x22, 0x00, 0x22, 0x00, 0x62, 0x00, 0xA2, 0x00, 0x1C, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00},  // Code for char R
	{0x07, 0x00, 0x00, 0x1C, 0x01, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0x22, 0x02, 0xC4, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char S
	{0x07, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0xFE, 0x03, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char T
	{0x08, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char U
	{0x08, 0x06, 0x00, 0x38, 0x00, 0xC0, 0x00, 0x00, 0x03, 0x00, 0x03, 0xC0, 0x00, 0x38, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char V
	{0x0A, 0x00, 0x00, 0x1E, 0x00, 0xE0, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x18, 0x00, 0xE0, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x1E, 0x00},  // Code for char W
	{0x07, 0x00, 0x00, 0x06, 0x03, 0x88, 0x00, 0x70, 0x00, 0x70, 0x00, 0x88, 0x00, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char X
	{0x07, 0x02, 0x00, 0x0C, 0x00, 0x10, 0x00, 0xE0, 0x03, 0x10, 0x00, 0x0C, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char Y
	{0x07, 0x00, 0x00, 0x02, 0x03, 0x82, 0x02, 0x62, 0x02, 0x12, 0x02, 0x0A, 0x02, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char Z
	{0x04, 0x00, 0x00, 0xFF, 0x0F, 0x01, 0x08, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char [
	{0x05, 0x03, 0x00, 0x1C, 0x00, 0x60, 0x00, 0x80, 0x03, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char BackSlash
	{0x04, 0x00, 0x00, 0x01, 0x08, 0x01, 0x08, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char ]
	{0x09, 0x00, 0x00, 0x10, 0x00, 0x08, 0x00, 0x04, 0x00, 0x02, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00, 0x00, 0x00},  // Code for char ^
	{0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00},  // Code for char _
	{0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char `
	{0x07, 0x00, 0x00, 0x80, 0x01, 0x48, 0x02, 0x48, 0x02, 0x48, 0x02, 0x48, 0x02, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char a
	{0x07, 0x00, 0x00, 0xFF, 0x03, 0x10, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char b
	{0x06, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char c
	{0x07, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x01, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char d
	{0x07, 0x00, 0x00, 0xF0, 0x01, 0x48, 0x02, 0x48, 0x02, 0x48, 0x02, 0x48, 0x02, 0x70, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char e
	{0x04, 0x08, 0x00, 0xFE, 0x03, 0x09, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char f
	{0x07, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x0A, 0x08, 0x0A, 0x08, 0x0A, 0x08, 0x09, 0xF8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char g
	{0x07, 0x00, 0x00, 0xFF, 0x03, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char h
	{0x02, 0x00, 0x00, 0xF9, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char i
	{0x03, 0x00, 0x08, 0x08, 0x08, 0xF9, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char j
	{0x06, 0x00, 0x00, 0xFF, 0x03, 0x40, 0x00, 0xA0, 0x00, 0x10, 0x01, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char k
	{0x02, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char l
	{0x0A, 0x00, 0x00, 0xF8, 0x03, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0xF0, 0x03, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0xF0, 0x03},  // Code for char m
	{0x07, 0x00, 0x00, 0xF8, 0x03, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char n
	{0x07, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char o
	{0x07, 0x00, 0x00, 0xF8, 0x0F, 0x10, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char p
	{0x07, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x01, 0xF8, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char q
	{0x05, 0x00, 0x00, 0xF8, 0x03, 0x10, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char r
	{0x06, 0x00, 0x00, 0x30, 0x02, 0x48, 0x02, 0x48, 0x02, 0x48, 0x02, 0x88, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char s
	{0x05, 0x08, 0x00, 0xFE, 0x01, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char t
	{0x07, 0x00, 0x00, 0xF8, 0x01, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x01, 0xF8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char u
	{0x06, 0x00, 0x00, 0x18, 0x00, 0xE0, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char v
	{0x0A, 0x00, 0x00, 0x18, 0x00, 0xE0, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x18, 0x00, 0xE0, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x18, 0x00},  // Code for char w
	{0x06, 0x00, 0x00, 0x18, 0x03, 0xA0, 0x00, 0x40, 0x00, 0xA0, 0x00, 0x18, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char x
	{0x06, 0x00, 0x00, 0x18, 0x00, 0xE0, 0x08, 0x00, 0x07, 0xE0, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char y
	{0x06, 0x00, 0x00, 0x08, 0x03, 0x88, 0x02, 0x48, 0x02, 0x28, 0x02, 0x18, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char z
	{0x06, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0xDE, 0x07, 0x01, 0x08, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char {
	{0x03, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char |
	{0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x01, 0x08, 0xDE, 0x07, 0x20, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char }
	{0x09, 0x00, 0x00, 0xC0, 0x00, 0x20, 0x00, 0x20, 0x00, 0x40, 0x00, 0x40, 0x00, 0x80, 0x00, 0x80, 0x00, 0x60, 0x00, 0x00, 0x00},  // Code for char ~
	{0x02, 0xFC, 0x01, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   // Code for char 
	};
const uint8_t VerdanaCyrillic10x12[64][21] = {
	{0x08, 0x00, 0x06, 0xC0, 0x01, 0x30, 0x01, 0x0C, 0x01, 0x0C, 0x01, 0x30, 0x01, 0xC0, 0x01, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00},  // Code for char А
	{0x07, 0x00, 0x00, 0xFC, 0x07, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char Б
	{0x07, 0x00, 0x00, 0xFC, 0x07, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x78, 0x04, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char В
	{0x07, 0x00, 0x00, 0xFC, 0x07, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char Г
	{0x08, 0x00, 0x0E, 0x80, 0x03, 0x7C, 0x02, 0x04, 0x02, 0x04, 0x02, 0x04, 0x02, 0xFC, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00},  // Code for char Д
	{0x07, 0x00, 0x00, 0xFC, 0x07, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char Е
	{0x0A, 0x00, 0x00, 0x04, 0x04, 0x04, 0x02, 0xB8, 0x01, 0x40, 0x00, 0xFC, 0x07, 0x40, 0x00, 0xB8, 0x01, 0x04, 0x02, 0x04, 0x04},  // Code for char 
	{0x06, 0x08, 0x02, 0x04, 0x04, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0xB8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 
	{0x08, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x02, 0x80, 0x01, 0x40, 0x00, 0x30, 0x00, 0x08, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char 
	{0x08, 0x00, 0x00, 0xFC, 0x07, 0x01, 0x02, 0x82, 0x01, 0x42, 0x00, 0x32, 0x00, 0x09, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char 
	{0x07, 0x00, 0x00, 0xFC, 0x07, 0x40, 0x00, 0xC0, 0x00, 0x38, 0x01, 0x04, 0x02, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char Л
	{0x08, 0x00, 0x04, 0x00, 0x04, 0xFC, 0x03, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char 
	{0x09, 0x00, 0x00, 0xFC, 0x07, 0x0C, 0x00, 0x70, 0x00, 0x80, 0x01, 0x80, 0x01, 0x70, 0x00, 0x0C, 0x00, 0xFC, 0x07, 0x00, 0x00},  // Code for char 
	{0x08, 0x00, 0x00, 0xFC, 0x07, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char 
	{0x09, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x02, 0xF0, 0x01, 0x00, 0x00},  // Code for char 
	{0x08, 0x00, 0x00, 0xFC, 0x07, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char 
	{0x07, 0x00, 0x00, 0xFC, 0x07, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x84, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char  
	{0x08, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x02, 0x00, 0x00, 0x00, 0x00},  // Code for char !
	{0x07, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0xFC, 0x07, 0x04, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char "
	{0x07, 0x00, 0x00, 0x0C, 0x04, 0x30, 0x04, 0xC0, 0x03, 0xC0, 0x00, 0x30, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char #
	{0x0A, 0x00, 0x00, 0xE0, 0x00, 0x10, 0x01, 0x08, 0x02, 0x08, 0x02, 0xFC, 0x07, 0x08, 0x02, 0x08, 0x02, 0x10, 0x01, 0xE0, 0x00},  // Code for char $
	{0x07, 0x00, 0x00, 0x0C, 0x06, 0x10, 0x01, 0xE0, 0x00, 0xE0, 0x00, 0x10, 0x01, 0x0C, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char %
	{0x08, 0x00, 0x00, 0xFC, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0xFC, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00},  // Code for char &
	{0x08, 0x00, 0x00, 0x7C, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char '
	{0x08, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x04, 0x00, 0x04, 0xFC, 0x07, 0x00, 0x04, 0x00, 0x04, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char (
	{0x0A, 0x00, 0x00, 0xFC, 0x07, 0x00, 0x04, 0x00, 0x04, 0xFC, 0x07, 0x00, 0x04, 0x00, 0x04, 0xFC, 0x07, 0x00, 0x04, 0x00, 0x0C},  // Code for char )
	{0x08, 0x04, 0x00, 0x04, 0x00, 0xFC, 0x07, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00},  // Code for char *
	{0x0A, 0x00, 0x00, 0xFC, 0x07, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x07},  // Code for char +
	{0x07, 0x00, 0x00, 0xFC, 0x07, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x04, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char ,
	{0x08, 0x00, 0x00, 0x08, 0x02, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x44, 0x04, 0x48, 0x02, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00},  // Code for char -
	{0x0A, 0x00, 0x00, 0xFC, 0x07, 0x40, 0x00, 0x40, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x04, 0x04, 0x04, 0x04, 0x08, 0x02, 0xF0, 0x01},  // Code for char .
	{0x07, 0x00, 0x04, 0x38, 0x02, 0x44, 0x01, 0xC4, 0x00, 0x44, 0x00, 0x44, 0x00, 0xFC, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char /
	{0x07, 0x00, 0x00, 0x00, 0x03, 0x90, 0x04, 0x90, 0x04, 0x90, 0x04, 0x90, 0x04, 0xE0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 0
	{0x07, 0x00, 0x00, 0xF8, 0x03, 0x14, 0x04, 0x12, 0x04, 0x12, 0x04, 0x12, 0x04, 0xE2, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 1
	{0x06, 0x00, 0x00, 0xF0, 0x07, 0x90, 0x04, 0x90, 0x04, 0x90, 0x04, 0x60, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 2
	{0x06, 0x00, 0x00, 0xF0, 0x07, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 3
	{0x07, 0x00, 0x0E, 0x80, 0x03, 0x70, 0x02, 0x10, 0x02, 0x10, 0x02, 0xF0, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 4
	{0x07, 0x00, 0x00, 0xE0, 0x03, 0x90, 0x04, 0x90, 0x04, 0x90, 0x04, 0x90, 0x04, 0xE0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 5
	{0x0A, 0x00, 0x00, 0x10, 0x04, 0x10, 0x02, 0x60, 0x01, 0x80, 0x00, 0xF0, 0x07, 0x80, 0x00, 0x60, 0x01, 0x10, 0x02, 0x10, 0x04},  // Code for char 6
	{0x05, 0x00, 0x00, 0x10, 0x04, 0x90, 0x04, 0x90, 0x04, 0x60, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 7
	{0x07, 0x00, 0x00, 0xF0, 0x07, 0x00, 0x01, 0x80, 0x00, 0x40, 0x00, 0x20, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 8
	{0x07, 0x00, 0x00, 0xF0, 0x07, 0x02, 0x01, 0x84, 0x00, 0x44, 0x00, 0x22, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char 9
	{0x06, 0x00, 0x00, 0xF0, 0x07, 0x80, 0x00, 0x60, 0x01, 0x10, 0x02, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char :
	{0x06, 0x00, 0x04, 0xF0, 0x03, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char ;
	{0x07, 0x00, 0x00, 0xF0, 0x07, 0x20, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x20, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char <
	{0x07, 0x00, 0x00, 0xF0, 0x07, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char =
	{0x07, 0x00, 0x00, 0xE0, 0x03, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char >
	{0x07, 0x00, 0x00, 0xF0, 0x07, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char ?
	{0x07, 0x00, 0x00, 0xF8, 0x0F, 0x10, 0x02, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char @
	{0x06, 0x00, 0x00, 0xE0, 0x03, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char A
	{0x06, 0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0xF0, 0x07, 0x10, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char B
	{0x06, 0x00, 0x00, 0x18, 0x00, 0xE0, 0x08, 0x00, 0x07, 0xE0, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char C
	{0x0A, 0x00, 0x00, 0xF0, 0x01, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0xFE, 0x0F, 0x08, 0x02, 0x08, 0x02, 0x08, 0x02, 0xF0, 0x01},  // Code for char D
	{0x06, 0x00, 0x00, 0x30, 0x06, 0x40, 0x01, 0x80, 0x00, 0x40, 0x01, 0x30, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char E
	{0x08, 0x00, 0x00, 0xF0, 0x03, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0x00, 0x02, 0xF0, 0x03, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00},  // Code for char F
	{0x07, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char G
	{0x08, 0x00, 0x00, 0xF0, 0x07, 0x00, 0x04, 0x00, 0x04, 0xF0, 0x07, 0x00, 0x04, 0x00, 0x04, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char H
	{0x09, 0x00, 0x00, 0xF0, 0x07, 0x00, 0x04, 0x00, 0x04, 0xF0, 0x07, 0x00, 0x04, 0x00, 0x04, 0xF0, 0x07, 0x00, 0x0C, 0x00, 0x00},  // Code for char I
	{0x07, 0x10, 0x00, 0x10, 0x00, 0xF0, 0x07, 0x80, 0x04, 0x80, 0x04, 0x80, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char J
	{0x08, 0x00, 0x00, 0xF0, 0x07, 0x80, 0x04, 0x80, 0x04, 0x80, 0x04, 0x00, 0x03, 0x00, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00},  // Code for char K
	{0x06, 0x00, 0x00, 0xF0, 0x07, 0x80, 0x04, 0x80, 0x04, 0x80, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char L
	{0x07, 0x00, 0x00, 0x20, 0x02, 0x10, 0x04, 0x90, 0x04, 0x90, 0x04, 0x90, 0x04, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // Code for char M
	{0x0A, 0x00, 0x00, 0xF0, 0x07, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x03, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0x10, 0x04, 0xE0, 0x03},  // Code for char N
	{0x06, 0x00, 0x04, 0x60, 0x02, 0x90, 0x01, 0x90, 0x00, 0x90, 0x00, 0xF0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}   // Code for char O
	};

				


/* Пользовательские функции */		

//Функция инициализация дисплея 	
uint8_t OLED_init(I2C_HandleTypeDef *i2c, uint16_t address) {
	_i2c = i2c;
	_address = address;
	
	//Получаем состояние устройства на шине
	uint8_t STATUS = HAL_I2C_IsDeviceReady(_i2c, _address, 5, 0xFF);
	//Если устройство не готово - возвращаемся с ошибкой
	if (STATUS != HAL_OK) {
		return STATUS;
	}
	//1. Set Display Off
	//__sendCommand(0xAE);
	//2. Set Osc Frequency
	__sendCommandWithData(0xD5, 0xF0); //Max
	//3. Set MUX Ratio (0xA8)
	__sendCommandWithData(0xA8,0x3F);
	//4. Set Display Offset
	__sendCommandWithData(0xD3, 0x00);
	//5. Set Display Start Line
	__sendCommand(0x40);
	//6. Enable charge pump regulator
	__sendCommandWithData(0x8D, 0x14);
	//7. Set Segment re-map. 0xA1 - left-to-right, 0xA0 - right-to-left
	__sendCommand(0xA1);
	//8. Set COM Output Scan Direction 0xC0 - down-to-up, 0xC8 - up-to-down
	__sendCommand(0xC8);
	//9. Set COM Pins hardware configuration
	__sendCommandWithData(0xDA, 0x12);
	//10. Set Contrast Control
	__sendCommandWithData(0x81, 0x7F);//127, half-brightness
	//11. Set Pre-Charge period
	__sendCommandWithData(0xD9, 0x22);
	//12. Set VCOMH Deselect Level
	__sendCommandWithData(0xDB, 0x30);
	//13. Disable Entire Display On
	__sendCommand(0xA4);
	//14. Set Normal Display
	__sendCommand(0xA6);
	//15. Clear Screen
	//16. Display On
	__sendCommand(0xAF);
	
	//Установка режма горизонтальной адресации
	__sendCommandWithData(0x20, 0x00);

	//end init

	return STATUS;
}

//Функция установки яркости дисплея
void OLED_setBrightness(uint8_t brightness) {
	__sendCommandWithData(0x81, brightness);
}
//Включить/выключить дисплей
void OLED_display(uint8_t state) {
	__sendCommand(0xAE | state);
}
//Включить/выключить инверсию
void OLED_inversion(uint8_t state) {
	__sendCommand(0xA6 | state);
}

//Функция очистки экрана
void OLED_clear(void) {
	memset(displayBuffer, 0x00, 1024);
	__displayUpdate();
}
//Функция заполнения экрана
void OLED_fill(void) {
	memset(displayBuffer, 0xFF, 1024);
	__displayUpdate();
}

/* Служебные функции */

//Функция для печати буфера на экране
static void __displayUpdate(void) {
	//Установка начального и конечного столбца
	uint8_t columns[6] = {0x80, 0x21, 0x80, 0, 0x80, 127};
	__transmitToDisplay(columns, 6);
	uint8_t pages[6] = {0x80, 0x22, 0x80, 0, 0x80, 7};
	__transmitToDisplay(pages, 6);
	for(uint8_t i = 0; i < 8; i++) {
		__sendDataArray(displayBuffer[i], 128);
	}
}
//Функция отправки одного байта данных
static uint8_t __sendData(uint8_t byte) {
	uint8_t i2c_buffer[2];
	i2c_buffer[0] = 0xC0;//we sending data
	i2c_buffer[1] = byte;
	return __transmitToDisplay(i2c_buffer, 2);
}
//Функция отправки массива данных
static uint8_t __sendDataArray(uint8_t bytes[], uint16_t size) {
	uint8_t i2c_buffer[size+1];
	i2c_buffer[0] = 0x40; //Мы отправляем массив данных
	for (uint16_t i = 1; i <= size; i++) {
		i2c_buffer[i] = bytes[i-1];
	}
	return __transmitToDisplay(i2c_buffer, size+1);
}
//Функция отправки команды
static uint8_t __sendCommand(uint8_t cmd) {
	uint8_t i2c_buffer[2];
	i2c_buffer[0] = 0x80; //we sending command
	i2c_buffer[1] = cmd;
	return 	__transmitToDisplay(i2c_buffer, 2);
}
//Функция отправки команды с данными
static uint8_t __sendCommandWithData(uint8_t cmd, uint8_t data) {
	uint8_t i2c_buffer[4];
	i2c_buffer[0] = 0x80; //we sending command
	i2c_buffer[1] = cmd;
	i2c_buffer[2] = 0x80; //we sending data after command
	i2c_buffer[3] = data;
	return __transmitToDisplay(i2c_buffer, 4);
}
//Функция отправки данных по шине I2C
static uint8_t __transmitToDisplay(uint8_t buff[], uint16_t size) {
	return HAL_I2C_Master_Transmit(_i2c, _address, buff, size, 0xFF);
}





































//Хуйня какая-то
void displayPrintCharXY(unsigned char byte, uint8_t column, uint8_t page) {
	//Печать латиницы или символов из ASCII
	if ((byte < 128) & (byte > 31)) {
		for (uint8_t byteNumber = 0; byteNumber < __getCharLenght(byte); byteNumber++) {
//			__setPage(page);
//			__setColumn(column);
			__sendData(VerdanaLatin10x12[byte-32][1+byteNumber*2]);
//			__setPage(page+1);
//			__setColumn(column);
			__sendData(VerdanaLatin10x12[byte-32][1+byteNumber*2+1]);
			if (column++ > 127) {
				column = 0;
				page++;
			}
		}
	}
	//Печать кириллицы
	if (byte >= 192) {
		for (uint8_t byteNumber = 0; byteNumber < __getCharLenght(byte); byteNumber++) {
//			__setPage(page);
//			__setColumn(column);
			__sendData(VerdanaCyrillic10x12[byte-192][1+byteNumber*2]);
//			__setPage(page+1);
//			__setColumn(column);
			__sendData(VerdanaCyrillic10x12[byte-192][1+byteNumber*2+1]);
			if (column++ > 127) {
				column = 0;
				page++;
			}
		}
	}
}
void displayPrintTextXY(char *text, uint8_t x, uint8_t y) {
	uint8_t i = 0;
	while(*text) {
		i = *text++;
		displayPrintCharXY(i, x, y);
		x += __getCharLenght(i);
	}		
}
void displayDraw(const uint8_t bitmap[], uint8_t lenght, uint8_t width, uint8_t x, uint8_t y) {
	uint8_t pages;
	if (width%8 == 0) {
		pages = width/8;
	} else {
		pages = width/8+1;
	}
	for (uint8_t page = 0; page < pages; page++) {
//		__setPage(page);
//		__setColumn(x);
		for (uint8_t column = 0; column < width; column++) {
			__sendData(bitmap[column + page*width]);
		}
	}
}

static uint8_t __getCharLenght(unsigned char byte) {
	if ((byte < 128) & (byte > 31)) {
		return VerdanaLatin10x12[byte-32][0];
	}
	if (byte >= 192) {
		return VerdanaCyrillic10x12[byte-192][0];
	}
	return 0;
}
