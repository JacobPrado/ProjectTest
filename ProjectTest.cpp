/*
Project: BIEN 4280 Final Project
Authors: Bob Dirmish, Nicohlas Ragonese, Sam Keller, Martin Boehm, Jacob Prado
Description:Implement a serial communication between host PC and mbed device (Arduino nano 33)
            to send the data received to Azure to use their speech-to-text services to use 
            different sensors on the Arduino
*/

#include <Windows.h>
#include <tchar.h>
#include <assert.h>
#include <stdio.h>

int main()
{
    HANDLE hFile;
    OVERLAPPED o;
    BOOL fSuccess;
    DWORD dwEvtMask;   
    
    hFile = CreateFile(
        L"COM6",
        GENERIC_READ | GENERIC_WRITE,
        NULL,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("CreateFile failed with error %d.\n", GetLastError());
        return 1;
    }
    
    //mask event

    fSuccess = SetCommMask(hFile, EV_CTS | EV_DSR);

    if (!fSuccess) {
        printf("SetCommMask failed with error %d.\n", GetLastError());
        return 1;
    }
    //event object for use by WaitCommEvent
    o.hEvent = CreateEvent(
        NULL,
        TRUE,
        FALSE,
        NULL);

    //initialize OVERLAPPED structure to zero
    o.Internal = 0;
    o.InternalHigh = 0;
    o.Offset = 0;
    o.OffsetHigh = 0;

    assert(o.hEvent);

    if (WaitCommEvent(hFile, &dwEvtMask, &o))
    {
        if (dwEvtMask & EV_DSR)
        {
            // To do.
            printf("DSR\n");
        }

        if (dwEvtMask & EV_CTS)
        {
            // To do. 
            printf("CTS\n");
        }
    }
    else
    {
        DWORD dwRet = GetLastError();
        if (ERROR_IO_PENDING == dwRet)
        {
            printf("I/O is pending...\n");

            // To do.
        }
        else
            printf("Wait failed with error %d.\n", GetLastError());
    }

    CloseHandle(hFile); //closes the COM port

    return 0;
}
