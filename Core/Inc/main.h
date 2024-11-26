/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_mainh
#define INC_mainh

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l412xx-bsp.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler ( void );

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/**
  * @brief  Perform ADC activation procedure to make it ready to convert
  *         (ADC instance: ADC1).
  * @param  None
  * @retval None
  */
void ADC_Activate ( void );

/**
  * @brief  Perform ADC group regular conversion start, poll for conversion
  *         completion.
  *         (ADC instance: ADC1).
  * @note   This function does not perform ADC group regular conversion stop:
  *         intended to be used with ADC in single mode, trigger SW start
  *         (only 1 ADC conversion done at each trigger, no conversion stop
  *         needed).
  *         In case of continuous mode or conversion trigger set to
  *         external trigger, ADC group regular conversion stop must be added
  *         after polling for conversion data.
  * @param  None
  * @retval None
  */
void ConversionStartPoll_ADC_GrpRegular ( void );

/* USER CODE END Private defines */

#ifdef __cplusplus
}

#endif

#endif /* INC_mainh */
