/*
Project: BIEN 4280 Final Project
Authors: Bob Dirmish, Nicohlas Ragonese, Sam Keller, Martin Boehm, Jacob Prado
Description:Implement a serial communication between host PC and mbed device (Arduino nano 33)
            to send the data received to Azure to use their speech-to-text services to use 
            different sensors on the Arduino
*/

#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#define READ_TIMEOUT    500  //ms
#define BUFFERSIZE      100
DWORD g_BytesTransferred = 0;

//void DisplayError(LPTSTR lpszFunction);

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped
);

VOID CALLBACK FileIOCompletionRoutine(
    __in  DWORD dwErrorCode,
    __in  DWORD dwNumberOfBytesTransfered,
    __in  LPOVERLAPPED lpOverlapped)
{
    _tprintf(TEXT("Error code:\t%x\n"), dwErrorCode);
    _tprintf(TEXT("Number of bytes:\t%x\n"), dwNumberOfBytesTransfered);
    g_BytesTransferred = dwNumberOfBytesTransfered;
}

HANDLE hComm;
DWORD dwBytesRead = 0;
LPDWORD lpNBytesRead = 0;
BOOL fWaitingOnRead = FALSE;
OVERLAPPED ol = { 0 };
LPOVERLAPPED LPol = { 0 };
char ReadBuffer[BUFFERSIZE] = "Hello World!";

int main()
{
    

    /*OVERLAPPED o;
    BOOL fSuccess; 
    DWORD dwEvtMask;*/
    
    hComm = CreateFile(
        L"COM6",
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        NULL,
        0);

    if (hComm == INVALID_HANDLE_VALUE) {
        printf("CreateFile failed with error %d.\n", GetLastError());
        return GetLastError();
    }
    else {
        printf("CreateFile initialized successfully\n");
    }
    while (1) {

        if (FALSE == ReadFile(hComm, ReadBuffer, BUFFERSIZE - 1, &dwBytesRead, NULL))
        {
            //DisplayError(TEXT("ReadFile"));
            printf("Terminal failure: Unable to read from file.\n GetLastError=%08x\n", GetLastError());
            CloseHandle(hComm);
            return -1;
        }

        SleepEx(5000, TRUE);

        printf("Value in ReadBuffer: %s.\n", ReadBuffer);
    }
    
    /*dwBytesRead = g_BytesTransferred;
    // This is the section of code that assumes the file is ANSI text. 
    // Modify this block for other data types if needed.

    if (dwBytesRead > 0 && dwBytesRead <= BUFFERSIZE - 1)
    {
        ReadBuffer[dwBytesRead] = '\0'; // NULL character

        _tprintf(TEXT("Data read from COM6 (%d bytes): \n"), dwBytesRead);
        printf("%s\n", ReadBuffer);
    }
    else if (dwBytesRead == 0)
    {
        _tprintf(TEXT("No data read from file COM6\n"));
    }
    else
    {
        printf("\n ** Unexpected value for dwBytesRead ** \n");
    }*/

    /*
    //mask event
    fSuccess = SetCommMask(hComm, EV_CTS | EV_DSR);

    if (!fSuccess) {
        printf("SetCommMask failed with error %d.\n", GetLastError());
        return GetLastError();
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

    if (WaitCommEvent(hComm, &dwEvtMask, &o))
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
    */
    CloseHandle(hComm); //closes the COM port

    return 0;
}
