/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "dma.h"

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init ( void )
{

    /* Init with LL driver */
    /* DMA controller clock enable */
    LL_AHB1_GRP1_EnableClock ( LL_AHB1_GRP1_PERIPH_DMA1 );

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    NVIC_SetPriority ( DMA1_Channel1_IRQn,
                       NVIC_EncodePriority ( NVIC_GetPriorityGrouping(), 0, 0 ) );
    NVIC_EnableIRQ ( DMA1_Channel1_IRQn );

}

/* USER CODE BEGIN 2 */
/* USER CODE END 2 */

