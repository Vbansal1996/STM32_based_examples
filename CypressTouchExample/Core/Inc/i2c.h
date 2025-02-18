/*
 * i2c.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Niharika
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "stm32g0xx_hal.h"

I2C_HandleTypeDef hi2c1;

void InitI2c(void);
void I2cWrite(uint8_t slaveAddress, uint8_t *data, uint8_t length);
void I2cRead(uint8_t slaveAddress, uint8_t *data, uint8_t length);

#endif /* SRC_I2C_H_ */
