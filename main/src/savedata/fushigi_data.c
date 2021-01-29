//============================================================================================
/**
 * @file	fushigi_data.c
 * @date	2006.04.28
 * @author	tamada / mitsuhara
 * @brief	�ӂ����ʐM�p�Z�[�u�f�[�^�֘A
 */
//============================================================================================

#include "common.h"
#include "savedata/savedata.h"
#include "savedata/fushigi_data.h"

#include "gflib/assert.h"

//============================================================================================
//============================================================================================

extern FUSHIGI_DATA * SaveData_GetFushigiData(SAVEDATA * sv);

#define UNIQ_TYPE			0xEDB88320L
#define FUSHIGI_DATA_NO_USED		0x00000000
#define FUSHIGI_DATA_MAX_EVENT		2048

#define FUSHIGI_MENU_FLAG		(FUSHIGI_DATA_MAX_EVENT - 1)

//------------------------------------------------------------------
/**
 * @brief	�ӂ����f�[�^�̒�`
 */
//------------------------------------------------------------------
struct FUSHIGI_DATA{
  u8 recv_flag[FUSHIGI_DATA_MAX_EVENT / 8];		//256 * 8 = 2048 bit
  GIFT_DELIVERY delivery[GIFT_DELIVERY_MAX];		// �z�B���W��
  GIFT_CARD card[GIFT_CARD_MAX];			// �J�[�h���R��
};


//============================================================================================
//
//		��ɃZ�[�u�V�X�e������Ă΂��֐�
//
//============================================================================================
//------------------------------------------------------------------
/**
 * @brief	�Z�[�u�f�[�^�T�C�Y�̎擾
 * @return	int		�ӂ����Z�[�u�f�[�^�̃T�C�Y
 */
//------------------------------------------------------------------
int FUSHIGIDATA_GetWorkSize(void)
{
  return sizeof(FUSHIGI_DATA);
}
//------------------------------------------------------------------
/**
 * @brief	�Z�[�u�f�[�^������
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 */
//------------------------------------------------------------------
void FUSHIGIDATA_Init(FUSHIGI_DATA * fd)
{
#ifdef DEBUG_ONLY_FOR_mituhara
  OS_TPrintf("�ӂ����f�[�^������\n");
  // ���̏�����saveload_system.c��SVDT_Init�ōs���Ă���̂ŉ������Ȃ�
  MI_CpuClearFast(fd, sizeof(FUSHIGI_DATA));
#endif
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
}

//============================================================================================
//
//		���p���邽�߂ɌĂ΂��A�N�Z�X�֐�
//
//============================================================================================


//------------------------------------------------------------------
/**
 * @brief	�f�[�^���L�����ǂ���
 * @param	gift_type
 * @return	BOOL =TRUE �L��
 */
//------------------------------------------------------------------

static BOOL FUSHIGIDATA_IsIn(u16 gift_type)
{
    if((gift_type > MYSTERYGIFT_TYPE_NONE) &&
       (gift_type < MYSTERYGIFT_TYPE_MAX)){
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------
/**
 * @brief	�z�B���f�[�^�̎擾
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	index		�z�B���f�[�^�̃C���f�b�N�X�i�O�I���W���j
 * @return	GIFT_DATA	�z�B���f�[�^�ւ̃|�C���^
 */
//------------------------------------------------------------------
GIFT_DELIVERY * FUSHIGIDATA_GetDeliData(FUSHIGI_DATA * fd, int index)
{
    if((index >= 0) && (index < GIFT_DELIVERY_MAX)){
        if(FUSHIGIDATA_IsIn(fd->delivery[index].gift_type)){
            // �f�[�^���L���Ȃ̂Ń|�C���^��Ԃ�
            return &fd->delivery[index];
        }
    }
    return (GIFT_DELIVERY *)NULL;
}

//------------------------------------------------------------------
/**
 * @brief	�J�[�h�f�[�^�̎擾
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	index		���蕨�f�[�^�̃C���f�b�N�X�i�O�I���W���j
 * @return	GIFT_CARD	�J�[�h�f�[�^�ւ̃|�C���^
 */
//------------------------------------------------------------------
GIFT_CARD *FUSHIGIDATA_GetCardData(FUSHIGI_DATA *fd, int index)
{
    if((index >= 0) && (index < GIFT_CARD_MAX)){
        if(FUSHIGIDATA_IsIn(fd->card[index].gift_type)){
            return &fd->card[index];
        }
    }
    return NULL;
}

//------------------------------------------------------------------
/**
 * @brief	�z�B���f�[�^���Z�[�u�f�[�^�֓o�^
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	p		�f�[�^�ւ̃|�C���^
 * @return	TRUE: �Z�[�u�ł��� : FALSE: �󂫃X���b�g����������
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_SetDeliData(FUSHIGI_DATA *fd, const void *p, int link)
{
  int i;
    BOOL bRet=FALSE;

  // �Z�[�u�ł���̈悪������΃Z�[�u���s
  if(FUSHIGIDATA_CheckDeliDataSpace(fd) == FALSE)	return FALSE;

  // ������ȍ~�͗e�ʓI�ɂ̓Z�[�u�ɐ�������͂�
  
  for(i = 0; i < GIFT_DELIVERY_MAX; i++){
    if(!FUSHIGIDATA_IsIn(fd->delivery[i].gift_type)){
      MI_CpuCopy8(p, &fd->delivery[i], sizeof(GIFT_DELIVERY));
      fd->delivery[i].link = link;
      bRet = TRUE;
        break;
    }
  }
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
  return bRet;
}

//------------------------------------------------------------------
/**
 * @brief	�J�[�h�f�[�^���Z�[�u�f�[�^�֓o�^
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	p		�f�[�^�ւ̃|�C���^
 * @return	TRUE: �Z�[�u�ł��� : FALSE: �󂫃X���b�g����������
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_SetCardData(FUSHIGI_DATA *fd, const void *p)
{
    int i;
    GIFT_CARD *gc = (GIFT_CARD *)p;
    BOOL bRet = FALSE;

    // �Z�[�u�ł���̈悪������΃Z�[�u���s
    if(FUSHIGIDATA_CheckCardDataSpace(fd) == FALSE)	return FALSE;
    // �z�B�����܂ރf�[�^�̏ꍇ�͔z�B�������`�F�b�N
    if(gc->beacon.delivery_flag == TRUE &&
       FUSHIGIDATA_CheckDeliDataSpace(fd) == FALSE)	return FALSE;

  // ������ȍ~�͗e�ʓI�ɂ̓Z�[�u�ɐ�������͂�
  
  // �J�[�h���Z�[�u����
    for(i = 0; i < GIFT_CARD_MAX; i++){
        if(!FUSHIGIDATA_IsIn(fd->card[i].gift_type)){
#ifdef DEBUG_ONLY_FOR_mituhara
            OS_TPrintf("�J�[�h���Z�[�u���܂��� [%d]\n", i);
#endif
            MI_CpuCopy8(gc, &fd->card[i], sizeof(GIFT_CARD));

      // �z�B�����Z�[�u����
            if(gc->beacon.delivery_flag == TRUE){
                FUSHIGIDATA_SetDeliData(fd, (const void *)&gc->gift_type, i);
            }
            bRet = TRUE;
            break;
        }
    }
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
    return bRet;
}

//------------------------------------------------------------------
/**
 * @brief	�z�B���f�[�^�𖕏�����
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	index		�z�B���f�[�^�̃C���f�b�N�X
 * @return	TRUE: �Z�[�u�ł��� : FALSE: �󂫃X���b�g����������
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_RemoveDeliData(FUSHIGI_DATA *fd, int index)
{
    GF_ASSERT(index < GIFT_DELIVERY_MAX);
    fd->delivery[index].gift_type = MYSTERYGIFT_TYPE_NONE;
    fd->delivery[index].link = 0;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
    return TRUE;
}

//------------------------------------------------------------------
/**
 * @brief	�J�[�h�f�[�^�𖕏����� �z�B����  BIT�����Ƃ�
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @return	TRUE: �Z�[�u�ł��� : FALSE: �󂫃X���b�g����������
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_RemoveCardDataPlusBit(FUSHIGI_DATA *fd, int index)
{
    GF_ASSERT(index < GIFT_CARD_MAX);
    fd->card[index].gift_type = MYSTERYGIFT_TYPE_NONE;
    //BIT�����Ƃ�
    FUSHIGIDATA_ResetEventRecvFlag(fd, fd->card[index].beacon.event_id);
    // �����N����Ă���J�[�h���ꏏ�ɖ���
    FUSHIGIDATA_RemoveCardLinkDeli(fd, index);

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
    return TRUE;
}

//------------------------------------------------------------------
/**
 * @brief	�J�[�h�f�[�^�𖕏�����
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @return	TRUE: �Z�[�u�ł��� : FALSE: �󂫃X���b�g����������
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_RemoveCardData(FUSHIGI_DATA *fd, int index)
{
    GF_ASSERT(index < GIFT_CARD_MAX);
    fd->card[index].gift_type = MYSTERYGIFT_TYPE_NONE;

#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
    return TRUE;
}

//------------------------------------------------------------------
/**
 * @brief	�z�B���f�[�^���Z�[�u�ł��邩�`�F�b�N
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @return	TRUE: �󂫂����� : FALSE: �󂫃X���b�g������
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_CheckDeliDataSpace(FUSHIGI_DATA *fd)
{
    int i;
    for(i = 0; i < GIFT_DELIVERY_MAX; i++){
        if(!FUSHIGIDATA_IsIn(fd->delivery[i].gift_type)){
            return TRUE;
        }
    }
    return FALSE;
}

//------------------------------------------------------------------
/**
 * @brief	�J�[�h�f�[�^���Z�[�u�ł��邩�`�F�b�N
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	size		�f�[�^�̃T�C�Y
 * @return	TRUE: �󂫂����� : FALSE: �󂫃X���b�g������
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_CheckCardDataSpace(FUSHIGI_DATA *fd)
{
  int i;
  for(i = 0; i < GIFT_CARD_MAX; i++){
      if(!FUSHIGIDATA_IsIn(fd->card[i].gift_type)){
          return TRUE;
      }
  }
  return FALSE;
}

//------------------------------------------------------------------
/**
 * @brief	���蕨�f�[�^�̑��݃`�F�b�N
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	index		���蕨�f�[�^�̃C���f�b�N�X�i�O�I���W���j
 * @return	BOOL	TRUE�̎��A���݂���
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_IsExistsDelivery(const FUSHIGI_DATA * fd, int index)
{
    GF_ASSERT(index < GIFT_DELIVERY_MAX);
  
    if(FUSHIGIDATA_IsIn(fd->delivery[index].gift_type)){
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------
/**
 * @brief	�J�[�h�f�[�^�����݂��邩�Ԃ�
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @return	BOOL	TRUE�̎��A���݂���
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_IsExistsCard(const FUSHIGI_DATA * fd, int index)
{
    GF_ASSERT(index < GIFT_CARD_MAX);

    if(FUSHIGIDATA_IsIn(fd->card[index].gift_type)){
        return TRUE;
    }
    return FALSE;
}


//------------------------------------------------------------------
/**
 * @brief	�Z�[�u�f�[�^���ɃJ�[�h�f�[�^�����݂��邩�Ԃ�
 * @param	NONE
 * @return	NONE
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_IsExistsCardAll(const FUSHIGI_DATA *fd)
{
  int i;
  for(i = 0; i < GIFT_CARD_MAX; i++)
    if(FUSHIGIDATA_IsExistsCard(fd, i) == TRUE)
      return TRUE;
  return FALSE;
}


//------------------------------------------------------------------
/**
 * @brief	�w��̃J�[�h�Ƀ����N����Ă���z�B�������݂��邩
 * @param	��index��0�`2���L��
 * @return	TRUE: �z�B���͑��݂��� FALSE: ���݂��Ȃ�
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_GetCardLinkDeli(const FUSHIGI_DATA *fd, int index)
{
    int i;

    for(i = 0; i < GIFT_DELIVERY_MAX; i++){
        if(FUSHIGIDATA_IsIn(fd->delivery[i].gift_type)){
            if(fd->delivery[i].link == index){
                return TRUE;
            }
        }
    }
    return FALSE;
}


//------------------------------------------------------------------
/**
 * @brief	�w��J�[�h�Ƀ����N����Ă���z�B�����폜
 * @param	��index��0�`2���L��(������Ή������Ȃ�)
 * @return	NONE
 */
//------------------------------------------------------------------
void FUSHIGIDATA_RemoveCardLinkDeli(const FUSHIGI_DATA *fd, int index)
{
    int i;

    for(i = 0; i < GIFT_DELIVERY_MAX; i++){
        if(FUSHIGIDATA_IsIn(fd->delivery[i].gift_type)){
            if(fd->delivery[i].link == index){
#if (DEBUG_ONLY_FOR_mituhara | DEBUG_ONLY_FOR_ohno)
                OS_TPrintf("�J�[�h�ƈꏏ�� %d �Ԃ̂�������̂������܂���\n", i );
#endif
                FUSHIGIDATA_RemoveDeliData((FUSHIGI_DATA *)fd, i);
                return;
            }
        }
    }
}

//------------------------------------------------------------------
/**
 * @brief	�w��̃C�x���g�͂��łɂ���������Ԃ�
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	num		�C�x���g�ԍ�
 * @return	BOOL	TRUE�̎��A���łɂ�����Ă���
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_IsEventRecvFlag(FUSHIGI_DATA * fd, int num)
{
  GF_ASSERT(num < FUSHIGI_DATA_MAX_EVENT);
  return !!(fd->recv_flag[num / 8] & (1 << (num & 7)));
}


//------------------------------------------------------------------
/**
 * @brief	�w��̃C�x���g���������t���O�𗧂Ă�
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	num		�C�x���g�ԍ�
 * @return	NONE
 */
//------------------------------------------------------------------
void FUSHIGIDATA_SetEventRecvFlag(FUSHIGI_DATA * fd, int num)
{
  GF_ASSERT(num < FUSHIGI_DATA_MAX_EVENT);
  fd->recv_flag[num / 8] |= (1 << (num & 7));
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
}

//------------------------------------------------------------------
/**
 * @brief	�w��̃C�x���g���������t���O�𗎂� �J�[�h�Ɣz�B�����ꏏ�ɏ����������g�p����
 * @param	fd		�ӂ����Z�[�u�f�[�^�ւ̃|�C���^
 * @param	num		�C�x���g�ԍ�
 * @return	NONE
 */
//------------------------------------------------------------------
void FUSHIGIDATA_ResetEventRecvFlag(FUSHIGI_DATA * fd, int num)
{
    u8 notbit = (u8)~(1 << (num & 7));
    GF_ASSERT(num < FUSHIGI_DATA_MAX_EVENT);
    
#if (DEBUG_ONLY_FOR_mituhara | DEBUG_ONLY_FOR_ohno)
    OS_TPrintf("FUSHIGIDATA_ResetEventRecvFlag %x ",  fd->recv_flag[num / 8]);
#endif
  fd->recv_flag[num / 8] &= notbit;
#if (DEBUG_ONLY_FOR_mituhara | DEBUG_ONLY_FOR_ohno)
    OS_TPrintf(" %x \n",  fd->recv_flag[num / 8]);
#endif
    
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
}

//------------------------------------------------------------------
/**
 * @brief	�ӂ����Ȃ�������̂�\���o���邩�H
 * @param	NONE
 * @return	TRUE: �\���@FALSE: �\�����Ȃ�
 */
//------------------------------------------------------------------
BOOL FUSHIGIDATA_IsFushigiMenu(FUSHIGI_DATA *fd)
{
  return FUSHIGIDATA_IsEventRecvFlag(fd, FUSHIGI_MENU_FLAG);
}


//------------------------------------------------------------------
/**
 * @brief	�ӂ����Ȃ�������̂̕\���t���O��ON�ɂ���
 * @param	NONE
 * @return	NONE
 */
//------------------------------------------------------------------
void FUSHIGIDATA_SetFushigiMenu(FUSHIGI_DATA *fd)
{
  FUSHIGIDATA_SetEventRecvFlag(fd, FUSHIGI_MENU_FLAG);
}


//============================================================================================
//
//		����ɏ�ʂ̊֐��Q�@��ɂ�������Ă�ł�������
//
//============================================================================================

static FUSHIGI_DATA *_fushigi_ptr = NULL;

//------------------------------------------------------------------
/**
 * @brief	����ȉ��̊֐����g�����߂ɕK�v�ȏ�����
 * @param	sv		�Z�[�u�f�[�^�\���ւ̃|�C���^
 * @param	heap_id		���[�N���擾����q�[�v��ID
 * @return	NONE
 */
//------------------------------------------------------------------
void FUSHIGIDATA_InitSlot(SAVEDATA * sv, int heap_id)
{
  LOAD_RESULT result;

  if(_fushigi_ptr == NULL){
    _fushigi_ptr = SaveData_GetFushigiData(sv);
  }
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_FUSHIGIDATA)
	SVLD_SetCrc(GMDATA_ID_FUSHIGIDATA);
#endif //CRC_LOADCHECK
}

//------------------------------------------------------------------
/**
 * @brief	����ȉ��̊֐����g���I�������̌�n��
 * @param	sv		�Z�[�u�f�[�^�\���ւ̃|�C���^
 * @param	flag		TRUE: �Z�[�u���� / FALSE: �Z�[�u���Ȃ�
 * @return	NONE
 */
//------------------------------------------------------------------
void FUSHIGIDATA_FinishSlot(SAVEDATA * sv, int flag)
{
  SAVE_RESULT result;
  if(_fushigi_ptr){
#if 0
    if(flag == TRUE)
      SaveData_Save(sv);
#endif
    _fushigi_ptr = NULL;
  }
}
     
//------------------------------------------------------------------
/**
 * @brief	�X���b�g�Ƀf�[�^�����邩�Ԃ��֐�
 * @param	NONE
 * @return	-1..�f�[�^�Ȃ�: 0�ȏ� �f�[�^index�ԍ�
*/
//------------------------------------------------------------------
int FUSHIGIDATA_GetSlotData(void)
{
    int i;
    for(i = 0; i < GIFT_DELIVERY_MAX; i++){
        if(FUSHIGIDATA_IsExistsDelivery(_fushigi_ptr, i) == TRUE)
            return i;
    }
    return -1;
}

//------------------------------------------------------------------
/**
 * @brief	�X���b�g�Ƀf�[�^�����邩�Ԃ��֐�
 * @param	NONE
 * @return	FALSE..�f�[�^�Ȃ�: TRUE �f�[�^����
*/
//------------------------------------------------------------------
BOOL FUSHIGIDATA_CheckSlotData(void)
{
    if(FUSHIGIDATA_GetSlotData() == -1){
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------
/**
 * @brief	�w��ԍ��̃X���b�g�f�[�^�̃^�C�v��Ԃ�
 * @param	index		�X���b�g�̃C���f�b�N�X�ԍ�
 * @return	int		MYSTERYGIFT_TYPE_xxxxx
 *
 * ���C���f�b�N�X�ԍ���FUSHIGIDATA_GetSlotData�ŕԂ��ꂽ�l
*/
//------------------------------------------------------------------
int FUSHIGIDATA_GetSlotType(int index)
{
    GIFT_DELIVERY *dv;

    dv = FUSHIGIDATA_GetDeliData(_fushigi_ptr, index);
    if(dv){
        return (int)dv->gift_type;
    }
    return MYSTERYGIFT_TYPE_NONE;
}


//------------------------------------------------------------------
/**
 * @brief	�w��ԍ��̃X���b�g�\���̂ւ̃|�C���^��Ԃ�
 * @param	index		�X���b�g�̃C���f�b�N�X�ԍ�
 * @return	GIFT_PRESENT	�\���̂ւ̃|�C���^
 *
 * ���C���f�b�N�X�ԍ���FUSHIGIDATA_GetSlotData�ŕԂ��ꂽ�l
 */
//------------------------------------------------------------------
GIFT_PRESENT *FUSHIGIDATA_GetSlotPtr(int index)
{
    GIFT_DELIVERY *dv;

    dv = FUSHIGIDATA_GetDeliData(_fushigi_ptr, index);
    if(dv){
        return &dv->data;
    }
    return NULL;
}

//------------------------------------------------------------------
/**
 * @brief	�w��̃X���b�g����������
 * @param	index		�X���b�g�̃C���f�b�N�X�ԍ�
 * @return	NONE
 *
 * ���C���f�b�N�X�ԍ���FUSHIGIDATA_GetSlotData�ŕԂ��ꂽ�l
 */
//------------------------------------------------------------------
void FUSHIGIDATA_RemoveSlot(int index)
{
  FUSHIGIDATA_RemoveDeliData(_fushigi_ptr, index);
}

//------------------------------------------------------------------


