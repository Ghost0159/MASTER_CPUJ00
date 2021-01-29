/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g2d - demolib
  File:     print.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.5 $
 *---------------------------------------------------------------------------*/

#include "g2d_demolib.h"
#include "g2d_demolib/print.h"
#include "g2d_demolib/fontData.h"

#define SCREEN_HEIGHT       192     // ��ʍ���
#define SCREEN_WIDTH        256     // ��ʕ�

#define SCREEN_CHARA_SIZE   8       // BG�L�����N�^�T�C�Y

#define SCR_PLTT_SHIFT      12      // �X�N���[���f�[�^���̃p���b�gNo�p�V�t�g��
#define FONT_COLOR          3       // �����F=��

// �L�����N�^�P�ʂ̉�ʃT�C�Y
#define SCREEN_HEIGHT_CHARA (SCREEN_HEIGHT / SCREEN_CHARA_SIZE)
#define SCREEN_WIDTH_CHARA  (SCREEN_WIDTH / SCREEN_CHARA_SIZE)

// �X�N���[���o�b�t�@
static u16 sScreenBuf[SCREEN_HEIGHT_CHARA][SCREEN_WIDTH_CHARA];


/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintInit

  Description:  ���C����ʂ�BG1�𕶎��\���p�ɃZ�b�g�A�b�v���܂��B
                system.c �� Init3DStuff_ �̏����Ɉˑ����Ă��܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintInit(void)
{
    // BG1 �̃Z�b�g�A�b�v
    GX_SetBankForBGExtPltt(GX_VRAM_BGEXTPLTT_01_F);
    GX_SetVisiblePlane( GX_PLANEMASK_BG0 | GX_PLANEMASK_BG1 | GX_PLANEMASK_OBJ );
    G2_SetBG1Control(
        GX_BG_SCRSIZE_TEXT_256x256,
        GX_BG_COLORMODE_256,
        GX_BG_SCRBASE_0x0000,
        GX_BG_CHARBASE_0x04000,
        GX_BG_EXTPLTT_01);
    G2_SetBG1Priority(3);

    // ���\�[�X�̃��[�h
    GX_LoadBG1Char(fontCharData, 0, sizeof(fontCharData));
    
    GX_BeginLoadBGExtPltt();
    GX_LoadBGExtPltt(fontPlttData, 0x2000, sizeof(fontPlttData));
    GX_EndLoadBGExtPltt();

    // �����o�b�t�@�̃N���A
    G2DDemo_PrintClear();
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintClear

  Description:  �����X�N���[���o�b�t�@���N���A���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintClear(void)
{
    MI_CpuClearFast( sScreenBuf, sizeof(sScreenBuf) );
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintOut

  Description:  ���C����ʂ�BG1�𗘗p���ĕ������\�����܂��B
                1�s32���� 24�s�̕\�����ł��܂��B

  Arguments:    x:      1�����ڂ�\���������w�肵�܂��B0����n�܂�܂��B
                y:      �\������s���w�肵�܂��B0����n�܂�܂��B
                string: �o�͂��镶����ł��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintOut(int x, int y, const char* string)
{
    const char* pos;
    SDK_NULL_ASSERT( string );
    SDK_ASSERT( 0 <= x && x < SCREEN_WIDTH_CHARA );
    SDK_ASSERT( 0 <= y && y < SCREEN_HEIGHT_CHARA );

    for( pos = string; *pos != '\0'; ++pos )
    {
        sScreenBuf[y][x] = (u16)((u16)(FONT_COLOR << SCR_PLTT_SHIFT) | (u16)(*pos));
        x++;

        if( x >= SCREEN_WIDTH )
        {
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintOutf

  Description:  ���C����ʂ�BG1�𗘗p���ĕ������\�����܂��B
                1�s32���� 24�s�̕\�����ł��܂��B

  Arguments:    x:      1�����ڂ�\���������w�肵�܂��B0����n�܂�܂��B
                y:      �\������s���w�肵�܂��B0����n�܂�܂��B
                format: printf�`���̃t�H�[�}�b�g������ł��B
                ...:    �t�H�[�}�b�g�ɑΉ�����p�����[�^�ł��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintOutf(int x, int y, const char* format, ...)
{
    va_list vlist;
    char buf[SCREEN_WIDTH_CHARA + 1];
	SDK_NULL_ASSERT( format );
    
    va_start( vlist, format );
    (void)vsnprintf( buf, sizeof(buf), format, vlist );
    va_end( vlist );
    
    G2DDemo_PrintOut(x, y, buf);
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_PrintApplyToHW

  Description:  �����X�N���[���o�b�t�@���n�[�h�E�F�A�ɓ]�����܂��B
                �ʏ�� V �u�����N���ƂɌĂяo���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G2DDemo_PrintApplyToHW(void)
{
    DC_FlushRange( sScreenBuf, sizeof(sScreenBuf) );
    GX_LoadBG1Scr( sScreenBuf, 0, sizeof(sScreenBuf) );
}

