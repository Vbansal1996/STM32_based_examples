/**
 * @file y-modem.h
 * @author Vaibhav Bansal
 * @brief Module designed to managed the Y-Modem Protocol
 * @version 0.1
 * @date 2024-05-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __Y_MODEM_H__
#define __Y_MODEM_H__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* Y_Modem Protocol
            <soh> 01H
            <eot> 04H
            <ack> 06H
            <nak> 15H
            <can> 18H
            <C>   43H
            "sending in batch mode etc."
                                                      C (command:rb)
            <SOH 00 FF foo.c NUL[1019] CRC CRC>
                                                      ACK
                                                      C
            <SOH 01 FE Data[1024] CRC CRC>
                                                      ACK
            <SOH 02 FC Data[1024] CRC CRC>
                                                      ACK
            <SOH 03 FB Data[1024] CRC CRC>
                                                      ACK
            <EOT>
                                                      NAK
            <EOT>
                                                      ACK
                                                      C
*/
#define OTA_FILE_MAX_SIZE           (180U * 1024U)

#define INIT_PACKET                 132U
#define DATA_PACKET                 1024U

#define ACK                         0x06U
#define NACK                        0x15U
#define SOH                         0x01U
#define STX                         0x02U
#define EOT                         0x04U
#define CAN                         0x18U
#define C_ACK                       0x43U

/**
 * @brief API designed to manage the OTA file transfer using y-modem protocol
 * 
 */
typedef struct Y_Modem_Handler
{
    uint32_t fota_file_length;
    uint32_t FlashStartIndex;
    uint32_t FlashEndIndex;
    uint32_t flashOffset;
}Y_Modem_Handler_t;

/**
 * @brief API used to validate the file name received as per the YModem Protocol
 * 
 * @param fileName : File Name Buffer
 * @param fileName_Length : File Name Length
 */
void Components_YModem_CheckFileName(uint8_t *fileName, uint8_t fileName_Length);

/**
 * @brief API used to store the decrypted file data into flash memory
 * 
 * @param data : Decrypted Data Buffer
 * @param dataLength : Data Buffer Length
 * @return : true/false
 */
bool Components_YModem_StoreDataInFlash(uint8_t *data, uint16_t dataLength);

/**
 * @brief API designed to calculate the CRC of the data received
 * 
 * @param data : data buffer
 * @param dataLength : data buffer length
 * @return uint8_t : CRC value
 */
uint8_t Components_YModem_CalculateCRC(uint8_t *data, uint16_t dataLength);

/**
 * @brief API used to manage the EOT received 
 * 
 */
void Components_YModem_EndOfFileTransfer(void);

#endif