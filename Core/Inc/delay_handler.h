// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: delay_handler.h
//
// Description: Handles system counters and delays
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes:
//
// ***************************************************************************

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INC_delay_handlerh
    #define INC_delay_handlerh

    #include <stdint.h>

    /* Delay timings for sweeping brightness */
    #define TOGGLE_DELAY_MS     (250)
    #define LOWER_SWEEP_TIME_MS (3375)                                          // time from 50%->0%
    #define UPPER_SWEEP_TIME_MS (4000)                                          // time from 50%->100%
    #define TOTAL_SWEEP_TIME_MS (7375)                                          // time from 0%->100%
    #define AVG_SWEEP_TIME_MS   (TOTAL_SWEEP_TIME_MS / 2)                       // 3687.5
    #define LOWER_STEP_TIME_MS  (LOWER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2))  // 135, time lower per step
    #define UPPER_STEP_TIME_MS  (UPPER_SWEEP_TIME_MS / (BRIGHTNESS_STEPS / 2))  // 160, time upper per step
    #define AVG_STEP_TIME_MS    ((UPPER_STEP_TIME_MS + LOWER_STEP_TIME_MS) / 2) // 147.5
    #define AVG_STEP_DIFF_MS    (AVG_STEP_TIME_MS    - LOWER_STEP_TIME_MS)      // 12.5, distance between lower step time and average step time
    #define LOW_STEP_TIME_MS    (LOWER_STEP_TIME_MS  - AVG_STEP_DIFF_MS)        // 122.5
    #define HIGH_STEP_TIME_MS   (UPPER_STEP_TIME_MS  + AVG_STEP_DIFF_MS)        // 172.5

    /* Check if delay has been hit */
    void    StartDelayCounter(void);     // start the counter
    void    RestartDelayCounter(void);   // restart the counter
    uint8_t DelayHit(uint32_t delay_ms); // is delay in ms hit
    /* Get ms delay for given brightness */
    uint16_t BrightnessDelay(int8_t brightness);

#endif /* INC_delay_handlerh */
