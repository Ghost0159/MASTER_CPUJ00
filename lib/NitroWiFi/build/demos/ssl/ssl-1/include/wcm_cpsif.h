/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - SSL - demos - ssl-1
  File:     wcm_cpsif.h

  Copyright 2005,2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_cpsif.h,v $
  Revision 1.3  2006/03/10 09:22:42  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/11/02 02:41:33  terui
  Fix comment.

  Revision 1.1  2005/07/21 11:47:03  seiki_masashi
  ssl/ssl-1 �Ɉړ�

  Revision 1.1  2005/07/18 08:10:26  seiki_masashi
  ssl-1 �f���̒ǉ�

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef NITROWIFI_WCM_CPSIF_H_
#define NITROWIFI_WCM_CPSIF_H_

#ifdef __cplusplus

extern "C" {
#endif

/*===========================================================================*/
#include <nitro/types.h>

/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/

// WCM_SendDCFData �֐��̕Ԃ�l ( �ُ펞 )
#define WCM_CPSIF_RESULT_BEFORE_INIT    (-1)    // WCM���C�u����������������Ă��Ȃ�
#define WCM_CPSIF_RESULT_ILLEGAL_PARAM  (-2)    // �w��p�����[�^���s��
#define WCM_CPSIF_RESULT_IN_IRQ_MODE    (-3)    // IRQ ���[�h�ł̌Ăяo��
#define WCM_CPSIF_RESULT_NO_CONNECTION  (-4)    // AP �Ɛڑ��m�����Ă��Ȃ��A�������͓r���Őؒf���ꂽ
#define WCM_CPSIF_RESULT_SEND_FAILURE   (-5)    // DCF �t���[���̑��M�ɂȂ�炩�̗��R�Ŏ��s( �v�đ� )

/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/

// DCF �t���[����M�R�[���o�b�N�֐��^
typedef void (*WCMRecvInd) (const u8*srcAddr, const u8*dstAddr, const u8*buf, s32 len);

/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
  Name:         WCM_SetRecvDCFCallback

  Description:  CPS �̎�M�R�[���o�b�N�֐���o�^����B
                �����h���C�o�� DCF �t���[������M����x�ɂ����œo�^������M
                �R�[���o�b�N�֐����Ăяo�����B
    NOTICE:     �����œo�^�����R�[���o�b�N�֐����I�[�o�[���C���ɂ���ꍇ�ɂ́A
                ��ѐ悪�������ɑ��݂��Ȃ��Ȃ�O�ɓo�^���Ă���R�[���o�b�N�֐���
                NULL �ŏ㏑���N���A����K�v������_�ɒ��ӁB

  Arguments:    callback    -   DCF ��M�R�[���o�b�N�֐����w�肷��B
                                NULL ���w�肷��Ǝ�M�ʒm����Ȃ��Ȃ�B

  Returns:      None.
 *---------------------------------------------------------------------------*/
void    WCM_SetRecvDCFCallback(WCMRecvInd callback);

/*---------------------------------------------------------------------------*
  Name:         WCM_GetApMacAddress

  Description:  �����h���C�o���ڑ����m�����Ă��� AP �� MAC �A�h���X���擾����B
    NOTICE:     ���̊֐��ɂ���Ď擾�����|�C���^�������o�b�t�@�̓��e�́A���荞��
                ���ɂ���ԕω��ɔ����ĕύX���ꂽ��A�o�b�t�@���g��������ꂽ��
                ����\��������_�ɒ��ӁB���荞�݋֎~������ԂŌĂяo���A���e��
                �ʃo�b�t�@�ɑޔ����邱�Ƃ𐄏�����B

  Arguments:    None.

  Returns:      u8*     -   �ڑ����m�����Ă��� AP �� MAC �A�h���X�ւ̃|�C���^��
                            �Ԃ��B�ڑ����m������Ă��Ȃ��ꍇ�ɂ� NULL ��Ԃ��B
 *---------------------------------------------------------------------------*/
u8*     WCM_GetApMacAddress(void);

/*---------------------------------------------------------------------------*
  Name:         WCM_SendDCFData

  Description:  �����h���C�o�� DCF �t���[�����M���w������BDCF �t���[���̑��M
                �����A�������͎��s����܂œ����Ńu���b�N���đ҂B�܂��A������
                �ڑ���Ԃ� DCF �t���[���𑗐M�ł��Ȃ���Ԃ̏ꍇ�ɂ͎��s����B
    NOTICE:     �����̃X���b�h����񓯊��ɌĂяo����邱�Ƃ�z�肵�Ă��邪�A
                ���荞�݃T�[�r�X���ŌĂяo����邱�Ƃ͑z�肵�Ă��Ȃ��_�ɒ��ӁB

  Arguments:    dstAddr -   DCF �t���[���̂��Đ�ƂȂ� MAC �A�h���X���w�肷��B
                buf     -   ���M����f�[�^�ւ̃|�C���^���w�肷��B�����A�h���X
                            �ł���K�v������A���M�f�[�^�����݂���̈���܂�
                            �L���b�V���������I�ɃX�g�A(�������֏����o��)�����B
                            �܂��A���֐�����߂�܂ł̓f�[�^�{�̂̓��e�͕ێ�����
                            �Ă��Ȃ���΂Ȃ�Ȃ��_�ɂ����ӁB
                len     -   ���M����f�[�^�̒������o�C�g�P�ʂŎw�肷��B

  Returns:      s32     -   ���M�ɐ��������ꍇ�ɑ��M���ꂽ�f�[�^����Ԃ��B
                            ���s�����ꍇ�ɂ͕��l��Ԃ��B
 *---------------------------------------------------------------------------*/
s32     WCM_SendDCFData(const u8* dstAddr, const u8* buf, s32 len);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* NITROWIFI_WCM_CPSIF_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/