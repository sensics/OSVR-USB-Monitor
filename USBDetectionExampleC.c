/** @file
    @brief Implementation

    @date 2016

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2016 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Internal Includes
#include "USBDetectionC.h"
#include "USBDeviceC.h"
// Library/third-party includes
// - none

// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void AddedCallback(void *userdata,
                   const struct OSVR_USBDeviceDescriptor *device) {
    OSVR_USBDeviceDescriptor desc;
    desc.productID = device->productID;
    desc.vendorID = device->vendorID;
    printf("Added device with VID: %u, and PID: %u \n", device->vendorID,
           device->productID);
}

void RemovedCallback(void *userdata,
                     const struct OSVR_USBDeviceDescriptor *device) {
    OSVR_USBDeviceDescriptor desc;
    desc.productID = device->productID;
    desc.vendorID = desc.vendorID;
    printf("Removed device with VID: %u, and PID: %u \n", device->vendorID,
           device->productID);
}

int main() {

    OSVR_USBMonitor monitor = osvrUSBDetectionMonitorInit(0);

    /* Get a list of connected device
    OSVR_USBDeviceDescriptor **devList = NULL;
    size_t numDevices = osvrUSBDetectionGetDeviceList(monitor, &devList);
    osvrUSBDetectionFreeDeviceList(devList, numDevices);
    for (int i = 0; i < numDevices; i++) {
        printf("Detected device with VID: 0x%x, PID: 0x%x",
    devList[i]->vendorID,
               devList[i]->productID);
    }
    */

    /* Start a blocking monitor for USB events
    osvrUSBDetectionRegisterDeviceAddedCallback(monitor, &AddedCallback, NULL);
    osvrUSBDetectionRegisterDeviceRemovedCallback(monitor, &RemovedCallback,
                                                  NULL);
    osvrUSBDetectionStartMonitor(monitor);
    */

    /* Start a non blocking monitor for USB events*/
    OSVR_USBDeviceDescriptor dev;
    USB_ReturnCode ret;
    int ctr = 0;
    while (ctr < 30) {
        ret = osvrUSBDetectionMonitorUpdate(monitor, &dev);
        if (ret == OSVR_USB_DEVICE_ADDED) {
            printf("Added a device with VID: 0x%x, PID: 0x%x\n", dev.vendorID,
                   dev.productID);
        } else if (ret == OSVR_USB_DEVICE_REMOVED) {
            printf("Removed a device with VID: 0x%x, PID: 0x%x\n", dev.vendorID,
                   dev.productID);
        } else {
            // printf("Got no status change \n");
        }
        Sleep(1000);
        ctr++;
    }
    osvrUSBDetectionMonitorShutdown(monitor);

    return 0;
}
