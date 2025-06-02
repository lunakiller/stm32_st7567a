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
- include the correct HAL header for your MCU ([#L18](https://github.com/lunakiller/stm32_st7567a/blob/fafc9b766338a6f2744eaa10ff6be3f7f175b925/st7567a/st7567a.h#L18))
- initialize SPI with HAL and provide the handle name ([#L37](https://github.com/lunakiller/stm32_st7567a/blob/fafc9b766338a6f2744eaa10ff6be3f7f175b925/st7567a/st7567a.h#L37))
- initialize GPIOs for the rest of the display pins with HAL and set port and pin assignments (CS, DC and RST) ([#L27-L34](https://github.com/lunakiller/stm32_st7567a/blob/fafc9b766338a6f2744eaa10ff6be3f7f175b925/st7567a/st7567a.h#L27-L34))
- include `st7567a.h` in your project

## Fonts
The library supports variable-width fonts. Two free pixel fonts are included:
- [RetroVille NC](https://www.dafont.com/retroville-nc.font) (9px)
- [videotype](https://www.dafont.com/videotype.font) (18px)

More fonts can be generated using BMFont and slightly patched `bmfont2c.py` script by [Lars Ole Pontoppidan](https://larsee.com/blog/2014/05/converting-fonts-to-c-source-using-bmfont2c/). Instructions can be found in [fonts/README.md](fonts/README.md).

## to-do
- [ ] Partial screen refresh
- [ ] Arbitrary lines


### Source
- [ST7567A Datasheet rev 1.2a](https://www.crystalfontz.com/controllers/Sitronix/ST7567A/)
- [dafont.com](https://www.dafont.com/)
