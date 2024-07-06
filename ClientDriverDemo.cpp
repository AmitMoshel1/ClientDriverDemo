#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "ClientDriverDemoHeader.h"

int main(int s, const char* argv[]) {

    HANDLE hDevice = CreateFile(LR"(\\.\DriverDemoDevice)", GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Handle to the device couldn't be created: %u\n", GetLastError());
        return -1;
    }

    ULONG InputBuffer = 0x1000;
    CHAR message[512];
    DWORD BytesReturned;
    int result = DeviceIoControl(hDevice,      // Handle to Device Object
        IOCTL_BUFFERED_METHOD,                 // IOCTL
        &InputBuffer,                          // InputBuffer
        1,                   // Size of InputBuffer
        message,                               // OutputBuffer
        512,                                   // size of OutputBuffer
        &BytesReturned,                        // Bytes Returned
        nullptr);
 
    if (result)
        printf("\n IOCTL_BUFFERED_METHOD:\nData Received from driver: \n%s\n", message);
    else
        printf("\n IOCTL_BUFFERED_METHOD Problem with executing IOCTL... (%u)\n", GetLastError());

    getchar();

    result = DeviceIoControl(hDevice,      // Handle to Device Object
        IOCTL_DIRECT_METHOD,               // IOCTL
        &InputBuffer,                      // InputBuffer
        sizeof(InputBuffer),               // Size of InputBuffer
        message,                           // OutputBuffer
        512,                               // size of OutputBuffer
        &BytesReturned,                    // Bytes Returned to OutputBuffer
        nullptr);
    
    if (result)
        printf("\n IOCTL_DIRECT_METHOD: Data Received from driver: %s\n", message);
    else
        printf("\n IOCTL_DIRECT_METHOD Problem with executing IOCTL... (%u)\n", GetLastError());

    getchar();

    result = DeviceIoControl(hDevice,       // Handle to Device Object
        IOCTL_METHOD_NEITHER,               // IOCTL
        nullptr,                            // InputBuffer
        0,                                  // Size of InputBuffer
        nullptr,                            // OutputBuffer
        0,                                  // size of OutputBuffer
        &BytesReturned,                     // Bytes Returned to OutputBuffer
        nullptr);

    if (result)
        printf("\nIOCTL_METHOD_NEITHER was executed successfully!\n");
   
    getchar();

    CloseHandle(hDevice);
    
    return 0;
}
