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


HANDLE hComm;
DWORD dwBytesRead = 0;
DWORD dwEventMask;
BOOL succ = FALSE;
char ReadBuffer[BUFFERSIZE] = { 0 };
char ReadData;
char SerialBuffer[64] = { 0 };
DCB dcbSerialParams = { 0 };
COMMTIMEOUTS timeouts = { 0 };
unsigned char loop = 0;

int main()
{    
    hComm = CreateFile(
        L"COM6",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
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

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    succ = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
    if (succ == FALSE)
    {
        printf("\nError to Get the Com state\n\n");
    }
    dcbSerialParams.BaudRate = CBR_9600;      //BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None

    succ = SetCommState(hComm, &dcbSerialParams);
    if (succ == FALSE)
    {
        printf("\nError to Setting DCB Structure\n\n");
       
    }
    //Setting Timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
    {
        printf("\nError to Setting Time outs");
    }
    while (1) {
        //Setting Receive Mask
        succ = SetCommMask(hComm, EV_RXCHAR);
        if (succ == FALSE)
        {
            printf("\nError to in Setting CommMask\n\n");
            return -1;
        }
        //Setting WaitComm() Event
        succ = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
        if (succ == FALSE)
        {
            printf("\nError! in Setting WaitCommEvent()\n\n");
            return -1;
        }

        do
        {
            succ = ReadFile(hComm, &ReadData, sizeof(ReadData), &dwBytesRead, NULL);
            SerialBuffer[loop] = ReadData;
            ++loop;
        } while (dwBytesRead > 0);
        --loop;

        int index = 0;
        for (index = 0; index < loop; ++index)
        {
            printf("%c", SerialBuffer[index]);
        }
        
    }
    
    
    CloseHandle(hComm); //closes the COM port

    return 0;
}
