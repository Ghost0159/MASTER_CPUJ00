/**
 *	@file	mail.c
 *	@brief	���[���Z�[�u�f�[�^�@����
 *	@author	Miyuki Iwasawa
 *	@date	06.02.09
 *
 *	���[���Z�[�u�f�[�^���f�菇
 *	1,MAIL_SearchNullID()�ŋ󂫗̈�������A�f�[�^ID�擾
 *	2,MailData_CreateWork()�Ń_�~�[�f�[�^�쐬�p���[�N�G���A���擾
 *	3,MailData_CreateDataFromSave()�Ȃǂ��g���ă��[�N�Ƀf�[�^���\�z
 *	4,MAIL_AddDataFromWork()�Ǝ擾�ς݂̃f�[�^ID,���[�N�̃f�[�^���g���āA
 *	�@�Z�[�u�̈�Ƀf�[�^�𔽉f������
 *	5,�_�~�[���[�N�G���A���J������
 */

#include "common.h"
#include "savedata/savedata.h"
#include "savedata/mail.h"
#include "savedata/mail_local.h"
#include "savedata/mystatus.h"
#include "poketool/poke_tool.h"
#include "poketool/pokeparty.h"
#include "poketool/pokeicon.h"
#include "system/pms_data.h"
#include "system/buflen.h"
#include "system/pm_str.h"
#include "poketool/icongra/poke_icon.naix"
#include "poketool/monsno.h"


///�|�P�����A�C�R���̍ő�CGXID(����ŃA�C�R����������Ȃ炱�����ς���K�v������)
#define ICON_CGXID_MAX		(NARC_poke_icon_poke_icon_519_05_NCGR)

///�v���`�i�ȍ~�Œǉ����ꂽ�|�P������cgx_id�ϊ��e�[�u��
///(����ŃA�C�R����������Ȃ炱�̃e�[�u�������₷�K�v������)
static const struct{
	u16 normal_cgx_id;		///<�t�H����0�̎���cgxID
	u16 form_cgx_id;		///<�t�H�������ς���Ă��鎞��cgxID
	u16 monsno;				///<�|�P�����ԍ�
	u8 form_no;				///<form_cgx_id�̓t�H����No������cgxID�Ȃ̂�
	u8 padding;			//�_�~�[
} MailIcon_CgxID_ConvTbl[] = {
	{
		NARC_poke_icon_poke_icon_509_NCGR,
		NARC_poke_icon_poke_icon_509_01_NCGR,
		MONSNO_KIMAIRAN,
		1,
	},
	{
		NARC_poke_icon_poke_icon_516_NCGR,
		NARC_poke_icon_poke_icon_516_01_NCGR,
		MONSNO_EURISU,
		1,
	},
	{
		NARC_poke_icon_poke_icon_519_NCGR,
		NARC_poke_icon_poke_icon_519_01_NCGR,
		MONSNO_PURAZUMA,
		1,
	},
	{
		NARC_poke_icon_poke_icon_519_NCGR,
		NARC_poke_icon_poke_icon_519_02_NCGR,
		MONSNO_PURAZUMA,
		2,
	},
	{
		NARC_poke_icon_poke_icon_519_NCGR,
		NARC_poke_icon_poke_icon_519_03_NCGR,
		MONSNO_PURAZUMA,
		3,
	},
	{
		NARC_poke_icon_poke_icon_519_NCGR,
		NARC_poke_icon_poke_icon_519_04_NCGR,
		MONSNO_PURAZUMA,
		4,
	},
	{
		NARC_poke_icon_poke_icon_519_NCGR,
		NARC_poke_icon_poke_icon_519_05_NCGR,
		MONSNO_PURAZUMA,
		5,
	},
};


/**
 *	@brief	���[���f�[�^�T�C�Y�擾
 *
 *	�����[���f�[�^��ʂ̃T�C�Y
 */
int MailData_GetDataWorkSize(void)
{
	return sizeof(MAIL_DATA);
}
/**
 *	@brief	���[���f�[�^�N���A(�����f�[�^�Z�b�g)
 */
void MailData_Clear(MAIL_DATA* dat)
{
	int i;
	
	dat->writerID = 0;
	dat->sex = PM_MALE;
	dat->region = CasetteLanguage;
	dat->version = CasetteVersion;
	dat->design = MAIL_DESIGN_NULL;

	PM_strclearEOM_(dat->name,BUFLEN_PERSON_NAME);

	for(i = 0;i < MAILDAT_ICONMAX;i++){
		dat->icon[i].dat = MAIL_ICON_NULL;
	}
	dat->form_bit = 0;
	for(i = 0;i < MAILDAT_MSGMAX;i++){
		PMSDAT_Clear(&dat->msg[i]);
	}
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^���L�����ǂ����Ԃ�
 *	@retval	FALSE	����
 *	@retval	TRUE	�L��
 */
BOOL MailData_IsEnable(MAIL_DATA* dat)
{
	if(	dat->design >= MAIL_DESIGN_START &&
		dat->design <= MAIL_DESIGN_END){
		return TRUE;
	}
	return FALSE;
}

/**
 *	@brief	���[���f�[�^�̃��[�N���擾���ĕԂ�
 *
 *	���Ăяo�������ӔC�����ĉ�����邱��
 *	
 */
MAIL_DATA* MailData_CreateWork(int heapID)
{
	MAIL_DATA* p;

	p = sys_AllocMemoryLo(heapID,sizeof(MAIL_DATA));
	MailData_Clear(p);

	return p;
}

/**
 *	@brief	���[���f�[�^�̍\���̃R�s�[
 */
void MailData_Copy(MAIL_DATA* src,MAIL_DATA* dest)
{
	MI_CpuCopy8(src,dest,sizeof(MAIL_DATA));
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	�f�U�C��No,�|�P�����̃|�W�V�����A�Z�[�u�f�[�^�w�肵�ă��[����V�K�쐬��Ԃɏ�����
 *	@param	dat	�f�[�^���쐬����MAIL_DATA�\���̌^�ւ̃|�C���^
 *	@param	design_no	���[���̃C���[�WNo
 *	@param	pos		���[������������|�P�����̎莝�����̃|�W�V����
 *	@param	save	�Z�[�u�f�[�^�ւ̃|�C���^
 */
void MailData_CreateFromSaveData(MAIL_DATA* dat,u8 design_no,u8 pos,SAVEDATA* save)
{
	u8	i,ct,pal,s;
	u16	monsno;
	u32 icon,egg,form;
	MYSTATUS	*my;
	POKEPARTY	*party;
	POKEMON_PARAM* pp;
	
	MailData_Clear(dat);
	dat->design = design_no;

	//�Z�[�u�f�[�^����
	party = SaveData_GetTemotiPokemon(save);
	my = SaveData_GetMyStatus(save);

	//���@�̖��O
	PM_strcpy(dat->name,MyStatus_GetMyName(my));
	//����
	dat->sex = (u8)MyStatus_GetMySex(my);
	//�g���[�i�[ID
	dat->writerID = MyStatus_GetID(my);

	//�|�P�����A�C�R���擾
	dat->form_bit = 0;
	for(i=pos,ct = 0;i < PokeParty_GetPokeCount(party);i++){
		pp = PokeParty_GetMemberPointer(party,i);
		monsno = PokeParaGet(pp,ID_PARA_monsno,NULL);
		egg = PokeParaGet(pp,ID_PARA_tamago_flag,NULL);
		form = PokeParaGet(pp,ID_PARA_form_no,NULL);
		icon = PokeIconCgxArcIndexGetByPP(pp);
//		pal = PokeIconPaletteNumberGet(monsno,egg);
		pal = PokeIconPalNumGet(monsno,form,egg);
		
		dat->icon[ct].cgxID = (u16)icon;
		dat->icon[ct].palID = pal;
		//�v���`�i�ȍ~�Œǉ����ꂽ�A�C�R���̏ꍇ�̃m�[�}���t�H�����ϊ�(�t�H�����ԍ��͕ʗ̈�֑ޔ�)
		for(s = 0; s < NELEMS(MailIcon_CgxID_ConvTbl); s++){
			if(MailIcon_CgxID_ConvTbl[s].form_cgx_id == dat->icon[ct].cgxID && 
					MailIcon_CgxID_ConvTbl[s].form_no == form){
				dat->icon[ct].cgxID = MailIcon_CgxID_ConvTbl[s].normal_cgx_id;
				dat->icon[ct].palID = PokeIconPalNumGet( monsno, 0, egg );	//�t�H����0�̃p���b�g
				dat->form_bit |= MailIcon_CgxID_ConvTbl[s].form_no << (ct*5);
				break;
			}
		}

		ct++;
		if(ct >= MAILDAT_ICONMAX){
			break;
		}
	}
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@�g���[�i�[ID�擾
 */
u32	MailData_GetWriterID(const MAIL_DATA* dat)
{
	return dat->writerID;
}
/**
 *	@brief	���[���f�[�^�@�g���[�i�[ID�K�p
 */
void MailData_SetWriterID(MAIL_DATA* dat,u32 id)
{
	dat->writerID = id;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@���C�^�[���擾
 */
STRCODE* MailData_GetWriterName(MAIL_DATA* dat)
{
	return &(dat->name[0]);
}
/**
 *	@brief	���[���f�[�^�@���C�^�[���K�p
 */
void MailData_SetWriterName(MAIL_DATA* dat,STRCODE* name)
{
	PM_strcpy(dat->name,name);
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@���C�^�[�̐��ʂ��擾
 */
u8	MailData_GetWriterSex(const MAIL_DATA* dat)
{
	return dat->sex;
}
/**
 *	@brief	���[���f�[�^�@���C�^�[�̐��ʂ�K�p
 */
void MailData_SetWriterSex(MAIL_DATA* dat,const u8 sex)
{
	dat->sex = sex;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@�f�U�C��No�擾
 */
u8	MailData_GetDesignNo(const MAIL_DATA* dat)
{
	return dat->design;
}
/**
 *	@brief	���[���f�[�^�@�f�U�C��No�K�p
 */
void MailData_SetDesignNo(MAIL_DATA* dat,const u8 design)
{
	if(design >= MAIL_DESIGN_MAX){
		return;
	}
	dat->design = design;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@���R�[�h�擾
 */
u8	MailData_GetCountryCode(const MAIL_DATA* dat)
{
	return dat->region;
}
/**
 *	@brief	���[���f�[�^�@���R�[�h�K�p
 */
void MailData_SetCountryCode(MAIL_DATA* dat,const u8 code)
{
	dat->region = code;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@�J�Z�b�g�o�[�W�����擾
 */
u8	MailData_GetCasetteVersion(const MAIL_DATA* dat)
{
	return dat->version;
}
/**
 *	@brief	���[���f�[�^�@�J�Z�b�g�o�[�W�����K�p
 */
void MailData_SetCasetteVersion(MAIL_DATA* dat,const u8 version)
{
	dat->version = version;
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@���[���A�C�R���p�����[�^�̎擾(�C���f�b�N�X�w���)
 *
 *	@param	mode	MAIL_ICONPRM_CGX:cgxNo�̎擾
 *					MAIL_ICONPRM_PAL:pltNo�̎擾
 *					MAIL_ICONPRM_ALL:u16�^(MAIL_ICON�^�փL���X�g��)�őo���̒l��Ԃ�
 *
 *	���A�C�R��CgxID�ƃ����X�^�[No�͓���ł͂���܂���B���ӁI
 */
u16	MailData_GetIconParamByIndex(const MAIL_DATA* dat,u8 index,u8 mode, u16 form_bit)
{
	MAIL_ICON mi;
	int s;
	
	if(index < MAILDAT_ICONMAX){
		mi = dat->icon[index];
		//�v���`�i�ȍ~�Œǉ����ꂽ�A�C�R���̃t�H����Index�֕ϊ�
		for(s = 0; s < NELEMS(MailIcon_CgxID_ConvTbl); s++){
			if(MailIcon_CgxID_ConvTbl[s].normal_cgx_id == mi.cgxID && 
					MailIcon_CgxID_ConvTbl[s].form_no == ((form_bit >> (index*5)) & 0x1f)){
				mi.cgxID = MailIcon_CgxID_ConvTbl[s].form_cgx_id;
				mi.palID = PokeIconPalNumGet( 
					MailIcon_CgxID_ConvTbl[s].monsno, MailIcon_CgxID_ConvTbl[s].form_no, 0 );
				break;
			}
		}
		if(mi.cgxID > ICON_CGXID_MAX){
			mi.cgxID = NARC_poke_icon_poke_icon_000_NCGR;
			mi.palID = 0;
		}
		switch(mode){
		case MAIL_ICONPRM_CGX:
			return mi.cgxID;
		case MAIL_ICONPRM_PAL:
			return mi.palID;
		case MAIL_ICONPRM_ALL:
		default:
			return mi.dat;
		}
	}else{
		return 0;
	}
}

/**
 *	@brief	���[���f�[�^�@form_bit�擾
 */
u16	MailData_GetFormBit(const MAIL_DATA* dat)
{
	return dat->form_bit;
}

/**
 *	@brief	���[���f�[�^�@�ȈՕ��擾(�C���f�b�N�X�w���)
 */
PMS_DATA*	MailData_GetMsgByIndex(MAIL_DATA* dat,u8 index)
{
	if(index < MAILDAT_MSGMAX){
		return &(dat->msg[index]);
	}else{
		return &(dat->msg[0]);
	}
}
/**
 *	@brief	���[���f�[�^�@�ȈՕ��Z�b�g(�C���f�b�N�X�w���)
 */
void MailData_SetMsgByIndex(MAIL_DATA* dat,PMS_DATA* pms,u8 index)
{
	if(index >= MAILDAT_MSGMAX){
		return;
	}
	PMSDAT_Copy(&dat->msg[index],pms);
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_SetCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
}

/**
 *	@brief	���[���f�[�^�@�ȈՕ�������擾(�C���f�b�N�X�w��)
 *
 *	@param	dat	MAIL_DATA*
 *	@param	index	�ȈՕ��C���f�b�N�X
 *	@param	buf		�擾����������|�C���^�̊i�[�ꏊ
 *
 *	@retval	FALSE	������̎擾�Ɏ��s(�܂��͊ȈՕ����L���ȃf�[�^�ł͂Ȃ�)
 *	
 *	@li	buf�ɑ΂��ē����Ń��������m�ۂ��Ă���̂ŁA�Ăяo�����������I�ɉ�����邱��
 *	@li	FALSE���Ԃ����ꍇ�Abuf��NULL�N���A�����
 */
BOOL MailData_GetMsgStrByIndex(const MAIL_DATA* dat,u8 index,STRBUF* buf,int heapID)
{
	if(index >= MAILDAT_MSGMAX){
		buf = NULL;
		return FALSE;
	}
	
	if(!PMSDAT_IsEnabled(&dat->msg[index])){
		buf = NULL;
		return FALSE;
	}

	buf = PMSDAT_ToString(&dat->msg[index],heapID);
	return TRUE;
}

//=================================================================
//
//=================================================================
//���[�J���֐��v���g�^�C�v
static int mail_GetNullData(MAIL_DATA* array,int num);
static int mail_GetNumEnable(MAIL_DATA* array,int num);
static MAIL_DATA* mail_GetAddress(MAIL_BLOCK* bloc,MAILBLOCK_ID blockID,int dataID);

/**
 *	@brief	�Z�[�u�f�[�^�u���b�N�ւ̃|�C���^���擾
 */
MAIL_BLOCK* SaveData_GetMailBlock(SAVEDATA* sv)
{
#if (CRC_LOADCHECK && CRCLOADCHECK_GMDATA_ID_MAILDATA)
	SVLD_CheckCrc(GMDATA_ID_MAILDATA);
#endif //CRC_LOADCHECK
	return SaveData_Get(sv,GMDATA_ID_MAILDATA);
}

/**
 *	@brief	���[���Z�[�u�f�[�^�u���b�N�T�C�Y�擾
 *
 *	�����[���f�[�^��ʂ̃T�C�Y�ł͂Ȃ��̂Œ��ӁI
 */
int MAIL_GetBlockWorkSize(void)
{
	return sizeof(MAIL_DATA)*MAIL_STOCK_MAX;
}
/**
 *	@brief	���[���Z�[�u�f�[�^�u���b�N������
 */
void MAIL_Init(MAIL_BLOCK* dat)
{
	int i = 0;

#if 0
	for(i = 0;i < MAIL_STOCK_TEMOTI;i++){
		MailData_Clear(&dat->temoti[i]);
	}
	for(i = 0;i < MAIL_STOCK_EXTRADE;i++){
		MailData_Clear(&dat->extrade[i]);
	}
	for(i = 0;i < MAIL_STOCK_SODATEYA;i++){
		MailData_Clear(&dat->sodateya[i]);
	}
#endif
	for(i = 0;i < MAIL_STOCK_PASOCOM;i++){
		MailData_Clear(&dat->paso[i]);
	}
}

/**
 *	@brief	�󂢂Ă��郁�[���f�[�^ID���擾
 *
 *	@param	id �ǉ����������[���u���b�NID
 *
 *	@return	int	�f�[�^��ǉ��ł���ꍇ�͎Q��ID
 *				�ǉ��ł��Ȃ��ꍇ�̓}�C�i�X�l���Ԃ�
 */
int MAIL_SearchNullID(MAIL_BLOCK* block,MAILBLOCK_ID id)
{
	switch(id){
#if 0
	case MAILBLOCK_TEMOTI:
		return mail_GetNullData(block->temoti,MAIL_STOCK_TEMOTI);
	case MAILBLOCK_EXTRADE:
		return mail_GetNullData(block->extrade,MAIL_STOCK_EXTRADE);
	case MAILBLOCK_SODATEYA:
		return mail_GetNullData(block->sodateya,MAIL_STOCK_SODATEYA);
#endif
	case MAILBLOCK_PASOCOM:
		return mail_GetNullData(block->paso,MAIL_STOCK_PASOCOM);
	default:
		return MAILDATA_NULLID;
	}
	return MAILDATA_NULLID;
}

/**
 *	@brief	���[���f�[�^���폜
 *
 *	@param	blockID	�u���b�N��ID
 *	@param	dataID	�f�[�^ID
 */
void MAIL_DelMailData(MAIL_BLOCK* block,MAILBLOCK_ID blockID,int dataID)
{
	MAIL_DATA* pd = NULL;
	
	pd = mail_GetAddress(block,blockID,dataID);
	if(pd != NULL){
		MailData_Clear(pd);
	}
}

/**
 *	@brief	���[���f�[�^���Z�[�u�u���b�N�ɒǉ�
 *
 *	�������n����MAIL_DATA�\���̌^�f�[�^�̒��g���Z�[�u�f�[�^�ɔ��f�����̂�
 *	�@�������ȃf�[�^�����Ȃ��悤�ɒ��ӁI
 */
void MAIL_AddMailFormWork(MAIL_BLOCK* block,MAILBLOCK_ID blockID,int dataID,MAIL_DATA* src)
{
	MAIL_DATA* pd = NULL;
	
	pd = mail_GetAddress(block,blockID,dataID);
	if(pd != NULL){
		MailData_Copy(src,pd);
	}
}

/**
 *	@brief	�w��u���b�N�ɗL���f�[�^���������邩�Ԃ�
 */
int MAIL_GetEnableDataNum(MAIL_BLOCK* block,MAILBLOCK_ID blockID)
{
	switch(blockID){
#if 0
	case MAILBLOCK_TEMOTI:
		return mail_GetNumEnable(block->temoti,MAIL_STOCK_TEMOTI);
	case MAILBLOCK_EXTRADE:
		return mail_GetNumEnable(block->extrade,MAIL_STOCK_EXTRADE);
	case MAILBLOCK_SODATEYA:
		return mail_GetNumEnable(block->sodateya,MAIL_STOCK_SODATEYA);
#endif
	case MAILBLOCK_PASOCOM:
		return mail_GetNumEnable(block->paso,MAIL_STOCK_PASOCOM);
	default:
		return 0;
	}
	return 0;

}

/**
 *	@brief	���[���f�[�^�̃R�s�[���擾
 *
 *	�������Ń��������m�ۂ���̂ŁA�Ăяo�������ӔC�����ė̈���J�����邱��
 *	������ID���w�肵���ꍇ�A��f�[�^��Ԃ�
 */
MAIL_DATA* MAIL_AllocMailData(MAIL_BLOCK* block,MAILBLOCK_ID blockID,int dataID,int heapID)
{
	MAIL_DATA* src = NULL;
	MAIL_DATA* dest;
	
	src = mail_GetAddress(block,blockID,dataID);
	dest = MailData_CreateWork(heapID);
	if(src != NULL){
		MailData_Copy(src,dest);
	}
	return dest;
}

/**
 *	@brief	���[���f�[�^�̃R�s�[���擾
 *
 *	�����炩���ߊm�ۂ���MAIL_DATA�^�������ɃZ�[�u�f�[�^���R�s�[���Ď擾
 */
void MAIL_GetMailData(MAIL_BLOCK* block,MAILBLOCK_ID blockID,int dataID,MAIL_DATA* dest)
{
	MAIL_DATA* src = NULL;
	
	src = mail_GetAddress(block,blockID,dataID);
	if(src == NULL){
		MailData_Clear(dest);
	}else{
		MailData_Copy(src,dest);
	}
}

//=================================================================
//
//=================================================================

/**
 *	@brief	�莝���u���b�N�̋󂫂�T���ĕԂ�
 *
 *	@param	array	���[���f�[�^�z��ւ̃|�C���^
 *	@param	num		�������œn�����z��̗v�f��
 */
static int mail_GetNullData(MAIL_DATA* array,int num)
{
	int i = 0;

	for(i = 0;i < num;i++){
		if(!MailData_IsEnable(&array[i])){
			return i;
		}
	}
	return MAILDATA_NULLID;
}

/**
 *	@brief	�L���f�[�^�̐���T���ĕԂ�
 */
static int mail_GetNumEnable(MAIL_DATA* array,int num)
{
	int i = 0;
	int ct = 0;
	
	for(i = 0;i < num;i++){
		if(MailData_IsEnable(&array[i])){
			ct++;
		}
	}
	return ct;
}

/**
 *	@brief	�w��ID�����u���b�N���̃��[���f�[�^�ւ̃|�C���^��Ԃ�
 */
static MAIL_DATA* mail_GetAddress(MAIL_BLOCK* block,MAILBLOCK_ID blockID,int dataID)
{
	MAIL_DATA* pd = NULL;
	
	switch(blockID){
#if 0
	case MAILBLOCK_TEMOTI:
		if(dataID < MAIL_STOCK_TEMOTI){
			pd = &(block->temoti[dataID]);
		}
		break;
	case MAILBLOCK_EXTRADE:
		if(dataID < MAIL_STOCK_EXTRADE){
			pd = &(block->extrade[dataID]);
		}
		break;
	case MAILBLOCK_SODATEYA:
		if(dataID >= MAIL_STOCK_SODATEYA){
			pd = &(block->sodateya[dataID]);
		}
		break;
#endif
	case MAILBLOCK_PASOCOM:
		if(dataID < MAIL_STOCK_PASOCOM){
			pd = &(block->paso[dataID]);
		}
		break;
	default:
		break;
	}
	return pd;
}
