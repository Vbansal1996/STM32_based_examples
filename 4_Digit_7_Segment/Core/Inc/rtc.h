/*
 * rtc.h
 *
 *  Created on: 28-Jan-2021
 *      Author: Vaibhav Bansal
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stm32g0xx_hal.h"
#include "stdio.h"

void InitRtc();
void SetRtcDateTime(uint8_t *rtcTime);
void GetRtcDateTime(uint8_t *rtcTime);
void SetRtcAlarm(uint8_t *AlarmTime, uint8_t WEEKDAY);
void StopRtcAlarm();
void RTC_TAMP_IRQHandler(void);
#endif /* INC_RTC_H_ */
