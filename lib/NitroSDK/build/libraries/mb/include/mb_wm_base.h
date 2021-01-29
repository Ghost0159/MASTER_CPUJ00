/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     mb_wm_base.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_wm_base.h,v $
  Revision 1.3  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.2  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.1  2005/02/21 00:40:57  yosizaki
  replace filename from tmp_lib_stub.

  Revision 1.14  2005/01/27 07:05:19  takano_makoto
  fix copyright header.

  Revision 1.13  2004/11/29 04:47:36  yosizaki
  allocate sendBuf. (avoid to share with SetMPData's buffer)

  Revision 1.12  2004/11/22 12:56:22  takano_makoto
  WM�̃X�e�[�^�X�o�b�t�@�擾�p�̍\���̃����o���폜

  Revision 1.11  2004/11/10 07:43:13  takano_makoto
  mb_gameinfo.c�̒��̎q�@���̏�������MBw�֐���r��

  Revision 1.10  2004/10/18 11:44:30  yosizaki
  add MB_StartParentEx, MB_EndEx.

  Revision 1.9  2004/10/05 09:45:29  terui
  MAC�A�h���X��u8�^�̔z��ɓ���B

  Revision 1.7  2004/09/22 09:42:11  sato_masaki
  IPL_branch 1.5.2.2�ƃ}�[�W

  Revision 1.6  2004/09/17 11:35:06  sato_masaki
  add function MBi_IsStarted()

  Revision 1.5  2004/09/15 06:25:01  sato_masaki
  MB_COMM_P_SENDLEN, MB_COMM_C_SENDLEN �̒�`�Ɉˑ����Ă���������ϐ����B

  Revision 1.4  2004/09/14 02:41:32  sato_masaki
  MBiParam �� mpBusy �ǉ��B

  Revision 1.2  2004/09/06 11:43:52  sato_masaki
  MBi_GetTgid()�֐��̒ǉ��B

  Revision 1.1  2004/09/03 07:06:28  sato_masaki
  �t�@�C�����@�\�ʂɕ����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef MB_WM_BASE_H_
#define MB_WM_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro.h>
#include <nitro/wm.h>

/* �ڑ��ő吔 */
#define MB_NUM_PARENT_INFORMATIONS              16

#define MB_MAX_SEND_BUFFER_SIZE         (0x400)

/*
 * �}���`�u�[�g�e / �q�̑I��.
 * (�q�� IPL �łȂ��Ǝ��s�ł��Ȃ�)
 */
#define MB_MODE_PARENT                          1
#define MB_MODE_CHILD                           2

/*
 * wm_lib �ŗL��`
 */

#define MB_CALLBACK_CHILD_CONNECTED             0
#define MB_CALLBACK_CHILD_DISCONNECTED          1
#define MB_CALLBACK_MP_PARENT_SENT              2
#define MB_CALLBACK_MP_PARENT_RECV              3
#define MB_CALLBACK_PARENT_FOUND                4
#define MB_CALLBACK_PARENT_NOT_FOUND            5
#define MB_CALLBACK_CONNECTED_TO_PARENT         6
#define MB_CALLBACK_DISCONNECTED                7
#define MB_CALLBACK_MP_CHILD_SENT               8
#define MB_CALLBACK_MP_CHILD_RECV               9
#define MB_CALLBACK_DISCONNECTED_FROM_PARENT    10
#define MB_CALLBACK_CONNECT_FAILED              11
#define MB_CALLBACK_DCF_CHILD_SENT              12
#define MB_CALLBACK_DCF_CHILD_SENT_ERR          13
#define MB_CALLBACK_DCF_CHILD_RECV              14
#define MB_CALLBACK_DISCONNECT_COMPLETE         15
#define MB_CALLBACK_DISCONNECT_FAILED           16
#define MB_CALLBACK_END_COMPLETE                17
#define MB_CALLBACK_MP_CHILD_SENT_ERR           18
#define MB_CALLBACK_MP_PARENT_SENT_ERR          19
#define MB_CALLBACK_MP_STARTED                  20
#define MB_CALLBACK_INIT_COMPLETE               21
#define MB_CALLBACK_END_MP_COMPLETE             22
#define MB_CALLBACK_SET_GAMEINFO_COMPLETE       23
#define MB_CALLBACK_SET_GAMEINFO_FAILED         24
#define MB_CALLBACK_MP_SEND_ENABLE              25
#define MB_CALLBACK_PARENT_STARTED              26
#define MB_CALLBACK_BEACON_LOST                 27
#define MB_CALLBACK_BEACON_SENT                 28
#define MB_CALLBACK_BEACON_RECV                 29
#define MB_CALLBACK_MP_SEND_DISABLE             30
#define MB_CALLBACK_DISASSOCIATE                31
#define MB_CALLBACK_REASSOCIATE                 32
#define MB_CALLBACK_AUTHENTICATE                33
#define MB_CALLBACK_SET_LIFETIME                34
#define MB_CALLBACK_DCF_STARTED                 35
#define MB_CALLBACK_DCF_SENT                    36
#define MB_CALLBACK_DCF_SENT_ERR                37
#define MB_CALLBACK_DCF_RECV                    38
#define MB_CALLBACK_DCF_END                     39
#define MB_CALLBACK_MPACK_IND                   40
#define MB_CALLBACK_MP_CHILD_SENT_TIMEOUT       41
#define MB_CALLBACK_SEND_QUEUE_FULL_ERR         42

#define MB_CALLBACK_API_ERROR                   255     // API�R�[���̖߂�l�ŃG���[
#define MB_CALLBACK_ERROR                       256


/*
 * �r�[�R���Ŏ�M�����X�̐e�@���. (�q�@�������ŊǗ�)
 */
typedef struct ParentInfo
{
    union
    {
        WMBssDesc parentInfo[1];
        u8      parentInfo_area[WM_BSS_DESC_SIZE] ATTRIBUTE_ALIGN(32);
    };
    /* ���� mac �� GameInfo �ȊO�S���g���Ă��Ȃ� */
    WMStartScanCallback scan_data;
    u8      reserved1[8];
}
ParentInfo;


/* MB �̃R�[���o�b�N�Ɏg�p����֐��̌`�� */
typedef void (*MBCallbackFunc) (u16 type, void *arg);


/*
 * wm_lib �p�����[�^�\����.
 * wm_lib - wm_tool �̋��n���������ꎞ�I�ɋ����A�������̂�,
 * union �̎g�����Ɋւ��� ANSI-STRICT �������Ă��Ȃ�����������.
 * (���ƂŒu������Ηǂ������̘b�Ȃ̂Ō��)
 */
typedef struct
{
    /* �e�@���ݒ�(�e�@���g�p) */
    union
    {
        WMParentParam parentParam;
        u8      parentParam_area[WM_PARENT_PARAM_SIZE] ATTRIBUTE_ALIGN(32);
    };

    /* StartMP �ɓn�� WM �����o�b�t�@. (SetMP �Ŏg���Ă͂Ȃ�Ȃ�) */
    u16     sendBuf[MB_MAX_SEND_BUFFER_SIZE / sizeof(u16)] ATTRIBUTE_ALIGN(32);

    /* �e�@���擾�o�b�t�@(�q�@���g�p) */
    union
    {
        WMBssDesc parentInfoBuf;
        u8      parentInfoBuf_area[WM_BSS_DESC_SIZE] ATTRIBUTE_ALIGN(32);
    };

    u16     p_sendlen;
    u16     p_recvlen;

    WMMpRecvBuf *recvBuf;              /* ��M�o�b�t�@ */

    /* �e�@ / �q�@ �R�[���o�b�N */
    void    (*callback_ptr) (void *arg);

    u8      mpBusy;                    /* MP���M���t���O */
    u8      mbIsStarted;
    u8      reserved0[10];

    u16     sendBufSize;               // ���M�o�b�t�@�̃T�C�Y
    u16     recvBufSize;               // MP��M�o�b�t�@�̃T�C�Y

    MBCallbackFunc callback;           // WM_lib�p�R�[���o�b�N
    const WMBssDesc *pInfo;            // �ڑ���e�@���|�C���^(�q�@�Ŏg�p)
    u16     mode;                      // MB_MODE_***
    u16     endReq;

    u16     mpStarted;                 // MP�J�n�ς݃t���O
    u16     child_bitmap;              // �q�@�ڑ���

    /* �A�����M�֘A */
    u16     contSend;                  // �A�����M���s�t���O(1:�A�����M, 0:1�t���[�����̒ʐM)
    u8      reserved1[2];

    // gameinfo�֘A
    u8      uname[WM_SIZE_USERNAME] ATTRIBUTE_ALIGN(4);
    u8      gname[WM_SIZE_GAMENAME] ATTRIBUTE_ALIGN(4);
    u16     currentTgid;               // �ڑ���e�@��TGID(BeaconRecv.Ind�Ń`�F�b�N����)
    u8      reserved2[22];

    u16     userGameInfo[((WM_SIZE_USER_GAMEINFO + 32) & ~(32 - 1)) /
                         sizeof(u16)] ATTRIBUTE_ALIGN(32);

    /* �q�@�ŗL�̃f�[�^ */
    struct
    {
        /*
         * �����ςݐe�@��.
         * �����l 0, StartScan ������ ++, StartConnect ���s�� 0.
         * ����͌���N�����Ă��Ȃ���,
         * BeconRecvState �ɂł�����Ă�����΃��[�U�ɂƂ��ĕ֗��ł��傤.
         */
        u16     found_parent_count;
        /*
         * ���g�Ɋ��蓖�Ă�ꂽAID;
         * �����l 0, StartConnect ������ n.
         * ����͌���N�����Ă��Ȃ�.
         */
        u16     my_aid;

        BOOL    scanning_flag;
        BOOL    scan_channel_flag;
        int     last_found_parent_no;

        u8      reserved8[16];

        /* �e�@ 16 �䕪�̏��z�� */
        ParentInfo parent_info[MB_NUM_PARENT_INFORMATIONS];
    };

}
MBiParam;


// ===============================================================================
// function

/* �Ō�Ɍ��������e���擾 */
int     MBi_GetLastFountParent(void);

/* �e�� BSS ���擾 */
WMBssDesc *MBi_GetParentBssDesc(int parent);

/* �ő�X�L�������Ԃ�ݒ� */
void    MBi_SetMaxScanTime(u16 time);

int     MBi_SendMP(const void *buf, int len, int pollbmp);

int     MBi_GetSendBufSize(void);

int     MBi_GetRecvBufSize(void);

int     MBi_CommConnectToParent(const WMBssDesc *bssDescp);

u32     MBi_GetGgid(void);

u16     MBi_GetTgid(void);

u8      MBi_GetAttribute(void);

int     MBi_RestartScan(void);

int     MBi_GetScanChannel(void);

u16     MBi_GetAid(void);

BOOL    MBi_IsStarted(void);

int     MBi_CommEnd(void);

void    MBi_CommParentCallback(u16 type, void *arg);
void    MBi_CommChildCallback(u16 type, void *arg);

#ifdef __cplusplus
}
#endif

#endif /*  MB_WM_BASE_H_    */
