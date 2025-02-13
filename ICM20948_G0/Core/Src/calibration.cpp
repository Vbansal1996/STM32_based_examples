/**
 * @file calibration.cpp
 * @author Vaibhav Bansal
 * @brief Source file containing APIs for handling the IMU Calibration
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "calibration.h"
#include "stdio.h"
#include "icm20948.h"
#include "stdlib.h"
#include "config.h"
#include "math.h"

/**
 * @brief APIs checking if device calibrated or not else put the IMU in calibration mode
 * 
 */
void Calibration::installDevice(void)
{
	if(isCalibrated == true)
		{
			globalData.icm.SetCalibratedOffsets(ax, ay, az, gx, gy, gz);
			globalData.motion.GpioConfig();
		}
	else
		{
			printf("Installing the device\n");
			gpioConfig();
			globalData.icm.MotionInterrupt(disable);
			globalData.icm.DrdyInterrupt(enable);
		}
	return;
}

/**
 * @brief API used for initialising the Interrupt Handler for IMU interrupts
 * 
 */
void Calibration::gpioConfig(void)
{
	globalData.MotionInterrupt.GpioPin = GPIO_PIN_0;
	globalData.MotionInterrupt.GpioMode = GPIO_MODE_IT_RISING;
	globalData.MotionInterrupt.GpioPort = GPIOB;
	globalData.MotionInterrupt.GpioPull = GPIO_NOPULL;
	globalData.MotionInterrupt.callback = &EventHandlers::CalibrationTask;
	globalData.Gpio.registerGpio(&globalData.MotionInterrupt);
}

/**
 * @brief ISR for IMU Interrupts
 * 
 */
void Calibration::CalibrationTask(void)
{
	if(globalData.icm.returnINTStatus() != 0)
		globalData.Schd.taskEnqueue(CalibrationTaskID, Single_Shot, 0);
}

/**
 * @brief Task handler for managing the IMU calibration activity
 * 
 */
void Calibration::CalibrationTaskHandler(void)
{
	globalData.icm.ReadAccData(&ax1[offsetsCounter], &ay1[offsetsCounter], &az1[offsetsCounter]);
	globalData.icm.ReadGyroData(&gx1[offsetsCounter], &gy1[offsetsCounter], &gz1[offsetsCounter]);
	offsetsCounter++;
	if(offsetsCounter < MAX_DATA_OFFSETS)
		{
			return;
		}

	globalData.MotionInterrupt.GpioMode = GPIO_MODE_OUTPUT_PP;
	globalData.Gpio.registerGpio(&globalData.MotionInterrupt);
	globalData.icm.DrdyInterrupt(disable);
	globalData.icm.MotionInterrupt(enable);

	int16_t Avgax, Avgay, Avgaz, Avggx, Avggy, Avggz;

	Avgax = CalculateAvg(ax1, MAX_DATA_OFFSETS);
	Avgay = CalculateAvg(ay1, MAX_DATA_OFFSETS);
	Avgaz = CalculateAvg(az1, MAX_DATA_OFFSETS);
	Avggx = CalculateAvg(gx1, MAX_DATA_OFFSETS);
	Avggy = CalculateAvg(gy1, MAX_DATA_OFFSETS);
	Avggz = CalculateAvg(gz1, MAX_DATA_OFFSETS);

	int16_t SDax, SDay, SDaz, SDgx, SDgy, SDgz;

	SDax = CalculateSD(ax1, Avgax, MAX_DATA_OFFSETS);
	SDay = CalculateSD(ay1, Avgay, MAX_DATA_OFFSETS);
	SDaz = CalculateSD(az1, Avgaz, MAX_DATA_OFFSETS);
	SDgx = CalculateSD(gx1, Avggx, MAX_DATA_OFFSETS);
	SDgy = CalculateSD(gy1, Avggy, MAX_DATA_OFFSETS);
	SDgz = CalculateSD(gz1, Avggz, MAX_DATA_OFFSETS);

	if(SDax <= SD_VALUE && SDay <= SD_VALUE && SDaz <= SD_VALUE
	&& SDgx <= SD_VALUE && SDgy <= SD_VALUE && SDgz <= SD_VALUE)
	{
		isCalibrated = true;
		printf("Device Calibrated\n");
		ax = Avgax;
		ay = Avgay;
		az = Avgaz;
		gx = Avggx;
		gy = Avggy;
		gz = Avggz;
		globalData.Schd.taskEnqueue(InstallDevicetaskID, Single_Shot, 10);
	}
	else
	{
		offsetsCounter = 0;
		globalData.MotionInterrupt.GpioMode = GPIO_MODE_IT_RISING;
		globalData.Gpio.registerGpio(&globalData.MotionInterrupt);
		globalData.icm.MotionInterrupt(disable);
		globalData.icm.DrdyInterrupt(enable);
	}
}

/**
 * @brief API used to calculate and return average for the data received
 * 
 * @param data 
 * @param length 
 * @return int16_t 
 */
int16_t Calibration::CalculateAvg(int16_t *data, uint8_t length)
{
	int32_t avg = 0;
	for(uint8_t i = 0;i < length; i++)
		{
			avg += MOD(data[i]);
		}

	return (int16_t)(avg / length);
}

/**
 * @brief API used to calculate the standard deviation
 * 
 * @param data 
 * @param avg 
 * @param length 
 * @return int16_t 
 */
int16_t Calibration::CalculateSD(int16_t *data, int16_t avg, uint8_t length)
{
	int32_t SDvalue = 0;
	for(uint8_t i = 0; i < length; i++)
		{
			SDvalue += ((MOD(data[i]) - avg) * (MOD(data[i]) - avg));
		}
	return (int16_t)sqrt((SDvalue / length));
}

/**
 * @brief API providing the calibration status of the IMU
 * 
 * @return true 
 * @return false 
 */
bool Calibration::getCalibrationStatus(void)
{
	return isCalibrated;
}
