/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_dma.h"
#include "fsl_common.h"
#include "fsl_reset.h"
#include "fsl_usart.h"
#include "fsl_clock.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Used DMA device. */
#define DMA0_DMA_BASEADDR DMA0
/* Used DMA device. */
#define DMA1_DMA_BASEADDR DMA1
/* DMA1 interrupt vector ID (number). */
#define DMA1_IRQN DMA1_IRQn

  /* Channel CH0 definitions */
/* Selected DMA channel number. */
#define DMA1_CH0_DMA_CHANNEL 0
/* Definition of peripheral ID */
#define USART0_PERIPHERAL ((USART_Type *)FLEXCOMM0)
/* Definition of the clock source frequency */
#define USART0_CLOCK_SOURCE 48000000UL
/* Rx transfer buffer size. */
#define USART0_RX_BUFFER_SIZE 10
/* Rx transfer buffer size. */
#define USART0_TX_BUFFER_SIZE 10

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern dma_handle_t DMA1_CH0_Handle;
extern const usart_config_t USART0_config;
extern usart_handle_t USART0_handle;
extern uint8_t USART0_rxBuffer[USART0_RX_BUFFER_SIZE];
extern const usart_transfer_t USART0_rxTransfer;
extern uint8_t USART0_txBuffer[USART0_TX_BUFFER_SIZE];
extern const usart_transfer_t USART0_txTransfer;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */