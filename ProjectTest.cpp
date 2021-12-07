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
#define BUFFERSIZE      200

//void DisplayError(LPTSTR lpszFunction);

HANDLE hComm;
DWORD dwBytesRead = 0;
BOOL succ = FALSE;
char ReadBuffer[BUFFERSIZE] = { 0 };

int main()
{    
    hComm = CreateFile(
        L"COM6",
        GENERIC_READ | GENERIC_WRITE,
        NULL,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hComm == INVALID_HANDLE_VALUE) {
        printf("CreateFile failed with error %d.\n", GetLastError());
        return GetLastError();
    }else{
        printf("CreateFile initialized successfully\n");
    }

   // while (1) {
    succ = ReadFile(hComm, ReadBuffer, BUFFERSIZE, &dwBytesRead, NULL);
        if (!succ && (GetLastError() != ERROR_IO_PENDING)){
            //DisplayError(TEXT("ReadFile"));
            printf("Terminal failure: Unable to read from file.\nGetLastError=%08x\n", GetLastError());
            CloseHandle(hComm);
            return -1;
        }
        
       
            printf("Value in ReadBuffer: %s.\n", ReadBuffer);
        
        
        
        //SleepEx(000, TRUE);
    //}
    
    
    CloseHandle(hComm); //closes the COM port

    return 0;
}
