//============================================================================================
/**
 * @file	regulation_data.c
 * @brief	�o�g�����M�����[�V�����f�[�^�A�N�Z�X�p�\�[�X
 * @author	k.ohno
 * @date	2006.5.24
 */
//============================================================================================

#include "savedata/savedata_def.h"	//SAVEDATA�Q�Ƃ̂���

#include "common.h"

#include "system/gamedata.h"
#include "savedata/savedata.h"


#include "savedata/regulation.h"
#include "regulation_local.h"

#include "system/pm_str.h"

#include "gflib/strbuf_family.h"

#include "msgdata/msg.naix"
#include "system/wordset.h"
#include "msgdata/msg_directbattlecorner.h"

#include "savedata/regulation_data.h"

//============================================================================================
//============================================================================================

REGULATION _cup[] = {
    {
        {EOM_},  //  // �X�^���_�[�h�J�b�v ���O��gmm�������Ă���
        0,     // ���v�Ȃ�
        3,      // 3�C
        50,     // LV50
        0,      // �g�������Ȃ�
        0,      // �̏d�����Ȃ�
        1,      // �i���|�P����OK
        0,      // ����s��
        0,      // �����|�P�����s��
        0,      // ��������s��
        0,// �Œ�_���[�W�Z���K�����s
    },
    {
        {EOM_},//�� �t�@���V�[�J�b�v
        80,//���x���̍��v�W�O�܂�
        3,//�R�C
        30,//�|�P�����̃��x���R�O�܂�
        -20,//�g�������Q�D�O������
        -20,// �̏d�����Q�O��������
        0,// �i���|�P�����Q���ł��Ȃ�
        0,// ���ʂȃ|�P�����Q���ł��Ȃ�
        0,// �����|�P�����Q���ł��Ȃ�
        0,// ��������������Ȃ�
        0,// �Œ�_���[�W�Z���K�����s
    },
    {
        {EOM_},//�� ���g���J�b�v
        0,//���x���̍��v�����Ȃ�
        3,//�R�C
        5,//�|�P�����̃��x���T�܂�
        0,// �g�������Ȃ�
        0,// �̏d�����Ȃ�
        0,// �i���|�P�����Q���ł��Ȃ�
        0,// ���ʂȃ|�P�����Q���ł��Ȃ�
        0,// �����|�P�����Q���ł��Ȃ�
        0,// ��������������Ȃ� 
        1,//���ʃ��[���F��イ�̂�����A�\�j�b�N�u�[���̖��������O�ɂȂ�B
    },
    {
        {EOM_},//�� ���C�g�J�b�v
        0,//���x���̍��v�����Ȃ�
        3,//�R�C
        50,//�|�P�����̃��x���T�O�܂�
        0,// �g�������Ȃ�
        -99,// �̏d�X�X��������
        0,// �i���|�P�����Q���ł��Ȃ�
        0,// ���ʂȃ|�P�����Q���ł��Ȃ�
        0,// �����|�P�����Q���ł��Ȃ�
        0,// ��������������Ȃ�
        0,// �Œ�_���[�W�Z���K�����s
    },
    {
        {EOM_},//�� �_�u���J�b�v
        0,//�@�E���x���̍��v��������
        4,//�E4�C
        50,//�@�E�|�P�����̃��x���T�O�܂�
        0,//�E �g�������Ȃ�
        0,//�E �̏d�����Ȃ�
        1,//�E �i���|�P�����Q���ł���
        0,//�E ���ʂȃ|�P�����Q���ł��Ȃ�
        0,//�E �����|�P�����Q���ł��Ȃ�
        0,//�E ��������������Ȃ�
        0,// �Œ�_���[�W�Z���K�����s
    },
};

//���M�����[�V������������(GDS�̘^��w�b�_�[�Ɋ܂߂�ׂ̃_�~�[�p)
REGULATION no_limit_cup = {
    {EOM_},  //
    0,     // ���v�Ȃ�
    6,      // 6�C
    100,     // LV100
    0,      // �g�������Ȃ�
    0,      // �̏d�����Ȃ�
    1,      // �i���|�P����OK
    1,      // �����
    1,      // �����|�P������
    1,      // ���������
    0,		// �Œ�_���[�W�Z�e���Ȃ�
};


//----------------------------------------------------------
/**
 * @brief	���M�����[�V�����f�[�^�ւ̃|�C���^�擾
 * @param	�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 * @param	���{�ڂ̃��M�����[�V�����f�[�^��
 * @return	REGULATION	���[�N�ւ̃|�C���^
 */
//----------------------------------------------------------
const REGULATION* Data_GetRegulation(SAVEDATA* pSave, int regNo)
{
	REGULATION_DATA* pRegData = NULL;

    if(regNo < NELEMS(_cup)){
        return &_cup[regNo];
    }
    pRegData = SaveData_Get(pSave, GMDATA_ID_REGULATION);
    return &pRegData->regulation_buff[0];  // ����Z�[�u�f�[�^�ɂ�1�{�����Ȃ�
}

//----------------------------------------------------------
/**
 * @brief	���M�����[�V�����f�[�^�ւ̃|�C���^�擾
 * @param	�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 * @param	���{�ڂ̃��M�����[�V�����f�[�^��
 * @param	REGULATION	���[�N�ւ̃|�C���^
 */
//----------------------------------------------------------
void Data_GetRegulationName(SAVEDATA* pSave, int regNo, STRBUF* pStrBuff, int HeapID)
{
    MSGDATA_MANAGER* msgman;
	REGULATION_DATA* pRegData = NULL;

    if(regNo < NELEMS(_cup)){
		msgman = MSGMAN_Create(MSGMAN_TYPE_NORMAL, ARC_MSG, NARC_msg_directbattlecorner_dat, HeapID);
        MSGMAN_GetString(msgman, msg_dbc_rule01+regNo, pStrBuff);
        MSGMAN_Delete(msgman);
    }
    else{
        Regulation_GetCupName(SaveData_GetRegulation(pSave,0), pStrBuff);
    }
}

//----------------------------------------------------------
/**
 * @brief	���������̃��M�����[�V�����f�[�^�ւ̃|�C���^�擾
 * @param	�Z�[�u�f�[�^�ێ����[�N�ւ̃|�C���^
 * @param	���{�ڂ̃��M�����[�V�����f�[�^��
 * @return	REGULATION	���[�N�ւ̃|�C���^
 */
//----------------------------------------------------------
const REGULATION* Data_GetNoLimitRegulation(void)
{
	return &no_limit_cup;
}

//--------------------------------------------------------------
/**
 * @brief   ���M�����[�V�����f�[�^���烌�M�����[�V����NO���擾����
 *
 * @param   reg		���M�����[�V�����f�[�^�ւ̃|�C���^
 *
 * @retval  ���M�����[�V����No(REGULATION_NO_???)
 */
//--------------------------------------------------------------
int Data_GetRegulationNo(const REGULATION *reg)
{
	int reg_no;
	
	if(reg == NULL){
		return REGULATION_NO_NULL;
	}
	for(reg_no = 0; reg_no < NELEMS(_cup); reg_no++){
		if(memcmp(reg, &_cup[reg_no], sizeof(REGULATION)) == 0){
			return reg_no;
		}
	}
	return REGULATION_NO_ETC;	//�J�X�^��
}
