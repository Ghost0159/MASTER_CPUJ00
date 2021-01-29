/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SOC - demos - httpclient
  File:     main.c

  Copyright 2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.6  2006/03/10 09:22:43  kitase_hirotake
  INDENT SOURCE

  Revision 1.5  2006/01/16 01:40:58  kitase_hirotake
  ���J�p�� sitedefs �̏C��

  Revision 1.4  2006/01/12 00:10:21  kitase_hirotake
  2005->2006 �ɏC��

  Revision 1.3  2006/01/10 04:18:39  kitase_hirotake
  �@�\���V���v���Ȃ��̂ɕύX

  Revision 1.2  2006/01/06 00:44:47  kitase_hirotake
  PAD���͑Ή���
  
  Revision 1.1  2005/12/22 04:34:06  adachi_hiroaki
  HTTP�N���C�A���g�̃f����SOC�Œu������

  $NoKeywords$
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
    http �N���C�A���g�Ƃ��ē��삷��T���v���v���O�����ł��B
    PAD��URL����͂��邱�ƂŃT�[�o�ւƐڑ����܂��B

    PAD�ł̕������͂̕��@:
        1. �������͂�PAD��p���čs���܂��B
           L�{�^�����������Ƃ�DIRECT_MODE��INDIRECT_MODE���؂�ւ��܂��B
           �������͂�INDIRECT_MODE�ōs���܂��B
        2. �㉺�L�[�őI�𒆂̕�����ς��܂��B
           R�{�^���ŕ����̎�ނ������A�啶���A�������A�L���ƕς��܂��B
           A�{�^���ŃJ�����g�̕��������肵�A���̕����̓��͂Ɉڂ�܂��B
           ���E�L�[�ō��܂łɓ��͂��������ɃJ�����g���ړ����A�ύX���ł��܂��B
        3. B�{�^���őI�𒆂̕����������܂��B
        4. X�{�^���ō��܂łɓ��͂���������𑗐M���܂��B
    
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitroWiFi.h>
#include <string.h>
#include <sitedefs.h>

#include "padkeyboard.h"
#include "output.h"
#include "screen.h"

#include "netconnect.h"
#include "http.h"
#include "ssl.h"

#define SITEDEFS_APCLASS    "WiFi.LAN.1.AP.1"

#define URL_LENGTH          64

static void VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}

void NitroMain()
{
    char    url[URL_LENGTH];

    /* �������֐�,�ڑ��֐��Ăяo�� */
    NcGlobalInit();
    NcStart(SITEDEFS_APCLASS);

    /* �X�N���[���̏����� */
    InitScreen();

    FlushScreen();
    FlushDebugOutput();

    /* �L�[���͏��擾�̋�Ăяo��(IPL �ł� A �{�^�������΍�) */
    (void)PAD_Read();

    /* �L�[�{�[�h�̏����� */
    InitKeyboardThread();
    KeyboardValid();

    /* HTTP server �ւ̐ڑ�             *
     * Pad �Őڑ���� URL ����� */
    DebugOutput("http_server_ip>");

    /* �����l��URL�̓��[�^�̊Ǘ���� */
    MI_CpuClear8(url, URL_LENGTH);
    {
        SOCInAddr   addr;
        SOC_GetGateway(NULL, (u8*) &addr);
        (void)OS_SNPrintf(url, sizeof(url), "http://%s/", SOC_InetNtoA(addr));
    }

    while (1)
    {
        PadKeySetString(url);

        /* URL �̓��͂����� */
        PadKeyGetInputString(url, URL_LENGTH);

        {
            static char         pageBuffer[1024 * 20];
            int         socket = -1;
            SOCSslConnection*   sslCon;

            SslInit();
            socket = HttpConnect(url, &sslCon);

            OS_TPrintf("URL: %s\n", url);

            if (socket < 0)
            {
                OS_Panic("failed to connect.");
            }

            if (HttpGet(socket, url, NULL, NULL, 0, pageBuffer, sizeof(pageBuffer)) < 0)
            {
                OS_Panic("failed to download\n");
            }

            OS_PutString(pageBuffer);
            (void)HttpClose(socket, sslCon);
        }
    }

    NcFinish();
    OS_Terminate();
}
