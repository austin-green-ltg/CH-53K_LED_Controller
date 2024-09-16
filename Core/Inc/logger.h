/*****************************************************************************
 *
 * @attention
 *  Copyright (c) 2022, 2023 Luminator, An LTG Company
 *  All rights reserved.
 *  Any use without the prior written consent of Luminator,
 *  An LTG Company is strictly prohibited.
 *
 *****************************************************************************
 *****************************************************************************
 *
 * @file logger.h
 *
 * @brief Handles logging and reading of data to memory
 *
 * Revision History:
 * Date       - Name         -  Ver -  Remarks
 * 07/31/2024 - Austin Green -  1.0 -  Initial Document
 * 08/05/2024 - Austin Green -  1.1 -  Added Log Number
 * 09/10/2024 - Austin Green -  2.0 -  Doxyfile documentation
 *
 * Notes:
 *
 *****************************************************************************/

#ifndef INC_loggerh
#define INC_loggerh

#include <stdint.h>


/**
  * @brief Log a string to tail_pointer, use write_beginning flag to write the beginning
  * @param[in] string Pointer to string to log
  * @param[in] write_beginning Write log at the beginning of log area
  */
void LogString ( const char* const string, uint8_t write_beginning );

/**
  * @brief Logs a number by converting the number to a string and using the LogString function
  * @param[in] number Number to log
  * @param[in] write_beginning Write log at the beginning of log area
  */
void LogNumber ( const int32_t number, uint8_t write_beginning );

/**
  * @brief Reads the log at a given address and size
  * @param[in] address Address to read from
  * @param[in] string Pointer to return data string
  * @param[in] bytes Number of bytes to read
  */
void ReadLog ( const uint32_t address, char* string, const uint32_t bytes );

#endif /* INC_loggerh */