/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WFS - include
  File:     client.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
#ifndef	NITRO_WFS_CLIENT_H_
#define	NITRO_WFS_CLIENT_H_


#include <nitro/wfs/format.h>
#include <nitro/fs.h>


#ifdef __cplusplus
extern  "C"
{
#endif


/*---------------------------------------------------------------------------*/
/* constants */

/* �q�@����M������ő�̃t�@�C���T�C�Y (4M�o�C�g�Ƃ��Ă��� ) */
#define	WFS_FILE_SIZE_MAX	(4 * 1024 * 1024)


/*---------------------------------------------------------------------------*/
/* declarations */

/* ROM�ǂݍ��݃��N�G�X�g�����R�[���o�b�N */
struct WFSClientContext;
typedef void (*WFSRequestClientReadDoneCallback)(struct WFSClientContext *context, BOOL succeeded, void *arg);

/* WBT�N���C�A���g�R���e�L�X�g�\���� */
typedef struct WFSClientContext
{
    /* ��{�ݒ� */
    void                   *userdata;
    WFSEventCallback        callback;
    MIAllocator            *allocator;
    u32                     fat_ready:1;
    u32                     flags:31;
    /* WBT�Ǘ� */
    WBTContext              wbt[1];
    WBTCommandList          wbt_list[2];
    WBTRecvBufTable         recv_buf_table;
    WBTPacketBitmapTable    recv_buf_packet_bmp_table;
    WBTBlockInfoTable       block_info_table;
    WBTBlockInfo            block_info[16];
    /* ��M�r�b�g�Z�b�g�Ǘ� */
    u32                    *recv_pkt_bmp_buf;
    u32                     max_file_size;
    /* FAT�Ǘ� */
    WFSTableFormat          table[1];
    /* ���N�G�X�g�Ǘ� */
    u32                     block_id;
    CARDRomRegion           request_region;
    void                   *request_buffer;
    WFSRequestClientReadDoneCallback request_callback;
    void                   *request_argument;
    u8                      padding[12];
}
WFSClientContext;


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         WFS_InitClient

  Description:  WFS�N���C�A���g�R���e�L�X�g��������.

  Arguments:    context          WFSClientContext�\����.
                userdata         �R���e�L�X�g�Ɋ֘A�t����C�ӂ̃��[�U��`�l.
                callback         �V�X�e���C�x���g�ʒm�R�[���o�b�N.
                                 �s�v�Ȃ�NULL���w�肷��.
                allocator        �����Ŏg�p����A���P�[�^.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_InitClient(WFSClientContext *context,
                    void *userdata, WFSEventCallback callback,
                    MIAllocator *allocator);

/*---------------------------------------------------------------------------*
  Name:         WFS_StartClient

  Description:  WFS�N���C�A���g�R���e�L�X�g�̒ʐM���J�n.

  Arguments:    context          WFSClientContext�\����.
                peer             ���g�̐ڑ����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_StartClient(WFSClientContext *context, const WFSPeerInfo *peer);

/*---------------------------------------------------------------------------*
  Name:         WFS_EndClient

  Description:  WFS�N���C�A���g�R���e�L�X�g�����.

  Arguments:    context          WFSClientContext�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_EndClient(WFSClientContext *context);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientConnectHook

  Description:  �N���C�A���g���̐ڑ��ʒm.

  Arguments:    context          WFSClientContext�\����.
                peer             �ڑ������ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallClientConnectHook(WFSClientContext *context, const WFSPeerInfo *peer);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientDisconnectHook

  Description:  �N���C�A���g���̐ؒf�ʒm.

  Arguments:    context          WFSClientContext�\����.
                peer             �ؒf�����ʐM��̏��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_CallClientDisconnectHook(WFSClientContext *context, const WFSPeerInfo *peer);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientPacketSendHook

  Description:  �N���C�A���g���̃p�P�b�g���M�\�^�C�~���O�ʒm.

  Arguments:    context          WFSClientContext�\����.
                packet           ���M�p�P�b�g�ݒ�.

  Returns:      ���ۂ̃p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
void WFS_CallClientPacketSendHook(WFSClientContext *context, WFSPacketBuffer *packet);

/*---------------------------------------------------------------------------*
  Name:         WFS_CallClientPacketRecvHook

  Description:  �N���C�A���g���̃p�P�b�g��M�^�C�~���O�ʒm.

  Arguments:    context          WFSClientContext�\����.
                packet           ���M���p�P�b�g���.

  Returns:      ���ۂ̃p�P�b�g�T�C�Y.
 *---------------------------------------------------------------------------*/
void WFS_CallClientPacketRecvHook(WFSClientContext *context, const WFSPacketBuffer *packet);

/*---------------------------------------------------------------------------*
  Name:         WFS_IsClientReady

  Description:  �N���C�A���g���̏������������Ă��邩����.
                WFS_EVENT_CLIENT_READY�ʒm�ȍ~��TRUE��Ԃ�.

  Arguments:    context          WFSClientContext�\����.

  Returns:      �N���C�A���g���̏������������Ă����TRUE.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
BOOL WFS_IsClientReady(const WFSClientContext *context)
{
    return context->fat_ready ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_GetTableFormat

  Description:  �T�[�o�����M����ROM�t�@�C���e�[�u�����擾.
                WFS_EVENT_CLIENT_READY�ʒm�ȍ~�ł̂ݗL���Ȓl�𓾂�.

  Arguments:    context          WFSClientContext�\����.

  Returns:      ROM�t�@�C���e�[�u���܂���NULL.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
const WFSTableFormat *WFS_GetTableFormat(const WFSClientContext *context)
{
    return WFS_IsClientReady(context) ? context->table : NULL;
}

/*---------------------------------------------------------------------------*
  Name:         WFS_RequestClientRead

  Description:  �T�[�o��ROM�C���[�W�ǂݍ��ݗv�����J�n.

  Arguments:    context          WFSClientContext�\����.
                buffer           �ǂݍ��݃f�[�^�̊i�[�惁����.
                offset           �f�o�C�X�̓ǂݍ��݊J�n�ʒu.
                length           �ǂݍ��݃T�C�Y.
                callback         �ǂݍ��݊����R�[���o�b�N.
                                 �s�v�Ȃ�NULL.
                arg              �ǂݍ��݊����R�[���o�b�N�ɗ^�������.
  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_RequestClientRead(WFSClientContext *context, void *buffer, u32 offset,
                           u32 length, WFSRequestClientReadDoneCallback callback,
                           void *arg);

/*---------------------------------------------------------------------------*
  Name:         WFS_GetClientReadProgress

  Description:  ROM�C���[�W�ǂݍ��ݗv���̐i���󋵂��擾.

  Arguments:    context          WFSClientContext�\����.
                current          ��M�ς݃p�P�b�g�����擾����ϐ�.
                total            ��M�\��̃p�P�b�g�������擾����ϐ�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_GetClientReadProgress(WFSClientContext *context, int *current, int *total);

/*---------------------------------------------------------------------------*
  Name:         WFS_ReplaceRomArchive

  Description:  WFS��ROM�t�@�C���e�[�u����"rom"�̃A�[�J�C�u�Ƀ}�E���g.

  Arguments:    context          WFSClientContext�\����.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_ReplaceRomArchive(WFSClientContext *context);


/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_WFS_CLIENT_H_ */
/*---------------------------------------------------------------------------*
  $Log: client.h,v $
  Revision 1.3  2007/08/22 05:22:32  yosizaki
  fix dependency.

  Revision 1.2  2007/06/11 09:02:30  yosizaki
  add some functions.

  Revision 1.1  2007/04/13 04:14:18  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
