################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/crl.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/internal.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/keys.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/ocsp.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/sniffer.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/ssl.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/tls.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/tls13.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/wolfio.c 

OBJS += \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/crl.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/internal.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/keys.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ocsp.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/sniffer.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ssl.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls13.o \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/wolfio.o 

C_DEPS += \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/crl.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/internal.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/keys.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ocsp.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/sniffer.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ssl.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls13.d \
./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/wolfio.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/crl.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/crl.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/internal.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/internal.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/keys.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/keys.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ocsp.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/ocsp.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/sniffer.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/sniffer.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ssl.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/ssl.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/tls.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls13.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/tls13.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/wolfio.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/src/wolfio.c Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-wolfSSL-2f-wolfSSL-2f-wolfSSL-2f-Core

clean-Middlewares-2f-wolfSSL-2f-wolfSSL-2f-wolfSSL-2f-Core:
	-$(RM) ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/crl.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/crl.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/crl.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/internal.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/internal.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/internal.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/keys.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/keys.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/keys.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ocsp.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ocsp.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ocsp.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/sniffer.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/sniffer.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/sniffer.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ssl.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ssl.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/ssl.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls13.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls13.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/tls13.su ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/wolfio.d ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/wolfio.o ./Middlewares/wolfSSL/wolfSSL/wolfSSL/Core/wolfio.su

.PHONY: clean-Middlewares-2f-wolfSSL-2f-wolfSSL-2f-wolfSSL-2f-Core

