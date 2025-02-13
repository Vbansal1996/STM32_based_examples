/*
 * GpioHandler.c
 *
 *  Created on: Jan 31, 2021
 *      Author: Vaibhav Bansal
 */
#include "GpioHandler.h"
#include "stm32g0xx_hal.h"
#include "stdio.h"

static uint32_t GpioHandlerObject[MAX_GPIO_OBJECTS];
static uint8_t ObjectCounter = 0;

void GpioHandler::registerGpio(GpioTypeDef *GpioDefine)
{
	if(GpioObjectStore(GpioDefine) == false)
		return;

	GPIO_InitTypeDef GpioInit = {0};
	GpioInit.Pin = GpioDefine->GpioPin;
	GpioInit.Pull = GpioDefine->GpioPull;
	GpioInit.Speed = GpioDefine->GpioSpeed;
	GpioInit.Mode = GpioDefine->GpioMode;
	GpioInit.Alternate = GpioDefine->GpioAlternate;
	enableGpioClock(GpioDefine->GpioPort);
	HAL_GPIO_Init(GpioDefine->GpioPort,&GpioInit);

	if((GpioDefine->GpioPin == GPIO_PIN_0 || GpioDefine->GpioPin == GPIO_PIN_1)
	&& (GpioDefine->GpioMode == GPIO_MODE_IT_RISING ||  GpioDefine->GpioMode == GPIO_MODE_IT_FALLING))
	{
		HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);
	}
	else if((GpioDefine->GpioPin == GPIO_PIN_2 || GpioDefine->GpioPin == GPIO_PIN_3)
	&& (GpioDefine->GpioMode == GPIO_MODE_IT_RISING ||  GpioDefine->GpioMode == GPIO_MODE_IT_FALLING))
	{
		HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
	}
	else if((GpioDefine->GpioPin >= GPIO_PIN_4 || GpioDefine->GpioPin <= GPIO_PIN_15)
	&& (GpioDefine->GpioMode == GPIO_MODE_IT_RISING ||  GpioDefine->GpioMode == GPIO_MODE_IT_FALLING))
	{
		HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
	}

}

void GpioHandler::enableGpioClock(GPIO_TypeDef *GpioPort)
{
	if(GpioPort == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();

	else if(GpioPort == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();

	else if(GpioPort == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();

	else if(GpioPort == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();

	else if(GpioPort == GPIOF)
		__HAL_RCC_GPIOF_CLK_ENABLE();
}

void GpioHandler::setGpio(GpioTypeDef *GpioDefine,GpioState_t state )
{
	HAL_GPIO_WritePin(GpioDefine->GpioPort, GpioDefine->GpioPin, (GPIO_PinState)state);
}

void GpioHandler::toggleGpio(GpioTypeDef *GpioDefine)
{
	HAL_GPIO_TogglePin(GpioDefine->GpioPort, GpioDefine->GpioPin);
}

uint8_t GpioHandler::readGpio(GpioTypeDef *GpioDefine)
{
	return HAL_GPIO_ReadPin(GpioDefine->GpioPort, GpioDefine->GpioPin);
}

bool GpioHandler::GpioObjectStore(GpioTypeDef *GpioDefine)
{
	if(ObjectCounter == MAX_GPIO_OBJECTS)
			return false;

	GpioHandlerObject[ObjectCounter] = (uint32_t)GpioDefine;
	ObjectCounter++;
	return true;
}

extern "C" void EXTI0_1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
	for(uint8_t i = 0;i < ObjectCounter;i++)
	{
		GpioTypeDef *GpioDefine = NULL;
		GpioDefine = (GpioTypeDef *)GpioHandlerObject[i];
		if((GpioDefine->GpioPin == GPIO_PIN_0 || GpioDefine->GpioPin == GPIO_PIN_1)
			&& (GpioDefine->GpioMode == GPIO_MODE_IT_FALLING || GpioDefine->GpioMode == GPIO_MODE_IT_RISING))
		{
			((GpioDefine->*(GpioDefine->callback))());
		}
	}
}

extern "C" void EXTI2_3_IRQHandler(void)
{
	for(uint8_t i = 0;i < ObjectCounter;i++)
	{
		GpioTypeDef *GpioDefine = NULL;
		GpioDefine = (GpioTypeDef *)GpioHandlerObject[i];
		if((GpioDefine->GpioPin == GPIO_PIN_2 || GpioDefine->GpioPin == GPIO_PIN_3)
			&& (GpioDefine->GpioMode == GPIO_MODE_IT_FALLING || GpioDefine->GpioMode == GPIO_MODE_IT_RISING))
		{
			HAL_GPIO_EXTI_IRQHandler(GpioDefine->GpioPin);
			((GpioDefine->*(GpioDefine->callback))());
		}
	}
}

extern "C" void EXTI4_15_IRQHandler(void)
{
	for(uint8_t i = 0;i < ObjectCounter;i++)
	{
		GpioTypeDef *GpioDefine = NULL;
		GpioDefine = (GpioTypeDef *)GpioHandlerObject[i];
		if((GpioDefine->GpioPin >= GPIO_PIN_4)
			&& (GpioDefine->GpioMode == GPIO_MODE_IT_FALLING || GpioDefine->GpioMode == GPIO_MODE_IT_RISING))
		{
			HAL_GPIO_EXTI_IRQHandler(GpioDefine->GpioPin);
			((GpioDefine->*(GpioDefine->callback))());
		}
	}
}
