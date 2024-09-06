/**
  *******************************************************************************
  *
  *  @attention
  *  Copyright (c) 2022, 2023 Luminator, An LTG Company
  *  All rights reserved.
  *  Any use without the prior written consent of Luminator,
  *  An LTG Company is strictly prohibited.
  *
  *******************************************************************************
  *
  *******************************************************************************
  *
  *  @file fram.h
  *
  *  @brief This module contains definitions and structures to support
  *      SPI FRAM operations.
  *
  *  Revision History:
  *    Date         Name        Ver  Remarks
  *    -----------  ----------  ---  -------------------------------------------
  *    04/09/2023   Mark Lane   0    Original Version
  *
  *
  *  Notes:
  *
  *
  *******************************************************************************
  */
#ifndef _FRAM_H_
#define _FRAM_H_

#include "stm32l412xx-bsp.h"

/* Definition */
/**
  * opcode command
  */
typedef enum {

  OC_WREN  = 6 ,  /**< set write enable latch */
  OC_WRDI  = 4 ,  /**< write disable          */
  OC_RDSR  = 5 ,  /**< read status register   */
  OC_WRSR  = 1 ,  /**< write status register  */
  OC_READ  = 3 ,  /**< read memory data       */
  OC_WRITE = 2 ,  /**< write memory data      */

} OPCODE_COMMANDS ;


/**
  * status register
  */
typedef enum {

  SR_WEL  = 0x2  ,  /**< write-enable latch       */
  SR_BP0  = 0x4  ,  /**< block protect bit 0      */
  SR_BP1  = 0x8  ,  /**< block protect bit 1      */
  SR_WPEN = 0x80 ,  /**< enable write protect pin */

} STATUS_REGISTER ;


/**
  * write protect state
  */
typedef enum {

  WPS_PROTECTED = 0 ,  /**< write protected */
  WPS_WRITEABLE = 1 ,  /**< write enabled   */

} WRITE_PROTECT_STATE ;


/**
  * chip select state
  */
typedef enum {

  CSS_ASSERT  = 0 ,  /**< chip select disabled */
  CSS_RELEASE = 1 ,  /**< chip select enabled */

} CHIP_SELECT_STATE ;


/* Prototype Definition */

/**
  * This routine sets the hardware write protect pin to the correct "state".
  *
  * @param[in] state disable = 0, enable = 1
  * @param[out] none
  */
void framWriteProtect( WRITE_PROTECT_STATE state  ) ;

/**
  * This routine sets the chip select pin to the correct "state".
  *
  * @param[in] state assert = 0, release = 1
  * @param[out] none
  */
void framChipSelect( CHIP_SELECT_STATE state  ) ;


/**
  * This routine reads the FRAM status register
  *
  * @param[in] destination pointer for FRAM status register
  * @param[out] none
  */
void framReadSr( unsigned char *srP ) ;

/**
  * This routine writes the FRAM status register
  *
  * @param[in] data value written to FRAM status register
  * @param[out] none
  */
void framWriteSr( unsigned char sr ) ;


/**
  * This routine resets the write enable latch
  * @param[out] none
  */
void framWriteDisable( void ) ;

/**
  * This routine sets the write enable latch
  * @param[out] none
  */
void framWriteEnable( void )  ;


/**
  * This routine reads FRAM memory starting at "addr" for "len" byte(s). The
  * FRAM read data is stored at the pointer referenced by "rdBufP".
  *
  * @param[in] addr FRAM memory address to read data
  * @param[in] rdBufP destination pointer to store read data
  * @param[in] len number of byte(s) to read
  * @param[out] none
  */
void framReadMemory ( unsigned short addr, unsigned char *rdBufP, unsigned short len ) ;

/**
  * This routine writes FRAM memory starting at "addr" for "len" byte(s). The
  * data referenced by the pointer "wrBufP" is written into FRAM memory.
  *
  * @param[in] addr FRAM memory address to write data
  * @param[in] wrBufP pointer to data to write
  * @param[in] len number of byte(s) to write
  * @param[out] none
  */
void framWriteMemory( unsigned short addr, const unsigned char* const wrBufP, unsigned short len ) ;


/**
  * This routine is a test function for FRAM access. It writes "TLEN" bytes of
  * an incrementing pattern into FRAM at address "TADD". It reads the same length
  * into a buffer and verifies the pattern.
  * @param[out] pass = 1, fail = 0
  */
uint8_t framTest( void ) ;


#endif