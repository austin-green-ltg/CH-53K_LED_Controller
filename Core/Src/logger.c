// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: logger.c
//
// Description: Handles logging and reading of data to memory
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes: Depends on the board support package bsp
//
// ***************************************************************************

#include <string.h>
#include <stdio.h>

#include "logger.h"
#include "fram.h"
#include "stm32l412xx-bsp.h"

// Signals end of data
static uint32_t tail_pointer = 0;

// Log a string to tail_pointer, use write_beginning flag to write the beginning
void LogString( const char* const string, uint8_t write_beginning )
{
    uint32_t write_bytes = strlen(string) + 1;
    uint32_t address = tail_pointer;
    tail_pointer += write_bytes;
    if (write_beginning)
    {
        address = 0;
        tail_pointer = write_bytes;
    }

    framWriteMemory( address, (unsigned char*)string, write_bytes );
}

// Logs a number by converting the number to a string and using the LogString function
void LogNumber( const int32_t number, uint8_t write_beginning )
{
    char str[12]; // max number of characters needed for 32 bit number 2 million = 10 numbers + "-" + '\0'
    memset(str, '\0', 12);
    sprintf(str, "%d", number);
    LogString(str, write_beginning);
}
// Reads the log at a given address and size
void ReadLog(  const uint32_t address, char* string, const uint32_t bytes )
{
    uint32_t read_bytes = bytes;
    if (string == NULL) return;
    if (bytes == 0)
    {
        string = "";
        return;
    }

    framReadMemory( address, (unsigned char*)string, read_bytes );
}
