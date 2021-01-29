/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - include
  File:     mb_wm.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_block.h,v $
  Revision 1.5  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.4  2005/03/01 01:57:00  yosizaki
  copyright �̔N���C��.

  Revision 1.3  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.2  2004/11/24 13:03:22  takano_makoto
  MBCommParentBlockHeader��MBCommChildBlockHeader��mb_common.h����ړ�

  Revision 1.1  2004/11/22 12:47:52  takano_makoto
  Initial update.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef _MB_BLOCK_H_
#define _MB_BLOCK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/types.h>
#include <nitro/mb.h>
#include <nitro/wm.h>
#include "mb_common.h"


/* ----------------------------------------------------------------------- *
 *  �\���̒�`
 * ----------------------------------------------------------------------- */

/*
 * �]���u���b�N�w�b�_(�e�u���b�N�ɕt������w�b�_)
   ���ۂɂ�Padding�����l�߂��`�ő���M���܂��B
 */
typedef struct
{
    u8      type;                      // �f�[�^�^�C�v                              1B
    u8      pad1[1];
    u16     fid;                       // �t�@�C���ԍ��l                            4B
    u16     seqno;                     // �e�@�g�p                                  6B
}
MBCommParentBlockHeader;


typedef struct
{
    u8      type;                      // �f�[�^�^�C�v                             1B
    u8      pad1[1];                   //                                          1B
    union                              //                              union�̍��v 16B
    {
        struct
        {
            u16     req;               // �u���b�N�ԍ��v���l                       2B
            u8      reserved[MB_COMM_CALC_REQ_DATA_PIECE_SIZE(MB_COMM_P_RECVLEN_MAX)];  // 14B
        }
        data;

        struct
        {
            u8      piece;             //  �f�Љ����ꂽ�f�[�^�̔ԍ�                1B
            //  �f�Љ����ꂽ�f�[�^���i�[����o�b�t�@
            u8      data[MB_COMM_CALC_REQ_DATA_PIECE_SIZE(MB_COMM_P_RECVLEN_MAX)];      //  14B
            u8      pad2[1];           //  1B
        }
        req_data;
    };
    // �v18B
}
MBCommChildBlockHeader;


// �������N�G�X�g�̎�M�o�b�t�@
typedef struct
{
    u32     data_buf[MB_MAX_CHILD][MB_COMM_CALC_REQ_DATA_BUF_SIZE(MB_COMM_P_RECVLEN_MAX) /
                                   sizeof(u32) + 1];
    u32     data_bmp[MB_MAX_CHILD];
}
MbRequestPieceBuf;

/* ----------------------------------------------------------------------- *
 *  �֐��錾
 * ----------------------------------------------------------------------- */

/* �q�@MP���M�T�C�Y�̐ݒ� */
void    MBi_SetChildMPMaxSize(u16 childMaxSize);
/* �e�@�ł̕������N�G�X�g��M�p�o�b�t�@�̐ݒ� */
void    MBi_SetParentPieceBuffer(MbRequestPieceBuf * buf);
/* �������N�G�X�g��M�p�o�b�t�@���N���A */
void    MBi_ClearParentPieceBuffer(u16 child_aid);

/* �e�@�̑��M�w�b�_��񂩂�A���ۂɑ��M����f�[�^���\�z */
u8     *MBi_MakeParentSendBuffer(const MBCommParentBlockHeader * hdr, u8 *sendbuf);
/* �q�@�����M�����f�[�^�o�b�t�@����\���̂��\�z */
u8     *MBi_SetRecvBufferFromChild(const u8 *recvbuf, MBCommChildBlockHeader * hdr, u16 child_id);

/* �q�@�̑��M�w�b�_��񂩂�A���ۂɑ��M����f�[�^���\�z */
u8     *MBi_MakeChildSendBuffer(const MBCommChildBlockHeader * hdr, u8 *sendbuf);
/* �q�@���瑗�M���郊�N�G�X�g�f�[�^�𕪊����܂��B */
u8      MBi_SendRequestDataPiece(u8 *pData, const MBCommRequestData *pReq);
/* �e�@�����M�����p�P�b�g����w�b�_�����擾���A�f�[�^���ւ̃|�C���^��Ԃ��܂��B */
u8     *MBi_SetRecvBufferFromParent(MBCommParentBlockHeader * hdr, const u8 *recvbuf);


#ifdef __cplusplus
}
#endif

#endif /*  _MB_BLOCK_H_    */
