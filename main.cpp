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
InfinityPortal myInfinityPortal;

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

	myInfinityPortal = InfinityPortal(infinityPortalIds[0]);

	while(true) {
		myInfinityPortal. fadeColour(1, 0xFF, 0x0, 0x0);
		myInfinityPortal. fadeColour(2, 0x0, 0xFF, 0x0);
		myInfinityPortal. fadeColour(3, 0x0, 0x0, 0xFF);
		usleep(500000); 
		myInfinityPortal. fadeColour(1, 0x0, 0xFF, 0x0);
		myInfinityPortal. fadeColour(2, 0x0, 0x0, 0xFF);
		myInfinityPortal. fadeColour(3, 0xFF, 0x0, 0x0);
		usleep(500000); 
		myInfinityPortal. fadeColour(1, 0x0, 0x0, 0xFF);
		myInfinityPortal. fadeColour(2, 0xFF, 0x0, 0x0);
		myInfinityPortal. fadeColour(3, 0x0, 0xFF, 0x0);
		usleep(500000);
	}
	auto turnOff = [] () {
		for(j = 1; j <= 3; j++) {
			myInfinityPortal.setColor(j, 0x0, 0x0, 0x0);
		}
	};
	atexit(turnOff);
	return 0;
}