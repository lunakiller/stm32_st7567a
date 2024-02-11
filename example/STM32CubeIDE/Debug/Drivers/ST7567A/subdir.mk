################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/lunakiller/PROJECTS/G431_LCD12864_test/ST7567A/fontlibrary.c \
/home/lunakiller/PROJECTS/G431_LCD12864_test/ST7567A/st7567a.c 

OBJS += \
./Drivers/ST7567A/fontlibrary.o \
./Drivers/ST7567A/st7567a.o 

C_DEPS += \
./Drivers/ST7567A/fontlibrary.d \
./Drivers/ST7567A/st7567a.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ST7567A/fontlibrary.o: /home/lunakiller/PROJECTS/G431_LCD12864_test/ST7567A/fontlibrary.c Drivers/ST7567A/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../../ST7567A -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/ST7567A/st7567a.o: /home/lunakiller/PROJECTS/G431_LCD12864_test/ST7567A/st7567a.c Drivers/ST7567A/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../Core/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc -I../../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../../Drivers/CMSIS/Include -I../../../ST7567A -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-ST7567A

clean-Drivers-2f-ST7567A:
	-$(RM) ./Drivers/ST7567A/fontlibrary.cyclo ./Drivers/ST7567A/fontlibrary.d ./Drivers/ST7567A/fontlibrary.o ./Drivers/ST7567A/fontlibrary.su ./Drivers/ST7567A/st7567a.cyclo ./Drivers/ST7567A/st7567a.d ./Drivers/ST7567A/st7567a.o ./Drivers/ST7567A/st7567a.su

.PHONY: clean-Drivers-2f-ST7567A

