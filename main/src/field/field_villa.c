//******************************************************************************
/**
 *
 * @file	field_villa.c
 * @brief	�t�B�[���h�@�ʑ�����
 * @author	kagaya
 * @data	05.07.13
 *
 */
//******************************************************************************
#include "common.h"
#include "fieldsys.h"
#include "fieldmap_work.h"
#include "system/snd_tool.h"
#include "include/savedata/gimmickwork.h"
#include "../fielddata/build_model/build_model_id.h"

#include "field_gimmick_def.h"
#include "syswork.h"
#include "field/evwkdef.h"
#include "sysflag.h"

#include "script.h"

#include "fielddata/maptable/zone_id.h"
#include "eventdata.h"

#include "gym_local.h"
#include "field_villa.h"

#include "..\fielddata\script\t07r0201_def.h"
#include "..\fielddata\script\tv_def.h"

//==============================================================================
//	define
//==============================================================================
#define GDFX(a) (GRID_SIZE_FX32(a))					///<�O���b�h�����ϊ�
#define GCFX(a) (GRID_SIZE_FX32(a)+GRID_HALF_FX32)	///<�O���b�h���S�����ϊ�

#define VILLA_GX_MAX (32)
#define VILLA_GZ_MAX (32)
#define VILLA_GRID_SIZE_MAX (VILLA_GX_MAX*VILLA_GZ_MAX)

//--------------------------------------------------------------
///	�Ƌ��ޔԍ�
//--------------------------------------------------------------
enum
{
	FTURE_0,
	FTURE_1,
	FTURE_2,
	FTURE_3,
	FTURE_4,
	FTURE_5,
	FTURE_6,
	FTURE_7,
	FTURE_8,
	FTURE_9,
	FTURE_10,
	FTURE_11,
	FTURE_12,
	FTURE_13,
	FTURE_14,
	FTURE_15,
	FTURE_16,
	FTURE_17,
	FTURE_18,
	FTURE_19,
	FTURE_MAX,
};

#define FTURE_PUT_MAX (23)	///<�Ƌ�z�u�ő吔

//==============================================================================
//	typedef
//==============================================================================
typedef struct _VILLA_WORK VILLA_WORK;

//--------------------------------------------------------------
///	FTURE_RECT
//--------------------------------------------------------------
typedef struct
{
	s16 left;				///<�Ƌ�u�����O���b�h�n�_
	s16 top;
	s16 right;
	s16 bottom;
}FTURE_RECT;

//--------------------------------------------------------------
///	FTURE_DATA
//--------------------------------------------------------------
typedef struct
{
	u16 no;				///<�Ƌ���
	u16 hit_flag;		///<�Ƌ���蔻�� TRUE=Hit
	VecFx32 draw_pos;	///<�Ƌ�\�����W
	FTURE_RECT rect;	///<�Ƌ���蔻��
}FTURE_DATA;

//--------------------------------------------------------------
///	FTURE_TALKDATA
//--------------------------------------------------------------
typedef struct
{
	u32 scr_id;
}FTURE_TALKDATA;

//--------------------------------------------------------------
///	FTURE_OBJ
//--------------------------------------------------------------
typedef struct
{
	u16 use_flag;
	u16	entry_idx;
	u32 bmid;
	M3DO_PTR m3do;
}FTURE_OBJ;

//--------------------------------------------------------------
///	VILLA_WORK
//--------------------------------------------------------------
struct _VILLA_WORK
{
	FIELDSYS_WORK *fsys;
	FLD_VILLA_WORK *gmc_villa;
	FTURE_OBJ obj_tbl[FTURE_PUT_MAX];
};

//==============================================================================
//	proto
//==============================================================================

//���f��
static void Villa_MdlSet( VILLA_WORK *work, const u32 no );

//�p�[�c
static BOOL Villa_EvFlagCheck( FIELDSYS_WORK *fsys, u32 no );
static BOOL Villa_FTureHitCheck(
	int gx, int gz, const FTURE_DATA *fdata, FIELDSYS_WORK *fsys );

//data
static const FTURE_DATA DATA_FTureDataTbl[FTURE_PUT_MAX];
static const u32 DATA_FTureBMIDTbl[FTURE_MAX];
static const FTURE_TALKDATA DATA_FTureTalkEventDataTbl[FTURE_MAX];

//==============================================================================
//	�t�B�[���h�@�M�~�b�N�@�ʑ�����
//==============================================================================
//--------------------------------------------------------------
/**
 * �ʑ��@�Z�b�g�A�b�v
 * @param	fsys	FIELDSYS_WORK
 * @retval	nothing
 */
//--------------------------------------------------------------
void FldVilla_Setup( FIELDSYS_WORK *fsys )
{
	int i;
	GIMMICKWORK *gmc;
	FLD_VILLA_WORK *gmc_villa;
	VILLA_WORK *work;
	
	gmc = SaveData_GetGimmickWork( GameSystem_GetSaveData(fsys) );
	gmc_villa = GIMMICKWORK_Get( gmc, FLD_GIMMICK_VILLA );
	
	work = sys_AllocMemory( HEAPID_FIELD, sizeof(VILLA_WORK) );
	memset( work, 0, sizeof(VILLA_WORK) );
	work->fsys = fsys;
	work->gmc_villa = gmc_villa;
	fsys->fldmap->Work = work;
	
	//�ȉ�������
	{
		int i;
		
		for( i = 0; i < FTURE_MAX; i++ ){
			if( Villa_EvFlagCheck(fsys,i) == TRUE ){
				Villa_MdlSet( work, i );
			}
		}
	}
}

//--------------------------------------------------------------
/**
 * �ʑ��@�I��
 * @param	fsys	FIELDSYS_WORK
 * @retval	nothing
 */
//--------------------------------------------------------------
void FldVilla_End( FIELDSYS_WORK *fsys )
{
	VILLA_WORK *work = fsys->fldmap->Work;
	
	//�I������
	
	sys_FreeMemoryEz( work );
	fsys->fldmap->Work = NULL;
}

//==============================================================================
//	�ʑ��@�����蔻��
//==============================================================================
//---------------------------------------------------------------------------
/**
 * �ʑ��@�����蔻��
 * @param	fsys		�t�B�[���h�V�X�e���|�C���^
 * @param	inGridX		�O���b�hX���W
 * @param	inGridZ		�O���b�hZ���W
 * @param	inHeight	����
 * @param	outHit		���茋��	TRUE:�q�b�g	FALSE:�q�b�g���ĂȂ�
 * @return	BOOL		TRUE:���̂��ƒʏ퓖���蔻����s��Ȃ�	FALSE:�ʏ퓖���蔻����s��
 */
//---------------------------------------------------------------------------
BOOL FldVilla_HitCheck( FIELDSYS_WORK *fsys,
	const int inGridX, const int inGridZ,
	const fx32 inHeight, BOOL *outHit )
{
	int i;
	const FTURE_RECT *rect;
	const FTURE_DATA *fdata = DATA_FTureDataTbl;
	
	for( i = 0; i < FTURE_PUT_MAX; i++, fdata++ ){
		if( Villa_FTureHitCheck(inGridX,inGridZ,fdata,fsys) == TRUE ){
			*outHit = TRUE;
			return( TRUE );
		}
	}
	
	*outHit = FALSE;
	return( FALSE );
}

//---------------------------------------------------------------------------
/**
 * �ʑ��@�b�������`�F�b�N
 * @param	fsys		�t�B�[���h�V�X�e���|�C���^
 * @param	inGridX		�O���b�hX���W
 * @param	inGridZ		�O���b�hZ���W
 * @return	BOOL		���茋�� TRUE=�b�������C�x���g�L��
 */
//---------------------------------------------------------------------------
BOOL FldVilla_TalkCheck( FIELDSYS_WORK *fsys,
	const int inGridX, const int inGridZ, const int dir )
{
	GIMMICKWORK *gmc = SaveData_GetGimmickWork(GameSystem_GetSaveData(fsys) );
	if( GIMMICKWORK_GetAssignID(gmc) == FLD_GIMMICK_VILLA ){
		int i;
		const FTURE_RECT *rect;
		const FTURE_DATA *fdata = DATA_FTureDataTbl;
	
		for( i = 0; i < FTURE_PUT_MAX; i++, fdata++ ){
			if( Villa_FTureHitCheck(inGridX,inGridZ,fdata,fsys) == TRUE ){
				const FTURE_TALKDATA *id =
					&DATA_FTureTalkEventDataTbl[fdata->no];
				
				if( id->scr_id == SCRID_TV ){	//TV����b�����������`�F�b�N
					if( dir != DIR_UP ){
						continue;
					}
				}
				
				EventSet_Script( fsys, id->scr_id, NULL );
				return( TRUE );
			}
		}
	}
	
	return( FALSE );
}

//==============================================================================
//	�ʑ��@�z�u���f��
//==============================================================================
//--------------------------------------------------------------
/**
 * �w�肳�ꂽ�Ƌ��z�u
 * @param	work	VILLA_WORK
 * @param	no		�Ƌ�ԍ�
 * @retval	nothing
 */
//--------------------------------------------------------------
static void Villa_MdlSet( VILLA_WORK *work, const u32 no )
{
	int pi,i;
	FTURE_OBJ *obj;
	VecFx32 rot = {0,0,0};
	int bmid = DATA_FTureBMIDTbl[no];
	const FTURE_DATA *fdata = DATA_FTureDataTbl;
	FIELDSYS_WORK *fsys = work->fsys;
	
	//no�Ŕz�u���鐔���A���[�N�̋󂫂ɃZ�b�g
	for( pi = 0; pi < FTURE_PUT_MAX; pi++, fdata++ ){
		if( fdata->no == no ){
			i = 0;
			obj = work->obj_tbl;
			
			do{
				if( obj->use_flag == FALSE ){
					obj->use_flag = TRUE;
					obj->bmid = bmid;
					obj->entry_idx = M3DO_SetMap3DObjExp(
						fsys->Map3DObjExp, fsys->MapResource,
						bmid, &fdata->draw_pos, &rot, fsys->field_3d_anime );
					obj->m3do = M3DO_GetMap3DObjDataFromID(
						fsys->Map3DObjExp, bmid );
					break;
				}
				i++;
				obj++;
			}while( i < FTURE_PUT_MAX );
		}
	}
}

//==============================================================================
//	�ʑ��@�p�[�c
//==============================================================================
//--------------------------------------------------------------
/**
 * �Ƌ�ԍ��Ή��C�x���g�t���O�`�F�b�N
 * @param	fsys	FIELDSYS_WORK
 * @param	no		�Ƌ�ԍ�
 * @retval	BOOL	TRUE=ON
 */
//--------------------------------------------------------------
static BOOL Villa_EvFlagCheck( FIELDSYS_WORK *fsys, u32 no )
{
	return( SysFlag_VillaFurniture(
		SaveData_GetEventWork(fsys->savedata),SYSFLAG_MODE_CHECK,no) );
}

//--------------------------------------------------------------
/**
 * �ʑ��@�Ƌ���蔻��`�F�b�N
 * @param	gx		�O���b�hX
 * @param	gz		�O���b�hZ
 * @param	fdata	FTURE_DATA
 * @param	fsys	FIELDSYS_WORK
 * @retval	BOOL	TRUE=�q�b�g
 */
//--------------------------------------------------------------
static BOOL Villa_FTureHitCheck(
	int gx, int gz, const FTURE_DATA *fdata, FIELDSYS_WORK *fsys )
{
	const FTURE_RECT *rect = &fdata->rect;
	
	if( fdata->hit_flag == TRUE &&
		Villa_EvFlagCheck(fsys,fdata->no) == TRUE ){
		if( gz >= rect->top && gz <= rect->bottom &&
			gx >= rect->left && gx <= rect->right ){
			return( TRUE );
		}
	}
	
	return( FALSE );
}

//==============================================================================
//	data
//==============================================================================
//--------------------------------------------------------------
///	�Ƌ���W�f�[�^�e�[�u��
//--------------------------------------------------------------
static const FTURE_DATA DATA_FTureDataTbl[FTURE_PUT_MAX] =
{
 {FTURE_0,TRUE,{GCFX(0x0c),GDFX(0),GDFX(0x07)},{0x0b,0x05,0x0d,0x08}},
 {FTURE_1,TRUE,{GDFX(0x0f),GDFX(0),GDFX(0x07)},{0x0e,0x05,0x0f,0x08}},
 {FTURE_2,TRUE,{GCFX(0x0d),GDFX(0),GCFX(0x0a)},{0x0b,0x09,0x0d,0x0a}},
 {FTURE_3,TRUE,{GCFX(0x12),GDFX(0),GDFX(0x08)},{0x12,0x06,0x14,0x09}}, 
 {FTURE_4,TRUE,{GCFX(0x14),GDFX(0),GCFX(0x07)},{0x11,0x06,0x11,0x07}}, 
 {FTURE_5,TRUE,{GDFX(0x0c),GDFX(0),GCFX(0x03)},{0x0b,0x03,0x0c,0x03}}, 
 {FTURE_6,TRUE,{GCFX(0x0e),GDFX(0),GCFX(0x03)},{0x0d,0x03,0x0f,0x03}}, 
 {FTURE_7,TRUE,{GDFX(0x11),GDFX(0),GCFX(0x03)},{0x10,0x03,0x11,0x03}}, 
 {FTURE_8,TRUE,{GDFX(0x13),GDFX(0),GCFX(0x03)},{0x12,0x03,0x13,0x03}}, 
 {FTURE_9,TRUE,{GCFX(0x01),GDFX(0),GCFX(0x03)},{0x01,0x03,0x01,0x03}}, 
 {FTURE_9,TRUE,{GCFX(0x14),GDFX(0),GCFX(0x03)},{0x14,0x03,0x14,0x03}}, 
 {FTURE_9,TRUE,{GCFX(0x01),GDFX(0),GCFX(0x0b)},{0x01,0x0b,0x01,0x0b}}, 
 {FTURE_9,TRUE,{GCFX(0x14),GDFX(0),GCFX(0x0b)},{0x14,0x0b,0x14,0x0b}}, 
 {FTURE_10,TRUE,{GDFX(0x02),GDFX(0),GCFX(0x08)},{0x01,0x07,0x02,0x09}},
 {FTURE_11,FALSE,{GCFX(0x12),GDFX(0),GCFX(0x03)},{0x00,0x00,0x00,0x00}},
 {FTURE_12,TRUE,{GCFX(0x02),GDFX(0),GCFX(0x03)},{0x02,0x03,0x02,0x03}},
 {FTURE_13,TRUE,{GCFX(0x06),GDFX(0),GCFX(0x03)},{0x06,0x03,0x06,0x03}},
 {FTURE_14,TRUE,{GCFX(0x02),GDFX(0),GCFX(0x05)},{0x01,0x04,0x02,0x06}},
 {FTURE_15,TRUE,{GDFX(0x06),GDFX(0),GDFX(0x0a)},{0x04,0x09,0x07,0x0a}},
 {FTURE_16,FALSE,{GCFX(0x0b),GDFX(0),GCFX(0x01)},{0x00,0x00,0x00,0x00}},
 {FTURE_17,FALSE,{GCFX(0x08),GDFX(0),GCFX(0x01)},{0x00,0x00,0x00,0x00}},
 {FTURE_18,FALSE,{GCFX(0x06),GDFX(0),GCFX(0x0a)},{0x00,0x00,0x00,0x00}},
 {FTURE_19,FALSE,{GCFX(0x07),GDFX(0),GCFX(0x06)},{0x00,0x00,0x00,0x00}},
};

#if 0
static const FTURE_DATA DATA_FTureDataTbl[FTURE_PUT_MAX] =
{
 {FTURE_0,TRUE,{GDFX(0x0d),GDFX(0),GCFX(0x07)},{0x0b,0x05,0x0d,0x08}},
 {FTURE_1,TRUE,{GCFX(0x0e),GDFX(0),GCFX(0x07)},{0x0e,0x05,0x0f,0x08}},
 {FTURE_2,TRUE,{GDFX(0x0d),GDFX(0),GCFX(0x09)},{0x0b,0x09,0x0d,0x0a}},
 {FTURE_3,TRUE,{GCFX(0x12),GDFX(0),GCFX(0x07)},{0x11,0x06,0x13,0x09}}, 
 {FTURE_4,TRUE,{GCFX(0x14),GDFX(0),GDFX(0x07)},{0x14,0x06,0x14,0x08}}, 
 {FTURE_5,TRUE,{GDFX(0x0c),GDFX(0),GDFX(0x03)},{0x0b,0x03,0x0c,0x03}}, 
 {FTURE_6,TRUE,{GDFX(0x0e),GDFX(0),GDFX(0x03)},{0x0d,0x03,0x0f,0x03}}, 
 {FTURE_7,TRUE,{GDFX(0x11),GDFX(0),GDFX(0x03)},{0x10,0x03,0x11,0x03}}, 
 {FTURE_8,TRUE,{GDFX(0x08),GDFX(0),GDFX(0x03)},{0x12,0x03,0x13,0x03}}, 
 {FTURE_9,TRUE,{GCFX(0x01),GDFX(0),GDFX(0x03)},{0x01,0x03,0x01,0x03}}, 
 {FTURE_9,TRUE,{GCFX(0x14),GDFX(0),GDFX(0x03)},{0x14,0x03,0x14,0x03}}, 
 {FTURE_9,TRUE,{GCFX(0x01),GDFX(0),GDFX(0x0b)},{0x01,0x0b,0x01,0x0b}}, 
 {FTURE_9,TRUE,{GCFX(0x14),GDFX(0),GDFX(0x0b)},{0x14,0x0b,0x14,0x0b}}, 
 {FTURE_10,TRUE,{GDFX(0x02),GDFX(0),GCFX(0x08)},{0x01,0x07,0x02,0x09}},
 {FTURE_11,FALSE,{GCFX(0x12),GDFX(0),GCFX(0x03)},{0x00,0x00,0x00,0x00}},
 {FTURE_12,TRUE,{GCFX(0x02),GDFX(0),GDFX(0x03)},{0x02,0x03,0x02,0x03}},
 {FTURE_13,TRUE,{GCFX(0x06),GDFX(0),GDFX(0x03)},{0x06,0x03,0x06,0x03}},
 {FTURE_14,TRUE,{GCFX(0x02),GDFX(0),GDFX(0x05)},{0x01,0x04,0x03,0x06}},
 {FTURE_15,TRUE,{GDFX(0x06),GDFX(0),GDFX(0x0a)},{0x04,0x09,0x07,0x0a}},
 {FTURE_16,FALSE,{GCFX(0x0b),GDFX(0),GCFX(0x01)},{0x00,0x00,0x00,0x00}},
 {FTURE_17,FALSE,{GCFX(0x08),GDFX(0),GCFX(0x01)},{0x00,0x00,0x00,0x00}},
 {FTURE_18,FALSE,{GCFX(0x06),GDFX(0),GCFX(0x0a)},{0x00,0x00,0x00,0x00}},
 {FTURE_19,FALSE,{GCFX(0x07),GDFX(0),GCFX(0x06)},{0x00,0x00,0x00,0x00}},
};
#endif

//--------------------------------------------------------------
///	�Ƌ�ԍ��ʔz�u���f��ID
//--------------------------------------------------------------
static const u32 DATA_FTureBMIDTbl[FTURE_MAX] =
{
	BMID_T7_O01,
	BMID_T7_O02,
	BMID_T7_O03,
	BMID_T7_O04,
	BMID_T7_O05,
	BMID_T7_O06,
	BMID_T7_O07,
	BMID_T7_O08,
	BMID_T7_O09,
	BMID_T7_O10,
	BMID_T7_O11,
	BMID_T7_O12,
	BMID_T7_O13,
	BMID_T7_O14,
	BMID_T7_O15,
	BMID_T7_O16,
	BMID_T7_O17,
	BMID_T7_O18,
	BMID_T7_O19,
	BMID_T7_O20,
};

//--------------------------------------------------------------
///	�Ƌ�ԍ��ʘb�������C�x���g�f�[�^
//--------------------------------------------------------------
static const FTURE_TALKDATA DATA_FTureTalkEventDataTbl[FTURE_MAX] =
{
	{SCRID_T07R0201_FURNITURE_01},
	{SCRID_T07R0201_FURNITURE_02},
	{SCRID_T07R0201_FURNITURE_03},
	{SCRID_T07R0201_FURNITURE_04},
	{SCRID_T07R0201_FURNITURE_05},
	//{SCRID_T07R0201_FURNITURE_06},
	{SCRID_TV},							//�ʏ��TV���N��������炵���ł� 08.04.15
	{SCRID_T07R0201_FURNITURE_07},
	{SCRID_T07R0201_FURNITURE_08},
	{SCRID_T07R0201_FURNITURE_09},
	{SCRID_T07R0201_FURNITURE_10},
	{SCRID_T07R0201_FURNITURE_11},
	{SCRID_T07R0201_FURNITURE_12},
	{SCRID_T07R0201_FURNITURE_13},
	{SCRID_T07R0201_FURNITURE_14},
	{SCRID_T07R0201_FURNITURE_15},
	{SCRID_T07R0201_FURNITURE_16},
	{SCRID_T07R0201_FURNITURE_17},
	{SCRID_T07R0201_FURNITURE_18},
	{SCRID_T07R0201_FURNITURE_19},
	{SCRID_T07R0201_FURNITURE_20},
};
