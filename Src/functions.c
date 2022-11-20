/**
  ******************************************************************************
  * @file           : functions.c
  * @brief          : source code for the functions to project task1
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

#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "functions.h"
#include "tim.h"
#include <stdint.h>

//Time definitions
static RTC_TimeTypeDef sTime;
static RTC_DateTypeDef sDate;
static uint32_t prevsec = 0;

//time variables
static uint32_t hour = 0;
static uint32_t minute = 0;
static uint32_t second = 0;

void set_backlight_dimmer(){
	uint32_t ADC_value = get_adc_value();
	htim3.Instance->CCR2 = ADC_value;
}

/**
 * @brief get_adc_value starts ADC and reads the analog signal and returns it to @param ADCvalue.
 * @param  void
 * @return ADCvalue variable to store the analog value.
 */
uint32_t get_adc_value(void){
	uint32_t ADC_value;
	HAL_ADC_Start(&hadc1);
	if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK){
		ADC_value = HAL_ADC_GetValue(&hadc1);
	}
	return ADC_value;
}

/**
*@brief initialize the display settings
*@param none
*@return void
*/
void display_init(){
	SET_CS;
	HAL_Delay(10);
	RESET_CS;

	display_reset();
	HAL_Delay(10);

	write_ins(0x3A);	//8-bit data length
	write_ins(0x09);	//4-line display
	write_ins(0x06);	//bot view
	write_ins(0x1E);	//BS1 = 1
	write_ins(0x39);	//8-bit data length extension
	write_ins(0x1B);	//BS0 = 1
	write_ins(0x6E);	//Devider on and set value
	write_ins(0x56);	//Booseter on and set contrast
	write_ins(0x7A);	//set contrast
	write_ins(0x38);	//8-bit data length extension
	write_ins(0x3a);	//set RE

	write_ins(0x06);
	write_ins(0x38);	//reset RE
	write_ins(0x0f);	//display, cursor, blink on
	write_ins(0x01);    //clearDisplay

	SET_CS;
}

/**
*@brief prints a string to a row on the display
*@param string with content to print
*@param int (0-3), marks row to write on
*@return void
*/
void write_row(uint8_t *string, uint8_t n){

	//make sure that the row is in the bounds
	if(n > 3 || n < 0){
		return;
	}

	switch(n){
	case 0:
		write_ins(0x80);
		break;
	case 1:
		write_ins(0xa0);
		break;
	case 2:
		write_ins(0xc0);
		break;
	case 3:
		write_ins(0xe0);
	}

	do{
		write_d(*string++);
	} while(*string);

}

/**
*@brief takes current RTC value and prints it on display
*@param none
*@return void
*/
void write_time(){
	char time[8];

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	sprintf(time, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
	if(prevsec != sTime.Seconds){
		write_row(time,0);
		prevsec = sTime.Seconds;
	}
}

/**
*@brief takes time information from user via UART
*		and sets the RTC time to given information
*@param none
*@return void
*/
void set_time(){

	//Get time from the user
	HAL_UART_Transmit(&huart5, "\nWrite hour\n\r", 13, 5000);
	while(HAL_UART_Receive(&huart5, (uint8_t *)&hour, 2, 2000) != HAL_OK);
	//if(hour > 23 || hour < 0) hour = 0;

	HAL_UART_Transmit(&huart5, "\nWrite minute\n\r", 15, 5000);
	while(HAL_UART_Receive(&huart5, (uint8_t *)&minute, 2, 2000) != HAL_OK);
	//if(minute > 59 || minute < 0) minute = 0;

	HAL_UART_Transmit(&huart5, "\nWrite second\n\r", 15, 5000);
	while(HAL_UART_Receive(&huart5, (uint8_t *)&second, 2, 2000) != HAL_OK);
	//if(second > 59 || second < 0) second = 0;

	sscanf((uint8_t *)&hour,"%hhu", &sTime.Hours);
	sscanf((uint8_t *)&minute,"%hhu", &sTime.Minutes);
	sscanf((uint8_t *)&second,"%hhu", &sTime.Seconds);

	//Set the time from user
	HAL_RTC_SetDate(&hrtc,&sDate,RTC_FORMAT_BIN);
	HAL_RTC_SetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
}

/**
*@brief sends 1 byte information via SPI to thh display
*@param 8-bit integer. Byte to send to display
*@return void
*/
void write_d(uint8_t byte){
	SET_CS;
	HAL_Delay(10);
	RESET_CS;

	uint8_t data[3];
	data[0] = 0x5f;
	data[1] = byte & 0x0f;
	data[2] = (byte >> 4) & 0x0f;
	HAL_SPI_Transmit(&hspi2, data, 3, 10);
}

/**
*@brief writes instructions to the display
*@param integer, hexadecimal value to target instruction
*@return void
*/
void write_ins(uint8_t ins){
	SET_CS;
	HAL_Delay(10);
	RESET_CS;

	uint8_t outputBuffer[3];
	outputBuffer[0] = 0x1F;
	outputBuffer[1] = ins & 0x0F;
	outputBuffer[2] = (ins >> 4) & 0x0F;
	HAL_SPI_Transmit(&hspi2, outputBuffer, 3, 10);
}

/**
*@brief resets the display
*@param none
*@return void
*/
void display_reset(){
	HAL_GPIO_WritePin(Display_reset_GPIO_Port,  Display_reset_Pin, GPIO_PIN_SET);
}
