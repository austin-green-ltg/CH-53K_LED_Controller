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
 * @file current_handler.c
 *
 * @brief Handles getting current and reporting values.
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 08/05/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp
 *        logger is used to log current transitions
 *
 *****************************************************************************/

/* Private includes ----------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "current_handler.h"
#include "stm32l412xx-bsp.h"
#include "logger.h"

/** Raw value out of ammeter to deciAmps (A*0.1) */
const float RawTodAmps = ( RAW_TO_MV * 2.0f / 100.0f );
/** DeciAmps (A*0.1) to raw value out of ammeter */
const float dAmpsToRaw = ( MV_TO_RAW / 2.0f * 100.0f );

/** High Current Level in dA */
const uint16_t CurrentHighThreshold_dA = 35u;
/** High Current Error Level in dA */
const uint16_t CurrentErrorThreshold_dA = 40u;

/** default current state is CurrentNormal */
static CurrentRange_e current_threshold = CurrentNormal;

static uint16_t numCurrentLogs = 0;

/**
  * @brief Logs current change to storage
  *
  * @param[in] range Range the current is in, see CurrentRange_e
  * @param[in] currentValue Current in dA
  */
static void LogCurrentChange ( CurrentRange_e range, uint16_t currentValue )
{
    char str [ 25 ];
    memset ( str, '\0', 25 );

    switch ( range )
    {
        case CurrentNormal:
            snprintf ( str, sizeof ( str ), "Normal Current %d dA\n", currentValue );
            break;

        case CurrentHigh:
            snprintf ( str, sizeof ( str ), "High Current %d dA\n", currentValue );
            break;

        case CurrentError:
            snprintf ( str, sizeof ( str ), "Error Current %d dA\n", currentValue );
            break;

        default:
            break;

    }

    LogString ( str, 0 );
}

/**
  * @brief Logs current to storage
  */
void LogCurrent ( void )
{
    // Double storage for eof constant, overwrite eof on next log
    char str [ CURRENT_LOG_SIZE * 2 ];
    uint16_t current = GetCurrent();

    numToCharArray ( str, current );
    numToCharArray (&str [ 2 ], eol_uint_const );

    if ( ( uint32_t ) numCurrentLogs * CURRENT_LOG_SIZE >= CURRENT_LOG_SPACE )
    {
        // Write at start of log
        numCurrentLogs = 0;

        WriteLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs * CURRENT_LOG_SIZE, str,
                   CURRENT_LOG_SIZE * 2 );
    }
    else if ( ( uint32_t ) numCurrentLogs * CURRENT_LOG_SIZE + CURRENT_LOG_SIZE >=
              CURRENT_LOG_SPACE )
    {
        // write end of log, then write eol to beginning of log
        WriteLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs * CURRENT_LOG_SIZE, str,
                   CURRENT_LOG_SIZE );
        WriteLog ( STARTING_CURRENT_ADDRESS,
                   &str [ CURRENT_LOG_SIZE ],
                   CURRENT_LOG_SIZE );
    }
    else
    {
        // write normal
        WriteLog ( STARTING_CURRENT_ADDRESS + numCurrentLogs * CURRENT_LOG_SIZE, str,
                   CURRENT_LOG_SIZE * 2 );
    }

    numCurrentLogs++;
}

/**
  * @brief Finds Current EOL string in logs
  */
void findCurrentLogEOL ( void )
{
    char str [ CURRENT_LOG_SIZE ];
    char eol [ CURRENT_LOG_SIZE ];

    numToCharArray ( eol, eol_uint_const );

    numCurrentLogs = 0; // if none found, start at beginning

    for ( uint32_t i = STARTING_CURRENT_ADDRESS;
            i <= STARTING_CURRENT_ADDRESS + CURRENT_LOG_SPACE;
            i += CURRENT_LOG_SIZE )
    {
        ReadLog ( i, str, CURRENT_LOG_SIZE );

        if ( eol [ 0 ] == str [ 0 ] && eol [ 1 ] == str [ 1 ] )
        {
            numCurrentLogs = ( i - STARTING_CURRENT_ADDRESS ) / CURRENT_LOG_SIZE;
            return;
        }
    }

    return;
}

/**
  * @brief Get current from ammeter
  *
  * @param[out] current level in dA
  */
uint16_t GetCurrent ( void )
{
    uint16_t current = ( uint16_t ) ( ( float ) GetCurrentValue() * RawTodAmps +
                                      0.5f );
    return ( current );
}

/**
  * @brief Get range that the current falls into
  *         Possible ranges are
  *           Normal  - Normal Operating Current
  *           High    - Current high, but ok
  *           Error   - Current too high
  * @param[out] Current current range
  */
CurrentRange_e GetCurrentRange ( void )
{
    uint16_t current = GetCurrent();
    CurrentRange_e prev_threshold = current_threshold;

    if ( current >= CurrentErrorThreshold_dA )
    {
        current_threshold = CurrentError;
    }
    else if ( current >= CurrentHighThreshold_dA )
    {
        current_threshold = CurrentHigh;
    }
    else
    {
        current_threshold = CurrentNormal;
    }

    if ( prev_threshold != current_threshold )
    {
        LogCurrentChange ( current_threshold, current );
    }

    return current_threshold;
}
