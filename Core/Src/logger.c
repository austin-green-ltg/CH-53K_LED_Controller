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
 * @file logger.c
 *
 * @brief Handles logging and reading of data to memory
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes: Depends on the board support package bsp
 *
 *****************************************************************************/

#include <string.h>
#include <stdio.h>

#include "logger.h"
#include "fram.h"
#include "stm32l412xx-bsp.h"

/** Signals end of data */
static uint32_t tail_pointer = 0;

/**
  * @brief Log a string to tail_pointer, use write_beginning flag to write the beginning
  * @param[in] string Pointer to string to log
  * @param[in] write_beginning Write log at the beginning of log area
  */
void LogString ( const char* const string, uint8_t write_beginning )
{
    uint32_t write_bytes = strlen ( string ) + 1;
    uint32_t address = tail_pointer;
    tail_pointer += write_bytes;

    if ( write_beginning )
    {
        address = 0;
        tail_pointer = write_bytes;
    }

    WriteLog ( address, string, write_bytes );
}

/**
  * @brief Logs a number by converting the number to a string and using the LogString function
  * @param[in] number Number to log
  * @param[in] write_beginning Write log at the beginning of log area
  */
void LogNumber ( const int32_t number, uint8_t write_beginning )
{
    char str [ 12
             ]; // max number of characters needed for 32 bit number 2 million = 10 numbers + "-" + '\0'
    memset ( str, '\0', 12 );
    sprintf ( str, "%d", number );
    LogString ( str, write_beginning );
}

/**
  * @brief Writes the log at a given address and size
  * @param[in] address Address to write from
  * @param[in] string Pointer to string to write
  * @param[in] bytes Number of bytes to write
  */
void WriteLog ( const uint32_t address, const char* const string,
                const uint32_t bytes )
{
    uint32_t write_bytes = bytes;

    if ( string == NULL || bytes == 0 )
    {
        return;
    }

    framWriteMemory ( address, ( unsigned char* ) string, write_bytes );
}

/**
  * @brief Reads the log at a given address and size
  * @param[in] address Address to read from
  * @param[in] string Pointer to return data string
  * @param[in] bytes Number of bytes to read
  */
void ReadLog ( const uint32_t address, char* string, const uint32_t bytes )
{
    uint32_t read_bytes = bytes;

    if ( string == NULL )
    {
        return;
    }

    if ( bytes == 0 )
    {
        string = "";
        return;
    }

    framReadMemory ( address, ( unsigned char* ) string, read_bytes );
}
