# LED Brightness Control (CMSIS)

CMSIS-only project for STM32F411RE Nucleo board to control LED brightness using PWM.

## Project Structure

- `Src/` : Source files (`main.c`, `led.c`)
- `Inc/` : Header files (`led.h`, `main.h`)
- `CMSIS/` : CMSIS core and STM32F4 device headers
- `startup_stm32f411xe.s` : Startup assembly file

## Features

- Basic LED blink using CMSIS
- PWM-based LED brightness control
- Modular LED API for embedded learning

## Notes

- No HAL, no CubeMX
- Uses STM32Cube_FW_F4_V1.28.0