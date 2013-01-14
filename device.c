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
#include <setupdat.h>
#include "utils.h"

#define SYNCDELAY() SYNCDELAY4
void reset();

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
BOOL handle_set_interface(BYTE ifc, BYTE alt_ifc) {  
 usb_printf("Set Interface 0x%x 0x%x.\n", ifc, alt_ifc);
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
 usb_printf("Set Configuration 0x%x.\n", cfg);
 //config=cfg;
 return TRUE;
}


//******************* VENDOR COMMAND HANDLERS **************************
long bench_size = 0;
BOOL handle_vendorcommand(BYTE cmd) {
	if(cmd == 0x90) {
		reset();
		EP0BCH = 0;
		EP0BCL = 0;
		EP0CS |= bmHSNAK;
		return TRUE;
	} else if(cmd == 0x91) {
		short val = SETUP_VALUE();
		if(val != 0) {
		 usb_debug_enable();
		 usb_printf("#Debug enabled\n");
		} else {
		 usb_debug_disable();
		}
		EP0BCH = 0;
		EP0BCL = 0;
		EP0CS |= bmHSNAK;
		return TRUE;
	} else if(cmd == 0x92) {
		if(bench_size == 0) {
			bench_size = SETUP_INDEX();
			bench_size = SETUP_VALUE() | (bench_size << 16);
			usb_printf("#Start %ld\n", bench_size);
		}
		EP0BCH = 0;
		EP0BCL = 0;
		EP0CS |= bmHSNAK;
		return TRUE;
	} else if(cmd == 0x93) {
		if(bench_size != 0) {
			usb_printf("#Interrupt\n");
			bench_size = 0;
			SYNCDELAY(); FIFORESET = bmNAKALL;
			SYNCDELAY(); FIFORESET = bmNAKALL | 2;  // reset EP2
			SYNCDELAY(); FIFORESET = 0x00;
		}
		EP0BCH = 0;
		EP0BCL = 0;
		EP0CS |= bmHSNAK;
		return TRUE;
	} else if(cmd == 0x99) {
		EP0BCH = 0;
		EP0BCL = 0;
		EP0CS |= bmHSNAK;
		usb_printf("#Test\n");
		return TRUE;
	}
	return FALSE;
}

void reset() {
 bench_size = 0;
  // Reset all the FIFOs
 FIFORESET = bmNAKALL;
 SYNCDELAY(); FIFORESET = bmNAKALL | 2;  // reset EP2
 SYNCDELAY(); FIFORESET = bmNAKALL | 4;  // reset EP4
 SYNCDELAY(); FIFORESET = bmNAKALL | 6;  // reset EP6
 SYNCDELAY(); FIFORESET = bmNAKALL | 8;  // reset EP8
 SYNCDELAY(); FIFORESET = 0x00;
 SYNCDELAY(); 
 
 /*// Reset data toggle to 0
 TOGCTL = 0x12;  // EP2 IN
 TOGCTL = 0x32;  // EP2 IN Reset
 SYNCDELAY();
 
  // Reset data toggle to 0
 TOGCTL = 0x18;  // EP8 IN
 TOGCTL = 0x38;  // EP8 IN Reset
 SYNCDELAY();*/
 usb_debug_disable();
}

//********************  INIT ***********************

void main_init() {
 REVCTL = 0x03;
 SYNCDELAY(); 
 SETIF48MHZ();
 SYNCDELAY(); 
  
 reset();
 
  // set IFCONFIG
 EP1INCFG &= ~bmVALID; SYNCDELAY(); 
 EP1OUTCFG &= ~bmVALID;
 EP2CFG = (bmVALID | bmBULK | bmBUF3X | bmBUF1024 | bmDIR); SYNCDELAY();
 EP4CFG &= ~bmVALID; /* = (bmVALID | bmISO | bmBUF2X | bmDIR);*/ SYNCDELAY(); 
 EP6CFG &= ~bmVALID;  SYNCDELAY();
 EP8CFG = (bmVALID | bmBULK | bmBUF2X | bmDIR); SYNCDELAY(); 
}

void main_loop() {
	if(bench_size) {
		while((EP2468STAT & bmEP2FULL) == 0) {
			int len = bench_size;
			if(len > 1024) {
				len = 1024;
			}
			bench_size -= len;
			
			// ARM ep2 in
			EP2BCH = MSB(len);
			SYNCDELAY();
			EP2BCL = LSB(len);
			SYNCDELAY();
			
			if(bench_size == 0) {
				usb_printf("#End\n");
				break;
			}
		}
	}
}
