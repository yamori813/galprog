/*
 * Copyright (c) 1998-2000 by Manfred Winterhoff
 * Copyright (c) 2020 Hiroki Mori
 * original is GALBLAST.C
 */

#define LATTICE 0xA1
#define NATIONAL 0x8F
#define SGSTHOMSON 0x20

static char *caption[]={"Read GAL","Verify GAL","Read PES","Test SCLK","Write GAL","Erase GAL","Erase ALL","Burn Security","Write PES","VPP Test"};
#define READGAL 0
#define VERIFYGAL 1
#define READPES 2
#define SCLKTEST 3
#define WRITEGAL 4
#define ERASEGAL 5
#define ERASEALL 6
#define BURNSECURITY 7
#define WRITEPES 8
#define VPPTEST 9

#define READVPP 48 // 12V

typedef enum { UNKNOWN,GAL16V8,GAL18V10,GAL20V8,GAL20RA10,GAL20XV10,GAL22V10,GAL26CV12,GAL6001,GAL6002} GALTYPE;

struct _galinfo
{
    GALTYPE type;
    unsigned char id0,id1;
    char *name;
    int fuses;
    int pins;
    int rows;
    int bits;
    int uesrow;
    int uesfuse;
    int uesbytes;
    int eraserow;
    int eraseallrow;
    int pesrow;
    int pesbytes;
    int cfgrow;
    int *cfg;
    int cfgbits;
};

