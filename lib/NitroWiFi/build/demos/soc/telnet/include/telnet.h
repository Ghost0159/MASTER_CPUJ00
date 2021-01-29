/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     telnet.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: telnet.h,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:23  adachi_hiroaki
  telnetデモの追加

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#ifndef TELNET_TELNET_H_
#define TELNET_TELNET_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/

/*---------------------------------------------------------------------------*
  定数定義
 *---------------------------------------------------------------------------*/
#define TELNET_OPTION_BINARY            0   /* 8-bit data path */
#define TELNET_OPTION_ECHO              1   /* echo */
#define TELNET_OPTION_RCP               2   /* prepare to reconnect */
#define TELNET_OPTION_SGA               3   /* suppress go ahead */
#define TELNET_OPTION_NAMS              4   /* approximate message size */
#define TELNET_OPTION_STATUS            5   /* give status */
#define TELNET_OPTION_TM                6   /* timing mark */
#define TELNET_OPTION_RCTE              7   /* remote controlled transmission and echo */
#define TELNET_OPTION_NAOL              8   /* negotiate about output line width */
#define TELNET_OPTION_NAOP              9   /* negotiate about output page size */
#define TELNET_OPTION_NAOCRD            10  /* negotiate about CR disposition */
#define TELNET_OPTION_NAOHTS            11  /* negotiate about horizontal tabstops */
#define TELNET_OPTION_NAOHTD            12  /* negotiate about horizontal tab disposition */
#define TELNET_OPTION_NAOFFD            13  /* negotiate about formfeed disposition */
#define TELNET_OPTION_NAOVTS            14  /* negotiate about vertical tab stops */
#define TELNET_OPTION_NAOVTD            15  /* negotiate about vertical tab disposition */
#define TELNET_OPTION_NAOLFD            16  /* negotiate about output LF disposition */
#define TELNET_OPTION_XASCII            17  /* extended ascic character set */
#define TELNET_OPTION_LOGOUT            18  /* force logout */
#define TELNET_OPTION_BM                19  /* byte macro */
#define TELNET_OPTION_DET               20  /* data entry terminal */
#define TELNET_OPTION_SUPDUP            21  /* supdup protocol */
#define TELNET_OPTION_SUPDUPOUTPUT      22  /* supdup output */
#define TELNET_OPTION_SNDLOC            23  /* send location */
#define TELNET_OPTION_TTYPE             24  /* terminal type */
#define TELNET_OPTION_EOR               25  /* end or record */
#define TELNET_OPTION_TUID              26  /* TACACS user identification */
#define TELNET_OPTION_OUTMRK            27  /* output marking */
#define TELNET_OPTION_TTYLOC            28  /* terminal location number */
#define TELNET_OPTION_3270REGIME        29  /* 3270 regime */
#define TELNET_OPTION_X3PAD             30  /* X.3 PAD */
#define TELNET_OPTION_NAWS              31  /* window size */
#define TELNET_OPTION_TSPEED            32  /* terminal speed */
#define TELNET_OPTION_LFLOW             33  /* remote flow control */
#define TELNET_OPTION_LINEMODE          34  /* Linemode option */
#define TELNET_OPTION_XDISPLOC          35  /* X Display Location */
#define TELNET_OPTION_OLD_ENVIRON       36  /* Old - Environment variables */
#define TELNET_OPTION_AUTHENTICATION    37  /* Authenticate */
#define TELNET_OPTION_ENCRYPT           38  /* Encryption option */
#define TELNET_OPTION_NEW_ENVIRON       39  /* New - Environment variables */
#define TELNET_OPTION_TN3270E           40  /* TN3270 enhancements */
#define TELNET_OPTION_XAUTH             41
#define TELNET_OPTION_CHARSET           42  /* Character set */
#define TELNET_OPTION_RSP               43  /* Remote serial port */
#define TELNET_OPTION_COM_PORT_OPTION   44  /* Com port control */
#define TELNET_OPTION_SLE               45  /* Suppress local echo */
#define TELNET_OPTION_STARTTLS          46  /* Start TLS */
#define TELNET_OPTION_KERMIT            47  /* Automatic Kermit file transfer */
#define TELNET_OPTION_SEND_URL          48
#define TELNET_OPTION_FORWARD_X         49
#define TELNET_OPTION_PRAGMA_LOGON      138
#define TELNET_OPTION_SSPI_LOGON        139
#define TELNET_OPTION_PRAGMA_HEARTBEAT  140
#define TELNET_OPTION_EXOPL             255 /* extended-options-list */

#define TELNET_OPTIONSTATE_INITIAL      0
#define TELNET_OPTIONSTATE_ACTIVE       1
#define TELNET_OPTIONSTATE_PASSIVE      2

typedef enum
{
    IAC = 255,          /* interpret as command: */
    DONT= 254,          /* you are not to use option */
    DO  = 253,          /* please, you use option */
    WONT= 252,          /* I won't use option */
    WILL= 251,          /* I will use option */
    SB  = 250,          /* interpret as subnegotiation */
    SE  = 240           /* end sub negotiation */
} TelnetCommands;

#define GA          249 /* you may reverse the line */
#define EL          248 /* erase the current line */
#define EC          247 /* erase the current character */
#define AYT         246 /* are you there */
#define AO          245 /* abort output--but let prog finish */
#define IP          244 /* interrupt process--permanently */
#define BREAK       243 /* break */
#define DM          242 /* data mark--for connect. cleaning */
#define NOP         241 /* nop */
#define EOR         239 /* end of record (transparent mode) */
#define ABORT       238 /* Abort process */
#define SUSP        237 /* Suspend process */
#define xEOF        236 /* End of file: EOF is already used... */
#define SB_DELTA    1024

typedef enum
{
    TOP_LEVEL,
    RECEIVED_IAC,
    RECEIVED_WILL,
    RECEIVED_WONT,
    RECEIVED_DO,
    RECEIVED_DONT,
    RECEIVED_SB,
    SUBNEGOT,
    SUBNEG_IAC,
    RECEIVED_CR
} TelnetState;

/*---------------------------------------------------------------------------*
  構造体定義
 *---------------------------------------------------------------------------*/
typedef struct _Telnet
{
    int         socket;
    u8          buffer[32];
    u8          sb_opt, sb_len;
    char        sb_buf[30];
    int         sb_size;
    int         in_synch;
    TelnetState state;
} Telnet;

typedef struct _TelnetOption
{
    u32     nOptionCode;
    BOOL    bEnable;
    const char*     pSubOption;
    int nSubOptionLength;
    int nState;
} TelnetOption;

/*---------------------------------------------------------------------------*
  関数プロトタイプ定義
 *---------------------------------------------------------------------------*/
BOOL    TelnetConnect(Telnet* pTelnet, const char* szHost, u16 nPort);
BOOL    TelnetMain(Telnet* pTelnet);
void    TelnetWrite(const Telnet* pTelnet, const void* pBuffer, int nLength);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif /* __cplusplus */
#endif /* TELNET_TELNET_H_ */
