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
        sizeof(InputBuffer),                   // Size of InputBuffer
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

    PVOID Buffer = VirtualAlloc(nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    ULONG Size = 0x1000;
    RtlCopyMemory(Buffer, &Size, sizeof(ULONG));
    result = DeviceIoControl(hDevice,      // Handle to Device Object
        IOCTL_METHOD_NEITHER,              // IOCTL
        Buffer,                            // InputBuffer
        sizeof(Buffer),                    // Size of InputBuffer
        Buffer,                            // OutputBuffer
        sizeof(Buffer),                    // size of OutputBuffer
        &BytesReturned,                    // Bytes Returned to OutputBuffer
        nullptr);

    if (result)
        printf("\n IOCTL_METHOD_NEITHER: Data Received from driver: %s\n", (char*)Buffer);
    else
        printf("\n IOCTL_METHOD_NEITHER Problem with executing IOCTL... (%u)\n", GetLastError());
   
    getchar();
    CloseHandle(hDevice);
    VirtualFree(Buffer, sizeof(Buffer), MEM_RELEASE);
    return 0;
}