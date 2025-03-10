/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init ( void )
{

    /* USER CODE BEGIN ADC1_Init 0 */

    /* USER CODE END ADC1_Init 0 */

    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_AHB2_GRP1_EnableClock ( LL_AHB2_GRP1_PERIPH_ADC );

    LL_AHB2_GRP1_EnableClock ( LL_AHB2_GRP1_PERIPH_GPIOA );
    LL_AHB2_GRP1_EnableClock ( LL_AHB2_GRP1_PERIPH_GPIOB );
    /**ADC1 GPIO Configuration
    PA0-CK_IN   ------> ADC1_IN5
    PB0   ------> ADC1_IN15
    PB1   ------> ADC1_IN16
    */
    GPIO_InitStruct.Pin = THERMISTOR_ADC_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init ( THERMISTOR_ADC_GPIO_Port, &GPIO_InitStruct );

    GPIO_InitStruct.Pin = VOLTMETER_ADC_Pin | AMPMETER_ADC_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init ( GPIOB, &GPIO_InitStruct );

    /* ADC1 DMA Init */

    /* ADC1 Init */
    LL_DMA_SetPeriphRequest ( DMA1, LL_DMA_CHANNEL_1, LL_DMA_REQUEST_0 );

    LL_DMA_SetDataTransferDirection ( DMA1, LL_DMA_CHANNEL_1,
                                      LL_DMA_DIRECTION_PERIPH_TO_MEMORY );

    LL_DMA_SetChannelPriorityLevel ( DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW );

    LL_DMA_SetMode ( DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR );

    LL_DMA_SetPeriphIncMode ( DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT );

    LL_DMA_SetMemoryIncMode ( DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT );

    LL_DMA_SetPeriphSize ( DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD );

    LL_DMA_SetMemorySize ( DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD );

    /* USER CODE BEGIN ADC1_Init 1 */

    /* USER CODE END ADC1_Init 1 */

    /** Common config
    */
    ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
    LL_ADC_Init ( ADC1, &ADC_InitStruct );
    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_ENABLE_3RANKS;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
    ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
    LL_ADC_REG_Init ( ADC1, &ADC_REG_InitStruct );
    LL_ADC_SetOverSamplingScope ( ADC1, LL_ADC_OVS_DISABLE );
    ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV256;
    ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
    LL_ADC_CommonInit ( __LL_ADC_COMMON_INSTANCE ( ADC1 ), &ADC_CommonInitStruct );
    LL_ADC_SetCommonPathInternalCh ( __LL_ADC_COMMON_INSTANCE ( ADC1 ),
                                     LL_ADC_PATH_INTERNAL_NONE );

    /* Disable ADC deep power down (enabled by default after reset state) */
    LL_ADC_DisableDeepPowerDown ( ADC1 );
    /* Enable ADC internal voltage regulator */
    LL_ADC_EnableInternalRegulator ( ADC1 );
    /* Delay for ADC internal voltage regulator stabilization. */
    /* Compute number of CPU cycles to wait for, from delay in us. */
    /* Note: Variable divided by 2 to compensate partially */
    /* CPU processing cycles (depends on compilation optimization). */
    /* Note: If system core clock frequency is below 200kHz, wait time */
    /* is only a few CPU processing cycles. */
    uint32_t wait_loop_index;
    wait_loop_index = ( ( LL_ADC_DELAY_INTERNAL_REGUL_STAB_US *
                          ( SystemCoreClock / ( 100000 * 2 ) ) ) / 10 );

    while ( wait_loop_index != 0 )
    {
        wait_loop_index--;
    }

    /** Configure Regular Channel
    */
    LL_ADC_REG_SetSequencerRanks ( ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_5 );
    LL_ADC_SetChannelSamplingTime ( ADC1, LL_ADC_CHANNEL_5,
                                    LL_ADC_SAMPLINGTIME_2CYCLES_5 );
    LL_ADC_SetChannelSingleDiff ( ADC1, LL_ADC_CHANNEL_5, LL_ADC_SINGLE_ENDED );

    /** Configure Regular Channel
    */
    LL_ADC_REG_SetSequencerRanks ( ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_15 );
    LL_ADC_SetChannelSamplingTime ( ADC1, LL_ADC_CHANNEL_15,
                                    LL_ADC_SAMPLINGTIME_2CYCLES_5 );
    LL_ADC_SetChannelSingleDiff ( ADC1, LL_ADC_CHANNEL_15, LL_ADC_SINGLE_ENDED );

    /** Configure Regular Channel
    */
    LL_ADC_REG_SetSequencerRanks ( ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_16 );
    LL_ADC_SetChannelSamplingTime ( ADC1, LL_ADC_CHANNEL_16,
                                    LL_ADC_SAMPLINGTIME_2CYCLES_5 );
    LL_ADC_SetChannelSingleDiff ( ADC1, LL_ADC_CHANNEL_16, LL_ADC_SINGLE_ENDED );
    /* USER CODE BEGIN ADC1_Init 2 */

    /* USER CODE END ADC1_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
