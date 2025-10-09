/*
 * st7567a.c
 *
 *  Created on: Feb 7, 2024
 *      Author: lunakiller
 *         git: https://github.com/lunakiller/stm32_st7567a
 */

#include "st7567a.h"
#include <stdint.h>
#include <stdbool.h>


#define ST7567A_DISPLAY_ON 					0xAF
#define ST7567A_DISPLAY_OFF					0xAE
#define ST7567A_SET_START_LINE			0x40			// + line0 - line63
#define ST7567A_SEG_NORMAL					0xA0
#define ST7567A_SEG_REVERSE					0xA1
#define ST7567A_COLOR_NORMAL				0xA6
#define ST7567A_COLOR_INVERSE				0xA7
#define ST7567A_DISPLAY_DRAM				0xA4
#define ST7567A_DISPLAY_ALL_ON			0xA5
#define ST7567A_SW_RESET						0xE2
#define ST7567A_COM_NORMAL					0xC0
#define ST7567A_COM_REVERSE					0xC8
#define ST7567A_POWER_CONTROL				0x28
#define ST7567A_SET_RR							0x20			// + RR[2:0]; 3.0, 3.5, ..., 6.5
#define ST7567A_SET_EV_CMD					0x81
#define ST7567A_NOP									0xE3

#define ST7567A_PAGE_ADDR						0xB0			// + 0x0 - 0x7 -> page0 - page7
#define ST7567A_COL_ADDR_H					0x10			// + X[7:4]
#define ST7567A_COL_ADDR_L					0x00			// + X[3:0]

#define ST7567A_BIAS7								0xA3
#define ST7567A_BIAS9								0xA2

#define ST7567A_PWR_BOOSTER_ON			0x04
#define ST7567A_PWR_REGULATOR_ON		0x02
#define ST7567A_PWR_FOLLOWER_ON			0x01

typedef struct {
		uint8_t curr_x;
		uint8_t curr_y;
} ST7567A_pos_t;


extern SPI_HandleTypeDef ST7567A_SPI_PORT;

static uint8_t ST7567A_buffer[ST7567A_BUFFER_SIZE];
static ST7567A_pos_t ST7567A;

// ----------------------------------------------------------------------------
void st7567a_Reset(void) {
	HAL_GPIO_WritePin(ST7567A_RST_GPIO_Port, ST7567A_RST_Pin, GPIO_PIN_RESET);			// reset low
	HAL_Delay(5);
	HAL_GPIO_WritePin(ST7567A_RST_GPIO_Port, ST7567A_RST_Pin, GPIO_PIN_SET);				// reset high
	HAL_Delay(5);
}

void st7567a_WriteCommand(uint8_t byte) {
	HAL_GPIO_WritePin(ST7567A_CS_GPIO_Port, ST7567A_CS_Pin, GPIO_PIN_RESET);				// chip select
	HAL_GPIO_WritePin(ST7567A_DC_GPIO_Port, ST7567A_DC_Pin, GPIO_PIN_RESET);				// byte is command
	HAL_SPI_Transmit(&ST7567A_SPI_PORT, &byte, 1, ST7567A_TIMEOUT);
	HAL_GPIO_WritePin(ST7567A_CS_GPIO_Port, ST7567A_CS_Pin, GPIO_PIN_SET);					// chip unselect
}

void st7567a_WriteData(uint8_t* buffer, uint8_t size) {
	HAL_GPIO_WritePin(ST7567A_CS_GPIO_Port, ST7567A_CS_Pin, GPIO_PIN_RESET);				// chip select
	HAL_GPIO_WritePin(ST7567A_DC_GPIO_Port, ST7567A_DC_Pin, GPIO_PIN_SET);					// byte is display data
	HAL_SPI_Transmit(&ST7567A_SPI_PORT, buffer, size, ST7567A_TIMEOUT);
	HAL_GPIO_WritePin(ST7567A_CS_GPIO_Port, ST7567A_CS_Pin, GPIO_PIN_SET);					// chip unselect
}

// ----------------------------------------------------------------------------
void st7567a_Init(void) {
	st7567a_Reset();

	st7567a_WriteCommand(ST7567A_BIAS9);
	st7567a_WriteCommand(ST7567A_SEG_NORMAL);
	st7567a_WriteCommand(ST7567A_COM_REVERSE);
	st7567a_WriteCommand(ST7567A_SET_RR | 0x4);		// regulation ratio 5.0
	st7567a_WriteCommand(ST7567A_SET_EV_CMD);
	st7567a_WriteCommand(38);											// set EV=38

	st7567a_WriteCommand(ST7567A_POWER_CONTROL | ST7567A_PWR_BOOSTER_ON);
	st7567a_WriteCommand(ST7567A_POWER_CONTROL | ST7567A_PWR_BOOSTER_ON | ST7567A_PWR_REGULATOR_ON);
	st7567a_WriteCommand(ST7567A_POWER_CONTROL | ST7567A_PWR_BOOSTER_ON | ST7567A_PWR_REGULATOR_ON | ST7567A_PWR_FOLLOWER_ON);

	st7567a_Fill(PIXEL_OFF);
	st7567a_Display();
	st7567a_WriteCommand(ST7567A_DISPLAY_DRAM);
	st7567a_WriteCommand(ST7567A_DISPLAY_ON);
}

void st7567a_Fill(ST7567A_PixelState_t state) {
	uint8_t val = (state == PIXEL_ON) ? 0xFF : 0x00;

	for(uint16_t i = 0; i < ST7567A_BUFFER_SIZE; ++i) {
		ST7567A_buffer[i] = val;
	}
}

void st7567a_Display(void) {
	st7567a_WriteCommand(ST7567A_SET_START_LINE);

	for(uint8_t i = 0; i < ST7567A_HEIGHT/8; i++) {
		st7567a_WriteCommand(ST7567A_PAGE_ADDR + i); 				// set DDRAM page
		st7567a_WriteCommand(ST7567A_COL_ADDR_H);						// set MSB column address
		st7567a_WriteCommand(ST7567A_COL_ADDR_L);						// set LSB column address
		st7567a_WriteData(&ST7567A_buffer[ST7567A_WIDTH * i], ST7567A_WIDTH);
	}
	st7567a_WriteCommand(ST7567A_DISPLAY_ON);
}

// default 0x20 = 32
void st7567a_SetContrast(uint8_t val) {
	st7567a_WriteCommand(ST7567A_SET_EV_CMD);
	st7567a_WriteCommand((val & 0x3f));
}

void st7567a_PowerSave(bool enable) {
	if(enable) {
		st7567a_WriteCommand(ST7567A_DISPLAY_OFF);
		st7567a_WriteCommand(ST7567A_DISPLAY_ALL_ON);
	}
	else {
		st7567a_WriteCommand(ST7567A_DISPLAY_DRAM);
		st7567a_WriteCommand(ST7567A_DISPLAY_ON);
	}
}

void st7567a_DrawPixel(uint8_t x, uint8_t y, ST7567A_PixelState_t state) {
	if(x >= ST7567A_WIDTH || y >= ST7567A_HEIGHT) {
		return;
	}

	// Draw in the right color
	if(state == PIXEL_ON) {
		ST7567A_buffer[x + (y / 8) * ST7567A_WIDTH] |= 1 << (y % 8);
	}
	else {
		ST7567A_buffer[x + (y / 8) * ST7567A_WIDTH] &= ~(1 << (y % 8));
	}
}

void st7567a_DrawVLine(uint8_t x, ST7567A_PixelState_t state) {
	if(state == PIXEL_ON) {
		for(int y = 0; y < ST7567A_PAGES; ++y) {
			ST7567A_buffer[x + y * ST7567A_WIDTH] |= 0xff;
		}
	}
	else {
		for(int y = 0; y < ST7567A_PAGES; ++y) {
			ST7567A_buffer[x + y * ST7567A_WIDTH] &= 0;
		}
	}
}

void st7567a_DrawHLine(uint8_t y, ST7567A_PixelState_t state) {
	uint8_t val = 1 << (y % 8);
	if(state == PIXEL_ON) {
		for(int x = 0; x < ST7567A_WIDTH; ++x) {
			ST7567A_buffer[x + (y / 8) * ST7567A_WIDTH] |= val;
		}
	}
	else {
		for(int x = 0; x < ST7567A_WIDTH; ++x) {
			ST7567A_buffer[x + (y / 8) * ST7567A_WIDTH] &= ~(val);
		}
	}
}

void st7567a_SetCursor(uint8_t x, uint8_t y) {
	ST7567A.curr_x = x;
	ST7567A.curr_y = y;
}

void st7567a_WriteChar(char ch, fontStyle_t font, ST7567A_PixelState_t state) {
	st7567a_WriteChar_FixedWidth(ch, font, 0, state);
}

// fixed_width = 0 -> variable-width
void st7567a_WriteChar_FixedWidth(char ch, fontStyle_t font, uint8_t fixed_width, ST7567A_PixelState_t state) {
	// check if char is available in the font
	if(ch < font.FirstAsciiCode) {
		ch = 0;
	}
	else {
		ch -= font.FirstAsciiCode;
	}

	// variable-width/fixed-width
	uint8_t g_width;
	if(fixed_width == 0) {
		g_width = font.GlyphWidth[(int)ch];
	}
	else {
		g_width = fixed_width;
	}

	// check remaining space on the current line
	if (ST7567A_WIDTH < (ST7567A.curr_x + g_width) ||
		ST7567A_HEIGHT < (ST7567A.curr_y + font.GlyphHeight)) {
		// not enough space
		return;
	}

	uint32_t chr;

	for(uint32_t j = 0; j < font.GlyphHeight; ++j) {
		uint8_t width = g_width;

		for(uint32_t w = 0; w < font.GlyphBytesWidth; ++w) {
			chr = font.GlyphBitmaps[(ch * font.GlyphHeight + j) * font.GlyphBytesWidth + w];

			uint8_t w_range = width;
			if(w_range >= 8) {
				w_range = 8;
				width -= 8;
			}

			for(uint32_t i = 0; i < w_range; ++i) {
				if((chr << i) & 0x80)  {
					st7567a_DrawPixel(ST7567A.curr_x + i + w*8, ST7567A.curr_y + j, state);
				}
			}
		}
	}

	ST7567A.curr_x += g_width;
}

void st7567a_WriteString(const char *str, fontStyle_t font, ST7567A_PixelState_t state) {
	while(*str) {
		st7567a_WriteChar(*str++, font, state);
	}
}

// fixed_width = 0 -> variable-width
void st7567a_WriteString_FixedWidth(const char *str, fontStyle_t font, uint8_t fixed_width, ST7567A_PixelState_t state) {
	while(*str) {
		st7567a_WriteChar_FixedWidth(*str++, font, fixed_width, state);
	}
}

// draw XBitMap (.xbm)
void st7567a_DrawXBitmap(uint8_t x, uint8_t y, const unsigned char* bitmap, uint8_t w, uint8_t h, ST7567A_PixelState_t state) {
	uint8_t byteWidth = (w + 7) / 8;		// pad each row to full bytes

	for (uint8_t j = 0; j < h; j++) {
		uint8_t yPos = y + j;
		if (yPos >= ST7567A_HEIGHT) {			// check boundaries
			continue;
		}

		for (uint8_t i = 0; i < w; i++) {
			uint8_t xPos = x + i;
			if (xPos >= ST7567A_WIDTH) {		// check boundaries
				continue;
			}

			uint8_t byte = bitmap[j * byteWidth + (i / 8)];
			// XBM: 0 = ON, 1 = OFF
			if ((byte & (1 << (i % 8))) == 0) {
				// calculate buffer index for (xPos, yPos)
				uint16_t index = xPos + (yPos / 8) * ST7567A_WIDTH;

				if (state == PIXEL_ON) {
						ST7567A_buffer[index] |= (1 << (yPos % 8));
				}
				else {
						ST7567A_buffer[index] &= ~(1 << (yPos % 8));
				}
			}
		}
	}
}
