/**
 * @file MotionActivity.cpp
 * @author Vaibhav Bansal
 * @brief File containing implement of TILT and RPY calculation through IMU data
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "MotionActivity.h"
#include "config.h"
#include "math.h"

#define ACCELERATION_VECTOR 	150

/**
 * @brief Config GPIOs to motion activity handler
 * 
 */
void MoitonActivityHandler::GpioConfig()
{
	globalData.MotionInterrupt.GpioMode = GPIO_MODE_IT_RISING;
	globalData.MotionInterrupt.callback = &EventHandlers::MotionInterrupt;
	globalData.Gpio.registerGpio(&globalData.MotionInterrupt);
}

/**
 * @brief Get the Tilt Angle X objectR
 * 
 * @return float 
 */
float  MoitonActivityHandler:: getTiltAngleX()
{
	return tiltX;
}

/**
 * @brief Get the Tilt Angle Y object
 * 
 * @return float 
 */
float  MoitonActivityHandler:: getTiltAngleY()
{
	return tiltY;
}

/**
 * @brief Get the Tilt Angle Z object
 * 
 * @return float 
 */
float  MoitonActivityHandler:: getTiltAngleZ()
{
	return tiltZ;
}

/**
 * @brief Get the ROLL from x axis
 * 
 * @return float 
 */
float MoitonActivityHandler::getCurrentAngleX()
{
	return angleX;
}

/**
 * @brief Get the PITCH from y axis
 * 
 * @return float 
 */
float MoitonActivityHandler::getCurrentAngleY()
{
	return angleY;
}

/**
 * @brief Get the YAW from z axis
 * 
 * @return float 
 */
float MoitonActivityHandler::getCurrentAngleZ()
{
	return angleZ;
}

/**
 * @brief ISR for the motion/data ready interrupts
 * 
 */
void MoitonActivityHandler::ICMInterruptHandler()
{
	uint8_t intStatus = globalData.icm.returnINTStatus();

	if(intStatus == 0)
		return;

	if(stateChange == 0)
	{
		stateChange = 1;
		angleX = 0;
		angleY = 0;
		eventCounter = 0;
		angleMeasure = true;
		globalData.icm.MotionInterrupt(disable);
		globalData.icm.DrdyInterrupt(enable);
	}
	else
		globalData.Schd.taskEnqueue(AngleMeasureTaskID, Single_Shot, 1);

}

/**
 * @brief Eventhandler for WOM interrupts
 * 
 */
void MoitonActivityHandler::ICMMotionHandle()
{
	globalData.Schd.taskEnqueue(MotionActivityTaskID, Single_Shot, 10);
}

/**
 * @brief Calculate the RPY and TILT along all axis
 * 
 */
void MoitonActivityHandler::DoAngleMeasurement()
{
	if(eventCounter <= MAX_EVENT_COUNT || angleMeasure == true ||  angleX !=0 || angleY != 0)
	{
		angleMeasure = false;
		float x = 0, y = 0;
		int16_t ax, ay, az, gx, gy, gz;
		globalData.icm.ReadAccData(&ax, &ay, &az);
		globalData.icm.ReadGyroData(&gx, &gy, &gz);

		//printf("Acc Axis data = %d | %d | %d \n", ax, ay, az);
		//printf("Gyro Axis data = %d | %d | %d \n", gx, gy, gz);

		if(MOD(ax) < ACCELERATION_VECTOR)
			ax = 0;
		if(MOD(ay) < ACCELERATION_VECTOR)
			ay = 0;
		if(MOD(az) < ACCELERATION_VECTOR)
			az = 0;

		accelVectorX = MOD(gx); /* Acceleration Magnitude */
		accelVectorY = MOD(gy);
		accelVectorZ = MOD(gz);

		if (accelVectorX > ACCELERATION_VECTOR)
		  {
		    accelVectorPositiveCounterX++;
		    accelVectorNegativeCounterX = 0;
		  }
		else
		  {
		    accelVectorNegativeCounterX++;
		    accelVectorPositiveCounterX = 0;
		  }

		if (accelVectorY > ACCELERATION_VECTOR)
		  {
		    accelVectorPositiveCounterY++;
		    accelVectorNegativeCounterY = 0;
		  }
		else
		  {
		    accelVectorNegativeCounterY++;
		    accelVectorPositiveCounterY = 0;
		  }

		if (accelVectorZ > ACCELERATION_VECTOR)
		  {
		    accelVectorPositiveCounterZ++;
		    accelVectorNegativeCounterZ = 0;
		  }
		else
		  {
		    accelVectorNegativeCounterZ++;
		    accelVectorPositiveCounterZ = 0;
		  }

		if (accelVectorPositiveCounterX > 3)
		  {
		    calculateAngleX = true;
		  }
		else if (accelVectorNegativeCounterX > 3)
		  {
		    calculateAngleX = false;
		  }

		if (accelVectorPositiveCounterY > 3)
		  {
		    calculateAngleY = true;
		  }
		else if (accelVectorNegativeCounterY > 3)
		  {
		    calculateAngleY = false;
		  }

		if (accelVectorPositiveCounterZ > 3)
		  {
		    calculateAngleZ = true;
		  }
		else if (accelVectorNegativeCounterZ > 3)
		  {
		    calculateAngleZ = false;
		  }

		if (calculateAngleX)
			{
				x = ((float)(gx) * 0.01525879);
				y = x * ((float)GYRO_SAMPLING_RATE/(float)1125);
			}

		angleX += y;    /* Angle in X axis */

		if (calculateAngleY)
			{
				x = ((float)(gy) * 0.01525879);
				y = x * ((float)GYRO_SAMPLING_RATE/(float)1125);
			}

		angleY += y;    /* Angle in Y axis */

		if (calculateAngleZ)
			{
				x = ((float)(gz) * 0.01525879);
				y = x * ((float)GYRO_SAMPLING_RATE/(float)1125);
			}

		angleZ += y;    /* Angle in Z axis */

		if ((MOD(angleX) < 2)  &&  (MOD(angleX) < maxAngleX) && (maxAngleX > 5))
		  {
			angleX = 0;
		  }
		if (maxAngleX < MOD(angleX))
		  {
		    maxAngleX = MOD(angleX);
		  }

		if ((MOD(angleY) < 2)  &&  (MOD(angleY) < maxAngleY) && (maxAngleY > 5))
		  {
			angleY = 0;
		  }
		if (maxAngleY < MOD(angleY))
		  {
		    maxAngleY = MOD(angleY);
		  }

		if ((MOD(angleZ) < 2)  &&  (MOD(angleZ) < maxAngleZ) && (maxAngleZ > 5))
		  {
			angleZ = 0;
		  }
		if (maxAngleZ < MOD(angleZ))
		  {
		    maxAngleZ = MOD(angleZ);
		  }

		tiltX = atan( ax / (sqrt(pow(ay,2) + pow(az,2))));
		tiltY = atan( ay / (sqrt(pow(ax,2) + pow(az,2))));
		tiltZ = atan( az / (sqrt(pow(ay,2) + pow(ax,2))));


		printf("Current angle value along x axis = %f\n", angleX);
		printf("Current angle value along y axis = %f\n", angleY);
		printf("Current angle value along z axis = %f\n", angleZ);

		printf("Current tilt value along x axis = %f\n", tiltX);
		printf("Current tilt value along y axis = %f\n", tiltY);
		printf("Current tilt value along z axis = %f\n", tiltZ);
	}
	else
	{
		printf("Max Angle achieved X axis = %d\n", maxAngleX);
		printf("Max Angle achieved Y axis = %d\n", maxAngleY);
		printf("Max Angle achieved Z axis = %d\n", maxAngleZ);
		maxAngleX = 0;
		maxAngleY = 0;
		maxAngleZ = 0;
		resetDeviceState();
	}
	eventCounter++;
}

/**
 * @brief Reset all the parameters to default state
 * 
 */
void MoitonActivityHandler::resetDeviceState()
{
	angleX = 0;
	angleY = 0;
	angleZ = 0;
	eventCounter = 0;
	angleMeasure = false;
	stateChange = 0;
	globalData.icm.returnINTStatus();
	globalData.icm.MotionInterrupt(enable);
	globalData.icm.DrdyInterrupt(disable);
}

/**
 * @brief Basic implementation of Kalman Filter
 * 
 * @param newAngle 
 * @param newRate 
 * @param dt 
 * @return float 
 */
float MoitonActivityHandler::KalmanfiltergetAngle(float newAngle, float newRate, float dt)
{
	angle = 0.0;
    rate = newRate - bias;    //new_rate is the latest Gyro measurement
    angle += dt * rate;

    //Step 2: Predict new uncertainty (or covariance) from old uncertainity and unknown uncertainty from the environment.

    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + QAngle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += QBias * dt;

   	//Step 3: Innovation i.e. predict th next measurement

    float y = newAngle - angle;

    //Step 4: Innovation covariance i.e. error in prediction

    float s = P[0][0] + RMeasure;

    //Step 5:  Calculate Kalman Gain

    float K[2]={0.0,0.0};
    K[0] = P[0][0]/s;
    K[1] = P[1][0]/s;

   //Step 6: Update the Angle

    angle += K[0] * y;
    bias  += K[1] * y;

   //Step 7: Calculate estimation error covariance - Update the error covariance

    float P00Temp = P[0][0];
    float P01Temp = P[0][1];

    P[0][0] -= K[0] * P00Temp;
    P[0][1] -= K[0] * P01Temp;
    P[1][0] -= K[1] * P00Temp;
    P[1][1] -= K[1] * P01Temp;

    return angle;

}

