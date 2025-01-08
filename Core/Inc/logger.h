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

#define CHAR_LOG_SIZE   (2) // 1 byte plus null pointer
#define SHORT_LOG_SIZE  (3) // 2 bytes plus null pointer
#define INT_LOG_SIZE    (5) // 4 bytes plus null pointer

#define PWM_LOG_SIZE            (CHAR_LOG_SIZE)
#define CURRENT_LOG_SIZE        (SHORT_LOG_SIZE)
#define TEMPERATURE_LOG_SIZE    (SHORT_LOG_SIZE)
#define VOLTAGE_LOG_SIZE        (SHORT_LOG_SIZE)

#define TOTAL_PWM_LOGS          (3) // IR, VIS, Previous Init
#define TOTAL_CURRENT_LOGS      (100)
#define TOTAL_TEMPERATURE_LOGS  (100)
#define TOTAL_VOLTAGE_LOGS      (100)

#define PWM_LOG_SPACE           (PWM_LOG_SIZE * TOTAL_PWM_LOGS)
#define CURRENT_LOG_SPACE       (CURRENT_LOG_SIZE * TOTAL_CURRENT_LOGS)
#define TEMPERATURE_LOG_SPACE   (TEMPERATURE_LOG_SIZE * TOTAL_TEMPERATURE_LOGS)
#define VOLTAGE_LOG_SPACE       (VOLTAGE_LOG_SIZE * TOTAL_VOLTAGE_LOGS)

#define STARTING_PWM_ADDRESS            (0x0)
#define STARTING_CURRENT_ADDRESS        (STARTING_PWM_ADDRESS + PWM_LOG_SPACE)
#define STARTING_TEMPERATURE_ADDRESS    (STARTING_CURRENT_ADDRESS + CURRENT_LOG_SPACE)
#define STARTING_VOLTAGE_ADDRESS        (STARTING_TEMPERATURE_ADDRESS + TEMPERATURE_LOG_SPACE)

#define numToCharArray(arr, num)    (arr) [0] = ( (num) & 0x00FF ); (arr) [ 1 ] = ( (num) & 0xFF00 ) >> 0x8;
#define numFromCharArray(arr, num)  (num) = (arr) [0] | ((arr)[1] << 0x8)

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
  * @brief Writes the log at a given address and size
  * @param[in] address Address to write from
  * @param[in] string Pointer to string to write
  * @param[in] bytes Number of bytes to write
  */
void WriteLog ( const uint32_t address, const char* const string,
                const uint32_t bytes );

/**
  * @brief Reads the log at a given address and size
  * @param[in] address Address to read from
  * @param[in] string Pointer to return data string
  * @param[in] bytes Number of bytes to read
  */
void ReadLog ( const uint32_t address, char* string, const uint32_t bytes );

#endif /* INC_loggerh */
