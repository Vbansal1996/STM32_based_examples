/*****************************************************************************
* File Name: CY8CMBR3xxx_APIs.c
*
* Version 1.00
*
* Description:
*   This file contains the definitions of all the high-level APIs.
*
* Note:
*   N/A
*
* Owner:
*   SRVS
*
* Related Document:
*   MBR3 Design Guide
*   MBR3 Device Register TRM
*   MBR3 Device Datasheet
*
* Hardware Dependency:
*   N/A
*
* Code Tested With:
*   PSoC Creator 3.0 CP7
*   CY3280-MBR3 Evaluation Kit
*   CY8CKIT-042 Pioneer Kit
*
******************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation.
******************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress) and is
* protected by and subject to worldwide patent protection (United States and
* foreign), United States copyright laws and international treaty provisions.
* Cypress hereby grants to licensee a personal, non-exclusive, non-transferable
* license to copy, use, modify, create derivative works of, and compile the
* Cypress Source Code and derivative works for the sole purpose of creating
* custom software in support of licensee product to be used only in conjunction
* with a Cypress integrated circuit as specified in the applicable agreement.
* Any reproduction, modification, translation, compilation, or representation of
* this software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the
* materials described herein. Cypress does not assume any liability arising out
* of the application or use of any product or circuit described herein. Cypress
* does not authorize its products for use as critical components in life-support
* systems where a malfunction or failure may reasonably be expected to result in
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of
* such use and in doing so indemnifies Cypress against all charges. Use may be
* limited by and subject to the applicable Cypress software license agreement.
*****************************************************************************/

/*******************************************************************************
* Included headers
*******************************************************************************/
#include "CY8CMBR3xxx_APIs.h"
#include "CY8CMBR3xxx_Device.h"
#include "CY8CMBR3xxx_Registers.h"
#include "CY8CMBR3xxx_CommandsAndConfig.h"
#include "CY8CMBR3xxx_HostFunctions.h"


/*******************************************************************************
* Address Validation Macro
*******************************************************************************/
#define CY8CMBR3xxx_SLAVE_ADDRESS_MIN                            (0x08)
#define CY8CMBR3xxx_SLAVE_ADDRESS_MAX                            (0x77)
#define CY8CMBR3xxx_IS_VALID_ADDRESS(address)                    ((slaveAddress >= CY8CMBR3xxx_SLAVE_ADDRESS_MIN) && (slaveAddress <= CY8CMBR3xxx_SLAVE_ADDRESS_MAX))


/*******************************************************************************
* Critical Delay Constants
*******************************************************************************/
#define CY8CMBR3xxx_DELAY_SAVE_CHECK_CRC_CMD                     (250)
#define CY8CMBR3xxx_DELAY_SOFTWARE_RESET_CMD                     (50)


/*******************************************************************************
* API Constants
*******************************************************************************/
#define CY8CMBR3xxx_COMMAND_STATUS_BUSY                          (0)
#define CY8CMBR3xxx_COMMAND_STATUS_SUCCESS                       (1)
#define CY8CMBR3xxx_COMMAND_STATUS_FAILED                        (2)
#define CY8CMBR3xxx_REG_ADDRESS_LENGTH                           (1)
#define CY8CMBR3xxx_CONFIG_DATA_LENGTH_WITH_CRC_AND_ADDR         (129)
#define CY8CMBR3xxx_MAX_DEVICE_REGISTERS                         (252)
#define CY8CMBR3xxx_COMMAND_NO_ERROR                             (0)
#define CY8CMBR3xxx_REG_ADDRESS_OFFSET                           (0)
#define CY8CMBR3xxx_WRITE_DATA_LSB_OFFSET                        (1)
#define CY8CMBR3xxx_WRITE_DATA_MSB_OFFSET                        (2)
#define CY8CMBR3xxx_BYTE_WRITE_LENGTH                            (2)
#define CY8CMBR3xxx_BYTE_READ_LENGTH                             (1)
#define CY8CMBR3xxx_DUAL_BYTE_WRITE_LENGTH                       (3)
#define CY8CMBR3xxx_DUAL_BYTE_READ_LENGTH                        (2)
#define CY8CMBR3xxx_READ_DATA_LSB_OFFSET                         (0)
#define CY8CMBR3xxx_READ_DATA_MSB_OFFSET                         (1)
#define CY8CMBR3xxx_CONFIG_DATA_LENGTH_WITH_CRC                  (128)
#define CY8CMBR3xxx_COMMAND_SEND_LENGTH                          (2)
#define CY8CMBR3xxx_COMMAND_OFFSET                               (1)
#define CY8CMBR3xxx_COMMAND_STATUS_LENGTH                        (4)
#define CY8CMBR3xxx_CMD_STAT_CTRL_CMD_OFFSET                     (0)
#define CY8CMBR3xxx_CMD_STAT_CTRL_CMD_STATUS_OFFSET              (2)
#define CY8CMBR3xxx_CMD_STAT_CTRL_CMD_ERR_OFFSET                 (3)
#define CY8CMBR3xxx_CONFIG_DATA_WRITE_OFFSET                     (1)
#define CY8CMBR3xxx_VERIFY_DEVICE_READ_BUFFER_LENGTH             (5)
#define CY8CMBR3xxx_VERIFY_DEVICE_FAMILYID_OFFSET                (0)
#define CY8CMBR3xxx_VERIFY_DEVICE_DEVID_LSB_OFFSET               (1)
#define CY8CMBR3xxx_VERIFY_DEVICE_DEVID_MSB_OFFSET               (2)
#define CY8CMBR3xxx_VERIFY_DEVICE_DEVREV_LSB_OFFSET              (3)
#define CY8CMBR3xxx_VERIFY_DEVICE_DEVREV_MSB_OFFSET              (4)
#define CY8CMBR3xxx_SENSORID_SEND_LENGTH                         (2)
#define CY8CMBR3xxx_SENSORID_OFFSET                              (1)
#define CY8CMBR3xxx_SENSOR_DATA_LENGTH                           (13)
#define CY8CMBR3xxx_SENSOR_DATA_SYNC1_OFFSET                     (0)
#define CY8CMBR3xxx_SENSOR_DATA_SENSOR_ID_OFFSET                 (1)
#define CY8CMBR3xxx_SENSOR_DATA_AVG_CNT_LSB_OFFSET               (9)
#define CY8CMBR3xxx_SENSOR_DATA_AVG_CNT_MSB_OFFSET               (10)
#define CY8CMBR3xxx_SENSOR_DATA_BASELINE_LSB_OFFSET              (5)
#define CY8CMBR3xxx_SENSOR_DATA_BASELINE_MSB_OFFSET              (6)
#define CY8CMBR3xxx_SENSOR_DATA_CP_OFFSET                        (2)
#define CY8CMBR3xxx_SENSOR_DATA_DIFFCOUNTS_LSB_OFFSET            (3)
#define CY8CMBR3xxx_SENSOR_DATA_DIFFCOUNTS_MSB_OFFSET            (4)
#define CY8CMBR3xxx_SENSOR_DATA_RAWCOUNTS_LSB_OFFSET             (7)
#define CY8CMBR3xxx_SENSOR_DATA_RAWCOUNTS_MSB_OFFSET             (8)
#define CY8CMBR3xxx_SENSOR_DATA_SYNC2_OFFSET                     (12)
#define CY8CMBR3xxx_DIFFCOUNT_DATA_LENGTH                        (35)
#define CY8CMBR3xxx_DIFFCOUNT_SYNC0_OFFSET                       (0)
#define CY8CMBR3xxx_DIFFCOUNT_SYNC1_OFFSET                       (34)
#define CY8CMBR3xxx_DIFFCOUNT_WIDTH                              (2)
#define CY8CMBR3xxx_DIFFCOUNT_LSB_OFFSET                         (1)
#define CY8CMBR3xxx_DIFFCOUNT_MSB_OFFSET                         (2)
#define CY8CMBR3xxx_SENSOR_STATUS_LENGTH                         (10)
#define CY8CMBR3xxx_BUTTONSTATUS_LSB_OFFSET                      (0)
#define CY8CMBR3xxx_BUTTONSTATUS_MSB_OFFSET                      (1)
#define CY8CMBR3xxx_LATCHEDBUTTONSTATUS_LSB_OFFSET               (2)
#define CY8CMBR3xxx_LATCHEDBUTTONSTATUS_MSB_OFFSET               (3)
#define CY8CMBR3xxx_PROXSTATUS_OFFSET                            (4)
#define CY8CMBR3xxx_LATCHEDPROXSTATUS_OFFSET                     (5)
#define CY8CMBR3xxx_SLIDER1POSITION_OFFSET                       (6)
#define CY8CMBR3xxx_LIFTOFFSLIDER1POSITION_OFFSET                (7)
#define CY8CMBR3xxx_SLIDER2POSITION_OFFSET                       (8)
#define CY8CMBR3xxx_LIFTOFFSLIDER2POSITION_OFFSET                (9)
#define CY8CMBR3xxx_BUFFER_INIT_VALUE                            (0x00)
#define CY8CMBR3xxx_CTRL_CMD_NO_ERROR                            (0)
#define CY8CMBR3xxx_DEFAULT_SENSOR_ID                            (0)
#define CY8CMBR3xxx_DEFAULT_CONFIG_INDEX                         (0)
#define CY8CMBR3xxx_BYTE_SHIFT_BITS                              (8)
#define CY8CMBR3xxx_BYTE_MASK                                    (0xFF)


/*******************************************************************************
*   Function Code
*******************************************************************************/

/*******************************************************************************
* Function Name: CY8CMBR3xxx_WriteData
********************************************************************************
*
* Summary:
*  This API writes one or more bytes of data to successive register locations 
*  in the CY8CMBR3xxx device. The initial register location is user – specified.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint8 *writeBuffer:
*   The buffer from which data is written to the device. 
*
*   The first element should always contain the location of the register 
*   of the device to write to. This value can be within 0 - 134.
*
*   Each successive element should contain the data to be written to that 
*   register and the successive registers. These elements can have a value 
*   between 0 - 255. The number of data bytes can range from 1 to 
*   min(128, 135 - register location). 
*
*  uint8 numberOfBytes:
*   Number of bytes to be written, equal to the number of elements in the
*   buffer (i.e. number of data bytes + 1).
*   Valid range: 1 to min(129, 136 - register location)
*
* Return:
*  Status
*    Value              Description
*    TRUE               Write was successful
*    FALSE              Write was not successful
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_WriteData(uint8 slaveAddress, uint8 *writeBuffer, uint8 numberOfBytes)
{
	uint8_t localStatus = FALSE; /* Default return is FALSE if anything goes wrong */
    
    /* Execute only if the slave address, buffer location and number of bytes are valid */
    if ((CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress)) && (writeBuffer) && 
        ((numberOfBytes >= CY8CMBR3xxx_REG_ADDRESS_LENGTH) && 
         (numberOfBytes <= CY8CMBR3xxx_CONFIG_DATA_LENGTH_WITH_CRC_AND_ADDR)))
    {
        /* Write the data using the Low level host API */
        localStatus =  Host_LowLevelWrite(slaveAddress, writeBuffer, numberOfBytes);
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_ReadData
********************************************************************************
*
* Summary:
*  This API reads one or more bytes of data from successive register 
*  locations in the CY8CMBR3xxx device. The initial register location is 
*  user – specified.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint8 registerAddress:
*   The register location to read from. The value passed to this argument 
*   should be one of the register offset macros defined in the file 
*   CY8CMBR3xxx_Registers.h. The macro for a register is defined only if it is
*   supported by the device defined by CY8CMBR3xxx_DEVICE macro.
*
*  uint8 *readBuffer:
*   The buffer to be updated with the data read from the device. 
*   Each successive element to contain the data read from successive 
*   registers. These elements can have a value between 0 – 255.
*
*  uint8 numberOfBytes:
*   Number of data bytes to be read, equal to the number of elements 
*   in the buffer. Valid range: 1 – 252
*
* Return:
*  Status
*    Value              Description
*    TRUE               Read was successful
*    FALSE              Read was not successful
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_ReadData(uint8 slaveAddress, uint8 registerAddress, uint8 *readBuffer, uint8 numberOfBytes)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* First byte in write buffer should be the register address */
    uint8 localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};    
    
    /* Execute only if the slave address, buffer location and number of bytes are valid */
    if ((CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress)) && (readBuffer) && 
        ((numberOfBytes >= CY8CMBR3xxx_BYTE_READ_LENGTH) && 
         (numberOfBytes <= CY8CMBR3xxx_MAX_DEVICE_REGISTERS)))
    {
        /* Use low level host API to write the register address first */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = registerAddress;
        if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Read the data using the low level host API */
            localStatus = Host_LowLevelRead(slaveAddress, readBuffer, numberOfBytes);
        }
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_WriteDualByte
********************************************************************************
*
* Summary:
*  This API writes to a two byte register in the CY8CMBR3xxx device. The 
*  register location is user - specified.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint8 registerAddress:
*   The register location to write to. The value passed to this argument 
*   should be one of the register offset macros defined in the file 
*   CY8CMBR3xxx_Registers.h. The macro for a register is defined only if it is
*   supported by the device defined by CY8CMBR3xxx_DEVICE macro.
*
*  uint16 writeData:
*   The two-byte value to be written to the register. The LSB forms the 
*   first byte written to the device, and the MSB forms the second byte. 
*   Valid range: 0 – 65535
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Write was successful
*    FALSE                Write was not successful
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_WriteDualByte(uint8 slaveAddress, uint8 registerAddress, uint16 writeData)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* Split the LSB of the 16-bit data */
    uint8 writeDataLSB = (writeData & CY8CMBR3xxx_BYTE_MASK);                            

    /* Split the MSB of the 16-bit data */
    uint8 writeDataMSB = ((writeData >> CY8CMBR3xxx_BYTE_SHIFT_BITS) & CY8CMBR3xxx_BYTE_MASK);
    
    /* Combine the data with the register address*/
    uint8 localWriteBuffer[CY8CMBR3xxx_DUAL_BYTE_WRITE_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};    
    
    /* Execute only if the slave address is valid */
    if (CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress))
    {
        /* Write the data using the Low level host API */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = registerAddress;                /* First byte is always Register address */
        localWriteBuffer[CY8CMBR3xxx_WRITE_DATA_LSB_OFFSET] = writeDataLSB;                /* LSB Next */
        localWriteBuffer[CY8CMBR3xxx_WRITE_DATA_MSB_OFFSET] = writeDataMSB;                /* MSB Last */
        localStatus = Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_DUAL_BYTE_WRITE_LENGTH);
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_ReadDualByte
********************************************************************************
*
* Summary:
*  This API reads from a two-byte register in the CY8CMBR3xxx device and 
*  updates the concatenated value to the read buffer. The register location
*  is user - specified.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint8 registerAddress:
*   The two-byte register to read from. The data will be read from this 
*   address and the successive address.
* 
*   The value passed to this argument should be one of the register offset 
*   macros defined in the file CY8CMBR3xxx_Registers.h. The macro for a 
*   register is defined only if it is supported by the device defined by 
*   CY8CMBR3xxx_DEVICE macro.
*
*  uint16 *readData:
*   The 2-byte buffer to be updated with the value read from the device. 
*   The API fills the LSB with the first byte read from the device, and the 
*   MSB with the second byte.
*   The value can be between 0 – 65535.
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Read was successful
*    FALSE                Read was not successful
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_ReadDualByte(uint8 slaveAddress, uint8 registerAddress, uint16 *readData)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* First byte in write buffer should be the register address */
    uint8 localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};    
    
    /* 2-byte buffer to read the dual-byte */
    uint8 localReadBuffer[CY8CMBR3xxx_DUAL_BYTE_READ_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};  
    
    /* Execute only if the slave address and buffer location are valid */
    if ((CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress)) && (readData))
    {
        /* Use low level host API to write the register address first */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = registerAddress;
        if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Read the data using the low level host API */
            if (Host_LowLevelRead(slaveAddress, localReadBuffer, CY8CMBR3xxx_DUAL_BYTE_READ_LENGTH))
            {
                /* Update the read buffer */
                *readData = (((uint16)localReadBuffer[CY8CMBR3xxx_READ_DATA_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS)
                                | (localReadBuffer[CY8CMBR3xxx_READ_DATA_LSB_OFFSET]));
                localStatus = TRUE;
            }
        }
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_SendCommand
********************************************************************************
*
* Summary:
*  This API issues a new command to the CY8CMBR3xxx device. Before calling 
*  this API, you should check whether the device is ready to accept a new 
*  command by calling CY8CMBR3xxx_CheckCommandStatus().
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint8 command:
*   The command op-code to issue. The value passed to this argument should be 
*   one of the op-code macros defined in the file CY8CMBR3xxx_CommandsAndConfig.h.
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Command was issued successfully
*    FALSE                Command was not issued successfully
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_SendCommand(uint8 slaveAddress, uint8 command)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* Data to be written is the command register address followed by command */
    uint8 localWriteBuffer[CY8CMBR3xxx_COMMAND_SEND_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};
    
    /* Execute only if the slave address is valid */
    if (CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress))
    {
        /* Use low level host API to send the command */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = CY8CMBR3xxx_CTRL_CMD;        /* First byte is the Register address */
        localWriteBuffer[CY8CMBR3xxx_COMMAND_OFFSET] = command;                         /* Command code is the second byte */
        localStatus =  Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_COMMAND_SEND_LENGTH);
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMRB3xxx_CheckCommandStatus
********************************************************************************
*
* Summary:
*  This API verifies whether the CY8CMBR3xxx device is ready to accept a new 
*  command, and whether the previously issued command has executed successfully.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint8 *errorCode:
*   Pointer to the error code returned from the last command execution. 
*   If the last command failed, the API updates this data to show what 
*   caused the failure. Valid range is 0 to 255.
*
*   Host should initialize this pointer to zero before calling this API. 
*   If return value is CY8CMBR3xxx_COMMAND_STATUS_FAILED after calling this API 
*   and errorCode is still zero, it means communication failure has occured.
*
* Return:
*  Status
*    Value                                       Description
*    CY8CMBR3xxx_COMMAND_STATUS_BUSY             Last command is still executing
*    CY8CMBR3xxx_COMMAND_STATUS_SUCCESS          Last command executed successfully
*    CY8CMBR3xxx_COMMAND_STATUS_FAILED           Last command did not execute successfully
*
*******************************************************************************/
uint8_t CY8CMRB3xxx_CheckCommandStatus(uint8 slaveAddress, uint8 *errorCode)
{
	uint8_t localStatus = CY8CMBR3xxx_COMMAND_STATUS_FAILED;                                           /* Default return is FALSE if anything goes wrong */
	uint8_t localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};        /* Data to be written is the command register address */
	uint8_t localReadBuffer[CY8CMBR3xxx_COMMAND_STATUS_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};      /* Buffer to read the Command Status and Error registers */
    
    /* Execute only if the slave address is valid */
    if (CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress))
    {
        /* Use low level host API to write the register address first */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = CY8CMBR3xxx_CTRL_CMD;
        if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Read 4-bytes (CTRL_CMD, CTRL_CMD_STATUS, RESERVED and CTRL_CMD_ERR) data using the low level host API */
            if (Host_LowLevelRead(slaveAddress, localReadBuffer, CY8CMBR3xxx_COMMAND_STATUS_LENGTH))
            {
                /* Last command has executed if CTRL_CMD regsiter has NO_COMMAND */
                if (CY8CMBR3xxx_CMD_NO_COMMAND == localReadBuffer[CY8CMBR3xxx_CMD_STAT_CTRL_CMD_OFFSET])
                {
                    /* CTRL_CMD_STATUS is non-zero if there was any error in executing previous command */
                    if (0 != localReadBuffer[CY8CMBR3xxx_CMD_STAT_CTRL_CMD_STATUS_OFFSET])
                    {
                        /* Update the error code pointer with the value from CTRL_CMD_ERR */
                        *errorCode = localReadBuffer[CY8CMBR3xxx_CMD_STAT_CTRL_CMD_ERR_OFFSET];
                        
                        /* Let the host know that the last command failed */
                        localStatus = CY8CMBR3xxx_COMMAND_STATUS_FAILED;
                    }
                    else
                    {
                        /* CTRL_CMD_STATUS is zero. Let the host know that the last command was successful */
                        localStatus = CY8CMBR3xxx_COMMAND_STATUS_SUCCESS;
                    }
                }
                else
                {
                    /* CTRL_CMD is non-zero. Let the host know that the last command is still executing */
                    localStatus = CY8CMBR3xxx_COMMAND_STATUS_BUSY;
                }
            }
        }
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_Configure
********************************************************************************
*
* Summary:
*  This API writes a user – specified configuration to the CY8CMBR3xxx device, 
*  saves the configuration to flash, and resets the device. To configure the 
*  device, you should first create a configuration using EZ-Click. The 
*  128-byte configuration data (generated in the project specific .H file 
*  in the EZ-Click project) should be used directly for this API.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  const unsigned char *configuration:
*   Buffer of 128 bytes to store the entire configuration for the device. 
*   This data should be taken directly from the EZ-Click generated 
*   configuration data. This includes 126-bytes of configuration registers 
*   data and two bytes of CRC.
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Device was configured successfully
*    FALSE                Device was not configured successfully
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_Configure(uint8 slaveAddress, const unsigned char *configuration)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* Buffer to write data: Maximum length is configuration data length + CRC (2bytes) + Address (1byte) */
	uint8_t localWriteBuffer[CY8CMBR3xxx_CONFIG_DATA_LENGTH_WITH_CRC_AND_ADDR] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};
    
    /* Buffer to read data */
	uint8_t localReadBuffer[CY8CMBR3xxx_BYTE_READ_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};
    
    /* Buffer index variable */
	uint8_t configurationIndex = CY8CMBR3xxx_DEFAULT_CONFIG_INDEX;
    
    /* Command error status variable */
	uint8_t localCommandError = CY8CMBR3xxx_CTRL_CMD_NO_ERROR;
    
    /* Execute only if the device on I2C bus is a valid MBR3 device */
    if (CY8CMBR3xxx_VerifyDeviceOnBus(slaveAddress))
    {
        /* Use low level host API to write the CTRL_CMD register address first */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = CY8CMBR3xxx_CTRL_CMD;
        if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Use low level host API to read the CTRL_CMD register contents */
            if(Host_LowLevelRead(slaveAddress, localReadBuffer, CY8CMBR3xxx_BYTE_READ_LENGTH))
            {
                /* Device is busy executing previous command if CTRL_CMD is non-zero */
                if (localReadBuffer[CY8CMBR3xxx_CMD_STAT_CTRL_CMD_OFFSET] == 0)        
                {
                    /* Setup the write buffer - Register address 0x00 followed by the configuration data */
                    localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = CY8CMBR3xxx_SENSOR_EN;
                    for (configurationIndex = CY8CMBR3xxx_DEFAULT_CONFIG_INDEX; configurationIndex < CY8CMBR3xxx_CONFIG_DATA_LENGTH_WITH_CRC; configurationIndex++)
                    {
                        localWriteBuffer[configurationIndex + CY8CMBR3xxx_CONFIG_DATA_WRITE_OFFSET] = configuration[configurationIndex];
                    }
                    
                    /* Use low level host API to send the configuration data to the device */
                    /* First register address (0x00) is appended with the configuration data (128-bytes) */
                    if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, (CY8CMBR3xxx_CONFIG_DATA_LENGTH_WITH_CRC_AND_ADDR)))
                    {
                        /* Issue SAVE_CHECK_CRC command to validate the new configuration data */
                        if (CY8CMBR3xxx_SendCommand(slaveAddress, CY8CMBR3xxx_CMD_SAVE_CHECK_CRC))
                        {
                            /* Wait for the SAVE_CHECK_CRC command to complete execution */
                            Host_LowLevelDelay(CY8CMBR3xxx_DELAY_SAVE_CHECK_CRC_CMD);

                            /* Read the device status and check for errors */
                            if (CY8CMBR3xxx_COMMAND_STATUS_SUCCESS == CY8CMRB3xxx_CheckCommandStatus(slaveAddress, &localCommandError))
                            {
                                /* Command was successful; issue a Software Reset */
                                if (CY8CMBR3xxx_SendCommand(slaveAddress, CY8CMBR3xxx_CMD_SOFTWARE_RESET))
                                {
                                    /* Wait for the SOFTWARE_RESET command to complete execution */
                                    /* Wait for the device to finish reset */
                                    Host_LowLevelDelay(CY8CMBR3xxx_DELAY_SOFTWARE_RESET_CMD);
                                    
                                    /* Read the device status and check for errors */
                                    if (CY8CMBR3xxx_COMMAND_STATUS_SUCCESS == CY8CMRB3xxx_CheckCommandStatus(slaveAddress, &localCommandError))
                                    {
                                        /* All the above operations were OK */
                                        localStatus = TRUE;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_VerifyDevice
********************************************************************************
*
* Summary:
*  This API verifies that the device on the I2C bus is a valid CY8CMBR3xxx 
*  device. It checks the Device ID, Family ID, and Device Revision to 
*  verify the device.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Device is valid
*    FALSE                Device is not valid, or I2C transaction failed
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_VerifyDeviceOnBus(uint8 slaveAddress)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* Data to be written is the Family_ID regsiter address */
	uint8_t localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_LENGTH] = {CY8CMBR3xxx_FAMILY_ID};
    
    /* Buffer to read 5-bytes */
	uint8_t localReadBuffer[CY8CMBR3xxx_VERIFY_DEVICE_READ_BUFFER_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};
    
    /* Variable to store the Family ID read from device */
	uint8_t localFamilyID = CY8CMBR3xxx_DEFAULT_FAMILY_ID;
    
    /* Variable to store the Device ID read from device */
	uint8_t localDeviceID = CY8CMBR3xxx_DEFAULT_DEVICE_ID;
    
    /* Variable to store the Device Revision read from device */
	uint8_t localDeviceRevision = CY8CMBR3xxx_DEFAULT_DEVICE_REV;
    
    /* Execute only if the slave address is valid */
    if (CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress))
    {
        /* Use low level host API to write the register address first */
        if(Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Use low level host API to read 5-bytes from device */
            if (Host_LowLevelRead(slaveAddress, localReadBuffer, CY8CMBR3xxx_VERIFY_DEVICE_READ_BUFFER_LENGTH))
            {
                /* Compile the required values from the data */
                localFamilyID = localReadBuffer[CY8CMBR3xxx_VERIFY_DEVICE_FAMILYID_OFFSET];
                localDeviceID = ((uint16)localReadBuffer[CY8CMBR3xxx_VERIFY_DEVICE_DEVID_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                    | localReadBuffer[CY8CMBR3xxx_VERIFY_DEVICE_DEVID_LSB_OFFSET];
                
                localDeviceRevision = ((uint16)localReadBuffer[CY8CMBR3xxx_VERIFY_DEVICE_DEVREV_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                        | localReadBuffer[CY8CMBR3xxx_VERIFY_DEVICE_DEVREV_LSB_OFFSET];
                
                /* Compare the compiled value against the pre-defined values */
                if ((CY8CMBR3xxx_DEFAULT_FAMILY_ID == localFamilyID) && 
                    (CY8CMBR3xxx_DEFAULT_DEVICE_ID == localDeviceID) && 
                    (CY8CMBR3xxx_DEFAULT_DEVICE_REV == localDeviceRevision))
                {
                    localStatus = TRUE;                                /* Device is valid */
                }
            }
        }
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_SetDebugDataSensorId
********************************************************************************
*
* Summary:
*  This API sets the Sensor ID for reading the corresponding debug data from 
*  the CY8CMBR3xxx device. 
*
*  To read the debug data from the device, you should first call this API, 
*  followed by a time wait equal to the device refresh interval. Then you 
*  should call CY8CMBR3xxx_ReadSensorDebugData() to read back the debug data 
*  from the device.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint8 sensorId:
*   The sensor number for which the data is requested. This number can be a 
*   maximum of one less than the total number of sensors supported by the device.
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Sensor ID write operation was successful
*    FALSE                Sensor ID write operation was not successful
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_SetDebugDataSensorId(uint8 slaveAddress, uint8 sensorId)
{
	uint8_t localStatus = FALSE;                                                /* Default return is FALSE if anything goes wrong */
    uint8 localWriteBuffer[CY8CMBR3xxx_SENSORID_SEND_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};      /* Data to be written is the Sensor_ID regsiter address followed by required sensor */
    
    /* Execute only if the slave address is valid */
    if (CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress))
    {
        /* Use low level host API to write the register address first */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = CY8CMBR3xxx_SENSOR_ID;        /* First byte is always Register address */
        localWriteBuffer[CY8CMBR3xxx_SENSORID_OFFSET] = sensorId;                        /* Second byte is the sensor id */
        localStatus =  Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_SENSORID_SEND_LENGTH);
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_ReadSensorDebugData
********************************************************************************
*
* Summary:
*  This API reads the Cp, Difference counts, Baseline, Raw counts, and Average 
*  Raw counts of a sensor and updates the data to a buffer. 
*
*  To read debug data, you should first call CY8CMBR3xxx_SetDebugDataSensorId(), 
*  and then wait for one refresh interval. After this time, you should call 
*  this API to read the data from the device.
*
*  The API updates the buffer only if the sync counters of the device match. 
*  In case they don’t, the API tries to read the data again. The maximum number
*  of read attempts is CY8CMBR3xxx_SYNC_COUNTER_MATCH_RETRY. You can set 
*  this value in CY8CMBR3xxx_APIs.h file.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  CY8CMBR3xxx_SENSORDATA *debugData:
*   Pointer to the user-defined buffer where the data read from the CY8CMBR3xxx 
*   device is stored. The buffer structure should be as defined in CY8CMBR3xxx_APIs.h.
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Buffer was updated
*    FALSE                Buffer was not updated
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_ReadSensorDebugData(uint8 slaveAddress, CY8CMBR3xxx_SENSORDATA *debugData)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* Data to be written is the Sensor_ID regsiter address followed by required sensor */
    uint8 localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};        
    
    /* Buffer to read 13-bytes */
    uint8 localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};
    
    /* Defines number of retries before returning fail status */
    uint8 retryCount = CY8CMBR3xxx_SYNC_COUNTER_MATCH_RETRY;
    
    /* Execute only if the slave address is valid */
    if (CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress))
    {
        /* Use low level host API to move the register address to SYNC_COUNTER1 */
        localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_OFFSET] = CY8CMBR3xxx_SYNC_COUNTER1;            /* First byte is always the register address */
        if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Use low level host API to read 13-bytes from device */
            do 
            {
                localStatus =  Host_LowLevelRead(slaveAddress, localReadBuffer, CY8CMBR3xxx_SENSOR_DATA_LENGTH);
            } while ((localStatus == TRUE) &&
                     (localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_SYNC1_OFFSET] != localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_SYNC2_OFFSET]) &&    /* To proceed SYNC_COUNTER1 should be equal to SYNC_COUNTER2 */
                     (--retryCount));                                                                               /* Try for a maximum of SYNC_COUNTER_MATCH_RETRY times */
            
            /* If retryCount is non-zero, and I2C transaction was successful - indicates sync counters matched */
            if (retryCount && localStatus)
            {
                /* Compute the required values from data read */
                debugData->sensorAverageCounts = (((uint16)localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_AVG_CNT_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                                        | localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_AVG_CNT_LSB_OFFSET]);
                debugData->sensorBaseline = (((uint16)localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_BASELINE_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                                    | localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_BASELINE_LSB_OFFSET]);
                debugData->sensorCp = localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_CP_OFFSET];
                debugData->sensorDiffCounts = (((uint16)localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_DIFFCOUNTS_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                                    | localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_DIFFCOUNTS_LSB_OFFSET]);
                debugData->sensorRawCounts = (((uint16)localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_RAWCOUNTS_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                                    | localReadBuffer[CY8CMBR3xxx_SENSOR_DATA_RAWCOUNTS_LSB_OFFSET]);
            }
        }
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_ReadDiffCounts
********************************************************************************
*
* Summary:
*  This API reads the Difference counts of all sensors of the CY8CMBR3xxx 
*  device and updates the data to a buffer.
*
*  The API updates the buffer only if the sync counters of the device match. 
*  In case they don’t, the API tries to read the data again. The maximum number
*  of read attempts is CY8CMBR3xxx_SYNC_COUNTER_MATCH_RETRY. You can set 
*  this value in CY8CMBR3xxx_APIs.h file.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  uint16 *differenceCounts:
*   Buffer to store the difference counts of all sensors. The buffer size 
*   should be equal to the number of sensors in the device.
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Buffer was updated
*    FALSE                Buffer was not updated
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_ReadDiffCounts(uint8 slaveAddress, uint16 *differenceCounts)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* Data to be written is the SYNC_COUNTER0 register address */
    uint8 localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_LENGTH] = {CY8CMBR3xxx_SYNC_COUNTER0};
    
    /* Buffer to read 35-bytes */
    uint8 localReadBuffer[CY8CMBR3xxx_DIFFCOUNT_DATA_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};
    
    /* Defines number of retries before returning fail status */
    uint8 retryCount = CY8CMBR3xxx_SYNC_COUNTER_MATCH_RETRY;
    
    /* To keep a count on the Sensor ID locally */
    uint8 localSensor = CY8CMBR3xxx_DEFAULT_SENSOR_ID;
    
    /* Execute only if the slave address and buffer location are valid */
    if ((CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress)) && (differenceCounts))
    {
        /* Use low level host API to move the register address to SYNC_COUNTER0 */
        if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Use low level host API to read 35-bytes from device */
            do 
            {
                localStatus =  Host_LowLevelRead(slaveAddress, localReadBuffer, CY8CMBR3xxx_DIFFCOUNT_DATA_LENGTH);                    
            } while ((localStatus == TRUE) && 
                     (localReadBuffer[CY8CMBR3xxx_DIFFCOUNT_SYNC0_OFFSET] != localReadBuffer[CY8CMBR3xxx_DIFFCOUNT_SYNC1_OFFSET]) &&   /* To proceed SYNC_COUNTER0 should be equal to SYNC_COUNTER1 */
                     (--retryCount));                                                                          /* Try for a maximum of SYNC_COUNTER_MATCH_RETRY times */
            
            /* If retryCount is non-zero, and I2C transaction was successful - indicates sync counters matched */
            if (retryCount && localStatus)
            {
                /* Compute the required values from data read */
                for (localSensor = CY8CMBR3xxx_DEFAULT_SENSOR_ID; localSensor < CY8CMBR3xxx_SENSOR_COUNT; localSensor++)
                {
                    differenceCounts[localSensor] = (((uint16)localReadBuffer[(localSensor * CY8CMBR3xxx_DIFFCOUNT_WIDTH) + CY8CMBR3xxx_DIFFCOUNT_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) | 
                                                                    (localReadBuffer[(localSensor * CY8CMBR3xxx_DIFFCOUNT_WIDTH) + CY8CMBR3xxx_DIFFCOUNT_LSB_OFFSET]));
                }
            }
        }
    }
    
    /* Return the status */
    return localStatus;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_ReadSensorStatus
********************************************************************************
*
* Summary:
*  This API reads the button status, latched button status, proximity status, 
*  latched proximity status, slider 1 and 2 positions, and slider 1 and 2 liftoff 
*  positions from the CY8CMBR3xxx device.
*  The slider positions and slider liftoff positions are valid for CY8CMBR3106S 
*  device only.
*
*  You can use this API when the device asserts a Host Interrupt pulse, to 
*  understand the corresponding status change.
*
* Parameters:
*  uint8 slaveAddress:
*   The I2C address of the CY8CMBR3xxx device. Valid range: 8 - 119
*
*  CY8CMBR3xxx_SENSORSTATUS *status:
*   Pointer to the user-defined buffer where the sensor status read from 
*   the CY8CMBR3xxx device is stored. The buffer structure should be as 
*   defined in CY8CMBR3xxx_APIs.h.
*
* Return:
*  Status
*    Value                Description
*    TRUE                 Buffer was updated
*    FALSE                Buffer was not updated
*
*******************************************************************************/
uint8_t CY8CMBR3xxx_ReadSensorStatus(uint8 slaveAddress, CY8CMBR3xxx_SENSORSTATUS *status)
{
    /* Default return is FALSE if anything goes wrong */
	uint8_t localStatus = FALSE;
    
    /* Data to be written is the BUTTON_STAT register address */
    uint8 localWriteBuffer[CY8CMBR3xxx_REG_ADDRESS_LENGTH] = {CY8CMBR3xxx_BUTTON_STAT};
    
    /* Buffer to read 10-bytes */
    uint8 localReadBuffer[CY8CMBR3xxx_SENSOR_STATUS_LENGTH] = {CY8CMBR3xxx_BUFFER_INIT_VALUE};           
    
    /* Execute only if the slave address is valid */
    if (CY8CMBR3xxx_IS_VALID_ADDRESS(slaveAddress))
    {
        /* Use low level host API to write the register address first */
        if (Host_LowLevelWrite(slaveAddress, localWriteBuffer, CY8CMBR3xxx_REG_ADDRESS_LENGTH))
        {
            /* Use low level host API to read 10-bytes from device */
            if (Host_LowLevelRead(slaveAddress, localReadBuffer, CY8CMBR3xxx_SENSOR_STATUS_LENGTH))
            {
                /* Compute the required values from data read */
                status->buttonStatus = ((uint16)localReadBuffer[CY8CMBR3xxx_BUTTONSTATUS_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                                    | localReadBuffer[CY8CMBR3xxx_BUTTONSTATUS_LSB_OFFSET];
                status->latchedButtonStatus = ((uint16)localReadBuffer[CY8CMBR3xxx_LATCHEDBUTTONSTATUS_MSB_OFFSET] << CY8CMBR3xxx_BYTE_SHIFT_BITS) 
                                                    | localReadBuffer[CY8CMBR3xxx_LATCHEDBUTTONSTATUS_LSB_OFFSET];
                status->proxStatus = localReadBuffer[CY8CMBR3xxx_PROXSTATUS_OFFSET];
                status->latchedProxStatus = localReadBuffer[CY8CMBR3xxx_LATCHEDPROXSTATUS_OFFSET];
                status->slider1Position = localReadBuffer[CY8CMBR3xxx_SLIDER1POSITION_OFFSET];
                status->liftoffSlider1Position = localReadBuffer[CY8CMBR3xxx_LIFTOFFSLIDER1POSITION_OFFSET];
                status->slider2Position = localReadBuffer[CY8CMBR3xxx_SLIDER2POSITION_OFFSET];
                status->liftoffSlider2Position = localReadBuffer[CY8CMBR3xxx_LIFTOFFSLIDER2POSITION_OFFSET];
                
                localStatus = TRUE;
            }
        }
    }
    
    /* Return the status */
    return localStatus;
}

/****************************End of File***************************************/
