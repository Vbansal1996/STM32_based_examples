



#ifndef INC_TOUCHSENSOR_H_
#define INC_TOUCHSENSOR_H_

#include "stm32g0xx_hal.h"

#define PRESSED		0x01
#define TOUCH_SLAVE_ADDRESS (0x37<<1)

typedef enum ButtonPressed
{
	BUTTON1 = 0,
	BUTTON2,
	BUTTON3 = 3,
	BUTTON4,
	BUTTON5,
}ButtonPressed_t;

typedef enum TouchPad
{
	MODE,
	SETValue,
	POWER,
	UP,
	DOWN,
	NONE,
}TouchPad_t;

typedef enum ModeSet
{
	SetTemp,
	SetSmartTime,
	SetRealTime,
}ModeSet_t;

typedef enum TimeSet
{
	Hour,
	Minute,
	Duration,
}TimeSet_t;

void TouchSensorInit(void);
void TouchHandler(TouchPad_t touch);
void TouchISR(void);
void EXTI2_3_IRQHandler(void);
#endif
