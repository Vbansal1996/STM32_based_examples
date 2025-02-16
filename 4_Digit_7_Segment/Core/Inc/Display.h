/*
 * Display.h
 *
 *  Created on: Jan 28, 2021
 *      Author: Vaibhav Bansal
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stm32g0xx_hal.h"
#include "stdio.h"
#include "stdbool.h"

#define RtcUpdateInterval	60*1000
typedef enum Line
{
	Line1,
	Line2,
	Line3,
	Line4,
}Line_t;

typedef enum Block
{
	Block1,
	Block2,
	Block3,
	Block4,
}Block_t;

typedef enum Digit
{
	DIGIT_0,
	DIGIT_1,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4,
	DIGIT_5,
	DIGIT_6,
	DIGIT_7,
	DIGIT_8,
	DIGIT_9,
	ALPHA_C = 0x0C,
	ALPHA_E = 0x0E,
	DIGIT_NULL = 0x0F,
}Digit_t;

typedef struct DisplayDigits
{
	uint8_t display1 :4;
	uint8_t display2 :4;
	uint8_t display3 :4;
	uint8_t display4 :4;
}DisplayDigits_t;

extern DisplayDigits_t Display_t;

#define DISPLAY_0 		0x01
#define DISPLAY_1 		0x4F
#define DISPLAY_2 		0x12
#define DISPLAY_3 		0x06
#define DISPLAY_4 		0x4c
#define DISPLAY_5 		0x24
#define DISPLAY_6 		0x20
#define DISPLAY_7 		0x0F
#define DISPLAY_8 		0x00
#define DISPLAY_9		0x04
#define Alpha_C			0x31
#define Alpha_E			0x30
#define DISPLAY_NULL	0x0F

void DisplaySegment(Block_t block, uint8_t value);
void DisplayDp(uint8_t line, uint8_t DP);
void DisplayState(bool state);
void DisplayInit(void);
void DisplayColumn(uint8_t value);
void DisplayBlinkTaskHandler(void);
void DisplayValue(uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4);
#endif /* INC_DISPLAY_H_ */
