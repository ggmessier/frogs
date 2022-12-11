# AVR Software

A mix of project and tutorial/demo code.

## Projects

- `FlashLed.X`: A hello world style program.  Covers: config register code, register access and digital output.

## Development Environment

Microchip's tool [overview page](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide):
- Download links for MPLABX are at the bottom of the page.
  + Install IDE and 8 bit support.
  + The IPE is a scaled down programmer geared towards production stuff.
  + Skip the MCC since we'll write directly to the registers.
- We want the [XC compiler](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-xc-compilers) and not gcc.
  + This compiler supports both PIC and AVR devices.




