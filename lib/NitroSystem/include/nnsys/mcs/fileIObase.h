/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - mcs
  File:     fileIObase.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.13 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_MCS_FILEIOBASE_H_
#define NNS_MCS_FILEIOBASE_H_

#ifdef __cplusplus
extern "C" {
#endif


/* ========================================================================
    �萔��`
   ======================================================================== */

#define NNS_MCS_FILEIO_WORKMEM_SIZE     sizeof(NNSiMcsFileIOWork)


/* ========================================================================
    �^��`
   ======================================================================== */

/*
    �t�@�C�����\����
*/
typedef struct NNSMcsFile NNSMcsFile;
struct NNSMcsFile
{
    u32                 handle;
    u32                 filesize;
    u32                 errCode;
    u32                 srvErrCode;

    u32                 tempData;
    volatile u32        bDone       : 1;
};

typedef struct NNSMcsFileFindData NNSMcsFileFindData;
struct NNSMcsFileFindData
{
    u32         attribute;
    u32         size;
    char        name[NNS_MCS_FILEIO_PATH_MAX];
};

typedef struct NNSiMcsFileIOWork NNSiMcsFileIOWork;
struct NNSiMcsFileIOWork
{
    NNSMcsRecvCBInfo    cbInfo;

    u8                  sendBuffer[NNSi_MCS_MSG_DATA_SIZE_MIN];
};


/* ========================================================================
    �}�N���֐�
   ======================================================================== */

#if defined(NNS_FINALROM)

    #define         NNS_McsGetFileSize(pFile)                           ( (void)(pFile), 0U)

    #define         NNS_McsGetServerErrorCode(pFile)                    ( (void)(pFile), 0U)

/* #if defined(NNS_FINALROM) */
#else

    #define         NNS_McsGetFileSize(pFile)                           ((pFile)->filesize)

    #define         NNS_McsGetServerErrorCode(pFile)                    ((pFile)->srvErrCode)

/* #if defined(NNS_FINALROM) */
#endif


/* ========================================================================
    �֐��v���g�^�C�v
   ======================================================================== */

#if defined(NNS_FINALROM)


    #define         NNS_McsInitFileIO(workMem)                          ( (void)(workMem) )

    #define         NNS_McsOpenFile(pFile, filename, openflag)          ( (void)( (void)( (void)(pFile), (filename) ), (openflag) ), NNS_MCS_FILEIO_ERROR_COMERROR )

    #define         NNS_McsCloseFile(pFile)                             ( (void)(pFile), NNS_MCS_FILEIO_ERROR_COMERROR )

    #define         NNS_McsReadFile(pFile, buffer, length, pReadBytes)  ( (void)( (void)( (void)( (void)(pFile), (buffer) ), (length) ), (pReadBytes) ), NNS_MCS_FILEIO_ERROR_COMERROR )

    #define         NNS_McsWriteFile(pFile, buffer, length)             ( (void)( (void)( (void)(pFile), (buffer) ), (length) ), NNS_MCS_FILEIO_ERROR_COMERROR )

    #define         NNS_McsSeekFile(pFile, distanceToMove, moveMethod, pNewFilePointer) \
                                                                        ( (void)( (void)( (void)( (void)(pFile), (distanceToMove) ), (moveMethod) ), (pNewFilePointer) ), NNS_MCS_FILEIO_ERROR_COMERROR )

    #define         NNS_McsFindFirstFile(pFile, pFindData, pattern)     ( (void)( (void)( (void)(pFile), (pFindData) ), (pattern) ), NNS_MCS_FILEIO_ERROR_COMERROR )

    #define         NNS_McsFindNextFile(pFile, pFindData)               ( (void)( (void)(pFile), (pFindData) ), NNS_MCS_FILEIO_ERROR_COMERROR )

    #define         NNS_McsCloseFind(pFile)                             ( (void)(pFile), NNS_MCS_FILEIO_ERROR_COMERROR )


/* #if defined(NNS_FINALROM) */
#else


    void            NNS_McsInitFileIO(
                        void*               workMem);

    u32             NNS_McsOpenFile(
                        NNSMcsFile*         pFile,
                        const char*         fileName,
                        u32                 openFlag);

    u32             NNS_McsCloseFile(
                        NNSMcsFile*         pFile);

    u32             NNS_McsReadFile(
                        NNSMcsFile*         pFile,
                        void*               buffer,
                        u32                 length,
                        u32*                pReadBytes);

    u32             NNS_McsWriteFile(
                        NNSMcsFile*         pFile,
                        const void*         buffer,
                        u32                 length);

    u32             NNS_McsSeekFile(
                        NNSMcsFile*         pFile,
                        s32                 distanceToMove,
                        u32                 moveMethod,
                        u32*                pNewFilePointer);

    u32             NNS_McsFindFirstFile(
                        NNSMcsFile*         pFile,
                        NNSMcsFileFindData* pFindData,
                        const char*         pattern);

    u32             NNS_McsFindNextFile(
                        NNSMcsFile*         pFile,
                        NNSMcsFileFindData* pFindData);

    u32             NNS_McsCloseFind(
                        NNSMcsFile*         pFile);


/* #if defined(NNS_FINALROM) */
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif

/* NNS_MCS_FILEIOBASE_H_ */
#endif

