//==============================================================================
//	�^�錾
//==============================================================================
// �R�[���o�b�N�֐��̏���
typedef void (*PTRStateFunc)(void);

#define _TRAINER_CARD_NUM_MAX (4)
#define UGSTATE_MOVE (0)

//==============================================================================
// ���[�N
//==============================================================================

typedef struct{
    TR_CARD_DATA* pTRCard[_TRAINER_CARD_NUM_MAX];  // �g���[�i�[�J�[�h����M����o�b�t�@�̃|�C���^
    u8 bTRCard[_TRAINER_CARD_NUM_MAX];
    COMM_MESSAGE* pCMess;
    FIELDSYS_WORK* pFSys;
    MATHRandContext32 sRand; ///< �e�q�@�l�S�V�G�[�V�����p�����L�[
    PTRStateFunc state;
    TCB_PTR pTcb;
    u16 timer;
    u8 connectIndex;   // �q�@���ڑ�����e�@��index�ԍ�
    u8 bStalth;
    u8 bStateNoChange;
    u8 bUGOverlay;
    u8 bReturnBattle; // �o�g������߂��Ă����ꍇ�͂P �����Ă����ꍇ�͂O
    u8 bBattleMoveRoom; // �J�n�ʒu�ɂ������Ƃ���M

    POKEPARTY*  party;	// �~�b�N�X�o�g���p�p�[�e�B

#ifdef PM_DEBUG		// Debug ROM
    u16 debugTimer;
#endif
} _COMM_FSTATE_WORK;


extern _COMM_FSTATE_WORK* _getCommFieldStateWork(void);
extern void _commStateInitialize(FIELDSYS_WORK* pFSys);
extern void _stateFinalize(void);

