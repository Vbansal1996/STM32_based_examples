/*
 * Scheduler.c
 *
 *  Created on: 31-Jan-2021
 *      Author: Vaibhav Bansal
 */

#include "Scheduler.h"
#include "stdio.h"
#include "config.h"

#define MAX_TASKS		20

TaskScheduler taskScheduler[MAX_TASKS];
uint8_t taskCounter;
uint32_t currentTickCount;


extern "C" void PendSV_Handler(void)
{
	uint32_t *pICSR = (uint32_t *)0xE000ED04;
	*pICSR &= ~(1 << 28);
	for(uint8_t i = 0; i < taskCounter; i++)
	{
		if(taskScheduler[i].taskState == running && taskScheduler[i].taskTimeoutInMs <= currentTickCount)
		{
			((taskScheduler[i].*(taskScheduler[i].callback))());
			taskScheduler[i].taskState = executed;
			if(taskScheduler[i].taskType == Periodic)
			{
				taskScheduler[i].taskState = running;
				taskScheduler[i].taskTimeoutInMs = currentTickCount + taskScheduler[i].tasktime;
			}
			return;
		}
	}
}


/**
  * @brief This function handles System tick timer.
  */
extern "C" void SysTick_Handler(void)
{
	currentTickCount++;
	if(currentTickCount == 0xFFFFFFFF)
		{
			currentTickCount = 0;
			for(uint8_t i = 0;i < taskCounter; i++)
			{
				taskScheduler[i].taskTimeoutInMs = currentTickCount + taskScheduler[i].tasktime;
			}
		}
	uint32_t *pICSR = (uint32_t *)0xE000ED04;
	*pICSR = 1 << 28;
}


void SchedulerHandler::registerTask(uint32_t taskId, void  (EventHandlers::*callback)(void))
{
	taskScheduler[taskCounter].taskId = taskId;
	taskScheduler[taskCounter].callback = callback;
	taskScheduler[taskCounter].taskState = on_halt;
	taskCounter++;
}

void SchedulerHandler::deleteTask(uint32_t taskId)
{
	for(uint8_t i = 0; i < taskCounter; i++)
		{
			if(taskScheduler[i].taskId == taskId)
			{
				for(uint8_t j = i; j < (taskCounter - 1); j++)
				{
					taskScheduler[j] = taskScheduler[j+1];
				}
			}
			taskCounter--;
		}
}

uint8_t SchedulerHandler::ShowTaskCount(void)
{
	printf("No Of task Registered = %d\n",taskCounter);
	return taskCounter;
}

void SchedulerHandler::taskEnqueue(uint32_t taskId, TaskType_t taskType, uint32_t taskTimeoutInMs)
{
	for(uint8_t i = 0;i < taskCounter; i++)
	{
		if(taskScheduler[i].taskId == taskId)
			{
				taskScheduler[i].tasktime = taskTimeoutInMs;
				taskScheduler[i].taskTimeoutInMs = currentTickCount + taskTimeoutInMs;
				taskScheduler[i].taskState = running;
				taskScheduler[i].taskType = taskType;
				return;
			}
	}
	printf("Invalid Task ID\n");
}

void SchedulerHandler::taskStop(uint32_t taskId)
{
	for(uint8_t i = 0;i < taskCounter; i++)
	{
		if(taskScheduler[i].taskId == taskId)
			{
				taskScheduler[i].taskState = on_halt;
				return;
			}
	}
	printf("Invalid Task ID\n");
}

void SchedulerHandler::changeTaskTimeout(uint32_t taskId, uint32_t taskTimeout)
{
	for(uint8_t i = 0;i < taskCounter; i++)
	{
		if(taskScheduler[i].taskId == taskId)
			{
				taskScheduler[i].tasktime = taskTimeout;
				taskScheduler[i].taskTimeoutInMs = currentTickCount + taskTimeout;
				return;
			}
	}
	printf("Invalid Task ID\n");
}

void SchedulerHandler::TaskSchedulerInit(void)
{
	uint32_t *pSYSCTRL	= (uint32_t *)0xE000E010;
	uint32_t *pSYSRVR	= (uint32_t *)0xE000E014;
	uint32_t countValue = (16000000 / 1000) - 1;
	*pSYSRVR &= ~(0x0FFFFFFF);
	*pSYSRVR |= countValue;
	*pSYSCTRL |= (1 << 2 | 1 << 1 | 1 << 0);
}

