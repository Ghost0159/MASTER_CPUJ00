/*---------------------------------------------------------------------------*
  Project:  NitroSDK - STD - demos - unicode-1
  File:     main.c

  Copyright 2006,2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: main.c,v $
  Revision 1.5  2007/02/20 00:28:11  kitase_hirotake
  indent source

  Revision 1.4  2006/08/10 07:54:13  yosizaki
  change STD-API interface.

  Revision 1.3  2006/07/24 12:25:58  yosizaki
  change callback.

  Revision 1.2  2006/07/24 09:36:56  yosizaki
  beautify indent.
  change of STD-API interface.

  Revision 1.1  2006/07/05 07:45:31  kitase_hirotake
  std �� sjis <-> unicode �ϊ����C�u�����̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include <nitro.h>

/*---------------------------------------------------------------------------*
  Name:         ConvertUnicodeCallback

  Description:  STD_ConvertStringSjisToUnicode�֐��ɐݒ肷��R�[���o�b�N.

  Arguments:    dst    : �ϊ�����Unicode�`���̕������i�[����|�C���^.
                dstlen : ���ۂɕϊ����ꂽdst�̕�����.
                src    : �ϊ����ׂ�ShiftJIS�`���̕������w���|�C���^.
                srclen : �ϊ��ɂ���ēǂݐi�߂�ׂ�src�̃o�C�g�T�C�Y.

  Returns:      �ϊ�����������.
 *---------------------------------------------------------------------------*/
static STDResult ConvertUnicodeCallback(u16 *dst, int *dstlen, const char *src, int *srclen)
{
#pragma unused(src)
    *srclen = 1;
    *dstlen = 1;
    MI_StoreLE16(&dst[0], 0x3044);
    return STD_RESULT_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         ConvertSjisCallback

  Description:  STD_ConvertStringSjisToUnicode�֐��ɐݒ肷��R�[���o�b�N.

  Arguments:    dst    : �ϊ�����ShiftJIS�`���̕������i�[����|�C���^.
                dstlen : ���ۂɕϊ����ꂽdst�̕�����.
                src    : �ϊ����ׂ�Unicode�`���̕������w���|�C���^.
                srclen : �ϊ��ɂ���ēǂݐi�߂�ׂ�src�̕�����.

  Returns:      �ϊ������o�C�g��.
 *---------------------------------------------------------------------------*/
static STDResult ConvertSjisCallback(char *dst, int *dstlen, const u16 *src, int *srclen)
{
#pragma unused(src)
    *srclen = 1;
    *dstlen = 2;
    dst[0] = (char)0x82;
    dst[1] = (char)0xA2;
    return STD_RESULT_SUCCESS;
}

/*---------------------------------------------------------------------------*
  Name:         NitroMain

  Description:  ������ϊ��e�X�g.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void NitroMain(void)
{
    OS_Init();
    OS_InitTick();
    OS_InitThread();

    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)OS_EnableIrq();

    OS_TPrintf("\n------STD_ConvertStringSjisToUnicode---------\n\n");
    {
        static const char mbs[] =
            "���������ܦ݂�����aiueo��_�`�_���P�P�`�A�����@�����K��/\\���A�B���@";
        u16     wcs[256];
        int     wcs_length;
        int     i;
        STDResult result;

        OS_TPrintf("ShiftJIS = ");
        for (i = 0; mbs[i] != '\0'; i++)
        {
            OS_TPrintf("%x ", (u8)mbs[i]);
        }
        OS_TPrintf("\n\n");

        wcs_length = sizeof(wcs) / sizeof(u16);
        result =
            STD_ConvertStringSjisToUnicode(wcs, &wcs_length, mbs, NULL, ConvertUnicodeCallback);
        OS_TPrintf("Unicode = ");
        for (i = 0; i < wcs_length; i++)
        {
            OS_TPrintf("%x ", wcs[i]);
        }
        OS_TPrintf("\n");
    }

    OS_TPrintf("\n------STD_ConvertStringUnicodeToSjis---------\n\n");
    {
        static const u16 wcs[] =
            L"���������ܦ݂�����aiueo��_�`�_���P�P�`�A�����@�����K��/\\���A�B���@";

        char    mbs[256];
        int     mbs_length;
        int     i;
        STDResult result;

        OS_TPrintf("Unicode = ");
        for (i = 0; wcs[i] != L'\0'; i++)
        {
            OS_TPrintf("%x ", wcs[i]);
        }
        OS_TPrintf("\n\n");

        mbs_length = sizeof(mbs) / sizeof(*mbs);
        result = STD_ConvertStringUnicodeToSjis(mbs, &mbs_length, wcs, NULL, ConvertSjisCallback);

        OS_TPrintf("ShiftJIS = ");
        for (i = 0; i < mbs_length; i++)
        {
            OS_TPrintf("%x ", (u8)mbs[i]);
        }
        OS_TPrintf("\n");
    }

    OS_TPrintf("\n------STD_ConverCharSjisToUnicode-----\n\n");
    {
        char    mb[] = "��";
        u16     wc;
        int     wc_length;

        OS_TPrintf("ShiftJIS �� = %x, %x\n", (u8)mb[0], (u8)mb[1]);

        wc_length = STD_ConvertCharSjisToUnicode(&wc, mb);

        OS_TPrintf("Unicode �� = %x\n", wc);
    }

    OS_TPrintf("\n------STD_ConverCharUnicodeToSjis-----\n\n");
    {
        u16     wc = L'��';
        char    mb[2];
        int     mb_length;

        OS_TPrintf("Unicode �� = %x\n", wc);

        mb_length = STD_ConvertCharUnicodeToSjis(mb, wc);

        OS_TPrintf("ShiftJIS �� = %x, %x\n", (u8)mb[0], (u8)mb[1]);

    }

    OS_TPrintf("\n" "++++++++++++++++++++++++++++++++++++++++\n" "Test Finish\n");

    OS_Terminate();

}
