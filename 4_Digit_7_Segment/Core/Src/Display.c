/*
 * Display.c
 *
 *  Created on: Jan 28, 2021
 *      Author: Vaibhav Bansal
 */

#include "Display.h"
#include "PinConfig.h"
#include "main.h"

static void SelectLine(uint8_t Line, uint8_t value);
static void DisplayDigit(uint8_t value);
static void DisplayNibble(uint8_t value);

bool DisplayPower = false;
DisplayDigits_t Display_t;
uint8_t digit1, digit2, digit3, digit4;
uint8_t counter = 0;

void DisplaySegment(Block_t block, uint8_t value)
{
	if(DisplayPower == false)
		return;

	switch(block)
	{
		case Block1:
			SelectLine(Line4, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line3, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line2, disable);
			DisplayDigit(DISPLAY_NULL);
			DisplayDigit(value);
			SelectLine(Line1, enable);
		break;

		case Block2:
			SelectLine(Line1, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line3, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line4, disable);
			DisplayDigit(DISPLAY_NULL);
			DisplayDigit(value);
			SelectLine(Line2, enable);
		break;

		case Block3:
			SelectLine(Line2, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line4, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line1, disable);
			DisplayDigit(DISPLAY_NULL);
			DisplayDigit(value);
			SelectLine(Line3, enable);
		break;

		case Block4:
			SelectLine(Line3, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line2, disable);
			DisplayDigit(DISPLAY_NULL);
			SelectLine(Line1, disable);
			DisplayDigit(DISPLAY_NULL);
			DisplayDigit(value);
			SelectLine(Line4, enable);
		break;

	}
}

static void SelectLine(uint8_t Line, uint8_t value)
{
	switch(Line)
	{
		case Line1:
			HAL_GPIO_WritePin(SEGMENT7_DIGIT1_PORT, SEGMENT7_DIGIT1_PIN ,value);
			break;
		case Line2:
			HAL_GPIO_WritePin(SEGMENT7_DIGIT2_PORT, SEGMENT7_DIGIT2_PIN ,value);
			break;
		case Line3:
			HAL_GPIO_WritePin(SEGMENT7_DIGIT3_PORT, SEGMENT7_DIGIT3_PIN ,value);
			break;
		case Line4:
			HAL_GPIO_WritePin(SEGMENT7_DIGIT4_PORT, SEGMENT7_DIGIT4_PIN ,value);
			break;
	}
}

static void DisplayDigit(uint8_t value)
{
	switch(value)
	{
		case DIGIT_0:
			DisplayNibble(DISPLAY_0);
		break;

		case DIGIT_1:
			DisplayNibble(DISPLAY_1);
		break;

		case DIGIT_2:
			DisplayNibble(DISPLAY_2);
		break;

		case DIGIT_3:
			DisplayNibble(DISPLAY_3);
		break;

		case DIGIT_4:
			DisplayNibble(DISPLAY_4);
		break;

		case DIGIT_5:
			DisplayNibble(DISPLAY_5);
		break;

		case DIGIT_6:
			DisplayNibble(DISPLAY_6);
		break;

		case DIGIT_7:
			DisplayNibble(DISPLAY_7);
		break;

		case DIGIT_8:
			DisplayNibble(DISPLAY_8);
		break;

		case DIGIT_9:
			DisplayNibble(DISPLAY_9);
		break;

		case ALPHA_C:
			DisplayNibble(Alpha_C);
		break;

		case ALPHA_E:
			DisplayNibble(Alpha_E);
		break;

		case DIGIT_NULL:
			DisplayNibble(DISPLAY_NULL);
		break;
	}
}

static void DisplayNibble(uint8_t value)
{
	HAL_GPIO_WritePin(SEGMENT7_A1_PORT, SEGMENT7_A1_PIN ,((value >> 6) & 0x01));
	HAL_GPIO_WritePin(SEGMENT7_B1_PORT, SEGMENT7_B1_PIN ,((value >> 5) & 0x01));
	HAL_GPIO_WritePin(SEGMENT7_C1_PORT, SEGMENT7_C1_PIN ,((value >> 4) & 0x01));
	HAL_GPIO_WritePin(SEGMENT7_D1_PORT, SEGMENT7_D1_PIN ,((value >> 3) & 0x01));
	HAL_GPIO_WritePin(SEGMENT7_E1_PORT, SEGMENT7_E1_PIN ,((value >> 2) & 0x01));
	HAL_GPIO_WritePin(SEGMENT7_F1_PORT, SEGMENT7_F1_PIN ,((value >> 1) & 0x01));
	HAL_GPIO_WritePin(SEGMENT7_G1_PORT, SEGMENT7_G1_PIN ,((value >> 0) & 0x01));
}

void DisplayDp(uint8_t line, uint8_t DP)
{
	SelectLine(line, enable);
	HAL_GPIO_WritePin(SEGMENT7_H1_PORT, SEGMENT7_H1_PIN ,DP);
}

void DisplayColumn(uint8_t value)
{
	if(DisplayPower == enable)
		HAL_GPIO_WritePin(DISPLAY_DP_PORT, DISPLAY_DP_PIN ,value);
}

void DisplayState(bool state)
{
	DisplayPower = state;
	SelectLine(Line1,state);
	SelectLine(Line2,state);
	SelectLine(Line3,state);
	SelectLine(Line4,state);
	DisplayDigit(DIGIT_8);
}

void DisplayInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	 /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_6
	                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_12, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
	                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
	                          |GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA0 PA1 PA4 PA6
	                           PA7 PA8 PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_6
	                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pins : PB0 PB1 PB2 PB10
	                           PB11 PB12 PB13 PB14
	                           PB15 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
	                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
	                          |GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PC6 PC7 */
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  /*Configure GPIO pin : PA11 */
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /*Configure GPIO pin : PD0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	  /*Configure GPIO pin : PB3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void DisplayBlinkTaskHandler(void)
{
	counter++;
	if((counter % 2) != 0)
		{
			digit1 = Display_t.display1;
			digit2 = Display_t.display2;
			digit3 = Display_t.display3;
			digit4 = Display_t.display4;
			DisplayValue(DISPLAY_NULL, DISPLAY_NULL, DISPLAY_NULL, DISPLAY_NULL);
		}
	else
		{
			DisplayValue(digit1, digit2, digit3, digit4);
		}
}

void DisplayValue(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
{
	Display_t.display1 = digit1;
	Display_t.display2 = digit2;
	Display_t.display3 = digit3;
	Display_t.display4 = digit4;
}
