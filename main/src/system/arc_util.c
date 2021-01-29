//============================================================================================
/**
 * @file	arc_util.c
 * @bfief	�A�[�J�C�u�f�[�^��֗��Ɏg�����߂̃��[�e�B���e�B�֐��Q
 * @author	taya
 * @date	05.08.30
 * @date	07.01.23	HANDLE�o�[�W�������쐬	tomoya takahashi
 */
//============================================================================================
#include "common.h"

#include "system\arc_tool.h"
#include "system\arc_util.h"


//==============================================================
// Prototype
//==============================================================
static u32 ArcUtil_BgCharSetCommon( void* p_data, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize );
static void ArcUtil_ScrnSetCommon( void* p_data, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize );
static u32 ArcUtil_ObjCharSetCommon( void* p_data, OBJTYPE objType, u32 offs, u32 transSize );
static void ArcUtil_PalSetEzCommon( void* p_data, PALTYPE palType, u32 srcOfs, u32 dstOfs, u32 transSize );
static void ArcUtil_PalSysLoadCommon( void* p_data, NNS_G2D_VRAM_TYPE type, u32 offs, NNSG2dImagePaletteProxy* proxy );
static u32 ArcUtil_CharSysLoadCommon( void* p_data, CHAR_MAPPING_TYPE mapType, u32 transSize, NNS_G2D_VRAM_TYPE vramType, u32 offs, NNSG2dImageProxy* proxy );
static void ArcUtil_CharSysLoadSyncroMappingModeCommon( void* p_data, CHAR_MAPPING_TYPE mapType, u32 transSize, NNS_G2D_VRAM_TYPE vramType, u32 offs, NNSG2dImageProxy* proxy );
static const void* ArcUtil_TransTypeCharSysLoadCommon( void* p_data, NNS_G2D_VRAM_TYPE vramType, u32 offs, NNSG2dImageProxy* proxy, NNSG2dCharacterData** charData );
static void* ArcUtil_CharDataGetCommon( void* p_data, NNSG2dCharacterData** charData );
static void* ArcUtil_ScrnDataGetCommon( void* p_data, NNSG2dScreenData** scrnData );
static void* ArcUtil_PalDataGetCommon( void* p_data, NNSG2dPaletteData** palData );
static void* ArcUtil_CellBankDataGetCommon( void* p_data, NNSG2dCellDataBank** cellBank );
static void* ArcUtil_AnimBankDataGetCommon( void* p_data, NNSG2dAnimBankData** anmBank );

//------------------------------------------------------------------
/**
 * BG����ް��� VRAM �]��
 *
 * @param   fileIdx			������̧�ٲ��ޯ��
 * @param   dataIndex		�������ް����ޯ��
 * @param   bgl				BGL�ް�
 * @param   frm				�]�����ڰ�����
 * @param   offs			�]���̾�āi��גP�ʁj
 * @param	transSize		�]�����黲�ށi�޲ĒP�� ==0�őS�]���j
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   heapID			�ް��ǂݍ��݁E�������؂Ƃ��Ďg��˰��ID
 *
 * @return  �]�������f�[�^�T�C�Y�i�o�C�g�j
 */
//------------------------------------------------------------------
u32 ArcUtil_BgCharSet(u32 fileIdx, u32 dataIdx, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize, BOOL compressedFlag, u32 heapID)
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_BgCharSetCommon( arcData, bgl, frm, offs, transSize );
}
//--------------------------------------------------------------------------------------------
/**
 * ��ذ��ް��� VRAM �]��
 * �� BGL���� ��ذ��ޯ̧ ���p�ӂ���Ă���΁A��ذ��ޯ̧ �ւ̓]�����s��
 *
 * @param   fileIdx			������̧�ٲ��ޯ��
 * @param   arcIndex		�������ް����ޯ��
 * @param   bgl				BGL�ް�
 * @param   frm				�]�����ڰ�����
 * @param   offs			�]���̾�āi��גP�ʁj
 * @param	transSize		�]�����黲�ށi�޲ĒP�� ==0�őS�]���j
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   heapID			�ް��ǂݍ��݁E�������؂Ƃ��Ďg��˰��ID
 *
 */
//--------------------------------------------------------------------------------------------
void ArcUtil_ScrnSet(u32 fileIdx, u32 dataIdx, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize, BOOL compressedFlag, u32 heapID)
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	ArcUtil_ScrnSetCommon( arcData, bgl, frm, offs, transSize );
}
//------------------------------------------------------------------
/**
 * ��گ��ް� �� VRAM �]��
 *
 * @param   fileIdx		������̧�ٲ��ޯ��
 * @param   dataIdx		�������ް����ޯ��
 * @param   palType		��گē]��������
 * @param   offs		��گē]����̾��
 * @param   transSize	��گē]�����ށi0 �őS�]���j
 * @param   heapID		�ް��ǂݍ��������؂Ƃ��Ďg��˰��ID
 *
 */
//------------------------------------------------------------------
void ArcUtil_PalSet( u32 fileIdx, u32 dataIdx, PALTYPE palType, u32 offs, u32 transSize, u32 heapID )
{
	ArcUtil_PalSetEx( fileIdx, dataIdx, palType, 0, offs, transSize, heapID );
}

//------------------------------------------------------------------
/**
 * ��گ��ް� �� VRAM �]���i�]�����̓ǂݍ��݊J�n�̾�Ďw��Łj
 *
 * @param   fileIdx		������̧�ٲ��ޯ��
 * @param   dataIdx		�������ް����ޯ��
 * @param   palType		��گē]��������
 * @param   srcOfs		��گē]�����ǂݍ��݊J�n�̾��
 * @param   dstOfs		��گē]����̾��
 * @param   transSize	��گē]�����ށi0 �őS�]���j
 * @param   heapID		�ް��ǂݍ��������؂Ƃ��Ďg��˰��ID
 *
 */
//------------------------------------------------------------------
void ArcUtil_PalSetEx( u32 fileIdx, u32 dataIdx, PALTYPE palType, u32 srcOfs, u32 dstOfs, u32 transSize, u32 heapID )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, FALSE, heapID, ALLOC_BOTTOM );
	ArcUtil_PalSetEzCommon( arcData, palType, srcOfs, dstOfs, transSize );
}

//------------------------------------------------------------------
/**
 * OBJ ����ް� �� VRAM �]��
 *
 * @param   fileIdx				������̧�ٲ��ޯ��
 * @param   dataIdx				�������ް����ޯ��
 * @param   objType				OBJ����(OBJTYPE_MAIN or OBJTYPE_SUB�j
 * @param   offs				�̾�āi�޲ĒP�ʁj
 * @param   transSize			�]�����ށi�޲ĒP�� : 0 �őS�]���j
 * @param   compressedFlag		���k���ꂽ�ް����H
 * @param   heapID				�ǂݍ��݁E�������؂Ƃ��Ďg��˰��ID
 *
 * @return  �]�������f�[�^�T�C�Y�i�o�C�g�j
 */
//------------------------------------------------------------------
u32 ArcUtil_ObjCharSet( u32 fileIdx, u32 dataIdx, OBJTYPE objType, u32 offs, u32 transSize, BOOL compressedFlag, u32 heapID )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	return ArcUtil_ObjCharSetCommon( arcData, objType, offs, transSize );
}

//------------------------------------------------------------------
/**
 * ��گ��ް� �� VRAM �]���� NITRO System ��گ���۷� ���쐬
 *�i3D, OBJ �p�ɂ̂ݑΉ��BBG �ɂ͎g���܂���j
 *
 * @param   fileIdx		[in] ������̧�ٲ��ޯ��
 * @param   dataIdx		[in] �������ް����ޯ��
 * @param   type		[in] �]��������
 * @param   offs		[in] �]���̾��
 * @param   heapID		[in] ˰��ID
 * @param   proxy		[out]�쐬������۷����ڽ
 *
 *	[ type ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
void ArcUtil_PalSysLoad( u32 fileIdx, u32 dataIdx, NNS_G2D_VRAM_TYPE type, u32 offs, u32 heapID, NNSG2dImagePaletteProxy* proxy )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, FALSE, heapID, ALLOC_BOTTOM );
	ArcUtil_PalSysLoadCommon( arcData, type, offs, proxy );
}
//------------------------------------------------------------------
/**
 * ����ް� �� VRAM �]���� NITRO System �Ұ����۷� ���쐬
 *�i3D, OBJ �p�ɂ̂ݑΉ��BBG �ɂ͎g���܂���j
 *
 *
 * @param   fileIdx			[in] ������̧�ٲ��ޯ��
 * @param   dataIdx			[in] �������ް����ޯ��
 * @param   compressedFlag	[in] ���k����Ă��邩
 * @param   mapType			[in] ϯ��ݸ�����
 * @param   transSize		[in] �]���T�C�Y�B�O�Ȃ�S�]���B
 * @param   vramType		[in] �]��������
 * @param   offs			[in] �]���̾��
 * @param   heapID			[in] �����؂Ɏg��˰��ID
 * @param   proxy			[out] �쐬������۷����ڽ
 *
 *
 *	[ mapType ]
 *		CHAR_MAP_1D = 0,
 *		CHAR_MAP_2D = 1,
 *
 *	�� VRAM�]���^�͕ʊ֐� ArcUtil_TranCharSysLoad ���g��
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
u32 ArcUtil_CharSysLoad( u32 fileIdx, u32 dataIdx, BOOL compressedFlag, CHAR_MAPPING_TYPE mapType, u32 transSize,
	NNS_G2D_VRAM_TYPE vramType, u32 offs, u32 heapID, NNSG2dImageProxy* proxy )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	return ArcUtil_CharSysLoadCommon( arcData, mapType, transSize, vramType, offs, proxy );
}

//------------------------------------------------------------------
/**
 * ����ް� �� VRAM �]���� NITRO System �Ұ����۷� ���쐬
 *�i3D, OBJ �p�ɂ̂ݑΉ��BBG �ɂ͎g���܂���j
 *
 * ����ް���ϯ��ݸ�Ӱ�ޒl���A���݂�ڼ޽��ݒ�ɍ��킹�ď��������܂�
 *
 * @param   fileIdx			[in] ������̧�ٲ��ޯ��
 * @param   dataIdx			[in] �������ް����ޯ��
 * @param   compressedFlag	[in] ���k����Ă��邩
 * @param   mapType			[in] ϯ��ݸ�����
 * @param   transSize		[in] �]���T�C�Y�B�O�Ȃ�S�]���B
 * @param   vramType		[in] �]��������
 * @param   offs			[in] �]���̾��
 * @param   heapID			[in] �����؂Ɏg��˰��ID
 * @param   proxy			[out] �쐬������۷����ڽ
 *
 *
 *	[ mapType ]
 *		CHAR_MAP_1D = 0,
 *		CHAR_MAP_2D = 1,
 *
 *	�� VRAM�]���^�͕ʊ֐� ArcUtil_TranCharSysLoad ���g��
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
void ArcUtil_CharSysLoadSyncroMappingMode( u32 fileIdx, u32 dataIdx, BOOL compressedFlag, CHAR_MAPPING_TYPE mapType, u32 transSize,
	NNS_G2D_VRAM_TYPE vramType, u32 offs, u32 heapID, NNSG2dImageProxy* proxy )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	ArcUtil_CharSysLoadSyncroMappingModeCommon( arcData, mapType, transSize, vramType, offs, proxy );
}

//------------------------------------------------------------------
/**
 * ����ް� �� VRAM �]���� NITRO System �Ұ����۷� ���쐬�BVRAM�]���^�̉摜�f�ޗp�B
 *
 * �� ���̊֐����g���Ă��AVRAM�ɉ摜�͓]������܂���
 *    ���̊֐���۰�ނ����ް��͉������܂���B�߂�l��const void*���Ǘ����āA
 *    �s�v�ɂȂ��������������s���Ă��������B
 *
 * @param   fileIdx			[in] ������̧�ٲ��ޯ��
 * @param   dataIdx			[in] �������ް����ޯ��
 * @param   compressedFlag	[in] ���k����Ă��邩
 * @param   vramType		[in] �]��������
 * @param   offs			[in] �]���̾��
 * @param   heapID			[in] �����؂Ɏg��˰��ID
 * @param   proxy			[out] �쐬������۷��̱��ڽ
 * @param   charData		[out] ����ް����ڽ��ێ������߲���̱��ڽ
 *
 * @retval  const void*		۰�ނ����ް��̱��ڽ
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
const void* ArcUtil_TransTypeCharSysLoad( u32 fileIdx, u32 dataIdx, BOOL compressedFlag, 
	NNS_G2D_VRAM_TYPE vramType, u32 offs, u32 heapID, NNSG2dImageProxy* proxy, NNSG2dCharacterData** charData )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_TransTypeCharSysLoadCommon( arcData, vramType, offs, proxy, charData );
}



//------------------------------------------------------------------
/**
 * ����ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   fileIdx				[in] ������̧�ٲ��ޯ��
 * @param   dataIdx				[in] �ް����ޯ��
 * @param   compressedFlag		[in] ���k����Ă��邩
 * @param   charData			[out] ����ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID				[in] ˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//------------------------------------------------------------------
void* ArcUtil_CharDataGet( u32 fileIdx, u32 dataIdx, BOOL compressedFlag, NNSG2dCharacterData** charData, u32 heapID )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_CharDataGetCommon( arcData, charData );
}
//--------------------------------------------------------------------------------------------
/**
 * ��ذ��ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   fileIdx			[in] ������̧�ٲ��ޯ��
 * @param   arcIndex		[in] �������ް����ޯ��
 * @param   compressedFlag	[in] ���k����Ă����ް����H
 * @param   scrnData		[out] ��ذ��ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			[in] ˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//--------------------------------------------------------------------------------------------
void* ArcUtil_ScrnDataGet(u32 fileIdx, u32 dataIdx, BOOL compressedFlag, NNSG2dScreenData** scrnData, u32 heapID)
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_ScrnDataGetCommon( arcData, scrnData );
}
//------------------------------------------------------------------
/**
 * ��گ��ް���۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   fileIdx			������̧�ٲ��ޯ��
 * @param   arcIndex		�������ް����ޯ��
 * @param   palData			��گ��ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//------------------------------------------------------------------
void* ArcUtil_PalDataGet( u32 fileIdx, u32 dataIdx, NNSG2dPaletteData** palData, u32 heapID )
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, FALSE, heapID, ALLOC_TOP );
	return ArcUtil_PalDataGetCommon( arcData, palData );
}
//--------------------------------------------------------------------------------------------
/**
 * ����ݸ�ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   fileIdx			������̧�ٲ��ޯ��
 * @param   arcIndex		�������ް����ޯ��
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   cellBank		����ݸ�ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			˰��ID
 * @param   pSize			����ݸ���޲Ļ��ނ��󂯎��ϐ��߲���i�v��Ȃ����NULL�Łj
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//--------------------------------------------------------------------------------------------
void* ArcUtil_CellBankDataGet(u32 fileIdx, u32 dataIdx, BOOL compressedFlag, NNSG2dCellDataBank** cellBank, u32 heapID )
{
	void* arcData;
	arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_CellBankDataGetCommon( arcData, cellBank );
}
//--------------------------------------------------------------------------------------------
/**
 * �����ݸ�ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   fileIdx			������̧�ٲ��ޯ��
 * @param   arcIndex		�������ް����ޯ��
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   anmBank			�����ݸ�ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//--------------------------------------------------------------------------------------------
void* ArcUtil_AnimBankDataGet(u32 fileIdx, u32 dataIdx, BOOL compressedFlag, NNSG2dAnimBankData** anmBank, u32 heapID)
{
	void* arcData = ArcUtil_Load( fileIdx, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_AnimBankDataGetCommon( arcData, anmBank );
}











//------------------------------------------------------------------
/**
 * LZ���k��A�[�J�C�u����Ă���f�[�^��ǂݏo���A�𓀂��ĕԂ�
 *
 * @param   fileIdx		�A�[�J�C�u�t�@�C���C���f�b�N�X
 * @param   dataIdx		�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   heapID		�ǂݏo���E�𓀂Ɏg���q�[�v�h�c
 *
 * @retval  void*		�𓀌�̃f�[�^�ۑ���A�h���X
 */
//------------------------------------------------------------------
void* ArcUtil_UnCompress(u32 fileIdx, u32 dataIdx, u32 heapID)
{
	return ArcUtil_Load(fileIdx, dataIdx, TRUE, heapID, ALLOC_TOP);
}

//------------------------------------------------------------------
/**
 * �A�[�J�C�u�f�[�^�̓ǂݏo���i�������m�ۂ���j
 *
 * @param   fileIdx			�A�[�J�C�u�t�@�C���C���f�b�N�X
 * @param   dataIdx			�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   compressedFlag	���k����Ă��邩�H
 * @param   heapID			�������m�ۂɎg���q�[�v�h�c
 * @param   allocType		�q�[�v�̂ǂ̈ʒu���烁�����m�ۂ��邩
 *
 * @retval  void*			�ǂݏo���̈�|�C���^
 */
//------------------------------------------------------------------
void* ArcUtil_Load(u32 fileIdx, u32 dataIdx, BOOL compressedFlag, u32 heapID, ALLOC_TYPE allocType)
{
	void* arcData;

	if( compressedFlag || allocType == ALLOC_BOTTOM )
	{
		arcData = sys_AllocMemoryLo( heapID, ArchiveDataSizeGet(fileIdx, dataIdx) );
	}
	else
	{
		arcData = sys_AllocMemory( heapID, ArchiveDataSizeGet(fileIdx, dataIdx) );
	}

	if( arcData != NULL )
	{
		ArchiveDataLoad(arcData, fileIdx, dataIdx);
		if( compressedFlag )
		{
			void* data;

			if( allocType == ALLOC_TOP )
			{
				data = sys_AllocMemory( heapID, MI_GetUncompressedSize( arcData ) );
			}
			else
			{
				data = sys_AllocMemoryLo( heapID, MI_GetUncompressedSize( arcData ) );
			}

			if( data )
			{
				MI_UncompressLZ8( arcData, data );
				sys_FreeMemoryEz( arcData );
			}
			arcData = data;
		}
	}
	return arcData;
}
//------------------------------------------------------------------
/**
 * �A�[�J�C�u�f�[�^�̓ǂݏo���i�ǂݏo�����T�C�Y���擾�ł���j
 *
 * @param   fileIdx			�A�[�J�C�u�t�@�C���C���f�b�N�X
 * @param   dataIdx			�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   compressedFlag	���k����Ă��邩�H
 * @param   heapID			�������m�ۂɎg���q�[�v�h�c
 * @param   allocType		�q�[�v�̂ǂ̈ʒu���烁�����m�ۂ��邩
 * @param   pSize			���f�[�^�̃o�C�g�T�C�Y�i���k����Ă���ꍇ�͓W�J��j
 *
 * @retval  void*			�ǂݏo���̈�|�C���^
 */
//------------------------------------------------------------------
void* ArcUtil_LoadEx(u32 fileIdx, u32 dataIdx, BOOL compressedFlag, u32 heapID, ALLOC_TYPE allocType, u32* pSize)
{
	void* arcData;

	*pSize = ArchiveDataSizeGet(fileIdx, dataIdx);

	if( compressedFlag || allocType == ALLOC_BOTTOM )
	{
		arcData = sys_AllocMemoryLo( heapID, *pSize );
	}
	else
	{
		arcData = sys_AllocMemory( heapID, *pSize );
	}

	if( arcData != NULL )
	{
		ArchiveDataLoad(arcData, fileIdx, dataIdx);
		if( compressedFlag )
		{
			void* data;

			*pSize = MI_GetUncompressedSize( arcData );

			if( allocType == ALLOC_TOP )
			{
				data = sys_AllocMemory( heapID, *pSize );
			}
			else
			{
				data = sys_AllocMemoryLo( heapID, *pSize );
			}

			if( data )
			{
				MI_UncompressLZ8( arcData, data );
				sys_FreeMemoryEz( arcData );
			}
			arcData = data;
		}
	}
	return arcData;
}


//-----------------------------------------------------------------------------
/**
 *		ARCHANDLE�o�[�W����
 */
//-----------------------------------------------------------------------------
//------------------------------------------------------------------
/**
 * BG����ް��� VRAM �]��
 *
 * @param   handle			�n���h��
 * @param   arcIndex		�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   bgl				BGL�ް�
 * @param   frm				�]�����ڰ�����
 * @param   offs			�]���̾�āi��גP�ʁj
 * @param	transSize		�]�����黲�ށi�޲ĒP�� ==0�őS�]���j
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   heapID			�ް��ǂݍ��݁E�������؂Ƃ��Ďg��˰��ID
 *
 * @return  �]�������f�[�^�T�C�Y�i�o�C�g�j
 */
//------------------------------------------------------------------
u32 ArcUtil_HDL_BgCharSet(ARCHANDLE* handle, u32 dataIdx, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize, BOOL compressedFlag, u32 heapID)
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_BgCharSetCommon( arcData, bgl, frm, offs, transSize );
}

//--------------------------------------------------------------------------------------------
/**
 * ��ذ��ް��� VRAM �]��
 * �� BGL���� ��ذ��ޯ̧ ���p�ӂ���Ă���΁A��ذ��ޯ̧ �ւ̓]�����s��
 *
 * @param   handle			�n���h��
 * @param   arcIndex		�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   bgl				BGL�ް�
 * @param   frm				�]�����ڰ�����
 * @param   offs			�]���̾�āi��גP�ʁj
 * @param	transSize		�]�����黲�ށi�޲ĒP�� ==0�őS�]���j
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   heapID			�ް��ǂݍ��݁E�������؂Ƃ��Ďg��˰��ID
 *
 */
//--------------------------------------------------------------------------------------------
void ArcUtil_HDL_ScrnSet(ARCHANDLE* handle, u32 dataIdx, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize, BOOL compressedFlag, u32 heapID)
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	ArcUtil_ScrnSetCommon( arcData, bgl, frm, offs, transSize );
}

//------------------------------------------------------------------
/**
 * OBJ ����ް� �� VRAM �]��
 *
 * @param   handle				�n���h��
 * @param   dataIdx				�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   objType				OBJ����
 * @param   offs				�̾�āi�޲ĒP�ʁj
 * @param   transSize			�]�����ށi�޲ĒP�� : 0 �őS�]���j
 * @param   compressedFlag		���k���ꂽ�ް����H
 * @param   heapID				�ǂݍ��݁E�������؂Ƃ��Ďg��˰��ID
 *
 * @return  �]�������f�[�^�T�C�Y�i�o�C�g�j
 */
//------------------------------------------------------------------
u32 ArcUtil_HDL_ObjCharSet( ARCHANDLE* handle, u32 dataIdx, OBJTYPE objType, u32 offs, u32 transSize, BOOL compressedFlag, u32 heapID )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	return ArcUtil_ObjCharSetCommon( arcData, objType, offs, transSize );
}

//------------------------------------------------------------------
/**
 * ��گ��ް� �� VRAM �]��
 *
 * @param   handle		�n���h��
 * @param   dataIdx		�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   palType		��گē]��������
 * @param   offs		��گē]����̾��
 * @param   transSize	��گē]�����ށi0 �őS�]���j
 * @param   heapID		�ް��ǂݍ��������؂Ƃ��Ďg��˰��ID
 *
 */
//------------------------------------------------------------------
void ArcUtil_HDL_PalSet( ARCHANDLE* handle, u32 dataIdx, PALTYPE palType, u32 offs, u32 transSize, u32 heapID )
{
	ArcUtil_HDL_PalSetEx( handle, dataIdx, palType, 0, offs, transSize, heapID );
}


//------------------------------------------------------------------
/**
 * ��گ��ް� �� VRAM �]���i�]�����̓ǂݍ��݊J�n�̾�Ďw��Łj
 *
 * @param   handle		�n���h��
 * @param   dataIdx		�������ް����ޯ��
 * @param   palType		��گē]��������
 * @param   srcOfs		��گē]�����ǂݍ��݊J�n�̾��
 * @param   dstOfs		��گē]����̾��
 * @param   transSize	��گē]�����ށi0 �őS�]���j
 * @param   heapID		�ް��ǂݍ��������؂Ƃ��Ďg��˰��ID
 *
 */
//------------------------------------------------------------------
void ArcUtil_HDL_PalSetEx( ARCHANDLE* handle, u32 dataIdx, PALTYPE palType, u32 srcOfs, u32 dstOfs, u32 transSize, u32 heapID )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, FALSE, heapID, ALLOC_BOTTOM );
	ArcUtil_PalSetEzCommon( arcData, palType, srcOfs, dstOfs, transSize );
}


//------------------------------------------------------------------
/**
 * ��گ��ް� �� VRAM �]���� NITRO System ��گ���۷� ���쐬
 *�i3D, OBJ �p�ɂ̂ݑΉ��BBG �ɂ͎g���܂���j
 *
 * @param   handle		�n���h��
 * @param   dataIdx		�������ް����ޯ��
 * @param   type		�]��������
 * @param   offs		�]���̾��
 * @param   heapID		˰��ID
 * @param   proxy		�쐬������۷��̱��ڽ
 *
 *	[ type ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
void ArcUtil_HDL_PalSysLoad( ARCHANDLE* handle, u32 dataIdx, NNS_G2D_VRAM_TYPE type, u32 offs, u32 heapID, NNSG2dImagePaletteProxy* proxy )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, FALSE, heapID, ALLOC_BOTTOM );
	ArcUtil_PalSysLoadCommon( arcData, type, offs, proxy );
}

//------------------------------------------------------------------
/**
 * ����ް� �� VRAM �]���� NITRO System �Ұ����۷� ���쐬
 *�i3D, OBJ �p�ɂ̂ݑΉ��BBG �ɂ͎g���܂���j
 *
 *
 * @param   handle			�n���h��
 * @param   dataIdx			�������ް����ޯ��
 * @param   compressedFlag	���k����Ă��邩
 * @param   mapType			ϯ��ݸ�����
 * @param   transSize		�]�����ށi0�Ȃ�S�]���j
 * @param   vramType		�]��������
 * @param   offs			�]���̾��
 * @param   heapID			�����؂Ɏg��˰��ID
 * @param   proxy			�쐬������۷��̱��ڽ
 *
 * @retval  �]�����ꂽ�ް����ށi�޲ĒP�ʁj
 *
 *	[ mapType ]
 *		MAP_TYPE_1D = 0,
 *		MAP_TYPE_2D = 1,
 *
 *	�� VRAM�]���^�͕ʊ֐� ArcUtil_TranCharSysLoad ���g��
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
u32 ArcUtil_HDL_CharSysLoad( ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, CHAR_MAPPING_TYPE mapType,
	u32 transSize, NNS_G2D_VRAM_TYPE vramType, u32 offs, u32 heapID, NNSG2dImageProxy* proxy )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	return ArcUtil_CharSysLoadCommon( arcData, mapType, transSize, vramType, offs, proxy );
}

//------------------------------------------------------------------
/**
 * ����ް� �� VRAM �]���� NITRO System �Ұ����۷� ���쐬
 *�i3D, OBJ �p�ɂ̂ݑΉ��BBG �ɂ͎g���܂���j
 *
 * ����ް���ϯ��ݸ�Ӱ�ޒl���A���݂�ڼ޽��ݒ�ɍ��킹�ď��������܂�
 *
 * @param   handle			�n���h��
 * @param   dataIdx			�������ް����ޯ��
 * @param   compressedFlag	���k����Ă��邩
 * @param   mapType			ϯ��ݸ�����
 * @param   transSize		�]���T�C�Y�B�O�Ȃ�S�]���B
 * @param   vramType		�]��������
 * @param   offs			�]���̾��
 * @param   heapID			�����؂Ɏg��˰��ID
 * @param   proxy			�쐬������۷����ڽ
 *
 *
 *	[ mapType ]
 *		CHAR_MAP_1D = 0,
 *		CHAR_MAP_2D = 1,
 *
 *	�� VRAM�]���^�͕ʊ֐� ArcUtil_TranCharSysLoad ���g��
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
void ArcUtil_HDL_CharSysLoadSyncroMappingMode( ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, CHAR_MAPPING_TYPE mapType, u32 transSize,
	NNS_G2D_VRAM_TYPE vramType, u32 offs, u32 heapID, NNSG2dImageProxy* proxy )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_BOTTOM );
	ArcUtil_CharSysLoadSyncroMappingModeCommon( arcData, mapType, transSize, vramType, offs, proxy );
}

//------------------------------------------------------------------
/**
 * ����ް� �� VRAM �]���� NITRO System �Ұ����۷� ���쐬�BVRAM�]���^�̉摜�f�ޗp�B
 *
 * �� ���̊֐����g���Ă��AVRAM�ɉ摜�͓]������܂���
 *    ���̊֐���۰�ނ����ް��͉������܂���B�߂�l��const void*���Ǘ����āA
 *    �s�v�ɂȂ��������������s���Ă��������B
 *
 * @param   handle			�n���h��
 * @param   dataIdx			�������ް����ޯ��
 * @param   compressedFlag	���k����Ă��邩
 * @param   vramType		�]��������
 * @param   offs			�]���̾��
 * @param   heapID			�����؂Ɏg��˰��ID
 * @param   proxy			�쐬������۷��̱��ڽ
 * @param   charData		����ް����ڽ��ێ������߲���̱��ڽ
 *
 * @retval  const void*		۰�ނ����ް��̱��ڽ
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 */
//------------------------------------------------------------------
const void* ArcUtil_HDL_TransTypeCharSysLoad( ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, 
	NNS_G2D_VRAM_TYPE vramType, u32 offs, u32 heapID, NNSG2dImageProxy* proxy, NNSG2dCharacterData** charData )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_TransTypeCharSysLoadCommon( arcData, vramType, offs, proxy, charData );
}

//------------------------------------------------------------------
/**
 * ����ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   handle				�n���h��
 * @param   dataIdx				�ް����ޯ��
 * @param   compressedFlag		���k����Ă��邩
 * @param   charData			����ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID				˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//------------------------------------------------------------------
void* ArcUtil_HDL_CharDataGet( ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, NNSG2dCharacterData** charData, u32 heapID )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_CharDataGetCommon( arcData, charData );
}

//--------------------------------------------------------------------------------------------
/**
 * ��ذ��ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   handle			�n���h��
 * @param   arcIndex		�������ް����ޯ��
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   scrnData		��ذ��ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			�ް��ǂݍ��݁E�������؂Ƃ��Ďg��˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//--------------------------------------------------------------------------------------------
void* ArcUtil_HDL_ScrnDataGet(ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, NNSG2dScreenData** scrnData, u32 heapID)
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_ScrnDataGetCommon( arcData, scrnData );
}

//------------------------------------------------------------------
/**
 * ��گ��ް���۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   handle			�n���h��
 * @param   arcIndex		�������ް����ޯ��
 * @param   palData			��گ��ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//------------------------------------------------------------------
extern void* ArcUtil_HDL_PalDataGet( ARCHANDLE* handle, u32 dataIdx, NNSG2dPaletteData** palData, u32 heapID )
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, FALSE, heapID, ALLOC_TOP );
	return ArcUtil_PalDataGetCommon( arcData, palData );
}

//--------------------------------------------------------------------------------------------
/**
 * ����ݸ�ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   handle			�n���h��
 * @param   arcIndex		�������ް����ޯ��
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   cellBank		����ݸ�ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//--------------------------------------------------------------------------------------------
void* ArcUtil_HDL_CellBankDataGet(ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, NNSG2dCellDataBank** cellBank, u32 heapID )
{
	void* arcData;
	arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_CellBankDataGetCommon( arcData, cellBank );
}

//--------------------------------------------------------------------------------------------
/**
 * �����ݸ�ް��� ۰�ނ��� Unpack ���邾���ł��B����͊e���ŁB
 *
 * @param   handle			�n���h��
 * @param   arcIndex		�������ް����ޯ��
 * @param   compressedFlag	���k����Ă����ް����H
 * @param   anmBank			�����ݸ�ް����ڽ��ێ������߲���̱��ڽ
 * @param   heapID			˰��ID
 *
 * @retval  void*		۰�ނ����ް��̐擪�߲��
 */
//--------------------------------------------------------------------------------------------
void* ArcUtil_HDL_AnimBankDataGet(ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, NNSG2dAnimBankData** anmBank, u32 heapID)
{
	void* arcData = ArcUtil_HDL_Load( handle, dataIdx, compressedFlag, heapID, ALLOC_TOP );
	return ArcUtil_AnimBankDataGetCommon( arcData, anmBank );
}


//------------------------------------------------------------------
/**
 * LZ���k��A�[�J�C�u����Ă���f�[�^��ǂݏo���A�𓀂��ĕԂ�
 *
 * @param   handle		�n���h��
 * @param   dataIdx		�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   heapID		�ǂݏo���E�𓀂Ɏg���q�[�v�h�c
 *
 * @retval  void*		�𓀌�̃f�[�^�ۑ���A�h���X
 */
//------------------------------------------------------------------
void* ArcUtil_HDL_UnCompress(ARCHANDLE* handle, u32 dataIdx, u32 heapID)
{
	return ArcUtil_HDL_Load(handle, dataIdx, TRUE, heapID, ALLOC_TOP);
}


//------------------------------------------------------------------
/**
 * �A�[�J�C�u�f�[�^�̓ǂݏo��
 *
 * @param   handle			�n���h��
 * @param   dataIdx			�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   compressedFlag	���k����Ă��邩�H
 * @param   heapID			�������m�ۂɎg���q�[�v�h�c
 * @param   allocType		�q�[�v�̂ǂ̈ʒu���烁�����m�ۂ��邩
 *
 * [allocType]
 *		ALLOC_TOP		�q�[�v�擪����m��
 *		ALLOC_BOTTOM	�q�[�v�������m��
 *
 * @retval  void*			�ǂݏo���̈�|�C���^
 */
//------------------------------------------------------------------
void* ArcUtil_HDL_Load(ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, u32 heapID, ALLOC_TYPE allocType)
{
	u32 siz_dmy;
	return ArcUtil_HDL_LoadEx( handle, dataIdx, compressedFlag, heapID, allocType, &siz_dmy );
}

//------------------------------------------------------------------
/**
 * �A�[�J�C�u�f�[�^�̓ǂݏo�����f�[�^�T�C�Y�擾�i���k����Ă�����𓀌�̃T�C�Y���擾����j
 *
 * @param   handle			�n���h��
 * @param   dataIdx			�A�[�J�C�u�f�[�^�C���f�b�N�X
 * @param   compressedFlag	���k����Ă��邩�H
 * @param   heapID			�������m�ۂɎg���q�[�v�h�c
 * @param   allocType		�q�[�v�̂ǂ̈ʒu���烁�����m�ۂ��邩
 * @param   pSize			���f�[�^�̃o�C�g�T�C�Y���󂯎��ϐ��̃|�C���^
 *
 * [allocType]
 *		ALLOC_TOP		�q�[�v�擪����m��
 *		ALLOC_BOTTOM	�q�[�v�������m��
 *
 * @retval  void*			�ǂݏo���̈�|�C���^
 */
//------------------------------------------------------------------
void* ArcUtil_HDL_LoadEx(ARCHANDLE* handle, u32 dataIdx, BOOL compressedFlag, u32 heapID, ALLOC_TYPE allocType, u32* pSize)
{
	void* arcData;

	*pSize = ArchiveDataSizeGetByHandle(handle, dataIdx);

	if( compressedFlag || allocType == ALLOC_BOTTOM )
	{
		arcData = sys_AllocMemoryLo( heapID, *pSize );
	}
	else
	{
		arcData = sys_AllocMemory( heapID, *pSize );
	}

	if( arcData != NULL )
	{
		ArchiveDataLoadByHandle(handle, dataIdx, arcData);

		if( compressedFlag )
		{
			void* data;

			*pSize = MI_GetUncompressedSize( arcData );

			if( allocType == ALLOC_TOP )
			{
				data = sys_AllocMemory( heapID, *pSize );
			}
			else
			{
				data = sys_AllocMemoryLo( heapID, *pSize );
			}

			if( data )
			{
				MI_UncompressLZ8( arcData, data );
				sys_FreeMemoryEz( arcData );
			}
			arcData = data;
		}
	}
	return arcData;
}


//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	BG�L�����f�[�^��VRAM�]�����ʕ���
 *
 *	@param	p_data			�f�[�^
 *	@param	bgl				BGL�f�[�^
 *	@param	frm				�]����t���[���i���o
 *	@param	offs			�]���I�t�Z�b�g�i�L�����P�ʁj
 *	@param	transSize		�]���T�C�Y�i�o�C�g�P�� ==0�őS�]���j
 *
 *	@return	�]�������f�[�^�T�C�Y�i�o�C�g�j
 */
//-----------------------------------------------------------------------------
static u32 ArcUtil_BgCharSetCommon( void* p_data, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize )
{
	if( p_data != NULL )
	{
		NNSG2dCharacterData* charData;

		if( NNS_G2dGetUnpackedBGCharacterData( p_data, &charData ) )
		{
			if( transSize == 0 )
			{
				transSize = charData->szByte;
			}
			GF_BGL_LoadCharacter(bgl, frm, charData->pRawData, transSize, offs);
		}

		sys_FreeMemoryEz( p_data );
	}
    return transSize;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^VRAM�]��
 *	�� BGL���� ��ذ��ޯ̧ ���p�ӂ���Ă���΁A��ذ��ޯ̧ �ւ̓]�����s��
 *
 *	@param	p_data			�f�[�^
 *	@param	bgl				GBL�f�[�^
 *	@param	frm				�]����t���[��
 *	@param	offs			�]���I�t�Z�b�g�i�L�����P�ʁj
 *	@param	transSize		�]���T�C�Y�i�o�C�g�P�� ==0�őS�]���j
 */
//-----------------------------------------------------------------------------
static void ArcUtil_ScrnSetCommon( void* p_data, GF_BGL_INI* bgl, u32 frm, u32 offs, u32 transSize )
{
	if( p_data != NULL )
	{
		NNSG2dScreenData* scrnData;

		if( NNS_G2dGetUnpackedScreenData( p_data, &scrnData ) )
		{
			if( transSize == 0 )
			{
				transSize = scrnData->szByte;
			}

			if( GF_BGL_ScreenAdrsGet( bgl, frm ) != NULL )
			{
				GF_BGL_ScreenBufSet( bgl, frm, scrnData->rawData, transSize );
			}
			GF_BGL_LoadScreen( bgl, frm, scrnData->rawData, transSize, offs );
		}
		sys_FreeMemoryEz( p_data );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	OBJ ����ް� �� VRAM �]��
 *
 *	@param	p_data		�f�[�^
 *	@param	objType		OBJ�^�C�v�iOBJTYPE_MAIN or OBJTYPE_SUB�j
 *	@param	offs		�I�t�Z�b�g�i�o�C�g�P�ʁj
 *	@param	transSize	�]���T�C�Y�i�o�C�g�P�� ==0�őS�]���j
 *
 *	@return	�]�������f�[�^�T�C�Y�i�o�C�g�P�ʁj
 */
//-----------------------------------------------------------------------------
static u32 ArcUtil_ObjCharSetCommon( void* p_data, OBJTYPE objType, u32 offs, u32 transSize )
{
	static void (* const load_func[])(const void*, u32, u32) = {
		GX_LoadOBJ,
		GXS_LoadOBJ,
	};

	if( p_data != NULL )
	{
		NNSG2dCharacterData* charData;

		if( NNS_G2dGetUnpackedCharacterData( p_data, &charData ) )
		{
			if( transSize == 0 )
			{
				transSize = charData->szByte;
			}

			DC_FlushRange( charData->pRawData, transSize );
			load_func[ objType ]( charData->pRawData, offs, transSize );
		}
		sys_FreeMemoryEz( p_data );
	}
	return transSize;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�f�[�^��VRAM�]��
 *
 *	@param	p_data			�f�[�^
 *	@param	palType			�p���b�g�]���^�C�v
 *	@param	srcOfs			�p���b�g�]�����ǂݍ��݊J�n�I�t�Z�b�g
 *	@param	dstOfs			�p���b�g�]����I�t�Z�b�g
 *	@param	transSize		�]���T�C�Y�i�O�őS�]���j
 */
//-----------------------------------------------------------------------------
static void ArcUtil_PalSetEzCommon( void* p_data, PALTYPE palType, u32 srcOfs, u32 dstOfs, u32 transSize )
{
	static void (* const load_func[])(const void*, u32, u32) = {
		GX_LoadBGPltt,
		GX_LoadOBJPltt,
		GX_LoadBGExtPltt,
		GX_LoadOBJExtPltt,
		GXS_LoadBGPltt,
		GXS_LoadOBJPltt,
		GXS_LoadBGExtPltt,
		GXS_LoadOBJExtPltt,
	};

	if( p_data != NULL )
	{
		NNSG2dPaletteData* palData;

		if( NNS_G2dGetUnpackedPaletteData( p_data, &palData ) )
		{
			(u8*)(palData->pRawData) += srcOfs;
			if( transSize == 0 )
			{
				transSize = palData->szByte - srcOfs;
			}
			DC_FlushRange( palData->pRawData, transSize );

			switch( palType ){
			case PALTYPE_MAIN_BG_EX:
				GX_BeginLoadBGExtPltt();
				load_func[ palType ]( palData->pRawData, dstOfs, transSize );
				GX_EndLoadBGExtPltt();
				break;

			case PALTYPE_SUB_BG_EX:
				GXS_BeginLoadBGExtPltt();
				load_func[ palType ]( palData->pRawData, dstOfs, transSize );
				GXS_EndLoadBGExtPltt();
				break;

			case PALTYPE_MAIN_OBJ_EX:
				GX_BeginLoadOBJExtPltt();
				load_func[ palType ]( palData->pRawData, dstOfs, transSize );
				GX_EndLoadOBJExtPltt();
				break;

			case PALTYPE_SUB_OBJ_EX:
				GXS_BeginLoadOBJExtPltt();
				load_func[ palType ]( palData->pRawData, dstOfs, transSize );
				GXS_EndLoadOBJExtPltt();
				break;

			default:
				load_func[ palType ]( palData->pRawData, dstOfs, transSize );
				break;
			}
		}

		sys_FreeMemoryEz( p_data );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�f�[�^��VRAM�]����NITROSystem�p���b�g�v���N�V���쐬
 *	�i3D,OBJ�p�ɂ̂ݑΉ��@BG�ɂ͎g���܂���j
 *
 *	@param	p_data		�f�[�^
 *	@param	type		�]����^�C�v
 *	@param	offs		�]���I�t�Z�b�g
 *	@param	proxy		�쐬����v���N�V
 *	[ type ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 */
//-----------------------------------------------------------------------------
static void ArcUtil_PalSysLoadCommon( void* p_data, NNS_G2D_VRAM_TYPE type, u32 offs, NNSG2dImagePaletteProxy* proxy )
{
	if( p_data != NULL )
	{
		NNSG2dPaletteData*  palData;
		NNSG2dPaletteCompressInfo*  cmpData;
		BOOL  cmpFlag;

		cmpFlag = NNS_G2dGetUnpackedPaletteCompressInfo( p_data, &cmpData );

		if( NNS_G2dGetUnpackedPaletteData( p_data, &palData ) )
		{
			if( cmpFlag )
			{
				NNS_G2dLoadPaletteEx( palData, cmpData, offs, type, proxy );
			}
			else
			{
				NNS_G2dLoadPalette( palData, offs, type, proxy );
			}
		}

		sys_FreeMemoryEz( p_data );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����f�[�^��VRAM�]����NITRO System�C���[�W�v���L�V���쐬
 *	�i3D,OBJ�p�ɂ̂ݑΉ��@BG�ɂ͎g���܂���j
 *
 *	@param	p_data			�f�[�^
 *	@param	mapType			�}�b�s���O�^�C�v
 *	@param	transSize		�]���T�C�Y�i�O���S�]���j
 *	@param	vramType		�]����^�C�v
 *	@param	offs			�]����I�t�Z�b�g
 *	@param	proxy			�쐬����v���L�V�A�h���X
 *
 *	[ mapType ]
 *		CHAR_MAP_1D = 0,
 *		CHAR_MAP_2D = 1,
 *
 *	�� VRAM�]���^�͕ʊ֐� ArcUtil_TranCharSysLoad ���g��
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 *
 *	@return	�]�������f�[�^�T�C�Y
 */
//-----------------------------------------------------------------------------
static u32 ArcUtil_CharSysLoadCommon( void* p_data, CHAR_MAPPING_TYPE mapType, u32 transSize, NNS_G2D_VRAM_TYPE vramType, u32 offs, NNSG2dImageProxy* proxy )
{
	static void (* const load_func[])(const NNSG2dCharacterData*, u32, NNS_G2D_VRAM_TYPE, NNSG2dImageProxy*) = {
		NNS_G2dLoadImage1DMapping,
		NNS_G2dLoadImage2DMapping,
	};
	u32  transed_size = 0;

	if( p_data != NULL )
	{
		NNSG2dCharacterData* charData;

		if( NNS_G2dGetUnpackedCharacterData( p_data, &charData ) )
		{
			if( transSize )
			{
				charData->szByte = transSize;
			}
			load_func[ mapType ]( charData, offs, vramType, proxy );
			transed_size = charData->szByte;
		}
		sys_FreeMemoryEz( p_data );
	}

	return transed_size;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����f�[�^��VRAM�]����NITRO System�C���[�W�v���L�V���쐬
 *	�i3D,OBJ�p�ɂ̂ݑΉ��@BG�ɂ͎g���܂���j
 * ����ް���ϯ��ݸ�Ӱ�ޒl���A���݂�ڼ޽��ݒ�ɍ��킹�ď��������܂�
 *
 *	@param	p_data			�f�[�^
 *	@param	mapType			�}�b�s���O�^�C�v
 *	@param	transSize		�]���T�C�Y�i�O���S�]���j
 *	@param	vramType		�]����^�C�v
 *	@param	offs			�]����I�t�Z�b�g
 *	@param	proxy			�쐬����v���L�V�A�h���X
 *
 *	[ mapType ]
 *		CHAR_MAP_1D = 0,
 *		CHAR_MAP_2D = 1,
 *
 *	�� VRAM�]���^�͕ʊ֐� ArcUtil_TranCharSysLoad ���g��
 *
 *	[ vramType ]
 *		NNS_G2D_VRAM_TYPE_3DMAIN = 0,
 *		NNS_G2D_VRAM_TYPE_2DMAIN = 1,
 *		NNS_G2D_VRAM_TYPE_2DSUB  = 2,
 */
//-----------------------------------------------------------------------------
static void ArcUtil_CharSysLoadSyncroMappingModeCommon( void* p_data, CHAR_MAPPING_TYPE mapType, u32 transSize, NNS_G2D_VRAM_TYPE vramType, u32 offs, NNSG2dImageProxy* proxy )
{
	static void (* const load_func[])(const NNSG2dCharacterData*, u32, NNS_G2D_VRAM_TYPE, NNSG2dImageProxy*) = {
		NNS_G2dLoadImage1DMapping,
		NNS_G2dLoadImage2DMapping,
	};

	if( p_data != NULL )
	{
		NNSG2dCharacterData* charData;

		if( NNS_G2dGetUnpackedCharacterData( p_data, &charData ) )
		{
			if( transSize )
			{
				charData->szByte = transSize;
			}

			switch( vramType ){
			case NNS_G2D_VRAM_TYPE_2DMAIN:
				charData->mapingType = GX_GetOBJVRamModeChar();
				break;
			case NNS_G2D_VRAM_TYPE_2DSUB:
				charData->mapingType = GXS_GetOBJVRamModeChar();
				break;
			}
			load_func[ mapType ]( charData, offs, vramType, proxy );
		}
		sys_FreeMemoryEz( p_data );
	}
}	

//----------------------------------------------------------------------------
/**
 *	@brief	����ް� �� VRAM �]���� NITRO System �Ұ����۷� ���쐬�BVRAM�]���^�̉摜�f�ޗp�B
 *
 * �� ���̊֐����g���Ă��AVRAM�ɉ摜�͓]������܂���
 *    ���̊֐���۰�ނ����ް��͉������܂���B�߂�l��const void*���Ǘ����āA
 *    �s�v�ɂȂ��������������s���Ă��������B
 *
 *	@param	p_data			�f�[�^
 *	@param	vramType		�]����^�C�v
 *	@param	offs			�]���I�t�Z�b�g
 *	@param	proxy			�쐬����v���L�V
 *	@param	charData		�L�����N�^�f�[�^�A�h���X�i�[��
 *
 *	@return const void*		���[�h�����f�[�^�A�h���X
 */
//-----------------------------------------------------------------------------
static const void* ArcUtil_TransTypeCharSysLoadCommon( void* p_data, NNS_G2D_VRAM_TYPE vramType, u32 offs, NNSG2dImageProxy* proxy, NNSG2dCharacterData** charData )
{
	if( p_data != NULL )
	{
		if( NNS_G2dGetUnpackedCharacterData( p_data, charData ) )
		{
			NNS_G2dLoadImageVramTransfer( *charData, offs, vramType, proxy );
		}
	}
	return p_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�L�����N�^�f�[�^���A���p�b�N
 *
 *	@param	p_data		�f�[�^
 *	@param	charData	�L�����N�^�f�[�^�A���p�b�N��
 *
 *	@return �f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void* ArcUtil_CharDataGetCommon( void* p_data, NNSG2dCharacterData** charData )
{
	if( p_data != NULL )
	{
		if( NNS_G2dGetUnpackedBGCharacterData( p_data, charData ) == FALSE)
		{
			// ���s������NULL
			sys_FreeMemoryEz( p_data );
			return NULL;
		}
	}
	return p_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^�A���p�b�N
 *
 *	@param	p_data		�f�[�^
 *	@param	scrnData	�X�N���[���f�[�^�i�[��
 *
 *	@return �f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void* ArcUtil_ScrnDataGetCommon( void* p_data, NNSG2dScreenData** scrnData )
{
	if( p_data != NULL )
	{
		if( NNS_G2dGetUnpackedScreenData( p_data, scrnData ) == FALSE )
		{
			sys_FreeMemoryEz( p_data );
			return NULL;
		}
	}
	return p_data;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�p���b�g�f�[�^�A���p�b�N�֐�
 *
 *	@param	p_data		�f�[�^
 *	@param	palData		�p���b�g�f�[�^�i�[��
 *
 *	@return �f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void* ArcUtil_PalDataGetCommon( void* p_data, NNSG2dPaletteData** palData )
{
	if( p_data != NULL )
	{
		if( NNS_G2dGetUnpackedPaletteData( p_data, palData ) == FALSE )
		{
			sys_FreeMemoryEz( p_data );
			return NULL;
		}
	}
	return p_data;
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���f�[�^�A���p�b�N�֐�
 *
 *	@param	p_data		�f�[�^
 *	@param	cellBank	�Z���f�[�^�i�[��
 *
 *	@return	�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void* ArcUtil_CellBankDataGetCommon( void* p_data, NNSG2dCellDataBank** cellBank )
{
	if( p_data != NULL )
	{
		if( NNS_G2dGetUnpackedCellBank( p_data, cellBank ) == FALSE )
		{
			sys_FreeMemoryEz( p_data );
			return NULL;
		}
	}
	return p_data;
}	

//----------------------------------------------------------------------------
/**
 *	@brief	�Z���A�j���f�[�^�A���p�b�N
 *
 *	@param	p_data		�f�[�^
 *	@param	anmBank		�Z���A�j���f�[�^�i�[��
 *
 *	@return	�f�[�^���[�N
 */
//-----------------------------------------------------------------------------
static void* ArcUtil_AnimBankDataGetCommon( void* p_data, NNSG2dAnimBankData** anmBank )
{
	if( p_data != NULL )
	{
		if( NNS_G2dGetUnpackedAnimBank( p_data, anmBank ) == FALSE )
		{
			sys_FreeMemoryEz( p_data );
			return NULL;
		}
	}
	return p_data;
}
