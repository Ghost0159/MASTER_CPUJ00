/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - telnet
  File:     telnet.c

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: telnet.c,v $
  Revision 1.2  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.1  2006/01/12 03:01:24  adachi_hiroaki
  telnet�f���̒ǉ�

  $NoKeywords$
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include "console.h"
#include "telnet.h"

static void TelnetRead(Telnet* pTelnet, void* pBuffer, int nLength);
static TelnetOption*    TelnetGetOptionSetting(Telnet* pTelnet, u8 option);
static void         TelnetSendIAC(Telnet* pTelnet, u8 verb, u8 option);
static void         TelnetSendSubOption(Telnet* pTelnet);
static void         TelnetSendOption(Telnet* pTelnet, u8 option);

static TelnetOption TelnetOptionList[] = {
    {TELNET_OPTION_TTYPE, TRUE, "\x00vt100", 6},
    {TELNET_OPTION_NAWS, TRUE, "\x00\x20\x00\x18", 4},
    {TELNET_OPTION_ECHO, FALSE, NULL}
};

/*---------------------------------------------------------------------------*
  Name        : TelnetConnect
  Description : Telnet�T�[�o�ɐڑ�����
  Arguments   : pTelnet          - �n���h��
                szHost           - �z�X�g��
                nPort            - �|�[�g�ԍ�
  Returns     : �ڑ������Ȃ�^
 *---------------------------------------------------------------------------*/
BOOL TelnetConnect(Telnet* pTelnet, const char* szHost, u16 nPort)
{
    SOCSockAddrIn   addr;
    {
        SOCHostEnt*     host_ent = NULL;
        host_ent = SOC_GetHostByName(szHost);
        if (host_ent)
        {
            MI_CpuCopy8(host_ent->addrList[0], &addr.addr, SOC_ALEN);
        }
        else
        {
            return FALSE;
        }
    }

    {
        int socket = SOC_Socket(SO_PF_INET, SO_SOCK_STREAM, 0);
        addr.len = sizeof(SOCSockAddrIn);
        addr.family = SOC_PF_INET;
        addr.port = SOC_HtoNs(nPort);

        if (SOC_Connect(socket, &addr) >= 0)
        {
            MI_CpuClear8(pTelnet, sizeof(Telnet));
            pTelnet->socket = socket;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name        : TelnetWrite
  Description : �ڑ��ς�Telnet�T�[�o�Ƀf�[�^���u���b�L���O���M����B
  Arguments   : pTelnet          - �n���h��
                buffer           - �f�[�^
                size             - �f�[�^�̃T�C�Y
  Returns     : �Ȃ��B
 *---------------------------------------------------------------------------*/
void TelnetWrite(const Telnet* pTelnet, const void* buffer, int size)
{
    int result;
    while (size > 0)
    {
        result = SOC_Write(pTelnet->socket, buffer, size);
        if (result >= 0)
        {
            (u8*)buffer += result;
            size -= result;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name        : TelnetMain
  Description : �ڑ��ς�Telnet�T�[�o���瑗���Ă����f�[�^����������
  Arguments   : pTelnet          - �n���h��
  Returns     : �ڑ����ꂽ�܂܂Ȃ�^
 *---------------------------------------------------------------------------*/
BOOL TelnetMain(Telnet* pTelnet)
{
    int result = SOC_Recv(pTelnet->socket, pTelnet->buffer, sizeof(pTelnet->buffer), SOC_MSG_DONTWAIT);
    if (result > 0)
    {
        TelnetRead(pTelnet, (char*)pTelnet->buffer, result);
    }
    else if (result == 0)
    {
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*
  Name        : TelnetSendIAC
  Description : �l�S�V�G�[�g�R�[�h�𑗐M����
  Arguments   : pTelnet          - �n���h��
                verb             - WILL/WONT/DONT/DO�̂ǂꂩ
                option           - �I�v�V�����ԍ�
  Returns     : �Ȃ��B
 *---------------------------------------------------------------------------*/
static void TelnetSendIAC(Telnet* pTelnet, u8 verb, u8 option)
{
    u8  replyseq[3];
    replyseq[0] = IAC;
    replyseq[1] = verb;
    replyseq[2] = option;
    TelnetWrite(pTelnet, replyseq, 3);

#ifdef NITRO_DEBUG
    switch (verb)
    {
    case WILL:
        OS_TPrintf(">IAC-WILL-%d\n", option);
        break;

    case WONT:
        OS_TPrintf(">IAC-WONT-%d\n", option);
        break;

    case DONT:
        OS_TPrintf(">IAC-DONT-%d\n", option);
        break;

    case DO:
        OS_TPrintf(">IAC-DO-%d\n", option);
        break;
    }
#endif
}

/*---------------------------------------------------------------------------*
  Name        : TelnetGetOptionSetting
  Description : �w�肳�ꂽ�I�v�V�������L���ł��邩�𒲂ׂ�
  Arguments   : pTelnet          - 
                option           - �I�v�V�����ԍ�
  Returns     : 
 *---------------------------------------------------------------------------*/
static TelnetOption* TelnetGetOptionSetting(Telnet* pTelnet, u8 option)
{
#pragma unused(pTelnet)
    int i;
    int nOptionCount = sizeof(TelnetOptionList) / sizeof(TelnetOption);
    for (i = 0; i < nOptionCount; ++i)
    {
        if (TelnetOptionList[i].nOptionCode == option)
        {
            return &TelnetOptionList[i];
        }
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name        : TelnetSendSubOption
  Description : �T�u�I�v�V�������M����
  Arguments   : pTelnet          - �n���h��
  Returns     : �Ȃ��B
 *---------------------------------------------------------------------------*/
static void TelnetSendSubOption(Telnet* pTelnet)
{
    TelnetOption*   pOption = TelnetGetOptionSetting(pTelnet, pTelnet->sb_opt);
    const char*     pSubOptionBody = NULL;
    int nBodyLength = 0;

    if (pOption && pOption->pSubOption)
    {
        pSubOptionBody = pOption->pSubOption;
        nBodyLength = pOption->nSubOptionLength;
    }

    if (pSubOptionBody && nBodyLength > 0)
    {
        int socket = pTelnet->socket;
        u8  sb_head[] = { IAC, SB, 0};
        static const u8 sb_end[] = { IAC, SE };

        sb_head[2] = pTelnet->sb_opt;
        TelnetWrite(pTelnet, sb_head, sizeof(sb_head));
        TelnetWrite(pTelnet, pSubOptionBody, nBodyLength);
        TelnetWrite(pTelnet, sb_end, sizeof(sb_end));
        OS_TPrintf(">IAC-SB-opt:%d", pTelnet->sb_opt);
    }
}

/*---------------------------------------------------------------------------*
  Name        : TelnetSendOption
  Description : �I�v�V�����̉�������
  Arguments   : pTelnet          - �n���h��
                option           - �I�v�V�����ԍ�
  Returns     : �Ȃ��B
 *---------------------------------------------------------------------------*/
static void TelnetSendOption(Telnet* pTelnet, u8 option)
{
    TelnetOption*   pOption = TelnetGetOptionSetting(pTelnet, option);
    u8  reply;

    switch (pTelnet->state)
    {
    case RECEIVED_DO:
        if (pOption && pOption->bEnable)
        {
            reply = WILL;
        }
        else
        {
            reply = WONT;
        }
        break;

    case RECEIVED_WILL:
        if (pOption && pOption->bEnable)
        {
            reply = DONT;
        }
        else
        {
            reply = DO;
        }
        break;
    }

    TelnetSendIAC(pTelnet, reply, option);
    if (reply == WILL && pOption && pOption->pSubOption)
    {
        pTelnet->sb_opt = option;
        TelnetSendSubOption(pTelnet);
    }
}

/*---------------------------------------------------------------------------*
  Name        : TelnetRead
  Description : ��M�f�[�^�̏���
  Arguments   : pTelnet          - �n���h��
                pBuffer          - ��M�f�[�^
                nLength          - �T�C�Y
  Returns     : �Ȃ��B
 *---------------------------------------------------------------------------*/
static void TelnetRead(Telnet* pTelnet, void* pBuffer, int nLength)
{
    while (nLength--)
    {
        unsigned char   c = *((unsigned char*)pBuffer)++;

        switch (pTelnet->state)
        {
        case TOP_LEVEL:
        case RECEIVED_CR:
            if (c == '\0' && pTelnet->state == RECEIVED_CR)
            {
                pTelnet->state = TOP_LEVEL;
            }
            else if (c == IAC)
            {
                pTelnet->state = RECEIVED_IAC;
                OS_TPrintf("IAC-");
            }
            else
            {
                OS_TPrintf("%c", c);
                ConsolePutChar((char)c);

                /*if (!pTelnet->in_synch)
                   c_write1(pTelnet, c);
                   if (c == CR && pTelnet->opt_states[o_they_bin.index] != ACTIVE)
                   pTelnet->state = RECEIVED_CR;
                   else */
                pTelnet->state = TOP_LEVEL;
            }
            break;

        case RECEIVED_IAC:
            switch (c)
            {
            case DO:
                pTelnet->state = RECEIVED_DO;
                OS_TPrintf("DO-");
                break;

            case DONT:
                pTelnet->state = RECEIVED_DONT;
                OS_TPrintf("DONT-");
                break;

            case WILL:
                pTelnet->state = RECEIVED_WILL;
                OS_TPrintf("WILL-");
                break;

            case WONT:
                pTelnet->state = RECEIVED_WONT;
                OS_TPrintf("WONT-");
                break;

            case SB:
                pTelnet->state = RECEIVED_SB;
                OS_TPrintf("SB-");
                break;

            case DM:
                {
                    pTelnet->in_synch = 0;
                    pTelnet->state = TOP_LEVEL;
                }

            default:
                {
                    /* IAC�̎����\�z�O�Ȃ疳�� */
                    if (c == IAC)
                    {
                        OS_TWarning("odd IAC!", c);
                    }

                    pTelnet->state = TOP_LEVEL;
                }
            }
            break;

        case RECEIVED_WILL:
        case RECEIVED_WONT:
        case RECEIVED_DO:
        case RECEIVED_DONT:
            TelnetSendOption(pTelnet, c);
            pTelnet->state = TOP_LEVEL;
            break;

        case RECEIVED_SB:
            OS_Printf("opt:%d\n", (u8) c);
            pTelnet->sb_opt = c;
            pTelnet->sb_len = 0;
            pTelnet->state = SUBNEGOT;
            break;

        case SUBNEGOT:
            if (c == IAC)
                pTelnet->state = SUBNEG_IAC;
            else
            {
subneg_addchar:
                if (pTelnet->sb_len >= pTelnet->sb_size)
                {
                    pTelnet->sb_size += SB_DELTA;
                }

                pTelnet->sb_buf[pTelnet->sb_len++] = (char)c;
                pTelnet->state = SUBNEGOT;
            }
            break;

        case SUBNEG_IAC:
            if (c != SE)
            {
                goto subneg_addchar;
            }
            else
            {
                TelnetSendSubOption(pTelnet);
                pTelnet->state = TOP_LEVEL;
            }
            break;
        }
    }
}
