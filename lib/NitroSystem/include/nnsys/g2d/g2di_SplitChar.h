/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2di_SplitChar.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#ifndef G2DI_SPLITCHAR_H_
#define G2DI_SPLITCHAR_H_

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif


// �����؂�o���R�[���o�b�N
typedef u16 (*NNSiG2dSplitCharCallback)(const void** ppChar);


/*---------------------------------------------------------------------------*
  Name:         NNSi_G2dGetChar*

  Description:  NNSiG2dGetCharCallback �^�̕����R�[�h�؂�o���֐��ł��B
                �o�C�g�X�g���[������ŏ��̕����̕����R�[�h���擾����ƂƂ���
                �X�g���[���|�C���^�����̕����Ɉړ������܂��B

  Arguments:    ppChar: �o�C�g�z��ւ̃|�C���^���i�[���Ă���o�b�t�@�ւ̃|�C���^�B
                        �֐�����Ԃ�ƁA���̃|�C���^�̐�̃o�b�t�@�ɂ�
                        ���̕����̐擪�ւ̃|�C���^���i�[����܂��B

  Returns:      *ppChar �̍ŏ��̕����̕����R�[�h�B
 *---------------------------------------------------------------------------*/
u16 NNSi_G2dSplitCharUTF16(const void** ppChar);
u16 NNSi_G2dSplitCharUTF8(const void** ppChar);
u16 NNSi_G2dSplitCharShiftJIS(const void** ppChar);
u16 NNSi_G2dSplitChar1Byte(const void** ppChar);






#ifdef __cplusplus
}/* extern "C" */
#endif

#endif // G2DI_SPLITCHAR_H_

