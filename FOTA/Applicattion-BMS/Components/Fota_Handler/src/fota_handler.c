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

#include "fota_handler.h"
#include "cmox_crypto.h"
#include "uart3.h"
#include "y-modem.h"
#include "flash_if.h"

/**
 * @brief Structure to manage the FOTA parameters Default Initialisation
 * 
 */
Fota_Handler_t fota_Hander = 
{
    .StartPacketIndex = 0,
    .PacketID = 0,
    .PacketSubID = 255,
    .packetCRC = 0,
    .rx_buffer_index = 0,
    .decrypted_packet_length = 0,
    .encrypted_buffer_length = 0,
};

/**
 * @brief Default Initialisation of OTA state.
 * 
 */
Fota_States_t fota_routine = NO_ACTIVITY;

/**
 * @brief Decryption Key, same key will be used for decrypion
 * 
 */
const uint8_t Key[] =
{
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
/**
 * @brief IV key for Decryption and Encryption
 * 
 */
const uint8_t IV[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

/**
 * @brief API used to enable the FOTA Interface
 * 
 */
void Fota_Handler_InitIterface(void)
{
    Peripherals_Uart3_RegisterUartCallback(Fota_Handler_ManageRxData);
    FLASH_If_Init();
    if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
    {
        Error_Handler();
    }
    uint8_t dataToSend = C_ACK;
    Peripherals_Uart3_WriteData(&dataToSend,1);
}

/**
 * @brief API used to decrypt the data received over FOTA buffer
 * 
 * @param encData : Encrypted data buffer
 * @param encdataLength : Encrypted data length
 * @param data : Decrypted Buffer
 * @param datalength : Decrypted Buffer length
 * @return uint8_t : status : 0-Fail | 1-Pass
 */
uint8_t Fota_Handler_DecryptReceivedData(uint8_t *encData, uint16_t encdataLength, uint8_t *data, uint16_t *datalength)
{
    cmox_cipher_retval_t retval;
    size_t decData_size = 1024;
    uint8_t ret = 0;
    retval = cmox_cipher_decrypt(CMOX_AES_CBC_DEC_ALGO,                 /* Use AES CBC algorithm */
                               encData, encdataLength, /* Ciphertext to decrypt */
                               Key, sizeof(Key),                      /* AES key to use */
                               IV, sizeof(IV),                        /* Initialization vector */
                               data, &decData_size);   /* Data buffer to receive generated plaintext */

    if (retval != CMOX_CIPHER_SUCCESS)
    {
        ret = 1;
    }
    *datalength = (uint16_t)decData_size;

    return ret;
}

/**
 * @brief API used to register as the callback function for the data received
 * 
 * @param data : Data Buffer
 * @param length : Data Buffer Length
 */
void Fota_Handler_ManageRxData(uint8_t data)
{
    fota_Hander.Rx_Data_buffer[fota_Hander.rx_buffer_index++] = data;

    if(fota_Hander.Rx_Data_buffer[0] == SOH && fota_Hander.rx_buffer_index == INIT_PACKET)
    {
        fota_Hander.data_rx_complete = 1;
    }
    else if(fota_Hander.Rx_Data_buffer[0] == STX && fota_Hander.rx_buffer_index == DATA_PACKET)
    {
        fota_Hander.data_rx_complete = 1;
    }
    else if(fota_Hander.Rx_Data_buffer[0] == EOT)
    {
        fota_Hander.data_rx_complete = 1;
    }
}

/**
 * @brief API used to check whether the data is receive or not for the processing,
 * 
 * @return uint8_t : 0 | 1
 */
uint8_t Fota_Handler_DataRxStatus(void)
{
    return fota_Hander.data_rx_complete;
}

/**
 * @brief API used to Process the OTA packet received
 * 
 */
void Fota_Handler_ProcessRxData(void)
{
    if(fota_Hander.Rx_Data_buffer[0] == SOH)
    {
        if((fota_Hander.PacketID == fota_Hander.Rx_Data_buffer[1]) && (fota_Hander.Rx_Data_buffer[2] == fota_Hander.PacketSubID))
        {
            Components_YModem_CheckFileName(fota_Hander.Rx_Data_buffer + 3,fota_Hander.rx_buffer_index - 3);
            fota_Hander.PacketID++;
            fota_Hander.PacketSubID--;
        }
        fota_Hander.rx_buffer_index = 0;
    }
    else if(fota_Hander.Rx_Data_buffer[0] == STX)
    {
        if(fota_Hander.Rx_Data_buffer[1] == fota_Hander.PacketID && fota_Hander.Rx_Data_buffer[2] == fota_Hander.PacketSubID)
        {
            fota_Hander.PacketID++;
            fota_Hander.PacketSubID--;
            uint8_t rxd_crc = fota_Hander.Rx_Data_buffer[fota_Hander.rx_buffer_index - 1];
            uint8_t calCRC = 0;
            calCRC = Components_YModem_CalculateCRC(fota_Hander.Rx_Data_buffer + 3,1024);
            if(rxd_crc == calCRC)
            {
                for(int i = 0;i < OTA_DECRYPT_PACKET_BUFFER_SIZE;i++)
                {
                    fota_Hander.encrypted_data_buffer[i] = fota_Hander.Rx_Data_buffer[i+3];
                }
                fota_Hander.encrypted_buffer_length = OTA_DECRYPT_PACKET_BUFFER_SIZE;
                fota_Hander.decrypted_packet_length = OTA_DECRYPT_PACKET_BUFFER_SIZE;
                fota_routine = DECRYPT_RX_DATA;
            }
            else
            {
                uint8_t dataToSend = NACK;
                Peripherals_Uart3_WriteData(&dataToSend,1);
                fota_routine = PROCESS_RX_DATA;
            }
        }
        fota_Hander.rx_buffer_index = 0;
    }
    else if(fota_Hander.Rx_Data_buffer[0] == EOT)
    {
        uint8_t dataToSend = ACK;
        Peripherals_Uart3_WriteData(&dataToSend,1);
        dataToSend = C_ACK;
        Peripherals_Uart3_WriteData(&dataToSend,1);
        Components_YModem_EndOfFileTransfer();
        fota_routine = NO_ACTIVITY;
        fota_Hander.rx_buffer_index = 0;
    }
}

/**
 * @brief API used to process the FOTA State machine
 * 
 */
void Fota_Handler_StateMachine(void)
{
    switch(fota_routine)
    {
        case NO_ACTIVITY:
        //This is no activity state, OTA process will wait for the command
        break;

        case INIT_FOTA_HANDLER:
        {
            Fota_Handler_InitIterface();
            fota_routine = PROCESS_RX_DATA;
        }
        break;
        
        case PROCESS_RX_DATA:
        {
            if(Fota_Handler_DataRxStatus() == 1)
            {
                fota_Hander.data_rx_complete = 0;
                Fota_Handler_ProcessRxData();
            }
        }
        break;
        
        case DECRYPT_RX_DATA:
        {
            if(Fota_Handler_DecryptReceivedData(fota_Hander.encrypted_data_buffer,fota_Hander.encrypted_buffer_length,fota_Hander.decrypted_buffer_packet,&fota_Hander.decrypted_packet_length) != 1)
            {
                fota_routine = STORE_DECRYPT_DATA;
            }
            else
            {
                uint8_t dataToSend = NACK;
                Peripherals_Uart3_WriteData(&dataToSend,1);
                fota_routine = PROCESS_RX_DATA;
            }
        }
        break;
        
        case STORE_DECRYPT_DATA:
        {
            if(Components_YModem_StoreDataInFlash(fota_Hander.decrypted_buffer_packet,fota_Hander.decrypted_packet_length) == true)
            {
                uint8_t dataToSend = ACK;
                Peripherals_Uart3_WriteData(&dataToSend,1);
            }
            else
            {
                uint8_t dataToSend = NACK;
                Peripherals_Uart3_WriteData(&dataToSend,1);
            }
            fota_routine = PROCESS_RX_DATA;
        }
        break;
    }
}

/**
 * @brief API used to update the FOTA state to Init state
 * 
 */
void Fota_Handler_InitFotaState(void)
{
    fota_routine = INIT_FOTA_HANDLER;
}
