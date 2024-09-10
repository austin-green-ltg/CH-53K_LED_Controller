/*****************************************************************************
 *
 *  @attention
 * Copyright © 2007 Luminator Mark IV
 * All rights reserved.
 * Any use without the prior written consent of Luminator Mark IV
 * is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file voltage_handler.h
 *
 * @brief Handles getting voltage and reporting values.
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 08/05/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes:
 *
 *****************************************************************************/

#ifndef INC_voltage_handlerh
    #define INC_voltage_handlerh

    #include <stdint.h>

    /** Voltage Range Enum */
    typedef enum
    {
        VoltageNormal       = 0,    /**< Normal Operating Voltage */
        VoltageLow          = 1,    /**< Voltage low, but ok      */
        VoltageHigh         = 2,    /**< Voltage high, but ok     */
        VoltageErrorLow     = 3,    /**< Voltage too low          */
        VoltageErrorHigh    = 4     /**< Voltage too high         */
    } VoltageRange_e;

    /**
      * @brief Get voltage from voltmeter
      *
      * @param[out] voltage level in dV
      */
    uint16_t GetVoltage( void );

    /**
      * @brief Get range that the voltage falls into
      *         Possible ranges are
      *         Normal      - Normal Operating Voltage
      *         Low         - Voltage low, but ok
      *         High        - Voltage high, but ok
      *         ErrorLow    - Voltage too low
      *         ErrorHigh   - Voltage too high
      * @param[out] Current voltage range
      */
    VoltageRange_e GetVoltageRange( void );

#endif /* INC_voltage_handlerh */
