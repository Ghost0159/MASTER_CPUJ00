#ifndef __WC_H_
#define __WC_H_

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
//  32�o�C�g�A���C���p 
//  For 32-byte alignment 
#define     WC_ROUNDUP_32B( x ) ( ( x + 31 ) & 0xffffffe0 )


/*---------------------------------------------------------------------------*
     �\���̒�` 
 *---------------------------------------------------------------------------*/
typedef enum WCStatus
{
    WC_STATUS_READY = 0,               //  ���������  
    WC_STATUS_ERROR,                   //  �ǂ��ɂ��Ȃ�Ȃ��G���[���  
    WC_STATUS_BUSY,                    //  ��ԑJ�ڒ��ɂ��r�W�[���  
    WC_STATUS_PARENT,                  //  �e�@�Ƃ��Đڑ��ςݏ��  
    WC_STATUS_CHILD,                   //  �q�@�Ƃ��Đڑ��ςݏ��  
    WC_STATUS_MAX
}
WCStatus;

//  �P��MP���[�h�p�̃R�[���o�b�N�֐��^��` 
typedef void (*WCCallbackFunc) (u16 aid, u16 *data, u16 length);

// �u���b�N�]�����[�h�p�̃R�[���o�b�N�֐��^��`
// ���݂́A�ȉ��̎��_�� WMCallback �|�C���^���n�����
// �EWM_StartMP ������
// �EWcSetParentData,WcSetChildData ������
// �EWM_Reset ������
typedef void (*WCBlockTransferCallbackFunc) (WMCallback *arg);

//  �s�N�g�`���b�g�T�[�`�p�̃R�[���o�b�N�֐��^��` 
typedef void (*WCPictoCatchCallbackFunc) (WMBssDesc *pBssDesc);


/*---------------------------------------------------------------------------*
    �֐���`
 *---------------------------------------------------------------------------*/
void WcInit(const WMParentParam *pp, BOOL dsFlag, BOOL btFlag, u16 framePeriod);        // �e�평�����B��Ƀ������m�� 
void WcFinish(void);                   // �������O�̏�Ԃɖ߂��B��Ƀ�������� 
void WcStartParent(void);              // �e�@�Ƃ��Ă̐ڑ��J�n 
void WcStartChild(void);               // �q�@�Ƃ��Ă̐ڑ��J�n 
void WcEnd(void);                      // ��������Ԃւ̈ڍs�J�n 
WCStatus WcGetStatus(void);            // WCStatus�^�̓�����Ԏ擾 
u16 WcGetAid(void);                    // AID�擾 

// MP�ʐM�̃f�[�^��M���ɌĂяo���R�[���o�b�N�ݒ�
void WcSetMpReceiveCallback(WCCallbackFunc cb);
// �u���b�N�]�����[�h�ݒ莞�ɌĂяo���R�[���o�b�N�ݒ�
void WcSetBlockTransferCallback(WCBlockTransferCallbackFunc cb);
// �e�@�Ƃ��Ă̑��M�f�[�^�Z�b�g
void WcSetParentData(const void *buf, u16 size);
// �q�@�Ƃ��Ă̑��M�f�[�^�Z�b�g
void WcSetChildData(const void *buf, u16 size);

// �ڑ�������̃f�[�^�V�F�A�ʐM��i�߂�B1�Q�[���t���[���Ɉ��R�[��
BOOL WcStepDataSharing(const void *send, u16 *bitmap);
// ��M�f�[�^����e�[���̃f�[�^�ւ̃|�C���^���擾����B
u8 *WcGetSharedDataAddress(u16 aid);

// �s�N�g�`���b�g�T�[�`�@�\��L���ɂ���
void WcTurnOnPictoCatch(WCPictoCatchCallbackFunc func);
// �s�N�g�`���b�g�T�[�`�@�\�𖳌��ɂ���
void WcTurnOffPictoCatch(void);


/*===========================================================================*/

#ifdef  __cplusplus
}       /* extern "C" */
#endif

#endif /* DEMOS_WIRELESS_SHARED_WC_H_ */

