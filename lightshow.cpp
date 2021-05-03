/** 
 * File:   lightshow.cpp
 * Author: Integ
 * Created on 10 August 2014, 21:09
 **/

#include <cstdlib>
#include <csignal>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include "libusb-1.0/libusb.h"
#include <unistd.h>
#include "InfinityPortal.h"

using namespace std;
InfinityPortal myInfinityPortal;

void turnOff(int signum) {
	cout << "Turning Lights Off, (" << signum << ") recived. \n";
	for(int i = 1; i <= 3; i++) {
		myInfinityPortal.setColour(i, 0x0, 0x0, 0x0);
	}
	exit(signum);
}

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

	myInfinityPortal = InfinityPortal(infinityPortalIds[0]);

	myInfinityPortal.setColour(1, 0xFF, 0x0, 0x0);
	myInfinityPortal.setColour(2, 0x0, 0xFF, 0x0);
	myInfinityPortal.setColour(3, 0x0, 0x0, 0xFF);

	signal(SIGINT, turnOff);
	signal(SIGTERM, turnOff);

        for (std::string line; std::getline(std::cin, line);) {
		size_t pos1 = line.find(" ", 0);
		size_t pos2 = line.find(" ", pos1 + 1);
		size_t pos3 = line.find(" ", pos2 + 1);
		size_t pos4 = line.find(" ", pos3 + 1);
		size_t pos5 = line.find(" ", pos4 + 1);
		size_t pos6 = line.find(" ", pos5 + 1);
		size_t pos7 = line.find(" ", pos6 + 1);
		size_t pos8 = line.find(" ", pos7 + 1);
	
		int r1 = stoi(line.substr(0, pos1).c_str());
		int g1 = stoi(line.substr(pos1 + 1, pos2 - pos1).c_str());
		int b1 = stoi(line.substr(pos2 + 1, pos3 - pos2).c_str());
        
		int r2 = stoi(line.substr(pos3 + 1, pos4 - pos3).c_str());
		int g2 = stoi(line.substr(pos4 + 1, pos5 - pos4).c_str());
		int b2 = stoi(line.substr(pos5 + 1, pos6 - pos5).c_str());
        
		int r3 = stoi(line.substr(pos6 + 1, pos7 - pos6).c_str());
		int g3 = stoi(line.substr(pos7 + 1, pos8 - pos7).c_str());
		int b3 = stoi(line.substr(pos8 + 1, line.size()).c_str());
        
		printf("Got r1:%d g1:%d b1:%d r2:%d g2:%d b2:%d r3:%d g3:%d b3:%d\n", r1, g1, b1, r2, g2, b2, r3, g3, b3);
		myInfinityPortal.setColour(1, r1, g1, b1);
		myInfinityPortal.setColour(2, r2, g2, b2);
		myInfinityPortal.setColour(3, r3, g3, b3);
	}

	return 0;
}
