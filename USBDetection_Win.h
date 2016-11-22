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

#ifndef INCLUDED_USBDetection_Win_h_GUID_49312DF7_61D5_4888_4724_EF81D0A6F99E
#define INCLUDED_USBDetection_Win_h_GUID_49312DF7_61D5_4888_4724_EF81D0A6F99E


// Internal Includes
//#include "Export.h"
#include "USBCallbackTypesC.h"
#include "USBDeviceC.h"
#include "USBReturnCodesC.h"

// Library/third-party includes
// - none

// Standard includes
#include <windows.h>
#include <winuser.h>
#include <Dbt.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <SetupAPI.h>
#include <stdint.h>
#include <algorithm>

#define USB_DEVICE_GUID                                                        \
    {                                                                          \
        0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, \
            0xED                                                               \
    }
#define CLS_NAME "DUMMY_CLASS"
#define HWND_MESSAGE ((HWND)-3)

struct OSVR_USBMonitorObject {

  public:
    OSVR_USBDETECTION_EXPORT OSVR_USBMonitorObject()
        : hWnd(NULL), m_addedCb(nullptr), m_removedCb(nullptr),
          m_status(OSVR_USB_NO_STATUS_CHANGE) {

        WNDCLASSEX wx;
        ZeroMemory(&wx, sizeof(wx));

        wx.cbSize = sizeof(WNDCLASSEX);
        wx.lpfnWndProc = reinterpret_cast<WNDPROC>(staticMessageHandler);
        wx.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
        wx.style = CS_HREDRAW | CS_VREDRAW;
        wx.hInstance = GetModuleHandle(0);
        wx.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wx.lpszClassName = CLS_NAME;

        if (RegisterClassEx(&wx)) {
            hWnd = CreateWindow(CLS_NAME, "DevNotifyWnd", WS_ICONIC, 0, 0,
                                CW_USEDEFAULT, 0, HWND_MESSAGE, NULL,
                                GetModuleHandle(0),
                                reinterpret_cast<void *>(this));
        }

        if (hWnd == NULL) {
            /// @todo Need an error handling here
            // qDebug() << "Could not create message window!";
        }
    }

    OSVR_USBDETECTION_EXPORT ~OSVR_USBMonitorObject() {
        monitorShutdown();
        DestroyWindow(hWnd);
    }

    OSVR_USBDETECTION_EXPORT void monitorShutdown(){
        PostQuitMessage(0);
    }

    OSVR_USBDETECTION_EXPORT void startMonitor() {
        if (hWnd == NULL) {
            /// @todo Need an error handling here
            return;
        } else {
            MSG msg;
            std::cout << "Starting USB monitor" << std::endl;
            while (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            std::cout << "Stopping USB monitor" << std::endl;
        }
    }

    OSVR_USBDETECTION_EXPORT USB_StatusCode
    checkMessages(OSVR_USBDeviceDescriptor *dev) {
        MSG msg;
        bool havePendingMsgs =
            (HIWORD(GetQueueStatus(QS_SENDMESSAGE)) & QS_SENDMESSAGE) != 0;
        // retrieve and dispatch latest sent message
        PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
        if (havePendingMsgs) {
            dev->vendorID = m_device.vendorID;
            dev->productID = m_device.productID;
            return m_status;
        }
        return OSVR_USB_NO_STATUS_CHANGE;
    }

    OSVR_USBDETECTION_EXPORT void
    registerDeviceAddedCallback(OSVR_DeviceAddedCallback cb, void *userdata) {
        m_addedCb = cb;
    }

    OSVR_USBDETECTION_EXPORT void
    registerDeviceRemovedCallback(OSVR_DeviceRemovedCallback cb,
                                  void *userdata) {
        m_removedCb = cb;
    }

    void triggerAddedCallback(OSVR_USBDeviceDescriptor const &dev) {
        m_addedCb(nullptr, &dev);
    }

    void triggerRemovedCallback(OSVR_USBDeviceDescriptor const &dev) {
        m_removedCb(nullptr, &dev);
    }

    OSVR_USBDETECTION_EXPORT std::vector<OSVR_USBDeviceDescriptor>
    getDeviceList() {
        std::vector<OSVR_USBDeviceDescriptor> deviceList;
        HDEVINFO hDevInfo;
        SP_DEVICE_INTERFACE_DATA DevIntfData;
        PSP_DEVICE_INTERFACE_DETAIL_DATA DevIntfDetailData;
        SP_DEVINFO_DATA DevData;

        DWORD dwSize, dwMemberIdx;
        GUID guid = USB_DEVICE_GUID;

        // We will try to get device information set for all USB devices that
        // have a
        // device interface and are currently present on the system (plugged
        // in).
        hDevInfo = SetupDiGetClassDevs(&guid, NULL, 0,
                                       DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

        if (hDevInfo != INVALID_HANDLE_VALUE) {
            // Prepare to enumerate all device interfaces for the device
            // information
            // set that we retrieved with SetupDiGetClassDevs(..)
            DevIntfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
            dwMemberIdx = 0;

            SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &guid, dwMemberIdx,
                                        &DevIntfData);

            while (GetLastError() != ERROR_NO_MORE_ITEMS) {

                DevData.cbSize = sizeof(DevData);

                SetupDiGetDeviceInterfaceDetail(hDevInfo, &DevIntfData, NULL, 0,
                                                &dwSize, NULL);

                DevIntfDetailData =
                    reinterpret_cast<PSP_DEVICE_INTERFACE_DETAIL_DATA>(
                        HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize));
                DevIntfDetailData->cbSize =
                    sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

                if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &DevIntfData,
                                                    DevIntfDetailData, dwSize,
                                                    &dwSize, &DevData)) {

                    std::string devPath(DevIntfDetailData->DevicePath);
                    OSVR_USBDeviceDescriptor desc;
                    parseDeviceDescriptorFromPath(&desc, devPath);
                    /*desc.path = (char *)malloc(strlen(devPath.c_str() + 1));
                    strcpy(desc.path, devPath.c_str());*/
                    deviceList.push_back(desc);
                }

                HeapFree(GetProcessHeap(), 0, DevIntfDetailData);

                // Continue looping
                SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &guid,
                                            ++dwMemberIdx, &DevIntfData);
            }
            SetupDiDestroyDeviceInfoList(hDevInfo);
        }

        return deviceList;
    }

  private:
    static void setObjectToHWnd(HWND hWnd, LPARAM lParam) {
        LPCREATESTRUCT cs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        OSVR_USBMonitorObject *pWnd =
            reinterpret_cast<OSVR_USBMonitorObject *>(cs->lpCreateParams);

        SetLastError(0);

        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA,
                              reinterpret_cast<DWORD_PTR>(pWnd)) &&
            GetLastError()) {
            /// @todo Add error handling here
            // qDebug() << "Got a windows error";
        }
    }

    static OSVR_USBMonitorObject *getObjectFromHwnd(HWND hWnd) {
        return reinterpret_cast<OSVR_USBMonitorObject *>(
            GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    static LRESULT staticMessageHandler(HWND__ *hwnd, UINT uint, WPARAM wparam,
                                        LPARAM lparam) {
        if (uint == WM_NCCREATE) {
            setObjectToHWnd(hwnd, lparam);
        }
        OSVR_USBMonitorObject *pWnd = getObjectFromHwnd(hwnd);

        if (pWnd) {
            return pWnd->messageHandler(hwnd, uint, wparam, lparam);
        } else {
            return DefWindowProc(hwnd, uint, wparam, lparam);
        }
    }

    LRESULT messageHandler(HWND__ *hwnd, UINT uint, WPARAM wparam,
                           LPARAM lparam) {
        switch (uint) {
        case WM_NCCREATE: // before window creation
            return true;
            break;

        case WM_CREATE: // the actual creation of the window
        {
            // you can get your creation params here..like GUID..
            LPCREATESTRUCT params = (LPCREATESTRUCT)lparam;

            GUID InterfaceClassGuid = USB_DEVICE_GUID;
            DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
            ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
            NotificationFilter.dbcc_size =
                sizeof(DEV_BROADCAST_DEVICEINTERFACE);
            NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
            NotificationFilter.dbcc_classguid = InterfaceClassGuid;
            HDEVNOTIFY dev_notify = RegisterDeviceNotification(
                hwnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
            if (dev_notify == NULL) {
                throw std::runtime_error(
                    "Could not register for device notifications!");
            }
            break;
        }

        case WM_DEVICECHANGE: {
            PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lparam;
            PDEV_BROADCAST_DEVICEINTERFACE lpdbv =
                (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
            std::string path;
            if (lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) {
                path = std::string(lpdbv->dbcc_name);
                parseDeviceDescriptorFromPath(&m_device, path);
                switch (wparam) {
                case DBT_DEVICEARRIVAL:
                    m_status = OSVR_USB_DEVICE_ADDED;
                    if (m_addedCb) {
                        m_addedCb(nullptr, &m_device);
                    }
                    break;

                case DBT_DEVICEREMOVECOMPLETE:
                    m_status = OSVR_USB_DEVICE_REMOVED;
                    if (m_removedCb) {
                        m_removedCb(nullptr, &m_device);
                    }
                    break;
                }
            }
            break;
        }
        }
        return 0L;
    }

    void parseDeviceDescriptorFromPath(OSVR_USBDeviceDescriptor *desc,
                                       std::string &path) {
        std::transform(path.begin(), path.end(), path.begin(), ::toupper);
        desc->vendorID = extractVidFromPath(path);
        desc->productID = extractPidFromPath(path);
    }

    uint16_t extractVidFromPath(const std::string &path) const {

        // do nothing for now
        std::size_t pos = path.find("VID_") + 4;
        std::string pidStr = path.substr(pos, 4);
        uint16_t pid = strtoul(pidStr.c_str(), NULL, 16);

        return pid;
    }

    uint16_t extractPidFromPath(const std::string &path) const {

        std::size_t pos = path.find("PID_") + 4;
        std::string vidStr = path.substr(pos, 4);
        uint16_t vid = strtoul(vidStr.c_str(), NULL, 16);

        return vid;
    }

    OSVR_DeviceAddedCallback m_addedCb;
    OSVR_DeviceRemovedCallback m_removedCb;
    HWND hWnd;
    OSVR_USBDeviceDescriptor m_device;
    USB_StatusCode m_status;
};

#endif // INCLUDED_USBDetection_Win_h_GUID_49312DF7_61D5_4888_4724_EF81D0A6F99E

