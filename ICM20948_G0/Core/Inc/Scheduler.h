/*
 * Scheduler.h
 *
 *  Created on: 31-Jan-2021
 *      Author: Vaibhav Bansal
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "stm32g0xx_hal.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "EventHandler.h"

typedef enum TaskType
{
	Periodic,
	Single_Shot,
}TaskType_t;

typedef enum TaskState
{
	on_halt,
	running,
	executed,
}TaskState_t;

class TaskScheduler:public EventHandlers
{
	public:
		uint32_t taskId;
		uint32_t taskTimeoutInMs;
		TaskState_t taskState;
		TaskType_t taskType;
		void (EventHandlers::*callback)(void);
		uint32_t tasktime;
};

class SchedulerHandler
{
	public:
		void TaskSchedulerInit(void);
		void registerTask(uint32_t taskId, void  (EventHandlers::*callback)(void));
		void deleteTask(uint32_t taskId);
		void taskEnqueue(uint32_t taskId, TaskType_t taskType, uint32_t taskTimeoutInMs);
		void taskStop(uint32_t taskId);
		void changeTaskTimeout(uint32_t taskId, uint32_t taskTimeout);
		uint8_t ShowTaskCount(void);
};

#endif /* INC_SCHEDULER_H_ */
