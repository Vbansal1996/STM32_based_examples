/**
 * @file Uart3.c
 * @author Vaibhav Bansal
 * @brief Module designed to manage the data send and received over Uart3 Interface
 * @version 0.1
 * @date 2024-05-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Uart3.h"
#include "main.h"

UART_HandleTypeDef huart3;

Uart3Handler_t Uart3Handler = 
{
    .callback = NULL,
};

/**
 * @brief API used to init the Uart3 peripheral
 * 
 */
void Peripherals_Uart3_InterfaceInit(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);
}

/**
 * @brief API used to write data over Uart3 interface
 * 
 * @param data : data buffer
 * @param length : data length
 */
void Peripherals_Uart3_WriteData(uint8_t *data, uint8_t length)
{
    uint8_t dataBuffer[200];
    dataBuffer[0] = START_OF_HEADER;
    for(int i = 0;i < length;i++)
    {
        dataBuffer[i+1] = data[i];
    }
    dataBuffer[length+1] = END_OF_PACKET;
    for(int i=0;i<length + 2;i++)
    {
        HAL_UART_Transmit(&huart3,&dataBuffer[i],1,1000);
    }
}

/**
 * @brief API used to register the uart 1 rx callback function
 * 
 * @param Rxcallback 
 */
void Peripherals_Uart3_RegisterUartCallback(Uart3_rx_callback_handler Rxcallback)
{
    Uart3Handler.callback = Rxcallback;
}

/**
 * @brief API used to work as ISR for the Uart3 Interrupt
 * 
 */
void Peripherals_Uart3_RxCallback(void)
{
    uint8_t data = 0;
    data = USART3->RDR;
    Uart3Handler.callback(data);
}
