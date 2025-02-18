/*****************************************************************************
* File Name: CY8CMBR3xxx_HostFunctions.h
*
* Version 1.00
*
* Description:
*   This file contains the declarations of the host-dependent, low-level APIs.
*
* Note:
*   These host-dependent Low Level APIs are provided as an example of
*   low level I2C read and write functions. This set of low level APIs are 
*   written for PSoC 4200/4100 devices and hence should be re-written
*   with equivalent host-dependent APIs from the respective IDEs, if the 
*   host design does not include PSoC 4200/4100 device.
* 
*   To use these APIs, the host should implement a working I2C communication
*   interface. This interface will be used by these APIs to communicate to the
*   CY8CMBR3xxx device.
*
*   For PSoC 4200/4100 devices, please ensure that you have created an instance 
*   of SCB component with I2C Master configuration. The component should be
*   named "SCB".
*
* Owner:
*   SRVS
*
* Related Document:
*   MBR3 Design Guide
*   MBR3 Device Datasheet
*
* Hardware Dependency:
*   PSoC 4200 (Update this as per the host used)
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

#if !defined(CY8CMBR3xxx_HOSTFUNCTIONS_H)
#define CY8CMBR3xxx_HOSTFUNCTIONS_H

/*******************************************************************************
* Included headers
*******************************************************************************/
#include<stdint.h>


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
typedef uint8 bool;
#endif

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
uint8_t Host_LowLevelWrite(uint8 slaveAddress, uint8 *writeBuffer, uint8 numberOfBytes);
uint8_t Host_LowLevelRead(uint8 slaveAddress, uint8 *readBuffer, uint8 numberOfBytes);
void Host_LowLevelDelay(uint16 milliseconds);

#endif

/****************************End of File***************************************/
