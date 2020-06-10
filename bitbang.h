/*
 * Copyright (c) 1998-2000 by Manfred Winterhoff
 * Copyright (c) 2020 Hiroki Mori
 * original is GALBLAST.C
 */

BOOL GetACK(void);
void SetSTROBE(BOOL on);
void SetSEL(BOOL on);
void SetFEED(BOOL on);
void SetINIT(BOOL on);
void SetSDIN(BOOL on);
void SetRow(int row);
void SetSCLK(BOOL on);
int OpenFTDI();
void CloseFTDI();
