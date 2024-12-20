/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef hpcd_USB_FS;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler ( void )
{
    /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

    /* USER CODE END NonMaskableInt_IRQn 0 */
    /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while ( 1 )
    {
    }

    /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler ( void )
{
    /* USER CODE BEGIN HardFault_IRQn 0 */

    /* USER CODE END HardFault_IRQn 0 */
    while ( 1 )
    {
        /* USER CODE BEGIN W1_HardFault_IRQn 0 */
        /* USER CODE END W1_HardFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler ( void )
{
    /* USER CODE BEGIN MemoryManagement_IRQn 0 */

    /* USER CODE END MemoryManagement_IRQn 0 */
    while ( 1 )
    {
        /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
        /* USER CODE END W1_MemoryManagement_IRQn 0 */
    }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler ( void )
{
    /* USER CODE BEGIN BusFault_IRQn 0 */

    /* USER CODE END BusFault_IRQn 0 */
    while ( 1 )
    {
        /* USER CODE BEGIN W1_BusFault_IRQn 0 */
        /* USER CODE END W1_BusFault_IRQn 0 */
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler ( void )
{
    /* USER CODE BEGIN UsageFault_IRQn 0 */

    /* USER CODE END UsageFault_IRQn 0 */
    while ( 1 )
    {
        /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
        /* USER CODE END W1_UsageFault_IRQn 0 */
    }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler ( void )
{
    /* USER CODE BEGIN SVCall_IRQn 0 */

    /* USER CODE END SVCall_IRQn 0 */
    /* USER CODE BEGIN SVCall_IRQn 1 */

    /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler ( void )
{
    /* USER CODE BEGIN DebugMonitor_IRQn 0 */

    /* USER CODE END DebugMonitor_IRQn 0 */
    /* USER CODE BEGIN DebugMonitor_IRQn 1 */

    /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler ( void )
{
    /* USER CODE BEGIN PendSV_IRQn 0 */

    /* USER CODE END PendSV_IRQn 0 */
    /* USER CODE BEGIN PendSV_IRQn 1 */

    /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler ( void )
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler ( void )
{
    /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

    /* Half Transfer Complete Interrupt management ******************************/
    if ( ( ( LL_DMA_IsActiveFlag_HT1 ( DMA1 ) ) != 0U ) &&
            ( ( LL_DMA_IsEnabledIT_HT ( DMA1, LL_DMA_CHANNEL_1 ) ) != 0U ) )
    {
        /* Disable the half transfer interrupt if the DMA mode is not CIRCULAR */
        if ( ( LL_DMA_GetMode ( DMA1, LL_DMA_CHANNEL_1 ) ) == 0U )
        {
            /* Disable the half transfer interrupt */
            LL_DMA_DisableIT_HT ( DMA1, LL_DMA_CHANNEL_1 );
        }

        /* Clear the half transfer complete flag */
        LL_DMA_ClearFlag_HT1 ( DMA1 );

        /* DMA peripheral state is not updated in Half Transfer */
        /* but in Transfer Complete case */
    }

    /* Transfer Complete Interrupt management ***********************************/
    else if ( ( ( LL_DMA_IsActiveFlag_TC1 ( DMA1 ) ) != 0U ) &&
              ( ( LL_DMA_IsEnabledIT_TC ( DMA1, LL_DMA_CHANNEL_1 ) ) != 0U ) )
    {
        if ( ( LL_DMA_GetMode ( DMA1, LL_DMA_CHANNEL_1 ) ) == 0U )
        {
            /* Disable the transfer complete interrupt if the DMA mode is not CIRCULAR */
            /* Disable the transfer complete and error interrupt */
            /* if the DMA mode is not CIRCULAR  */
            LL_DMA_DisableIT_TC ( DMA1, LL_DMA_CHANNEL_1 );
            LL_DMA_DisableIT_TE ( DMA1, LL_DMA_CHANNEL_1 );
        }

        /* Clear the transfer complete flag */
        LL_DMA_ClearFlag_TC1 ( DMA1 );
    }

    /* Transfer Error Interrupt management **************************************/
    else if ( ( ( LL_DMA_IsActiveFlag_TE1 ( DMA1 ) ) != 0U ) &&
              ( ( LL_DMA_IsEnabledIT_TE ( DMA1, LL_DMA_CHANNEL_1 ) ) != 0U ) )
    {
        /* When a DMA transfer error occurs */
        /* A hardware clear of its EN bits is performed */
        /* Disable ALL DMA IT */
        LL_DMA_DisableIT_TC ( DMA1, LL_DMA_CHANNEL_1 );
        LL_DMA_DisableIT_HT ( DMA1, LL_DMA_CHANNEL_1 );
        LL_DMA_DisableIT_TE ( DMA1, LL_DMA_CHANNEL_1 );

        /* Clear all flags */
        LL_DMA_ClearFlag_TE1 ( DMA1 );
    }

    /* USER CODE END DMA1_Channel1_IRQn 0 */
    /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

    /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles USB event interrupt through EXTI line 17.
  */
void USB_IRQHandler ( void )
{
    /* USER CODE BEGIN USB_IRQn 0 */

    /* USER CODE END USB_IRQn 0 */
    HAL_PCD_IRQHandler (&hpcd_USB_FS );
    /* USER CODE BEGIN USB_IRQn 1 */

    /* USER CODE END USB_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
