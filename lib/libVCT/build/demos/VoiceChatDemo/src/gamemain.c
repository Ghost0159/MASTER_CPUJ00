#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "dbs.h"

#include "gamemain.h"

extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;  // �Q�[��������\���� 
extern KeyControl stKeyCnt;    // �L�[���͐���\���� 
extern DWCFriendsMatchControl stDwcCnt;    // DWC����\���� 

extern DWCUserData stUserData;  // �{�̌ŗL�̃��[�UID(�{���̓��[�U�̓��͂ł�������Q�[�����ƂɊ��蓖�Ă�����̂ł������肷��l)
extern GameSaveData saveData;  

static volatile int stNameCbLevel; // �v���C���[���擾�����҂����� 

static void LoginCallback(DWCError error, int profileID, void* param);
static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param);
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param);
static void DeleteFriendListCallback(int deletedIndex, int srcIndex,void* param);
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param);
static void LoadFromServerCallback(BOOL success, int profileID, char* data, int len, void* param);

/*---------------------------------------------------------------------------*
  �I�t���C�������C���֐�
 *---------------------------------------------------------------------------*/
GameMode GameMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_MAIN];
    GameMode returnSeq = GAME_MODE_NUM;
    int curIdx = 0;

    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );
        
        ReadKeyData();  // �L�[�f�[�^�擾 

        DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V 

        if (DWC_GetLastError(NULL)){
            // ���O�C���E�T�[�o�A�b�v�f�[�g���s���̏���
            ShutdownInet();  // �l�b�g���[�N�ؒf 
            DWC_ClearError();
            stGameCnt.blocking = FALSE;

            returnSeq = GAME_MODE_MAIN;
        }

        if (stGameCnt.blocking){
            // DWC�������̓L�[������֎~����
            dbs_DemoUpdate();
            //SVC_WaitVBlankIntr();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();

            // �X�^�b�N���`�F�b�N
            OS_CheckStack(OS_GetCurrentThread());
            continue;
        }

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă�����F�؊���
        if (returnSeq == GAME_MODE_LOGIN){
            // ���O�C�����I������Ă����ꍇ
            if (stGameCnt.userData.isValidProfile){
               // �F�ؐ����Ńv���t�@�C��ID���擾�ł����ꍇ�̓��[�v�𔲂���
                 break;
            }
            else {
                // �F�؎��s�̏ꍇ�̓��[�h��i�߂Ȃ��悤�ɂ���
                returnSeq = GAME_MODE_NUM;
            }
        }
        else if (returnSeq != GAME_MODE_NUM){
            // ����ȊO�̏ꍇ�͕K��������
            break;
        }
            
        ////////// �ȉ��L�[���쏈��
        if (stKeyCnt.trg & PAD_BUTTON_A){
           // A�{�^���Ń��j���[����
            returnSeq = gameSeq->menuList[curIdx].mode;
            stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

            switch (returnSeq){
            case GAME_MODE_LOGIN:  // ���O�C�� 

                // DWC���C�u����������
                // ���C�u�������g���\�P�b�g�̑���M�o�b�t�@�T�C�Y��
                // �f�t�H���g�̂܂܁i8KByte�j�ɂ��Ă���
                DWC_InitFriendsMatch(&stDwcCnt, &stUserData, GAME_PRODUCTID, GAME_NAME,
                                     GAME_SECRET_KEY, 0, 0,
                                     stGameCnt.friendList.keyList, GAME_MAX_FRIEND_LIST);

                // �F�ؗp�֐����g���ă��O�C��
                DWC_LoginAsync(L"Name", NULL, LoginCallback, &returnSeq);
                break;
            default:
                break;
            }
        }
        else if (stKeyCnt.trg & PAD_KEY_UP){
            // �\���L�[��ŃJ�[�\���ړ�
            curIdx--;
            if (curIdx < 0) curIdx = gameSeq->numMenu-1;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if (stKeyCnt.trg & PAD_KEY_DOWN){
             // �\���L�[���ŃJ�[�\���ړ�
            curIdx++;
            if (curIdx >= gameSeq->numMenu) curIdx = 0;
            // ���j���[���X�g�ĕ\��
            DispMenuMsgWithCursor(gameSeq, curIdx, NULL);
        }
        else if ( stKeyCnt.trg & PAD_BUTTON_Y){
            Heap_Dump();
        }
        ////////// �L�[���쏈�������܂�

        dbs_DemoUpdate();
        //SVC_WaitVBlankIntr();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
        Heap_Debug();
        dbs_DemoLoad();
        // �X�^�b�N���`�F�b�N
        OS_CheckStack(OS_GetCurrentThread());
    }

    return returnSeq;
}

/*---------------------------------------------------------------------------*
  ���O�C���R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void LoginCallback(DWCError error, int profileID, void *param)
{
    BOOL result;
    u64 friend_key;
    
    if (error == DWC_ERROR_NONE){
        // �F�ؐ����A�v���t�@�C��ID�擾
        OS_TPrintf("Login succeeded. profileID = %u\n\n", profileID);
        stGameCnt.userData.profileID = profileID;
        stGameCnt.userData.isValidProfile = TRUE;

        // ingamesn�`�F�b�N�̌��ʂ��擾����
        if( DWC_GetIngamesnCheckResult() == DWC_INGAMESN_INVALID )
        {
            // �s�K�؂Ȗ��O�Ɣ��f���ꂽ�ꍇ�͓��ʂȏ������K�v
            OS_TPrintf("BAD ingamesn is detected by NAS.\n");
        }

        // �F�B�o�^���쐬
        friend_key = DWC_CreateFriendKey( &stUserData );
		if( friend_key ){
            OS_TPrintf("friend_key = %lld\n", friend_key);
        }
        
        // �F�B���X�g���������J�n
        result = DWC_UpdateServersAsync(stGameCnt.userData.playerName,
                                        UpdateServersCallback, param,
                                        FriendStatusCallback, param,
                                        DeleteFriendListCallback, param);
        if (result == FALSE){
           // �Ă�ł�������ԁi���O�C�����������Ă��Ȃ���ԁj�ŌĂ񂾎��̂�
            // FALSE���Ԃ��Ă���̂ŁA���ʂ�TRUE
            OS_Panic("--- DWC_UpdateServersAsync error teminated.\n");
        }

        // �X�g���[�W�T�[�o�Z�[�u�E���[�h�����ʒm�R�[���o�b�N��o�^����
        // Register a callback for making notification that storage save/load is complete
        DWC_SetStorageServerCallback(SaveToServerCallback,
                                     LoadFromServerCallback);
    }
    else {
        // �F�؎��s
        DispErrorMessage();
        OS_TPrintf("Login failed. %d\n\n", error);

        // ������x�ŏ�����
        *(GameMode *)param = GAME_MODE_NUM;

        stGameCnt.blocking = FALSE;  // �u���b�L���O���� 
    }
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�������������R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void UpdateServersCallback(DWCError error, BOOL isChanged, void* param)
{
#pragma unused(param)
    int i;

    if (error == DWC_ERROR_NONE){
        // �F�B���X�g������������
        OS_TPrintf("Updating servers succeeded.\n");

        if (isChanged){
            // �F�B���X�g���ύX����Ă�����Z�[�u����
            // �i�Z�[�u�̂���j
            MI_CpuCopy32(&stGameCnt.friendList, &saveData.friendList, sizeof(GameFriendList));
        }

        // �F�B�̒ʐM��ԂƖ��O���擾����
        stNameCbLevel = 0;
        for (i = 0; i < GAME_MAX_FRIEND_LIST; i++){
            if (DWC_IsValidFriendData(&stGameCnt.friendList.keyList[i])){
                stGameCnt.friendStatus[i] =
                    DWC_GetFriendStatus(&stGameCnt.friendList.keyList[i], NULL);
    
                // �v���C���[�������[�h
                if (DWC_LoadOthersDataAsync("\\dwc_name", i, (void *)i)){
                    stNameCbLevel++;
                }
            }
        }

        // �F�B�̖��O���擾�ł���܂ł����ő҂�
        while (stNameCbLevel){
            ReadKeyData();  // �L�[�f�[�^�擾 

            DWC_ProcessFriendsMatch();  // DWC�ʐM�����X�V 

            dbs_DemoUpdate();
            OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
            dbs_DemoLoad();
        }

        // �F�B���X�g��\������
        DispFriendList(FALSE);
    }
    else {
        // ���s���Ă����ɉ������Ȃ�
        OS_TPrintf("Failed to update GameSpy servers. %d\n\n", error);
    }

    stGameCnt.blocking = FALSE;  // �u���b�L���O���� 
}

/*---------------------------------------------------------------------------*
   �F�B��ԕω��ʒm�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void FriendStatusCallback(int index, u8 status, const char* statusString, void* param)
{
#pragma unused(param)
#ifdef SDK_FINALROM
#pragma unused(index, status, statusString)
#endif

    OS_TPrintf("Friend[%d] changed status -> %d (statusString : %s).\n",
               index, status, statusString);
}


/*---------------------------------------------------------------------------*
  �F�B���X�g�폜�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void DeleteFriendListCallback(int deletedIndex, int srcIndex, void* param)
{
#pragma unused(param)
#ifdef SDK_FINALROM
#pragma unused(deletedIndex, srcIndex)
#endif
#if 0
     // �F�B�f�[�^�̍폜�E�O�l�߂ɍ��킹�āA����ɑΉ�����f�[�^���O�ɋl�߂�
    if (index < GAME_MAX_FRIEND_LIST-1){
        // �F�B�̒ʐM��Ԃ�O�ɋl�߂�
        MI_CpuCopy8(&stGameCnt.friendStatus[index+1], &stGameCnt.friendStatus[index],
                    (u32)(GAME_MAX_FRIEND_LIST-index-1));

        // �F�B�̒ʐM��Ԃ�O�ɋl�߂�
        MI_CpuCopy8(&stGameCnt.friendList.playerName[index+1],
                    &stGameCnt.friendList.playerName[index],
                    (u32)(GAME_MAX_PLAYER_NAME*(GAME_MAX_FRIEND_LIST-index-1)));
    }

    stGameCnt.friendStatus[GAME_MAX_FRIEND_LIST-1] = DWC_STATUS_OFFLINE;
    MI_CpuClear32(&stGameCnt.friendList.playerName[GAME_MAX_FRIEND_LIST-1],
                  GAME_MAX_PLAYER_NAME);

#else
    OS_TPrintf("friend[%d] was deleted (equal friend[%d]).\n",
               deletedIndex, srcIndex);
#endif
}


/*---------------------------------------------------------------------------*
  �X�g���[�W�T�[�o�ւ̃f�[�^�Z�[�u�����R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void SaveToServerCallback(BOOL success, BOOL isPublic, void* param)
{
#ifdef SDK_FINALROM
#pragma unused(success, isPublic, param)
#endif

    OS_TPrintf("Saved data to server. %08x\n", (u32)param);
    OS_TPrintf("result %d, isPublic %d.\n", success, isPublic);
}


/*---------------------------------------------------------------------------*
  �X�g���[�W�T�[�o����̃f�[�^���[�h�����R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
static void LoadFromServerCallback(BOOL success, int profileID, char* data, int len, void* param)
{
#ifdef SDK_FINALROM
#pragma unused(profileID, len)
#endif

    OS_TPrintf("Loaded data from server. %08x\n", (u32)(param));
    OS_TPrintf("result %d, profileID %u, data '%s', len %d\n",
               success, profileID, data, len);

    if (success){
        // �ėpkey/value�^�����񑀍�֐����g���v���C���[�����擾
        (void)DWC_GetCommonValueString("dwc_name",
                                       stGameCnt.friendList.playerName[(int)param],
                                       data, '\\');

        stNameCbLevel--;
    }
}


