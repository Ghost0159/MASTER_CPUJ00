/*---------------------------------------------------------------------------*
  Project:  NitroSDK - MB - libraries
  File:     mb_parent.c

  Copyright 2003-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: mb_parent.c,v $
  Revision 1.59  2007/07/31 00:58:42  yosizaki
  update copyright

  Revision 1.58  2007/07/30 03:32:38  yosizaki
  fix about MB_RegisterFile

  Revision 1.57  2006/11/16 12:42:32  yosizaki
  add workaround for MBi_ReloadCache().

  Revision 1.56  2006/01/18 02:11:30  kitase_hirotake
  do-indent

  Revision 1.55  2005/12/21 14:25:56  yosizaki
  fix about page-cache.

  Revision 1.54  2005/12/13 10:49:04  yosizaki
  remove temporary hook for SEND_FAILED

  Revision 1.53  2005/12/12 07:32:18  yosizaki
  fix about nextb.

  Revision 1.52  2005/12/09 08:17:53  yosizaki
  add hook for SEND_FAILED (temporary)

  Revision 1.51  2005/03/14 07:09:58  sato_masaki
  MB_GetGameEntryBitmap()�֐���ǉ��B

  Revision 1.50  2005/02/28 05:26:24  yosizaki
  do-indent.

  Revision 1.49  2005/02/21 00:39:34  yosizaki
  replace prefix MBw to MBi.

  Revision 1.48  2005/02/18 11:56:03  yosizaki
  fix around hidden warnings.

  Revision 1.47  2005/01/27 07:04:43  takano_makoto
  small fix.

  Revision 1.46  2004/12/07 10:20:37  yosizaki
  fix to reload cache from correct archive. (not only "rom")

  Revision 1.45  2004/11/22 12:44:14  takano_makoto
  �������N�G�X�g���M�̏�����mb_block�Ƃ��ĕ���

  Revision 1.44  2004/11/11 11:47:26  yosizaki
  add cache-reloading.

  Revision 1.43  2004/11/10 13:15:48  yosizaki
  change to use cache-system.

  Revision 1.42  2004/11/02 18:12:04  takano_makoto
  fix comments.

  Revision 1.41  2004/11/02 10:23:01  yosizaki
  fix comment.

  Revision 1.40  2004/10/21 13:43:57  takano_makoto
  MB_END���̃��[�N�J���ʒu��ύX

  Revision 1.39  2004/10/19 06:35:52  sato_masaki
  bug fix

  Revision 1.38  2004/10/12 05:39:21  sato_masaki
  WM_APIID_RESET, WM_APIID_END �Ɋւ���Callback�G���[��MB_ERRCODE_WM_FAILURE�ɕύX�B

  Revision 1.37  2004/10/01 07:38:36  sato_masaki
  MB_COMM_PSTATE_WAIT_TO_SEND��Ԃ�ǉ��B�q�@�ւ�DownloadFileInfo���M�́A���̎q�@���󂯎�ꂽ������s��Ȃ��悤�ɕύX�B

  Revision 1.36  2004/09/29 13:03:15  sato_masaki
  MB_COMM_PSTATE_BOOT_STARTABLE��Ԃ�Ԃ��^�C�~���O��ύX�B

  Revision 1.35  2004/09/25 09:36:42  sato_masaki
  MB_CommGetChildUser()�ɂāA�q�@�����o�b�t�@�ɃR�s�[����悤�ɕύX�B

  Revision 1.34  2004/09/25 05:30:24  sato_masaki
  ���C�����[�v�ɂ����ČĂ΂꓾��API���s���ɁA���荞�݋֎~�����鏈����ǉ��B

  Revision 1.33  2004/09/22 09:41:32  sato_masaki
  MBi_CommParentSendBlock()�ɁA��������ǉ��B

  Revision 1.32  2004/09/21 01:39:15  sato_masaki
  WM�ɋN������G���[���AMB_ERRCODE_WM_FAILURE�ɓ���BMB_ERRCODE_MP_SENT_FAILURE��p�~�B

  Revision 1.31  2004/09/20 13:10:46  sato_masaki
  - MB_COMM_PSTATE_ERROR�@�Ɋւ��āAMBErrCode�񋓎q�̒l��arg�ɓn���悤�ɕύX�B
  - ���M�p�֐��ŁA�p���Ă���MB_ERROR���AMB_SENDFUNC_STATE_ERR(���C�u���������̒�`)�ɕύX�B

  Revision 1.30  2004/09/18 12:01:11  sato_masaki
  �q�@���M�w�b�_��`�ɁAreserved��݂���B

  Revision 1.29  2004/09/18 03:22:04  sato_masaki
  CHILD_CONNECTED, CHILD_DISCONNECTED����AID0�������ꍇ�̑Ώ��B

  Revision 1.28  2004/09/16 12:49:19  sato_masaki
  mb.h�ɂāAMBi_***�Œ�`���Ă����֐��A�^���AMB_***�ɕύX�B

  Revision 1.27  2004/09/15 06:51:59  sato_masaki
  WBT��`�ɓZ��鏈�����폜�B

  Revision 1.26  2004/09/15 06:25:29  sato_masaki
  MB_COMM_P_SENDLEN, MB_COMM_C_SENDLEN �̒�`�Ɉˑ����Ă���������ϐ����B

  Revision 1.25  2004/09/14 13:31:21  sato_masaki
  small fix

  Revision 1.24  2004/09/14 13:19:44  sato_masaki
  ���N�G�X�g�f�[�^��f�Љ����đ���悤�ɕύX�B

  Revision 1.23  2004/09/14 06:06:50  sato_masaki
  MB_CommBootRequest**, MB_CommStartSending**���C�����C�����B->mb.h�ֈړ�

  Revision 1.22  2004/09/13 13:08:55  sato_masaki
  remove member word from MB_CommBlockHeader

  Revision 1.21  2004/09/13 08:08:35  sato_masaki
  MBi_CommChangeParentStateCallbackOnly(), MBi_CommCallParentError()��ǉ��B
  �G���g���[���N�G�X�g���󂯂��ہAfileID�����ł͂Ȃ��A�q�@����̃��N�G�X�gGGID����r����悤�ɕύX�B

  Revision 1.20  2004/09/11 11:47:02  sato_masaki
  MB_UpdateGameInfoMember()�������o�[�ύX�������ꍇ���Ă΂�Ă����s����C���B

  Revision 1.19  2004/09/11 11:17:41  sato_masaki
  change mbc to pPwork

  Revision 1.18  2004/09/11 07:24:03  sato_masaki
  change MB_COMM_PSTATE_REQUESTED callback arg &req_data -> &req_data.userinfo
  
  Revision 1.17  2004/09/10 04:43:05  yosizaki
  change OS_Printf to MB_DEBUG_OUTPUT

  Revision 1.16  2004/09/10 02:54:44  sato_masaki
  small fix

  Revision 1.15  2004/09/10 02:54:06  sato_masaki
  CALLBACK_WM_STATE��`�Ɋւ���ύX�B

  Revision 1.14  2004/09/10 01:14:23  sato_masaki
  MbUserInfo��掞�ɁAplayerNo���Z�b�g���鏈����ǉ��B

  Revision 1.13  2004/09/09 14:01:47  sato_masaki
  mb_gameinfo.c�ɂ�����APlayerFlag�̃t�H�[�}�b�g�ύX�B�i��ƒ��j

  Revision 1.12  2004/09/09 02:49:14  sato_masaki
  MB_CALLBACK_CHILD_DISCONNECTED���́A�ڑ����ƃG���g���[���Ɋւ���N���A�����\����ύX�B

  Revision 1.11  2004/09/09 01:15:35  sato_masaki
  �o�^����Ă��Ȃ��AfileNo�ւ̃��N�G�X�g�͋���KICK���鏈����ǉ��B

  Revision 1.10  2004/09/08 07:19:18  sato_masaki
  small fix

  Revision 1.9  2004/09/08 04:38:53  sato_masaki
  - MB_CommBlockHeader����Afileid���폜(mb_common.h)�B����ɔ��������ύX�B
  - MB_CommBootRequestAll()�֐���ǉ��B�ύX�O��MB_CommBootRequest()�ƌ݊��̋@�\�����֐��ł��B

  Revision 1.8  2004/09/08 01:26:13  sato_masaki
  �q�@�����MB_CommRequestData��MB_COMM_PSTATE_REQUESTED�R�[���o�b�N�̎��_�Ŋi�[����悤�ɕύX�B

  Revision 1.7  2004/09/07 11:55:14  sato_masaki
  - MB_CommBootRequest()��AID�w��̊֐��ɕύX�B
  - MB_COMM_PSTATE_CANCEL��MB_COMM_PSTATE_END�ɖ��̕ύX�B
    MB���C�u�����̏I���R�[���o�b�N�́AMB_COMM_PSTATE_END�݂̂Ƃ���B
  - USER���N�G�X�g�̃N���A����^�C�~���O��ύX�B
  - Boot���N�G�X�g�܂��̏�����ύX�B

  Revision 1.6  2004/09/07 04:36:00  sato_masaki
  IsChildAidValid()��p���A�q�@AID�̃`�F�b�N�����d���B
  �ꕔ�̃R�[���o�b�N�ɂāA������n���悤�ɂ����B

  Revision 1.5  2004/09/06 09:48:39  sato_masaki
  small fix

  Revision 1.4  2004/09/04 10:26:13  sato_masaki
  �l�����������ɂ��Ă̏C���B

  Revision 1.3  2004/09/04 06:46:15  sato_masaki
  ������ߎ��̃R�[���o�b�N�X�e�[�g(MB_COMM_*STATE_MEMBER_FULL)��݂���B

  Revision 1.2  2004/09/04 04:38:24  sato_masaki
  �_�E�����[�h�l�������`�F�b�N�������B
  �q�@�ł�MB_COMM_CSTATE_REQ_REFUSED���Ԃ��Ă��܂��B

  Revision 1.1  2004/09/03 07:04:36  sato_masaki
  �t�@�C�����@�\�ʂɕ����B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/

#include "mb_private.h"

// ----------------------------------------------------------------------------
// definition

#define MB_SEND_THRESHOLD                       2       // send�̂Ƃ�2block�����܂ő���

// ----------------------------------------------------------------------------
// variable

/* �ŐV�� MP �Ŏq�@����]���v�����󂯂��t�@�C��ID�������r�b�g�Z�b�g */
static int any_recv_bitmap = 0;

// ----------------------------------------------------------------------------
// static funciton's prototypes

// --- for parent
static void MBi_CommChangeParentState(u16 child, int state, void *arg);
static void MBi_CommChangeParentStateCallbackOnly(u16 child, int state, void *arg);
static void MBi_CommParentRecvDataPerChild(void *arg, u16 child);
static void MBi_CommParentRecvData(void *arg);
static int MBi_CommParentSendMsg(u8 type, u16 pollbmp);
static int MBi_CommParentSendDLFileInfo(void);
static int MBi_CommParentSendBlock(void);
static int MBi_CommParentSendData(void);
static void MBi_calc_sendblock(u8 file_id);
static u16 MBi_calc_nextsendblock(u16 next_block, u16 next_block_req);

// --- miscellany
static inline u16 max(u16 a, u16 b);
static BOOL IsChildAidValid(u16 child_aid);
static void MBi_CommCallParentError(u16 aid, u16 errcode);

/*  ============================================================================

    �e�@�p�֐�

    ============================================================================*/

/*---------------------------------------------------------------------------*
  Name:         MB_CommSetParentStateCallback

  Description:  �e�@�C�x���g�R�[���o�b�N�̐ݒ�

  Arguments:    callback       �w�肷��R�[���o�b�N�֐�.

  Returns:      None.
 *---------------------------------------------------------------------------*/

void MB_CommSetParentStateCallback(MBCommPStateCallback callback)
{
    OSIntrMode enabled;

    SDK_ASSERT(pPwork != 0);

    enabled = OS_DisableInterrupts();  /* ���荞�݋֎~ */

    pPwork->parent_callback = callback;

    (void)OS_RestoreInterrupts(enabled);        /* ���荞�݋֎~���� */
}

/*---------------------------------------------------------------------------*
  Name:         MB_CommGetParentState

  Description:  �e�q�@�ɑ΂���e�@��Ԃ̎擾

  Arguments:    child_aid(WM��AID�\���`���ɍ��킹��1-15�ň���)

  Returns:      
 *---------------------------------------------------------------------------*/

int MB_CommGetParentState(u16 child_aid)
{

    if (pPwork && IsChildAidValid(child_aid))
    {
        return pPwork->p_comm_state[child_aid - 1];
    }
    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         MB_CommGetChildUser

  Description:  �ڑ����Ă���q�@�̃��[�U�[���擾

  Arguments:    child(WM��AID�\���`���ɍ��킹��1-15�ň���)

  Returns:      MbUser structure
 *---------------------------------------------------------------------------*/

const MBUserInfo *MB_CommGetChildUser(u16 child_aid)
{
    OSIntrMode enabled = OS_DisableInterrupts();        /* ���荞�݋֎~ */

    if (pPwork && IsChildAidValid(child_aid))
    {
        MI_CpuCopy8(&pPwork->childUser[child_aid - 1], &pPwork->childUserBuf, sizeof(MBUserInfo));
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return &pPwork->childUserBuf;
    }
    (void)OS_RestoreInterrupts(enabled);        /* ���荞�݋֎~���� */
    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         MB_CommGetChildrenNumber

  Description:  �ڑ����Ă���q�@�̐l���擾

  Arguments:    

  Returns:      number of children
 *---------------------------------------------------------------------------*/

u8 MB_CommGetChildrenNumber(void)
{
    if (pPwork)
    {
        return pPwork->child_num;
    }
    return 0;
}

/*---------------------------------------------------------------------------*
  Name:         MB_GetGameEntryBitmap

  Description:  �z�M����GameRegistry�ɑ΂��ăG���g���[���Ă���
                AID�r�b�g�}�b�v���擾����B

  Arguments:    game_req      - �ΏۂƂȂ�GameRegistry�ւ̃|�C���^

  Returns:      �w�肵��GameRegistry�ɃG���g���[���Ă���AID�r�b�g�}�b�v
                (�e�@AID:0�A�q�@AID:1-15)
                �Q�[�����z�M���ł͂Ȃ��ꍇ�A�Ԃ�l��0�ƂȂ�B
                �i�Q�[�����z�M���̏ꍇ�A�K���e�@AID:0���G���g���[�����o�[��
                  �܂܂�Ă���B�j
 *---------------------------------------------------------------------------*/

u16 MB_GetGameEntryBitmap(const MBGameRegistry *game_reg)
{
    int     i;
    for (i = 0; i < MB_MAX_FILE; i++)
    {
        if ((pPwork->fileinfo[i].active) && ((u32)pPwork->fileinfo[i].game_reg == (u32)game_reg))
        {
            return pPwork->fileinfo[i].gameinfo_child_bmp;
        }
    }
    return 0;
}


/*---------------------------------------------------------------------------*
  Name:         MB_CommIsBootable

  Description:  �u�[�g�\���𔻒�

  Arguments:    child_aid - �Ώێq�@��AID

  Returns:      yes - TRUE  no - FALSE
 *---------------------------------------------------------------------------*/

BOOL MB_CommIsBootable(u16 child_aid)
{
    SDK_ASSERT(pPwork != NULL);

    if (pPwork && IsChildAidValid(child_aid))
    {
        /* ���M���������q�@�̏ꍇ�ATRUE��Ԃ��B */
        if (pPwork->p_comm_state[child_aid - 1] == MB_COMM_PSTATE_SEND_COMPLETE)
        {
            return TRUE;
        }
    }
    return FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         MB_CommResponseRequest

  Description:  �q�@����̐ڑ����N�G�X�g�ɑ΂��鉞�����w��

  Arguments:    None.

  Returns:      success - TRUE  failed - FALSE
 *---------------------------------------------------------------------------*/

BOOL MB_CommResponseRequest(u16 child_aid, MBCommResponseRequestType ack)
{
    u16     req;
    int     state;
    OSIntrMode enabled;

    SDK_ASSERT(pPwork != NULL);

    enabled = OS_DisableInterrupts();  /* ���荞�݋֎~ */

    switch (ack)
    {
    case MB_COMM_RESPONSE_REQUEST_KICK:
        state = MB_COMM_PSTATE_REQUESTED;
        req = MB_COMM_USER_REQ_KICK;
        break;
    case MB_COMM_RESPONSE_REQUEST_ACCEPT:
        state = MB_COMM_PSTATE_REQUESTED;
        req = MB_COMM_USER_REQ_ACCEPT;
        break;

    case MB_COMM_RESPONSE_REQUEST_DOWNLOAD:
        state = MB_COMM_PSTATE_WAIT_TO_SEND;
        req = MB_COMM_USER_REQ_SEND_START;
        break;

    case MB_COMM_RESPONSE_REQUEST_BOOT:
        state = MB_COMM_PSTATE_SEND_COMPLETE;
        req = MB_COMM_USER_REQ_BOOT;
        break;
    default:
        (void)OS_RestoreInterrupts(enabled);    /* ���荞�݋֎~���� */
        return FALSE;
    }

    if (pPwork && IsChildAidValid(child_aid))
    {
        if (pPwork->p_comm_state[child_aid - 1] == state)
        {
            pPwork->req2child[child_aid - 1] = req;
            (void)OS_RestoreInterrupts(enabled);        /* ���荞�݋֎~���� */
            return TRUE;
        }
    }

    (void)OS_RestoreInterrupts(enabled);        /* ���荞�݋֎~���� */

    return FALSE;
}


/*---------------------------------------------------------------------------*
  Name:         MBi_CommChangeParentState

  Description:  �e�@��Ԃ̕ύX

  Arguments:    child(WM��AID�\���`���ɍ��킹��1-15�ň����B0�Ԃ͐e��\��), 
                state

  Returns:      
 *---------------------------------------------------------------------------*/

static void MBi_CommChangeParentState(u16 child, int state, void *arg)
{
    SDK_ASSERT(pPwork && child >= 0 && child <= WM_NUM_MAX_CHILD);

    /* child�̔ԍ����w�肳��Ă���Ȃ�A���̎q�@�ɑ΂����Ԃ�ύX�B */
    if (IsChildAidValid(child))
    {
        pPwork->p_comm_state[child - 1] = state;
    }

    MBi_CommChangeParentStateCallbackOnly(child, state, arg);

}

/*---------------------------------------------------------------------------*
  Name:         MBi_CommChangeParentStateCallbackOnly

  Description:  �q�@��Ԓʒm���R�[���o�b�N�Ăяo���݂̂ōs��
                �����̏�Ԃ͕ύX����

  Arguments:    

  Returns:      
 *---------------------------------------------------------------------------*/

static void MBi_CommChangeParentStateCallbackOnly(u16 child, int state, void *arg)
{
    if (pPwork->parent_callback)       // ��ԑJ�ڃR�[���o�b�N
    {
        (*pPwork->parent_callback) (child, (u32)state, arg);
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBi_CommParentCallback

  Description:  �e�@�R�[���o�b�N�{��

  Arguments:    type:WM_TYPE event arg:callback argument

  Returns:      None.
 *---------------------------------------------------------------------------*/

void MBi_CommParentCallback(u16 type, void *arg)
{
    MB_COMM_WMEVENT_OUTPUT(type, arg);

    switch (type)
    {
    case MB_CALLBACK_INIT_COMPLETE:
        /* ���������� */
        /* Callback�����ɁAWMStartParentCallback�^��arg��n���B */
        MBi_CommChangeParentState(0, MB_COMM_PSTATE_INIT_COMPLETE, arg);
        break;

    case MB_CALLBACK_END_COMPLETE:
        /* �I������ */
        // �֐��̍Ō�Ŏ��s

        break;

    case MB_CALLBACK_CHILD_CONNECTED:
        {
            WMstartParentCallback *sparg = (WMstartParentCallback *)arg;

            /* AID��0(�e�@)�̏ꍇ�͖��� */
            if (sparg->aid == 0)
                break;

            if (sparg->aid >= 16)
            {
                OS_TWarning("Connected Illegal AID No. ---> %2d\n", sparg->aid);
                break;
            }

            MB_DEBUG_OUTPUT("child %d connected to get bootimage!\n", sparg->aid);

            /* Callback�����ɁAWMStartParentCallback�^��arg��n���B */
            MBi_CommChangeParentState(sparg->aid, MB_COMM_PSTATE_CONNECTED, arg);
        }
        break;

    case MB_CALLBACK_CHILD_DISCONNECTED:
        {
            WMstartParentCallback *sparg = (WMstartParentCallback *)arg;

            /* AID��0(�e�@)�̏ꍇ�͖��� */
            if (sparg->aid == 0)
                break;

            if (sparg->aid >= 16)
            {
                OS_TWarning("Disconnected Illegal AID No. ---> %2d\n", sparg->aid);
                break;
            }

            MB_DEBUG_OUTPUT("child %d disconnected \n", sparg->aid);

            /*  �q�@��������
               (�ؒf���N�������ꍇ�A����AID�Ɋւ���q�@���͏�������) */
            pPwork->childversion[sparg->aid - 1] = 0;
            MI_CpuClear8(&pPwork->childggid[sparg->aid - 1], sizeof(u32));
            MI_CpuClear8(&pPwork->childUser[sparg->aid - 1], sizeof(MBUserInfo));

            /* ��M�o�b�t�@���N���A */
            MBi_ClearParentPieceBuffer(sparg->aid);

            pPwork->req2child[sparg->aid - 1] = MB_COMM_USER_REQ_NONE;

            /* �v��FileID���Z�b�g����Ă����ꍇ�BID��-1�ɃN���A����B */
            if (pPwork->fileid_of_child[sparg->aid - 1] != -1)
            {
                u8      fileID = (u8)pPwork->fileid_of_child[sparg->aid - 1];
                u16     nowChildFlag = pPwork->fileinfo[fileID].gameinfo_child_bmp;
                u16     child = sparg->aid;

                pPwork->fileinfo[fileID].gameinfo_child_bmp &= ~(MB_GAMEINFO_CHILD_FLAG(child));
                pPwork->fileinfo[fileID].gameinfo_changed_bmp |= MB_GAMEINFO_CHILD_FLAG(child);
                pPwork->fileid_of_child[child - 1] = -1;
                pPwork->fileinfo[fileID].pollbmp &= ~(0x0001 << (child));

                MB_DEBUG_OUTPUT("Update Member (AID:%2d)\n", child);
            }

            /* �ڑ������N���A */
            if (pPwork->child_entry_bmp & (0x0001 << (sparg->aid)))
            {
                pPwork->child_num--;
                pPwork->child_entry_bmp &= ~(0x0001 << (sparg->aid));
            }

            /* MB_COMM_PSTATE_BOOT_REQUEST�̎��ɁADisconnect���ꂽ�ꍇ�A
               �u�[�g�ɂ��ʐM�I���Ƃ݂Ȃ��AMB_COMM_PSTATE_BOOT_STARTABLE��Ԃ�ʒm */
            if (pPwork->p_comm_state[sparg->aid - 1] == MB_COMM_PSTATE_BOOT_REQUEST)
            {
                MBi_CommChangeParentState(sparg->aid, MB_COMM_PSTATE_BOOT_STARTABLE, NULL);
            }

            /* Callback�����ɁAWMStartParentCallback�^��arg��Ԃ��B */
            MBi_CommChangeParentState(sparg->aid, MB_COMM_PSTATE_DISCONNECTED, arg);
            pPwork->p_comm_state[sparg->aid - 1] = MB_COMM_PSTATE_NONE;
        }
        break;

    case MB_CALLBACK_MP_PARENT_RECV:
        MBi_CommParentRecvData(arg);
        break;

    case MB_CALLBACK_MP_SEND_ENABLE:
        (void)MBi_CommParentSendData();
        break;

    case MB_CALLBACK_BEACON_SENT:
        {
            u8      i;
            /* �e�Q�[���ɑ΂��āAGameInfo�����o�[�����X�V */
            for (i = 0; i < MB_MAX_FILE; i++)
            {
                /* �o�^�t�@�C�����A�N�e�B�u�ŁA
                   �Q�[���̃G���g���[�����o�[�̕ύX�t���O�����Ă����ꍇ��
                   GameInfo�̃����o�[�����X�V����. */
                if (pPwork->fileinfo[i].active && pPwork->fileinfo[i].gameinfo_changed_bmp)
                {
                    MB_UpdateGameInfoMember(&pPwork->fileinfo[i].game_info,
                                            &pPwork->childUser[0],
                                            pPwork->fileinfo[i].gameinfo_child_bmp,
                                            pPwork->fileinfo[i].gameinfo_changed_bmp);
                    /* �X�V��A�ω���bmp���N���A */
                    pPwork->fileinfo[i].gameinfo_changed_bmp = 0;
                }
            }
        }
        /* Beacon��GameInfo���悹�Ĕ��M */
        MB_SendGameInfoBeacon(MBi_GetGgid(), MBi_GetTgid(), MBi_GetAttribute());
        break;

    case MB_CALLBACK_API_ERROR:
        /* ARM9��WM API���R�[���������_�ł̕Ԃ�l�G���[ */
        {
            u16     apiid, errcode;

            apiid = ((u16 *)arg)[0];
            errcode = ((u16 *)arg)[1];

            switch (errcode)
            {
            case WM_ERRCODE_INVALID_PARAM:
            case WM_ERRCODE_FAILED:
            case WM_ERRCODE_WM_DISABLE:
            case WM_ERRCODE_NO_DATASET:
            case WM_ERRCODE_FIFO_ERROR:
            case WM_ERRCODE_TIMEOUT:
                MBi_CommCallParentError(0, MB_ERRCODE_FATAL);
                break;
            case WM_ERRCODE_OPERATING:
            case WM_ERRCODE_ILLEGAL_STATE:
            case WM_ERRCODE_NO_CHILD:
            case WM_ERRCODE_OVER_MAX_ENTRY:
            case WM_ERRCODE_NO_ENTRY:
            case WM_ERRCODE_INVALID_POLLBITMAP:
            case WM_ERRCODE_NO_DATA:
            case WM_ERRCODE_SEND_QUEUE_FULL:
            case WM_ERRCODE_SEND_FAILED:
            default:
                MBi_CommCallParentError(0, MB_ERRCODE_WM_FAILURE);
                break;
            }
        }
        break;
    case MB_CALLBACK_ERROR:
        {
            /* WM API�R�[����ɕԂ��Ă����R�[���o�b�N�ɂ�����G���[ */
            WMCallback *pWmcb = (WMCallback *)arg;
            switch (pWmcb->apiid)
            {
            case WM_APIID_INITIALIZE:
            case WM_APIID_SET_LIFETIME:
            case WM_APIID_SET_P_PARAM:
            case WM_APIID_SET_BEACON_IND:
            case WM_APIID_START_PARENT:
            case WM_APIID_START_MP:
            case WM_APIID_SET_MP_DATA:
            case WM_APIID_START_DCF:
            case WM_APIID_SET_DCF_DATA:
            case WM_APIID_DISCONNECT:
            case WM_APIID_START_KS:
                /* �ȏ�̃G���[�́AWM�ŏ������̕K�v�ȃG���[ */
                MBi_CommCallParentError(0, MB_ERRCODE_FATAL);
                break;
            case WM_APIID_RESET:
            case WM_APIID_END:
            default:
                /* ���̑��̃G���[�ɂ��ẮA�R�[���o�b�N�G���[�Ƃ��ĕԂ� */
                MBi_CommCallParentError(0, MB_ERRCODE_WM_FAILURE);
                break;
            }
        }
        break;

    }


#if ( CALLBACK_WM_STATE == 1 )
    MBi_CommChangeParentState(0, (u32)(MB_COMM_PSTATE_WM_EVENT | type), arg);
#endif

    if (type == MB_CALLBACK_END_COMPLETE)
    {
        MBCommPStateCallback tmpCb = pPwork->parent_callback;

        /* ���[�N�̉�� */
        MI_CpuClearFast(pPwork, sizeof(MB_CommPWork));
        pPwork = NULL;
        if (tmpCb)                     // ��ԑJ�ڃR�[���o�b�N
        {
            (*tmpCb) (0, MB_COMM_PSTATE_END, NULL);
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_CommParentRecvDataPerChild

  Description:  �e�q�@�ɑ΂����M�f�[�^����

  Arguments:    arg : pointer to callback argument, child : child AID

  Returns:      None.
 *---------------------------------------------------------------------------*/

static void MBi_CommParentRecvDataPerChild(void *arg, u16 child)
{
    MBCommChildBlockHeader hd;
    int     state;
    u8     *p_data;

    // child �͈̔͂�1-15
    if (child == 0 || child > WM_NUM_MAX_CHILD)
    {
        return;
    }

    p_data = MBi_SetRecvBufferFromChild((u8 *)&((WMMpRecvData *)arg)->cdata[0], &hd, child);

    state = pPwork->p_comm_state[child - 1];

    MB_DEBUG_OUTPUT("RECV (CHILD:%2d)", child);
    MB_COMM_TYPE_OUTPUT(hd.type);

    switch (hd.type)                   // �f�[�^�^�C�v�ʂ̏���
    {
    case MB_COMM_TYPE_CHILD_FILEREQ:
        {
            // MB_COMM_PSTATE_CONNECTED�̏ꍇ�̂ݎ󂯓����
            if (state == MB_COMM_PSTATE_CONNECTED)
            {
                MBCommRequestData req_data;

                if (p_data == NULL)
                {
                    // �q�@�̃��N�G�X�g�f�[�^�����ׂđ����܂Ŏ��̃X�e�[�g�ɂ͈ڍs���Ȃ��B
                    break;
                }

                MI_CpuCopy8(p_data, &req_data, MB_COMM_REQ_DATA_SIZE);

                pPwork->childggid[child - 1] = req_data.ggid;
                pPwork->childversion[child - 1] = req_data.version;
                MB_DEBUG_OUTPUT("Child [%2d] MB_IPL_VERSION : %04x\n", child, req_data.version);
                MI_CpuCopy8(&req_data.userinfo, &pPwork->childUser[child - 1], sizeof(MBUserInfo));
                pPwork->childUser[child - 1].playerNo = child;
                /* Callback �����Ɏ󂯎����MBCommRequestData��n���B */
                MBi_CommChangeParentState(child, MB_COMM_PSTATE_REQUESTED, &req_data.userinfo);
            }

            if (state == MB_COMM_PSTATE_REQUESTED)
            {
                u8      i, entry_num = 0;
                u8      fileid = ((MBCommRequestData *)p_data)->fileid;

                /* ACTIVE�ł͂Ȃ�fileNo�̗v���A�s����fileid�A
                   ��������fileNo��GGID�ƃ��N�G�X�g���Ă���GGID����v���Ȃ�������
                   KICK����. */
                if (fileid >= MB_MAX_FILE
                    || pPwork->fileinfo[fileid].active == 0
                    || pPwork->childggid[child - 1] != pPwork->fileinfo[fileid].game_reg->ggid)
                {
                    pPwork->req2child[child - 1] = MB_COMM_USER_REQ_KICK;
                }
                else
                {
                    /* �ݒ�l���𒴉߂��Ă�����AKICK���� */
                    for (i = 0; i < WM_NUM_MAX_CHILD + 1; i++)
                    {
                        if (pPwork->fileinfo[fileid].gameinfo_child_bmp & (0x0001 << i))
                        {
                            entry_num++;
                        }
                    }

                    if (entry_num >= pPwork->fileinfo[fileid].game_reg->maxPlayerNum)
                    {
                        MB_DEBUG_OUTPUT("Member full (AID:%2d)\n", child);
                        /* ���N�G�X�g�͋����L�����Z�� */
                        pPwork->req2child[child - 1] = MB_COMM_USER_REQ_NONE;
                        MBi_CommChangeParentState(child, MB_COMM_PSTATE_MEMBER_FULL, NULL);
                        break;
                    }
                }

                switch (pPwork->req2child[child - 1])
                {
                case MB_COMM_USER_REQ_ACCEPT:
                    {

                        if (0 == (pPwork->child_entry_bmp & (0x0001 << (child))))
                        {
                            pPwork->child_num++;
                            pPwork->child_entry_bmp |= (0x0001 << (child));
                            pPwork->fileid_of_child[child - 1] = (s8)fileid;

                            pPwork->fileinfo[fileid].gameinfo_child_bmp |=
                                MB_GAMEINFO_CHILD_FLAG(child);
                            pPwork->fileinfo[fileid].gameinfo_changed_bmp |=
                                MB_GAMEINFO_CHILD_FLAG(child);
                            MB_DEBUG_OUTPUT("Update Member (AID:%2d)\n", child);
                            pPwork->req2child[child - 1] = MB_COMM_USER_REQ_NONE;

                            MBi_CommChangeParentState(child, MB_COMM_PSTATE_REQ_ACCEPTED, NULL);
                        }
                    }
                    break;

                case MB_COMM_USER_REQ_KICK:
                    MB_DEBUG_OUTPUT("Kick (AID:%2d)\n", child);
                    pPwork->req2child[child - 1] = MB_COMM_USER_REQ_NONE;
                    MBi_CommChangeParentState(child, MB_COMM_PSTATE_KICKED, NULL);
                    break;
                }
            }
        }
        break;

    case MB_COMM_TYPE_CHILD_ACCEPT_FILEINFO:

        /* MB_COMM_PSTATE_REQ_ACCEPTED�̏ꍇ�AMB_COMM_PSTATE_WAIT_TO_SEND�ɑJ�ڂ�����̂� */
        if (state == MB_COMM_PSTATE_REQ_ACCEPTED)
        {
            MBi_CommChangeParentState(child, MB_COMM_PSTATE_WAIT_TO_SEND, NULL);
        }

        /* MB_COMM_PSTATE_WAIT_TO_SEND�̏ꍇ�A
           ���N�G�X�g���ꂽ�t�@�C���ɑ΂��Apollbitmap��ǉ�����
           MB_COMM_PSTATE_SEND_PROCEED�ɑJ�ڂ����� */
        else if (state == MB_COMM_PSTATE_WAIT_TO_SEND)
        {
            // SendStart�̃g���K���������Ă����ꍇ�A�u���b�N���M��ԂɑJ��
            if (pPwork->req2child[child - 1] == MB_COMM_USER_REQ_SEND_START)
            {
                u8      fid = (u8)pPwork->fileid_of_child[child - 1];
                pPwork->fileinfo[fid].pollbmp |= (0x0001 << (child));
                pPwork->fileinfo[fid].currentb = 0;

                pPwork->req2child[child - 1] = MB_COMM_USER_REQ_NONE;
                MBi_CommChangeParentState(child, MB_COMM_PSTATE_SEND_PROCEED, NULL);
            }
        }
        break;

    case MB_COMM_TYPE_CHILD_CONTINUE:
        if (state == MB_COMM_PSTATE_SEND_PROCEED)
        {
            u8      fileid = (u8)pPwork->fileid_of_child[child - 1];

            if (fileid == (u8)-1)
                break;

            // ������ child ���炫�� nextSend �̂����ő�̂��̂𑗐M�ΏۂƂ���
            SDK_ASSERT(fileid < MB_MAX_FILE);
            SDK_ASSERT(pPwork->fileinfo[fileid].pollbmp);

            pPwork->fileinfo[fileid].nextb =
                MBi_calc_nextsendblock(pPwork->fileinfo[fileid].nextb, hd.data.req);
            any_recv_bitmap |= (1 << fileid);
        }
        break;

    case MB_COMM_TYPE_CHILD_STOPREQ:
        if (state == MB_COMM_PSTATE_SEND_PROCEED)
        {
            u8      fileid = (u8)pPwork->fileid_of_child[child - 1];

            if (fileid == (u8)-1)
                break;

            SDK_ASSERT(fileid < MB_MAX_FILE);

            pPwork->fileinfo[fileid].pollbmp &= ~(0x0001 << (child));

            MBi_CommChangeParentState(child, MB_COMM_PSTATE_SEND_COMPLETE, NULL);       // ���M����
        }
        else if (state == MB_COMM_PSTATE_SEND_COMPLETE)
        {
            if (pPwork->req2child[child - 1] == MB_COMM_USER_REQ_BOOT)
            {
                pPwork->req2child[child - 1] = MB_COMM_USER_REQ_NONE;
                MBi_CommChangeParentState(child, MB_COMM_PSTATE_BOOT_REQUEST, NULL);
                break;
            }
        }
        break;

    case MB_COMM_TYPE_CHILD_BOOTREQ_ACCEPTED:
        if (state == MB_COMM_PSTATE_BOOT_REQUEST)
        {
            /* �q�@�����BOOTREQ_ACCEPTED�́A��ԑJ�ڂɎg�p���܂���D */
            break;
        }

        break;

    default:
        break;
    }

}

/*---------------------------------------------------------------------------*
  Name:         MBi_CommParentRecvData

  Description:  �e�@�f�[�^��M

  Arguments:    arg : pointer to callback argument

  Returns:      None.
 *---------------------------------------------------------------------------*/

static void MBi_CommParentRecvData(void *arg)
{
    // ����arg�Ɏ�M�o�b�t�@�̃|�C���^�������Ă���
    WMmpRecvHeader *mpHeader = (WMmpRecvHeader *)arg;

    u16     i;
    WMmpRecvData *datap;

    // MBi_CommParentRecvDataPerChild�ɂē��v�I�ɕ]������ׁA�����ŏ�����
    for (i = 0; i < MB_MAX_FILE; i++)
    {
        if (pPwork->fileinfo[i].active)
            pPwork->fileinfo[i].nextb = 0;
    }
    any_recv_bitmap = 0;

    // �e�q�@����̎�M�f�[�^��\��
    for (i = 1; i <= WM_NUM_MAX_CHILD; ++i)
    {
        // AID==i �̎q�@�f�[�^�̐擪�A�h���X���擾
        datap = WM_ReadMPData(mpHeader, (u16)i);
        // AID==i �̎q�@�f�[�^�����݂����ꍇ
        if (datap != NULL)
        {
            // ��M�f�[�^�̕\��
            if (datap->length == 0xffff)
            {
            }
            else if (datap->length != 0)
            {
                // �e�q�@�f�[�^�ɂ��Ă̏���
                MBi_CommParentRecvDataPerChild(datap, i);
            }
        }
    }
}


/*---------------------------------------------------------------------------*
  Name:         MBi_CommParentSendMsg

  Description:  �e�@���烁�b�Z�[�W�𑗐M

  Arguments:    pollbmp

  Returns:      None.
 *---------------------------------------------------------------------------*/

static int MBi_CommParentSendMsg(u8 type, u16 pollbmp)
{
    MBCommParentBlockHeader hd;

    /* ���M�J�n�ʒm. (��������) */
    hd.type = type;

    (void)MBi_MakeParentSendBuffer(&hd, (u8 *)pPwork->common.sendbuf);
    return MBi_BlockHeaderEnd(MB_COMM_PARENT_HEADER_SIZE, pollbmp, pPwork->common.sendbuf);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_CommParentSendDLFileInfo

  Description:  �e�@����DownloadFileInfo�𑗐M

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/

static int MBi_CommParentSendDLFileInfo(void)
{
    MBCommParentBlockHeader hd;
    u8     *databuf = ((u8 *)pPwork->common.sendbuf) + MB_COMM_PARENT_HEADER_SIZE;
    u16     child;
    u8      i, fid;
    s8      send_candidate_fid = -1;
    static s8 prev_fid = -1;
    u8      file_req_num[MB_MAX_FILE];
    u16     pollbmp = 0;

    MI_CpuClear8(&file_req_num[0], sizeof(u8) * MB_MAX_FILE);

    // �q���v�����Ă���FileID���W�v
    for (child = 1; child <= WM_NUM_MAX_CHILD; child++)
    {
        if (pPwork->p_comm_state[child - 1] == MB_COMM_PSTATE_REQ_ACCEPTED)
        {
            // MB_COMM_PSTATE_REQ_ACCEPTED�Ώۂ̎q�@�̂݃J�E���g����
            ++(file_req_num[pPwork->fileid_of_child[child - 1]]);
        }
    }

    fid = (u8)prev_fid;

    for (i = 0; i < MB_MAX_FILE; i++)  // ���M�t�@�C��ID�����肷��
    {
#if 1
        fid = (u8)((fid + 1) % MB_MAX_FILE);

        if (pPwork->fileinfo[fid].active && file_req_num[fid] > 0)
        {
            send_candidate_fid = (s8)fid;
            break;
        }

#else
        if (pPwork->fileinfo[i].active)
        {
            if (file_req_num[i] > 0)
            {

                /* 
                   �������@
                   �i  ���̏ꍇ�A�����q�@���قȂ�t�@�C���Ń_�E�����[�h�J�n�҂����Ă����
                   DownloadFileInfo�̑��M���A�����̕��̃t�@�C���Ƀ��b�N���ꂽ��ԂɂȂ�B
                   �}���`�u�[�g����Ɋւ��Ă̖��͂Ȃ����A
                   �q�@�A�v���̃X�e�[�g��MB_COMM_CSTATE_REQ_ENABLE����i�܂Ȃ��Ȃ�̂�
                   (�e�@�����Download�����ɂ����̎q�@�Ɋւ��Ă̓G���g���[�������i�ށB)
                   �Ώ����K�v���B�j
                 */

                if (send_candidate_fid == -1 || file_req_num[i] > file_req_num[send_candidate_fid])
                {
                    send_candidate_fid = i;
                }

            }
        }
#endif

    }

    if (send_candidate_fid == -1)
        return MB_SENDFUNC_STATE_ERR;

    prev_fid = send_candidate_fid;

    // poll bitmap�ݒ�(���M����File�ԍ��ƈꏏ�̃��N�G�X�g���o���Ă���q�@�̂�)
    for (child = 1; child <= WM_NUM_MAX_CHILD; child++)
    {
        if (pPwork->p_comm_state[child - 1] == MB_COMM_PSTATE_REQ_ACCEPTED
            && pPwork->fileid_of_child[child - 1] == send_candidate_fid)
        {
            pollbmp |= (1 << child);
        }
    }

    MB_DEBUG_OUTPUT("DLinfo No %2d Pollbmp %04x\n", send_candidate_fid, pollbmp);

    // ��ԍŌ�ɗv�����󂯂��q�@��FileInfo�𑗂�
    hd.type = MB_COMM_TYPE_PARENT_DL_FILEINFO;
    hd.fid = send_candidate_fid;

    databuf = MBi_MakeParentSendBuffer(&hd, (u8 *)pPwork->common.sendbuf);
    if (databuf)
    {
        // ���M�o�b�t�@�Ƀf�[�^���R�s�[
        MI_CpuCopy8(&pPwork->fileinfo[send_candidate_fid].dl_fileinfo,
                    databuf, sizeof(MBDownloadFileInfo));
    }

    return MBi_BlockHeaderEnd(sizeof(MBDownloadFileInfo) + MB_COMM_PARENT_HEADER_SIZE, pollbmp,
                              pPwork->common.sendbuf);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_ReloadCache

  Description:  �w�肳�ꂽ�p���L���b�V���Ƀf�[�^�������[�h.

  Arguments:    p_task           param[0] �� MBiCacheInfo ���i�[�����^�X�N

  Returns:      None.
 *---------------------------------------------------------------------------*/
static void MBi_ReloadCache(MBiTaskInfo * p_task)
{
    MBiCacheInfo *const p_info = (MBiCacheInfo *) p_task->param[0];
    MBiCacheList *const p_list = (MBiCacheList *) p_task->param[1];
    FSFile  file[1];
    FSArchive   *arc;
    arc = FS_FindArchive(p_list->arc_name, (int)p_list->arc_name_len);
    if (!arc)
    {
        arc = p_list->arc_pointer;
    }

    FS_InitFile(file);

    /* MB_ReadSegment() ���̑ΏۃA�[�J�C�u���w�� */
    if (FS_OpenFileDirect(file,
                          arc,
                          p_info->src, p_info->src + p_info->len, (u32)~0))
    {
        if (FS_ReadFile(file, p_info->ptr, (int)p_info->len) == p_info->len)
        {
            /* ���̏�� READY �ɂ��Ă��\��Ȃ� */
            p_info->state = MB_CACHE_STATE_READY;
        }
        (void)FS_CloseFile(file);
    }

    /* �J�[�h�����ȂǂŃt�@�C���̓ǂݍ��݂Ɏ��s */
    if (p_info->state != MB_CACHE_STATE_READY)
    {
        /*
         * ���̃y�[�W��MB_CACHE_STATE_BUSY�̂܂ܒu���Ă�����
         * ���[�N���ăL���b�V���S�̂̃y�[�W�����������Ă��܂��̂�,
         * ��ɃC���f�b�N�X[0]�ɑ��݂���ROM�w�b�_(0x00000000-)
         * �Ɠ����A�h���X�ݒ��MB_CACHE_STATE_READY�ɂ��Ă���.
         * ����ɂ��, �o�b�t�@�ɕs��ȓ��e���܂܂ꂽ�܂܂ł����Ă�
         * �����ĎQ�Ƃ���邱�ƂȂ��Ăю���̃y�[�W�t�H���g�ΏۂɂȂ�.
         * (������, ���ۂɂ̓J�[�h�����������_�œ�x�ƌĂ΂�Ȃ��͂�)
         */
        p_info->src = 0;
        p_info->state = MB_CACHE_STATE_READY;
    }
}

/*---------------------------------------------------------------------------*
  Name:         MBi_CommParentSendBlock

  Description:  �e�@����Block�f�[�^�𑗐M

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/

static int MBi_CommParentSendBlock(void)
{
    MBCommParentBlockHeader hd;
    u8     *databuf;
    u8      i;
    MB_BlockInfo bi;

    // ���MFile�ԍ��̌���
    if (pPwork->file_num == 0)
        return MB_SENDFUNC_STATE_ERR;

    // ����������block�]���{��

    // ���M�Ώۂ̃t�@�C��������
    for (i = 0; i < MB_MAX_FILE; i++)
    {
        pPwork->cur_fileid = (u8)((pPwork->cur_fileid + 1) % MB_MAX_FILE);
        if (pPwork->fileinfo[pPwork->cur_fileid].active
            && pPwork->fileinfo[pPwork->cur_fileid].pollbmp)
        {
            MB_DEBUG_OUTPUT("Send File ID:%2d\n", pPwork->cur_fileid);
            break;
        }
    }
    if (i == MB_MAX_FILE)              // ���M�Ώۂ̃t�@�C�����Ȃ�
    {
        return MB_SENDFUNC_STATE_ERR;
    }

    /* ����t�@�C���ɑ΂��鑗�M�u���b�N�̎Z�� */
    MBi_calc_sendblock(pPwork->cur_fileid);

    // �u���b�N���̎擾
    if (!MBi_get_blockinfo(&bi,
                           &pPwork->fileinfo[pPwork->cur_fileid].blockinfo_table,
                           pPwork->fileinfo[pPwork->cur_fileid].currentb,
                           &pPwork->fileinfo[pPwork->cur_fileid].dl_fileinfo.header))
    {
        return MB_SENDFUNC_STATE_ERR;
    }
    /* ���M�p�P�b�g�̏��� */
    hd.type = MB_COMM_TYPE_PARENT_DATA;
    hd.fid = pPwork->cur_fileid;
    hd.seqno = pPwork->fileinfo[pPwork->cur_fileid].currentb;
    databuf = MBi_MakeParentSendBuffer(&hd, (u8 *)pPwork->common.sendbuf);

    /* �L���b�V���o�R�̃A�N�Z�X. (�������[���Ȃ��Ƀq�b�g) */
    {
        /* �u���b�N�I�t�Z�b�g���� CARD �A�h���X���v�Z���� */
        u32     card_addr = (u32)(bi.offset -
                                  pPwork->fileinfo[pPwork->cur_fileid].blockinfo_table.
                                  seg_src_offset[bi.segment_no] +
                                  pPwork->fileinfo[pPwork->cur_fileid].card_mapping[bi.segment_no]);
        /* �w��� CARD �A�h���X�ɑ΂��ăL���b�V�����[�h */
        MBiCacheList *const pl = pPwork->fileinfo[pPwork->cur_fileid].cache_list;
        if (!MBi_ReadFromCache(pl, card_addr, databuf, bi.size))
        {
            /* �L���b�V���~�X�Ȃ�^�X�N�X���b�h�փ����[�h�v�� */
            MBiTaskInfo *const p_task = &pPwork->cur_task;
            if (!MBi_IsTaskBusy(p_task))
            {
                /* �A���y�[�W�t�H���g������邽�߂̃��C�t�^�C�� */
                if (pl->lifetime)
                {
                    --pl->lifetime;
                }
                else
                {
                    /*
                     * ��ԃA�h���X�̎Ⴂ�L���b�V����j��.
                     * ���̕����̓���͍����������������.
                     */
                    MBiCacheInfo *pi = pl->list;
                    MBiCacheInfo *trg = NULL;
                    int     i;
                    for (i = 0; i < MB_CACHE_INFO_MAX; ++i)
                    {
                        if (pi[i].state == MB_CACHE_STATE_READY)
                        {
                            if (!trg || (trg->src > pi[i].src))
                                trg = &pi[i];
                        }
                    }
                    if (!trg)
                    {
                        OS_TPanic("cache-list is invalid! (all the pages are locked)");
                    }
                    pl->lifetime = 2;
                    trg->state = MB_CACHE_STATE_BUSY;
                    trg->src = (card_addr & ~31);
                    p_task->param[0] = (u32)trg;        /* MBiCacheInfo* */
                    p_task->param[1] = (u32)pl; /* MBiCacheList* */
                    MBi_SetTask(p_task, MBi_ReloadCache, NULL, 4);
                }
            }
            return MB_SENDFUNC_STATE_ERR;
        }
    }

    return MBi_BlockHeaderEnd((int)(bi.size + MB_COMM_PARENT_HEADER_SIZE),
                              pPwork->fileinfo[pPwork->cur_fileid].pollbmp, pPwork->common.sendbuf);
}

/*---------------------------------------------------------------------------*
  Name:         MBi_CommParentSendData

  Description:  �e�@�f�[�^���M

  Arguments:    None.

  Returns:      None.
 *---------------------------------------------------------------------------*/

static int MBi_CommParentSendData(void)
{
    struct bitmap
    {
        u16     connected;
        u16     req;
        u16     kick;
        u16     boot;
        u16     mem_full;
    };
    struct bitmap bmp;
    u16     child;
    int     errcode;

    MI_CpuClear16(&bmp, sizeof(struct bitmap));

    // �e�q�@�ɑ΂��Ă̐e�@��Ԃ�]��
    for (child = 1; child <= WM_NUM_MAX_CHILD; child++)
    {

        switch (pPwork->p_comm_state[child - 1])
        {
        case MB_COMM_PSTATE_CONNECTED:
            bmp.connected |= (1 << child);
            break;

        case MB_COMM_PSTATE_REQ_ACCEPTED:
            bmp.req |= (1 << child);
            break;

        case MB_COMM_PSTATE_KICKED:
            bmp.kick |= (1 << child);
            break;

        case MB_COMM_PSTATE_SEND_PROCEED:
            break;

        case MB_COMM_PSTATE_BOOT_REQUEST:
            bmp.boot |= (1 << child);
            break;

        case MB_COMM_PSTATE_MEMBER_FULL:
            bmp.mem_full |= (1 << child);
            break;

        default:
            break;                     // ��L�ȊO�̏�Ԃ̓m�[�J�E���g
        }

    }
    /*
       Startmsg > DLFileInfo > Block
       �̗D�揇�ʂő��M���s��
     */
    if (bmp.boot)
    {
        errcode = MBi_CommParentSendMsg(MB_COMM_TYPE_PARENT_BOOTREQ, bmp.boot);
    }
    else if (bmp.connected)            // �G���g���[�v�������b�Z�[�W���M
    {
        errcode = MBi_CommParentSendMsg(MB_COMM_TYPE_PARENT_SENDSTART, bmp.connected);
    }
    else if (bmp.mem_full)             // �����o�[���߃��b�Z�[�W���M
    {
        errcode = MBi_CommParentSendMsg(MB_COMM_TYPE_PARENT_MEMBER_FULL, bmp.mem_full);
    }
    else if (bmp.kick)                 // �G���g���[���ۃ��b�Z�[�W���M
    {
        errcode = MBi_CommParentSendMsg(MB_COMM_TYPE_PARENT_KICKREQ, bmp.kick);
    }
    else if (bmp.req)                  // MbDownloadFileInfo�̑��M
    {
        errcode = MBi_CommParentSendDLFileInfo();
    }
    else                               // Block�f�[�^�̑��M
    {
        errcode = MBi_CommParentSendBlock();
    }

    // Connection�ێ��̂��߂�MP���M
    if (MB_SENDFUNC_STATE_ERR == errcode)
    {
        errcode = MBi_CommParentSendMsg(MB_COMM_TYPE_DUMMY, 0xffff);
    }

    return errcode;

}


/*---------------------------------------------------------------------------*
  Name:         MBi_calc_sendblock

  Description:  ���M����u���b�N���v�Z

  Arguments:    file_id - ���M����t�@�C����ID

  Returns:      
 *---------------------------------------------------------------------------*/

static void MBi_calc_sendblock(u8 file_id)
{
    /* �q�@����w��u���b�N�ւ̗v������M���Ă��Ȃ���΍X�V���Ȃ� */
    if ((any_recv_bitmap & (1 << file_id)) == 0)
    {
        return;
    }

    if (pPwork->fileinfo[file_id].active && pPwork->fileinfo[file_id].pollbmp)
    {
        if (pPwork->fileinfo[file_id].nextb <= pPwork->fileinfo[file_id].currentb &&
            pPwork->fileinfo[file_id].currentb <=
            pPwork->fileinfo[file_id].nextb + MB_SEND_THRESHOLD)
        {
            pPwork->fileinfo[file_id].currentb++;
        }
        else
        {
            pPwork->fileinfo[file_id].currentb = pPwork->fileinfo[file_id].nextb;
        }
        MB_DEBUG_OUTPUT("**FILE %2d SendBlock %d\n", file_id, pPwork->fileinfo[file_id].currentb);
    }

}

/*---------------------------------------------------------------------------*
  Name:         MBi_calc_nextsendblock

  Description:  ���ɑ���u���b�N��Ԃ�

  Arguments:    

  Returns:      
 *---------------------------------------------------------------------------*/

static u16 MBi_calc_nextsendblock(u16 next_block, u16 next_block_req)
{
    return max(next_block_req, next_block);
}


/*  ============================================================================

    miscellany functions

    ============================================================================*/

static inline u16 max(u16 a, u16 b)
{
    return (a > b) ? a : b;
}

static BOOL IsChildAidValid(u16 child_aid)
{
    return (child_aid >= 1 && child_aid <= WM_NUM_MAX_CHILD) ? TRUE : FALSE;
}

static void MBi_CommCallParentError(u16 aid, u16 errcode)
{
    MBErrorStatus e_stat;
    e_stat.errcode = errcode;

    MBi_CommChangeParentStateCallbackOnly(aid, MB_COMM_PSTATE_ERROR, &e_stat);
}
