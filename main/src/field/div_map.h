#ifndef _DIV_MAP_H_
#define _DIV_MAP_H_

#include "field_common.h"
#include "div_map_cont_def.h"
#include "worldmap.h"
#include "mapresource.h"
#include "field_glb_state.h"
#include "field_3d_anime.h"
#include "height_def.h"
#include "map3d_object.h"

typedef enum {
	MAP_LAOD_NONE,
	MAP_LOAD_RIGHT,
	MAP_LOAD_DOWN,
	MAP_LOAD_LEFT,
	MAP_LOAD_UP,
}MAP_LOAD_DIRECTION;

typedef enum {
	DML_LOAD_BLOCK,
	DML_LOAD_WAIT,
	DML_NONE,
	DML_FREE_WAIT,
}DML_SEQUENS;

typedef enum {
	BLOCKMODE_OVERLOAD = 0,		//�ʏ�͈̔͊O�ǂݍ��ݗL��^
	BLOCKMODE_2x2,				//2x2�u���b�N�Œ�^
}DIV_BLOCK_MODE;

typedef enum {
	DIVM3DO_SET,				//�z�u���f����W�J
	DIVM3DO_OFF,				//�z�u���f����W�J���Ȃ�
}DIV_M3DO_FLAG;

//�u���b�N���[�h�I����̃R�[���o�b�N
typedef void (*DIV_LOAD_CALL_BACK)(	void *work, const int inBlockIndex, const M3DOL_PTR inObjList );

extern void DivMapBindTarget(VecFx32 const *inTarget, DMC_PTR outDivMapCont);
extern void DivMapPurgeTarget(DMC_PTR outDivMapCont);

extern DMC_PTR InitDivMap(	WORLD_MAP_PTR world,
							MAP_RESOURCE_PTR mapresource,
							FLD_3D_ANM_MNG_PTR inFld3DAnmPtr,
							const DIV_MAP_MODE inMode );
extern void LoadDivMap(	DMC_PTR outDivMapCont, 
								const int x,
								const int z);
extern void DrawAroundBlock(DMC_CONST_PTR inDivMapCont,
							const GLST_DATA_PTR	glst_data);

extern void DivMapLoad_UpdateBlockData(const u8 inDirection, DMC_PTR ioDivMapCont);
//extern void UpdateNowData(const int inGridX, const int inGridZ, DMC_PTR outDivMapCont);
extern BOOL CheckNextGridIO(const u8 inDirection, DMC_CONST_PTR inDivMapCont, int *outGrid);
extern void GetGridXZ(fx32 inX, fx32 inZ, int *outGridX, int *outGridZ);
/**
void GetGridXZFromGridNo(	DMC_CONST_PTR inDivMapCont,
							const int inGridNo,
							int *outGridX,
							int *outGridZ);
*/							
extern BOOL WaitMapFree(DMC_CONST_PTR inDivMapCont);
extern void DivMapLoad_FreeMap(DMC_PTR ioDivMapCont);
extern void DivMapLoadMain(	FIELDSYS_WORK *fsys, DMC_PTR ioDivMapCont);

extern void FreeDivMapCont(DMC_PTR outDivMapCont);

//extern u32 GetNowZone(DMC_CONST_PTR inDivMapCont);
//extern u32 GetZoneIDFromGrid(const int inGrid,DMC_CONST_PTR inDivMapCont);
///extern u32 GetBlockIdxFromGridXZ(const int inGridX, const int inGridZ, const int inMapBlockW);

//�w�胍�[�J���C���f�b�N�X����n�a�i���X�g���擾
extern void Get3DObjListFromLocalIdx(	const u8 inIdx,
										DMC_CONST_PTR inDivMapCont,
										M3DOL_PTR *outObjList);

//���݂̃u���b�N���[�J���C���f�b�N�X���擾
extern u8 GetNowLocalIndex(DMC_CONST_PTR inDivMapCont);
//���݂̃u���b�N�C���f�b�N�X���擾
extern int GetNowBlockIndex(DMC_CONST_PTR inDivMapCont);
//���[�J���C���f�b�N�X����u���b�N�C���f�b�N�X���擾
extern int GetBlockIndexFromLocalIndex(DMC_CONST_PTR inDivMapCont, const u8 inLocalIndex);
//�u���b�N�C���f�b�N�X����n�`�I�t�Z�b�g���v�Z
extern u8 CalcPositionInBlockFromGridNo(const u32 inGridNo, const int inMapGridW);
extern u8 CalcLocalIdxFromBlockIdx(	const int inBlockIndex,
									const u8 inPosInBlock,
									DMC_CONST_PTR inDivMapCont);
extern BOOL CheckFourBlockIO(	DMC_CONST_PTR inDivMapCont,
								const int inX,
								const int inZ,
								u8 *outLocalIndex);

extern MHI_CONST_PTR GetDivMapHeightInfo(DMC_CONST_PTR inDivMapCont, const u8 inLocalIndex);
extern u16 const *GetDivMapAttribute(DMC_CONST_PTR inDivMapCont, const u8 inLocalIndex);

///extern void SetUpDivFuncList(FIELDSYS_WORK *fsys, const u8 inMode);

extern void DIVM_SetLoadCallBack( DMC_PTR outDivMapCont, DIV_LOAD_CALL_BACK call_back, void *work );

extern ARCHANDLE * GetDivMapArcHandle( DMC_PTR dmc );
extern DMC_PTR InitDivMapDummy(
	WORLD_MAP_PTR world, MAP_RESOURCE_PTR mapresource, ARCHANDLE *arc );
extern void InitDivMapDummyContinue(
	DMC_PTR map_cont_ptr,
	WORLD_MAP_PTR world,
	MAP_RESOURCE_PTR mapresource,
	ARCHANDLE *arc );
extern void DivMapLoad_FreeMapDummy( DMC_PTR ioDivMapCont );
extern void DivMapLoad_FreeMapDummyM3DObj( DMC_PTR ioDivMapCont );
extern void FreeDivMapContDummy( DMC_PTR outDivMapCont );
extern void LoadDivMapDummy( DMC_PTR outDivMapCont, const int x, const int z );
extern void ReloadDivMapDummy( DMC_PTR outDivMapCont, const int x, const int z );

extern void LoadBlockListSetUpForDummy( DMC_PTR outDivMapCont,
		const int inGridX, const int inGridZ, int *block_tbl );
extern void LoadBlockListNoForDummy(
		DMC_PTR outDivMapCont, int list_no, int block_no );

extern void DrawAroundBlockDummy( DMC_CONST_PTR inDivMapCont,
						const GLST_DATA_PTR glst_data );

extern void DivMapLoad_RemoveReset( DMC_PTR ioDivMapCont);
extern void RemoveDivMapDummyMain(
		WORLD_MAP_PTR world,
		MAP_RESOURCE_PTR mapres,
		DMC_PTR dmy_dmc, DMC_PTR main_dmc,
		const int inGridX, 
		const int inGridZ );
extern void RemoveDivMapMainDummy(
		DMC_PTR dmy_dmc, DMC_PTR main_dmc );
extern void DivMapLoadMainDummy( FIELDSYS_WORK *fsys, DMC_PTR ioDivMapCont );

extern void ResetDivMap(
		DMC_PTR map_cont_ptr,
		WORLD_MAP_PTR world,
		MAP_RESOURCE_PTR mapresource );
extern void LoadDivMapNonAlloc(
	DMC_PTR outDivMapCont, const int x, const int z);
extern void LoadBlockListSetUp( DMC_PTR outDivMapCont,
						const int inGridX, 
						const int inGridZ,
						int *list_tbl );
extern void LoadBlockListNoNonAlloc(
	DMC_PTR outDivMapCont, int list_no, int block_no );
extern void DivMapLoad_CleanM3DObj( DMC_PTR ioDivMapCont );
//pl �ǉ����
extern void SetDivMapOriginGridPos( DMC_PTR dmc, int x, int y, int z );
extern void SetDivMapBlockMode( DMC_PTR dmc, DIV_BLOCK_MODE mode );
extern int GetDivMapOriginGridX( DMC_CONST_PTR dmc );
extern int GetDivMapOriginGridY( DMC_CONST_PTR dmc );
extern int GetDivMapOriginGridZ( DMC_CONST_PTR dmc );
extern void GetDivMapOriginPos( DMC_CONST_PTR dmc, VecFx32 *pos );
extern void SetDivMapM3dObjSetFlag( DMC_PTR dmc, DIV_M3DO_FLAG flag );
extern void SetDivMapWorldMap( DMC_PTR dmc, WORLD_MAP_PTR inWorld );
extern void SetDivMapDrawFlag( DMC_PTR dmc, int block_no, BOOL flag );
extern void UpdateDivMapNowPos( DMC_PTR dmc, int gx, int gz );

extern void AllocOnlyDivMapBlockList(DMC_PTR outDivMapCont);


//debug
#ifdef PM_DEBUG
extern void SetDebugOutRangeFlg(void);
#endif

#endif //_DIV_MAP_H_
