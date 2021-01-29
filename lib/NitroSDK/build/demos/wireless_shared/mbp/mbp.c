/*---------------------------------------------------------------------------*
  Project:  NitroSDK - wireless_shared - demos - mbp
  File:     mbp.c

  Copyright 2003-2006 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mbp.c,v $
  Revision 1.10  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.9  2005/11/22 03:17:54  yosizaki
  fix about MBP_STATE_REBOOTING state.

  Revision 1.8  2005/04/08 05:47:55  yosizaki
  bitmap����̏C��.

  Revision 1.7  2005/02/28 05:26:25  yosizaki
  do-indent.

  Revision 1.6  2005/02/18 10:38:30  yosizaki
  fix around hidden warnings.

  Revision 1.5  2004/12/03 08:13:32  takano_makoto
  MPB_USING_MB_EX -> MBP_USING_MB_EX �ɏC���A MBP_USING_PREVIOUS_DOWNLOAD�X�C�b�`��ǉ�

  Revision 1.4  2004/11/24 06:15:12  yosizaki
  rename MB_StartParentEx/MB_EndEx

  Revision 1.3  2004/11/18 07:57:41  takano_makoto
  MB_SetParentCommSize, MB_SetParentCommParam�Ŏq�@���M�T�C�Y�̐ݒ������p�~

  Revision 1.2  2004/11/12 11:57:35  yosizaki
  add option for cloneboot.

  Revision 1.1  2004/11/12 09:02:39  yosizaki
  (none)

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <nitro/wm.h>
#include <nitro/mb.h>

#include "mbp.h"




//============================================================================
//  �v���g�^�C�v�錾
//============================================================================

static BOOL MBP_RegistFile(const MBGameRegistry *gameInfo);
static inline void MBP_AddBitmap(u16 *pBitmap, u16 aid);
static inline void MBP_RemoveBitmap(u16 *pBitmap, u16 aid);
static inline void MBP_DisconnectChild(u16 aid);
static inline void MBP_DisconnectChildFromBmp(u16 aid);
static void ParentStateCallback(u16 child_aid, u32 status, void *arg);
static void MBP_ChangeState(u16 state);


// �R�[���o�b�N��IRQ�œ��삵�Ă��܂��̂ŃR�[���o�b�N���ŕ��G�ȏ���������ꍇ�ɂ́A
// lcf�t�@�C����IRQ�X�^�b�N�T�C�Y�������傫�߂ɐݒ肵�Ă������������S�ł��B
//
// ����OS_Printf()�͑�ʂ̃X�^�b�N�������̂ŁA
// �R�[���o�b�N���ł͂ł��邾���y�ʔł�OS_TPrintf()���g�p����悤�ɂ��Ă��������B

#define MBP_DEBUG
#if defined( MBP_DEBUG )
#define MBP_Printf          OS_TPrintf //
#else
#define MBP_Printf(...)     ((void)0)
#endif



//============================================================================
//  �ϐ���`
//============================================================================

// �q�@�ڑ����
static MBPState mbpState;
static MBPChildInfo childInfo[MBP_CHILD_MAX];

/* MB ���C�u�����֊��蓖�Ă郏�[�N�̈� */
static u32 *sCWork = NULL;

/* �q�@�N���o�C�i���]���p�o�b�t�@ */
static u8 *sFilebuf = NULL;

//============================================================================
//  �֐���`
//============================================================================

/*---------------------------------------------------------------------------*
  Name:         MBP_Init

  Description:  �}���`�u�[�g�e�@��������������B

  Arguments:    ggid  �Q�[���z�M���̐e��GGID���w��.
                tgid  �Q�[���z�M���̐e��TGID���w��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_Init(u32 ggid, u16 tgid)
{
    /* �q�@��ʂ֕\������e�@���ݒ�p */
    MBUserInfo myUser;

    /* MB�q�@��ʂɕ\������e�@���Ƃ��Ăǂ̂悤�Ȓl��ݒ肵�Ȃ����   *
     * �Ȃ�Ȃ����͌����ɂ͌��肵�Ă��܂���B                           *
     * �����ł�IPL�œo�^���ꂽ���[�U����MB�e�@���Ƃ��Đݒ肵�܂����A*
     * �Q�[�����Őݒ肵�����O���Z�b�g����ꍇ�����邩������܂���B     *
     * ������2�o�C�g�R�[�h�Őݒ肷��̂�Y��Ȃ��ł��������B            */
#if ( SDK_NVRAM_FORMAT >= 100 )
    OSOwnerInfo info;

    OS_GetOwnerInfo(&info);
    myUser.favoriteColor = info.favoriteColor;
    myUser.nameLength = (u8)info.nickNameLength;
    MI_CpuCopy8(info.nickName, myUser.name, (u32)(info.nickNameLength * 2));
#else  // ��IPL�p
    NVRAMConfig *info = (NVRAMConfig *)(OS_GetSystemWork()->nvramUserInfo);

#define DEFAULT_FAVORIT_COLOR    1
    myUser.favoriteColor = DEFAULT_FAVORIT_COLOR;
    myUser.nameLength = (u8)info->ncd.owner.nickname.length;
    MI_CpuCopy8(info->ncd.owner.nickname.name, myUser.name, NVRAM_CONFIG_NICKNAME_LENGTH * 2);
#endif

    myUser.playerNo = 0;               // �e�@��0��

    // �X�e�[�^�X����������
    mbpState = (const MBPState)
    {
    MBP_STATE_STOP, 0, 0, 0, 0, 0, 0};

    /* MB �e�@������J�n���܂�. */
    // MB���[�N�̈�m�ہB
#if !defined(MBP_USING_MB_EX)
    sCWork = OS_Alloc(MB_SYSTEM_BUF_SIZE);
#else
    /* �O���� WM �̏��������ς܂��Ă���΃��[�N�T�C�Y�͏��Ȃ����Ă��\���܂���. */
    sCWork = OS_Alloc(MB_SYSTEM_BUF_SIZE - WM_SYSTEM_BUF_SIZE);
#endif

    if (MB_Init(sCWork, &myUser, ggid, tgid, MBP_DMA_NO) != MB_SUCCESS)
    {
        OS_Panic("ERROR in MB_Init\n");
    }

    // �ő�ڑ��q�@����ݒ�(�e�@�̐��͔������l��ݒ�)
    (void)MB_SetParentCommParam(MB_COMM_PARENT_SEND_MIN, MBP_CHILD_MAX);

    MB_CommSetParentStateCallback(ParentStateCallback);

    MBP_ChangeState(MBP_STATE_IDLE);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_Start

  Description:  �}���`�u�[�g�e�@�J�n

  Arguments:    gameInfo �z�M�o�C�i�����.
                channel  �g�p����`�����l��.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_Start(const MBGameRegistry *gameInfo, u16 channel)
{
    SDK_ASSERT(MBP_GetState() == MBP_STATE_IDLE);

    MBP_ChangeState(MBP_STATE_ENTRY);
#if !defined(MBP_USING_MB_EX)
    if (MB_StartParent(channel) != MB_SUCCESS)
#else
    /* WM ���C�u������ IDLE �X�e�[�g�Ɉڍs���Ă���, ���ڊJ�n���܂�. */
    if (MB_StartParentFromIdle(channel) != MB_SUCCESS)
#endif
    {
        MBP_Printf("MB_StartParent fail\n");
        MBP_ChangeState(MBP_STATE_ERROR);
        return;
    }

    /* --------------------------------------------------------------------- *
     * MB_StartParent()�R�[�����ɏ���������Ă��܂��܂��B
     * MB_RegisterFile()�͕K��MB_StartParent()�̌�œo�^���Ă��������B
     * --------------------------------------------------------------------- */

    /* �_�E�����[�h�v���O�����t�@�C������o�^���܂�. */
    if (!MBP_RegistFile(gameInfo))
    {
        OS_Panic("Illegal multiboot gameInfo\n");
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBP_RegistFile

  Description:  �}���`�u�[�g�o�C�i���̓o�^

  Arguments:    gameInfo �}���`�u�[�g�o�C�i�����ւ̃|�C���^.
                         ���̃����o�̂��� romFilePathp �� NULL �̏ꍇ,
                         �N���[���u�[�g���w�肳�ꂽ���̂Ƃ��ē��삵�܂�.

  Returns:      �t�@�C���̃I�[�v�������������� TRUE.
                ���s������ FALSE��Ԃ��B
 *---------------------------------------------------------------------------*/
static BOOL MBP_RegistFile(const MBGameRegistry *gameInfo)
{
    FSFile  file, *p_file;
    u32     bufferSize;
    BOOL    ret = FALSE;

    /* --------------------------------------------------------------------- *
     * MB_StartParent()�������MB_RegisterFile()�Ńo�C�i����o�^���Ă��A
     * MB_StartParent()�R�[�����ɏ���������Ă��܂��܂��B
     * MB_RegisterFile()�͕K��MB_StartParent()�̌�œo�^���Ă��������B
     * --------------------------------------------------------------------- */

    /*
     * ���̊֐��̎d�l�Ƃ���, romFilePathp �� NULL �ł����
     * �N���[���u�[�g�Ƃ��ē��삵�܂�.
     * �����łȂ���Ύw�肳�ꂽ�t�@�C�����q�@�v���O�����Ƃ��Ĉ����܂�.
     */
    if (!gameInfo->romFilePathp)
    {
        p_file = NULL;
    }
    else
    {
        /* 
         * �v���O�����t�@�C���� FS_ReadFile() �œǂݏo����K�v������܂�.
         * �ʏ�̓v���O������ CARD-ROM ���Ƀt�@�C���Ƃ��ĕێ�����͂��Ȃ̂�
         * ���͂���܂���, ��������ȃ}���`�u�[�g�V�X�e����z�肷��ꍇ,
         * FSArchive �œƎ��A�[�J�C�u���\�z���đΏ����Ă�������. 
         */
        FS_InitFile(&file);
        if (!FS_OpenFile(&file, gameInfo->romFilePathp))
        {
            /* �t�@�C�����J���Ȃ�, */
            OS_Warning("Cannot Register file\n");
            return FALSE;
        }
        p_file = &file;
    }

    /*
     * �Z�O�����g���̃T�C�Y���擾.
     * �����ȃ_�E�����[�h�v���O�����łȂ��ꍇ,
     * ���̃T�C�Y�� 0 ���Ԃ��Ă��܂�.
     */
    bufferSize = MB_GetSegmentLength(p_file);
    if (bufferSize == 0)
    {
        OS_Warning("specified file may be invalid format.\"%s\"\n",
                   gameInfo->romFilePathp ? gameInfo->romFilePathp : "(cloneboot)");
    }
    else
    {
        /*
         * �_�E�����[�h�v���O�����̃Z�O�����g����ǂݍ��ރ��������m��.
         * �t�@�C���̃��W�X�g�ɐ��������ꍇ��MB_End()���Ă΂��܂�
         * ���̗̈悪�g�p����܂��B
         * ���̃������̓T�C�Y�����[���ł���� �ÓI�ɗp�ӂ���Ă��Ă��\���܂���.
         */
        sFilebuf = OS_Alloc(bufferSize);
        if (sFilebuf == NULL)
        {
            /* �Z�O�����g�����i�[����o�b�t�@�̊m�ێ��s */
            OS_Warning("can't allocate Segment buffer size.\n");
        }
        else
        {
            /* 
             * �Z�O�����g�����t�@�C�����璊�o.
             * ���o�����Z�O�����g����, �_�E�����[�h�v���O�����z�M��
             * ���C����������ɏ풓�����Ă����K�v������܂�.
             */
            if (!MB_ReadSegment(p_file, sFilebuf, bufferSize))
            {
                /* 
                 * �s���ȃt�@�C���ɂ��Z�O�����g���o�Ɏ��s
                 * �T�C�Y�擾�����������ɂ��ւ�炸�����Œ��o���������s����̂�,
                 * �t�@�C���n���h���ɉ����ύX��^�����ꍇ�Ȃǂł�.
                 * (�t�@�C�������, �ʒu���V�[�N����, ...)
                 */
                OS_Warning(" Can't Read Segment\n");
            }
            else
            {
                /*
                 * ���o�����Z�O�����g���� MBGameRegistry ��
                 * �_�E�����[�h�v���O������o�^���܂�.
                 */
                if (!MB_RegisterFile(gameInfo, sFilebuf))
                {
                    /* �s���ȃv���O�������ɂ��o�^���s */
                    OS_Warning(" Illegal program info\n");
                }
                else
                {
                    /* �����͐������������܂��� */
                    ret = TRUE;
                }
            }
            if (!ret)
                OS_Free(sFilebuf);
        }
    }

    /* �N���[���u�[�g�łȂ���΃t�@�C�����N���[�Y */
    if (p_file == &file)
    {
        (void)FS_CloseFile(&file);
    }

    return ret;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_AcceptChild

  Description:  �q�@�̃��N�G�X�g���󂯕t����B

  Arguments:    child_aid       �ڑ��q�@��aid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_AcceptChild(u16 child_aid)
{
    if (!MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_ACCEPT))
    {
        // ���N�G�X�g�Ɏ��s�����ꍇ���̎q�@��ؒf����B
        MBP_DisconnectChild(child_aid);
        return;
    }

    MBP_Printf("accept child %d\n", child_aid);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_KickChild

  Description:  �ڑ��q�@���L�b�N�B

  Arguments:    child_aid       �ڑ��q�@��aid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_KickChild(u16 child_aid)
{
    if (!MB_CommResponseRequest(child_aid, MB_COMM_RESPONSE_REQUEST_KICK))
    {
        // ���N�G�X�g�Ɏ��s�����ꍇ���̎q�@��ؒf����B
        MBP_DisconnectChild(child_aid);
        return;
    }

    {
        OSIntrMode enabled = OS_DisableInterrupts();

        mbpState.requestChildBmp &= ~(1 << child_aid);
        mbpState.connectChildBmp &= ~(1 << child_aid);

        (void)OS_RestoreInterrupts(enabled);
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBP_StartDownload

  Description:  �q�@�փ_�E�����[�h�J�n�M���𑗐M�B

  Arguments:    child_aid       �ڑ��q�@��aid

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_StartDownload(u16 child_aid)
{
    if (!MB_CommStartSending(child_aid))
    {
        // ���N�G�X�g�Ɏ��s�����ꍇ���̎q�@��ؒf����B
        MBP_DisconnectChild(child_aid);
        return;
    }

    {
        OSIntrMode enabled = OS_DisableInterrupts();

        mbpState.entryChildBmp &= ~(1 << child_aid);
        mbpState.downloadChildBmp |= 1 << child_aid;

        (void)OS_RestoreInterrupts(enabled);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBP_StartDownloadAll

  Description:  ���݃G���g���[���̑S�q�@�ɑ΂��ăf�[�^�]�����J�n����B
                �ȍ~�A�q�@�̃G���g���[��t���s�Ȃ�Ȃ��B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_StartDownloadAll(void)
{
    u16     i;

    // �G���g���[��t�I��
    MBP_ChangeState(MBP_STATE_DATASENDING);

    for (i = 1; i < 16; i++)
    {
        if (!(mbpState.connectChildBmp & (1 << i)))
        {
            continue;
        }

#ifdef MBP_USING_PREVIOUS_DOWNLOAD
        /* �G���g���[�㑦�f�[�^���M���J�n���Ă���ꍇ */
        // �G���g���[���łȂ��q�@�͐ؒf����
        if (MBP_GetChildState(i) == MBP_CHILDSTATE_CONNECTING)
        {
            MBP_DisconnectChild(i);
            continue;
        }
#else
        /* �f�[�^���M��S�q�@��ĂɊJ�n����ꍇ */
        if (mbpState.requestChildBmp & (1 << i))
        {
            // ���݃G���g���[���̎q�@�͌�ŏ������ł��Ă��珈��������
            continue;
        }

        // �G���g���[���łȂ��q�@�͐ؒf����
        if (!(mbpState.entryChildBmp & (1 << i)))
        {
            MBP_DisconnectChild(i);
            continue;
        }

        // �G���g���[���̎q�@�̓_�E�����[�h�J�n
        MBP_StartDownload(i);
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBP_IsBootableAll

  Description:  �ڑ�����Ă���q�@���ׂĂ��u�[�g�\��ԂɂȂ��Ă��邩��
                �`�F�b�N���܂��B

  Arguments:    None.

  Returns:      �S�q�@���u�[�g�\�ł����       TRUE
                �u�[�g�s�\�Ȏq�@�����ł������ FALSE
 *---------------------------------------------------------------------------*/
BOOL MBP_IsBootableAll(void)
{
    u16     i;

    if (mbpState.connectChildBmp == 0)
    {
        return FALSE;
    }

    for (i = 1; i < 16; i++)
    {
        if (!(mbpState.connectChildBmp & (1 << i)))
        {
            continue;
        }

        if (!MB_CommIsBootable(i))
        {
            return FALSE;
        }
    }
    return TRUE;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_StartRebootAll

  Description:  �u�[�g�\�Ȑڑ��q�@�����ׂău�[�g����B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_StartRebootAll(void)
{
    u16     i;
    u16     sentChild = 0;

    for (i = 1; i < 16; i++)
    {
        if (!(mbpState.bootableChildBmp & (1 << i)))
        {
            continue;
        }
        if (!MB_CommBootRequest(i))
        {
            // ���N�G�X�g�Ɏ��s�����ꍇ���̎q�@��ؒf����B
            MBP_DisconnectChild(i);
            continue;
        }
        sentChild |= (1 << i);
    }

    // �ڑ��q�@��0�ɂȂ�����G���[�I��
    if (sentChild == 0)
    {
        MBP_ChangeState(MBP_STATE_ERROR);
        return;
    }

    // ��Ԃ�ύX���f�[�^�]���̎�t���I�����܂��B
    MBP_ChangeState(MBP_STATE_REBOOTING);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_Cancel

  Description:  �}���`�u�[�g���L�����Z������B

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_Cancel(void)
{
    MBP_ChangeState(MBP_STATE_CANCEL);
#if !defined(MBP_USING_MB_EX)
    MB_End();
#else
    MB_EndToIdle();
#endif
}

/*---------------------------------------------------------------------------*
  Name:         MBPi_CheckAllReboot

  Description:  �ڑ����̑S�q�@�� MB_COMM_PSTATE_BOOT_STARTABLE ��Ԃ����肵,
                �����ł���� MB ���C�u�����I���������J�n����.

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBPi_CheckAllReboot(void)
{
    if ((mbpState.state == MBP_STATE_REBOOTING) &&
        (mbpState.connectChildBmp == mbpState.rebootChildBmp))
    {
        MBP_Printf("call MB_End()\n");
#if !defined(MBP_USING_MB_EX)
        MB_End();
#else
        MB_EndToIdle();
#endif
    }
}

/*---------------------------------------------------------------------------*
  Name:         ParentStateCallback

  Description:  �e�@��Ԓʒm�R�[���o�b�N

  Arguments:    child_aid       �q�@aid.
                status          �R�[���o�b�N���
                arg             �I�v�V��������
  Returns:      None.
 *---------------------------------------------------------------------------*/
static void ParentStateCallback(u16 child_aid, u32 status, void *arg)
{
#if defined( MBP_DEBUG )
    // �f�o�b�O�o�͗p
    static const char *MB_CALLBACK_TABLE[] = {
        "MB_COMM_PSTATE_NONE",
        "MB_COMM_PSTATE_INIT_COMPLETE",
        "MB_COMM_PSTATE_CONNECTED",
        "MB_COMM_PSTATE_DISCONNECTED",
        "MB_COMM_PSTATE_KICKED",
        "MB_COMM_PSTATE_REQ_ACCEPTED",
        "MB_COMM_PSTATE_SEND_PROCEED",
        "MB_COMM_PSTATE_SEND_COMPLETE",
        "MB_COMM_PSTATE_BOOT_REQUEST",
        "MB_COMM_PSTATE_BOOT_STARTABLE",
        "MB_COMM_PSTATE_REQUESTED",
        "MB_COMM_PSTATE_MEMBER_FULL",
        "MB_COMM_PSTATE_END",
        "MB_COMM_PSTATE_ERROR",
        "MB_COMM_PSTATE_WAIT_TO_SEND",
    };
#endif

    MBP_Printf("get callback %s %d\n", MB_CALLBACK_TABLE[status], child_aid);

    switch (status)
    {
        //-----------------------------------------------------------
        // �e�@�̏����������������̒ʒm
    case MB_COMM_PSTATE_INIT_COMPLETE:
        // None.
        break;

        //-----------------------------------------------------------
        // �q�@����̐ڑ��������u�Ԃ̒ʒm
    case MB_COMM_PSTATE_CONNECTED:
        {
            // �e�@���G���g���[��t��ԈȊO�̏ꍇ�̐ڑ��͎󂯕t���Ȃ�
            if (MBP_GetState() != MBP_STATE_ENTRY)
            {
                break;
            }

            MBP_AddBitmap(&mbpState.connectChildBmp, child_aid);
            // �q�@MacAddress�̕ۑ�
            WM_CopyBssid(((WMStartParentCallback *)arg)->macAddress,
                         childInfo[child_aid - 1].macAddress);

            childInfo[child_aid - 1].playerNo = child_aid;
        }
        break;

        //-----------------------------------------------------------
        // �q�@���ڑ���؂������̒ʒm
    case MB_COMM_PSTATE_DISCONNECTED:
        {
            // �q�@�̃��u�[�g�ȊO�̏����Őؒf���ꂽ�ꍇ�̓G���g���[���폜���܂��B
            if (MBP_GetChildState(child_aid) != MBP_CHILDSTATE_REBOOT)
            {
                MBP_DisconnectChildFromBmp(child_aid);
                // ������ӂ܂���������, �S���u�[�g�ς݂����肵�܂�.
                MBPi_CheckAllReboot();
            }
        }
        break;

        //-----------------------------------------------------------
        // �q�@����G���g���[���N�G�X�g�������u�Ԃ̒ʒm
    case MB_COMM_PSTATE_REQUESTED:
        {
            const MBUserInfo *userInfo;

            userInfo = (MBUserInfo *)arg;

            OS_TPrintf("callback playerNo = %d\n", userInfo->playerNo);

            // �e�@���G���g���[��t��ԂłȂ��ꍇ�ɃG���g���[�v�������Ă���
            // �q�@�͊m�F�Ȃ��ɃL�b�N����B
            if (MBP_GetState() != MBP_STATE_ENTRY)
            {
                MBP_KickChild(child_aid);
                break;
            }

            // �q�@�̃G���g���[���󂯕t����
            mbpState.requestChildBmp |= 1 << child_aid;

            MBP_AcceptChild(child_aid);

            // MB_COMM_PSTATE_CONNECTED�̃^�C�~���O�ł͂܂�UserInfo���Z�b�g����Ă��Ȃ��̂�
            // MB_CommGetChildUser��REQUESTED�ȍ~�̏�ԂŌĂ΂Ȃ��ƈӖ�������܂���B
            userInfo = MB_CommGetChildUser(child_aid);
            if (userInfo != NULL)
            {
                MI_CpuCopy8(userInfo, &childInfo[child_aid - 1].user, sizeof(MBUserInfo));
            }
            MBP_Printf("playerNo = %d\n", userInfo->playerNo);
        }
        break;

        //-----------------------------------------------------------
        // �q�@�ւ�ACCEPT�ɑ΂���ACK�̒ʒm
    case MB_COMM_PSTATE_REQ_ACCEPTED:
        // ���ɏ����͕K�v�Ȃ��B
        break;
        //-----------------------------------------------------------
        // �q�@����̃_�E�����[�h�v�����󂯂����̒ʒm
    case MB_COMM_PSTATE_WAIT_TO_SEND:
        {
            // �q�@�̏�Ԃ��G���g���[�ς�=�_�E�����[�h�҂��̏�Ԃ֕ύX
            // ���荞�ݒ��̏����Ȃ̂œ��Ɋ��荞�݋֎~�ݒ�ɂ����ɕύX
            mbpState.requestChildBmp &= ~(1 << child_aid);
            mbpState.entryChildBmp |= 1 << child_aid;

            // ���C�����[�`������MBP_StartDownload()���R�[�������ƃf�[�^���M���J�n����
            // ���łɃf�[�^���M��Ԃɓ����Ă��܂��Ă���ꍇ�͂��̎q�@�ւ��f�[�^���M���J�n����B
#ifdef MBP_USING_PREVIOUS_DOWNLOAD
            /* �G���g���[�㑦�f�[�^���M���J�n����ꍇ */
            MBP_StartDownload(child_aid);
#else
            /* �f�[�^���M��S�q�@��ĂɊJ�n����ꍇ */
            if (MBP_GetState() == MBP_STATE_DATASENDING)
            {
                MBP_StartDownload(child_aid);
            }
#endif
        }
        break;

        //-----------------------------------------------------------
        // �q�@���L�b�N�������̒ʒm
    case MB_COMM_PSTATE_KICKED:
        // None.
        break;

        //-----------------------------------------------------------
        // �q�@�փo�C�i�����M���J�n���鎞�̒ʒm
    case MB_COMM_PSTATE_SEND_PROCEED:
        // None.
        break;

        //-----------------------------------------------------------
        // �q�@�ւ̃o�C�i�����M���I���������̒ʒm
    case MB_COMM_PSTATE_SEND_COMPLETE:
        {
            // ���荞�ݒ��̏����Ȃ̂œ��Ɋ��荞�݋֎~�ݒ�ɂ����ɕύX
            mbpState.downloadChildBmp &= ~(1 << child_aid);
            mbpState.bootableChildBmp |= 1 << child_aid;
        }
        break;

        //-----------------------------------------------------------
        // �q�@�ɑ΂��ău�[�g�������������������̒ʒm
    case MB_COMM_PSTATE_BOOT_STARTABLE:
        {
            // ���荞�ݒ��̏����Ȃ̂œ��Ɋ��荞�݋֎~�ݒ�ɂ����ɕύX
            mbpState.bootableChildBmp &= ~(1 << child_aid);
            mbpState.rebootChildBmp |= 1 << child_aid;

            // �S�q�@���u�[�g���������ꍇ�ɂ͐e�@���Đڑ������ɓ���
            MBPi_CheckAllReboot();
        }
        break;

        //-----------------------------------------------------------
        // �Ώێq�@�փu�[�g�v���̑��M���J�n�������̒ʒm
        // �R�[���o�b�N�Ƃ��Ă͕Ԃ�Ȃ��B
    case MB_COMM_PSTATE_BOOT_REQUEST:
        // None.
        break;

        //-----------------------------------------------------------
        // �����o�[���t���ɂȂ����ꍇ�̒ʒm
    case MB_COMM_PSTATE_MEMBER_FULL:
        // None.
        break;

        //-----------------------------------------------------------
        // �}���`�u�[�g�I�����̒ʒm
    case MB_COMM_PSTATE_END:
        {
            if (MBP_GetState() == MBP_STATE_REBOOTING)
                // ���u�[�g�����������AMB���I�����Ďq�@�ƍĐڑ����s�Ȃ�
            {
                MBP_ChangeState(MBP_STATE_COMPLETE);
            }
            else
                // �V���b�g�_�E�������ASTOP��Ԃ֖߂�
            {
                MBP_ChangeState(MBP_STATE_STOP);
            }

            // �Q�[���z�M�p�Ɏg�p���Ă����o�b�t�@���N���A
            // MB_COMM_PSTATE_END�̃R�[���o�b�N���A���Ă������_��
            // ���[�N�͊J������Ă���̂�Free���Ă��܂��Ă悢�B
            if (sFilebuf)
            {
                OS_Free(sFilebuf);
                sFilebuf = NULL;
            }
            if (sCWork)
            {
                OS_Free(sCWork);
                sCWork = NULL;
            }

            /* MB_End���Ă�work���J�����邱�Ƃŏ���������邽��MB_UnregisterFile *
             * �͏ȗ����邱�Ƃ��ł��܂��B                                        */

        }
        break;

        //-----------------------------------------------------------
        // �G���[�������̒ʒm
    case MB_COMM_PSTATE_ERROR:
        {
            MBErrorStatus *cb = (MBErrorStatus *)arg;

            switch (cb->errcode)
            {
                //------------------------------
                // �ʒm���x���̃G���[. ���ɏ������s�Ȃ��K�v�͂Ȃ�.
            case MB_ERRCODE_WM_FAILURE:
                // None.
                break;
                //------------------------------
                // ���������Z�b�g����K�v�����郌�x���̃G���[�B
            case MB_ERRCODE_INVALID_PARAM:
            case MB_ERRCODE_INVALID_STATE:
            case MB_ERRCODE_FATAL:
                MBP_ChangeState(MBP_STATE_ERROR);
                break;
            }
        }
        break;

        //-----------------------------------------------------------
    default:
        OS_Panic("Get illegal parent state.\n");
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBP_ChangeState

  Description:  �e�@��ԕύX

  Arguments:    state       �ύX������.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBP_ChangeState(u16 state)
{
#if defined( MBP_DEBUG )
    // �f�o�b�O�o�͗p
    static const char *STATE_NAME[] = {
        "MBP_STATE_STOP",
        "MBP_STATE_IDLE",
        "MBP_STATE_ENTRY",
        "MBP_STATE_DATASENDING",
        "MBP_STATE_REBOOTING",
        "MBP_STATE_COMPLETE",
        "MBP_STATE_CANCEL",
        "MBP_STATE_ERROR"
    };
#endif

    SDK_ASSERT(state < MBP_STATE_NUM);

    MBP_Printf("%s -> %s\n", STATE_NAME[mbpState.state], STATE_NAME[state]);
    mbpState.state = state;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetState

  Description:  �e�@��Ԃ̎擾

  Arguments:    None.

  Returns:      �e�@���.
 *---------------------------------------------------------------------------*/
u16 MBP_GetState(void)
{
    return mbpState.state;
}

/*---------------------------------------------------------------------------*
  Name:         MBP_GetConnectState

  Description:  �ڑ����̎擾

  Arguments:    ���݂̐ڑ���Ԃ��擾���邽�߂̃o�b�t�@�̈�.

  Returns:      None.
 *---------------------------------------------------------------------------*/
void MBP_GetConnectState(MBPState * state)
{
    MI_CpuCopy8(&mbpState, state, sizeof(MBPState));
}

/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildBmp

  Description:  �q�@�̐ڑ��r�b�g�}�b�v���擾���܂�

  Arguments:    �擾����ڑ��r�b�g�}�b�v�̎��.

  Returns:      �q�@���.
 *---------------------------------------------------------------------------*/
u16 MBP_GetChildBmp(MBPBmpType bmpType)
{
    u16     tmpBitmap;
    static const u16 *BITMAP_TABLE[MBP_BMPTYPE_NUM] = {
        &mbpState.connectChildBmp,
        &mbpState.requestChildBmp,
        &mbpState.entryChildBmp,
        &mbpState.downloadChildBmp,
        &mbpState.bootableChildBmp,
        &mbpState.rebootChildBmp,
    };

    SDK_ASSERT(bmpType < MBP_BMPTYPE_NUM);

    tmpBitmap = *(BITMAP_TABLE[bmpType]);

    return tmpBitmap;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildState

  Description:  �q�@�̐ڑ���Ԃ��擾

  Arguments:    aid     �ڑ��q�@�ԍ�.

  Returns:      �q�@�̏��.
 *---------------------------------------------------------------------------*/
MBPChildState MBP_GetChildState(u16 aid)
{
    MBPState tmpState;
    u16     bitmap = (u16)(1 << aid);

    // ���荞�݂ɂ���ĕs�ӂɐڑ���Ԃ��ς�鎖�����邽��
    // ���荞�݂��֎~���Ă��̎��_�̏�Ԃ̃X�i�b�v�V���b�g���擾
    OSIntrMode enabled = OS_DisableInterrupts();
    if ((mbpState.connectChildBmp & bitmap) == 0)
    {
        (void)OS_RestoreInterrupts(enabled);
        return MBP_CHILDSTATE_NONE;    // ���ڑ�
    }
    MI_CpuCopy8(&mbpState, &tmpState, sizeof(MBPState));
    (void)OS_RestoreInterrupts(enabled);

    // �ڑ���Ԕ���
    if (tmpState.requestChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_REQUEST; // �ڑ����N�G�X�g��
    }
    if (tmpState.entryChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_ENTRY;   // �G���g���[��
    }
    if (tmpState.downloadChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_DOWNLOADING;      // �_�E�����[�h��
    }
    if (tmpState.bootableChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_BOOTABLE; // �u�[�g��������
    }
    if (tmpState.rebootChildBmp & bitmap)
    {
        return MBP_CHILDSTATE_REBOOT;  // ���u�[�g�ς�
    }

    return MBP_CHILDSTATE_CONNECTING;  // �R�l�N�g��
}


/*---------------------------------------------------------------------------*
  Name:         MBP_AddBitmap

  Description:  �ڑ���ԃr�b�g�b�v�Ɏq�@��ǉ�

  Arguments:    pBitmap     �ǉ�����r�b�g�}�b�v�ւ̃|�C���^.
                aid         �ǉ�����q�@��aid

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_AddBitmap(u16 *pBitmap, u16 aid)
{
    // �l�̕ύX���̕s�ӂȊ��荞�݂ɂ���ď�Ԃ����Ȃ��悤��
    // ���荞�݂��֎~���܂��B
    OSIntrMode enabled = OS_DisableInterrupts();
    *pBitmap |= (1 << aid);
    (void)OS_RestoreInterrupts(enabled);
}


/*---------------------------------------------------------------------------*
  Name:         MBP_RemoveBitmap

  Description:  �ڑ���ԃr�b�g�}�b�v����q�@���폜

  Arguments:    pBitmap     �폜����r�b�g�}�b�v�ւ̃|�C���^.
                aid         �폜����q�@��aid

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_RemoveBitmap(u16 *pBitmap, u16 aid)
{
    // �l�̕ύX���̕s�ӂȊ��荞�݂ɂ���ď�Ԃ����Ȃ��悤��
    // ���荞�݂��֎~���܂��B
    OSIntrMode enabled = OS_DisableInterrupts();
    *pBitmap &= ~(1 << aid);
    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
  Name:         MBP_DisconnectChildFromBmp

  Description:  �q�@���ؒf���ꂽ���ɂ��ׂẴr�b�g�}�b�v����q�@�̃t���O���폜
                ���܂��B

  Arguments:    aid     �ؒf���ꂽ�q�@��aid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_DisconnectChildFromBmp(u16 aid)
{
    u16     aidMask = (u16)~(1 << aid);

    OSIntrMode enabled = OS_DisableInterrupts();

    mbpState.connectChildBmp &= aidMask;
    mbpState.requestChildBmp &= aidMask;
    mbpState.entryChildBmp &= aidMask;
    mbpState.downloadChildBmp &= aidMask;
    mbpState.bootableChildBmp &= aidMask;
    mbpState.rebootChildBmp &= aidMask;

    (void)OS_RestoreInterrupts(enabled);
}

/*---------------------------------------------------------------------------*
  Name:         MBP_DisconnectChild

  Description:  �q�@��ؒf���܂��B

  Arguments:    aid     �ؒf����q�@��aid.

  Returns:      None.
 *---------------------------------------------------------------------------*/
static inline void MBP_DisconnectChild(u16 aid)
{
    // �����I��MB_Disconnect���p�ӂ���邩������܂���B
    MBP_Printf(" WM_Disconnect %d\n", aid);
    MBP_DisconnectChildFromBmp(aid);
    MB_DisconnectChild(aid);
}

/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildInfo

  Description:  �q�@�̃��[�U���ւ̃|�C���^���擾���܂��B

  Arguments:    child_aid   �q�@��aid.

  Returns:      ���[�U���ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
const MBPChildInfo *MBP_GetChildInfo(u16 child_aid)
{
    if (!(mbpState.connectChildBmp & (1 << child_aid)))
    {
        return NULL;
    }
    return &childInfo[child_aid - 1];
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetPlayerNo

  Description:  �q�@�����u�[�g���čĐڑ�������ɁA�}���`�u�[�g�q�@�����������
                �v���C���[�ԍ�(aid)��MAC�A�h���X�����Ɍ������擾�ł��܂��B

  Arguments:    macAddress      �q�@��MAC�A�h���X.

  Returns:      �}���`�u�[�g�ڑ����̎q�@aid.
 *---------------------------------------------------------------------------*/
u16 MBP_GetPlayerNo(const u8 *macAddress)
{
    u16     i;

    for (i = 1; i < MBP_CHILD_MAX + 1; i++)
    {
        if ((mbpState.connectChildBmp & (1 << i)) == 0)
        {
            continue;
        }
        if (WM_IsBssidEqual(macAddress, childInfo[i - 1].macAddress))
        {
            return childInfo[i - 1].playerNo;
        }
    }

    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         MBP_GetChildMacAddress

  Description:  �}���`�u�[�g�q�@��MAC�A�h���X���擾

  Arguments:    aid     �q�@��aid.

  Returns:      MAC�A�h���X�ւ̃|�C���^.
 *---------------------------------------------------------------------------*/
const u8 *MBP_GetChildMacAddress(u16 aid)
{
    if (!(mbpState.connectChildBmp & (1 << aid)))
    {
        return NULL;
    }
    return childInfo[aid - 1].macAddress;
}
