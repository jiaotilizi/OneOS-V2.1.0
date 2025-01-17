/**
  ******************************************************************************
  * @file    hk32f39a_flash.c
  * @author  Jane.L
  * @version V1.0.0
  * @brief   This file provides all the FLASH firmware functions.
  * @changelist
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "hk32f39a_flash.h"
#include "hk32f39a_cache.h"

/** @addtogroup HK32F39A_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH 
  * @brief FLASH driver modules
  * @{
  */ 

/** @defgroup FLASH_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Defines
  * @{
  */ 

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE) 
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

#define ECR_HPER_Set               ((uint32_t)0x00000001)
#define ECR_HPER_Reset             ((uint32_t)0x000001FE)
#define ECR_WPG_Set                ((uint32_t)0x00000002)
#define ECR_WPG_Reset              ((uint32_t)0x000001FD) 
#define ECR_2WPG_Set               ((uint32_t)0x00000004)
#define ECR_2WPG_Reset             ((uint32_t)0x000001FB)
#define ECR_4WPG_Set               ((uint32_t)0x00000008)
#define ECR_4WPG_Reset             ((uint32_t)0x000001F7)
#define ECR_Inf_HPER_Set           ((uint32_t)0x00000010)
#define ECR_Inf_HPER_Reset         ((uint32_t)0x000001EF)
#define ECR_Inf_HWPG_Set           ((uint32_t)0x00000020)
#define ECR_Inf_HWPG_Reset         ((uint32_t)0x000001DF)
#define ECR_Inf_WPG_Set            ((uint32_t)0x00000040)
#define ECR_Inf_WPG_Reset          ((uint32_t)0x000001BF)
#define ECR_Inf_2WPG_Set           ((uint32_t)0x00000080)
#define ECR_Inf_2WPG_Reset         ((uint32_t)0x0000017F)
#define ECR_Inf_4WPG_Set           ((uint32_t)0x00000100)
#define ECR_Inf_4WPG_Reset         ((uint32_t)0x000000FF)

/* FLASH Mask */
#define RDPRT_Mask               ((uint32_t)0x00000002)
#define WRP0_Mask                ((uint32_t)0x000000FF)
#define WRP1_Mask                ((uint32_t)0x0000FF00)
#define WRP2_Mask                ((uint32_t)0x00FF0000)
#define WRP3_Mask                ((uint32_t)0xFF000000)
//#define OB_USER_BFB2             ((uint16_t)0x0008)//jane cancel, the default value is 1 in bit19

/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
#define FLASH_Encry              ((uint32_t)0x1357eca8)
#define FLASH_Decry              ((uint32_t)0x2468db97)
#define FLASH_IWDGRLIVCMD        ((uint32_t)0xa5a55b1e)
#define FLASH_LSILPCTL           ((uint32_t)0x369CF0F0)
#define FLASH_DebugCMD           ((uint32_t)0x1234bcde)

/* Delay definition */   
#define EraseTimeout          ((uint32_t)0x000B0000)
#define ProgramTimeout        ((uint32_t)0x0000F000)
/**
  * @}
  */ 

/** @defgroup FLASH_Private_Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_Variables
  * @{
  */

/**
  * @}
  */ 

/** @defgroup FLASH_Private_FunctionPrototypes
  * @{
  */
  
/**
  * @}
  */

/** @defgroup FLASH_Private_Functions
  * @{
  */

/**
@code  
 
 This driver provides functions to configure and program the Flash memory of all HK32F39A devices. 

 HK32F39A devices feature up to 512kbytes with dual bank architecture for read-while-write (RWW) capability:
 While other HK32F39A devices features only one bank with memory up to 512 Kbytes.

 In version V3.3.0, some functions were updated and new ones were added to support
 HK32F39A_XL devices. Thus some functions manages all devices, while other are 
 dedicated for XL devices only.
 
 The table below presents the list of available functions depending on the used HK32F39A devices.  
      
   ***************************************************
   * Legacy functions used for all HK32F39A devices *
   ***************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |HK32F39A    |   HK32F39A    |    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_SetLatency                    |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_HalfCycleAccessCmd            |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_PrefetchBufferCmd             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_Unlock                        |    Yes     |      Yes      | - For HK32F39A devices: unlock Bank1 and it is equivalent.     |
   |                                    |            |               |   to FLASH_UnlockBank1 function.                               |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_Lock                          |    Yes     |      Yes      | - For HK32F39A devices: lock Bank1 and it is equivalent.       |
   |                                    |            |               |   to FLASH_LockBank1 function.                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ErasePage                     |    Yes     |      Yes      | - For HK32F39A devices: erase a page in Bank1                  |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EraseAllPages                 |    Yes     |      Yes      | - For HK32F39A devices: erase all pages in Bank1               |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EraseOptionBytes              |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramWord,2 and 4 words     |    Yes     |      Yes      | Updated to program up to 1MByte (depending on the used device) |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramHalfWord               |    Yes     |      Yes      | Updated to program up to 1MByte (depending on the used device) |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ProgramOptionByteData         |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_EnableWriteProtection         |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ReadOutProtection             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_UserOptionByteConfig          |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetUserOptionByte             |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetWriteProtectionOptionByte  |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetReadOutProtectionStatus    |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetPrefetchBufferStatus       |    Yes     |      Yes      | No change                                                      |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ITConfig                      |    Yes     |      Yes      | - For HK32F39A devices: enable Bank1's interrupts              |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetFlagStatus                 |    Yes     |      Yes      | - For HK32F39A devices: return Bank1's flag status             |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_ClearFlag                     |    Yes     |      Yes      | - For HK32F39A devices: clear Bank1's flag                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_GetStatus                     |    Yes     |      Yes      | - Return the status of Bank1 (for all devices)                 |
   |                                    |            |               |   equivalent to FLASH_GetBank1Status function                  |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_WaitForLastOperation          |    Yes     |      Yes      | - Wait for Bank1 last operation (for all devices)              |
   |                                    |            |               |   equivalent to: FLASH_WaitForLastBank1Operation function      |
   +----------------------------------------------------------------------------------------------------------------------------------+

   ************************************************************************************************************************
   * New functions used for all HK32F39A devices to manage Bank1:                                                         *
   *   - These functions have separate control for Bank1 and bank2(39A 512K flash has bank1 only                          *
   *   - For other devices, these functions are optional (covered by functions listed above)                              *
   ************************************************************************************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |HK32F39A    | HK32F39A      |    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_UnlockBank1                  |    Yes     |      Yes      | - Unlock Bank1                                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_LockBank1                     |    Yes     |      Yes      | - Lock Bank1                                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_EraseAllBank1Pages           |    Yes     |      Yes      | - Erase all pages in Bank1                                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_GetBank1Status               |    Yes     |      Yes      | - Return the status of Bank1                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_WaitForLastBank1Operation    |    Yes     |      Yes      | - Wait for Bank1 last operation                                |
   +----------------------------------------------------------------------------------------------------------------------------------+

   *****************************************************************************
   * New Functions used only with 1M flash devices to manage Bank2 *
   *****************************************************************************
   +----------------------------------------------------------------------------------------------------------------------------------+
   |       Functions prototypes         |HK32F39A    |  HK32F39A     |    Comments                                                    |
   |                                    |   devices  |  devices      |                                                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_UnlockBank2                  |    No      |      No       | - Unlock Bank2                                                 |
   |----------------------------------------------------------------------------------------------------------------------------------|
   |FLASH_LockBank2                     |    No      |      No       | - Lock Bank2                                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_EraseAllBank2Pages           |    No      |      No       | - Erase all pages in Bank2                                     |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_GetBank2Status               |    No      |      No       | - Return the status of Bank2                                   |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_WaitForLastBank2Operation    |    No      |      No       | - Wait for Bank2 last operation                                |
   |----------------------------------------------------------------------------------------------------------------------------------|
   | FLASH_BootConfig                   |    No      |      No       | - Configure to boot from Bank1 or Bank2                        |
   +----------------------------------------------------------------------------------------------------------------------------------+
@endcode
*/


/**
  * @brief  Sets the code latency value.
  * @note   This function can be used for all HK32F39A devices.
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *   This parameter can be one of the following values:
  *     @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *     @arg FLASH_Latency_1: FLASH One Latency cycle
  *     @arg FLASH_Latency_2: FLASH Two Latency cycles
  *      ......
  *     @arg FLASH_Latency_31: FLASH Thirty-onw Latency cycles
  * @retval None
  *///jane changed
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
  uint32_t tmpreg = 0,tmpreg1 = 0;
  
  /* Check the parameters */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* Read the ACR register */
  tmpreg = FLASH->ACR;  
  
  /* Sets the Latency value */
  tmpreg &= ACR_LATENCY_Mask;
  tmpreg |= FLASH_Latency & 0x07;
  
  /* Sets the Latency-ex value */
  tmpreg1 |= FLASH_Latency >>3;
  
  /* Write the ACR register */
  FLASH->ACR = tmpreg;
  FLASH->LATENCY_EX = tmpreg1;
}

/**
  * @brief  Enables or disables the Half cycle flash access.
  * @note   This function can be used for all HK32F39A devices.
  * @param  FLASH_HalfCycleAccess: specifies the FLASH Half cycle Access mode.
  *   This parameter can be one of the following values:
  *     @arg FLASH_HalfCycleAccess_Enable: FLASH Half Cycle Enable
  *     @arg FLASH_HalfCycleAccess_Disable: FLASH Half Cycle Disable
  * @retval None
  */
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess)
{
  /* Check the parameters */
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));
  
  /* Enable or disable the Half cycle access */
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/**
  * @brief  Enables or disables the Prefetch Buffer.
  * @note   This function can be used for all HK32F39A devices.
  * @param  FLASH_PrefetchBuffer: specifies the Prefetch buffer status.
  *   This parameter can be one of the following values:
  *     @arg FLASH_PrefetchBuffer_Enable: FLASH Prefetch Buffer Enable
  *     @arg FLASH_PrefetchBuffer_Disable: FLASH Prefetch Buffer Disable
  * @retval None
  */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
  /* Check the parameters */
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* Enable or disable the Prefetch Buffer */
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

/**
  * @brief  Unlocks the FLASH Program Erase Controller.
  * @note   This function can be used for all HK32F39A devices.
  *         - For all other devices it unlocks Bank1 and it is equivalent 
  *           to FLASH_UnlockBank1 function.. 
  * @param  None
  * @retval None
  */
void FLASH_Unlock(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/**
  * @brief  Locks the FLASH Program Erase Controller.
  * @note   This function can be used for all HK32F39A devices.
  *         - For HK32F10X_XL devices this function Locks Bank1 and Bank2.
  *         - For all other devices it Locks Bank1 and it is equivalent 
  *           to FLASH_LockBank1 function.
  * @param  None
  * @retval None
  */
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank1 */
  FLASH->CR |= CR_LOCK_Set;
}

/**
  * @brief  Erases a specified FLASH half page.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Page_Address: The half page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseHalfPage(uint32_t HPage_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(HPage_Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the half page */
    FLASH->ECR|= ECR_HPER_Set;
    FLASH->AR = HPage_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    /* Disable the HPER Bit */
    FLASH->ECR &= ECR_HPER_Reset;
  }
  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Erases a specified information block half page.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Page_Address: The half page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASHInfo_EraseHalfPage(uint32_t InfPage_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_INF_ADDRESS(InfPage_Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
		FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    /* if the previous operation is completed, proceed to erase the half page */
    FLASH->ECR|= ECR_Inf_HPER_Set;
    FLASH->AR = InfPage_Address; 
		CACHE->CTL |= 0x0800;
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    /* Disable the HPER Bit */
    FLASH->ECR &= ECR_Inf_HPER_Reset;
  }
  /* Return the Erase Status */
  return status;
}


/**
  * @brief  Erases a specified FLASH page.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Page_Address: The page address to be erased.
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Page_Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* if the previous operation is completed, proceed to erase the page */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
		CACHE->CTL |= 0x0800;
    FLASH->CR|= CR_STRT_Set;
    
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    /* Disable the PER Bit */
    FLASH->CR &= CR_PER_Reset;
  }

  /* Return the Erase Status */
  return status;
}

/**
  * @brief  Erases all FLASH pages.
  * @note   This function can be used for all HK32F39A devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to erase all pages */
	FLASH->CR |= CR_MER_Set;
	CACHE->CTL |= 0x0800;
	FLASH->CR |= CR_STRT_Set;

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(EraseTimeout);

	/* Disable the MER Bit */
	FLASH->CR &= CR_MER_Reset;
  }

  /* Return the Erase Status */
  return status;
}


/**
  * @brief  Erases the FLASH option bytes.
  * @note   This functions erases all option bytes except the Read protection (RDP). 
  * @note   This function can be used for all HK32F39A devices.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseOptionBytes(void)
{
  uint16_t rdptmp = RDP_Key;

  FLASH_Status status = FLASH_COMPLETE;

  /* Get the actual read protection Option Byte value */ 
  if(FLASH_GetReadOutProtectionStatus() != RESET)
  {
    rdptmp = 0x00;  
  }

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* Authorize the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* if the previous operation is completed, proceed to erase the option bytes */
    FLASH->CR |= CR_OPTER_Set;
		CACHE->CTL |= 0x0800;
    FLASH->CR |= CR_STRT_Set;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit */
      FLASH->CR &= CR_OPTER_Reset;
       
      /* Enable the Option Bytes Programming operation */
      FLASH->CR |= CR_OPTPG_Set;
      /* Restore the last read protection Option Byte value */
      OB->RDP = (uint16_t)rdptmp; 
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* Disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }  
  }
  /* Return the erase status */
  return status;
}

/**
  * @brief  Programs a word at a specified address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __IO uint32_t tmp = 0;

  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first 
    half word */
	FLASH->ECR |= ECR_WPG_Set;

	*(__IO uint32_t*)Address = (uint32_t)Data;
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);

	/* Disable the PG Bit */
	FLASH->ECR &= ECR_WPG_Reset;
  }         
   
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a word at a specified information block address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASHInfo_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_INF_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
	FLASH->OPTKEYR = FLASH_KEY1;
	FLASH->OPTKEYR = FLASH_KEY2;
	/* if the previous operation is completed, proceed to program the new first 
	half word */
	FLASH->ECR |= ECR_Inf_WPG_Set;
	CACHE->CTL |= 0x0800;
	*(__IO uint32_t*)Address = (uint32_t)Data;
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);

	/* Disable the PG Bit */
	FLASH->ECR &= ECR_Inf_WPG_Reset;
  }         
   
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a 2 words at a specified address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_Program2Words(uint32_t Address, uint32_t DataL, uint32_t DataH)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first 
    half word */
	FLASH->ECR |= ECR_2WPG_Set;

	FLASH->PW0 = DataL;
	FLASH->PW1 = DataH;

	FLASH->AR = Address;

	CACHE->CTL |= 0x0800;             /* modified by veiko */
	FLASH->CR |= CR_STRT_Set;
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);

	/* Disable the PG Bit */
	FLASH->ECR &= ECR_2WPG_Reset;
  }         
   
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a 2 words at a specified information block address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASHInfo_Program2Words(uint32_t Address, uint32_t DataL, uint32_t DataH)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_INF_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
	FLASH->OPTKEYR = FLASH_KEY1;
	FLASH->OPTKEYR = FLASH_KEY2;
	/* if the previous operation is completed, proceed to program the new first 
	half word */
	FLASH->ECR |= ECR_Inf_2WPG_Set;

	FLASH->PW0 = DataL;
	FLASH->PW1 = DataH;

	FLASH->AR = Address;
	CACHE->CTL |= 0x0800;             /* modified by veiko */
	FLASH->CR |= CR_STRT_Set;
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);

	/* Disable the PG Bit */
	FLASH->ECR &= ECR_Inf_2WPG_Reset;										
  }         
   
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a 4 words at a specified address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_Program4Words(uint32_t Address, uint32_t *Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new first 
    half word */
	FLASH->ECR |= ECR_4WPG_Set;

	FLASH->PW0 = Data[0];
	FLASH->PW1 = Data[1];
	FLASH->PW2 = Data[2];
	FLASH->PW3 = Data[3];

	FLASH->AR = Address;
	CACHE->CTL |= 0x0800;             /* modified by veiko */
	FLASH->CR |= CR_STRT_Set;
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);

	/* Disable the PG Bit */
	FLASH->ECR &= ECR_4WPG_Reset;
  }         
   
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a 4 words at a specified information block address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASHInfo_Program4Words(uint32_t Address, uint32_t *Data)
{
  FLASH_Status status = FLASH_COMPLETE;

  /* Check the parameters */
  assert_param(IS_FLASH_INF_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
	FLASH->OPTKEYR = FLASH_KEY1;
	FLASH->OPTKEYR = FLASH_KEY2;
	/* if the previous operation is completed, proceed to program the new first 
	half word */
	FLASH->ECR |= ECR_Inf_4WPG_Set;

	FLASH->PW0 = Data[0];
	FLASH->PW1 = Data[1];
	FLASH->PW2 = Data[2];
	FLASH->PW3 = Data[3];

	FLASH->AR = Address;
	CACHE->CTL |= 0x0800;             /* modified by veiko */
	FLASH->CR |= CR_STRT_Set;
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);

	/* Disable the 4WPG Bit */
	FLASH->ECR &= ECR_Inf_4WPG_Reset;
  }         
   
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a half word at a specified address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* if the previous operation is completed, proceed to program the new data */
    FLASH->CR |= CR_PG_Set;
    CACHE->CTL |= 0x0800;
    *(__IO uint16_t*)Address = Data;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    
    /* Disable the PG Bit */
    FLASH->CR &= CR_PG_Reset;
  } 
  
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a half word at a specified information block address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASHInfo_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FLASH_INF_ADDRESS(Address));

  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
	FLASH->OPTKEYR = FLASH_KEY1;
	FLASH->OPTKEYR = FLASH_KEY2;
	/* if the previous operation is completed, proceed to program the new data */
	FLASH->ECR |= ECR_Inf_HWPG_Set;
	CACHE->CTL |= 0x0800;
	*(__IO uint16_t*)Address = Data;
	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);

	/* Disable the PG Bit */
	FLASH->ECR &= ECR_Inf_HWPG_Reset;
  } 
  
  /* Return the Program Status */
  return status;
}

/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Address: specifies the address to be programmed.
  *   This parameter can be 0x1FFFF804 or 0x1FFFF806. 
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_OB_DATA_ADDRESS(Address));
  status = FLASH_WaitForLastOperation(ProgramTimeout);

  if(status == FLASH_COMPLETE)
  {
	/* Authorize the small information block programming */
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
	FLASH->OPTKEYR = FLASH_KEY1;
	FLASH->OPTKEYR = FLASH_KEY2;
	/* Enables the Option Bytes Programming operation */
	FLASH->CR |= CR_OPTPG_Set; 
	*(__IO uint16_t*)Address = Data;

	/* Wait for last operation to be completed */
	status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }
  /* Return the Option Byte Data Program Status */
  return status;
}

/**
  * @brief  Write protects the desired pages
  * @note   This function can be used for all HK32F39A devices.
  * @param  FLASH_Pages: specifies the address of the pages to be write protected.
  *   This parameter can be:
  *     @arg: two pages from 0~61 and 62~255 protect together  
  *     @arg FLASH_WRProt_AllPages
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
  uint16_t WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;
  
  FLASH_Status status = FLASH_COMPLETE;
  
  /* Check the parameters */
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  
  FLASH_Pages = (uint32_t)(~FLASH_Pages);
  WRP0_Data = (uint16_t)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (uint16_t)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (uint16_t)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (uint16_t)((FLASH_Pages & WRP3_Mask) >> 24);
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;
    if(WRP0_Data != 0xFF)
    {
		CACHE->CTL |= 0x0800;
		OB->WRP0 = WRP0_Data;

		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
		CACHE->CTL |= 0x0800;
		OB->WRP1 = WRP1_Data;

		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
		CACHE->CTL |= 0x0800;
		OB->WRP2 = WRP2_Data;

		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    
    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
		CACHE->CTL |= 0x0800;
		OB->WRP3 = WRP3_Data;

		/* Wait for last operation to be completed */
		status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
          
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  } 
  /* Return the write protection operation Status */
  return status;       
}

/**
  * @brief  Enables or disables the read out protection.
  * @note   If the user has already programmed the other option bytes before calling 
  *   this function, he must re-program them since this function erases all option bytes.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Newstate: new state of the ReadOut Protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* Authorizes the small information block programming */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTER_Set;
		CACHE->CTL |= 0x0800;
    FLASH->CR |= CR_STRT_Set;
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* if the erase operation is completed, disable the OPTER Bit */
      FLASH->CR &= CR_OPTER_Reset;
      /* Enable the Option Bytes Programming operation */
      FLASH->CR |= CR_OPTPG_Set; 
      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;  
      }
      /* Wait for last operation to be completed */
      status = FLASH_WaitForLastOperation(EraseTimeout); 
    
      if(status != FLASH_TIMEOUT)
      {
        /* if the program operation is completed, disable the OPTPG Bit */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else 
    {
      if(status != FLASH_TIMEOUT)
      {
        /* Disable the OPTER Bit */
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* Return the protection operation Status */
  return status;       
}

/**
  * @brief  Programs the FLASH User Option Byte: IWDG_SW / RST_STOP / RST_STDBY.
  * @note   This function can be used for all HK32F39A devices.
  * @param  OB_IWDG: Selects the IWDG mode
  *   This parameter can be one of the following values:
  *     @arg OB_IWDG_SW: Software IWDG selected
  *     @arg OB_IWDG_HW: Hardware IWDG selected
  * @param  OB_STOP: Reset event when entering STOP mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STOP_NoRST: No reset generated when entering in STOP
  *     @arg OB_STOP_RST: Reset generated when entering in STOP
  * @param  OB_STDBY: Reset event when entering Standby mode.
  *   This parameter can be one of the following values:
  *     @arg OB_STDBY_NoRST: No reset generated when entering in STANDBY
  *     @arg OB_STDBY_RST: Reset generated when entering in STANDBY
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG, 
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* Check the parameters */
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* Authorize the small information block programming */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Enable the Option Bytes Programming operation */
    FLASH->CR |= CR_OPTPG_Set; 
           
    OB->USER = OB_IWDG | (uint16_t)(OB_STOP | (uint16_t)(OB_STDBY | ((uint16_t)0xF8))); 
  
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  Returns the FLASH User Option Bytes values.
  * @note   This function can be used for all HK32F39A devices.
  * @param  None
  * @retval The FLASH User Option Bytes values:IWDG_SW(Bit0), RST_STOP(Bit1)
  *         and RST_STDBY(Bit2).
  */
uint32_t FLASH_GetUserOptionByte(void)
{
  /* Return the User Option Byte */
  return (uint32_t)(FLASH->OBR >> 2);
}

/**
  * @brief  Returns the FLASH Write Protection Option Bytes Register value.
  * @note   This function can be used for all HK32F39A devices.
  * @param  None
  * @retval The FLASH Write Protection  Option Bytes Register value
  */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
  /* Return the Flash write protection Register value */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @brief  Checks whether the FLASH Read Out Protection Status is set or not.
  * @note   This function can be used for all HK32F39A devices.
  * @param  None
  * @retval FLASH ReadOut Protection Status(SET or RESET)
  */
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;
  if ((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/**
  * @brief  Checks whether the FLASH Prefetch Buffer status is set or not.
  * @note   This function can be used for all HK32F39A devices.
  * @param  None
  * @retval FLASH Prefetch Buffer Status (SET or RESET).
  */
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
  FlagStatus bitstatus = RESET;
  
  if ((FLASH->ACR & ACR_PRFTBS_Mask) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) */
  return bitstatus; 
}

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @note   This function can be used for all HK32F39A devices.
  *         -it enables or disables the specified FLASH interrupts for Bank1.
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *     @arg FLASH_IT_ERROR: FLASH Error Interrupt
  *     @arg FLASH_IT_EOP: FLASH end of operation Interrupt
  * @param  NewState: new state of the specified Flash interrupts.
  *   This parameter can be: ENABLE or DISABLE.      
  * @retval None 
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if(NewState != DISABLE)
  {
    /* Enable the interrupt sources */
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* Disable the interrupt sources */
    FLASH->CR &= ~(uint32_t)FLASH_IT;
  }
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @note   This function can be used for all HK32F39A devices.
  *         - it checks whether the specified Bank1 flag is 
  *           set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *     @arg FLASH_FLAG_BSY: FLASH Busy flag           
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag       
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag      
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag           
  *     @arg FLASH_FLAG_OPTERR:  FLASH Option Byte error flag     
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;

  /* Check the parameters */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR) 
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
   if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }

  /* Return the new state of FLASH_FLAG (SET or RESET) */
  return bitstatus;
}

/**
  * @brief  Clears the FLASH's pending flags.
  * @note   This function can be used for all HK32F39A devices.
  *         -  it clears Bank1�s pending flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:         
  *     @arg FLASH_FLAG_PGERR: FLASH Program error flag       
  *     @arg FLASH_FLAG_WRPRTERR: FLASH Write protected error flag      
  *     @arg FLASH_FLAG_EOP: FLASH End of Operation flag           
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
  /* Check the parameters */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* Clear the flags */
  FLASH->SR = FLASH_FLAG;
}

/**
  * @brief  Returns the FLASH Status.
  * @note   This function can be used for all HK32F39A devices, it is equivalent
  *         to FLASH_GetBank1Status function.
  * @param  None
  * @retval FLASH Status: The returned value can be: FLASH_BUSY, FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP or FLASH_COMPLETE
  */
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* Return the Flash Status */
  return flashstatus;
}

/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @note   This function can be used for all HK32F39A devices, 
  *         it is equivalent to FLASH_WaitForLastBank1Operation.
  *         -  it waits for a Flash operation to complete 
  *           or a TIMEOUT to occur.
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* Check for the Flash Status */
  status = FLASH_GetStatus();
  /* Wait for a Flash operation to complete or a TIMEOUT to occur */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    status = FLASH_GetStatus();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* Return the operation status */
  return status;
}

/**
  * @brief  Programs the FLASH Option Byte RLR interval time.
  * @note   This function can be used for all HK32F39A devices.
  * @param  OB_IWDGRLIV: set the RLR interval value
  *   This parameter can be one of the following values0~0xffff:
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG, 
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OptionIWDG_RLIV(uint16_t OB_IWDGRLIV)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* Authorize the small information block programming */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Enable the Option Bytes Programming operation */
    FLASH->CR |= CR_OPTPG_Set; 
           
    OB->IWDG_RL_IV = OB_IWDGRLIV; 
  
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  Programs the FLASH Option Byte to enable or disable RLR interval time.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Newstate: new state of the ReadOut Protection.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG, 
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OptionIWDG_RLIV_CMD(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE; 
	
	if(NewState != DISABLE)
	{
    	OB->IWDG_INI_KEY = FLASH_IWDGRLIVCMD; 
		
		status=FLASH_ProgramOptionByteData(0x1ffff834, (FLASH_IWDGRLIVCMD)&0xffff);
		status=FLASH_ProgramOptionByteData(0x1ffff836, (FLASH_IWDGRLIVCMD>>16)&0xffff);
		
	}
	else
	{
		status=FLASH_ProgramOptionByteData(0x1ffff834, 0x0);
		status=FLASH_ProgramOptionByteData(0x1ffff836, 0x0);
	}

  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  Programs the FLASH can be WAKE up by IWDG or not when in STOP and STANDBY mode.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Newstate: new state of the ReadOut Protection.
  *   This parameter can be: ENABLE: cannot wake up by IWDG in stop and standby
  *                          DISABLE: can be wake up by IWDG in stop and standby.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG, 
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_Option_LSI_CMD(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* Authorize the small information block programming */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Enable the Option Bytes Programming operation */
    FLASH->CR |= CR_OPTPG_Set; 
	if(NewState != DISABLE)
	{
    	OB->LSI_LP_CTL = FLASH_LSILPCTL; 
	}
	else
	{
		OB->IWDG_INI_KEY = 0;
	}
  
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @brief  enable or disable Debug clock.
  * @note   This function can be used for all HK32F39A devices.
  * @param  Newstate: new state of the ReadOut Protection.
  *   This parameter can be: ENABLE: close Debug clock
  *                          DISABLE: open Debug clock.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG, 
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_Option_Debug_CMD(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* Authorize the small information block programming */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* Wait for last operation to be completed */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* Enable the Option Bytes Programming operation */
    FLASH->CR |= CR_OPTPG_Set; 
	if(NewState != DISABLE)
	{
    	OB->LSI_LP_CTL = FLASH_DebugCMD; 
	}
	else
	{
		OB->IWDG_INI_KEY = 0;
	}
  
    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* if the program operation is completed, disable the OPTPG Bit */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* Return the Option Byte program Status */
  return status;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT   HKMicroChip *****END OF FILE****/
