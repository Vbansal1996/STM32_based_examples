/**
 * @file fota_handler.c
 * @author Vaibhav Bansal
 * @brief Module Designed to manage the FOTA application for the STM32 Micro-controller
 * @version 0.1
 * @date 2024-05-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __FOTA_HANDLER_H__
#define __FOTA_HANDLER_H__

#include <stdint.h>
#include <stdio.h>

/**
 * @brief OTA Buffer Size
 * 
 */
#define OTA_RX_BUFFER_SIZE                  1032U

/**
 * @brief Decryted Packet Buffer size
 * 
 */
#define OTA_DECRYPT_PACKET_BUFFER_SIZE      1024U

typedef enum Fota_States
{
    NO_ACTIVITY,
    INIT_FOTA_HANDLER,
    PROCESS_RX_DATA,
    DECRYPT_RX_DATA,
    STORE_DECRYPT_DATA,
}Fota_States_t;
/**
 * @brief Structure to manage the FOTA parameters
 * 
 */
typedef struct Fota_Handler
{
    uint8_t StartPacketIndex;
    uint8_t PacketID;
    uint8_t PacketSubID;
    uint8_t packetCRC;
    uint8_t Rx_Data_buffer[OTA_RX_BUFFER_SIZE];
    uint16_t rx_buffer_index;
    uint8_t data_rx_complete;
    uint8_t encrypted_data_buffer[OTA_DECRYPT_PACKET_BUFFER_SIZE];
    uint16_t encrypted_buffer_length;
    uint8_t decrypted_buffer_packet[OTA_DECRYPT_PACKET_BUFFER_SIZE];
    uint16_t decrypted_packet_length;
}Fota_Handler_t;
/**
 * @brief API used to enable the FOTA Interface
 * 
 */
void Fota_Handler_InitIterface(void);

/**
 * @brief API used to decrypt the data received over FOTA buffer
 * 
 * @param encData : Encrypted data buffer
 * @param encdataLength : Encrypted data length
 * @param data : Decrypted Buffer
 * @param datalength : Decrypted Buffer length
 * @return uint8_t : status : 0-Fail | 1-Pass
 */
uint8_t Fota_Handler_DecryptReceivedData(uint8_t *encData, uint16_t encdataLength, uint8_t *data, uint16_t *datalength);

/**
 * @brief API used to register as the callback function for the data received
 * 
 * @param data : Data Buffer
 * @param length : Data Buffer Length
 */
void Fota_Handler_ManageRxData(uint8_t data);

/**
 * @brief API used to check whether the data is receive or not for the processing,
 * 
 * @return uint8_t : 0 | 1
 */
uint8_t Fota_Handler_DataRxStatus(void);

/**
 * @brief API used to Process the OTA packet received
 * 
 */
void Fota_Handler_ProcessRxData(void);

/**
 * @brief API used to process the FOTA State machine
 * 
 */
void Fota_Handler_StateMachine(void);

/**
 * @brief API used to update the FOTA state to Init state
 * 
 */
void Fota_Handler_InitFotaState(void);


#endif