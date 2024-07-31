// ***************************************************************************
// Copyright © 2007 Luminator Mark IV
// All rights reserved.
// Any use without the prior written consent of Luminator Mark IV
// is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
// Filename: logger.h
//
// Description: Handles logging and reading of data to memory
//
// Revision History:
// Date       - Name         -  Ver -  Remarks
// 07/31/2024 - Austin Green -  1.0 -  Initial Document
//
// Notes:
//
// ***************************************************************************

#ifndef INC_loggerh
    #define INC_loggerh

    #include <stdint.h>

    /* Store and Read Logs */
    void LogString( const char* const string, uint8_t write_beginning );
    void ReadLog(  const uint32_t address, char* string, const uint32_t bytes );

#endif /* INC_loggerh */