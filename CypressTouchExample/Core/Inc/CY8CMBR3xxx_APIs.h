/*****************************************************************************
* File Name: CY8CMBR3xxx_APIs.h
*
* Version 1.00
*
* Description:
*   This file contains the declarations of all the high-level APIs.
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

#if !defined(CY8CMBR3xxx_APIS_H)
#define CY8CMBR3xxx_APIS_H
    
/*******************************************************************************
* Included headers
*******************************************************************************/
#include<stdint.h>


/*******************************************************************************
* User defined Macros
*******************************************************************************/
/* The APIs CY8CMBR3xxx_ReadDiffCounts() and CY8CMBR3xxx_ReadSensorDebugData() 
 * try to read from the device for a maximum number of times defined by this 
 * value, as long as the sync counters do not match. Please change as required.
 */
#define CY8CMBR3xxx_SYNC_COUNTER_MATCH_RETRY             (10)


/*******************************************************************************
* Data Type Definitions
*******************************************************************************/
#if !defined(CY8CMBR3xxx_DATATYPE_GUARD)
#define CY8CMBR3xxx_DATATYPE_GUARD

#define TRUE                                             (1)
#define FALSE                                            (0)

typedef unsigned char   uint8;
typedef unsigned short  uint16;
typedef unsigned long   uint32;
//typedef uint8 bool;
#endif

/*******************************************************************************
* Structure Definitions
*******************************************************************************/
/* 
 * This structure will hold the debug data read from the device by the 
 * CY8CMBR3xxx_ReadSensorData() API.
 */
typedef struct 
{
    uint8 sensorCp;
    uint16 sensorDiffCounts;
    uint16 sensorBaseline;
    uint16 sensorRawCounts;
    uint16 sensorAverageCounts;
} CY8CMBR3xxx_SENSORDATA;

/* 
 * This structure will hold the sensor status read from the device
 * by the CY8CMBR3xxx_ReadSensorStatus() API.
 * The slider positions and slider liftoff positions are valid for 
 * CY8CMBR3106S device only.
 */
typedef struct 
{
    uint16 buttonStatus;
    uint16 latchedButtonStatus;
    uint8 proxStatus;
    uint8 latchedProxStatus;
    uint8 slider1Position;
    uint8 liftoffSlider1Position;
    uint8 slider2Position;
    uint8 liftoffSlider2Position;
} CY8CMBR3xxx_SENSORSTATUS;


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
uint8_t CY8CMBR3xxx_WriteData(uint8 slaveAddress, uint8 *writeBuffer, uint8 numberOfBytes);
uint8_t CY8CMBR3xxx_ReadData(uint8 slaveAddress, uint8 registerAddress, uint8 *readBuffer, uint8 numberOfBytes);
uint8_t CY8CMBR3xxx_WriteDualByte(uint8 slaveAddress, uint8 registerAddress, uint16 writeData);
uint8_t CY8CMBR3xxx_ReadDualByte(uint8 slaveAddress, uint8 registerAddress, uint16 *readData);
uint8_t CY8CMBR3xxx_SendCommand(uint8 slaveAddress, uint8 command);
uint8_t CY8CMRB3xxx_CheckCommandStatus(uint8 slaveAddress, uint8 *errorCode);
uint8_t CY8CMBR3xxx_Configure(uint8 slaveAddress, const unsigned char *configuration);
uint16 CY8CMBR3xxx_CalculateCrc(uint8 *configuration);
uint8_t CY8CMBR3xxx_VerifyDeviceOnBus(uint8 slaveAddress);
uint8_t CY8CMBR3xxx_SetDebugDataSensorId(uint8 slaveAddress, uint8 sensorId);
uint8_t CY8CMBR3xxx_ReadSensorDebugData(uint8 slaveAddress, CY8CMBR3xxx_SENSORDATA *debugData);
uint8_t CY8CMBR3xxx_ReadDiffCounts(uint8 slaveAddress, uint16 *differenceCounts);
uint8_t CY8CMBR3xxx_ReadSensorStatus(uint8 slaveAddress, CY8CMBR3xxx_SENSORSTATUS *status);
#endif

/****************************End of File***************************************/
