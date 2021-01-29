/*
  Project:  NitroSDK - CHT - demos - catch-min
  File:     wmscan.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wmscan.h,v $
  Revision 1.2  2006/01/18 02:12:40  kitase_hirotake
  do-indent

  Revision 1.1  2005/07/14 01:34:40  kitase_hirotake
  Initial Release

  $NoKeywords$
 */
#include <nitro/types.h>
#include <nitro/wm.h>

typedef void (*WSStartScanCallbackFunc) (WMBssDesc *bssDesc);

enum
{
    WS_SYSSTATE_STOP,                  // ������~��
    WS_SYSSTATE_IDLE,                  // IDLE���
    WS_SYSSTATE_SCANNING,              // �X�L������
    WS_SYSSTATE_SCANIDLE,              // �X�L������~��
    WS_SYSSTATE_BUSY,                  // �r�W�[��
    WS_SYSSTATE_ERROR,                 // �G���[����
    WS_SYSSTATE_FATAL                  // FATAL�G���[����
};


/*---------------------------------------------------------------------------*
  Name:         WS_Initialize
  Description:  ���������������܂��B
  Arguments:    buf   WM�ɓn���o�b�t�@�T�C�Y�ł��BWM_SYSTEM_BUF_SIZE�̗̈悪�K�v�ł��B
                dmaNo �����Ŏg�p�����DMA�ԍ����w�肵�܂��B
  Returns:      ����������ɊJ�n���ꂽ�ꍇ�ɂ� TRUE
                �J�n����Ȃ������ꍇ�ɂ͏ꍇ�ɂ� FALSE
 *---------------------------------------------------------------------------*/
BOOL    WS_Initialize(void *buf, u16 dmaNo);

/*---------------------------------------------------------------------------*
  Name:         WS_StartScan
  Description:  �e�@�̃X�L�������J�n���܂��B
  Arguments:    callback �e�@�������̃R�[���o�b�N
                macAddr  �e�@��MAC�A�h���X���w�肵�܂��B
                         �S�e�@����������ꍇ�� FF:FF:FF:FF:FF:FF���w�肵�܂�
                continuous ���̃t���O��TRUE�̏ꍇ�́AWS_EndScan���R�[�������܂ł̊�
                           �L���ȑS�`�����l�����X�L�����������܂��B
                           FALSE�̏ꍇ�́A�L���ȃ`�����l�������[�e�[�V�������Ȃ���A
                           �P�X�L�������ɃX�L�������~���AWS_SYSSTATE_SCANIDLE��ԂɑJ�ڂ��܂��B
  Returns:      ����������ɊJ�n���ꂽ�ꍇ�ɂ� TRUE
                �J�n����Ȃ������ꍇ�ɂ͏ꍇ�ɂ� FALSE
 *---------------------------------------------------------------------------*/
BOOL    WS_StartScan(WSStartScanCallbackFunc callback, const u8 *macAddr, BOOL continuous);

/*---------------------------------------------------------------------------*
  Name:         WS_EndScan
  Description:  �X�L�������I������֐�
  Arguments:    None.
  Returns:      ����������ɊJ�n���ꂽ�ꍇ�ɂ� TRUE
                �J�n����Ȃ������ꍇ�ɂ͏ꍇ�ɂ� FALSE
 *---------------------------------------------------------------------------*/
BOOL    WS_EndScan(void);

/*---------------------------------------------------------------------------*
  Name:         WS_End
  Description:  �����ʐM���I������B
  Arguments:    None.
  Returns:      ��������ΐ^�B
 *---------------------------------------------------------------------------*/
BOOL    WS_End(void);

/*---------------------------------------------------------------------------*
  Name:         WS_SetGgid
  Description:  �Q�[���O���[�vID��ݒ肵�܂��B
                �e�@�̐ڑ��O�ɌĂяo���܂��B
  Arguments:    ggid    �ݒ肷��Q�[���O���[�vID.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WS_SetGgid(u32 ggid);

/*---------------------------------------------------------------------------*
  Name:         WS_GetSystemState
  Description:  WS��Ԃ��擾���܂��B
  Arguments:    None.
  Returns:      WS���.
 *---------------------------------------------------------------------------*/
int     WS_GetSystemState(void);

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOnPictoCatch
  Description:  �s�N�g�L���b�`�@�\��L���ɂ���B
                WS_StartScan�ɂăX�L�������ɁA�s�N�g�`���b�g�𔭌������ꍇ�ɂ�
                �R�[���o�b�N�֐����Ă΂��悤�ɂȂ�B
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WS_TurnOnPictoCatch(void);

/*---------------------------------------------------------------------------*
  Name:         WS_TurnOffPictoCatch
  Description:  �s�N�g�L���b�`�@�\�𖳌��ɂ���B
  Arguments:    None.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WS_TurnOffPictoCatch(void);
