###### OTA FILE #####
## Important steps to be followed ##

This file will tell the process that needs to be followed for the OTA file creations

Currently the flash is divided into three parts:

BOOTLOADER: 12KB in size

START_ADDRESS - 0X0800000
END_ADDRESS - 0X08002FFF

PRIMARY APPLICATION : 200KB in size

START_ADDRESS - 0X08003000
END_ADDRESS - 0X08035000

SECONDARY APPLICATION

START_ADDRESS - 0x08040000
END_ADDRESS - 0X08072000


> STEP 1 : The START_ADDRESS and size should be updated in the STM32G0B1RETX_FLASH.ld and STM32G0B1RETX_RAM.ld based on which application you are running.
> STEP 2 : The START_ADDRESS and END_ADDRESS of the next application will going to be updated in flash_if.h

>> STEP 3 : For example if you are running the primary application
>>> Then START_ADDRESS of Primary Application will going to be update in STM32G0B1RETX_FLASH.ld and STM32G0B1RETX_RAM.ld files at location  - 
'''
/* Memories definition */
MEMORY
{
  RAM    (xrw)    : ORIGIN = 0x20000000,   LENGTH = 144K
  FLASH    (rx)    : ORIGIN = 0x8003000,   LENGTH = 200K
}
'''

Here the Flash ORIGIN will be the START_ADDRESS Of the PRIMARY_APPLICATION
and LENGTH = 200K for the size of the PRIMARY APPLICATION

>> STEP 4 : In the same "APPLICATION_ADDRESS" and "USER_FLASH_END_ADDRESS" macros present in the file - flash_if.h
should be update with the START_ADDRESS of the SECONDARY APPLICATION and END_ADDRESS of the secondary application

for example:

#define APPLICATION_ADDRESS     (uint32_t)0x08040000
#define USER_FLASH_END_ADDRESS        0x08072000

>> STEP 5: Very important step: You need to change the address of the vector table, else application won't run.
>>> TO DO THAT, open "system_stm32g0xx.c" file, 
>>> SEARCH FOR THE MACRO - VECT_TAB_BASE_ADDRESS
>>> Update the MACRO value with the START_ADDRESS of the current application.

For example - 

If you want to create the BUILD for the PRIMARY APPLICATION
>> then value of this macro will be - 0x8003000

If you want to create the BUILD for the SECONDARY APPLICATION
>> then value of this macro will be - 0x8072000


!NOTE: 
>> ALL THIS HAVE TO BE DONE MANUALLY ELSE PROCESS WILL FAIL, ALL THESE CHANGES ARE VERY CRUCIAL AND SHOULD BE VERIFIED BY THE EXPERT.

>> OTA WON'T WORK IF PRIMARY_APPLICATION RUNNING IN THE FILED, AND YOU PROVIDE OTA WITH THE ADDRESS VALUE OF PRIMARY APPLICATION, OTA WILL FAIL.

>> SIMILARY, IF SECONDARY APPLICATION IS RUNNING IN THE FIELD, AND YOU PROVIDE OTA WITH THE ADDRESS VALUE OF SECONDARY APPLICATION, OTA WILL FAIL.

>> FOR THE PROCESS TO WORK, YOU HAVE TO VICE VERSA, IF PRIMARY APPLICATION IS RUNNING IN THE FIELD, THEN GENERATE THE OTA FILE WITH SECONDARY APPLICATION ADDRESSES AND VICE VERSA.



>> APPLICATION TO WHICH YOU WANT BOOTLOADER TO JUMP SHOULD BE UPDATE IN THE EEPROM
>>> If you want to jump of PRIMARY APPLICATION then START_ADDRESS of the PRIMARY_APPLICATION to be stored in EEPROM at specific location.
>>> If you want to jump on SECONDARY APPLICATION then START_ADDRESS of the SECONDARY_APPLICATION to be stored in the EEPROM at specific location.
>> This is not needed to be done by the user, application will take care of it my itself.
>> Only to make sure that ALL the STEPS, followed are done correctly.
>> They are very crucial for the process to succeed.


> If user want to switch from any application, first needs to make sure the application user want to jump, should be present in the memory, then user needs to update the START_ADDRESS of that application in the EEPROM and reset the device in Bootloader mode.

> Then bootloader will take care of it automatically.
