/**
 * @file uart3.h
 * @author Vaibhav Bansal
 * @brief Module designed to manage the data send and received over UART3 Interface
 * @version 0.1
 * @date 2024-05-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdio.h>

#define START_OF_HEADER     '{'
#define END_OF_PACKET       '}'

/**
 * @brief Typedef function pointer for the uart 1 rx callback funtion
 * 
 */
typedef void (*Uart3_rx_callback_handler) (uint8_t data);

/**
 * @brief Typedef Struct for Uart3 data Handler
 * 
 */
typedef struct Uart3Handler
{
    Uart3_rx_callback_handler callback;
}Uart3Handler_t;

/**
 * @brief API used to init the Uart3 peripheral
 * 
 */
void Peripherals_Uart3_InterfaceInit(void);

/**
 * @brief API used to write data over Uart3 interface
 * 
 * @param data : data buffer
 * @param length : data length
 */
void Peripherals_Uart3_WriteData(uint8_t *data, uint8_t length);

/**
 * @brief API used to register the uart 1 rx callback function
 * 
 * @param Rxcallback 
 */
void Peripherals_Uart3_RegisterUartCallback(Uart3_rx_callback_handler Rxcallback);

/**
 * @brief API used to work as ISR for the Uart3 Interrupt
 * 
 */
void Peripherals_Uart3_RxCallback(void);

#endif