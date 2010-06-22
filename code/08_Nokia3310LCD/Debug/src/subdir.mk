################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/N3310.c \
../src/cr_startup_lpc11.c \
../src/eeprom_i2c.c \
../src/i2c.c \
../src/main.c \
../src/ssp.c \
../src/uart.c 

OBJS += \
./src/N3310.o \
./src/cr_startup_lpc11.o \
./src/eeprom_i2c.o \
./src/i2c.o \
./src/main.o \
./src/ssp.o \
./src/uart.o 

C_DEPS += \
./src/N3310.d \
./src/cr_startup_lpc11.d \
./src/eeprom_i2c.d \
./src/i2c.d \
./src/main.d \
./src/ssp.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_CMSIS=CMSISv1p30_LPC11xx -DF_AHB=24000000UL -DDEBUG -D__CODE_RED -I../../inc -O1 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


