/*****************************************************************************
* File Name: CY8CMBR3xxx_CRC.c
*
* Version 1.00
*
* Description:
*   This file contains the API to calculate CRC for a given configuration of 
*   CY8CMBR3xxx device.
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
#include <stdint.h>
#include "CY8CMBR3xxx_APIs.h"


/*******************************************************************************
* API Constants
*******************************************************************************/
#define CY8CMBR3xxx_CONFIG_DATA_LENGTH            (126)
#define CY8CMBR3xxx_CRC_BIT_WIDTH                 (sizeof(uint16) * 8)
#define CY8CMBR3xxx_CRC_BIT4_MASK                 (0x0F)
#define CY8CMBR3xxx_CRC_BIT4_SHIFT                (0x04)
#define CY8CMBR3xxx_CCITT16_DEFAULT_SEED          ((uint16) 0xffff)
#define CY8CMBR3xxx_CCITT16_POLYNOM               ((uint16) 0x1021)


/*******************************************************************************
* Function Name: CY8CMBR3xxx_Calc4BitsCRC
****************************************************************************//**
*
* Summary:
*   Local function for 4 bit CRC calculation  
*
* Parameters:
*  uint8 value:
*   byte value for CRC
*
*  uint16 remainder:
*   remainder from previous operation  
*
* Return:
*    Calculated remainder    
*
**//***************************************************************************/
static uint16 CY8CMBR3xxx_Calc4BitsCRC(uint8 value, uint16 remainder)
{
    uint8 tableIndex; 

    /* Divide the value by polynomial, via the CRC polynomial */
    tableIndex = (value & CY8CMBR3xxx_CRC_BIT4_MASK) ^
                 ((remainder) >> (CY8CMBR3xxx_CRC_BIT_WIDTH - CY8CMBR3xxx_CRC_BIT4_SHIFT));
    remainder = (CY8CMBR3xxx_CCITT16_POLYNOM * tableIndex) ^ (remainder << CY8CMBR3xxx_CRC_BIT4_SHIFT);
    return remainder;
}

/*******************************************************************************
* Function Name: CY8CMBR3xxx_CalculateCrc
********************************************************************************
*
* Summary:
*  This API calculates the CRC checksum of a given configuration setting 
*  for the CY8CMBR3xxx device.
*
* Parameters:
*  uint8 *configuration:
*   This 126-byte buffer would hold the entire configuration setting for the device.
*
* Return:
*  A two-byte value (with the higher byte being the MSB) for the calculated CRC.
*
**//***************************************************************************/
uint16 CY8CMBR3xxx_CalculateCrc(uint8 *configuration)
{
    uint32 messageIndex;
    uint8 byteValue;
    uint16 seed = CY8CMBR3xxx_CCITT16_DEFAULT_SEED;
    
    /* don't make count down cycle! CRC will be different! */
    for(messageIndex = 0; messageIndex < CY8CMBR3xxx_CONFIG_DATA_LENGTH; messageIndex++)
    {
        byteValue = configuration[messageIndex];
        seed = CY8CMBR3xxx_Calc4BitsCRC(byteValue >> CY8CMBR3xxx_CRC_BIT4_SHIFT, seed);
        seed = CY8CMBR3xxx_Calc4BitsCRC(byteValue, seed);
    }

    return seed;
}

/****************************End of File***************************************/
