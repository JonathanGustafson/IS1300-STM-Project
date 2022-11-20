/**
  ******************************************************************************
  * @file           : functions.h
  * @brief          : header file for functions.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 Gurkanson-Electronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Gurkan under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

#include "stdint.h"
#include "gpio.h"
#include "stdio.h"
#include "stm32l4xx_hal.h"
#include "spi.h"

#define RESET_CS HAL_GPIO_WritePin(CS_SPI2_GPIO_Port, CS_SPI2_Pin, 0)
#define SET_CS HAL_GPIO_WritePin(CS_SPI2_GPIO_Port, CS_SPI2_Pin, 1)

void set_backlight_dimmer(void);
uint32_t get_adc_value(void);

void write_time(void);
void write_row(uint8_t * string, uint8_t n);

void display_init(void);
void display_reset(void);
void write_d(uint8_t byte);
void write_ins(uint8_t ins);

void write_time(void);
void set_time(void);
void write1stRow(uint8_t *string);


