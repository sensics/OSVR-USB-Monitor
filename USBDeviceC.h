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

#ifndef INCLUDED_USBDeviceC_h_GUID_102E66C5_3FDA_4DB2_36D2_1F14098BD0CF
#define INCLUDED_USBDeviceC_h_GUID_102E66C5_3FDA_4DB2_36D2_1F14098BD0CF

// Internal Includes

// Library/third-party includes
// - none

// Standard includes
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSVR_USBMonitorObject *OSVR_USBMonitor;

typedef struct OSVR_USBDeviceDescriptor {
    uint16_t productID;
    uint16_t vendorID;
} OSVR_USBDeviceDescriptor;

#ifdef __cplusplus
}
#endif

#endif // INCLUDED_USBDeviceC_h_GUID_102E66C5_3FDA_4DB2_36D2_1F14098BD0CF
