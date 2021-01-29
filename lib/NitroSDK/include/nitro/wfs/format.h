/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WFS - include
  File:     format.h

  Copyright 2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
#ifndef	NITRO_WFS_FORMAT_H_
#define	NITRO_WFS_FORMAT_H_


#include <nitro/misc.h>
#include <nitro/types.h>
#include <nitro/platform.h>

#include <nitro/wbt/context.h>

#include <nitro/mi/allocator.h>
#include <nitro/mi/device.h>
#include <nitro/mi/endian.h>
#include <nitro/card.h>


#ifdef __cplusplus
extern  "C"
{
#endif


/*---------------------------------------------------------------------------*/
/* constants */

/* �����f�o�b�O�o�͐؂�ւ��t���O */
//#define	WFS_DEBUG_OUTPUT_ON

/* �����e�X�g�p�f�o�b�O�o�̓}�N�� */
#if	defined(WFS_DEBUG_OUTPUT_ON) && !defined(SDK_FINALROM)
#define	WFS_DEBUG_OUTPUT(formats)   \
    OS_TPrintf("WFS| ");            \
    OS_TPrintf formats ;            \
    OS_TPrintf("\n")
#else
#define	WFS_DEBUG_OUTPUT(...)	(void)0
#endif

/* WBT���b�Z�[�W�^�C�v     type     arg1     arg2  */
#define	WFS_MSG_LOCK_REQ    0   /* offset   length */
#define	WFS_MSG_LOCK_ACK    1   /*   ID      TRUE  */
                                /*    0      FALSE */
#define	WFS_MSG_UNLOCK_REQ  2   /*   ID      TRUE  */
#define	WFS_MSG_UNLOCK_ACK  3   /*   ID      TRUE  */

/* �\��u���b�NID */
#define	WFS_LOCKED_BLOCK_INDEX	 0x10001U
#define WFS_TABLE_BLOCK_INDEX    0x20000U

/* WFS�e�[�u���t�H�[�}�b�g */
typedef enum WFSTableRegionType
{
    WFS_TABLE_REGION_FAT,
    WFS_TABLE_REGION_FNT,
    WFS_TABLE_REGION_OV9,
    WFS_TABLE_REGION_OV7,
    WFS_TABLE_REGION_MAX
}
WFSTableRegionType;

/* WFS�C�x���g�ʒm */
typedef enum WFSEventType
{
    WFS_EVENT_SERVER_SEGMENT_REQUEST,
    WFS_EVENT_CLIENT_READY
}
WFSEventType;

#if (PLATFORM_BYTES_ENDIAN == PLATFORM_ENDIAN_LITTLE)
#define MI_LEToH32_BITFIELD(width, value) MI_LEToH32(value)
#define MI_HToLE32_BITFIELD(width, value) MI_HToLE32(value)
#else
#define MI_LEToH32_BITFIELD(width, value) (u32)(MI_LEToH32(value << (32 - width)))
#define MI_HToLE32_BITFIELD(width, value) (u32)(MI_HToLE32(value) >> (32 - width))
#endif


/*---------------------------------------------------------------------------*/
/* declarations */

/*---------------------------------------------------------------------------*
 * �d�g�ɏ��ʐM�t�H�[�}�b�g. (���ׂă��g���G���f�B�A��)
 *---------------------------------------------------------------------------*/

/* FAT�G���g����� */
typedef struct WFSFATFormat
{
    u32     top;
    u32     bottom;
} PLATFORM_STRUCT_PADDING_FOOTER
WFSFATFormat;

/* �I�[�o�[���C�G���g����� */
typedef struct WFSOVLFormat
{
    u32     id;
    u8     *ram_address;
    u32     ram_size;
    u32     bss_size;
    void  (*sinit_init);
    void  (*sinit_init_end);
    u32     file_id;
#if (PLATFORM_BITFIELDS_ENDIAN == PLATFORM_ENDIAN_LITTLE)
    u32     compressed:24;
    u32     flag:8;
#else
    u32     flag:8;
    u32     compressed:24;
#endif
} PLATFORM_STRUCT_PADDING_FOOTER
WFSOVLFormat;

/* ROM�t�@�C���e�[�u����� */
typedef struct WFSTableFormat
{
    u32             origin;
    u8             *buffer;
    u32             length;
    CARDRomRegion   region[WFS_TABLE_REGION_MAX];
} PLATFORM_STRUCT_PADDING_FOOTER
WFSTableFormat;


/* WFS_MSG_* ���b�Z�[�W�\���� */
typedef struct WFSMessageFormat
{
#if (PLATFORM_BITFIELDS_ENDIAN == PLATFORM_ENDIAN_LITTLE)
    u32     type:4;
    u32     packet_hi:4;
#else
    u32     packet_hi:4;
    u32     type:4;
#endif
    u32     arg2:24;
    u32     arg1;
    u8      packet_lo;
    u8      reserved[3];
} PLATFORM_STRUCT_PADDING_FOOTER
WFSMessageFormat;

PLATFORM_COMPILER_ASSERT(sizeof(WFSFATFormat) == 8);
PLATFORM_COMPILER_ASSERT(sizeof(WFSOVLFormat) == 32);
PLATFORM_COMPILER_ASSERT(sizeof(WFSMessageFormat) == 12);


/*---------------------------------------------------------------------------*
 * �d�g�ɏ��Ȃ����[�J���\����.
 *---------------------------------------------------------------------------*/

/* WFS�C�x���g�R�[���o�b�N */
typedef void (*WFSEventCallback)(void *context, WFSEventType, void *argument);

/* �p�P�b�g���\���� */
typedef struct WFSPacketBuffer
{
    u8     *buffer;         /* ���M�p�o�b�t�@�܂��͎�M�o�b�t�@ */
    int     length;         /* ���M�\�ő�T�C�Y�܂��͎�M�T�C�Y */
    int     bitmap;         /* AID �r�b�g�}�b�v */
}
WFSPacketBuffer;

/* �ڑ�����\���� */
typedef struct WFSPeerInfo
{
    int     aid;            /* AID */
    u8      mac[6];         /* MAC �A�h���X */
    u8      padding[2];
}
WFSPeerInfo;


/*---------------------------------------------------------------------------*/
/* functions */

/*---------------------------------------------------------------------------*
  Name:         WFS_LoadTable

  Description:  �f�o�C�X����NTR�o�C�i����ROM�t�@�C���e�[�u����ǂݍ���.

  Arguments:    table            ����������WFSTableFormat�\����.
                allocator        �����Ń������m�ۂɎg�p����A���P�[�^.
                device           FAT�̓ǂݍ��ݑΏۂ��i�[���ꂽ�f�o�C�X.
                fatbase          NTR�o�C�i�����z�u����Ă���f�o�C�X���I�t�Z�b�g.
                overlay          �}�[�W�������I�[�o�[���C���܂�NTR�o�C�i����
                                 �z�u����Ă���f�o�C�X���I�t�Z�b�g.
                                 (������ROM�t�@�C���e�[�u�����}�[�W���Ȃ��Ȃ�
                                  ���̒l��fatbase�Ɠ���)

  Returns:      FAT�𐳂������[�h�ł����TRUE.
 *---------------------------------------------------------------------------*/
BOOL WFS_LoadTable(WFSTableFormat *table, MIAllocator *allocator,
                   MIDevice *device, u32 fatbase, u32 overlay);

/*---------------------------------------------------------------------------*
  Name:         WFS_ParseTable

  Description:  NTR�o�C�i����ROM�t�@�C���e�[�u�����ǂݍ��܂ꂽ�����������.

  Arguments:    table            ����������WFSTableFormat�\����.
                                 �����obuffer��length�ɂ͑O������
                                 ROM�t�@�C���e�[�u�����ݒ肳��Ă���K�v������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void WFS_ParseTable(WFSTableFormat *table);

/*---------------------------------------------------------------------------*
  Name:         WFS_SendMessage

  Description:  WBT���[�U�f�[�^�R�}���h���g�p����WFS�ŗL���b�Z�[�W�𑗐M.

  Arguments:    context          WBTContext�\����.
                callback         �����R�[���o�b�N.
                bitmap           ���M��.
                type             ���b�Z�[�W���.
                arg1             ���b�Z�[�W�ŗL�̈���.
                arg2             ���b�Z�[�W�ŗL�̈���.

  Returns:      �����WBT�R�}���h�𔭍s�ł����TRUE.
 *---------------------------------------------------------------------------*/
PLATFORM_ATTRIBUTE_INLINE
BOOL WFS_SendMessage(WBTContext *context, WBTEventCallback callback, int bitmap,
                     int type, u32 arg1, u32 arg2)
{
    WFSMessageFormat message;
    int packet = WBT_GetParentPacketLength(context) + WBT_PACKET_SIZE_MIN;
    message.type = (u8)type;
    message.arg1 = MI_HToLE32(arg1);
    message.arg2 = MI_HToLE32_BITFIELD(24, arg2);
    message.packet_hi = (u8)(packet >> 8);
    message.packet_lo = (u8)(packet >> 0);
    return WBT_PostCommandMSG(context, (u16)bitmap, callback, &message, WBT_SIZE_USER_DATA);
}
PLATFORM_ATTRIBUTE_INLINE
BOOL WFS_SendMessageLOCK_REQ(WBTContext *context, WBTEventCallback callback, int bitmap,
                             u32 offset, u32 length)
{
    return WFS_SendMessage(context, callback, bitmap, WFS_MSG_LOCK_REQ, offset, length);
}
PLATFORM_ATTRIBUTE_INLINE
BOOL WFS_SendMessageLOCK_ACK(WBTContext *context, WBTEventCallback callback, int bitmap,
                             u32 id)
{
    return WFS_SendMessage(context, callback, bitmap, WFS_MSG_LOCK_ACK, id, (u32)(id != 0));
}
PLATFORM_ATTRIBUTE_INLINE
BOOL WFS_SendMessageUNLOCK_REQ(WBTContext *context, WBTEventCallback callback, int bitmap,
                               u32 id)
{
    return WFS_SendMessage(context, callback, bitmap, WFS_MSG_UNLOCK_REQ, id, TRUE);
}
PLATFORM_ATTRIBUTE_INLINE
BOOL WFS_SendMessageUNLOCK_ACK(WBTContext *context, WBTEventCallback callback, int bitmap,
                               u32 id)
{
    return WFS_SendMessage(context, callback, bitmap, WFS_MSG_UNLOCK_ACK, id, (u32)(id != 0));
}


/*---------------------------------------------------------------------------*/


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* NITRO_WFS_FORMAT_H_ */
/*---------------------------------------------------------------------------*
  $Log: format.h,v $
  Revision 1.4  2007/08/22 05:22:32  yosizaki
  fix dependency.

  Revision 1.3  2007/06/14 13:13:52  yosizaki
  disable debug output.

  Revision 1.2  2007/06/11 09:02:48  yosizaki
  delete WFS_EVENT_CLIENT_READ_COMPLETE.

  Revision 1.1  2007/04/13 04:14:18  yosizaki
  initial upload.

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
