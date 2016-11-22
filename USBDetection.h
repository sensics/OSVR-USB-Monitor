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

#ifndef INCLUDED_USBDetection_h_GUID_4089DA4D_11DE_44A7_5776_DC0CD6E6F03C
#define INCLUDED_USBDetection_h_GUID_4089DA4D_11DE_44A7_5776_DC0CD6E6F03C

#include "PlatformConfig.h"

// Library/third-party includes
// - none

// Standard includes
#include <string>

#if defined(OSVR_WINDOWS)
#include "USBDetection_Win.h"
#else
#error "USBDetection not yet implemented for this platform!"

#endif

#endif // INCLUDED_USBDetection_h_GUID_4089DA4D_11DE_44A7_5776_DC0CD6E6F03C
