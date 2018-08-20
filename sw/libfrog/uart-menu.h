/* 
 * File:   uart-macros.h
 * Author: gmessier
 *
 * Created on July 20, 2018, 7:34 PM
 */

#ifndef UART_MENU_H
#define	UART_MENU_H

#include "mcc_generated_files/eusart.h"


#define UART_PRINT_STR(str) { \
    uint8_t ind = 0; \
    while( str[ind]!='\0' ){ \
        EUSART_Write(str[ind]); \
        ind++; \
    } \
}

#define UART_MENU_PROMPT { \
    UART_PRINT_STR("Press '?' for menu.\n\r"); \
    UART_PRINT_STR("{*} "); \
}

#define UART_READ_MENU_COMMAND { \
    uint8_t command, menuInd; \
    command = EUSART_Read(); \
    EUSART_Read(); \
    if( command == 0x3f ){ \
        for(menuInd = 0; menuInd < uartMenuSize; menuInd++ ){ \
            EUSART_Write(uartMenu[menuInd].command + 0x30); \
            UART_PRINT_STR(": "); \
            UART_PRINT_STR(uartMenu[menuInd].help); \
            UART_PRINT_STR("\n\r"); \
        } \
    } \
    else{ \
        command -= 0x30; \
        for(menuInd = 0; menuInd < uartMenuSize; menuInd++ ){ \
            if( command == uartMenu[menuInd].command ) { \
                (*(uartMenu[menuInd].func))(); \
                break; \
            } \
        } \
        if( menuInd == uartMenuSize ) { \
            UART_PRINT_STR("Error: Invalid command.\r\n"); \
        } \
    } \
}
    
    
    

typedef void (*UartCommandFunc)();

typedef struct{
    uint8_t command;
    uint8_t *help;
    UartCommandFunc func;    
} UartCommand;






            


#endif	/* UART_MENU_H */

