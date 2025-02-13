/**
 * @file GpioHandler.h
 * @author Vaibhav Bansal
 * @brief Header file for handling the GPIO Configuration in Project
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef INC_GPIOHANDLER_H_
#define INC_GPIOHANDLER_H_

#include "stm32g0xx_hal.h"
#include <stdint.h>
#include "EventHandler.h"

#define MAX_GPIO_OBJECTS	25
typedef enum ErrorCodes
{
	GpioInitFail,
	GpioAlreadyRegisterd,
}ErrorCodes_t;

class GpioTypeDef:public EventHandlers
{
	public:
		GPIO_TypeDef *GpioPort;
		uint32_t GpioPin;
		uint32_t GpioMode;
		uint32_t GpioPull;
		uint32_t GpioSpeed;
		uint32_t GpioAlternate;
		void (EventHandlers::*callback)(void);
};

typedef enum GpioState
{
	LOW,
	HIGH,
}GpioState_t;

class GpioHandler
{
	private:
		void enableGpioClock(GPIO_TypeDef *GpioPort);
		bool GpioObjectStore(GpioTypeDef *GpioDefine);
	public:
		void registerGpio(GpioTypeDef *GpioDefine);
		void setGpio(GpioTypeDef *GpioDefine,GpioState_t state );
		void toggleGpio(GpioTypeDef *GpioDefine);
		uint8_t readGpio(GpioTypeDef *GpioDefine);
};

#endif /* INC_GPIOHANDLER_H_ */
