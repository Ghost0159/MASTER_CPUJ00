#include <nitro.h>
#include <ninet/ip.h>
#include <dwc.h>

#include "main.h"
#include "init.h"
#include "utility.h"
#include "tp_input.h"
#include "wc.h"
#include "screen.h"
#include "dbs.h"
#include "mp.h"

#include "registerfriend.h"

extern DWCUserData stUserData; // ���[�U�f�[�^���i�[����\���́B
extern GameSequence gameSeqList[GAME_MODE_NUM];
extern GameControl stGameCnt;  // �Q�[��������\����
extern KeyControl stKeyCnt;    // �L�[���͐���\����

// �F�B�o�^��
static char stFriendKey[sizeof(DWCFriendData)];

// �T���v��MP�����p�F�B�f�[�^
static DWCFriendData stSampleFriendData ATTRIBUTE_ALIGN(32);

//�f�[�^�ʐM�p�o�b�t�@
static u16* gSendBuf ATTRIBUTE_ALIGN(32);//���M�p�o�b�t�@

/*---------------------------------------------------------------------------*
  �F�B�o�^���C���֐�
  *---------------------------------------------------------------------------*/
GameMode GameRegisterFriendMain(void)
{
    GameSequence *gameSeq = &gameSeqList[GAME_MODE_REG_FRIEND];
    GameMode returnSeq = GAME_MODE_NUM;
    RegFriendParam cntParam;
    int curIdx = 0;
    int friendIdx;
    int maxFigure;
    int validFlg;
    char tp_data;

    int i, j;

    tp_init();// �^�b�`�y�����͂̏�����
    
    cntParam.step   = 0;
    cntParam.figure = 0;

    MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));
    
    while (1){
        dbs_Print( 0, 0, "s:%d", DWC_GetMatchingState() );
        dbs_Print( 7, 0, "n:%d", DWC_GetNumConnectionHost() );
        dbs_Print( 0, 1, "w:%d", DWC_GetLinkLevel() );
        dbs_Print( 10,1, "p:%d", stGameCnt.userData.profileID );

        ReadKeyData();// �L�[�f�[�^�擾

        // ���ɐi�ނׂ����[�h���Z�b�g����Ă����甲����
        if (returnSeq != GAME_MODE_NUM){
            break;
        }
        
        ////////// �ȉ��L�[���쏈��
        if (cntParam.step == 0){
            // �R�}���h�I��
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // A�{�^���Ń��j���[����
                returnSeq = gameSeq->menuList[curIdx].mode;
                stGameCnt.blocking = gameSeq->menuList[curIdx].blocking;

                if ( curIdx == 0 ){
                    // ����������
                    DWC_CleanupInet();
                    
                    // MP�ʐM�p�f�[�^�쐬
                    DWC_CreateExchangeToken(&stUserData, &stSampleFriendData);
                    gSendBuf = (u16*)(&stSampleFriendData);
                    
                    mp_match(gSendBuf, stGameCnt.friendList.keyList);//MP�ʐM

                    friendIdx = GetAvailableFriendListIndex();// �����MP�f�[�^���Ȃ�
                    for(i = 0; i < friendIdx; i++){
                        for(j = i+1; j < friendIdx; j++){
                            if(DWC_IsEqualFriendData( (const DWCFriendData*)&stGameCnt.friendList.keyList[i], (const DWCFriendData*)&stGameCnt.friendList.keyList[j])){
                                MI_CpuClear8(&stGameCnt.friendList.keyList[j], sizeof(DWCFriendData));
                            }
                        }
                    }
                    // ���j���[���X�g�ĕ\��
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
                
                else if ((curIdx == 1) || (curIdx == 2)){
                    // �F�B�o�^�����͂ɂ��F�B�o�^�A
                    // �������̓C���f�b�N�X���w�肵�ėF�B�f�[�^���폜
                    cntParam.step   = 1;
                    cntParam.figure = 0;
                    cntParam.value  = 0;
                    
                    // ���j���[���X�g�ĕ\��
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                    
                    if (curIdx == 1){
                        maxFigure = 12;  // �F�B�o�^���͍ō��P�Q��
                    }
                    else {
                        maxFigure = 2;   // �F�B���X�g�C���f�b�N�X�͍ō��Q��
                    }
                }
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // �\���L�[��ŃJ�[�\���ړ�
                curIdx--;
                if (curIdx < 0) curIdx = gameSeq->numMenu-1;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // �\���L�[���ŃJ�[�\���ړ�
                curIdx++;
                if (curIdx >= gameSeq->numMenu) curIdx = 0;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        else {
            if(cntParam.figure < maxFigure){
                tp_data = tp_input();
				if(tp_data){
                    if(tp_data > 47){//0�`9�܂ł̐��l�����͂��ꂽ�ꍇ
                        stFriendKey[cntParam.figure] = tp_data;
                        if (cntParam.figure < maxFigure-1) cntParam.figure++;
                    }
                    else if(tp_data == 1){//<-�����͂��ꂽ�ꍇ
                        if (cntParam.figure > 0) cntParam.figure--;
                    }
                    else if(tp_data == 2){//->�����͂��ꂽ�ꍇ
                        if (cntParam.figure < maxFigure-1) cntParam.figure++;
                    }
                    // delete�p�f�[�^
                    cntParam.value = (u32)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
                // �^�b�`�y�����͂̂��߂̕\��
                PrintString(10, 4, 0xf,  "-------------");
                PrintString(10, 5, 0xf,  "l   l   l   l");
                PrintString(10, 6, 0xf,  "l 7 l 8 l 9 l");
                PrintString(10, 7, 0xf,  "l   l   l   l");
                PrintString(10, 8, 0xf,  "l-----------l");
                PrintString(10, 9, 0xf,  "l   l   l   l");
                PrintString(10, 10, 0xf, "l 4 l 5 l 6 l");
                PrintString(10, 11, 0xf, "l   l   l   l");
                PrintString(10, 12, 0xf, "l-----------l");
                PrintString(10, 13, 0xf, "l   l   l   l");
                PrintString(10, 14, 0xf, "l 1 l 2 l 3 l");
                PrintString(10, 15, 0xf, "l   l   l   l");
                PrintString(10, 16, 0xf, "l-----------l");
                PrintString(10, 17, 0xf, "l   l   l   l");
                PrintString(10, 18, 0xf, "l 0 l<--l-->l");
                PrintString(10, 19, 0xf, "l   l   l   l");
                PrintString(10, 20, 0xf, "-------------");

                PrintString(2, 22, 0xf, "Set: PUSH A   Calcel: PUSH B");
            }
            
            // ���l���͒�
            if (stKeyCnt.trg & PAD_BUTTON_A){
                // A�{�^���ŗF�B�̓o�^�E�폜���s��
                if (curIdx == 1){
                    // �F�B�o�^
                    if (cntParam.value){
                        friendIdx = GetAvailableFriendListIndex();
                        if (friendIdx == -1){
                            OS_TPrintf("Failed to register new friend. Friend list is full.\n");
                        }
                        else {
                            if( DWC_CheckFriendKey( &stUserData, DWC_StringToFriendKey(stFriendKey)) ){// �F�B�o�^���̐������`�F�b�N
                                DWC_CreateFriendKeyToken(&stGameCnt.friendList.keyList[friendIdx], DWC_StringToFriendKey(stFriendKey));// �F�B�o�^���ɂ��F�B�o�^
                            }
                        }
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // �v���t�@�C��ID=0�͖���
                    }
                }
                else {
                    // �F�B�폜
                    if (cntParam.value < GAME_MAX_FRIEND_LIST){
                        // �F�B���X�g����F�B���폜
                        DWC_DeleteBuddyFriendData(&stGameCnt.friendList.keyList[cntParam.value]);
                        validFlg = 1;
                    }
                    else {
                        validFlg = 0;  // �F�B���X�g�͈͊O
                    }
                }

                if (validFlg){
                    // �L���ȓ��͂ł���΃R�}���h�I���ɖ߂�
                    cntParam.step = 0;
                    // ���j���[���X�g�ĕ\��
                    DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                }
                ClearScreen();
            }
            else if (stKeyCnt.trg & PAD_BUTTON_B){
                MI_CpuFill8( stFriendKey, '0', sizeof(stFriendKey));
                // B�{�^���ŃR�}���h�I���ɖ߂�
                cntParam.step = 0;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
                ClearScreen();
            }
            else if (stKeyCnt.trg & PAD_KEY_UP){
                // �\���L�[�㉺�őI�𒆂̐��l�̑���
                stFriendKey[cntParam.figure]++;
                if(stFriendKey[cntParam.figure] > 57){
                    stFriendKey[cntParam.figure] = '0';
                }
                //delete�p�f�[�^
                cntParam.value = (u32)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_DOWN){
                // �\���L�[�㉺�őI�𒆂̐��l�̑���
                stFriendKey[cntParam.figure]--;
                if(stFriendKey[cntParam.figure] < 48){
                    stFriendKey[cntParam.figure] = '9';
                }
                //delete�p�f�[�^
                cntParam.value = (u32)((stFriendKey[0]-48)*10+(stFriendKey[1]-48));
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_LEFT){
                // �\���L�[���E�őI�������̈ړ�
                if (cntParam.figure > 0 && cntParam.figure <= maxFigure-1 ) cntParam.figure--;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
            else if (stKeyCnt.trg & PAD_KEY_RIGHT){
                // �\���L�[���E�őI�������̈ړ�
                if (cntParam.figure >= 0 && cntParam.figure < maxFigure-1 ) cntParam.figure++;
                // ���j���[���X�g�ĕ\��
                DispMenuMsgWithCursor(gameSeq, curIdx, &cntParam);
            }
        }
        ////////// �L�[���쏈�������܂�

        dbs_DemoUpdate();
        OS_WaitIrq(TRUE, OS_IE_V_BLANK);
            Heap_Debug();
        dbs_DemoLoad();
        // �X�^�b�N���`�F�b�N
        OS_CheckStack(OS_GetCurrentThread());
    }

    return returnSeq;
}

/*---------------------------------------------------------------------------*
  �F�B�o�^�E�������[�h�̒ǉ��\���p�R�[���o�b�N�֐�
 *---------------------------------------------------------------------------*/
void RegFriendModeDispCallback(int curIdx, void* param)
{
    RegFriendParam cntParam;
    int i;

    if (!param){
        // �����param = NULL�ŌĂ΂��
        cntParam.step = 0;
    }
    else {
        cntParam = *(RegFriendParam *)param;
    }

    OS_TPrintf("\n");

    if (cntParam.step == 0){
        // �R�}���h�I�𒆂͗F�B���X�g��\��
        DispFriendList(FALSE);
    }
    else {
        // ���l���͒�
        if (curIdx == 1){
            // �F�B�o�^�B�v���t�@�C��ID���͒�
            OS_TPrintf("Set friend key : ");
            for(i = 0; i < 12; i++){
                OS_TPrintf("%c", stFriendKey[i]);
            }
            OS_TPrintf("\n");

            // �I�𒆂̐����������ׂ̉�����\������
            for (i = 0; i < cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("                 -\n");
        }
        else {
            // �F�B�폜�B�F�B���X�g�C���f�b�N�X���͒�
            DispFriendList(FALSE);  // �F�B���X�g��\��
            
            OS_TPrintf("Delete friend Index : ");
            for(i = 0; i < 2; i++){
                OS_TPrintf("%c", stFriendKey[i]);
            }
            OS_TPrintf("\n");

            for (i = 0; i < cntParam.figure; i++){
                OS_TPrintf(" ");
            }
            OS_TPrintf("                      -\n");
        }
    }
}

