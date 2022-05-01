################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/benchmark/benchmark.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/test/test.c \
D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/IDE/STM32Cube/wolfssl_example.c 

OBJS += \
./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/benchmark.o \
./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/test.o \
./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/wolfssl_example.o 

C_DEPS += \
./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/benchmark.d \
./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/test.d \
./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/wolfssl_example.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/benchmark.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/benchmark/benchmark.c Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/test.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/wolfcrypt/test/test.c Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/wolfssl_example.o: D:/TCC/A/nucleo/Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/IDE/STM32Cube/wolfssl_example.c Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../wolfSSL -I../../Middlewares/Third_Party/wolfSSL_wolfSSL_wolfSSL/wolfssl/ -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-wolfSSL-2f-wolfSSL-2f-wolfCrypt-2f-Test

clean-Middlewares-2f-wolfSSL-2f-wolfSSL-2f-wolfCrypt-2f-Test:
	-$(RM) ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/benchmark.d ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/benchmark.o ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/benchmark.su ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/test.d ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/test.o ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/test.su ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/wolfssl_example.d ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/wolfssl_example.o ./Middlewares/wolfSSL/wolfSSL/wolfCrypt/Test/wolfssl_example.su

.PHONY: clean-Middlewares-2f-wolfSSL-2f-wolfSSL-2f-wolfCrypt-2f-Test

