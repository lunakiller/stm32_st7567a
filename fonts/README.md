## How to generate fonts

1) pick the font you want to use in .ttf/.otf format
2) use *Bitmap Font Generator (bmfont)* to generate the bitmap:
    - online version: [SnowB Bitmap Font](https://snowb.org/)
    - standalone version: [bmfont](https://www.angelcode.com/products/bmfont/)
3) choose which glyphs to convert
    - examples use the ASCII range 32-126:
        - `` !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~``
4) fill the "*Font size*" and "*Line height*", other sections can remain at default values
5) set "*Fill*" to solid white (#FFFFFF) and "*Background Color*" to black (#000000)
6) export as "*BMFont XML*", you will get files `<fontname>.fnt` and `<fontname>.png`
7) copy both files to the same folder where the `bmfont2c.py` script is
8) prepare the `bmfont2c.cfg` config file in the same folder and tweak the settings
    - example:
        ```
        [General]
        OutputHeader = fontlibrary.h
        OutputSource = fontlibrary.c

        [Font1]
        InputFile = videotype_18.fnt
        CFontName = videotype_18
        FirstAscii = 32
        LastAscii = 126
        BytesWidth = 2
        BytesHeight = 18
        CropX = 0
        CropY = 2
        ```
    - add more fonts by adding sections named Font1, Font2, Font3, ...
9) run the `bmfont2c.py` script to generate the header and source file
10) check the output font library and repeat steps 8. and 9. until the result matches your expectations
11) replace/modify the font library in the display driver library folder


### Source
- original script and instructions by [Lars Ole Pontoppidan](https://larsee.com/blog/2014/05/converting-fonts-to-c-source-using-bmfont2c/)