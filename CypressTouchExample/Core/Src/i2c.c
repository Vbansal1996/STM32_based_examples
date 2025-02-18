/*
 * i2c.c
 *
 *  Created on: Jan 25, 2021
 *      Author: Vaibhav Bansal
 */
#include "i2c.h"
#include "stdio.h"

void InitI2c(void)
{
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.Timing = 0x00303D5B;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
		  printf("I2c Init error\n");
	  }
	  /** Configure Analogue filter
	  */
	  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
		  printf("I2c Init error\n");
	  }
	  /** Configure Digital filter
	  */
	  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	  {
		  printf("I2c Init error\n");
	  }
}

void I2cWrite(uint8_t slaveAddress, uint8_t *data, uint8_t length)
{
	uint8_t err = HAL_I2C_Master_Transmit(&hi2c1, slaveAddress, data, length, 50);
	if(err != HAL_OK)
    	printf("I2C return with error code %d\n",err);

}

void I2cRead(uint8_t slaveAddress, uint8_t *data, uint8_t length)
{
	uint8_t err;
	err = HAL_I2C_Master_Transmit(&hi2c1, slaveAddress, data, 1, 50);
	if(err != HAL_OK)
		printf("I2C return with error code %d\n",err);

	err = HAL_I2C_Master_Receive(&hi2c1, slaveAddress, data, length, 50);
	if(err != HAL_OK)
    	printf("I2C return with error code %d\n",err);
}


