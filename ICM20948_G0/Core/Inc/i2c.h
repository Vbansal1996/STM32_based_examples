/*
 * i2c.h
 *
 *  Created on: Jan 26, 2021
 *      Author: Niharika
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32g0xx_hal.h"
#include "stdio.h"
#include "stdlib.h"

class I2CHandler
{
	public:
	void I2CInit(void);
	void I2CWrite_Data(uint8_t slaveAddress, uint8_t devAddress ,uint8_t *data, uint8_t length);
	void I2CRead_Data(uint8_t slaveAddress, uint8_t devAddress ,uint8_t *data, uint8_t length);
};



#endif /* INC_I2C_H_ */
