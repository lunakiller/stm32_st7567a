# stm32_st7567a
Library for ST7567A based monochrome displays for STM32 MCUs. 
Based on STM32 HAL libraries. 

Tested with EstarDyn 128x64 SPI LCD Display on WeAct STM32G431CoreBoard. ([Ali](https://a.aliexpress.com/_Exts5wt))

Right now the library supports 4-wire SPI only.

## Features
- framebuffer-based
- basic graphic primitives
    - point
    - horintal line (fast)
    - vertical line (fast)
- variable-width fonts
- power-save procedure
- example project
    - build with Makefile or STM32CubeIDE

## How-to
- include the correct HAL header for your MCU
- initialize SPI with HAL and provide the handle name
- set port and pin assignments for the rest of the pins (CS, DC and RST)
- include `st7567a.h` in your project

## Custom fonts
More fonts can be generated using BMFont and slightly patched `bmfont2c.py` script by [Lars Ole Pontoppidan](https://larsee.com/blog/2014/05/converting-fonts-to-c-source-using-bmfont2c/). Instructions to come!

## to-do
- [ ] Partial screen refresh
- [ ] Arbitrary lines


### Source
- [ST7567A Datasheet rev 1.2a](https://www.crystalfontz.com/controllers/Sitronix/ST7567A/)
