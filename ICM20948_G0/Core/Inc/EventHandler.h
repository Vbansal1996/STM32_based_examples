/**
 * @file EventHandler.h
 * @author Vaibhav Bansal
 * @brief Header file for handling the Event Handling APIs in project
 * @version 0.1
 * @date 2021-11-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef INC_EVENTHANDLER_H_
#define INC_EVENTHANDLER_H_

/**
 * @brief Class storing all the Event Handling APIs
 * 
 */
class EventHandlers
{
	public:
	void InstallDevicetask(void);
	void CalibrationTask(void);
	void CalibrationTaskHandler(void);
	void MotionInterruptHandler(void);
	void MotionInterrupt(void);
	void AngleMeasurement(void);
};


#endif /* INC_EVENTHANDLER_H_ */
