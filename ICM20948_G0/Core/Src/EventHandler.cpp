/*
 * EventHandler.cpp
 *
 *  Created on: 04-Feb-2021
 *      Author: Vaibhav Bansal
 */

#include "EventHandler.h"
#include "config.h"

void EventHandlers::InstallDevicetask()
{
	globalData.Calb.installDevice();
}

void EventHandlers::CalibrationTaskHandler()
{
	globalData.Calb.CalibrationTaskHandler();
}

void EventHandlers::CalibrationTask()
{
	globalData.Calb.CalibrationTask();
}

void EventHandlers::MotionInterruptHandler(void)
{
	globalData.motion.ICMInterruptHandler();
}

void EventHandlers::MotionInterrupt(void)
{
	globalData.motion.ICMMotionHandle();
}

void EventHandlers::AngleMeasurement(void)
{
	globalData.motion.DoAngleMeasurement();
}
