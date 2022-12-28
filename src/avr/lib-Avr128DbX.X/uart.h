/* 
 * File:   uart.h
 * Author: gmessier
 * Macros for AVR MCU UART communication and a simple menu.
 * Created on July 20, 2018, 7:34 PM
 */

#ifndef UART_AVR_LIB_H
#define	UART_AVR_LIB_H

#define USART_WRITE_BYTE(data){ \
    USART0.STATUS |= 0b01000000; \
    USART0.TXDATAL = data; \
    while( (USART0.STATUS & 0b01000000) == 0) ; \
}

#define USART_READ_BYTE(data){ \
    while( (USART0.STATUS & 0b10000000) == 0) ; \
    data = USART0.RXDATAL; \
}

#define USART_PRINT_STR(str) { \
    uint8_t ind = 0; \
    while( str[ind]!='\0' ){ \
        USART_WRITE_BYTE(str[ind]); \
        ind++; \
    } \
}

#define USART_MENU_PROMPT { \
    USART_PRINT_STR("Press '?' for menu.\n\r"); \
    USART_PRINT_STR("{*} "); \
}

#define USART_READ_MENU_COMMAND { \
    uint8_t command, menuInd; \
    USART_READ_BYTE(command); \
    if( command == 0x3f ){ \
        USART_PRINT_STR("\n\r"); \
        for(menuInd = 0; menuInd < uartMenuSize; menuInd++ ){ \
            USART_WRITE_BYTE(uartMenu[menuInd].command + 0x30); \
            USART_PRINT_STR(": "); \
            USART_PRINT_STR(uartMenu[menuInd].help); \
            USART_PRINT_STR("\n\r"); \
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
            USART_PRINT_STR("Error: Invalid command.\r\n"); \
        } \
    } \
}
    
    
    

typedef void (*UartCommandFunc)();

typedef struct{
    uint8_t command;
    uint8_t *help;
    UartCommandFunc func;    
} UartCommand;




#endif
