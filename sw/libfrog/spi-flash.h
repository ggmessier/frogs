/* 
 * File:   spi-flash.h
 * Author: gmessier
 *
 * Created on August 21, 2018, 5:13 PM
 */

#ifndef SPI_FLASH_H
#define	SPI_FLASH_H


#define MAX_FNAME_SIZE 7

typedef struct{
    uint8_t startSym; // 1 byte
    uint8_t name[MAX_FNAME_SIZE];  // 7 bytes
    uint32_t startAddr; // 4 bytes
    uint32_t readAddr;  // 4 bytes
    uint32_t endAddr;  // 4 bytes
    uint32_t maxAddr;  // 4 bytes
} SpiFlashFileInfo;  // 24 bytes

// Can store a 4kB file table but smaller reduces table read/write time.
#define SPI_FLASH_MAX_NUM_FILES 4 
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

#define SPI_FLASH_TX_BYTE(d){ \
    SSP1BUF = (d); \
    WAIT_FOR_SPI_TRANSFER; \
    }

#define SPI_FLASH_RX_BYTE(d){ \
    SSP1BUF = DUMMY_DATA; \
    WAIT_FOR_SPI_TRANSFER; \
    d = SSP1BUF; \
    }


#define WAIT_FOR_SPI_TRANSFER { \
    while(SSP1STATbits.BF == 0){ ; } \
    }

#define WAIT_FOR_IDLE_CHIP { \
  while( SpiFlashReadStatus(0) & 0x01){ ; } \
  } 


#define SELECT_FLASH_CHIP   IO_RC7_SetLow()
#define DESELECT_FLASH_CHIP IO_RC7_SetHigh()



/*
 * Initialize FLASH chip.
 */
void SpiFlashInit();


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

/*
 * Read status register.
 * - Register number is 0 or 1.
 */
uint8_t SpiFlashReadStatus(uint8_t registerNum);




#endif	/* SPI_FLASH_H */

