/**
  ******************************************************************************
  * @file           : test.c
  * @brief          : test.c test code for project task 1
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
#include "test.h"
#include "functions.h"
#include <stdint.h>


void test_program(){

	display_init();
	backlight_test();
	write_row_test();
}

/*
 * @brief	tests the backlight of the display by making
 * 			the display blink in all possible colors.
 */

void backlight_test(){

	//blink green
	HAL_GPIO_WritePin(DisplayGreen_GPIO_Port, DisplayGreen_Pin, SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(DisplayGreen_GPIO_Port, DisplayGreen_Pin, RESET);

	//blink red
	HAL_GPIO_WritePin(DisplayRed_GPIO_Port, DisplayRed_Pin, SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(DisplayRed_GPIO_Port, DisplayRed_Pin, RESET);

	//blink white
	HAL_GPIO_WritePin(DisplayWhite_GPIO_Port, DisplayWhite_Pin, SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(DisplayWhite_GPIO_Port, DisplayWhite_Pin, RESET);

}


/*
 * @brief	tests the write_row function by writing
 * 			to each row using the function.
 */
void write_row_test(){

	write_row("testarRad0", 0);
	write_row("testarRad1", 1);
	write_row("testarRad2", 2);
	write_row("testarRad3", 3);
	HAL_Delay(2000);

	display_init();
}
