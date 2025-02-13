/**
 * @file MotionActivity.h
 * @author Vaibhav Bansal
 * @brief Header file containing APIs for handing the motion activities
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef INC_MOTIONACTIVITY_H_
#define INC_MOTIONACTIVITY_H_

#include "stm32g0xx_hal.h"

#define MAX_EVENT_COUNT 	60

/**
 * @brief Class containing APIs for Handling the TILT and RPY of IMU
 * 
 */
class MoitonActivityHandler
{
	private:
		float angleX;
		float angleY;
		float angleZ;
		float tiltX;
		float tiltY;
		float tiltZ;
		int8_t maxAngleX;
		int8_t maxAngleY;
		int8_t maxAngleZ;
		uint8_t stateChange = 0;
		uint8_t eventCounter = 0;
		bool angleMeasure = false;
		bool calculateAngleX = false;
		bool calculateAngleY = false;
		bool calculateAngleZ = false;
		uint16_t accelVectorX;
		uint16_t accelVectorY;
		uint16_t accelVectorZ;
		uint8_t accelVectorPositiveCounterX = 0, accelVectorNegativeCounterX = 0,
				accelVectorPositiveCounterY = 0, accelVectorNegativeCounterY = 0,
				accelVectorPositiveCounterZ = 0, accelVectorNegativeCounterZ = 0;
		void KalmanFilterInit();
		float KalmanfiltergetAngle(float newAngle, float newRate, float dt);
        float QAngle = 0.001;                     // Q (ANGLE) unknown uncertainty from the environment
        float QBias = 0.003 ;                    // Q (BIAS) unknown uncertainty from the environment. Here - covariance is degree of correlation between variances of the angle and its error/bias.
        float RMeasure = 0.1;
        float angle = 0.0;
        float bias = 0.0;
        float rate = 0.0;
        float P[2][2]={{0.0,0.0},{0.0,0.0}};
	public:
		void GpioConfig();
		float getCurrentAngleX();
		float getCurrentAngleY();
		float getCurrentAngleZ();
		float getTiltAngleX();
		float getTiltAngleY();
		float getTiltAngleZ();
		void ICMInterruptHandler();
		void ICMMotionHandle();
		void DoAngleMeasurement();
		void resetDeviceState();
};




#endif /* INC_MOTIONACTIVITY_H_ */
