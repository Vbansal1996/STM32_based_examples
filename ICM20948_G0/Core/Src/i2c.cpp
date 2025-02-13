/*
 * i2c.cpp
 *
 *  Created on: Jan 26, 2021
 *      Author: Vaibhav Bansal
 */

#include "i2c.h"
#include "string.h"

I2C_HandleTypeDef hi2c1;

void I2CHandler::I2CInit()
{
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.Timing = 0x0010061A;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
		  printf("I2c driver init error\r\n");
	  }
	  /** Configure Analogue filter
	  */
	  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
		  printf("I2c driver analog filter init error\r\n");
	  }
	  /** Configure Digital filter
	  */
	  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	  {
		  printf("I2c driver digital filter init error\r\n");
	  }
	  /* USER CODE BEGIN I2C1_Init 2 */
}

void I2CHandler::I2CRead_Data(uint8_t slaveAddress, uint8_t regAddress ,uint8_t *data, uint8_t length)
{
	uint8_t err;
	err = HAL_I2C_Master_Transmit(&hi2c1, (slaveAddress | 0x00), &regAddress, 1, 50);
	if(err != HAL_OK)
		printf("I2c return with error code %d\n",err);
	err = HAL_I2C_Master_Receive(&hi2c1, (slaveAddress | 0x01), data, length, 50);
	if(err != HAL_OK)
		printf("I2c return with error code %d\n",err);
}

void I2CHandler::I2CWrite_Data(uint8_t slaveAddress, uint8_t regAddress ,uint8_t *data, uint8_t length)
{
	uint8_t err;
	uint8_t dataToSend[length+1];
	dataToSend[0] = regAddress;
	memcpy(dataToSend+1, data, length);
	err = HAL_I2C_Master_Transmit(&hi2c1, (slaveAddress | 0x00), dataToSend, length+1, 50);
	if(err != HAL_OK)
			printf("I2c return with error code %d\n",err);
}
