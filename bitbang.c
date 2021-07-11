/*
 * Copyright (c) 1998-2000 by Manfred Winterhoff
 * Copyright (c) 2020 Hiroki Mori
 * original is GALBLAST.C
 */

#include <stdio.h>
#if defined(__APPLE__)
#include <unistd.h>
#endif

#include "ftd2xx.h"

FT_HANDLE datHandle = NULL;
FT_HANDLE ctlHandle = NULL;

unsigned char porta;
unsigned char portb;

DWORD dwBytesInQueue = 0;

BOOL GetACK(void)
{
unsigned char data;

	FT_GetBitMode(ctlHandle, &data);

	return (data & 0x01) != 0;
}

void SetSTROBE(BOOL on)
{
	if(on)
	portb |= 0x02;
	else
	portb &= ~0x02;
	FT_Write(ctlHandle, &portb, 1, &dwBytesInQueue);
}

void SetSEL(BOOL on)
{
	if(on)
	portb |= 0x04;
	else
	portb &= ~0x04;
	FT_Write(ctlHandle, &portb, 1, &dwBytesInQueue);
}

void SetFEED(BOOL on)
{
	if(on)
		portb |= 0x08;
	else
		portb &= ~0x08;
    	FT_Write(ctlHandle, &portb, 1, &dwBytesInQueue);
}

void SetINIT(BOOL on)
{

}

void SetSDIN(BOOL on)
{
	if(on)
		porta |= 0x01;
	else
		porta &= ~0x01;
    	FT_Write(datHandle, &porta, 1, &dwBytesInQueue);
}

void SetRow(int row)
{
	porta = (porta & 0x81) | (row << 1);
	FT_Write(datHandle, &porta, 1, &dwBytesInQueue);
}

void SetSCLK(BOOL on)
{
	if(on)
		porta |= 0x80;
	else
		porta &= ~0x80;
    	FT_Write(datHandle, &porta, 1, &dwBytesInQueue);
}

int OpenFTDI()
{
	int i, j;
	FT_STATUS	ftStatus;
	int iport;

	iport = 0;
	ftStatus = FT_Open(iport, &datHandle);
	if(ftStatus != FT_OK) {
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open(%d) failed = %d\n", iport, ftStatus);
		return 0;
	}
	iport = 1;
	ftStatus = FT_Open(iport, &ctlHandle);
	if(ftStatus != FT_OK) {
		/* 
			This can fail if the ftdi_sio driver is loaded
		 	use lsmod to check this and rmmod ftdi_sio to remove
			also rmmod usbserial
		 */
		printf("FT_Open(%d) failed = %d\n", iport, ftStatus);
		return 0;
	}

	ftStatus = FT_SetBitMode(datHandle, 0xff, 0x01);
	if(ftStatus != FT_OK) {
		printf("Failed to set Asynchronous Bit bang Mode");
	}

	ftStatus = FT_SetBitMode(ctlHandle, 0x0e, 0x01);
	if(ftStatus != FT_OK) {
		printf("Failed to set Asynchronous Bit bang Mode");
	}

	usleep(1000*200);
	ftStatus = FT_Purge(datHandle, FT_PURGE_RX);
	if(ftStatus != FT_OK) {
		printf("Failed to FT_Purge\n");	
	}
	ftStatus = FT_SetBaudRate(datHandle, 9600);
	if(ftStatus != FT_OK) {
		printf("Failed to FT_SetBaudRate\n");	
	}
	usleep(1000*200);
	ftStatus = FT_Purge(ctlHandle, FT_PURGE_RX);
	if(ftStatus != FT_OK) {
		printf("Failed to FT_Purge\n");	
	}
	ftStatus = FT_SetBaudRate(ctlHandle, 9600);
	if(ftStatus != FT_OK) {
		printf("Failed to FT_SetBaudRate\n");	
	}

	return 1;
}

void CloseFTDI()
{
	if(datHandle != NULL) {
		FT_Close(datHandle);
		datHandle = NULL;
	}
	if(ctlHandle != NULL) {
		FT_Close(ctlHandle);
		ctlHandle = NULL;
	}	
}
