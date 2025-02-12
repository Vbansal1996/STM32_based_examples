/**
 * @file y-modem.c
 * @author Vaibhav Bansal
 * @brief Module designed to managed the Y-Modem Protocol
 * @version 0.1
 * @date 2024-05-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "y-modem.h"
#include "uart3.h"
#include "flash_if.h"

Y_Modem_Handler_t YModem_Handler = 
{
    .FlashEndIndex = USER_FLASH_END_ADDRESS,
    .flashOffset = 0x00000000,
    .FlashStartIndex = APPLICATION_ADDRESS,
    .fota_file_length = 0x000000
};

/**
 * @brief API used to validate the file name received as per the YModem Protocol
 * 
 * @param fileName : File Name Buffer
 * @param fileName_Length : File Name Length
 */
void Components_YModem_CheckFileName(uint8_t *fileName, uint8_t fileName_Length)
{
    YModem_Handler.fota_file_length = fileName[0] << 24 | fileName[1] << 16 | fileName[2] << 8 | fileName[3];

    if(YModem_Handler.fota_file_length > OTA_FILE_MAX_SIZE)
    {
        uint8_t dataToSend = NACK;
        Peripherals_Uart3_WriteData(&dataToSend,1);
    }
    else
    {
        FLASH_If_Erase(YModem_Handler.FlashStartIndex);
        uint8_t dataToSend = ACK;
        Peripherals_Uart3_WriteData(&dataToSend,1);
    }
}

/**
 * @brief API used to store the decrypted file data into flash memory
 * 
 * @param data : Decrypted Data Buffer
 * @param dataLength : Data Buffer Length
 * @return : true/false
 */
bool Components_YModem_StoreDataInFlash(uint8_t *data, uint16_t dataLength)
{
    if(YModem_Handler.flashOffset < YModem_Handler.fota_file_length)
    {
        if(FLASH_If_Write(YModem_Handler.FlashStartIndex + YModem_Handler.flashOffset , data, dataLength) != FLASHIF_OK)
        {
            return false;
        }
        else
        {
            YModem_Handler.flashOffset += dataLength;
            return true;
        }  
    }
    else
    {
        return false;
    }
}

/**
 * @brief API designed to calculate the CRC of the data received
 * 
 * @param data : data buffer
 * @param dataLength : data buffer length
 * @return uint8_t : CRC value
 */
uint8_t Components_YModem_CalculateCRC(uint8_t *data, uint16_t dataLength)
{
    uint8_t crc = 0;
    for(int i = 0; i < dataLength;i++)
    {
        crc = crc + data[i];
        if((i % 64) == 0)
        {
        	continue;
        }
    }
    return crc;
}

/**
 * @brief API used to manage the EOT received 
 * 
 */
void Components_YModem_EndOfFileTransfer(void)
{
    //todo: write data in Flash Directly
    //todo: Check if data is updated in EEPROM or not
    //todo: Reset the device using NVIC in boot mode
}
