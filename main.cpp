/** 
 * File:   main.cpp
 * Author: Integ
 *
 * Created on 10 August 2014, 21:09
 **/

#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <math.h>
#include "libusb-1.0/libusb.h"
#include <unistd.h>
#include "InfinityPortal.h"

using namespace std;

int main(int argc, char** argv) {
	libusb_device** devices;
	libusb_context* context;
	struct libusb_device_handle* tryDeviceHandler;

	libusb_init(&context);
	int devicesCount = libusb_get_device_list(context, &devices);

	struct libusb_device_descriptor descriptor;
	libusb_device_handle* deviceHandler;

	int retVal = 0;
	int infinityPortalIds[0xff];
	int infinityPortalCount = 0;

	for(int i = 0 ; i < devicesCount ; i++) {
		retVal = libusb_open(devices[i], &tryDeviceHandler);
		if(retVal < 0) {
			continue;
		}
		libusb_get_device_descriptor(devices[i], &descriptor);
		if(descriptor.idVendor == 0x0e6f && descriptor.idProduct == 0x0129) {
			printf("Found Infinity Portal\n");
			infinityPortalIds[infinityPortalCount] = i;
			infinityPortalCount++;
		}
	}

	if(infinityPortalCount == 0) {
		printf("Please plug in an Infinity Base\n");
		return -1;
	}

	InfinityPortal infinityPortals[infinityPortalCount];
	int j;

	for(j = 0 ; j < infinityPortalCount ; j++) {
		infinityPortals[j] = InfinityPortal(infinityPortalIds[j]);
	}

	while(true) {
		for(j = 0 ; j < infinityPortalCount; j++) {
			infinityPortals[j]. fadeColour(1, 0xFF, 0x0, 0x0);
			infinityPortals[j]. fadeColour(2, 0x0, 0xFF, 0x0);
			infinityPortals[j]. fadeColour(3, 0x0, 0x0, 0xFF);
			usleep(500000); 
			infinityPortals[j]. fadeColour(1, 0x0, 0xFF, 0x0);
			infinityPortals[j]. fadeColour(2, 0x0, 0x0, 0xFF);
			infinityPortals[j]. fadeColour(3, 0xFF, 0x0, 0x0);
			usleep(500000); 
			infinityPortals[j]. fadeColour(1, 0x0, 0x0, 0xFF);
			infinityPortals[j]. fadeColour(2, 0xFF, 0x0, 0x0);
			infinityPortals[j]. fadeColour(3, 0x0, 0xFF, 0x0);
			usleep(500000);
		}
	}
	return 0;
}
