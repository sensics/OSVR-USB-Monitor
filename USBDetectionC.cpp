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
#include "USBDetection.h"

// Library/third-party includes
// - none

// Standard includes
#include <stdlib.h>

OSVR_USBMonitor osvrUSBDetectionMonitorInit() {
    OSVR_USBMonitor monitor = new OSVR_USBMonitorObject();
    return monitor;
}

void osvrUSBDetectionStartMonitor(OSVR_USBMonitor monitor) {

    monitor->startMonitor();
}

USB_ReturnCode osvrUSBDetectionMonitorUpdate(OSVR_USBMonitor monitor,
                                             OSVR_USBDeviceDescriptor *dev) {

    return monitor->checkMessages(dev);
}

OSVR_ReturnCode osvrUSBDetectionStopMonitor(OSVR_USBMonitor monitor) {

    return OSVR_RETURN_SUCCESS;
}

void osvrUSBDetectionMonitorShutdown(OSVR_USBMonitor monitor) {
    monitor->monitorShutdown();
    delete (monitor);
}

size_t osvrUSBDetectionGetDeviceList(OSVR_USBMonitor monitor,
                                     OSVR_USBDeviceDescriptor ***devices) {

    std::vector<OSVR_USBDeviceDescriptor> devList = monitor->getDeviceList();
    size_t numDevices = devList.size();
    OSVR_USBDeviceDescriptor **list = (OSVR_USBDeviceDescriptor **)malloc(
        numDevices * sizeof(OSVR_USBDeviceDescriptor *));
    for (int i = 0; i < numDevices; i++) {
        list[i] = (OSVR_USBDeviceDescriptor *)malloc(
            sizeof(OSVR_USBDeviceDescriptor));
        list[i]->productID = devList.at(i).productID;
        list[i]->vendorID = devList.at(i).vendorID;
    }
    *devices = list;
    return numDevices;
}

void osvrUSBDetectionFreeDeviceList(OSVR_USBDeviceDescriptor **devices,
                                    size_t numDevices) {
    for (int i = 0; i < numDevices; i++) {
        free(devices[i]);
    }
    free(devices);
}

OSVR_ReturnCode osvrUSBDetectionRegisterDeviceAddedCallback(
    OSVR_USBMonitor monitor, OSVR_DeviceAddedCallback cb, void *userdata) {
    monitor->registerDeviceAddedCallback(cb, userdata);
    return OSVR_RETURN_SUCCESS;
}

OSVR_ReturnCode osvrUSBDetectionRegisterDeviceRemovedCallback(
    OSVR_USBMonitor monitor, OSVR_DeviceRemovedCallback cb, void *userdata) {
    monitor->registerDeviceRemovedCallback(cb, userdata);
    return OSVR_RETURN_SUCCESS;
}
