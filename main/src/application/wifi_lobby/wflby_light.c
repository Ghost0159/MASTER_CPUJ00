//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *	GAME FREAK inc.
 *
 *	@file		wflby_light.c
 *	@brief		���C�g�R���g���[��
 *	@author		tomoya takahashi
 *	@data		2008.01.25
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"
#include "wflby_light.h"

//-----------------------------------------------------------------------------
/**
 *					�R�[�f�B���O�K��
 *		���֐���
 *				�P�����ڂ͑啶������ȍ~�͏������ɂ���
 *		���ϐ���
 *				�E�ϐ�����
 *						const�ɂ� c_ ��t����
 *						static�ɂ� s_ ��t����
 *						�|�C���^�ɂ� p_ ��t����
 *						�S�č��킳��� csp_ �ƂȂ�
 *				�E�O���[�o���ϐ�
 *						�P�����ڂ͑啶��
 *				�E�֐����ϐ�
 *						�������Ɓh�Q�h�Ɛ������g�p���� �֐��̈���������Ɠ���
*/
//-----------------------------------------------------------------------------
#ifdef PM_DEBUG
//#define WFLBY_DEBUG_LIGHT_PRINT_ON
#endif

#ifdef WFLBY_DEBUG_LIGHT_PRINT_ON
#define WFLBY_DEBUG_LIGHT_PRINT(...)		OS_TPrintf(__VA_ARGS__)
#else
#define WFLBY_DEBUG_LIGHT_PRINT(...)		((void)0);
#endif


//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define WFLBY_LIGHT_LIGHT_NUM	(2)		// ���C�g��
#define WFLBY_LIGHT_MAT_NUM		(4)		// �}�e���A��
//  RGB
enum{
	WFLBY_LIGHT_COL_R,
	WFLBY_LIGHT_COL_G,
	WFLBY_LIGHT_COL_B,
	WFLBY_LIGHT_COL_PAD,	// �p�f�B���O
	WFLBY_LIGHT_COL_NUM,
};

// ���C�g�f�[�^���f�t���[����
#define WFLBY_LIGHT_FADE_COUNT	( 60 )

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	���C�g�f�[�^
//=====================================
typedef struct{
	GXRgb lightcolor;
	GXRgb matcolor[WFLBY_LIGHT_MAT_NUM];
} WFLBY_LIGHTDATA_ROOM;
typedef struct{
	GXRgb lightcolor;
} WFLBY_LIGHTDATA_FLOOR;

//-------------------------------------
///	�J���[�f�[�^
//=====================================
typedef struct {
	s8	color[ WFLBY_LIGHT_COL_NUM ];
} WFLBY_LIGHT_COL;


//-------------------------------------
///	���C�g�ڍ׃f�[�^
//=====================================
typedef struct {
	WFLBY_LIGHT_COL lightcolor_ful[WFLBY_LIGHT_LIGHT_NUM];
	WFLBY_LIGHT_COL matcolor_ful[WFLBY_LIGHT_MAT_NUM];
} WFLBY_LIGHT_FULL;


//-------------------------------------
///	���C�g�V�X�e��
//=====================================
typedef struct _WFLBY_LIGHTWK {
	WFLBY_LIGHT_FULL	now;		// �ݒ�l
	WFLBY_LIGHT_FULL	last;		// �ύX�O
	WFLBY_LIGHT_FULL	next;		// �ύX��
	s32					lightcount[WFLBY_LIGHT_LIGHT_NUM];	// �ύX�J�E���^
	s32					matcount[WFLBY_LIGHT_MAT_NUM];	// �ύX�J�E���^
}WFLBY_LIGHTWK;


//-----------------------------------------------------------------------------
/**
 *					���C�g�f�[�^
 */
//-----------------------------------------------------------------------------
//-------------------------------------
///	�������C�g�f�[�^
//=====================================
static const WFLBY_LIGHTDATA_ROOM sc_WFLBY_LIGHTDATA_ROOM[ WFLBY_LIGHT_NEON_ROOMNUM ] = {
	// WFLBY_LIGHT_NEON_ROOM0
	{
		GX_RGB( 23,23,25 ),
		{
			GX_RGB( 16,16,16 ),
			GX_RGB( 14,14,14 ),
			GX_RGB( 20,20,20 ),
			GX_RGB( 16,16,16 ),
		},
	},

	// WFLBY_LIGHT_NEON_ROOM1
	{
		GX_RGB( 10,10,16 ),
		{
			GX_RGB( 16,16,16 ),
			GX_RGB( 14,14,14 ),
			GX_RGB( 20,20,20 ),
			GX_RGB( 16,16,16 ),
		},
	},

	// WFLBY_LIGHT_NEON_ROOM2
	{
		GX_RGB( 8,8,12 ),
		{
			GX_RGB( 16,16,16 ),
			GX_RGB( 14,14,14 ),
			GX_RGB( 20,20,20 ),
			GX_RGB( 16,16,16 ),
		},
	},

	// WFLBY_LIGHT_NEON_ROOM3
	{
		GX_RGB( 5,5,8 ),
		{
			GX_RGB( 16,16,16 ),
			GX_RGB( 13,13,14 ),
			GX_RGB( 20,20,20 ),
			GX_RGB( 16,16,16 ),
		},
	},

	// WFLBY_LIGHT_NEON_ROOM4
	{
		GX_RGB( 3,3,7 ),
		{
			GX_RGB( 16,16,16 ),
			GX_RGB( 10,10,12 ),
			GX_RGB( 20,20,20 ),
			GX_RGB( 14,14,16 ),
		},
	},

	// WFLBY_LIGHT_NEON_ROOM5
	{
		GX_RGB( 0,0,6 ),
		{
			GX_RGB( 16,16,16 ),
			GX_RGB( 8,8,10 ),
			GX_RGB( 20,20,20 ),
			GX_RGB( 12,12,16 ),
		},
	},
};

//-------------------------------------
///	���@���j�������g���C�g�f�[�^
//=====================================
static const WFLBY_LIGHTDATA_FLOOR sc_WFLBY_LIGHTDATA_FLOOR[ WFLBY_LIGHT_NEON_FLOORNUM ] = {
	// WFLBY_LIGHT_NEON_FLOOR0
	{
		GX_RGB( 14,14,14 ),
	},

	// WFLBY_LIGHT_NEON_FLOOR1
	{
		GX_RGB( 20,20,20 ),
	},

	// WFLBY_LIGHT_NEON_FLOOR2
	{
		GX_RGB( 31,31,31 ),
	},

	// WFLBY_LIGHT_NEON_FLOOR3
	{
		GX_RGB( 8,8,8 ),
	},

};



//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WFLBY_LIGHT_ConvetLightColToNitro( const WFLBY_LIGHT_COL* cp_data, GXRgb* p_col );
static void WFLBY_LIGHT_ConvetNitroToLightCol( WFLBY_LIGHT_COL* p_data, const GXRgb* cp_col );
static void WFLBY_LIGHT_Col_Marge( const WFLBY_LIGHT_COL* cp_data0, const WFLBY_LIGHT_COL* cp_data1, WFLBY_LIGHT_COL* p_datain, s32 count, s32 countmax );

static void WFLBY_LIGHT_Ful_SetLIGHTDATA_ROOM( WFLBY_LIGHT_FULL* p_data, const WFLBY_LIGHTDATA_ROOM* cp_lightdata );
static void WFLBY_LIGHT_Ful_SetLIGHTDATA_FLOOR( WFLBY_LIGHT_FULL* p_data, const WFLBY_LIGHTDATA_FLOOR* cp_lightdata );
static void WFLBY_LIGHT_Ful_SetLight( const WFLBY_LIGHT_FULL* cp_data0 );


//----------------------------------------------------------------------------
/**
 *	@brief	���C�g������
 *
 *	@param	heapID		�q�[�vID
 *	@param	roomtype	�����^�C�v
 *	@param	floor		���^�C�v
 *	@param	monu		���j�������g�^�C�v
 *
 *	@return	�o�������[�N
 */
//-----------------------------------------------------------------------------
WFLBY_LIGHTWK* WFLBY_LIGHT_Init( u32 heapID, WFLBY_LIGHT_NEON_ROOMTYPE roomtype, WFLBY_LIGHT_NEON_FLOORTYPE floor, WFLBY_LIGHT_NEON_MONUTYPE monu )
{
	WFLBY_LIGHTWK* p_wk;
	int i;

	p_wk = sys_AllocMemory( heapID, sizeof(WFLBY_LIGHTWK) );
	memset( p_wk, 0, sizeof(WFLBY_LIGHTWK) );

	WFLBY_LIGHT_Ful_SetLIGHTDATA_ROOM( &p_wk->now, &sc_WFLBY_LIGHTDATA_ROOM[ roomtype ] );
	WFLBY_LIGHT_Ful_SetLIGHTDATA_FLOOR( &p_wk->now, &sc_WFLBY_LIGHTDATA_FLOOR[ floor ] );

	for( i=0; i<WFLBY_LIGHT_LIGHT_NUM; i++ ){
		p_wk->lightcount[i] = WFLBY_LIGHT_FADE_COUNT;
	}
	for( i=0; i<WFLBY_LIGHT_MAT_NUM; i++ ){
		p_wk->matcount[i] = WFLBY_LIGHT_FADE_COUNT;
	}

	WFLBY_LIGHT_Ful_SetLight( &p_wk->now );
	
	return p_wk;
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C�g�j��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_LIGHT_Exit( WFLBY_LIGHTWK* p_wk )
{
	sys_FreeMemoryEz( p_wk );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C�g���C��
 *
 *	@param	p_wk	���[�N
 */
//-----------------------------------------------------------------------------
void WFLBY_LIGHT_Main( WFLBY_LIGHTWK* p_wk )
{
	int i;
	BOOL set_light = FALSE;

	for( i=0; i<WFLBY_LIGHT_LIGHT_NUM;  i++ ){
		if( (p_wk->lightcount[i]+1) <= WFLBY_LIGHT_FADE_COUNT ){
			p_wk->lightcount[i] ++;
			WFLBY_LIGHT_Col_Marge( &p_wk->last.lightcolor_ful[i],  &p_wk->next.lightcolor_ful[i], 
					&p_wk->now.lightcolor_ful[i], p_wk->lightcount[i], WFLBY_LIGHT_FADE_COUNT );
			set_light = TRUE;
		}
	}

	for( i=0; i<WFLBY_LIGHT_MAT_NUM;  i++ ){
		if( (p_wk->matcount[i]+1) <= WFLBY_LIGHT_FADE_COUNT ){
			p_wk->matcount[i] ++;
			WFLBY_LIGHT_Col_Marge( &p_wk->last.matcolor_ful[i],  &p_wk->next.matcolor_ful[i], 
					&p_wk->now.matcolor_ful[i], p_wk->matcount[i], WFLBY_LIGHT_FADE_COUNT );
			set_light = TRUE;
		}
	}

	if( set_light ){
		WFLBY_LIGHT_Ful_SetLight( &p_wk->now );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	�����l�I���ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	roomtype	�����^�C�v
 */
//-----------------------------------------------------------------------------
void WFLBY_LIGHT_SetRoom( WFLBY_LIGHTWK* p_wk, WFLBY_LIGHT_NEON_ROOMTYPE roomtype )
{
	int i;
	
	// ���X�g�ɍ��܂ł̕���ݒ�
	// �J�E���g�ݒ�
	p_wk->last.lightcolor_ful[0]	= p_wk->now.lightcolor_ful[0];
	p_wk->lightcount[0]				= 0;
	WFLBY_LIGHT_ConvetNitroToLightCol( &p_wk->next.lightcolor_ful[0], 
			&sc_WFLBY_LIGHTDATA_ROOM[ roomtype ].lightcolor );

	for( i=0; i<WFLBY_LIGHT_MAT_NUM; i++ ){
		p_wk->last.matcolor_ful[i]		= p_wk->now.matcolor_ful[i];
		p_wk->matcount[i]				= 0;
		WFLBY_LIGHT_ConvetNitroToLightCol( &p_wk->next.matcolor_ful[i], 
				&sc_WFLBY_LIGHTDATA_ROOM[ roomtype ].matcolor[i] );
	}
}

//----------------------------------------------------------------------------
/**
 *	@brief	���l�I���ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	floor		���l�I���ݒ�
 */
//-----------------------------------------------------------------------------
void WFLBY_LIGHT_SetFloor( WFLBY_LIGHTWK* p_wk, WFLBY_LIGHT_NEON_FLOORTYPE floor )
{
	p_wk->last.lightcolor_ful[1]	= p_wk->now.lightcolor_ful[1];
	p_wk->lightcount[1]				= 0;
	WFLBY_LIGHT_ConvetNitroToLightCol( &p_wk->next.lightcolor_ful[1], 
			&sc_WFLBY_LIGHTDATA_FLOOR[ floor ].lightcolor );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���j�������g�l�I���ݒ�
 *
 *	@param	p_wk		���[�N
 *	@param	monu		���j�������g�l�I���ݒ�
 */
//-----------------------------------------------------------------------------
void WFLBY_LIGHT_SetMonu( WFLBY_LIGHTWK* p_wk, WFLBY_LIGHT_NEON_MONUTYPE monu )
{
}






//-----------------------------------------------------------------------------
/**
 *		�v���C�x�[�g�֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�J���[�R���o�[�g
 *
 *	@param	cp_data		�J���[�f�[�^
 *	@param	p_col		NitroRGB
 */
//-----------------------------------------------------------------------------
static void WFLBY_LIGHT_ConvetLightColToNitro( const WFLBY_LIGHT_COL* cp_data, GXRgb* p_col )
{
	WFLBY_DEBUG_LIGHT_PRINT( "red=%d	green=%d	blue=%d\n", cp_data->color[ WFLBY_LIGHT_COL_R ], cp_data->color[ WFLBY_LIGHT_COL_G ], cp_data->color[ WFLBY_LIGHT_COL_B ] );
	
	*p_col = GX_RGB( cp_data->color[ WFLBY_LIGHT_COL_R ],
			cp_data->color[ WFLBY_LIGHT_COL_G ],
			cp_data->color[ WFLBY_LIGHT_COL_B ] );
}
static void WFLBY_LIGHT_ConvetNitroToLightCol( WFLBY_LIGHT_COL* p_data, const GXRgb* cp_col )
{
	p_data->color[ WFLBY_LIGHT_COL_R ] = ((*cp_col) & GX_RGB_R_MASK) >> GX_RGB_R_SHIFT;
	p_data->color[ WFLBY_LIGHT_COL_G ] = ((*cp_col) & GX_RGB_G_MASK) >> GX_RGB_G_SHIFT;
	p_data->color[ WFLBY_LIGHT_COL_B ] = ((*cp_col) & GX_RGB_B_MASK) >> GX_RGB_B_SHIFT;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�J���[Merge	�f�[�^�P����f�[�^�Q�ɂ���
 *
 *	@param	cp_data0		�f�[�^�P
 *	@param	cp_data1		�f�[�^�Q
 *	@param	p_datain		�i�[��
 *	@param	count			�J�E���^
 *	@param	countmax		�J�E���^MAX
 */
//-----------------------------------------------------------------------------
static void WFLBY_LIGHT_Col_Marge( const WFLBY_LIGHT_COL* cp_data0, const WFLBY_LIGHT_COL* cp_data1, WFLBY_LIGHT_COL* p_datain, s32 count, s32 countmax )
{
	s32 r;
	s32 g;
	s32 b;
	
	r = cp_data1->color[ WFLBY_LIGHT_COL_R ] - cp_data0->color[ WFLBY_LIGHT_COL_R ];
	g = cp_data1->color[ WFLBY_LIGHT_COL_G ] - cp_data0->color[ WFLBY_LIGHT_COL_G ];
	b = cp_data1->color[ WFLBY_LIGHT_COL_B ] - cp_data0->color[ WFLBY_LIGHT_COL_B ];
	
	p_datain->color[ WFLBY_LIGHT_COL_R ] = cp_data0->color[ WFLBY_LIGHT_COL_R ] + ( (count * r) / countmax );
	p_datain->color[ WFLBY_LIGHT_COL_G ] = cp_data0->color[ WFLBY_LIGHT_COL_G ] + ( (count * g) / countmax );
	p_datain->color[ WFLBY_LIGHT_COL_B ] = cp_data0->color[ WFLBY_LIGHT_COL_B ] + ( (count * b) / countmax );
}



//----------------------------------------------------------------------------
/**
 *	@brief	���C�g�f�[�^�̐ݒ�
 *
 *	@param	p_data			���[�N
 *	@param	cp_lightdata	���C�g�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_LIGHT_Ful_SetLIGHTDATA_ROOM( WFLBY_LIGHT_FULL* p_data, const WFLBY_LIGHTDATA_ROOM* cp_lightdata )
{
	int i;

	WFLBY_LIGHT_ConvetNitroToLightCol( &p_data->lightcolor_ful[0], &cp_lightdata->lightcolor );

	for( i=0; i<WFLBY_LIGHT_MAT_NUM;  i++ ){
		WFLBY_LIGHT_ConvetNitroToLightCol( &p_data->matcolor_ful[i], &cp_lightdata->matcolor[i] );
	}
}
// �����C�g�p
static void WFLBY_LIGHT_Ful_SetLIGHTDATA_FLOOR( WFLBY_LIGHT_FULL* p_data, const WFLBY_LIGHTDATA_FLOOR* cp_lightdata )
{
	WFLBY_LIGHT_ConvetNitroToLightCol( &p_data->lightcolor_ful[1], &cp_lightdata->lightcolor );
}

//----------------------------------------------------------------------------
/**
 *	@brief	���C�g�f�[�^�ݒ�
 *
 *	@param cp_data0			�f�[�^
 */
//-----------------------------------------------------------------------------
static void WFLBY_LIGHT_Ful_SetLight( const WFLBY_LIGHT_FULL* cp_data0 )
{
	int i;
	GXRgb rgb, rgb2;

	for( i=0; i<WFLBY_LIGHT_LIGHT_NUM;  i++ ){
		WFLBY_DEBUG_LIGHT_PRINT( "light %d color\n", i );
		WFLBY_LIGHT_ConvetLightColToNitro( &cp_data0->lightcolor_ful[i], &rgb  );
		NNS_G3dGlbLightColor( i, rgb );
	}
	WFLBY_DEBUG_LIGHT_PRINT( "diff\n" );
	WFLBY_LIGHT_ConvetLightColToNitro( &cp_data0->matcolor_ful[0], &rgb  );
	WFLBY_DEBUG_LIGHT_PRINT( "amb\n" );
	WFLBY_LIGHT_ConvetLightColToNitro( &cp_data0->matcolor_ful[1], &rgb2  );
	NNS_G3dGlbMaterialColorDiffAmb( rgb, rgb2,  FALSE );
	
	WFLBY_DEBUG_LIGHT_PRINT( "spec\n" );
	WFLBY_LIGHT_ConvetLightColToNitro( &cp_data0->matcolor_ful[2], &rgb  );
	WFLBY_DEBUG_LIGHT_PRINT( "emi\n" );
	WFLBY_LIGHT_ConvetLightColToNitro( &cp_data0->matcolor_ful[3], &rgb2  );
	NNS_G3dGlbMaterialColorSpecEmi( rgb, rgb2,  FALSE );
}


