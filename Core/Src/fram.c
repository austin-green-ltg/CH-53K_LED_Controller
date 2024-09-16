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
 *  @file fram.c
 *
 *  @brief This module contains routines to access the SPI FRAM. The FRAM used
 *         in the device is 32Kx8 with an operating frequency up to 20 MHz.
 *         It supports SPI Mode 0 & 3. The hardware supports the external
 *         Write Protect pin. The nature of this FRAM supports unlimited
 *         read/writes and data retention beyond the human life span.
 *
 *  Revision History:
 *     Date         Name           Ver     Remarks
 *     -----------  ----------     ---     -----------------------------------
 *     04/09/2023   Mark Lane      0       Original Version
 *     06/10/2024   Austin Green   1       Add Logging
 *     08/12/2024   Austin Green   1.2     Remove Mirror References
 *     09/10/2024 - Austin Green - 2.0 -   Doxyfile documentation
 *
 *  Notes:
 *
 *****************************************************************************/

/* -----------------------------  Include(s) -------------------------------- */
#include <string.h>

#include "fram.h"
#include "stm32l412xx-bsp.h"
#include "spi.h"

/**
  * @fn framWriteProtect
  *
  * @brief This routine sets the hardware write protect pin to the correct
  *          "state".
  *
  * @param[in] state disable = 0, enable = 1
  * @param[out] none
  */
void framWriteProtect ( WRITE_PROTECT_STATE state )
{
    if ( state == WPS_WRITEABLE )
    {
        enableWriteProtect();
    }
    else
    {
        disableWriteProtect();
    }
}

/**
  * @fn framChipSelect
  *
  * @brief This routine sets the chip select pin to the correct "state".
  *
  * @param[in] state assert = 0, release = 1
  * @param[out] none
  */
void framChipSelect ( CHIP_SELECT_STATE state )
{
    if ( state == CSS_RELEASE )
    {
        enableChipSelect();
    }
    else
    {
        disableChipSelect();
    }
}

/**
  * @fn framReadSr
  *
  * @brief This routine reads the FRAM status register
  *
  * @param[in] srP destination pointer for FRAM status register
  * @param[out] none
  */
void framReadSr ( unsigned char* srP )
{
    unsigned char rxbuf [ 2 ] ;
    unsigned char txbuf [ 2 ] ;

    memset ( rxbuf, '\0', sizeof ( rxbuf ) ) ;
    memset ( txbuf, '\0', sizeof ( txbuf ) ) ;

    txbuf [ 0 ] = OC_RDSR ;

    framChipSelect ( CSS_ASSERT ) ;

    transferData ( txbuf, sizeof ( txbuf ) );
    receiveData ( rxbuf, sizeof ( rxbuf ) );

    *srP = rxbuf [ 1 ] ;

    framChipSelect ( CSS_RELEASE ) ;
}

/**
  * @fn framWriteSr
  *
  * @brief This routine writes the FRAM status register
  *
  * @param[in] sr data value written to FRAM status register
  * @param[out] none
  */
void framWriteSr ( unsigned char sr )
{
    unsigned char txbuf [ 2 ] ;

    txbuf [ 0 ] = OC_WRSR ;
    txbuf [ 1 ] = sr ;

    framChipSelect ( CSS_ASSERT ) ;

    transferData ( txbuf, sizeof ( txbuf ) );

    framChipSelect ( CSS_RELEASE ) ;
}

/**
  * @fn framWriteDisable
  *
  * @brief This routine resets the write enable latch
  * @param[out] none
  */
void framWriteDisable ( void )
{
    unsigned char txbuf [ 1 ] = { OC_WRDI } ;

    framChipSelect ( CSS_ASSERT ) ;

    transferData ( txbuf, sizeof ( txbuf ) );

    framChipSelect ( CSS_RELEASE ) ;
}

/**
  * @fn framWriteEnable
  *
  * @brief This routine resets the write enable latch
  * @param[out] none
  */
void framWriteEnable ( void )
{
    unsigned char txbuf [ 1 ] = { OC_WREN } ;

    framChipSelect ( CSS_ASSERT ) ;

    transferData ( txbuf, sizeof ( txbuf ) );

    framChipSelect ( CSS_RELEASE ) ;
}

/**
  * @fn framReadMemory
  *
  * @brief This routine reads FRAM memory starting at "addr" for "len"
  *         byte(s). The FRAM read data is stored at the pointer referenced by
  *         "rdBufP".
  *
  * @param[in] addr FRAM memory address to read data
  * @param[in] rdBufP destination pointer to store read data
  * @param[in] len number of byte(s) to read
 *
 *  NOTE    : argument len must not be greater than 256
 *
 */
void framReadMemory ( unsigned short addr, unsigned char* rdBufP,
                      unsigned short len )
{
    unsigned char txbuf [ 3 ] ;

    txbuf [ 0 ] = OC_READ ;
    txbuf [ 1 ] = * ( ( unsigned char* ) &addr + 1 )
                  ; /* byte swap for big endian */
    txbuf [ 2 ] = * ( ( unsigned char* ) &addr + 0 ) ;

    framChipSelect ( CSS_ASSERT ) ;

    /* send the read command & address */
    transferData ( txbuf, sizeof ( txbuf ) );

    /* read the fram data */
    receiveData ( rdBufP, len );

    framChipSelect ( CSS_RELEASE ) ;
}

/**
  * @fn framWriteMemory
  *
  * @brief This routine writes FRAM memory starting at "addr" for "len"
  *         byte(s). The data referenced by the pointer "wrBufP" is written
  *         into FRAM memory.
  *
  * @param[in] addr FRAM memory address to read data
  * @param[in] wrBufP pointer to data to write
  * @param[in] len number of byte(s) to write
 *
 *  NOTE    : argument len must not be greater than 256
 *
 */
void framWriteMemory ( unsigned short addr, const unsigned char* const wrBufP,
                       unsigned short len )
{
    unsigned char txbuf [ 3 ] ;
    unsigned char sr ;

    framWriteEnable() ; /* set write enable latch */
    /* Read the FRAM status register and verify the SR_WEL was set */
    framReadSr (&sr ) ; /* read the FRAM status register */

    txbuf [ 0 ] = OC_WRITE ;
    txbuf [ 1 ] = * ( ( unsigned char* ) &addr + 1 )
                  ; /* byte swap for big endian */
    txbuf [ 2 ] = * ( ( unsigned char* ) &addr + 0 ) ;

    framChipSelect ( CSS_ASSERT ) ;

    /* send the write command & address */
    transferData ( txbuf, sizeof ( txbuf ) );

    transferData ( wrBufP, len );

    framChipSelect ( CSS_RELEASE ) ;
}

/**
  * @fn framTest
  *
  * @brief This routine is a test function for FRAM access. It writes "TLEN"
  *         bytes of an incrementing pattern into FRAM at address "TADD". It
  *         reads the same length into a buffer and verifies the pattern.
  *
  * @param[out] 1 = pass, 0 = fail
  *
  */
uint8_t framTest ( void )
{
#define TLEN    (16)     /**< test length */
#define TADD    (0x200)  /**< test addresss */

    unsigned char txbuf [ TLEN ] ;
    unsigned char rxbuf [ TLEN ] ;

    int i, pass ;

    /* initialize txbuf to incrementing pattern */
    for ( i = 0; i < TLEN; i++ )
    {
        txbuf [ i ] = i + 1 ;
        rxbuf [ i ] = 0 ; /* clear rxbuf */
    }

    framWriteMemory ( TADD, txbuf, TLEN ) ;

    framReadMemory ( TADD, rxbuf, TLEN ) ;

    for ( i = 0; i < TLEN; i++ )
    {
        if ( rxbuf [ i ] != txbuf [ i ] )
        {
            break ;
        }
    }

    if ( i == TLEN )
    {
        pass = 1 ;
    }
    else
    {
        pass = 0 ;
    }

    return pass;
}


/* ------------------------------- CODE ENDS -------------------------------- */
