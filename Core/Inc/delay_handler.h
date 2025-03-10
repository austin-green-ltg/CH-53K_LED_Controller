/*****************************************************************************
 *
 *  @attention
 *  Copyright (c) 2022, 2023 Luminator, An LTG Company
 *  All rights reserved.
 *  Any use without the prior written consent of Luminator,
 *  An LTG Company is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file delay_handler.h
 *
 * @brief Handles system counters and delays
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes:
 *
 *****************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_delay_handlerh
#define INC_delay_handlerh

#include <stdint.h>

/**
  * @brief Starts the delay counter, only needs to be called once on init
  */
void StartDelayCounter ( void ); // start the counter

/**
  * @brief Starts the log delay counter, only needs to be called once on init
  */
void StartLogDelayCounter ( void ); // start the counter

/**
  * @brief Starts the live log delay counter, only needs to be called once on init
  */
void StartLiveLogDelayCounter ( void ); // start the counter

/**
  * @brief Restart the delay counter
  */
void RestartDelayCounter ( void );

/**
  * @brief Restart the log delay counter
  */
void RestartLogDelayCounter ( void );

/**
  * @brief Restart the live log delay counter
  */
void RestartLiveLogDelayCounter ( void );

/**
  * @brief Checks if the delay (ms) was hit based on timer value
  * @param[in] delay_ms Time in ms to check if timer has hit
  * @param[out] Returns 1 if delay has been hit
  */
uint8_t DelayHit ( uint16_t delay_ms );

/**
  * @brief Checks if the log delay (ms) was hit based on timer value
  * @param[in] delay_ms Time in ms to check if log timer has hit
  * @param[out] Returns 1 if log delay has been hit
  */
uint8_t LogDelayHit ( uint32_t delay_ms );

/**
  * @brief Checks if the live log delay (ms) was hit based on timer value
  * @param[in] delay_ms Time in ms to check if live log timer has hit
  * @param[out] Returns 1 if live log delay has been hit
  */
uint8_t LiveLogDelayHit ( uint16_t delay_ms );

/**
  * @brief Returns a delay value for a brightness level
  * @param[in] brightness Current brightness level
  * @param[out] Returns delay to satisfy specs at current brightness level
  */
uint16_t BrightnessDelay ( int8_t brightness );

#endif /* INC_delay_handlerh */
