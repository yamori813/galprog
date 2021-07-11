/*
 * Copyright (c) 2020 Hiroki Mori
 * original is GALBLAST.C
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#if defined(__APPLE__)
#include "WinTypes.h"
#endif

#if defined(_WIN32)
#pragma comment(lib, "ftd2xx.lib")
#include <windows.h>
#endif

#include "gal.h"
#include "bitbang.h"

char buffer[16348];

extern struct _galinfo galinfo[];

#if defined(_WIN32)
void usleep(__int64 usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}
#endif

int main(int argc, char *argv[])
{
int i;

	if (argc < 3 || (strcmp(argv[1], "t") != 0 &&
	    strcmp(argv[1], "e") != 0 &&
	    strcmp(argv[1], "r") != 0 && strcmp(argv[1], "w") != 0)) {
		printf("usage: galprog [t,e,r,w] <device> <file name>\n");
		printf("Support devices: ");
		for (i = 1; i < infocount(); ++i) {
			if (i != 1)
				printf(",");
			printf("%s", galinfo[i].name);
		}
		printf("\n");
		return 1;
	}

	if (SetGALType(argv[2])) {
		if(strcmp(argv[1], "t") ==0) {
			if(OpenFTDI()) {
				if (TestProperGAL()) {
					TestOP();
					printf("%s\n", buffer);
					CloseFTDI();
				}
			}
		}
		else if(strcmp(argv[1], "e") ==0) {
			if(OpenFTDI()) {
				if (TestProperGAL()) {
					EraceOP();
					CloseFTDI();
				}
                        }
		}
		else if(strcmp(argv[1], "r") ==0) {
			if(OpenFTDI()) {
				if (TestProperGAL()) {
					ReadOP();
					printf("%s\n", buffer);
					CloseFTDI();
				}
                        }
		}
		else if(strcmp(argv[1], "w") ==0) {
			FILE *jedec;
			if ((jedec = fopen(argv[3], "rb")) != NULL) {
				struct stat jedec_stat;
#if defined(_WIN32)
				fstat(_fileno(jedec), &jedec_stat);
#else
				fstat(fileno(jedec), &jedec_stat);
#endif
				int size;
				size = jedec_stat.st_size;
				fread(buffer, 1, size, jedec);
				fclose(jedec);
				buffer[size] = '\0';
				if(OpenFTDI()) {
					if (TestProperGAL()) {
						WriteOP();
						CloseFTDI();
					}
				}
			} else {
				printf("Can't open %s\n", argv[3]);
			}
		}
	}
	else {
		printf("Unknown GAL\n");
	}

	return 0;
}
