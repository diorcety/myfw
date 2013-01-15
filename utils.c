#include "utils.h"
#include <fx2macros.h>
#include <delay.h>
#define SYNCDELAY() SYNCDELAY4


#define MAX_BUFFER
static __bit usb_debug = 0;
void usb_debug_enable() {
  usb_debug = 1;
}

__bit usb_debug_enabled() {
  return usb_debug;
}
void usb_debug_disable() {
  usb_debug = 0;
}

int usb_printf(const char *format, ...) {
    int len;
    int ret;
    va_list argptr;
    char *dest = EP6FIFOBUF;
    //return -1;
    if(!usb_debug) return -1;
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
