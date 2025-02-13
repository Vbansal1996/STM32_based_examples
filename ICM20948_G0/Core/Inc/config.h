/**
 * @file config.h
 * @author Vaibhav Bansal
 * @brief Header file for handling device config parameters
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_

#include "GpioHandler.h"
#include "Scheduler.h"
#include "Calibration.h"
#include "icm20948.h"
#include "MotionActivity.h"

#define SD_VALUE	20
#define MOD(x) ((x < 0) ? (x * (-1)) : x)

/**
 * @brief ENUM for handling the Tasks running in the project
 * 
 */
typedef enum Tasks
{
	InstallDevicetaskID,
	CalibrationTaskID,
	MotionActivityTaskID,
	AngleMeasureTaskID,
}Tasks_t;

/**
 * @brief Structure Handling the Global data used in Project
 * 
 */
typedef struct GlobalData
{
	GpioHandler	Gpio;
	SchedulerHandler Schd;
	Calibration	Calb;
	EventHandlers Event;
	ICM20948 icm;
	GpioTypeDef MotionInterrupt;
	GpioTypeDef GreenLed;
	MoitonActivityHandler motion;
}GlobalData_t;

extern GlobalData_t globalData;

#endif /* INC_CONFIG_H_ */
