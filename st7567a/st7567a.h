/*
 * st7567a.h
 *  Library for ST7567A based monochrome displays for STM32 MCUs.
 *
 *  Created on: Feb 7, 2024
 *      Author: lunakiller
 *         git: https://github.com/lunakiller/stm32_st7567a
 */

#ifndef __ST7567A_H
#define __ST7567A_H

#include "fontlibrary.h"
#include <stdbool.h>

/* ------------------------------- SETTINGS -------------------------------- */
// Include relevant ST HAL
#include "stm32g4xx_hal.h"

// Display parameters
#define ST7567A_WIDTH						128
#define ST7567A_HEIGHT					64
#define ST7567A_PAGES						8
#define ST7567A_BUFFER_SIZE   	(ST7567A_WIDTH * ST7567A_HEIGHT / ST7567A_PAGES)

// Pin assignments
#define ST7567A_RST_GPIO_Port		GPIOA
#define ST7567A_RST_Pin					GPIO_PIN_4

#define ST7567A_CS_GPIO_Port		GPIOA
#define ST7567A_CS_Pin					GPIO_PIN_3

#define ST7567A_DC_GPIO_Port		GPIOA
#define ST7567A_DC_Pin					GPIO_PIN_6

// SPI settings
#define ST7567A_SPI_PORT 				hspi1
#define ST7567A_TIMEOUT  				100

// enum for "color"
typedef enum {
	PIXEL_OFF = 0x0,
	PIXEL_ON = 0x1
} ST7567A_PixelState_t;


/* ------------------------------- FUNCTIONS ------------------------------- */
void st7567a_Init(void);
void st7567a_Fill(ST7567A_PixelState_t state);
void st7567a_Display(void);

void st7567a_DrawPixel(uint8_t x, uint8_t y, ST7567A_PixelState_t state);
void st7567a_DrawVLine(uint8_t x, ST7567A_PixelState_t state);
void st7567a_DrawHLine(uint8_t y, ST7567A_PixelState_t state);

void st7567a_SetCursor(uint8_t x, uint8_t y);
void st7567a_WriteChar(char ch, fontStyle_t font, ST7567A_PixelState_t state);
void st7567a_WriteString(const char *str, fontStyle_t font, ST7567A_PixelState_t state);
void st7567a_WriteChar_FixedWidth(char ch, fontStyle_t font, uint8_t fixed_width, ST7567A_PixelState_t state);
void st7567a_WriteString_FixedWidth(const char *str, fontStyle_t font, uint8_t fixed_width, ST7567A_PixelState_t state);

void st7567a_SetContrast(uint8_t val);
void st7567a_PowerSave(bool enable);

// low-level functions
void st7567a_Reset(void);
void st7567a_WriteCommand(uint8_t byte);
void st7567a_WriteData(uint8_t* buffer, uint8_t size);


#endif /* __ST7567A_H */

