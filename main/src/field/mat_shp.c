//============================================================================================
/**
 * @file	mat_shp.c
 * @bfief	�}�b�g�E�V�F�C�v�f�[�^�֘A�t�@�C��
 * @author	Nozomu Saitou
 *
 */
//============================================================================================
#include "mat_shp.h"

//1mat1shape�Ή��f�[�^�@�o�C�i���t�H�[�}�b�g



typedef struct MAT_SHP_HEADER_tag
{
	u16 Num;	//�I�u�W�F�N�g������MAT_SHP�̗v�f��
	u16	Idx;	//MAT_SHP�z��̎Q��IndexNo
}MAT_SHP_HEADER;

typedef struct MAT_SHP_DATA_tag
{
	MAT_SHP_HEADER	*Header;
	MAT_SHP			*MatShp;
}MAT_SHP_DATA;

//==============================================================================
/**
 * �}�b�g�E�V�F�C�v�f�[�^�̃������m��
 *
 * @retval  MSD_PTR		�}�b�g�E�V�F�C�v�f�[�^�|�C���^	
 */
//==============================================================================
MSD_PTR AllocMatShpData(void)
{
	MSD_PTR data;
	data = sys_AllocMemory( HEAPID_FIELD, sizeof(MAT_SHP_DATA) );
	GF_ASSERT( data != NULL && "ERROR:mat_shp no enough memory" );
	return data;
}

//==============================================================================
/**
 * �}�b�g�E�V�F�C�v�f�[�^�̃��[�h
 *
 * @param   inName			�t�@�C���p�X
 * @param	outData			�}�b�g�E�V�F�C�v�f�[�^�|�C���^
 *
 * @retval  none	
 */
//==============================================================================
void LoadMatShape(const char * inName, MSD_PTR outData)
{
	FSFile file;
	u16 obj_num;
	u16 mat_shp_num;
	int header_size;
	int mat_shp_size;

	FS_InitFile(&file);
	if (FS_OpenFile(&file, inName)){
		int ret;
		ret = FS_ReadFile(&file, &obj_num, 2);
		GF_ASSERT( ret >= 0 && "ERROR:mat_shp can not read" );
		
		ret = FS_ReadFile(&file, &mat_shp_num, 2);
		GF_ASSERT( ret >= 0 && "ERROR:mat_shp can not read" );
		
		//�������m��
		header_size = sizeof(MAT_SHP_HEADER)*obj_num;
		mat_shp_size = sizeof(MAT_SHP)*mat_shp_num;

		outData->Header = sys_AllocMemory( HEAPID_FIELD, header_size );
		GF_ASSERT( outData->Header != NULL&& "ERROR:mat_shp no enough memory" );
		outData->MatShp = sys_AllocMemory( HEAPID_FIELD, mat_shp_size );
		GF_ASSERT( outData->MatShp != NULL&& "ERROR:mat_shp no enough memory" );

		ret = FS_ReadFile(&file, outData->Header, header_size);
		GF_ASSERT( ret >= 0 && "ERROR:mat_shp can not read" );
		ret = FS_ReadFile(&file, outData->MatShp, mat_shp_size);
		GF_ASSERT( ret >= 0 && "ERROR:mat_shp can not read" );

		(void)FS_CloseFile(&file);
	}else{
		OS_Printf("FS_OpenFile(\"%s\") ... ERROR!\n",inName);
		GF_ASSERT(0);
	}
}

//==============================================================================
/**
 * �}�b�g�E�V�F�C�v�f�[�^�̉��
 *
 * @param	outData			�}�b�g�E�V�F�C�v�f�[�^�|�C���^
 *
 * @retval  none	
 */
//==============================================================================
void FreeMatShpData(MSD_PTR outData)
{
	sys_FreeMemoryEz(outData->MatShp);
	sys_FreeMemoryEz(outData->Header);
	sys_FreeMemoryEz(outData);
}

//==============================================================================
/**
 * �}�b�g�E�V�F�C�v���̎擾
 *
 * @param   inObjIdx		3�cOBJ�C���f�b�N�X
 * @param	inData			�}�b�g�E�V�F�C�v�f�[�^�|�C���^
 * @param	outNum			�}�b�g�E�V�F�C�v��
 *
 * @retval  none	
 */
//==============================================================================
void GetMatShpNum(const int inObjIdx,
						MSD_CONST_PTR inData,
						u16 *outNum)
{
	*outNum = inData->Header[inObjIdx].Num;
}

//==============================================================================
/**
 * �}�b�g�E�V�F�C�v���ƃC���f�b�N�X���擾
 *
 * @param   inObjIdx		3�cOBJ�C���f�b�N�X
 * @param	inData			�}�b�g�E�V�F�C�v�f�[�^�|�C���^
 * @param	outNum			�}�b�g�E�V�F�C�v��
 * @param	outIdx			�C���f�b�N�X
 *
 * @retval  none	
 */
//==============================================================================
void GetMatShpNumAndIdx(const int inObjIdx,
						MSD_CONST_PTR inData,
						u16 *outNum,
						u16 *outIdx)
{
	*outNum = inData->Header[inObjIdx].Num;
	*outIdx = inData->Header[inObjIdx].Idx;
}

//==============================================================================
/**
 * �C���f�b�N�X���L�[�ɂ��āA�}�b�g�E�V�F�C�v���擾
 *
 * @param   inIdx			�C���f�b�N�X
 * @param	inData			�}�b�g�E�V�F�C�v�f�[�^�|�C���^
 *
 * @retval  MAT_SHP			�}�b�g�E�V�F�C�v�\���̃|�C���^
 */
//==============================================================================
MAT_SHP const *GetMatShp(const u16 inIdx, MSD_CONST_PTR inData)
{
	return &inData->MatShp[inIdx];
}

