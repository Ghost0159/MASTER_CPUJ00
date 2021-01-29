/*---------------------------------------------------------------------------*
  Project:  NitroWiFi - WCM - include
  File:     wcm_cpsif.h

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wcm_cpsif.h,v $
  Revision 1.3.4.2  2007/09/18 09:23:55  okubata_ryoma
  Copyright fix

  Revision 1.3.4.1  2007/09/18 09:16:59  okubata_ryoma
  HEAD�ƃ}�[�W

  Revision 1.3  2006/03/10 09:22:19  kitase_hirotake
  INDENT SOURCE

  Revision 1.2  2005/08/08 11:15:49  terui
  WCM_GetApEssid �֐���ǉ�

  Revision 1.1  2005/07/07 10:45:37  terui
  �V�K�ǉ�

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
  Name:         WCM_GetApEssid

  Description:  �����h���C�o���ڑ����m�����Ă��� AP �� ESS-ID ���擾����B
    NOTICE:     ���̊֐��ɂ���Ď擾�����|�C���^�������o�b�t�@�̓��e�́A���荞��
                ���ɂ���ԕω��ɔ����ĕύX���ꂽ��A�o�b�t�@���g��������ꂽ��
                ����\��������_�ɒ��ӁB���荞�݋֎~������ԂŌĂяo���A���e��
                �ʃo�b�t�@�ɑޔ����邱�Ƃ𐄏�����B

  Arguments:    length  -   ESS-ID �̒������擾���� u16 �^�ϐ��ւ̃|�C���^���w��
                            ����B�ڑ����m������Ă��Ȃ��ꍇ�ɂ� 0 ���i�[�����B
                            �������擾����K�v���Ȃ��ꍇ�ɂ́A���̈����� NULL ��
                            �w�肵�Ă��悢�B

  Returns:      u8*     -   �ڑ����m�����Ă��� AP �� ESS-ID �f�[�^��ւ̃|�C���^
                            ��Ԃ��B�ڑ����m������Ă��Ȃ��ꍇ�ɂ� NULL ��Ԃ��B
 *---------------------------------------------------------------------------*/
u8*     WCM_GetApEssid(u16* length);

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

/*---------------------------------------------------------------------------*
  Name:         WCM_SendDCFDataEx

  Description:  �����h���C�o�� DCF �t���[�����M���w������BDCF �t���[���̑��M
                �����A�������͎��s����܂œ����Ńu���b�N���đ҂B�܂��A������
                �ڑ���Ԃ� DCF �t���[���𑗐M�ł��Ȃ���Ԃ̏ꍇ�ɂ͎��s����B
    NOTICE:     �����̃X���b�h����񓯊��ɌĂяo����邱�Ƃ�z�肵�Ă��邪�A
                ���荞�݃T�[�r�X���ŌĂяo����邱�Ƃ͑z�肵�Ă��Ȃ��_�ɒ��ӁB

  Arguments:    dstAddr -   DCF �t���[���̂��Đ�ƂȂ� MAC �A�h���X���w�肷��B
                header      -   ���M����f�[�^�̃w�b�_�����ւ̃|�C���^���w�肷��B
                headerLen   -   ���M����f�[�^�̃w�b�_�̒������o�C�g�P�ʂŎw�肷��B
                body        -   ���M����f�[�^�̃{�f�B�����ւ̃|�C���^���w�肷��B
                bodyLen     -   ���M����f�[�^�̃{�f�B�����̒������o�C�g�P�ʂŎw�肷��B
                
                * header�Abody �͋����A�h���X�ł���K�v������A���M�f�[�^�����݂���̈���܂�
                  �L���b�V���������I�ɃX�g�A(�������֏����o��)�����B
                  �܂��A���֐�����߂�܂ł̓f�[�^�{�̂̓��e�͕ێ�����Ă��Ȃ���΂Ȃ�Ȃ��_�ɂ����ӁB

  Returns:      s32     -   ���M�ɐ��������ꍇ�ɑ��M���ꂽ�f�[�^����Ԃ��B
                            ���s�����ꍇ�ɂ͕��l��Ԃ��B
 *---------------------------------------------------------------------------*/
s32     WCM_SendDCFDataEx(const u8* dstAddr, const u8* header, s32 headerLen,
                          const u8* body, s32 bodyLen);

/*===========================================================================*/
#ifdef __cplusplus

}       /* extern "C" */
#endif

#endif /* NITROWIFI_WCM_CPSIF_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
