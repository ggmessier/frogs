/*
 * File: spi.h
 * General purpose SPI communication routines.
 * Author: Geoffrey Messier
 */


#ifndef SPI_LIB_H
#define SPI_LIB_H

#include <avr/io.h>
#include "lib-avr.h"

#define SPI_DUMMY_DATA 0x00

#define WAIT_FOR_SPI_TRANSFER { \
    while( (SPI0.INTFLAGS & 0x80) == 0 ){ ; } \
}

#define SPI_TX_BYTE(d){ \
    SPI0.DATA = d; \
    WAIT_FOR_SPI_TRANSFER; \
}

#define SPI_RX_BYTE(d){ \
    SPI0.DATA = SPI_DUMMY_DATA; \
    WAIT_FOR_SPI_TRANSFER; \
    d = SPI0.DATA; \
}



#endif