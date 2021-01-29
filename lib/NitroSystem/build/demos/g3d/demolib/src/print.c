/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib
  File:     print.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.7 $
 *---------------------------------------------------------------------------*/

#include "g3d_demolib/print.h"
#include "g3d_demolib/font.h"

#define SCREEN_HEIGHT       192     // ��ʍ���
#define SCREEN_WIDTH        256     // ��ʕ�

#define SCREEN_CHARA_SIZE   8       // BG�L�����N�^�T�C�Y

#define SCR_PLTT_SHIFT      12      // �X�N���[���f�[�^���̃p���b�gNo�p�V�t�g��
#define FONT_COLOR          4       // �����F=��

// �L�����N�^�P�ʂ̉�ʃT�C�Y
#define SCREEN_HEIGHT_CHARA (SCREEN_HEIGHT / SCREEN_CHARA_SIZE)
#define SCREEN_WIDTH_CHARA  (SCREEN_WIDTH / SCREEN_CHARA_SIZE)

// �X�N���[���o�b�t�@
static u16 sScreenBuf[SCREEN_HEIGHT_CHARA][SCREEN_WIDTH_CHARA];


/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitConsole

  Description:  ���C����ʂ�BG1�𕶎��\���p�ɃZ�b�g�A�b�v���܂��B
                system.c �� Init3DStuff_ �̏����Ɉˑ����Ă��܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G3DDemo_InitConsole(void)
{
    // ���\�[�X�̃��[�h
    GX_LoadBG1Char(fontCharData, 0, sizeof(fontCharData));
	GX_LoadBGPltt (fontPlttData, 0, 0x00200);

    // �����o�b�t�@�̃N���A
    G3DDemo_ClearConsole();
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_ClearConsole

  Description:  �����X�N���[���o�b�t�@���N���A���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G3DDemo_ClearConsole(void)
{
    MI_CpuClearFast( sScreenBuf, sizeof(sScreenBuf) );
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_Print

  Description:  ���C����ʂ�BG1�𗘗p���ĕ������\�����܂��B
                1�s32���� 24�s�̕\�����ł��܂��B

  Arguments:    x:      1�����ڂ�\���������w�肵�܂��B0����n�܂�܂��B
                y:      �\������s���w�肵�܂��B0����n�܂�܂��B
                string: �o�͂��镶����ł��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G3DDemo_Print(int x, int y, int color, const char* string)
{
    const char* pos;
    SDK_NULL_ASSERT( string );
    SDK_ASSERT( 0 <= x && x < SCREEN_WIDTH_CHARA );
    SDK_ASSERT( 0 <= y && y < SCREEN_HEIGHT_CHARA );

    for( pos = string; *pos != '\0'; ++pos )
    {
        sScreenBuf[y][x] = (u16)((u16)(color << SCR_PLTT_SHIFT) | (u16)(*pos));
        x++;

        if( x >= SCREEN_WIDTH )
        {
            break;
        }
    }
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_Printf

  Description:  ���C����ʂ�BG1�𗘗p���ĕ������\�����܂��B
                1�s32���� 24�s�̕\�����ł��܂��B

  Arguments:    x:      1�����ڂ�\���������w�肵�܂��B0����n�܂�܂��B
                y:      �\������s���w�肵�܂��B0����n�܂�܂��B
                format: printf�`���̃t�H�[�}�b�g������ł��B
                ...:    �t�H�[�}�b�g�ɑΉ�����p�����[�^�ł��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G3DDemo_Printf(int x, int y, int color, const char* format, ...)
{
    va_list vlist;
    char buf[SCREEN_WIDTH_CHARA + 1];
	SDK_NULL_ASSERT( format );
    
    va_start( vlist, format );
    (void)vsnprintf( buf, sizeof(buf), format, vlist );
    va_end( vlist );
    
    G3DDemo_Print(x, y, color, buf);
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_PrintApplyToHW

  Description:  �����X�N���[���o�b�t�@���n�[�h�E�F�A�ɓ]�����܂��B
                �ʏ�� V �u�����N���ƂɌĂяo���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
void G3DDemo_PrintApplyToHW(void)
{
    DC_FlushRange( sScreenBuf, sizeof(sScreenBuf) );
    GX_LoadBG1Scr( sScreenBuf, 0, sizeof(sScreenBuf) );
}

