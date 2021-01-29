/*---------------------------------------------------------------------------*
  Project:  NitroSDK - CHT - include
  File:     pictocatch.h

  Copyright 2003-2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: pictocatch.h,v $
  Revision 1.3  2005/03/02 23:42:49  terui
  Update copyright

  Revision 1.2  2005/03/02 12:43:56  terui
  �֐��p�����[�^��const��ǉ��B

  Revision 1.1  2004/12/22 02:40:58  terui
  Initial upload

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITRO_CHT_ARM9_PICTOCATCH_H_
#define NITRO_CHT_ARM9_PICTOCATCH_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include <nitro/types.h>
#include <nitro/wm.h>


/*---------------------------------------------------------------------------*
  Name:         CHT_IsPictochatParent

  Description:  �e�@�̃r�[�R����񂪃s�N�g�`���b�g�̃r�[�R���ł��邩�ǂ�����
                ��������B

  Arguments:    pWmBssDesc  -   ��������r�[�R�����ւ̃|�C���^�B

  Returns:      BOOL        -   �s�N�g�`���b�g�̃r�[�R���ł���ꍇ��TRUE���A
                                �����łȂ��ꍇ��FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
BOOL    CHT_IsPictochatParent( const WMBssDesc* pWmBssDesc );

/*---------------------------------------------------------------------------*
  Name:         CHT_GetPictochatClientNum

  Description:  �s�N�g�`���b�g�̃r�[�R����񂩂�O���[�v�ɎQ�����Ă���q�@��
                ���𒲍�����B
                �s�N�g�`���b�g�̃r�[�R���łȂ��ꍇ�͕s��Ȓl��Ԃ����ƂɂȂ�
                �̂ŁACHT_IsPictochatParent �֐��Ŕ��肵�Ă���g�p����B

  Arguments:    pWmBssDesc  -   ��������r�[�R�����ւ̃|�C���^�B

  Returns:      int         -   �O���[�v�ɎQ�����Ă���q�@�̐���Ԃ��B
                                �ُ�ȃr�[�R�����ł������ꍇ�� -1 ��Ԃ��B
 *---------------------------------------------------------------------------*/
int     CHT_GetPictochatClientNum( const WMBssDesc* pWmBssDesc );

/*---------------------------------------------------------------------------*
  Name:         CHT_GetPictochatRoomNumber

  Description:  �s�N�g�`���b�g�̃r�[�R����񂩂烋�[���ԍ����擾����B
                �s�N�g�`���b�g�̃r�[�R���łȂ��ꍇ�͕s��Ȓl��Ԃ����ƂɂȂ�
                �̂ŁACHT_IsPictochatParent �֐��Ŕ��肵�Ă���g�p����B

  Arguments:    pWmBssDesc  -   ��������r�[�R�����ւ̃|�C���^�B

  Returns:      int         -   ���[���ԍ���Ԃ��B
                                �ُ�ȃr�[�R�����ł������ꍇ�� -1 ��Ԃ��B
 *---------------------------------------------------------------------------*/
int     CHT_GetPictochatRoomNumber( const WMBssDesc* pWmBssDesc );


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif  /* NITRO_CHT_ARM9_PICTOCATCH_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
