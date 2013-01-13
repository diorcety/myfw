/**
 * Copyright (C) 2009 Ubixum, Inc. 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **/

#include <fx2macros.h>
#include <delay.h>

#define SYNCDELAY() SYNCDELAY4

//#ifdef DEBUG_FIRMWARE
#include <stdio.h>
#include <string.h>
//#else
#undef printf
int printf(const char *format, ...) {
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
//#define printf(...)
//#endif

//************************** Configuration Handlers *****************************

// change to support as many interfaces as you need
//volatile xdata BYTE interface=0;
//volatile xdata BYTE alt=0; // alt interface

// set *alt_ifc to the current alt interface for ifc
BOOL handle_get_interface(BYTE ifc, BYTE* alt_ifc) {
// *alt_ifc=alt;
 return TRUE;
}
// return TRUE if you set the interface requested
// NOTE this function should reconfigure and reset the endpoints
// according to the interface descriptors you provided.
BOOL handle_set_interface(BYTE ifc,BYTE alt_ifc) {  
 //printf( "Set Interface.\n" );
 //interface=ifc;
 //alt=alt_ifc;
 return TRUE;
}

// handle getting and setting the configuration
// 1 is the default.  If you support more than one config
// keep track of the config number and return the correct number
// config numbers are set int the dscr file.
//volatile BYTE config=1;
BYTE handle_get_configuration() { 
 return 1;
}

volatile __bit aa=0;
// NOTE changing config requires the device to reset all the endpoints
BOOL handle_set_configuration(BYTE cfg) { 
 printf ( "Set Configuration %d.\n", ++aa);
 //config=cfg;
 return TRUE;
}


//******************* VENDOR COMMAND HANDLERS **************************


BOOL handle_vendorcommand(BYTE cmd) {
 // your custom vendor handler code here..
 return FALSE; // not handled by handlers
}


//********************  INIT ***********************

void main_init() {

 REVCTL=3;
 SETIF48MHZ();

 // set IFCONFIG
 EP2CFG = 0xA2; // 10100010
 SYNCDELAY();
 EP6CFG = 0xE2; // 11100010 
 SYNCDELAY();
 EP1INCFG = 0xA0;
 SYNCDELAY();
 EP1OUTCFG = 0xA0;
 SYNCDELAY();
 EP4CFG = 0x90;
 SYNCDELAY();
 EP8CFG = 0xA0;
 SYNCDELAY(); 

 //printf ( "Initialization Done.\n" );

}


void main_loop() {
// printf ( "Test\n");
 // do some work
}


