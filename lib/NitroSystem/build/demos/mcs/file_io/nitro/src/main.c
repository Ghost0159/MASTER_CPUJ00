/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - mcs - file_io - nitro
  File:     main.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

// ============================================================================
//  �f���̓��e
//      mcs���C�u�������g�p���āAPC��̃t�@�C���̓ǂݏ�����
//      PC��̃t�@�C���̗񋓂��s���܂��B
//      ��̓I�ɂ͎��̂��Ƃ��s���܂��B
//
//        1. NITRO-System���[�g�f�B���N�g�����̃t�@�C��
//             "build\demos\mcs\file_io\nitro\data\test.txt"
//           ��ǂݏo���A���e�𔽓]����
//             "build\demos\mcs\file_io\nitro\data\test_wr.txt"
//           �Ƃ����ʃt�@�C���Ƃ��ď������݂܂��B
//        2. NITRO-System���[�g�f�B���N�g�����̎��̃f�B���N�g��
//             "build\demos\mcs\file_io\nitro\data"
//           �̉��ɂ���g���q��"txt"�̃t�@�C�����������܂��B
//
// ============================================================================

#include <nitro.h>
#include <extras.h>

#include <nnsys/fnd.h>
#include <nnsys/mcs.h>
#include <nnsys/misc.h>

// ���̃v���W�F�N�g�̃f�B���N�g��
#define PROJECT_DIR "%NITROSYSTEM_ROOT%\\build\\demos\\mcs\\file_io\\nitro"

// �t�@�C���f�[�^�Ǘ��p�\����
typedef struct FileData FileData;
struct FileData
{
    char*   buf;
    u32     size;
};

static NNSFndHeapHandle gExpHeap;

/*---------------------------------------------------------------------------*
  Name:         VBlankIntr

  Description:  V�u�����N���荞�݃n���h���ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
VBlankIntr(void)
{
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);

    NNS_McsVBlankInterrupt();
}

/*---------------------------------------------------------------------------*
  Name:         CartIntrFunc

  Description:  �J�[�h���b�W���荞�݃n���h���ł��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
CartIntrFunc(void)
{
    OS_SetIrqCheckFlag(OS_IE_CARTRIDGE);

    NNS_McsCartridgeInterrupt();
}

/*---------------------------------------------------------------------------*
  Name:         InitInterrupt

  Description:  ���荞�݊֘A�̏��������s���܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
InitInterrupt()
{
    (void)OS_DisableIrq();

    // V�u�����N���荞�݂�L���ɂ��AVBlank���荞�ݓ���
    // NNS_McsVBlankInterrupt()���Ă΂��悤�ɂ���
    OS_SetIrqFunction(OS_IE_V_BLANK, VBlankIntr);
    (void)OS_EnableIrqMask(OS_IE_V_BLANK);
    (void)GX_VBlankIntr(TRUE);

    // �J�[�g���b�W���荞�݂�L���ɂ��A�J�[�g���b�W���荞�ݓ���
    // NNS_McsCartridgeInterrupt()���Ă΂��悤�ɂ���
    OS_SetIrqFunction(OS_IE_CARTRIDGE, CartIntrFunc);
    (void)OS_EnableIrqMask(OS_IE_CARTRIDGE);

    (void)OS_EnableIrq();
}

/*---------------------------------------------------------------------------*
  Name:         SampleFileRead

  Description:  �t�@�C���̓��e��ǂݍ��݂܂��B

  Arguments:    pFileData:  �t�@�C���̏���ێ�����\���̂ւ̃|�C���^�B

  Returns:      �ǂݍ��݂ɐ�������� TRUE�A���s����� FALSE ��Ԃ��܂��B
 *---------------------------------------------------------------------------*/
static BOOL
SampleFileRead(FileData* pFileData)
{
    void* pDataBuf = 0;
    BOOL bSuccess = FALSE;
    NNSMcsFile info;
    u32 errCode;
    u32 readSize;

    OS_Printf("read test.txt\n");

    // �t�@�C���̃I�[�v��
    errCode = NNS_McsOpenFile(
        &info,
        PROJECT_DIR "\\data\\test.txt",
            NNS_MCS_FILEIO_FLAG_READ
          | NNS_MCS_FILEIO_FLAG_INCENVVAR); // PC�̊��ϐ��������W�J����
    if (errCode)
    {
        OS_Printf("error : NNS_McsOpenFile() test.txt .\n");
        return FALSE;
    }

    // �t�@�C���S�̂�ǂݍ��ނ��߂̃o�b�t�@���m��
    // �S�̂𕶎���Ƃ݂Ȃ����߁A�I�[������p��1�o�C�g��]���Ɋm�ۂ��܂��B
    pDataBuf = NNS_FndAllocFromExpHeap(gExpHeap, NNS_McsGetFileSize(&info) + 1);
    NNS_NULL_ASSERT(pDataBuf);

    // �t�@�C���̓ǂݍ���
    errCode = NNS_McsReadFile(&info, pDataBuf, NNS_McsGetFileSize(&info), &readSize);
    if (0 == errCode)
    {
        pFileData->buf = pDataBuf;
        pFileData->size = readSize;
        pFileData->buf[readSize] = '\0';
        bSuccess = TRUE;
    }
    else
    {
        // �ǂݍ��݂Ɏ��s�����̂ŁA�����������
        NNS_FndFreeToExpHeap(gExpHeap, pDataBuf);
    }

    // �t�@�C���̃N���[�Y
    (void)NNS_McsCloseFile(&info);

    return bSuccess;
}

/*---------------------------------------------------------------------------*
  Name:         SampleFileWrite

  Description:  �t�@�C���֏������݂܂��B

  Arguments:    pFileData:  �t�@�C���̏���ێ�����\���̂ւ̃|�C���^�B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
SampleFileWrite(FileData* pFileData)
{
    NNSMcsFile info;
    u32 errCode;

    OS_Printf("write test_wr.txt\n");

    // �t�@�C���̃I�[�v��
    errCode = NNS_McsOpenFile(
        &info,
        PROJECT_DIR "\\data\\test_wr.txt",
            NNS_MCS_FILEIO_FLAG_WRITE       // �������݃��[�h
          | NNS_MCS_FILEIO_FLAG_FORCE       // �t�@�C�������݂��Ă�����㏑��
          | NNS_MCS_FILEIO_FLAG_INCENVVAR); // PC�̊��ϐ��������W�J����
    if (errCode)
    {
        OS_Printf("error : NNS_McsOpenFile() test_wr.txt .\n");
        return;
    }

    // �t�@�C���̏�������
    errCode = NNS_McsWriteFile(&info, pFileData->buf, pFileData->size);
    if (errCode)
    {
        OS_Printf("error : NNS_McsWriteFile().\n");
    }

    // �t�@�C���̃N���[�Y
    (void)NNS_McsCloseFile(&info);
}

/*---------------------------------------------------------------------------*
  Name:         SampleFileIO

  Description:  �t�@�C������f�[�^��ǂݎ��A
                ���e�𔽓]���ĕʂ̃t�@�C���ɏ������݂܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
SampleFileIO()
{
    BOOL bSuccess = FALSE;
    FileData fileData;

    // �t�@�C����ǂݍ���
    bSuccess = SampleFileRead(&fileData);

    if (bSuccess)
    {
        // ���e�𔽓]
        (void)strrev(fileData.buf);

        // �t�@�C���֏�������
        SampleFileWrite(&fileData);
        NNS_FndFreeToExpHeap(gExpHeap, fileData.buf);
    }
}

/*---------------------------------------------------------------------------*
  Name:         SampleFind

  Description:  �g���q�� txt �̃t�@�C����񋓂��܂��B

  Arguments:    �Ȃ��B

  Returns:      �Ȃ��B
 *---------------------------------------------------------------------------*/
static void
SampleFind()
{
    NNSMcsFile info;
    NNSMcsFileFindData findData;
    u32 errCode;

    // �p�^�[���Ɉ�v����t�@�C��������
    errCode = NNS_McsFindFirstFile(
        &info,
        &findData,
        PROJECT_DIR "\\data\\*.txt");

    // ��v����t�@�C����������Ȃ�
    if (errCode == NNS_MCS_FILEIO_ERROR_NOMOREFILES)
    {
        OS_Printf("not match *.txt .\n");
        return;
    }

    // ���̑��̃G���[
    if (errCode != 0)
    {
        OS_Printf("error : NNS_McsFindFirstFile()\n");
        return;
    }

    // ���Ƀp�^�[���Ɉ�v����t�@�C���������B
    // �֐� NNS_McsFindNextFile ��0�ȊO��Ԃ��܂Ń��[�v���܂��B
    // ��v����t�@�C��������ȏ�Ȃ��ꍇ�́A
    // NNS_MCS_FILEIO_ERROR_NOMOREFILES ���Ԃ�܂��B
    do
    {
        OS_Printf("find filename %s %dbytes.\n", findData.name, findData.size);
        errCode = NNS_McsFindNextFile(&info, &findData);
    }while (0 == errCode);

    if (errCode != NNS_MCS_FILEIO_ERROR_NOMOREFILES)
    {
        OS_Printf("error : NNS_McsFindNextFile()\n");
    }

    (void)NNS_McsCloseFind(&info);
}

void
NitroMain(void)
{
    u8* mcsWorkMem;
    NNSMcsDeviceCaps deviceCaps;
    u8* mcsFileIOWorkMem;

    OS_Init();
    InitInterrupt();

    /*
        ���C���̃A���[�i�S�Ă��g���q�[�v�Ɋ��蓖�Ă�
    */
    {
        void *const arenaLo = OS_GetArenaLo(OS_ARENA_MAIN);

        OS_SetArenaLo(OS_ARENA_MAIN, OS_GetArenaHi(OS_ARENA_MAIN));

        gExpHeap = NNS_FndCreateExpHeap(arenaLo, (u32)OS_GetArenaHi(OS_ARENA_MAIN) - (u32)arenaLo);
    }

    // mcs�̏�����
    mcsWorkMem = NNS_FndAllocFromExpHeapEx(gExpHeap, NNS_MCS_WORKMEM_SIZE, 4); // MCS�̃��[�N�p���������m��
    NNS_McsInit(mcsWorkMem);

    if (NNS_McsGetMaxCaps() > 0)
    {
        // �f�o�C�X�̃I�[�v��
        if (NNS_McsOpen(&deviceCaps))
        {
            // DS�{�̂�IS-NITRO-UIC�ŒʐM����ꍇ�́A
            // mcs�T�[�o���ڑ����ĂȂ��\��������̂ŁA
            // mcs�T�[�o���ڑ����Ă���܂ő҂�
            if (! NNS_McsIsServerConnect())
            {
                OS_Printf("Waiting server connect.\n");
                do
                {
                    SVC_WaitVBlankIntr();
                }while (! NNS_McsIsServerConnect());
            }

            // �t�@�C�����o�͋@�\�̏�����
            mcsFileIOWorkMem = NNS_FndAllocFromExpHeapEx(gExpHeap, NNS_MCS_FILEIO_WORKMEM_SIZE, 4);     // MCS FileI/O �̃��[�N�p���������m��
            NNS_McsInitFileIO(mcsFileIOWorkMem);

            SampleFileIO();
            SampleFind();

            // �f�o�C�X�̃N���[�Y
            (void)NNS_McsClose();
        }
        else
        {
            OS_Printf("device open fail.\n");
        }
    }
    else
    {
        OS_Printf("device not found.\n");
    }

    while (TRUE) {}
}

