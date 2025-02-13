/**
 * @file calibration.h
 * @author Vaibhav Bansal
 * @brief Header file for APIs Handling IMU Calibration Activity
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef INC_CALIBRATION_H_
#define INC_CALIBRATION_H_

#include "stm32g0xx_hal.h"

#define MAX_DATA_OFFSETS 50

/**
 * @brief Class containing APIs for handling the IMU Calibration Activity
 * 
 */
class Calibration
{
	private:
		int16_t ax, ay, az, gx, gy, gz;
		int16_t ax1[MAX_DATA_OFFSETS], ay1[MAX_DATA_OFFSETS], az1[MAX_DATA_OFFSETS], gx1[MAX_DATA_OFFSETS], gy1[MAX_DATA_OFFSETS], gz1[MAX_DATA_OFFSETS];
		uint8_t offsetsCounter = 0;
		bool isCalibrated = false;
		void gpioConfig(void);
		int16_t CalculateAvg(int16_t *data, uint8_t length);
		int16_t CalculateSD(int16_t *data, int16_t avg, uint8_t length);
	public:
		void installDevice(void);
		void CalibrationTaskHandler(void);
		void CalibrationTask(void);
		bool getCalibrationStatus(void);
};


#endif /* INC_CALIBRATION_H_ */
