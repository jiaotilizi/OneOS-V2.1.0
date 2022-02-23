/**
  ******************************************************************************
  * @file    hk32f39a_iwdg.c
  * @author  Thomas.W
  * @version V1.0  
  * @brief   API file of IWDG module
  * @changelist  
  ******************************************************************************
  */ 

/*
  ============================================================================== 
                          ##### IWDG features #####
  ============================================================================== 
    [..] The IWDG can be started by either software or hardware (configurable
         through option byte).
             
    [..] The IWDG is clocked by its own dedicated low-speed clock (LSI) and
         thus stays active even if the main clock fails.
         Once the IWDG is started, the LSI is forced ON and cannot be disabled
         (LSI cannot be disabled too), and the counter starts counting down from 
         the reset value of 0xFFF. When it reaches the end of count value (0x000)
         a system reset is generated.
         The IWDG counter should be reloaded at regular intervals to prevent
         an MCU reset.
                             
    [..] The IWDG is implemented in the VDD voltage domain that is still functional
         in STOP and STANDBY mode (IWDG reset can wake-up from STANDBY).
              
    [..] IWDGRST flag in RCC_CSR register can be used to inform when a IWDG
         reset occurs.
              
    [..] Min-max timeout value @40KHz (LSI): ~0.1ms / ~28.3s
         The IWDG timeout may vary due to LSI frequency dispersion. HK32F39A
         devices provide the capability to measure the LSI frequency (LSI clock
         should be seleted as RTC clock which is internally connected to TIM10 CH1
         input capture). The measured value can be used to have an IWDG timeout with
         an acceptable accuracy. 
         For more information, please refer to the HK32F39A Reference manual.
            
                          ##### How to use this driver ##### 
  ============================================================================== 
    [..] This driver allows to use IWDG peripheral with either window option enabled
         or disabled. To do so follow one of the two procedures below.
    (#) Window option is enabled:    
        (++) Start the IWDG using IWDG_Enable() function, when the IWDG is used
             in software mode (no need to enable the LSI, it will be enabled
             by hardware).        
        (++) Enable write access to IWDG_PR and IWDG_RLR registers using
             IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable) function.
        (++) Configure the IWDG prescaler using IWDG_SetPrescaler() function.
        (++) Configure the IWDG counter value using IWDG_SetReload() function.
             This value will be loaded in the IWDG counter each time the counter
             is reloaded, then the IWDG will start counting down from this value.
        (++) Wait for the IWDG registers to be updated using IWDG_GetFlagStatus() function.
        (++) Configure the IWDG refresh window using IWDG_SetWindowValue() function.

    (#) Window option is disabled:    
        (++) Enable write access to IWDG_PR and IWDG_RLR registers using
             IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable) function.
        (++) Configure the IWDG prescaler using IWDG_SetPrescaler() function.
        (++) Configure the IWDG counter value using IWDG_SetReload() function.
             This value will be loaded in the IWDG counter each time the counter
             is reloaded, then the IWDG will start counting down from this value.
        (++) Wait for the IWDG registers to be updated using IWDG_GetFlagStatus() function.
        (++) reload the IWDG counter at regular intervals during normal operation 
             to prevent an MCU reset, using IWDG_ReloadCounter() function.
        (++) Start the IWDG using IWDG_Enable() function, when the IWDG is used
             in software mode (no need to enable the LSI, it will be enabled
             by hardware).
*/




/* Includes ------------------------------------------------------------------*/
#include "hk32f39a_iwdg.h"

/* KR register bit mask */
#define KR_KEY_RELOAD    ((uint16_t)0xAAAA)
#define KR_KEY_ENABLE    ((uint16_t)0xCCCC)

/**
  * @brief  Enables or disables write access to IWDG_PR and IWDG_RLR registers.
  * @param  IWDG_WriteAccess: new state of write access to IWDG_PR and IWDG_RLR registers.
  *   This parameter can be one of the following values:
  *     @arg IWDG_WriteAccess_Enable: Enable write access to IWDG_PR and IWDG_RLR registers
  *     @arg IWDG_WriteAccess_Disable: Disable write access to IWDG_PR and IWDG_RLR registers
  * @retval None
  */
void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess)
{
  /* Check the parameters */
  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));
  IWDG->KR = IWDG_WriteAccess;
}

/**
  * @brief  Sets IWDG Prescaler value.
  * @param  IWDG_Prescaler: specifies the IWDG Prescaler value.
  *   This parameter can be one of the following values:
  *     @arg IWDG_Prescaler_4: IWDG prescaler set to 4
  *     @arg IWDG_Prescaler_8: IWDG prescaler set to 8
  *     @arg IWDG_Prescaler_16: IWDG prescaler set to 16
  *     @arg IWDG_Prescaler_32: IWDG prescaler set to 32
  *     @arg IWDG_Prescaler_64: IWDG prescaler set to 64
  *     @arg IWDG_Prescaler_128: IWDG prescaler set to 128
  *     @arg IWDG_Prescaler_256: IWDG prescaler set to 256
  * @retval None
  */
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler)
{
  /* Check the parameters */
  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));
  IWDG->PR = IWDG_Prescaler;
}

/**
  * @brief  Sets IWDG Reload value.
  * @param  Reload: specifies the IWDG Reload value.
  *   This parameter must be a number between 0 and 0x0FFF.
  * @retval None
  */
void IWDG_SetReload(uint16_t Reload)
{
  /* Check the parameters */
  assert_param(IS_IWDG_RELOAD(Reload));
  IWDG->RLR = Reload;
}

/**
  * @brief  Reloads IWDG counter with value defined in the reload register
  *   (write access to IWDG_PR and IWDG_RLR registers disabled).
  * @param  None
  * @retval None
  */
void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_RELOAD;
}

/**
  * @brief  Sets the IWDG window value.
  * @param  WindowValue: specifies the window value to be compared to the downcounter.
  * @retval None
  */
void IWDG_SetWindowValue(uint16_t WindowValue)
{
  /* Check the parameters */
  assert_param(IS_IWDG_WINDOW_VALUE(WindowValue));
  IWDG->WINR = WindowValue;
}
/**
  * @brief  Enables IWDG (write access to IWDG_PR and IWDG_RLR registers disabled).
  * @param  None
  * @retval None
  */
void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_ENABLE;
}

/**
  * @brief  Checks whether the specified IWDG flag is set or not.
  * @param  IWDG_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *     @arg IWDG_FLAG_PVU: Prescaler Value Update on going
  *     @arg IWDG_FLAG_RVU: Reload Value Update on going
  * @retval The new state of IWDG_FLAG (SET or RESET).
  */
FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_IWDG_FLAG(IWDG_FLAG));
  if ((IWDG->SR & IWDG_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* Return the flag status */
  return bitstatus;
}