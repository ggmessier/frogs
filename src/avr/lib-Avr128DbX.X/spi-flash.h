/* 
 * File:   spi-flash.h
 * Author: gmessier
 *
 * Created on August 21, 2018, 5:13 PM
 */


#ifndef SPI_FLASH_H
#define	SPI_FLASH_H

#include <avr/io.h>
#include "lib-avr.h"

/*
 * File System Index Table
 *  startSym: Valid File = 0x33, No File = 0xff
 *  name: ACII file name (terminated with null char)
 *  startAddr: Address of first byte in the file (constrained to be at
 *    the start of a page).
 *   
 */

#define FLASH_NUM_FILES 4 // Max number of files in the filesystem.
#define FLASH_BLOCK_SIZE 4096 // 4 kbytes
#define FLASH_CAPACITY 4194304 // 4 Mbytes
#define FLASH_NUM_BLOCKS (FLASH_CAPACITY/FLASH_BLOCK_SIZE)
#define FLASH_NUM_FILE_BLOCKS (FLASH_NUM_BLOCKS-1)
#define FLASH_FILE_SIZE_BLOCKS (int)(FLASH_NUM_FILE_BLOCKS/FLASH_NUM_FILES)

#define MAX_FNAME_CHARS 50

#define FLASH_VALID_FILE 0x33
#define FLASH_NO_FILE 0xff


// File System Index Table
typedef struct{
    uint8_t startSym;
    /*
     * Equal to FLASH_VALID_FILE for a valid file, and FLASH_NO_FILE
     * if this index is empty.
     */
    
    uint8_t name[MAX_FNAME_CHARS];  
    /*
     * ASCII file name (terminated with a NULL character).
     */
    
    uint32_t startAddr;
    /*
     * Address of the first byte in the file.  Constrained to be on a block
     * boundary.
     */
    
    uint32_t readAddr;
    /*
     * Next byte read from the file.  Initialized to the start of the file
     * and then updated by consecutive read operations.
     */
    
    uint32_t writeAddr;  
    /* 
     * First free byte in the file.  File size in bytes is equal to 
     * (writeAddr - startAddr).
     */
    
    uint32_t maxAddr;
    /*
     * The maximum address allowed in the file.
     */
    
} SpiFlashFileInfo; 




// Can store a 4kB file table but smaller reduces table read/write time.
#define SPI_FLASH_TABLE_ENTRY_SIZE 24
#define SPI_FLASH_TABLE_SIZE (SPI_FLASH_MAX_NUM_FILES * SPI_FLASH_TABLE_ENTRY_SIZE)
#define SPI_FLASH_FILE_START_ADDR 0x001000
#define SPI_FLASH_FILE_START_SYM 0x33

#define SPI_FLASH_WRITE_OK 0
#define SPI_FLASH_WRITE_TRUNC 1
#define SPI_FLASH_WRITE_OFLOW 2
#define SPI_FLASH_WRITE_LARGE_BUFF 3

#define SPI_FLASH_READ_OK 0
#define SPI_FLASH_END_OF_FILE 1

#define SPI_FLASH_MAX_FILES_REACHED 0xff
#define SPI_FLASH_FILE_TOO_SMALL 0xfe
#define SPI_FLASH_BAD_FILE_ID 0xfd
#define SPI_FLASH_FILE_OVERFLOW 0xfc


#define WAIT_FOR_IDLE_CHIP { \
    while( SpiFlashReadStatus(1) & 0x01){ ; } \
} 

#define WAIT_FOR_WRITE_ENABLE { \
    while( (SpiFlashReadStatus(1) & 0x02) == 0){ ; } \
}


#define SELECT_FLASH_CHIP { \
    PORTA.OUT &= 0b11110111; \
}

#define DESELECT_FLASH_CHIP { \
    PORTA.OUT |= 0b00001000; \
}






/*
 * Read status register.
 * - Register number is 1 or 2.
 */
uint8_t SpiFlashReadStatus(uint8_t registerNum);


/*
 * Enable a write/erase operation.
 */
void SpiFlashWriteEnable();



#ifdef STILL_NEED_TO_PORT


/*
 * Load file attribute table from FLASH.
 */

void SpiFlashLoadAttributeTable();

/*
 * Initialize FLASH with a blank attribute table.
 */
void SpiFlashInitAttributeTable();





// ----------------------------

/*
 * Saves updates to the file system to FLASH.
 * - NOTE: ** Must be called before power down or the file system will be corrupted.
 */
void SpiFlashSaveFileTable();


/*
 * Clear filesystem.
 * - Deletes all files from the FLASH chip.
 */
void SpiFlashDeleteFilesystem();

void QuickFilesystemDelete();

/*
 * Creates a file.
 * - File size is 2^sizeExp bytes.
 */
int SpiFlashFileOpen(uint8_t *name, uint32_t fileSize);


/*
 * Rewinds to the start of the file.
 */
int SpiFlashRewindRead(uint8_t fileId);

/*
 * Write data to a file.
 * - Writes a maximum of 256 bytes.
 * - Terminates write on page or max file size boundaries. 
 */
int SpiFlashWrite(uint8_t fileId, uint8_t numBytes, uint8_t *buffer);

/*
 * Read data from a file.
 * - Terminates read when the end of file is reached.
 */
int SpiFlashRead(uint8_t fileId, uint8_t numBytes, uint8_t *buffer);

/*
 * Print the file table.
 */
void SpiPrintFileTable();

// =============== Internal Functions =================

/*
 * Enable write or erase operations.
 */
void SpiFlashWriteEnable();


#endif


#endif	/* SPI_FLASH_H */

