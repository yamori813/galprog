/*
 * Copyright (c) 1998-2000 by Manfred Winterhoff
 * Copyright (c) 2020 Hiroki Mori
 * original is GALBLAST.C
 */

#include "WinTypes.h"
#include "gal.h"

extern struct _galinfo galinfo[];
extern unsigned char pes[12];
extern GALTYPE gal;
extern int security,erase,pulse,vpp;
extern char fusemap[10000];
extern char buffer[16348];

static unsigned short checksum;

static unsigned short CheckSum(int n)
{
    unsigned short c,e;
    long a;
    int i;

    c=e=0;
    a=0;
    for(i=0;i<n;i++)
    {
        e++;
        if(e==9)
	{
            e=1;
	    a+=c;
            c=0;
	}
        c>>=1;
	if(fusemap[i]) c+=0x80;
    }
    return (unsigned short)((c>>(8-e))+a);
}

void FormatJEDEC(int gal,char *fuses,char *buffer)
{
    int i,j,k,n;
    int unused,start;
    time_t now;
    BOOL unquote;
    unsigned char ch;

    time(&now);
    n=sprintf(buffer,"JEDEC file for %s created on %s",(LPSTR)galinfo[gal].name,(LPSTR)asctime(localtime(&now)))-1;
    n+=sprintf(buffer+n,"\r\n*QP%d*QF%d*QV0*F0*G0*X0*\r\n",galinfo[gal].pins,galinfo[gal].fuses);
    if(gal==GAL6001||gal==GAL6002)
    {
	for(i=k=0;i<64;i++)
	{
	    start=n;
	    unused=TRUE;
	    n+=sprintf(buffer+n,"L%04d ",k);
	    for(j=0;j<114;j++)
	    {
		if(fuses[k]) unused=FALSE;
		buffer[n++]='0'+fuses[k++];
	    }
	    n+=sprintf(buffer+n,"*\r\n");
	    if(unused) n=start;
	}
	for(i=0;i<11;i++)
	{
	    start=n;
	    unused=TRUE;
	    n+=sprintf(buffer+n,"L%04d ",k);
	    for(j=0;j<78;j++)
	    {
		if(fuses[k]) unused=FALSE;
		buffer[n++]='0'+fuses[k++];
	    }
	    n+=sprintf(buffer+n,"*\r\n");
	    if(unused) n=start;
	}
    }
    else for(i=k=0;i<galinfo[gal].bits;i++)
    {
	start=n;
	unused=TRUE;
	n+=sprintf(buffer+n,"L%04d ",k);
	for(j=0;j<galinfo[gal].rows;j++)
	{
	    if(fuses[k]) unused=FALSE;
	    buffer[n++]='0'+fuses[k++];
	}
	n+=sprintf(buffer+n,"*\r\n");
	if(unused) n=start;
    }
    if(k<galinfo[gal].uesfuse)
    {
	start=n;
	unused=TRUE;
	n+=sprintf(buffer+n,"L%04d ",k);
	while(k<galinfo[gal].uesfuse)
	{
	    if(fuses[k]) unused=FALSE;
	    buffer[n++]='0'+fuses[k++];
	}
	n+=sprintf(buffer+n,"*\r\n");
	if(unused) n=start;
    }
    start=n;
    unused=TRUE;
    n+=sprintf(buffer+n,"N UES");
    unquote=TRUE;
    for(j=0;j<galinfo[gal].uesbytes;j++)
    {
	ch=0;
	for(i=0;i<8;i++) if(fuses[k+8*j+i]) ch|=1<<i;
	if(isprint(ch))
	{
	    if(unquote)
	    {
		buffer[n++]=' ';
		buffer[n++]='"';
		unquote=FALSE;
	    }
	    buffer[n++]=ch;
	}
	else
	{
	    if(!unquote)
	    {
		buffer[n++]='"';
		unquote=TRUE;
	    }
	    n+=sprintf(buffer+n," %02X",ch);
	}
    }
    n+=sprintf(buffer+n,"\"*\r\nL%04d "+unquote,k);
    for(j=0;j<8*galinfo[gal].uesbytes;j++)
    {
	if(fuses[k]) unused=FALSE;
	buffer[n++]='0'+fuses[k++];
    }
    n+=sprintf(buffer+n,"*\r\n");
    if(unused) n=start;
    if(k<galinfo[gal].fuses)
    {
	start=n;
	unused=TRUE;
	n+=sprintf(buffer+n,"L%04d ",k);
	while(k<galinfo[gal].fuses)
	{
	    if(fuses[k]) unused=FALSE;
	    buffer[n++]='0'+fuses[k++];
	}
	n+=sprintf(buffer+n,"*\r\n");
	if(unused) n=start;
    }
    n+=sprintf(buffer+n,"N PES");
    for(i=0;i<galinfo[gal].pesbytes;i++)
    {
	n+=sprintf(buffer+n," %02X",pes[i]);
    }
    n+=sprintf(buffer+n,"*\r\nC%04X\r\n*",CheckSum(galinfo[gal].fuses));
    buffer[n]='\0';
}

static int ParseFuseMap(char *ptr)
{
    int i,n,type,checksumpos,address,pins,lastfuse,state; // 0=outside JEDEC, 1=skipping comment or unknown, 2=read command

    state=0;
    security=0;
    checksum=0;
    checksumpos=0;
    pins=0;
    lastfuse=0;
    for(n=0;ptr[n];n++)
    {
	if(ptr[n]=='*') state=2; // else ignored
	else switch(state)
	{
	case 2:
	    if(!isspace(ptr[n])) switch(ptr[n])
	    {
	    case 'L':
		address=0;
		state=3;
		break;
	    case 'F':
		state=5;
		break;
	    case 'G':
                state=13;
		break;
	    case 'Q':
		state=7;
		break;
	    case 'C':
		checksumpos=n;
		state=14;
                break;
	    default:
		state=1;
	    }
	    break;
	case 3:
	    if(!isdigit(ptr[n])) return n;
	    address=ptr[n]-'0';
	    state=4;
	    break;
	case 4:
	    if(isspace(ptr[n]))
	    {
		state=6;
	    }
	    else if(isdigit(ptr[n]))
	    {
		address=10*address+(ptr[n]-'0');
	    }
	    else
	    {
		return n;
	    }
	    break;
	case 5:
	    if(isspace(ptr[n])) break; // ignored
	    if(ptr[n]=='0'||ptr[n]=='1')
	    {
		memset(fusemap,ptr[n]-'0',sizeof(fusemap));
	    }
	    else
	    {
                return n;
	    }
	    state=1;
	    break;
	case 6:
	    if(isspace(ptr[n])) break; // ignored
	    if(ptr[n]=='0'||ptr[n]=='1')
	    {
		fusemap[address++]=ptr[n]-'0';
	    }
	    else
	    {
		return n;
	    }
	    break;
	case 7:
	    if(isspace(ptr[n])) break; // ignored
            if(ptr[n]=='P')
	    {
		pins=0;
                state=8;
            }
            else if(ptr[n]=='F')
	    {
		lastfuse=0;
		state=9;
	    }
	    else state=2;
            break;
	case 8:
	    if(isspace(ptr[n])) break; // ignored
	    if(!isdigit(ptr[n])) return n;
	    pins=ptr[n]-'0';
            state=10;
	    break;
	case 9:
	    if(isspace(ptr[n])) break; // ignored
            if(!isdigit(ptr[n])) return n;
	    lastfuse=ptr[n]-'0';
	    state=11;
            break;
        case 10:
            if(isdigit(ptr[n]))
	    {
                pins=10*pins+(ptr[n]-'0');
	    }
	    else if(isspace(ptr[n]))
	    {
               state=12;
	    }
	    else return n;
            break;
	case 11:
            if(isdigit(ptr[n]))
	    {
		lastfuse=10*lastfuse+(ptr[n]-'0');
	    }
            else if(isspace(ptr[n]))
	    {
	       state=12;
            }
            else return n;
            break;
	case 12:
            if(!isspace(ptr[n])) return n;
	    break;
	case 13:
	    if(isspace(ptr[n])) break; // ignored
	    if(ptr[n]=='0'||ptr[n]=='1')
	    {
		security=ptr[n]-'0';
	    }
	    else
	    {
		return n;
	    }
	    state=1;
	    break;
	case 14:
	    if(isspace(ptr[n])) break; // ignored
            if(isdigit(ptr[n]))
            {
		checksum=ptr[n]-'0';
	    }
            else if(toupper(ptr[n])>='A'&&toupper(ptr[n])<='F')
	    {
		checksum=toupper(ptr[n])-'A'+10;
	    }
            else return n;
	    state=15;
	    break;
	case 15:
	    if(isdigit(ptr[n]))
            {
		checksum=16*checksum+ptr[n]-'0';
	    }
	    else if(toupper(ptr[n])>='A'&&toupper(ptr[n])<='F')
            {
		checksum=16*checksum+toupper(ptr[n])-'A'+10;
	    }
            else if(isspace(ptr[n]))
            {
                state=2;
	    }
	    else return n;
	    break;
	}
    }
    if(lastfuse||pins)
    {
	if(checksum&&checksum!=CheckSum(lastfuse))
	{
/*
	    if(Message(wnd,"Checksum given %04X calculated %04X",NULL,MB_OKCANCEL,checksum,CheckSum(lastfuse))==IDCANCEL)
	    {
		return checksumpos;
	    }
*/
	}
	for(type=0,i=1;i<infocount();i++)
	{
	    if((lastfuse==0||galinfo[i].fuses==lastfuse||galinfo[i].uesfuse==lastfuse&&galinfo[i].uesfuse+8*galinfo[i].uesbytes==galinfo[i].fuses)
	    && (pins==0||galinfo[i].pins==pins||galinfo[i].pins==24&&pins==28))
	    {
		if(gal==0)
		{
		    type=i;
		    break;
		}
		else if(!type)
                {
		    type=i;
		}
	    }
	}
	if(gal!=type)
	{
	    gal=type;
//	    SendDlgItemMessage(wnd,101,CB_SETCURSEL,gal-1,0L);
//	    SendMessage(wnd,WM_COMMAND,101,MAKELONG(0,CBN_SELCHANGE));
	}
    }
    return n;
}

BOOL CheckJEDEC()
{
    int position;

    position=ParseFuseMap(buffer);
    if(position==strlen(buffer)) return TRUE;
    printf("Error in JEDEC\n");
//    Message(wnd,"Error in JEDEC",NULL,MB_OK|MB_ICONEXCLAMATION);
//    SendDlgItemMessage(wnd,102,EM_SETSEL,0,MAKELONG(position,position));
//    SendMessage(wnd,WM_NEXTDLGCTL,GetDlgItem(wnd,102),TRUE);
    return FALSE;
}

