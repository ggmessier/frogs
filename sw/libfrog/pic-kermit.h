/* 
 * File:   pic-kermit.h
 * Author: gmessier
 *
 * Created on July 22, 2018, 5:26 PM
 */

#ifndef PIC_KERMIT_H
#define	PIC_KERMIT_H

#ifdef	__cplusplus
extern "C" {
#endif


typedef enum { TX_SEND_INIT, 
        TX_SEND_FILE_HEADER, 
        TX_SEND_FILE_DATA,
        TX_SEND_EOF,
        TX_SEND_BREAK,
        TX_SEND_COMPLETE,
        TX_ABORT
} KermitTxStates;

typedef enum { RX_SEND_INIT_WAIT,
        RX_FILE_HEADER_WAIT,
        RX_DATA_PACKET_WAIT,
        RX_SEND_COMPLETE,
        RX_ABORT
} KermitRxStates;
    

typedef struct {
    uint8_t maxl; // Maximum packet length (char offset 32)
    uint8_t time; // Timeout in sec for other terminal (char offset 32)
    uint8_t npad; // Number of padding chars before each packet (char offset 32)
    uint8_t padc; // Padding character (usually 0 or 127) (xor with 64)
    uint8_t eol; // Character to terminate a packet (char offset 32)
    uint8_t qctl; // Char used to quote control chars (usually #) (char offset 32)    
} ReqInitData;
    
    
void PicKermitRxFile();
uint8_t PicKermitRxPacket(uint8_t *rxPacketType, uint8_t *rxSeq, uint8_t nDataBytes, uint8_t *dataPtr);
void PicKermitTxPacket(uint8_t txPacketType, uint8_t txSeq, uint8_t nDataBytes, uint8_t *dataPtr);

    
    


#ifdef	__cplusplus
}
#endif

#endif	/* PIC_KERMIT_H */

