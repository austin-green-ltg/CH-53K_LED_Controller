
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_HANDLER_H
#define __DELAY_HANDLER_H

#include <stdint.h>

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

void    startDelayCounter(void);     // start the counter
void    restartDelayCounter(void);   // restart the counter
uint8_t delayHit(uint32_t delay_ms); // is delay in ms hit
uint16_t brightnessDelay(int8_t brightness);

#endif /* __DELAY_HANDLER_H */
