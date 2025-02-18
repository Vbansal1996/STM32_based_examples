/*****************************************************************************
* File Name: CY8CMBR3xxx_Device.h
*
* Version 1.00
*
* Description:
*   This file contains the macros to define the various devices in CY8CMBR3xxx 
*   family. Also, it defines which device is currently being used for the Host APIs.
*
* Note:
*   N/A
*
* Owner:
*   SRVS
*
* Related Document:
*   MBR3 Design Guide
*   MBR3 Device Datasheet
*   MBR3 Device Register TRM
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

#if !defined(CY8CMBR3xxx_DEVICE_H)
#define CY8CMBR3xxx_DEVICE_H

/*******************************************************************************
* Device MPN Constants
*******************************************************************************/
#define CY8CMBR3xxx_CY8CMBR3116                (1 << 1)
#define CY8CMBR3xxx_CY8CMBR3106S               (1 << 2)
#define CY8CMBR3xxx_CY8CMBR3110                (1 << 3)
#define CY8CMBR3xxx_CY8CMBR3108                (1 << 4)
#define CY8CMBR3xxx_CY8CMBR3102                (1 << 5)

/*******************************************************************************
* Device Selected in Project
*******************************************************************************/
#define CY8CMBR3xxx_DEVICE                     (CY8CMBR3xxx_CY8CMBR3108)

/*******************************************************************************
* Family ID Constant
*******************************************************************************/
#define CY8CMBR3xxx_DEFAULT_FAMILY_ID                      (0x9A)

/*******************************************************************************
* Device ID and Sensor Count Constants
*******************************************************************************/
#if (CY8CMBR3xxx_DEVICE == CY8CMBR3xxx_CY8CMBR3116)
    #define CY8CMBR3xxx_DEFAULT_DEVICE_ID                      (2565)
    #define CY8CMBR3xxx_SENSOR_COUNT                           (16)
#elif (CY8CMBR3xxx_DEVICE == CY8CMBR3xxx_CY8CMBR3106S)         
    #define CY8CMBR3xxx_DEFAULT_DEVICE_ID                      (2566)
    #define CY8CMBR3xxx_SENSOR_COUNT                           (16)
#elif (CY8CMBR3xxx_DEVICE == CY8CMBR3xxx_CY8CMBR3110)
    #define CY8CMBR3xxx_DEFAULT_DEVICE_ID                      (2562)
    #define CY8CMBR3xxx_SENSOR_COUNT                           (10)
#elif (CY8CMBR3xxx_DEVICE == CY8CMBR3xxx_CY8CMBR3108)
    #define CY8CMBR3xxx_DEFAULT_DEVICE_ID                      (2563)
    #define CY8CMBR3xxx_SENSOR_COUNT                           (8)
#elif (CY8CMBR3xxx_DEVICE == CY8CMBR3xxx_CY8CMBR3102)
    #define CY8CMBR3xxx_DEFAULT_DEVICE_ID                      (2561)
    #define CY8CMBR3xxx_SENSOR_COUNT                           (2)
#endif
    
/*******************************************************************************
* Device Revision Constant
*******************************************************************************/
#define CY8CMBR3xxx_DEFAULT_DEVICE_REV                         (0x01)

#endif

/****************************End of File***************************************/
