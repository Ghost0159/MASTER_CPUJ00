/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_common.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_common.h,v $
  Revision 1.22  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.21  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.20  2005/02/18 11:56:03  yosizaki
  fix around hidden warnings.

  Revision 1.19  2005/01/27 07:05:51  takano_makoto
  fix copyright header.

  Revision 1.18  2005/01/27 07:04:12  takano_makoto
  small fix.

  Revision 1.17  2004/11/24 13:03:54  takano_makoto
  MBCommParentBlockHeader��MBCommChildBlockHeader��mb_block.h�ֈړ��AMBErrorCallback��ǉ�

  Revision 1.16  2004/11/22 12:54:59  takano_makoto
  MBCommRequestData��$(NITROSDK_ROOT)/include/mb/mb.h�ֈړ�

  Revision 1.15  2004/10/05 14:41:06  yosizaki
  export some MB_COMM_P_* range consts.

  Revision 1.14  2004/09/28 08:56:17  sato_masaki
  MB_COMM_P_SENDLEN_MIN�AMB_COMM_P_RECVLEN_MIN�̒�`�ǉ��B

  Revision 1.13  2004/09/20 12:54:11  sato_masaki
  MB_SENDFUNC_STATE_ERR��`��ǉ��B

  Revision 1.12  2004/09/18 12:00:38  sato_masaki
  �q�@���M�w�b�_��`�ɁAreserved��݂���B

  Revision 1.11  2004/09/15 06:51:08  sato_masaki
  USE_POLLBMP�̒�`���폜

  Revision 1.10  2004/09/15 06:20:15  sato_masaki
  - MB_COMM_P_SENDLEN, MB_COMM_C_SENDLEN �̒�`�Ɉˑ����Ă����Adefine��`���}�N�����B
  - MB_COMM_P_SENDLEN_DEFAULT, MB_COMM_P_RECVLEN_DEFAULT        (8)
    MB_COMM_P_SENDLEN_MAX, MB_COMM_P_RECVLEN_MAX ���`�B

  Revision 1.9  2004/09/14 13:19:50  sato_masaki
  ���N�G�X�g�f�[�^��f�Љ����đ���悤�ɕύX�B

  Revision 1.8  2004/09/13 13:06:29  sato_masaki
  remove member word from MB_CommBlockHeader

  Revision 1.7  2004/09/11 11:11:14  sato_masaki
  - MB_CommUserReq�^�Ƀ����o�ǉ��B
  - MBi_BlockHeaderBegin(), MBi_BlockHeaderEnd()�̈�����sendbuf��ǉ��B
  - MBi_DebugPrint()�ǉ��B

  Revision 1.6  2004/09/10 02:59:10  sato_masaki
  PRINT_DEBUG, CALLBACK_WM_STATE��`�Ɋւ���ύX�B

  Revision 1.5  2004/09/09 02:50:29  sato_masaki
  �f�o�b�O�֐��AMBi_comm_type_output()�̈����^�C�v��u16�ɕύX�B

  Revision 1.4  2004/09/08 04:35:12  sato_masaki
  MB_CommBlockHeader����Afileid���폜�B

  Revision 1.3  2004/09/04 07:04:11  sato_masaki
  small fix

  Revision 1.2  2004/09/04 07:01:28  sato_masaki
  MB_CommType�ɁAMB_COMM_TYPE_PARENT_MEMBER_FULL�ǉ��B

  Revision 1.1  2004/09/03 07:06:28  sato_masaki
  �t�@�C�����@�\�ʂɕ����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef __MB_COMMON_H__
#define __MB_COMMON_H__

/* macro definition -------------------------------------------------------- */

// for debug
#ifdef	PRINT_DEBUG
#define MB_COMM_TYPE_OUTPUT             MBi_comm_type_output
#define MB_COMM_WMEVENT_OUTPUT          MBi_comm_wmevent_output
#else
#define MB_COMM_TYPE_OUTPUT( ... )      ((void)0)
#define MB_COMM_WMEVENT_OUTPUT( ... )   ((void)0)
#endif

/* ����M�T�C�Y��` */
#define MB_COMM_P_SENDLEN_DEFAULT       (256)
#define MB_COMM_P_RECVLEN_DEFAULT       (8)

#define MB_COMM_P_SENDLEN_MAX           MB_COMM_PARENT_SEND_MAX
#define MB_COMM_P_RECVLEN_MAX           MB_COMM_PARENT_RECV_MAX

#define MB_COMM_P_SENDLEN_MIN           (sizeof(MBDownloadFileInfo))    // min size
#define MB_COMM_P_RECVLEN_MIN           MB_COMM_PARENT_RECV_MIN

#define MB_COMM_REQ_DATA_SIZE           (29)

/* �f�Љ����N�G�X�g�f�[�^�Ɋւ���v�Z�}�N�� */
#define MB_COMM_CALC_REQ_DATA_PIECE_SIZE( __P_RECVLEN__ )       (( __P_RECVLEN__ ) - 2)
#define MB_COMM_CALC_REQ_DATA_PIECE_NUM( __P_RECVLEN__ )        \
                                ( (MB_COMM_REQ_DATA_SIZE + 1) / (MB_COMM_CALC_REQ_DATA_PIECE_SIZE( __P_RECVLEN__ )) )
#define MB_COMM_CALC_REQ_DATA_BUF_SIZE( __P_RECVLEN__ )     (MB_COMM_REQ_DATA_SIZE + 1)
/*
 * ���_���I�ɊԈႢ������܂����AIPL�����̎d�l�œ��삵�Ă��܂��Ă���̂ŁA
 *   �C���͂ł��܂���B �{���Ȃ��
 * #define MB_COMM_CALC_REQ_DATA_PICE_NUM( __P_RECV_LEN__)                                                            \
 *                                  ( (MB_COMM_REQ_DATA_SIZE + MB_COMM_CALC_REQ_DATA_PIECE_SIZE( __P_RECVLEN__ ) - 1) \
 *                                    / MB_COMM_CALC_REQ_DATA_PIECE_SIZE( __P_RECVLEN__ ) )
 * #define MB_COMM_CALC_REQ_DATA_BUF_SIZE( __P_RECV_LEN__ )                                 \
 *                                  ( MB_COMM_CALC_REQ_DATA_PIECE_SIZE( __P_RECVLEN__ )     \
 *                                    * MB_COMM_CALC_REQ_DATA_PIECE_NUM( __P_RECVLEN__ ) )
 */


/* �u���b�N�w�b�_�̃T�C�Y */
#define MB_COMM_PARENT_HEADER_SIZE      (6)     // MBCommParentBlockHeader�̃T�C�Y(padding����)
#define MB_COMM_CHILD_HEADER_SIZE       (8)     // MBCommChildBlockHeader�̃T�C�Y(padding����)

#define MB_COMM_CALC_BLOCK_SIZE( __P_SENDLEN__ )                (( __P_SENDLEN__ ) - MB_COMM_PARENT_HEADER_SIZE)

/* �f�[�^���M�֐����Ԃ��G���[��`
   WM��ERRCODE�Ɣ��Ȃ��l�Œ�`���Ă��܂��B */
#define MB_SENDFUNC_STATE_ERR           (WM_ERRCODE_MAX + 1)

/* �u���b�N�]���f�[�^�^�C�v */
typedef enum MBCommType
{
    MB_COMM_TYPE_DUMMY = 0,            //  0

    MB_COMM_TYPE_PARENT_SENDSTART,     //  1
    MB_COMM_TYPE_PARENT_KICKREQ,       //  2
    MB_COMM_TYPE_PARENT_DL_FILEINFO,   //  3
    MB_COMM_TYPE_PARENT_DATA,          //  4
    MB_COMM_TYPE_PARENT_BOOTREQ,       //  5
    MB_COMM_TYPE_PARENT_MEMBER_FULL,   //  6

    MB_COMM_TYPE_CHILD_FILEREQ,        //  7
    MB_COMM_TYPE_CHILD_ACCEPT_FILEINFO, //  8
    MB_COMM_TYPE_CHILD_CONTINUE,       //  9
    MB_COMM_TYPE_CHILD_STOPREQ,        //  10
    MB_COMM_TYPE_CHILD_BOOTREQ_ACCEPTED //  11
}
MBCommType;

/* ���[�U�[����̃��N�G�X�g�^ */
typedef enum MBCommUserReq
{
    MB_COMM_USER_REQ_NONE = 0,         //  0
    MB_COMM_USER_REQ_DL_START,         //  1
    MB_COMM_USER_REQ_SEND_START,       //  2
    MB_COMM_USER_REQ_ACCEPT,           //  3
    MB_COMM_USER_REQ_KICK,             //  4
    MB_COMM_USER_REQ_BOOT              //  5
}
MBCommUserReq;

/* structure definition ---------------------------------------------------- */

/*
 * �G���[�ʒm�R�[���o�b�N
 */
typedef struct
{
    u16     apiid;                     // API�R�[�h
    u16     errcode;                   // �G���[�R�[�h
}
MBErrorCallback;


/* functions --------------------------------------------------------------- */

/*
 * �u���b�N�w�b�_����������, �^�C�v�����ݒ肷��.
 * ���̂��� MBi_BlockHeaderEnd() �ő��M����܂ł̊Ԃ�
 * ���܂��܂Ƃ����t�B�[���h�𖄂߂Ă���.
 * �������Ȃ���΂��̂܂܂ŗǂ�.
 */
void    MBi_BlockHeaderBegin(u8 type, u32 *sendbuf);

/*
 * �u���b�N�w�b�_�ݒ��������, �`�F�b�N�T�����v�Z����.
 * ���̌�, pollbmp �Ŏw�肵������֎��ۂɑ��M����.
 */
int     MBi_BlockHeaderEnd(int body_len, u16 pollbmp, u32 *sendbuf);


/*
 * �`�F�b�N�T���v�Z
 */
u16     MBi_calc_cksum(const u16 *buf, int length);

// --- for debug
void    MBi_DebugPrint(const char *file, int line, const char *func, const char *fmt, ...);
void    MBi_comm_type_output(u16 type);
void    MBi_comm_wmevent_output(u16 type, void *arg);

#endif /* __MB_COMMON_H__ */
