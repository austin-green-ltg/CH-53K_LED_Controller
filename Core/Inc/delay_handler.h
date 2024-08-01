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

    /* Check if delay has been hit */
    void    StartDelayCounter(void);     // start the counter
    void    RestartDelayCounter(void);   // restart the counter
    uint8_t DelayHit(uint32_t delay_ms); // is delay in ms hit
    /* Get ms delay for given brightness */
    uint16_t BrightnessDelay(int8_t brightness);

#endif /* INC_delay_handlerh */
