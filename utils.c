#include "utils.h"
#include <fx2macros.h>
#include <delay.h>
#define SYNCDELAY() SYNCDELAY4

int usb_printf(const char *format, ...) {
    int len;
    int ret;
    va_list argptr;
    char *dest = EP6FIFOBUF;
    while(EP2468STAT & bmEP6FULL) {
           __asm
           nop
           nop
           nop
           nop
           nop
           nop
           nop
           __endasm;
    }
    dest[0]='\0';
    va_start(argptr, format);
    ret = vsprintf(dest, format, argptr);
    va_end(argptr);
    len = strlen(dest);
    if(len > 0) {
    len++;
      // ARM ep6 out
      EP6BCH=MSB(len);
      SYNCDELAY();
      EP6BCL=LSB(len);
    }
    return ret;
}
int usb_printf2(const char *format, ...) {
    int len;
    int ret;
    va_list argptr;
    char *dest = EP4FIFOBUF;
    while(EP2468STAT & bmEP4FULL) {
           __asm
           nop
           nop
           nop
           nop
           nop
           nop
           nop
           __endasm;
    }
    dest[0]='\0';
    va_start(argptr, format);
    ret = vsprintf(dest, format, argptr);
    va_end(argptr);
    len = strlen(dest);
    if(len > 0) {
    len++;
      // ARM ep4 out
      EP4BCH=MSB(len);
      SYNCDELAY();
      EP4BCL=LSB(len);
    }
    return ret;
}
//#define printf(...)
//#endif