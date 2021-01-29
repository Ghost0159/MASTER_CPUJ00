//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_scrdraw.c
 *	@brief		�X�N���[���`��ݒ菈��
 *	@author		tomoya takahshi
 *	@data		2007.03.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]

#include "common.h"

#define __WF2DMAP_SCRDRAW_H_GLOBAL
#include "application/wifi_2dmap/wf2dmap_scrdraw.h"

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
//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define WF2DMAP_SCRDRAW_SCRNSIZE_X	(33)	// �X�N���[�����������ރX�N���[���T�C�Y
#define WF2DMAP_SCRDRAW_SCRNSIZE_Y	(25)	// �X�N���[�����������ރX�N���[���T�C�Y

//-------------------------------------
///	�X�N���[���^�C�v
//=====================================
enum {
	WF2DMAP_SCRNTYPE_256x256,
	WF2DMAP_SCRNTYPE_512x256,
	WF2DMAP_SCRNTYPE_256x512,
	WF2DMAP_SCRNTYPE_512x512,
	WF2DMAP_SCRNTYPE_NUM,
} ;


//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�X�N���[���`�惏�[�N
//=====================================
typedef struct _WF2DMAP_SCRDRAW {
	CLACT_U_EASYRENDER_DATA* p_render;
	GF_BGL_INI* p_bgl;
	void* p_scrnbuff;
	NNSG2dScreenData* p_scrn;
	u32 surface_type;
	u32 bg_frame;
	u16 now_scrn_x;	// ���݂̃X�N���[�������o����
	u16 now_scrn_y;	// ���݂̃X�N���[�������o����
}WF2DMAP_SCRDRAW;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJDrawSysBGCNTSet( GF_BGL_INI* p_bgl, int bg_frame, const WF2DMAP_SCRDRAWINIT* cp_data );
static void WF2DMAP_OBJDrawSysBGScrSet( WF2DMAP_SCRDRAW* p_sys, s16 scr_x, s16 scr_y );
static void WF2DMAP_OBJDrawSysBGScrnWrite( GF_BGL_INI* p_bgl, int bg_frame, const NNSG2dScreenData* cp_scrn, s16 scrn_x, s16 scrn_y );

static void WF2DMAP_OBJDrawSysScrnDraw( GF_BGL_INI * ini, u8 frmnum, u8 write_px, u8 write_py, u8 write_sx, u8 write_sy, const void * buf, u8 buf_px, u8 buf_py, u8 buf_sx, u8 buf_sy );
static u8 WF2DMAP_OBJDrawSysScrnTypeGet( u8 scrn_siz_x, u8 scrn_siz_y );
static const void* WF2DMAP_OBJDrawSysScrnBuffPtrGet( const u8* p_buf, u8 scrnblck_x, u8 scrnblck_y, u8 scrntype, u8 scrn_siz_x, u8 scrn_siz_y, WF2DMAP_POS* p_siz );

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���`��V�X�e���쐬
 *
 *	@param	p_render			�����_���[�f�[�^
 *	@param	p_bgl				�f�e�Q�a�f�k�f�[�^
 *	@param	cp_scrn				�X�N���[���f�[�^
 *	@param	cp_init				�������f�[�^
 *	@param	heapID				�q�[�v�h�c
 *
 *	@return	�X�N���[���`��
 *
 */
//-----------------------------------------------------------------------------
WF2DMAP_SCRDRAW* WF2DMAP_SCRDrawSysInit( CLACT_U_EASYRENDER_DATA* p_render, GF_BGL_INI* p_bgl, const WF2DMAP_SCRDRAWINIT* cp_init, u32 heapID )
{
	WF2DMAP_SCRDRAW* p_sys;

	p_sys = sys_AllocMemory( heapID, sizeof(WF2DMAP_SCRDRAW) );
	memset( p_sys, 0, sizeof(WF2DMAP_SCRDRAW) );
	p_sys->p_render = p_render;
	p_sys->p_bgl = p_bgl;
	p_sys->surface_type = cp_init->surface_type;
	p_sys->bg_frame = cp_init->bg_frame;

	// BGCONT�ݒ�
	WF2DMAP_OBJDrawSysBGCNTSet( p_sys->p_bgl, p_sys->bg_frame, cp_init );

	// �X�N���[���O���t�B�b�N�ǂݍ���
	p_sys->p_scrnbuff = ArcUtil_ScrnDataGet( cp_init->arcid_scrn, cp_init->dataid_scrn,
			cp_init->arcdata_comp, &p_sys->p_scrn, heapID );

	return p_sys;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���`��V�X�e���j��
 *
 *	@param	p_sys	�X�N���[���`��V�X�e��
 */
//-----------------------------------------------------------------------------
void WF2DMAP_SCRDrawSysExit( WF2DMAP_SCRDRAW* p_sys )
{
	sys_FreeMemoryEz( p_sys->p_scrnbuff );
	sys_FreeMemoryEz( p_sys );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[�����C��
 *
 *	@param	p_sys		�X�N���[���`��V�X�e��
 *	@param	cp_scroll	�X�N���[���f�[�^
 */
//-----------------------------------------------------------------------------
void WF2DMAP_SCRDrawSysMain( WF2DMAP_SCRDRAW* p_sys, const WF2DMAP_SCROLL* cp_scroll )
{
	s16 scr_x;
	s16 scr_y;
	
	// �X�N���[�����W����X�N���[�����W���e�\���V�X�e���ɐݒ�
	scr_y = WF2DMAP_SCROLLSysTopGet( cp_scroll );	
	scr_x = WF2DMAP_SCROLLSysLeftGet( cp_scroll );	

	// �T�[�t�F�[�X
	if( p_sys->surface_type == CLACT_U_EASYRENDER_SURFACE_MAIN ){
		CLACT_U_SetMainSurfaceMatrix( p_sys->p_render, FX32_CONST(scr_x), FX32_CONST(scr_y) );
	}else{
		CLACT_U_SetSubSurfaceMatrix( p_sys->p_render, FX32_CONST(scr_x), FX32_CONST(scr_y)+SUB_SURFACE_Y );
	}

	// BGL
	// �X�N���[�����W����X�N���[���f�[�^����������œ]��
	WF2DMAP_OBJDrawSysBGScrSet( p_sys,  scr_x, scr_y );
}


//-----------------------------------------------------------------------------
/**
 *			�v���C�x�[�g
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[������a�f�ʂ̂a�f�R���g���[���ݒ�
 *
 *	@param	p_bgl		�a�f�k
 *	@param	bg_frame	�a�f�t���[��
 *	@param	cp_data		�������f�[�^
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJDrawSysBGCNTSet( GF_BGL_INI* p_bgl, int bg_frame, const WF2DMAP_SCRDRAWINIT* cp_data )
{
	GF_BGL_BGCNT_HEADER TextBgCntDat = {
		0, 0, 0x1000, 0, GF_BGL_SCRSIZ_512x256,
	};

	TextBgCntDat.colorMode = cp_data->colorMode;
	TextBgCntDat.screenBase = cp_data->screenBase;
	TextBgCntDat.charBase = cp_data->charbase;
	TextBgCntDat.bgExtPltt = cp_data->bgExtPltt;
	TextBgCntDat.priority = cp_data->priority;
	TextBgCntDat.areaOver = 0;
	TextBgCntDat.mosaic = cp_data->mosaic;

	GF_BGL_BGControlExit( p_bgl, bg_frame );
	GF_BGL_BGControlSet( p_bgl, bg_frame, &TextBgCntDat, GF_BGL_MODE_TEXT );
//	GF_BGL_ScrClear( p_bgl, bg_frame );
}	

//----------------------------------------------------------------------------
/**
 *	@brief	BG�ʂ̃X�N���[���ݒ�
 *
 *	@param	p_sys		�V�X�e��
 *	@param	scr_x		�X�N���[���w���W
 *	@param	scr_y		�X�N���[���x���W
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJDrawSysBGScrSet( WF2DMAP_SCRDRAW* p_sys, s16 scr_x, s16 scr_y )
{
	s16 scrn_x;	// �X�N���[���ʒu
	s16 scrn_y;	
	s16 ofs_x;	// �X�N���[�����W
	s16 ofs_y;

	// �X�N���[���ʒu�ƃX�N���[�����W�����߂�
	scrn_x = scr_x / 8;
	scrn_y = scr_y / 8;
	ofs_x = scr_x % 8;	
	ofs_y = scr_y % 8;

	// �X�N���[���ʒu���ς��Ȃ��Ƃ��̓X�N���[���̏����������s��Ȃ�
	if( (scrn_x != p_sys->now_scrn_x) || (scrn_y != p_sys->now_scrn_y) ){
		p_sys->now_scrn_x = scrn_x;
		p_sys->now_scrn_y = scrn_y;
		WF2DMAP_OBJDrawSysBGScrnWrite( p_sys->p_bgl, p_sys->bg_frame, p_sys->p_scrn, -scrn_x, -scrn_y );
		GF_BGL_LoadScreenV_Req( p_sys->p_bgl, p_sys->bg_frame );
	}

	// �X�N���[�����W��ݒ�
	GF_BGL_ScrollReq( p_sys->p_bgl, p_sys->bg_frame, GF_BGL_SCROLL_X_SET, ofs_x );
	GF_BGL_ScrollReq( p_sys->p_bgl, p_sys->bg_frame, GF_BGL_SCROLL_Y_SET, ofs_y );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^��������
 *
 *	@param	p_bgl		�a�f�f�[�^
 *	@param	bg_frame	�a�f�t���[��
 *	@param	cp_scrn		�X�N���[���f�[�^
 *	@param	scrn_x		�X�N���[���f�[�^�̏����o���ʒu
 *	@param	scrn_y		�X�N���[���f�[�^�̏����o���ʒu
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJDrawSysBGScrnWrite( GF_BGL_INI* p_bgl, int bg_frame, const NNSG2dScreenData* cp_scrn, s16 scrn_x, s16 scrn_y )
{
	s16 write_siz_x, write_siz_y;
	s16 read_x, read_y;
	s16 write_x, write_y;
	s16 scrndata_sizx, scrndata_sizy;

	// �X�N���[���f�[�^�T�C�Y�擾
	scrndata_sizx = (cp_scrn->screenWidth/8);
	scrndata_sizy = (cp_scrn->screenHeight/8);
	

	// �ǂݍ��ݐ�ݒ�
	if( scrn_x < 0 ){
		read_x = -scrn_x;	
	}else{
		read_x = 0;
	}
	if( scrn_y < 0 ){
		read_y = -scrn_y;	
	}else{
		read_y = 0;
	}

	// �������ݐ�ݒ�
	if( scrn_x > 0 ){
		write_x = scrn_x;
	}else{
		write_x = 0;
	}
	if( scrn_y > 0 ){
		write_y = scrn_y;
	}else{
		write_y = 0;
	}

	// �X�N���[���f�[�^�ɏ������ނقǂ̃T�C�Y������̂��`�F�b�N
	write_siz_x = WF2DMAP_SCRDRAW_SCRNSIZE_X - write_x;
	write_siz_y = WF2DMAP_SCRDRAW_SCRNSIZE_Y - write_y;
	if( scrndata_sizx < (read_x+write_siz_x) ){
		write_siz_x -= (read_x+write_siz_x) - scrndata_sizx;
	}
	if( scrndata_sizy < (read_y+write_siz_y) ){
		write_siz_y -= (read_y+write_siz_y) - scrndata_sizy;
	}

	// �X�N���[���N���[��
	GF_BGL_ScrFill( p_bgl, bg_frame, 0, 
			0, 0, 
			WF2DMAP_SCRDRAW_SCRNSIZE_X, WF2DMAP_SCRDRAW_SCRNSIZE_Y, GF_BGL_SCRWRT_PALIN );
	
	// ���ĂɂQ�X�N���[���ɕ����Ȃ��Ă͂����Ȃ����`�F�b�N
	WF2DMAP_OBJDrawSysScrnDraw(
			p_bgl, bg_frame,
			write_x, write_y,				//  �������݊J�n�ʒu
			write_siz_x,					// �������݃T�C�Y 
			write_siz_y,					// �������݃T�C�Y
			cp_scrn->rawData,				// �X�N���[���f�[�^
			read_x, read_y,					// �������݊J�n�ʒu
			scrndata_sizx, scrndata_sizy	// �X�N���[���T�C�Y
			);
}


//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���f�[�^�`��
 *
 *	@param	p_bgl			BGL
 *	@param	bg_frame		�t���[��
 *	@param	write_x			�������݊J�n�@���L����
 *	@param	write_y			�������݊J�n�@���L����
 *	@param	write_siz_x		�������݃T�C�Y
 *	@param	write_siz_y		�������݃T�C�Y
 *	@param	buf				�X�N���[���f�[�^
 *	@param	read_x			�ǂݍ��݊J�n�@���L����
 *	@param	read_y			�ǂݍ��݊J�n�@���L����
 *	@param	buf_sx			�X�N���[���f�[�^�@���L�����T�C�Y
 *	@param	buf_sy			�X�N���[���f�[�^�@���L�����T�C�Y
 */
//-----------------------------------------------------------------------------
static void WF2DMAP_OBJDrawSysScrnDraw( GF_BGL_INI * p_bgl, u8 bg_frame, u8 write_x, u8 write_y, u8 write_siz_x, u8 write_siz_y, const void * buf, u8 read_x, u8 read_y, u8 buf_sx, u8 buf_sy )
{
	s8 ws_x, ws_y;	// �������񂾃T�C�Y
	s8 nws_x, nws_y;// ���̏������ރT�C�Y
	const u16* write_buf;
	WF2DMAP_POS siz;	// �o�b�t�@�T�C�Y
	s8 scrnblck_x, scrnblck_y;
	u8 scrn_type;
	s8 reedw_x, writew_x;	// �ǂݍ��ݏ������ݐ�

	scrn_type = WF2DMAP_OBJDrawSysScrnTypeGet( buf_sx, buf_sy );

//	OS_Printf( "BUFF_size x[%d] y[%d] scrntype[%d]\n", buf_sx, buf_sy, scrn_type );
	
	// �o�b�t�@���R�Q���R�Q���ɓ����Ă���Ȃ炻�̂܂܏�������
	if( scrn_type == WF2DMAP_SCRNTYPE_256x256 ){
		GF_BGL_ScrWriteExpand(
				p_bgl, bg_frame,
				write_x, write_y,				// �������݊J�n�ʒu
				write_siz_x,					// �������݃T�C�Y 
				write_siz_y,					// �������݃T�C�Y
				buf,							// �X�N���[���f�[�^
				read_x, read_y,					// �������݊J�n�ʒu
				buf_sx, buf_sy					// �X�N���[���T�C�Y
			);
		return ;
	}

	// �c�����������݃��[�v
	ws_y = write_siz_y;
	while( ws_y > 0 ){
		
		if( (((read_y)%32) + ws_y) <= 32 ){	// Y�����������݂��R�Q�ȏ�ɂȂ邩�`�F�b�N
			// �ȉ��Ȃ�P��ł�����
			nws_y = ws_y;
			ws_y = 0;
		}else{
			// �傫���Ȃ�A�������邾������
			nws_y = 32 - (read_y%32);
			ws_y -= nws_y;
		}

		// �X�N���[���u���b�N�擾
		scrnblck_y = read_y / 32;

		ws_x = write_siz_x;
		reedw_x = read_x;
		writew_x = write_x;
		// X�����������݃��[�v
		while( ws_x > 0 ){
			if( (((reedw_x)%32) + ws_x) <= 32 ){	// Y�����������݂��R�Q�ȏ�ɂȂ邩�`�F�b�N
				// �ȉ��Ȃ�P��ł�����
				nws_x = ws_x;
				ws_x = 0;
			}else{
				// �傫���Ȃ�A�������邾������
				nws_x = 32 - (reedw_x%32);
				ws_x -= nws_x;
			}

			// �X�N���[���u���b�N�擾
			scrnblck_x = reedw_x / 32;

			// ��������
			write_buf = WF2DMAP_OBJDrawSysScrnBuffPtrGet( buf, scrnblck_x, scrnblck_y, scrn_type,  buf_sx, buf_sy, &siz );

			/*
			OS_Printf( "writew_x[%d] write_y[%d]\n", writew_x, write_y );
			OS_Printf( "nws_x[%d] nws_y[%d]\n", nws_x, nws_y );
			OS_Printf( "reedw_x[%d] read_y[%d]\n", reedw_x%32, read_y%32 );
			OS_Printf( "siz_x[%d] siz_y[%d]\n", siz.x, siz.y );
			//*/

			GF_BGL_ScrWriteExpand(
					p_bgl, bg_frame,
					writew_x, write_y,				//  �������݊J�n�ʒu
					nws_x,							// �������݃T�C�Y 
					nws_y,							// �������݃T�C�Y
					write_buf,						// �X�N���[���f�[�^
					reedw_x%32, read_y%32,			// �������݊J�n�ʒu
					siz.x, siz.y					// �X�N���[���T�C�Y
				);

			// �ǂݍ��ݐ�𓮂���
			reedw_x += nws_x;

			// �������ݐ�𓮂���
			writew_x += nws_x;
		}

		// �ǂݍ��ݐ�𓮂���
		read_y += nws_y;

		// �������ݐ�𓮂���
		write_y += nws_y;
	}

	OS_Printf( "\n" );
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���^�C�v�擾
 *
 *	@param	scrn_siz_x		�X�N���[���T�C�Y��
 *	@param	scrn_siz_y		�X�N���[���T�C�Y��
 *
 *	@return	�X�N���[���^�C�v
 */
//-----------------------------------------------------------------------------
static u8 WF2DMAP_OBJDrawSysScrnTypeGet( u8 scrn_siz_x, u8 scrn_siz_y )
{
	GF_ASSERT( scrn_siz_x <= 64 );
	GF_ASSERT( scrn_siz_y <= 64 );
	
	if( scrn_siz_x <= 32 ){
		if( scrn_siz_y <= 32 ){
			return WF2DMAP_SCRNTYPE_256x256;
		}else{
			return WF2DMAP_SCRNTYPE_256x512;
		}
	}else{
		if( scrn_siz_y <= 32 ){
			return WF2DMAP_SCRNTYPE_512x256;
		}else{
			return WF2DMAP_SCRNTYPE_512x512;
		}
	}
	GF_ASSERT(0);
	return 0;
}

//----------------------------------------------------------------------------
/**
 *	@brief	�X�N���[���u���b�N�i���o�[����X�N���[���̐擪�|�C���^���擾
 *
 *	@param	p_buf			�o�b�t�@
 *	@param	scrnblck_x		�X�N���[���u���b�N�i���o�[��
 *	@param	scrnblck_y		�X�N���[���u���b�N�i���o�[��
 *	@param	scrntype		�X�N���[���^�C�v
 *
 *	@return	���̃u���b�N�̃o�b�t�@�̐擪�o�b�t�@
 */
//-----------------------------------------------------------------------------
static const void* WF2DMAP_OBJDrawSysScrnBuffPtrGet( const u8* p_buf, u8 scrnblck_x, u8 scrnblck_y, u8 scrntype, u8 scrn_siz_x, u8 scrn_siz_y, WF2DMAP_POS* p_siz )
{
	s16 one_blck_siz;
	u16 idx;

	// ���̂Ƃ���A�����ȃX�N���[���T�C�Y�ɑΉ����Ă��܂��B
	// ��������Ȃǂ������Ȃ��Ă��܂����߁A�������x�I�ɖ�肪����΁A
	// ��΂�256�̔{���̃X�N���[���T�C�Y�ɂ���Ƃ������@�ō�����������K�v������Ǝv���܂�
	
	switch( scrntype ){
	case WF2DMAP_SCRNTYPE_256x256:
		p_siz->x = scrn_siz_x;
		p_siz->y = scrn_siz_y;
		return p_buf;
		
	case WF2DMAP_SCRNTYPE_512x256:
		// �X�N���[���������݃T�C�Y
		if( ((scrnblck_x+1)*32) <= scrn_siz_x ){
			p_siz->x = 32;
		}else{
			p_siz->x = scrn_siz_x - (scrnblck_x*32);
		}
		p_siz->y = scrn_siz_y;

		// 1blck�̃T�C�Y
		one_blck_siz = scrn_siz_y * (2*32);
		return &p_buf[ (scrnblck_x * one_blck_siz) ];
		
	case WF2DMAP_SCRNTYPE_256x512:
		// �X�N���[���������݃T�C�Y
		p_siz->x = scrn_siz_x;
		if( ((scrnblck_y+1)*32) <= scrn_siz_y ){
			p_siz->y = 32;
		}else{
			p_siz->y = scrn_siz_y - (scrnblck_y*32);
		}

		// 1blck�̃T�C�Y
		one_blck_siz = (scrn_siz_x*2) * 32;
		return &p_buf[ (scrnblck_y * one_blck_siz) ];
		
	case WF2DMAP_SCRNTYPE_512x512:
		// �X�N���[���������݃T�C�Y
		if( ((scrnblck_x+1)*32) <= scrn_siz_x ){
			p_siz->x = 32;
		}else{
			p_siz->x = scrn_siz_x - (scrnblck_x*32);
		}
		if( ((scrnblck_y+1)*32) <= scrn_siz_y ){
			p_siz->y = 32;
		}else{
			p_siz->y = scrn_siz_y - (scrnblck_y*32);
		}

		// �o�b�t�@�̎Q�Ɛ��ݒ�
		// 0 1
		// 2 3
		// �ƃX�N���[���͕���ł���
		if( scrnblck_y == 0 ){
			idx = 0;

			if( scrnblck_x > 0 ){
				idx += (32*2) * 32;
			}
		}else{
			idx = (scrn_siz_x*2)*32;

			if( scrnblck_x > 0 ){
				idx += (32*2) *scrn_siz_y;
			}
		}

		return &p_buf[ idx ];

	default:
		GF_ASSERT(0);
		break;
	}

	GF_ASSERT(0);
	return NULL;
}

