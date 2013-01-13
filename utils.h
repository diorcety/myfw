#include <stdio.h>
#include <string.h>

// EPxCFG bits
#define bmBULK bmBIT5
#define bmISO bmBIT4
#define bmINT (bmBIT5 | bmBIT4)
#define bmBUF2X bmBIT1
#define bmBUF4X 0
#define bmBUF3X (bmBIT1 | bmBIT2)

#ifndef UTILS__
#define UTILS__
int usb_printf(const char *format, ...);
int usb_printf2(const char *format, ...);
#endif //UTILS__

#ifdef DEBUG_FIRMWARE 
#include <serial.h>
#include <stdio.h>
#else
#define printf(...)
#endif
