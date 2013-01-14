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
 usb_printf ( "Set Configuration %d.\n", ++aa);
 //config=cfg;
 return TRUE;
}


//******************* VENDOR COMMAND HANDLERS **************************


BOOL handle_vendorcommand(BYTE cmd) {
	if(cmd == 0x99) {
		EP0BCH=0;
		EP0BCL=0;
		EP0CS |= bmHSNAK;
		usb_printf2("Test\n");
		return TRUE;
	}
	return FALSE;
}


//********************  INIT ***********************

void main_init() {

 REVCTL=0x0;
 SETIF48MHZ();

  // Reset all the FIFOs
 FIFORESET = bmNAKALL; SYNCDELAY();
 FIFORESET = bmNAKALL | 2; SYNCDELAY();  // reset EP2
 FIFORESET = bmNAKALL | 4; SYNCDELAY();  // reset EP4
 FIFORESET = bmNAKALL | 6; SYNCDELAY();  // reset EP6
 FIFORESET = bmNAKALL | 8; SYNCDELAY();  // reset EP8
 FIFORESET = 0x00; SYNCDELAY();
 
 // set IFCONFIG
 EP1INCFG &= ~bmVALID; SYNCDELAY();
 EP1OUTCFG &= ~bmVALID; SYNCDELAY();
 EP2CFG &= ~bmVALID; SYNCDELAY();
 EP4CFG = (bmVALID | bmISO | bmBUF2X | bmDIR); SYNCDELAY();
 EP6CFG = (bmVALID | bmBULK | bmBUF2X | bmDIR); SYNCDELAY();
 EP8CFG &= ~bmVALID; SYNCDELAY(); 
}

int bb=0;
int cc=0;
void main_loop() {
}
