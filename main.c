/*
 * Copyright (c) 2020 Hiroki Mori
 * original is GALBLAST.C
 */

#include <stdio.h>
#include <sys/stat.h>

char buffer[16348];

int main(int argc, char *argv[])
{

	if (argc < 3 || (strcmp(argv[1], "t") != 0 &&
	    strcmp(argv[1], "e") != 0 &&
	    strcmp(argv[1], "r") != 0 && strcmp(argv[1], "w") != 0)) {
		printf("usage: galprog [t,e,r,w] <chip type> <file name>\n");
		return 1;
	}

	if (setgaltype(argv[2])) {
		if(strcmp(argv[1], "t") ==0) {
			if(OpenFTDI()) {
				if (TestProperGAL()) {
					TestOP();
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
				fstat(fileno(jedec), &jedec_stat);
				int size;
				size = jedec_stat.st_size;
				fread(buffer, 1, size, jedec);
				buffer[size] = '\0';
				if(OpenFTDI()) {
					if (TestProperGAL()) {
						WriteOP();
						CloseFTDI();
					}
				}
			}
		}
	}
	else {
		printf("Unknown GAL\n");
	}

	return 0;
}
