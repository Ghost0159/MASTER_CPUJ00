/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - mcs
  File:     base.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.25 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_MCS_BASE_H_
#define NNS_MCS_BASE_H_

#include <nitro/types.h>
#include <nnsys/fnd/list.h>
#include <isdbglib.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================
    �萔��`
   ======================================================================== */

#define NNS_MCS_WORKMEM_SIZE        sizeof(NNSiMcsWork)


/* =======================================================================
    �^��`
   ======================================================================== */


// �f�o�C�X���
typedef struct NNSMcsDeviceCaps NNSMcsDeviceCaps;
struct NNSMcsDeviceCaps
{
    u32         deviceID;           // �f�o�C�X����ID
    u32         maskResource;       // ���̃f�o�C�X�𓮍삳����̂ɕK�v�Ȃ�\�[�X
};


// ��M���̃R�[���o�b�N�֐��̌^
typedef void (*NNSMcsRecvCallback)(const void* recv, u32 recvSize, u32 userData, u32 offset, u32 totalSize);


// ��M���̃R�[���o�b�N�֐��̏��
typedef struct NNSMcsRecvCBInfo NNSMcsRecvCBInfo;
struct NNSMcsRecvCBInfo
{
    u32                 channel;
    NNSMcsRecvCallback  cbFunc;
    u32                 userData;
    NNSFndLink          link;
};

// MCS�p���[�N������
typedef struct NNSiMcsWork NNSiMcsWork;
struct NNSiMcsWork
{
    u8                  bProtocolError;                     // mcs���C�u�������x���̕s��v
    u8                  bLengthEnable;                      // length���擾���āA0�ȊO�̒l�𓾂���ΐ^
    u8                  bHostDataRecived;                   // �ŏ��Ƀz�X�g����f�[�^����M�ł�����^
    u8                  padding;

    OSMutex             mutex;
    NNSFndList          recvCBInfoList;                     // ��M���R�[���o�b�N�֐����X�g

    NNSiMcsMsg          writeBuf;                           // �������ݗp�o�b�t�@
};


/* =======================================================================
    �֐��v���g�^�C�v
   ======================================================================== */

#if defined(NNS_FINALROM)

    #define             NNS_McsInit(workMem)     ( (void)(workMem) )

    #define             NNS_McsGetMaxCaps()     (0)

    #define             NNS_McsOpen(pCaps)      ( (void)(pCaps), FALSE )

    #define             NNS_McsClose()          (FALSE)

    #define             NNS_McsRegisterRecvCallback(pInfo, channel, cbFunc, userData)   ( (void)( (void)( (void)( (void)( (void)(pInfo), (channel) ), (cbFunc) ), (userData) ), 0 ) )

    #define             NNS_McsRegisterStreamRecvBuffer(channel, buf, bufSize)          ( (void)( (void)( (void)( (void)(channel), (buf) ), (bufSize) ), 0 ) )

    #define             NNS_McsUnregisterRecvResource(channel)      ( (void)(channel) )

    #define             NNS_McsGetStreamReadableSize(channel)       ( (void)(channel), 0U )

    #define             NNS_McsGetTotalStreamReadableSize(channel)  ( (void)(channel), 0U )

    #define             NNS_McsReadStream(channel, data, size, pReadSize)   ( (void)( (void)( (void)( (void)(channel), (data) ), (size) ), (pReadSize) ), FALSE )

    #define             NNS_McsGetStreamWritableLength(pLength)             ( (void)(pLength), FALSE )

    #define             NNS_McsWriteStream(channel, data, size)             ( (void)( (void)( (void)(channel), (data) ), (size) ), FALSE )

    #define             NNS_McsClearBuffer()            ((void)0)

    #define             NNS_McsIsServerConnect()        (FALSE)

    #define             NNS_McsPollingIdle()            ((void)0)

    #define             NNS_McsVBlankInterrupt()        ((void)0)

    #define             NNS_McsCartridgeInterrupt()     ((void)0)

/* #if defined(NNS_FINALROM) */
#else

    void                NNS_McsInit(void* workMem);

    int                 NNS_McsGetMaxCaps(void);

    BOOL                NNS_McsOpen(
                            NNSMcsDeviceCaps* pCaps);

    BOOL                NNS_McsClose(void);

    void                NNS_McsRegisterRecvCallback(
                            NNSMcsRecvCBInfo*   pInfo,
                            u16                 channel,
                            NNSMcsRecvCallback  cbFunc,
                            u32                 userData);

    void                NNS_McsRegisterStreamRecvBuffer(
                            u16     channel,
                            void*   buf,
                            u32     bufSize);

    void                NNS_McsUnregisterRecvResource(
                            u16     channel);

    u32                 NNS_McsGetStreamReadableSize(
                            u16     channel);

    u32                 NNS_McsGetTotalStreamReadableSize(
                            u16     channel);

    BOOL                NNS_McsReadStream(
                            u16         channel,
                            void*       data,
                            u32         size,
                            u32*        pReadSize);

    BOOL                NNS_McsGetStreamWritableLength(
                            u32*    pLength);

    BOOL                NNS_McsWriteStream(
                            u16         channel,
                            const void* data,
                            u32         size);

    void                NNS_McsClearBuffer(void);

    BOOL                NNS_McsIsServerConnect(void);

    void                NNS_McsPollingIdle(void);

    void                NNS_McsVBlankInterrupt(void);

    void                NNS_McsCartridgeInterrupt(void);


/* #if defined(NNS_FINALROM) */
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_MCS_BASE_H_ */
#endif
