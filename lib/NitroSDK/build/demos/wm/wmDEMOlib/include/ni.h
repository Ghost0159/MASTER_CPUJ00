/*---------------------------------------------------------------------------*
  Project:  NitroSDK - WM - demos - wmDEMOlib
  File:     ni.h

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: ni.h,v $
  Revision 1.8  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.7  2005/02/28 05:26:27  yosizaki
  do-indent.

  Revision 1.6  2005/02/18 14:23:18  seiki_masashi
  Copyright 表記の修正

  Revision 1.5  2005/02/18 13:48:06  seiki_masashi
  warning 対策

  Revision 1.4  2004/07/27 09:26:02  ikedae
  (none)

  Revision 1.3  2004/07/27 09:21:42  ikedae
  paddingあわせ

  Revision 1.2  2004/07/27 08:07:25  yasu
  Add #pragma  warn_padding off

  Revision 1.1  2004/07/23 15:13:50  terui
  整理して新規追加

  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#ifndef	WMDEMOLIB_NI_H_
#define	WMDEMOLIB_NI_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*===========================================================================*/


#define NI_MODE_PARENT      0
#define NI_MODE_CHILD       1

#define NI_MAX_FILE         4
#define NI_FILENAME_SIZE    12

#define NI_TYPE_DATA        100
#define NI_TYPE_FILEINFO    101
#define NI_TYPE_REQUEST     102
#define NI_TYPE_CONTINUE    103
#define NI_TYPE_STOPREQUEST 104

#define NI_SEND_THRESHOLD   2          // sendのとき2blockさきまで送る
#define NI_MAX_CHILD        15

typedef struct NI_STATUS
{
    u8      fileid;
    u8      num_of_children;
    u8      mode;
    u8      cur_id;
    u16     cksum_err;
    u16     send_err;
    u16     recv_err;
    u16     cur_block[NI_MAX_FILE];    // 親の時に使用する
    u16     next_send[NI_MAX_FILE];    // 親の時に使用する
    u16     dummy1;
    BOOL    busyflag[NI_MAX_FILE];     // 親の時に使用する
    s8      fileid_of_child[NI_MAX_CHILD];
    u8      dummy2;
    u16     got_block;                 // 子
    u16     dummy3;
    u32     total_length;              // 子
    u8     *data_addr;                 // 子
}
NI_STATUS;


typedef struct NI_Header
{
    u8      type;
    u8      fileid;
    u16     seqno;                     // 0から増えていく
    u16     length;                    // パケット内の大きさ
    u16     cksum;
    u16     next_block;
}
NI_Header;


#define NI_HEADER_SIZE  sizeof (NI_Header)

#if 1
#define NI_P_SENDLEN        510        // max size
#else
#define NI_P_SENDLEN        256
#endif

#define NI_C_SENDLEN        sizeof(NI_Header)

#define NI_BUFSIZE      (NI_P_SENDLEN - NI_HEADER_SIZE)

//----------------------------------------------------
#define NI_GAME_INFO_TYPE_TEXT			0xaa00
typedef struct NI_GAME_INFO
{
    u16     game_info_type;
    u16     filenum;
    struct
    {
        char    filename[NI_FILENAME_SIZE];
    }
    fileinfo[NI_MAX_FILE];
    u16     dummy[10];                 // 32 バイトアラインのため
}
NI_GAME_INFO;

typedef struct NI_INFO
{
    u32     filenum;
    struct NI_FILE_INFO
    {
        char    filename[NI_FILENAME_SIZE];
        u32     filesize;
        u8     *addr;
        BOOL    compress_flag;
    }
    fileinfo[NI_MAX_FILE];
}
NI_INFO;

typedef void (*NI_CALLBACK) (u32 status);

void    NI_Initialize(u16 mode, WM_lib_param * lptr);
void    NI_GetStatus(NI_STATUS * stat);

void    NI_End(void);

int     NI_Proceed(void);

int     NI_RegisterFile(char *filename, BOOL compress_flag, u8 *addr, u32 size);

void    NI_GetInfo(NI_INFO * ni_info);
int     NI_RequestFile(u8 fileid, u8 *addr, u32 size, NI_CALLBACK ni_callback);


/*===========================================================================*/

#ifdef	__cplusplus
}          /* extern "C" */
#endif

#endif /* WMDEMOLIB_NI_H_ */

/*---------------------------------------------------------------------------*
  End of file
 *---------------------------------------------------------------------------*/
