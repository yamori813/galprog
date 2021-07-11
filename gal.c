/*
 * Copyright (c) 1998-2000 by Manfred Winterhoff
 * Copyright (c) 2020 Hiroki Mori
 * original is GALBlast.c
 */

#include <stdio.h>
#if defined(__APPLE__)
#include <strings.h>
#include <unistd.h>

#include "WinTypes.h"
#endif

#if defined(_WIN32)
#include <windows.h>
#define strcasecmp _stricmp
#endif

#include "gal.h"
#include "bitbang.h"
#include "jedec.h"

static int cfg16V8[]=
{
    2128,2129,2130,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142,2143,2144,2145,2146,2147,2148,2149,2150,2151,2152,2153,2154,2155,2156,2157,2158,2159,
    2048,2049,2050,2051,
    2193,
    2120,2121,2122,2123,2124,2125,2126,2127,
    2192,
    2052,2053,2054,2055,
    2160,2161,2162,2163,2164,2165,2166,2167,2168,2169,2170,2171,2172,2173,2174,2175,2176,2177,2178,2179,2180,2181,2182,2183,2184,2185,2186,2187,2188,2189,2190,2191
};
static int cfg16V8AB[]=
{
    2048,2049,2050,2051,
    2193,
    2120,2121,2122,2123,
    2128,2129,2130,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142,2143,2144,2145,2146,2147,2148,2149,2150,2151,2152,2153,2154,2155,2156,2157,2158,2159,2160,2161,2162,2163,2164,2165,2166,2167,2168,2169,2170,2171,2172,2173,2174,2175,2176,2177,2178,2179,2180,2181,2182,2183,2184,2185,2186,2187,2188,2189,2190,2191,
    2124,2125,2126,2127,
    2192,
    2052,2053,2054,2055
};
static int cfg18V10[]=
{
    3457,3456,
    3459,3458,
    3461,3460,
    3463,3462,
    3465,2464,
    3467,3466,
    3469,3468,
    3471,3470,
    3473,3472,
    3475,3474
};
static int cfg20V8[]=
{
    2640,2641,2642,2643,2644,2645,2646,2647,2648,2649,2650,2651,2652,2653,2654,2655,2656,2657,2658,2659,2660,2661,2662,2663,2664,2665,2666,2667,2668,2669,2670,2671,
    2560,2561,2562,2563,
    2705,
    2632,2633,2634,2635,2636,2637,2638,2639,
    2704,
    2564,2565,2566,2567,
    2672,2673,2674,2675,2676,2677,2678,2679,2680,2681,2682,2683,2684,2685,2686,2687,2688,2689,2690,2691,2692,2693,2694,2695,2696,2697,2698,2699,2700,2701,2702,2703
};
static int cfg20V8AB[]=
{
    2560,2561,2562,2563,
    2705,
    2632,2633,2634,2635,
    2640,2641,2642,2643,2644,2645,2646,2647,2648,2649,2650,2651,2652,2653,2654,2655,2656,2657,2658,2659,2660,2661,2662,2663,2664,2665,2666,2667,2668,2669,2670,2671,2672,2673,2674,2675,2676,2677,2678,2679,2680,2681,2682,2683,2684,2685,2686,2687,2688,2689,2690,2691,2692,2693,2694,2695,2696,2697,2698,2699,2700,2701,2702,2703,
    2636,2637,2638,2639,
    2704,
    2564,2565,2566,2567
};
static int cfg20RA10[]=
{
    3200,3201,3202,3203,3204,3205,3206,3207,3208,3209
};
static int cfg20XV10[]=
{
    1630,
    1628,1629,
    1620,1621,1622,
    1610,1611,1612,1613,1614,
    1600,1601,1602,1603,1604,
    1627,1626,
    1623,1624,1625,
    1619,1618,1617,1616,1615,
    1609,1608,1607,1606,1605
};
static int cfg22V10[]=
{
    5809,5808,
    5811,5810,
    5813,5812,
    5815,5814,
    5817,5816,
    5819,5818,
    5821,5820,
    5823,5822,
    5825,5824,
    5827,5826
};
static int cfg26CV12[]=
{
    6345,6344,
    6347,6346,
    6349,6348,
    6351,6350,
    6353,6352,
    6355,6354,
    6357,6356,
    6359,6358,
    6361,6360,
    6363,6362,
    6365,6364,
    6367,6366
};
static int cfg6001[]=
{
    8221,8220,
    8179,8183,8187,8191,8195,8199,8203,8207,8211,8215,
    8214,8210,8206,8202,8198,8194,8190,8186,8182,8178,
    8216,8217,8212,8213,8208,8209,8204,8205,8200,8201,
      8196,8197,8192,8193,8188,8189,8184,8185,8180,8181,
    8156,8159,8162,8165,8168,8171,8174,8177,
    8154,8157,8160,8163,8166,8169,8172,8175,
    8176,8173,8170,8167,8164,8161,8158,8155,
    8218,8219
};
static int cfg6002[]=
{
    8257,8256,

    8179,8183,8187,8191,8195,8199,8203,8207,8211,8215,
    8214,8210,8206,8202,8198,8194,8190,8186,8182,8178,
    8216,8217,8212,8213,8208,8209,8204,8205,8200,8201,
      8196,8197,8192,8193,8188,8189,8184,8185,8180,8181,
    8255,8254,8253,8252,8251,8250,8249,8248,8247,
      8246,8245,8244,8243,8242,8241,8240,8239,8238,
    8220,8221,8222,8223,8224,8225,8226,8227,8228,
      8229,8230,8231,8232,8233,8234,8235,8236,8237,
    8156,8159,8162,8165,8168,8171,8174,8177,
    8154,8157,8160,8163,8166,8169,8172,8175,
    8176,8173,8170,8167,8164,8161,8158,8155,
    8218,8219
};

struct _galinfo galinfo[]=
{
    {UNKNOWN,  0x00,0x00,"unknown",    0, 0, 0,  0, 0,   0,0, 0, 0, 0, 8, 0,NULL,0},
    {GAL16V8,  0x00,0x1A,"GAL16V8",  2194,20,32, 64,32,2056,8,63,54,58, 8,60,cfg16V8AB,sizeof(cfg16V8AB)/sizeof(int)},
    {GAL18V10, 0x50,0x51,"GAL18V10", 3540,20,36, 96,36,3476,8,61,60,58,10,16,cfg18V10,sizeof(cfg18V10)/sizeof(int)},
    {GAL20V8,  0x20,0x3A,"GAL20V8",  2706,24,40, 64,40,2568,8,63,59,58, 8,60,cfg20V8AB,sizeof(cfg20V8AB)/sizeof(int)},
    {GAL20RA10,0x60,0x61,"GAL20RA10",3274,24,40, 80,40,3210,8,61,60,58,10,16,cfg20RA10,sizeof(cfg20RA10)/sizeof(int)},
    {GAL20XV10,0x65,0x66,"GAL20XV10",1671,24,40, 40,44,1631,5,61,60,58, 5,16 /* 4,5,6,29,36,37 macht was,38,44,46,52 CLEARALL,53 macht was,60 CLEARALL */,cfg20XV10,sizeof(cfg20XV10)/sizeof(int)},
    {GAL22V10, 0x48,0x49,"GAL22V10", 5892,24,44,132,44,5828,8,61,60,58,10,16,cfg22V10,sizeof(cfg22V10)/sizeof(int)},
    {GAL26CV12,0x58,0x59,"GAL26CV12",6432,28,52,122,52,6368,8,61,60,58,12,16,cfg26CV12,sizeof(cfg26CV12)/sizeof(int)},
    {GAL6001,  0x40,0x41,"GAL6001",  8294,24,78, 75,97,8222,9,63,62,96, 8, 8,cfg6001,sizeof(cfg6001)/sizeof(int)},
    {GAL6002,  0x44,0x44,"GAL6002",  8330,24,78, 75,97,8258,9,63,62,96, 8, 8,cfg6002,sizeof(cfg6002)/sizeof(int)},
};

extern char buffer[16348];

GALTYPE gal;
unsigned char pes[12];
int security=0,erase=0,pulse=0,vpp=0;
char fusemap[10000];

static int duration[16]={1,2,5,10,20,30,40,50,60,70,80,90,100,200,0,0};

static void Delay(int msec)
{

    usleep(1000 * msec);
}

static void SetPV(BOOL on)
{
    SetSEL(on);
}

static void SetSTB(BOOL on)
{
    SetSTROBE(on);
}

/* set/reset individual pins of GAL */
static void SetVCC(BOOL on)
{
    SetINIT(!on);
}

static void SetVPP(int volt)
{
    if(volt)
        SetFEED(1);
    else
        SetFEED(0);
}

static BOOL GetSDOUT(void)
{
    return GetACK();
}

static BOOL ReceiveBit(void)
{
    BOOL bit;

    bit=GetSDOUT();
    SetSCLK(1);
    SetSCLK(0);
    return bit;
}

static void DiscardBits(int n)
{
    while(n-->0) ReceiveBit();
}

static void SendBit(int bit)
{
    SetSDIN(bit);
    SetSCLK(1);
    SetSCLK(0);
}

static void SendBits(int n,int bit)
{
    while(n-->0) SendBit(bit);
}

static void SendAddress(int n,int row)
{
    while(n-->0)
    {
	SendBit(row&1);
	row>>=1;
    }
}

static void Strobe(int msec)
{
    Delay(2);
    SetSTB(0);
    Delay(msec);
    SetSTB(1);
    Delay(2);
}

static void StrobeRow(int row)
{
    switch(gal)
    {
    case GAL16V8:
    case GAL20V8:
	SetRow(row);
	break;
    case GAL6001:
    case GAL6002:
	SetRow(0);
	SendBits(95,0);
	SendBit(1);
	SendAddress(7,row);
	SendBits(16,0);
	break;
    default:
	SetRow(0);
	SendBits(galinfo[gal].bits,0);
	SendAddress(6,row);
    }
    Strobe(2);
}

static void TurnOff(void)
{
    Delay(100);
    SetRow(0x3F);
    SetSDIN(1);
    // SetSCLK(1);
    // SetPV(1);
    SetVPP(0); // turn off VPP
    Delay(2);
    SetVCC(0); // turn off VCC and drivers
}

static BOOL TurnOn(int mode)
{
    BOOL writeorerase;

    if(mode==WRITEGAL||mode==ERASEGAL||mode==ERASEALL||mode==BURNSECURITY||mode==WRITEPES||mode==VPPTEST)
    {
        writeorerase=TRUE;
    }
    else
    {
        writeorerase=FALSE;
    }
    SetVPP(0); // assure VPP is 0
    SetSTB(1); // set all (disabled) outputs to H (high impedance)
    SetSCLK(1);
    SetSDIN(1);
    SetPV(1);
    SetRow(0x3F);
    SetVCC(1); // turn on VCC and drivers
    Delay(100);
    SetVPP(writeorerase?vpp:READVPP); // turn on VPP (readout at 12V)
    SetSCLK(0); // now that all pins are in programming mode
    SetPV(0); // bring critical pins down to L
    Delay(10);
    if(writeorerase)
    {
	SetPV(1);
	Delay(10);
    }
    return TRUE;
}

static void GetPES(unsigned char pes[])
{
    int bitmask,byte;

    StrobeRow(galinfo[gal].pesrow);
    if(gal==GAL6001||gal==GAL6002) DiscardBits(20);
    for(byte=0;byte<galinfo[gal].pesbytes;byte++)
    {
	pes[byte]=0;
	for(bitmask=0x1;bitmask<=0x80;bitmask<<=1)
	{
	    if(ReceiveBit()) pes[byte]|=bitmask;
	}
    }
}

static void ReadPES(unsigned char pes[])
{
    TurnOn(READPES);
    GetPES(pes);
    TurnOff();
}

static void WritePES(char pes[])
{
    int byte,bitmask;

    if(TurnOn(WRITEPES))
    {
        switch(gal)
        {
        case GAL16V8:
        case GAL20V8:
            SetRow(galinfo[gal].pesrow);
            for(byte=0;byte<8;byte++) for(bitmask=0x01;bitmask<=0x80;bitmask<<=1) SendBit((pes[byte]&bitmask)!=0);
            break;
        case GAL6001:
        case GAL6002:
            SetRow(0);
            SendBits(20,0);
            for(byte=0;byte<galinfo[gal].pesbytes;byte++) for(bitmask=0x01;bitmask<=0x80;bitmask<<=1) SendBit((pes[byte]&bitmask)!=0);
            if(galinfo[gal].pesbytes*8<galinfo[gal].bits) SendBits(galinfo[gal].bits-galinfo[gal].pesbytes*8,0);
            SendBit(1);
            SendAddress(7,galinfo[gal].pesrow);
            SendBits(16,0);
            SetSDIN(0);
            break;
        default:
            SetRow(0);
            for(byte=0;byte<galinfo[gal].pesbytes;byte++) for(bitmask=0x01;bitmask<=0x80;bitmask<<=1) SendBit((pes[byte]&bitmask)!=0);
            if(galinfo[gal].pesbytes*8<galinfo[gal].bits) SendBits(galinfo[gal].bits-galinfo[gal].pesbytes*8,0);
            SendAddress(6,galinfo[gal].pesrow);
            SetSDIN(0);
        }
        Strobe(pulse);
	TurnOff();
    }
}

static void WriteGAL(char *fuses)
{
    int row,bit;

    if(TurnOn(WRITEGAL))
    {
        switch(gal)
        {
        case GAL16V8:
        case GAL20V8:
            for(row=0;row<galinfo[gal].rows;row++)
            {
                SetRow(row);
                for(bit=0;bit<galinfo[gal].bits;bit++)
                {
                    SendBit(fuses[galinfo[gal].rows*bit+row]);
                }
                Strobe(pulse);
            }
            // UES
            SetRow(galinfo[gal].uesrow);
            for(bit=0;bit<64;bit++)
            {
                SendBit(fuses[galinfo[gal].uesfuse+bit]);
            }
            Strobe(pulse);
            // CFG
            SetRow(galinfo[gal].cfgrow);
            for(bit=0;bit<82;bit++)
            {
                switch(pes[2])
                {
                case 0x00:
                    SendBit(fuses[cfg16V8[bit]]);
                    break;
                case 0x1A:
                    SendBit(fuses[cfg16V8AB[bit]]);
                    break;
                case 0x20:
                    SendBit(fuses[cfg20V8[bit]]);
                    break;
                case 0x3A:
                    SendBit(fuses[cfg20V8AB[bit]]);
                    break;
                }
            }
            Strobe(pulse);
            break;
        case GAL6001:
        case GAL6002:
            SetRow(0);
            for(row=0;row<78;row++)
            {
                SendBits(20,0);
                for(bit=0;bit<11;bit++) SendBit(fuses[7296+78*bit+row]);
                for(bit=0;bit<64;bit++) SendBit(fuses[114*bit+row]);
                SendBit(1);
                SendAddress(7,row);
                SendBits(16,0);
                SetSDIN(0);
                Strobe(pulse);
            }
            for(row=0;row<64;row++)
            {
                for(bit=0;bit<20;bit++) SendBit(fuses[78+114*row+bit]);
                SendBits(11,0);
                for(bit=0;bit<64;bit++) SendBit(bit!=row);
                SendBits(8,0);
                for(bit=0;bit<16;bit++) SendBit(fuses[98+114*row+bit]);
                SetSDIN(0);
                Strobe(pulse);
            }
            // UES
            SendBits(20,0);
            for(bit=0;bit<72;bit++) SendBit(fuses[galinfo[gal].uesfuse+bit]);
            SendBits(3,0);
            SendBit(1);
            SendAddress(7,galinfo[gal].uesrow);
            SendBits(16,0);
            SetSDIN(0);
            Strobe(pulse);
            // CFG
            SetRow(galinfo[gal].cfgrow);
            for(bit=0;bit<galinfo[gal].cfgbits;bit++)
            {
                SendBit(fuses[galinfo[gal].cfg[bit]]);
            }
            SetSDIN(0);
            Strobe(pulse);
            break;
        default:
            SetRow(0);
            for(row=0;row<galinfo[gal].rows;row++)
            {
                for(bit=0;bit<galinfo[gal].bits;bit++)
                {
                    SendBit(fuses[galinfo[gal].rows*bit+row]);
                }
                SendAddress(6,row);
                SetSDIN(0);
                Strobe(pulse);
            }
            // UES
            for(bit=0;bit<galinfo[gal].uesbytes*8;bit++)
            {
                SendBit(fuses[galinfo[gal].uesfuse+bit]);
            }
            if(galinfo[gal].uesbytes*8<galinfo[gal].bits)
            {
                SendBits(galinfo[gal].bits-galinfo[gal].uesbytes*8,0);
            }
            SendAddress(6,galinfo[gal].uesrow);
            SetSDIN(0);
            Strobe(pulse);
            // CFG
            SetRow(galinfo[gal].cfgrow);
            for(bit=0;bit<galinfo[gal].cfgbits;bit++)
            {
                SendBit(fuses[galinfo[gal].cfg[bit]]);
            }
            SetSDIN(0);
            Strobe(pulse);
        }
	TurnOff();
    }
}

static void ReadGAL(char *fuses)
{
    int row,bit;

    TurnOn(READGAL);
    switch(gal)
    {
    case GAL16V8:
    case GAL20V8:
	for(row=0;row<galinfo[gal].rows;row++)
	{
	    StrobeRow(row);
	    for(bit=0;bit<galinfo[gal].bits;bit++)
	    {
		fuses[galinfo[gal].rows*bit+row]=ReceiveBit();
	    }
	}
	// UES
	StrobeRow(galinfo[gal].uesrow);
	for(bit=0;bit<64;bit++)
	{
	    fuses[galinfo[gal].uesfuse+bit]=ReceiveBit();
	}
	// CFG
	StrobeRow(galinfo[gal].cfgrow);
	for(bit=0;bit<82;bit++)
	{
	    switch(pes[2])
	    {
	    case 0x00:
		fuses[cfg16V8[bit]]=ReceiveBit();
		break;
	    case 0x1A:
		fuses[cfg16V8AB[bit]]=ReceiveBit();
		break;
	    case 0x20:
		fuses[cfg20V8[bit]]=ReceiveBit();
		break;
	    case 0x3A:
		fuses[cfg20V8AB[bit]]=ReceiveBit();
		break;
	    }
	}
	break;
    case GAL6001:
    case GAL6002:
	for(row=0;row<78;row++)
	{
	    StrobeRow(row);
	    DiscardBits(20);
	    for(bit=0;bit<11;bit++) fuses[7296+78*bit+row]=ReceiveBit();
	    for(bit=0;bit<64;bit++) fuses[114*bit+row]=ReceiveBit();
	    DiscardBits(24);
	}
	for(row=0;row<64;row++)
	{
	    SendBits(31,0);
	    for(bit=0;bit<64;bit++) SendBit(bit!=row);
	    SendBits(24,0);
            SetSDIN(0);
	    Strobe(2);
	    for(bit=0;bit<20;bit++) fuses[78+114*row+bit]=ReceiveBit();
	    DiscardBits(83);
	    for(bit=0;bit<16;bit++) fuses[98+114*row+bit]=ReceiveBit();
	}
	// UES
	StrobeRow(galinfo[gal].uesrow);
	DiscardBits(20);
	for(bit=0;bit<72;bit++) fuses[galinfo[gal].uesfuse+bit]=ReceiveBit();
	// CFG
	SetRow(galinfo[gal].cfgrow);
	Strobe(2);
	for(bit=0;bit<galinfo[gal].cfgbits;bit++) fuses[galinfo[gal].cfg[bit]]=ReceiveBit();
	break;
    default:
	for(row=0;row<galinfo[gal].rows;row++)
	{
	    StrobeRow(row);
	    for(bit=0;bit<galinfo[gal].bits;bit++)
	    {
		fuses[galinfo[gal].rows*bit+row]=ReceiveBit();
	    }
	}
	// UES
	StrobeRow(galinfo[gal].uesrow);
	for(bit=0;bit<galinfo[gal].uesbytes*8;bit++)
	{
	    fuses[galinfo[gal].uesfuse+bit]=ReceiveBit();
	}
	// CFG
	SetRow(galinfo[gal].cfgrow);
	Strobe(2);
	for(bit=0;bit<galinfo[gal].cfgbits;bit++)
	{
	    fuses[galinfo[gal].cfg[bit]]=ReceiveBit();
	}
    }
    TurnOff();
}

static void EraseGAL(int gal)
{
    if(TurnOn(ERASEGAL))
    {
        SetRow(galinfo[gal].eraserow);
	if(gal==GAL16V8||gal==GAL20V8) SendBit(1);
        Strobe(erase);
	TurnOff();
    }
}

static void EraseWholeGAL(int gal)
{
    int i;

    if(TurnOn(ERASEALL))
    {
        SetRow(galinfo[gal].eraseallrow);
	if(gal==GAL16V8||gal==GAL20V8) SendBit(1);
        Strobe(erase);
	TurnOff();
    }
}

static void BurnSecurity(int gal)
{
    if(TurnOn(BURNSECURITY))
    {
        switch(gal)
        {
        case GAL16V8:
        case GAL20V8:
            SetRow(61);
            SendBit(1);
            break;
        case GAL6001:
        case GAL6002:
            SetRow(0);
            SendBits(20+11+64,0);
            SendBit(1);
            SendAddress(7,98);
            SendBits(16,0);
            SetSDIN(0);
            break;
        default:
            SetRow(0);
            switch(gal)
            {
            case GAL18V10:
                SendBits(96,0);
                break;
            case GAL20RA10:
                SendBits(80,0);
                break;
            case GAL20XV10:
                SendBits(40,0);
                break;
            case GAL22V10:
                SendBits(132,0);
                break;
            case GAL26CV12:
                SendBits(122,0);
                break;
            }
            SendAddress(6,61);
            SetSDIN(0);
        }
        Strobe(pulse);
	TurnOff();
    }
}

static void ParsePES(void)
{
    int algo=pes[1]&0x0F;

    if(algo==5)
    {
	erase=(25<<((pes[4]>>2)&7))/2;
	pulse=duration[((((unsigned)pes[5]<<8)|pes[4])>>5)&15];
	vpp=2*((pes[5]>>1)&31)+20;
//        vread=2*(((((unsigned)pes[6]<<8)|pes[5])>>6)&31)+20;
    }
    else switch(gal)
    {
    case GAL16V8:
    case GAL20V8:
	erase=100;
	goto more;
    case GAL6001:
    case GAL6002:
	erase=50;
    more:
	switch(algo)
	{
	case 0:
	    vpp=63; // 15.75V
	    pulse=100;
	    break;
	case 1:
	    vpp=63; // 15.75V
	    pulse=80;
	    break;
	case 2:
	    vpp=66; // 16.5V
	    pulse=10;
	    break;
	case 3:
	    vpp=pes[3]==NATIONAL?60:58; // 15/14.5V
	    pulse=40;
	    break;
	case 4:
	    vpp=56; // 14V
	    pulse=100;
	    break;
	}
	break;
    default:
	erase=pes[3]==NATIONAL?50:100;
	switch(algo)
	{
	case 0:
	    vpp=66; // 16.5V
	    pulse=10;
	    break;
	case 1:
	    vpp=63; // 15.75V
	    pulse=100;
	    break;
	case 2:
	    vpp=pes[3]==NATIONAL?60:58; // 15/14.5V
	    pulse=40;
	    break;
	case 3:
	    vpp=56; // 14V
	    pulse=100;
	    break;
	}
	break;
    }
}

BOOL TestProperGAL()
{
    int type;

    ReadPES(pes);
    for(type=sizeof(galinfo)/sizeof(galinfo[0]);type;type--)
    {
	if(pes[2]==galinfo[type].id0||pes[2]==galinfo[type].id1) break;
    }
    if(type==0)
    {
//	pesread=0;
//	if(Message(wnd,"Unknown or illegal PES read, continue ?",NULL,MB_YESNO|MB_ICONQUESTION)==IDNO) return FALSE;
        printf("Unknown or illegal PES read\n");
        return FALSE;
    }
    else
    {
//	pesread=type;
	if(type!=gal)
	{
/*
	    switch(Message(wnd,"PES indicates a different GAL type than selected. Change GAL type ?",NULL,MB_YESNOCANCEL|MB_ICONQUESTION)==IDCANCEL)
	    {
	    case IDCANCEL:
		return FALSE;
	    case IDYES:
		gal=type;
	    }
*/
	    printf("PES indicates a different GAL type than selected\n");
            return FALSE;
	}
    }
    ParsePES();
    return TRUE;
}

int infocount()
{

    return sizeof(galinfo)/sizeof(galinfo[0]);
}

BOOL SetGALType(char *type)
{
int i;

    gal = UNKNOWN;
    for(i = 1; i < infocount(); ++i) {
        if (strcasecmp(galinfo[i].name, type) == 0) {
            gal = galinfo[i].type; 
            break;
        }
    }
    if (gal == UNKNOWN)
        return FALSE;

    return TRUE;
}

BOOL TestOP()
{
int i;

    for (i = 0; i < galinfo[gal].pesbytes; ++i)
        printf("%02x ",pes[i]);
    printf("\n");
    if(pes[1]&0x10)
    {
        strcpy(buffer,"3.3V");
    }
    else
    {
        strcpy(buffer,"5V");
     }
     switch(pes[3])
     {
         case LATTICE:
             strcat(buffer," Lattice ");
             break;
         case NATIONAL:
             strcat(buffer," National ");
             break;
         case SGSTHOMSON:
           strcat(buffer," ST Microsystems ");
           break;
         default:
         strcpy(buffer," Unknown ");
    }
    strcat(buffer,galinfo[gal].name);
    i=strlen(buffer);
    sprintf(buffer+i," VPP=%d.%02dV Pulse=%dms Erase=%dms",vpp/4,(vpp%4)*25,pulse,erase);
    return TRUE;
}

BOOL EraceOP()
{
    EraseGAL(gal);
    return TRUE;
}

BOOL ReadOP()
{
    ReadGAL(fusemap);
    FormatJEDEC(gal,fusemap,buffer);
    return TRUE;
}

BOOL WriteOP()
{
    if(CheckJEDEC()) {
        WriteGAL(fusemap);
        if(security) {
//            BurnSecurity(gal);
        }
        return TRUE;
    }
    return FALSE;
}
