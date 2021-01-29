/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wireless_shared
  File:     wc.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: wc.h,v $
  Revision 1.11  2006/03/13 06:04:46  okubata_ryoma
  �}�N���̔��C��

  Revision 1.10  2006/01/18 02:12:39  kitase_hirotake
  do-indent

  Revision 1.9  2005/04/26 02:35:02  terui
  Fix comment

  Revision 1.8  2005/02/28 05:26:34  yosizaki
  do-indent.

  Revision 1.7  2005/02/02 04:21:20  terui
  TurnOff �̂Â�ԈႢ���C���B

  Revision 1.6  2005/01/11 07:51:38  terui
  Update copyright.

  Revision 1.5  2005/01/11 04:59:09  terui
  WcFinish�֐���ǉ��B

  Revision 1.4  2004/12/22 02:46:48  terui
  �s�N�g�`���b�g�T�[�`�ɑΉ�

  Revision 1.3  2004/10/29 01:54:47  terui
  �q�@�̃X�L�������̃`�����l����WcInit�ɓn���ꂽ�`�����l�����g�p����悤�C���B

  Revision 1.2  2004/10/28 11:20:16  yosizaki
  merge with wbt-1.

  Revision 1.1  2004/10/22 11:04:14  terui
  wc���W���[�������L�̈�ɐ؂�o���B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef DEMOS_WIRELESS_SHARED_WC_H_
#define DEMOS_WIRELESS_SHARED_WC_H_

#ifdef  __cplusplus
extern "C" {
#endif

/*===========================================================================*/

#include    <nitro/types.h>
#include    <nitro/wm.h>


/*---------------------------------------------------------------------------*
    �萔��`
 *---------------------------------------------------------------------------*/
#define     WC_DMA_NO                   2       // GX����DMA�ԍ��Əd�Ȃ�Ȃ��悤����
#define     WC_DEFAULT_PORT             4       // �|�[�g�ԍ�( 4�`7 )
#define     WC_DEFAULT_PORT_PRIO        2       // �|�[�g�̗D��x( 0�`3 )
#define     WC_DS_PORT_NUMBER           12      // �f�[�^�V�F�A�p�|�[�g�ԍ�( 12�`15 )


/*---------------------------------------------------------------------------*
    �}�N����`
 *---------------------------------------------------------------------------*/
// 32�o�C�g�A���C���p
#define     WC_ROUNDUP_32B( x ) ( ( (x) + 31 ) & 0xffffffe0 )


/*---------------------------------------------------------------------------*
    �\���̒�`
 *---------------------------------------------------------------------------*/
typedef enum WCStatus
{
    WC_STATUS_READY = 0,               // ���������
    WC_STATUS_ERROR,                   // �ǂ��ɂ��Ȃ�Ȃ��G���[���
    WC_STATUS_BUSY,                    // ��ԑJ�ڒ��ɂ��r�W�[���
    WC_STATUS_PARENT,                  // �e�@�Ƃ��Đڑ��ςݏ��
    WC_STATUS_CHILD,                   // �q�@�Ƃ��Đڑ��ςݏ��
    WC_STATUS_MAX
}
WCStatus;

// �P��MP���[�h�p�̃R�[���o�b�N�֐��^��`
typedef void (*WCCallbackFunc) (u16 aid, u16 *data, u16 length);

// �u���b�N�]�����[�h�p�̃R�[���o�b�N�֐��^��`
// ���݂́A�ȉ��̎��_�� WMCallback �|�C���^���n�����
// �EWM_StartMP ������
// �EWcSetParentData,WcSetChildData ������
// �EWM_Reset ������
typedef void (*WCBlockTransferCallbackFunc) (WMCallback *arg);

// �s�N�g�`���b�g�T�[�`�p�̃R�[���o�b�N�֐��^��`
typedef void (*WCPictoCatchCallbackFunc) (WMBssDesc *pBssDesc);


/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/
void    WcInit(const WMParentParam *pp, BOOL dsFlag, BOOL btFlag, u16 framePeriod);     // �e�평�����B��Ƀ������m��
void    WcFinish(void);                // �������O�̏�Ԃɖ߂��B��Ƀ��������
void    WcStartParent(void);           // �e�@�Ƃ��Ă̐ڑ��J�n
void    WcStartChild(void);            // �q�@�Ƃ��Ă̐ڑ��J�n
void    WcEnd(void);                   // ��������Ԃւ̈ڍs�J�n
WCStatus WcGetStatus(void);            // WCStatus�^�̓�����Ԏ擾
u16     WcGetAid(void);                // AID�擾

// MP�ʐM�̃f�[�^��M���ɌĂяo���R�[���o�b�N�ݒ�
void    WcSetMpReceiveCallback(WCCallbackFunc cb);
// �u���b�N�]�����[�h�ݒ莞�ɌĂяo���R�[���o�b�N�ݒ�
void    WcSetBlockTransferCallback(WCBlockTransferCallbackFunc cb);
// �e�@�Ƃ��Ă̑��M�f�[�^�Z�b�g
void    WcSetParentData(const void *buf, u16 size);
// �q�@�Ƃ��Ă̑��M�f�[�^�Z�b�g
void    WcSetChildData(const void *buf, u16 size);

// �ڑ�������̃f�[�^�V�F�A�ʐM��i�߂�B1�Q�[���t���[���Ɉ��R�[��
BOOL    WcStepDataSharing(const void *send, u16 *bitmap);
// ��M�f�[�^����e�[���̃f�[�^�ւ̃|�C���^���擾����B
u8     *WcGetSharedDataAddress(u16 aid);

// �s�N�g�`���b�g�T�[�`�@�\��L���ɂ���
void    WcTurnOnPictoCatch(WCPictoCatchCallbackFunc func);
// �s�N�g�`���b�g�T�[�`�@�\�𖳌��ɂ���
void    WcTurnOffPictoCatch(void);


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* DEMOS_WIRELESS_SHARED_WC_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
