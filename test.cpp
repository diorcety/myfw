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

#include <stdlib.h>
#include <cstdio>
#include <cassert>
#ifdef WIN32
#include <windows.h>
	void usleep(int s) {
	  Sleep(s/1000);
	}
	#include <libusbx-1.0/libusb.h>
#else
	#include <libusb-1.0/libusb.h>
#endif

typedef struct _debug_packet {
	unsigned char *data;
	libusb_device_handle *hndl;
} debug_packet;

void debug_wait(libusb_device_handle *hndl);

void debug_callback(libusb_transfer *transfer) {
	printf("!!Receive\n");
	debug_packet *dp = (debug_packet *) transfer->user_data;
	libusb_free_transfer(transfer);
	free(dp->data);
	free(dp);
}

void debug_wait(libusb_device_handle *hndl) {
	int rv;
	libusb_transfer *transfer = libusb_alloc_transfer(1);
	debug_packet *dp = (debug_packet *)malloc(sizeof(debug_packet));
	dp->data = (unsigned char *)malloc(256);
	dp->hndl = hndl;
    libusb_fill_iso_transfer(transfer, hndl, 0x4, dp->data, 256, 1, (libusb_transfer_cb_fn)debug_callback, dp, 0);
	rv = libusb_submit_transfer(transfer);
	if(rv != 0) {
		perror ("");
        printf("libusb_submit_transfer failed: %s(%d)\n", libusb_error_name(rv), rv);
	} else {
		printf("!!Submit\n");
	}
}

int main(int argc, char* argv[]) {
 int rv;
 libusb_context* ctx;
 libusb_init(&ctx);
 libusb_device_handle* hndl = libusb_open_device_with_vid_pid(ctx,0x04b4,0x1004);
 if(hndl == NULL) {
   printf("Can't open device\n");
   return -100;
 }
 
#ifndef WIN32
 rv = libusb_kernel_driver_active(hndl, 0); 
 if (rv == 1) { 
	rv = libusb_detach_kernel_driver(hndl, 0); 
 } 
#endif
 libusb_set_configuration(hndl, 1);
 libusb_claim_interface(hndl,0);
 libusb_set_interface_alt_setting(hndl,0,0);
 
 debug_wait(hndl);
 
 int transferred;
 unsigned char buf2[128];
 int np = 0;
 while(1) {
   printf(".");
   fflush(stdout);
   rv=libusb_bulk_transfer(hndl,0x86,(unsigned char*)buf2,sizeof(buf2),&transferred,100); 
   if (rv == 0 && transferred > 0) {
	printf("%s", buf2); 
	fflush(stdout);
   } else if(rv != LIBUSB_ERROR_TIMEOUT) {
        printf ( "IN Transfer failed: %s(%d)\n", libusb_error_name(rv), rv);
        return rv;

   }
   
   if(((++np)%50) == 0) {
     printf("Send control\n");
	 rv = libusb_control_transfer(hndl, 
		LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE,
		0x99,
		0x11,
		0x22,
		0,
		0,
		0);
	if(rv != 0) {
        printf ( "CONTROL Transfer failed: %s(%d)\n", libusb_error_name(rv), rv);
        return rv;
	}
   }
 }


 return 0;
}
