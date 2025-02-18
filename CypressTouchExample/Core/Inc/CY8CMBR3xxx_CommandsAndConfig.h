/*****************************************************************************
* File Name: CY8CMBR3xxx_CommandsAndConfig.h
*
* Version 1.00
*
* Description:
*   This file contains the macros to define the various op-codes of the command  
*   register (CTRL_CMD).
*
* Note:
*   N/A
*
* Owner:
*   SRVS
*
* Related Document:
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

#if !defined(CY8CMBR3xxx_COMMANDSANDCONFIG_H)
#define CY8CMBR3xxx_COMMANDSANDCONFIG_H

/*******************************************************************************
* Included Headers
*******************************************************************************/
#include "CY8CMBR3xxx_Device.h"

    
/*******************************************************************************
* Supported Commands
*******************************************************************************/
// There is no command currently executing.
// The device writes this value at startup and upon completion of any command.
#define CY8CMBR3xxx_CMD_NO_COMMAND                             (0)     

// The device calculates a CRC checksum over the configuration data in this register map and compares the result with the content of CONFIG_CRC. 
// If the two values match, the device saves the configuration and the CRC checksum to nonvolatile memory.
#define CY8CMBR3xxx_CMD_SAVE_CHECK_CRC                         (2)     

// The device discontinues scanning and enters the low power mode.  
// The device will exit this mode upon an I2C address match event.
#define CY8CMBR3xxx_CMD_SLEEP                                  (7)     

// The device sets the contents of LATCHED_BUTTON_STAT and LATCHED_PROX_STAT to 0 and 
// sets the contents of LIFTOFF_SLIDER1_POSITION and LIFTOFF_SLIDER2_POSITION to 0xFF.
#define CY8CMBR3xxx_CMD_CLEAR_LATCHED_STATUS                   (8)     

#if !(CY8CMBR3xxx_DEVICE & (CY8CMBR3xxx_CY8CMBR3106S))
// The device resets the Advanced Low Pass filter for proximity sensor PS0
#define CY8CMBR3xxx_CMD_RESET_PROX0_FILTER                     (9)     

// The device resets the Advanced Low Pass filter for proximity sensor PS1
#define CY8CMBR3xxx_CMD_RESET_PROX1_FILTER                     (10)     
#endif

// The device resets itself
#define CY8CMBR3xxx_CMD_SOFTWARE_RESET                         (255)     

#endif

/****************************End of File***************************************/
