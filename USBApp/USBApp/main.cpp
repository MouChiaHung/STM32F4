#include "pch.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

typedef char *PC;


std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}




LONG __cdecl
_tmain(
    LONG     Argc,
    LPTSTR * Argv
    )
/*++

Routine description:

    Sample program that communicates with a USB device using WinUSB

--*/
{
    DEVICE_DATA           deviceData;
    HRESULT               hr;
    USB_DEVICE_DESCRIPTOR deviceDesc;
    BOOL                  bResult;
    BOOL                  noDevice;
    ULONG                 lengthReceived;

    UNREFERENCED_PARAMETER(Argc);
    UNREFERENCED_PARAMETER(Argv);

    //
    // Find a device connected to the system that has WinUSB installed using our
    // INF
    //

    //mou add start
    PC pc = (PC)malloc(64);
    if (pc != NULL) strncpy_s(pc, 64, "Hello USBApp", strlen("Hello USBApp"));
    printf("%s\n", pc);
    free(pc);
    //mou add end

    hr = OpenDevice(&deviceData, &noDevice);

    if (FAILED(hr)) {

        if (noDevice) {

            wprintf(L"Device not connected or driver not installed\n");

        } else {

            wprintf(L"Failed looking for device, HRESULT 0x%x\n", hr);
        }

        return 0;
    }

    //
    // Get device descriptor
    //
    bResult = WinUsb_GetDescriptor(deviceData.WinusbHandle,
                                   USB_DEVICE_DESCRIPTOR_TYPE,
                                   0,
                                   0,
                                   (PBYTE) &deviceDesc,
                                   sizeof(deviceDesc),
                                   &lengthReceived);

    if (FALSE == bResult || lengthReceived != sizeof(deviceDesc)) {

        wprintf(L"Error among LastError %d or lengthReceived %d\n",
                FALSE == bResult ? GetLastError() : 0,
                lengthReceived);
        CloseDevice(&deviceData);
        return 0;
    }

    //
    // Print a few parts of the device descriptor
    //
    wprintf(L"Device found: VID_%04X&PID_%04X; bcdUsb %04X\n",
            deviceDesc.idVendor,
            deviceDesc.idProduct,
            deviceDesc.bcdUSB);

    //Query the Device for USB Descriptors
    //additional device information
    unsigned char spd = ' ';
    if (GetUSBDeviceSpeed(deviceData.WinusbHandle, &spd))
        printf("USB device speed:%d\n", spd);
    else
        printf("SendDatatoDefaultEndpoint failed:%s\n", GetLastErrorAsString().c_str());

    //interface and its endpoints
    PIPE_ID pipe_id; 
    if (QueryDeviceEndpoints(deviceData.WinusbHandle, &pipe_id))
        printf("PipeInId:%d, PipeOutId:%d\n", pipe_id.PipeInId, pipe_id.PipeOutId);
    else
        printf("SendDatatoDefaultEndpoint failed:%s\n", GetLastErrorAsString().c_str());
    
    
#if 0
    //Send Control Transfer to the Default Endpoint
    UCHAR req_buf = 'A';
    if (SendDatatoDefaultEndpoint(deviceData.WinusbHandle, 0x66, &req_buf, 1))
        printf("SendDatatoDefaultEndpoint OK\n");
    else
        printf("SendDatatoDefaultEndpoint failed:%s\n", GetLastErrorAsString().c_str());
#endif    

    //Issue I/O Requests
    ULONG len_written = 0;

    //reset pipe
    if (WinUsb_ResetPipe(deviceData.WinusbHandle, pipe_id.PipeInId)) {
        printf("WinUsb_ResetPipe for PipeIn OK\n");   
    }
    else {
        printf("WinUsb_ResetPipe for PipeIn failed:%s\n", GetLastErrorAsString().c_str());
    }
    if (WinUsb_ResetPipe(deviceData.WinusbHandle, pipe_id.PipeOutId)) {
        printf("WinUsb_ResetPipe for PipeOut OK\n");   
    }
    else {
        printf("WinUsb_ResetPipe for PipeOut failed:%s\n", GetLastErrorAsString().c_str());
    }
    //set pipe timeout
    ULONG timeout = 1000;
    if (WinUsb_SetPipePolicy(deviceData.WinusbHandle, pipe_id.PipeInId, PIPE_TRANSFER_TIMEOUT, sizeof(ULONG), &timeout)) {
        printf("WinUsb_SetPipePolicy for PipeIn OK\n");   
    }
    else {
        printf("WinUsb_SetPipePolicy for PipeIn failed:%s\n", GetLastErrorAsString().c_str());
    }
    if (WinUsb_SetPipePolicy(deviceData.WinusbHandle, pipe_id.PipeOutId, PIPE_TRANSFER_TIMEOUT, sizeof(ULONG), &timeout)) {
        printf("WinUsb_SetPipePolicy for PipeOut OK\n");   
    }
    else {
        printf("WinUsb_SetPipePolicy for PipeOut failed:%s\n", GetLastErrorAsString().c_str());
    }
    //write bulk
    if (WriteToBulkEndpoint(deviceData.WinusbHandle, &pipe_id.PipeOutId, &len_written))
        printf("WriteToBulkEndpoint OK:%d bytes\n", len_written);
    else
        printf("WriteToBulkEndpoint failed:%s\n", GetLastErrorAsString().c_str());

#if 1
    ULONG len_read = 0;
    int len_print = 0;
    int p = 0;
    UCHAR buf_read[128];
    if (ReadFromBulkEndpoint(deviceData.WinusbHandle, &pipe_id.PipeInId, buf_read, 128, &len_read)) {
        printf("ReadFromBulkEndpoint OK:%d bytes\n", len_read);   
        len_print = len_read;
        while(len_print--)
            printf("%c", buf_read[p++]);
        printf("\n");
    }
    else
        printf("ReadFromBulkEndpoint failed:%s\n", GetLastErrorAsString().c_str());
#elif 0
    if (WinUsb_ResetPipe(deviceData.WinusbHandle, pipe_id.PipeInId)) {
        printf("WinUsb_ResetPipe for PipeIn OK\n");   
    }
    else
        printf("WinUsb_ResetPipe for PipeIn failed:%s\n", GetLastErrorAsString().c_str());
    if (WinUsb_ResetPipe(deviceData.WinusbHandle, pipe_id.PipeOutId)) {
        printf("WinUsb_ResetPipe for PipeOut OK\n");   
    }
    else
        printf("WinUsb_ResetPipe for PipeOut failed:%s\n", GetLastErrorAsString().c_str());
#elif 0
    if (WinUsb_FlushPipe(deviceData.WinusbHandle, pipe_id.PipeInId)) {
        printf("WinUsb_FlushPipe for PipeIn OK\n");   
    }
    else
        printf("WinUsb_FlushPipe for PipeIn failed:%s\n", GetLastErrorAsString().c_str());
    if (WinUsb_FlushPipe(deviceData.WinusbHandle, pipe_id.PipeOutId)) {
        printf("WinUsb_FlushPipe for PipeOut OK\n");   
    }
    else
        printf("WinUsb_FlushPipe for PipeOut failed:%s\n", GetLastErrorAsString().c_str());
#endif

    CloseHandle(deviceData.DeviceHandle);
    WinUsb_Free(deviceData.WinusbHandle);
    
    Sleep(1000);
    system("pause");
    return 0;
}



