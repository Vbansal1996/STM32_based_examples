/**
  ******************************************************************************
  * @file    IAP_Main/Src/flash_if.c 
  * @author  MCD Application Team
  * @version 1.0.0
  * @date    8-April-2015
  * @brief   This file provides all the memory related operation functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/** @addtogroup STM32L4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include <string.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*  1M0 flash 1 * 1024 * 1024 */
#define FLASH_START_ADRESS    0x08000000U

//#warning "FLASH_PAGE_NBPERBANK To be changed with HAL parameter"
#define FLASH_PAGE_NBPERBANK  (FLASH_BANK_SIZE / FLASH_PAGE_SIZE)
#define PAGE_ADDRESS(__PAGE__)   (uint32_t)(FLASH_BASE + (__PAGE__) * FLASH_PAGE_SIZE)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
  /* Unlock the Program memory */
  HAL_FLASH_Unlock();

  /* Clear all FLASH flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGSERR | FLASH_FLAG_WRPERR | FLASH_FLAG_OPTVERR);
  /* Unlock the Program memory */
  HAL_FLASH_Lock();
}
#if 0
/**
  * @brief  Gets the bank of a given address
  * @param  Address Address of the FLASH Memory
  * @retval Bank_Number The bank of a given address
  */
static uint32_t GetBankNumber(uint32_t Address)
{
  uint32_t bank = 0U;

  if (OB_USER_DUALBANK_SWAP_DISABLE)
  {
    /* No Bank swap */
    if (Address < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_1;
    }
    else
    {
      bank = FLASH_BANK_2;
    }
  }
  else
  {
    /* Bank swap */
    if (Address < (FLASH_BASE + FLASH_BANK_SIZE))
    {
      bank = FLASH_BANK_2;
    }
    else
    {
      bank = FLASH_BANK_1;
    }
  }

  return bank;
}
#endif
/**
  * @brief  This function does an erase of all user flash area
  * @param  start: start of user flash area
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
uint32_t FLASH_If_Erase(uint32_t start)
{
//  uint32_t NbrOfPages = 0;
  uint32_t StartPageNum = 0;
//  uint32_t pageNum = 0;
  uint32_t NbrOfFwPages = 0;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef pEraseInit;
  HAL_StatusTypeDef status = HAL_OK;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Get the number of page to  erase */
//  NbrOfPages = (FLASH_START_ADRESS + USER_FLASH_SIZE);
//  NbrOfPages = (NbrOfPages - start) / FLASH_PAGE_SIZE;
  
  StartPageNum = GetPage(start);
  NbrOfFwPages =  (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS) / FLASH_PAGE_SIZE;
  
  if((StartPageNum + NbrOfFwPages) > FLASH_PAGE_NBPERBANK)
  {
    pEraseInit.Banks = FLASH_BANK_2;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.NbPages = (StartPageNum + NbrOfFwPages) % FLASH_PAGE_NBPERBANK;
    pEraseInit.Page = 0;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
  }  
    //Erase bank 1
  if(status == HAL_OK)  
  {
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.NbPages = (FLASH_PAGE_NBPERBANK - StartPageNum);
    pEraseInit.Page = StartPageNum;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
  }
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  if (status != HAL_OK)
  {
    /* Error occurred while page erase */
    return FLASHIF_ERASEKO;
  }

  return FLASHIF_OK;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t destination, uint8_t *p_source, uint32_t length)
{
  uint32_t status = FLASHIF_OK;
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* DataLength must be a multiple of 64 bit */
  for (i = 0; (i < length) && (destination <= (USER_FLASH_END_ADDRESS-8)); i+=8)
  {
    uint64_t n;
    memcpy(&n, p_source + i, sizeof(uint64_t));
    const uint64_t dataToWrite = n;
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, destination, dataToWrite ) == HAL_OK)
    {
     /* Check the written value */
      if (*(uint64_t*)destination != dataToWrite)
      {
        /* Flash content doesn't match SRAM content */
        status = FLASHIF_WRITINGCTRL_ERROR;
        break;
      }
      /* Increment FLASH destination address */
      destination += 8;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      status = FLASHIF_WRITING_ERROR;
      break;
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return status;
}

/**
  * @brief  Returns the write protection status of application flash area.
  * @param  None
  * @retval If a sector in application area is write-protected returned value is a combinaison
            of the possible values : FLASHIF_PROTECTION_WRPENABLED, FLASHIF_PROTECTION_PCROPENABLED, ...
  *         If no sector is write-protected FLASHIF_PROTECTION_NONE is returned.
  */
uint32_t FLASH_If_GetWriteProtectionStatus(void)
{
  uint32_t ProtectedPAGE = FLASHIF_PROTECTION_NONE;
  FLASH_OBProgramInitTypeDef OptionsBytesStruct1, OptionsBytesStruct2;
  
  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();
 
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_ZONE_A;
  OptionsBytesStruct1.PCROPConfig = FLASH_BANK_1;
  OptionsBytesStruct2.WRPArea = OB_WRPAREA_ZONE_B;
  OptionsBytesStruct2.PCROPConfig = FLASH_BANK_1;
  
  /* Check if there are write protected sectors inside the user flash area ***/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct1);
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct2);
  
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
  
  /* Check PCROP areas */
  if(OptionsBytesStruct1.PCROP1AEndAddr > OptionsBytesStruct1.PCROP1AStartAddr)
  {
    /* check if user area are included inside this range */
    if(OptionsBytesStruct1.PCROP1AStartAddr > APPLICATION_ADDRESS) 
    {
      ProtectedPAGE|= FLASHIF_PROTECTION_PCROPENABLED;
    }
  }
  
  if(OptionsBytesStruct2.PCROP1BEndAddr > OptionsBytesStruct2.PCROP1BStartAddr)
  {
    /* check if user area are included inside this range */
    if(OptionsBytesStruct1.PCROP1BStartAddr > APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_PCROPENABLED;
    }
  }
  
  /* check WRP */
  if(OptionsBytesStruct1.WRPEndOffset > OptionsBytesStruct1.WRPStartOffset)
  {
    /* check if area is inside the WRP Range */ 
    if((OptionsBytesStruct1.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE) >= APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_WRPENABLED;
    }   
  }

  if(OptionsBytesStruct2.WRPEndOffset > OptionsBytesStruct2.WRPStartOffset)
  {
    /* check if area is inside the WRP Range */ 
    if((OptionsBytesStruct2.WRPStartOffset * FLASH_PAGE_SIZE + FLASH_BASE) >= APPLICATION_ADDRESS)
    {
       ProtectedPAGE|= FLASHIF_PROTECTION_WRPENABLED;
    }   
  }

  
  if(OptionsBytesStruct2.RDPLevel == 1)
  {
    ProtectedPAGE|= FLASHIF_PROTECTION_RDPENABLED;
  }
    
  return ProtectedPAGE;
}

/**
  * @brief  Configure the write protection status of user flash area.
  * @param  protectionstate : FLASHIF_WRP_DISABLE or FLASHIF_WRP_ENABLE the protection
  * @retval uint32_t FLASHIF_OK if change is applied.
  */
uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate)
{
  FLASH_OBProgramInitTypeDef OptionsBytesStruct1;
  HAL_StatusTypeDef retr;
  
  /* Unlock the Flash to enable the flash control register access *************/
  retr = HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  retr|= HAL_FLASH_OB_Unlock();
  
  OptionsBytesStruct1.OptionType = OPTIONBYTE_WRP;
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_ZONE_A;
  if( protectionstate == FLASHIF_WRP_ENABLE)
  {
    /* Enable the WRP protection for all flash BANK1 */
    OptionsBytesStruct1.WRPEndOffset = FLASH_PAGE_NBPERBANK - 1;
    OptionsBytesStruct1.WRPStartOffset = 0x00;
  }
  else
  {
    /* Remove all WRP protection */
    OptionsBytesStruct1.WRPEndOffset = 0x00;
    OptionsBytesStruct1.WRPStartOffset = 0xFF;
  }
  retr|= HAL_FLASHEx_OBProgram(&OptionsBytesStruct1);
  
  OptionsBytesStruct1.RDPLevel = OB_RDP_LEVEL_0;
  OptionsBytesStruct1.OptionType = OPTIONBYTE_WRP;
  OptionsBytesStruct1.WRPArea = OB_WRPAREA_ZONE_B;
  OptionsBytesStruct1.WRPEndOffset = 0x00;
  OptionsBytesStruct1.WRPStartOffset = 0xFF;
  retr|= HAL_FLASHEx_OBProgram(&OptionsBytesStruct1);
    
  
  
  return (retr == HAL_OK ? FLASHIF_OK: FLASHIF_PROTECTION_ERRROR);
}

//################################  Below functions are Added By Mukesh ###################################
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Added below By Mukesh for erasing the specific page.
/**
* @brief  This function erases a page of flash.
*         The Page Erase bit (PER) is set at the beginning and reset at the end
*         of the function, in case of successive erase, these two operations
*         could be performed outside the function.
* @param  page_addr is an address inside the page to erase
* @retval None
*/
void FlashErasePage(uint32_t page_addr)
{   
	uint32_t FirstPage = 0; //NbOfPages = 0;
	uint32_t PageError = 0;
	/*Variable used for Erase procedure*/
	static FLASH_EraseInitTypeDef EraseInitStruct = {0};
	
	/* Get the 1st page to erase */
	FirstPage = GetPage(page_addr);
	
	/* Get the number of pages to erase from 1st page */
	// NbOfPages = GetPage(FLASH_USER_END_ADDR) - FirstPage + 1;
	
	/* Fill EraseInit structure*/
    EraseInitStruct.Banks = FLASH_BANK_1;
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page        = FirstPage;
	EraseInitStruct.NbPages     = 1;//NbOfPages;
	
	/*Erase Page*/
	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
}

/**
* @brief  Gets the page of a given address
* @param  Addr: Address of the FLASH Memory
* @retval The page of a given address
*/
uint32_t GetPage(uint32_t Addr)
{
	return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//################################################################################################
/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
