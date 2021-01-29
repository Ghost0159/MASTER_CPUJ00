#ifndef _HEIGHT_H_
#define _HEIGHT_H_
#include <nitro.h>

#include "common.h"
#include "../height_def.h"

//#include "grid_def.h"

//XZ���_�f�[�^
typedef struct XZ_VERTEX_tag
{
	fx32 X;
	fx32 Z;
}XZ_VERTEX;
/**
//�|���S���f�[�^��
typedef struct SPLIT_GRID_DATA_tag{
	u16		NumX;		//�O���b�h�w��
	u16		NumZ;		//�O���b�h�y��

	fx32	StartX;		//�n�_�ʒu�w
	fx32	StartZ;		//�n�_�ʒu�y
	fx32	EndX;		//�I�_�ʒu�w
	fx32	EndZ;		//�I�_�ʒu�y
	fx32	SizeX;		//�O���b�h�w�T�C�Y
	fx32	SizeZ;		//�O���b�h�y�T�C�Y
}SPLIT_GRID_DATA;
*/
typedef struct POLYGON_DATA_tag{
	u16 vtx_idx0;
	u16 vtx_idx1;
///	u16 vtx_idx2;	//3�p�|���S�����`������A���_�f�[�^�z��ւ̃C���f�b�N�XNo
	u16	nrm_idx;	//�@���f�[�^�z��(���K���ς�)�ւ̃C���f�b�N�XNo
	
///	fx32	paramD;		//���ʂ̕��������瓱�����A�␳�lD
	u16 d_idx;
}POLYGON_DATA;

typedef struct HEIGHT_ARRAY_tag
{
	fx32 Height;
	int Prev;
	int Next;
}HEIGHT_ARRAY;

typedef struct LINE_DATA_tag
{
	fx32 LineZ;
	u16 EntryNum;
	u16 ParamIndex;
}LINE_DATA;

typedef struct MAP_HEIGHT_INFO_tag{
///	SPLIT_GRID_DATA	*SplitGridData;
	POLYGON_DATA	*PolygonData;
	fx32			*DArray;
///	u16				*GridDataTbl;
	/*u16*/LINE_DATA				*LineDataTbl;
	u16				*PolyIDList;
	XZ_VERTEX		*VertexArray;
	VecFx32			*NormalArray;
	BOOL			DataValid;		//�f�[�^�L���L��
	int				LineNum;
}MAP_HEIGHT_INFO;

#endif	//_HEIGHT_H_
