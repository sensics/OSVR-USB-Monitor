/** @file
    @brief Header

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

#ifndef INCLUDED_USBDetectionC_h_GUID_04543989_122A_4FA3_21EF_EE8B921D272A
#define INCLUDED_USBDetectionC_h_GUID_04543989_122A_4FA3_21EF_EE8B921D272A

// Internal Includes
#include "USBCallbackTypesC.h"
#include "USBDeviceC.h"
#include "USBReturnCodesC.h"
#include "Export.h"
// Library/third-party includes
// - none

// Standard includes

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Initializes USB monitor. There are two modes : blocking and
 * non-blocking. Use 0 for non-blocking and 1 for blocking monitor
*/
OSVR_USBDETECTION_EXPORT OSVR_USBMonitor osvrUSBDetectionMonitorInit();

/* @brief Starts a blocking monitoring for USB events. For non-blocking, you
 * need to use osvrUSBDetectionMonitorUpdate */
OSVR_USBDETECTION_EXPORT void
osvrUSBDetectionStartMonitor(OSVR_USBMonitor monitor);

/* @brief Shuts down USB Monitor and destroys the associated object and
 * pointer*/
OSVR_USBDETECTION_EXPORT USB_ReturnCode
osvrUSBDetectionStopMonitor(OSVR_USBMonitor monitor);

/* @brief Checks if there is a USB event and will save info to device
descriptor.
If such an event occurs, its status code will tell you if device has been
added/removed or no change.
*/
OSVR_USBDETECTION_EXPORT USB_StatusCode osvrUSBDetectionMonitorUpdate(
    OSVR_USBMonitor monitor, OSVR_USBDeviceDescriptor *dev);

/* @brief Shuts down the USB monitor and cleans up*/
OSVR_USBDETECTION_EXPORT void
osvrUSBDetectionMonitorShutdown(OSVR_USBMonitor monitor);

/* @brief Returns number of USB devices that are connected and populates the
 * provided list of device descriptors. Must call free device list before
 * calling again*/
OSVR_USBDETECTION_EXPORT size_t osvrUSBDetectionGetDeviceList(
    OSVR_USBMonitor monitor, OSVR_USBDeviceDescriptor ***list);

/* @brief Cleans up a list of device descriptors */
OSVR_USBDETECTION_EXPORT void
osvrUSBDetectionFreeDeviceList(OSVR_USBDeviceDescriptor **devices,
                               size_t numDevices);

/* @brief Registers a callback for notifications when a new USB device is added
 * to a system. Must be called before osvrUSBDetectionStartMonitor */
OSVR_USBDETECTION_EXPORT USB_ReturnCode
osvrUSBDetectionRegisterDeviceAddedCallback(OSVR_USBMonitor monitor,
                                            OSVR_DeviceAddedCallback cb,
                                            void *userdata);

/* @brief Registers a callback for notifications when any USB device is removed
 * a system. Must be called before osvrUSBDetectionStartMonitor */
OSVR_USBDETECTION_EXPORT USB_ReturnCode
osvrUSBDetectionRegisterDeviceRemovedCallback(OSVR_USBMonitor monitor,
                                              OSVR_DeviceRemovedCallback cb,
                                              void *userdata);
#ifdef __cplusplus
}
#endif

#endif // INCLUDED_USBDetectionC_h_GUID_04543989_122A_4FA3_21EF_EE8B921D272A
