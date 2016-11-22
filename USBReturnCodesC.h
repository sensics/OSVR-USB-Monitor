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

#ifndef INCLUDED_ReturnCodesC_h_GUID_D810ED13_F3B8_41BE_C947_51D0D51F420B
#define INCLUDED_ReturnCodesC_h_GUID_D810ED13_F3B8_41BE_C947_51D0D51F420B

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
// - none

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSVR_USB_NO_STATUS_CHANGE,
    OSVR_USB_DEVICE_ADDED,
    OSVR_USB_DEVICE_REMOVED
} USB_ReturnCode;

#ifdef __cplusplus
}
#endif

#endif // INCLUDED_ReturnCodesC_h_GUID_D810ED13_F3B8_41BE_C947_51D0D51F420B
