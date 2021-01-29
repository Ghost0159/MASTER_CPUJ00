/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WFS - include
  File:     server.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
#ifndef	NITRO_WFS_SERVER_H_
#define	NITRO_WFS_SERVER_H_


#include <nitro/wfs/format.h>
#include <nitro/mi/device.h>
#include <nitro/fs.h>


#ifdef __cplusplus
extern  "C"
{
#endif


/*---------------------------------------------------------------------------*/
/* constants */

/* �]�����b�N�n���h������ (�ő哯���ڑ��N���C�A���g����������Ώ[��) */
#define	WFS_LOCK_HANDLE_MAX	    15


/*---------------------------------------------------------------------------*/
/* declarations */

/* WFS_EVENT_SERVER_SEGMENT_REQUEST�C�x���g�Œʒm�������\���� */
typedef struct WFSSegmentBuffer
{
    u32     offset;
    u32     length;
    void   *buffer;
}
WFSSegmentBuffer;

/* �]�����b�N�͈͏��\���� */
typedef struct WFSLockInfo
{
    WBTBlockInfoList info;
    int     ref;        /* �Q�ƃJ�E���g */
    u32     offset;     /* ROM �擪�A�h���X */
    u32     length;     /* ROM ���[�h�T�C�Y */
    u32     ack_seq;    /* ���ɕԂ��ׂ��V�[�P���X�ԍ� */
}
WFSLockInfo;

/* WFS�T�[�o���\���� */
typedef struct WFSServerContext
{
    void               *userdata;
    WFSEventCallback    callback;

    /* �����X���b�h��� */
    void               *thread_work;
    void (*thread_hook)(void *thread, void *argument);

    /* ���b�N�̈���Ǘ����郊�X�g */
    int                 use_bitmap;
    WFSLockInfo         list[WFS_LOCK_HANDLE_MAX];

    /* ROM�t�@�C���e�[�u�� */
    WFSTableFormat      table[1];
    WBTBlockInfoList    table_info[1];
    MIAllocator        *allocator;

    /* �N���C�A���g��ԊǗ������o */
    int                 all_bitmap;     /* �ڑ��� */
    int                 busy_bitmap;    /* �A�N�Z�X�� */
    int                 sync_bitmap;    /* ������ */
    int                 ack_bitmap;     /* �T�[�o����̉����҂� */
    WFSMessageFormat    recv_msg[16];   /* �ŐV�̃��b�Z�[�W */

    /* �p�P�b�g�T�C�Y���I�ύX�Ɋւ��郁���o */
    BOOL                is_changing;    /* �ύX�\�蒆�t���O */
    int                 new_packet;     /* �ύX��̃p�P�b�g�T�C�Y */
    int                 deny_bitmap;	/* �ύX�ɔ������ۉ����Ώ� */

    /* WBT�֘A�����o */
    u8                  cache_hit_buf[512];
    WBTContext          wbt[1];
    WBTCommandList      wbt_list[2];
    BOOL                msg_busy;       /* �������b�Z�[�W���M�� */
}
WFSServerContext;


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         WFS_InitServer

  Description:  WFS�T�[�o�R���e�L�X�g��������.

  Arguments:    context          WFSServerContext�\����.
                userdata         �R���e�L�X�g�Ɋ֘A�t����C�ӂ̃��[�U��`�l.
                callback         �V�X�e���C�x���g�ʒm�R�[���o�b�N.
                                 �s�v�Ȃ�NULL���w�肷��.
                allocator        �����Ŏg�p����A���P�[�^.
                packet           �e�@�̏����p�P�b�g�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_InitServer(WFSServerContext *context,
                    void *userdata, WFSEventCallback callback,
                    MIAllocator *allocator, int packet);

/*---------------------------------------------------------------------------*
  Name:         WFS_EndServer

  Description:  WFS�T�[�o�R���e�L�X�g�����.

  Arguments:    context          WFSServerContext�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_EndServer(WFSServerContext *context);

/*---------------------------------------------------------------------------*
  Name:         WFS_RegisterServerTable

  Description:  �f�o�C�X����ROM�t�@�C���e�[�u�������[�h���ăT�[�o�ɓo�^.

  Arguments:    context          WFSServerContext�\����.
                device           NTR�o�C�i�����i�[����Ă���f�o�C�X.
                fatbase          NTR�o�C�i�����z�u����Ă���f�o�C�X���I�t�Z�b�g.
                overlay          �}�[�W�������I�[�o�[���C���܂�NTR�o�C�i����
                                 �z�u����Ă���f�o�C�X���I�t�Z�b�g.
                                 (������ROM�t�@�C���e�[�u�����}�[�W���Ȃ��Ȃ�
                                  ���̒l��fatbase�Ɠ���)

  Returns:      ROM�t�@�C���e�[�u���𐳂������[�h���o�^�ł����TRUE.
 *---------------------------------------------------------------------------*/
BOOL WFS_RegisterServerTable(WFSServerContext *context,
                             MIDevice *device, u32 fatbase, u32 overlay);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerConnectHook

  Description:  �T�[�o���̐ڑ��ʒm.

  Arguments:    context          WFSServerContext�\����.
                peer             �ڑ������ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerConnectHook(WFSServerContext *context, const WFSPeerInfo *peer);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerDisconnectHook

  Description:  �T�[�o���̐ؒf�ʒm.

  Arguments:    context          WFSServerContext�\����.
                peer             �ؒf�����ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerDisconnectHook(WFSServerContext *context, const WFSPeerInfo *peer);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerPacketSendHook

  Description:  �T�[�o���̃p�P�b�g���M�\�^�C�~���O�ʒm.

  Arguments:    context          WFSServerContext�\����.
                packet           ���M�p�P�b�g�ݒ�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerPacketSendHook(WFSServerContext *context, WFSPacketBuffer *packet);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallServerPacketRecvHook

  Description:  �T�[�o���̃p�P�b�g��M�ʒm.

  Arguments:    context          WFSServerContext�\����.
                packet           ���M���p�P�b�g���.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallServerPacketRecvHook(WFSServerContext *context, const WFSPacketBuffer *packet);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetServerConnectedBitmap

  Description:  ���݃T�[�o�ɐڑ����̃N���C�A���g�Q�̃r�b�g�}�b�v���擾.

  Arguments:    context          WFSServerContext�\����.

  Returns:      ���݃T�[�o�ɐڑ����̃N���C�A���g�Q.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
int WFS_GetServerConnectedBitmap(const WFSServerContext *context)
{
    return context->all_bitmap;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetServerBusyBitmap

  Description:  ���݃T�[�o�ɃA�N�Z�X���̃N���C�A���g�Q�̃r�b�g�}�b�v���擾.

  Arguments:    context          WFSServerContext�\����.

  Returns:      ���݃T�[�o�ɃA�N�Z�X���̃N���C�A���g�Q.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
int WFS_GetServerBusyBitmap(const WFSServerContext *context)
{
    return context->busy_bitmap;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetServerSyncBitmap

  Description:  ���݃T�[�o���A�N�Z�X�������Ă���N���C�A���g�Q�̃r�b�g�}�b�v���擾.

  Arguments:    context          WFSServerContext�\����.

  Returns:      ���݃T�[�o���A�N�Z�X�������Ă���N���C�A���g�Q.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
int WFS_GetServerSyncBitmap(const WFSServerContext *context)
{
    return context->sync_bitmap;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetServerPacketLength

  Description:  �T�[�o���M�p�P�b�g�T�C�Y���擾.

  Arguments:    context          WFSServerContext�\����.

  Returns:      ���ݐݒ肳��Ă���p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
int WFS_GetServerPacketLength(const WFSServerContext *context);

/*---------------------------------------------------------------------------*
  Name:         WFS_SetServerPacketLength

  Description:  �T�[�o���M�p�P�b�g�T�C�Y��ݒ�.

  Arguments:    context          WFSServerContext�\����.
                length           �ݒ肷��p�P�b�g�T�C�Y.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_SetServerPacketLength(WFSServerContext *context, int length);

/*---------------------------------------------------------------------------*
  Name:         WFS_SetServerSync

  Description:  �T�[�o���ŃA�N�Z�X���������N���C�A���g�Q��ݒ�.
                ���̊֐���, ����̃t�@�C���Q�𓯂������ŃA�N�Z�X���邱�Ƃ�
                ���炩�ɕۏ؂���Ă���N���C�A���g�Q�։����𓯊�����ɂ��
                WBT���C�u�����̓������������������I�ȓ]������������.
                ������, �����J�n�̃^�C�~���O���_���I�Ɉ��S�łȂ��ꍇ
                ����������ăf�b�h���b�N����_�ɒ��ӂ���K�v������.

  Arguments:    context          WFSServerContext�\����.
                bitmap           ����������N���C�A���g��AID�r�b�g�}�b�v.
                                 0���w�肷��Ɠ����͍s���Ȃ�. (�f�t�H���g)
                                 �ŉ��ʃr�b�g 1 �͏�ɖ��������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_SetServerSync(WFSServerContext *context, int bitmap);

/*---------------------------------------------------------------------------*
  Name:         WFS_ExecuteRomServerThread

  Description:  �w���ROM�t�@�C����z�M����悤WFS���C�u�����֓o�^��,
                ������ROM�A�N�Z�X�p�̃X���b�h�������I�ɋN������.
                ���̃X���b�h��WFS_EndServer�֐��̓����Ŏ����I�ɔj�������.

  Arguments:    context          WFSServerContext�\����.
                file             �z�M����t�@�C���V�X�e��������SRL�t�@�C��.
                                 �N���[���u�[�g�Ȃ�NULL���w�肷��.
                sharedFS         �t�@�C���V�X�e����e�q�ŋ��L������Ȃ�TRUE.
                                 ���̏ꍇ, �e�@���̂̎��t�@�C���V�X�e����
                                 file�̎��I�[�o�[���C�����𒊏o���Ēǉ�����
                                 �����t�@�C���V�X�e����z�M����.
                                 file��NULL���w�肵���ꍇ�̓N���[���u�[�g�Ȃ̂�
                                 ���̈����͖��������. (���TRUE�Ɖ��߂����)


  Returns:      ROM�t�@�C���̓o�^�ƃX���b�h�̐����ɐ��������TRUE.
 *---------------------------------------------------------------------------*/
BOOL WFS_ExecuteRomServerThread(WFSServerContext *context, FSFile *file, BOOL sharedFS);


/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_WFS_SERVER_H_ */
/*---------------------------------------------------------------------------*
  $Log: server.h,v $
  Revision 1.5  2007/08/22 05:22:32  yosizaki
  fix dependency.

  Revision 1.4  2007/06/14 13:14:10  yosizaki
  add WFS_ExecuteRomServerThread.

  Revision 1.3  2007/06/11 09:03:37  yosizaki
  add WFS_GetServerPacketLength().

  Revision 1.2  2007/04/17 00:00:52  yosizaki
  rename some structures.

  Revision 1.1  2007/04/13 04:14:18  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

