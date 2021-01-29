//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_scroll.h
 *	@brief		�X�N���[���f�[�^�Ǘ�+�X�N���[�����W�Ǘ����W���[��
 *	@author		tomoya takahashi
 *	@data		2007.03.19
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WF2DMAP_SCROLL_H__
#define __WF2DMAP_SCROLL_H__

#include "application/wifi_2dmap/wf2dmap_common.h"
#include "application/wifi_2dmap/wf2dmap_map.h"
#include "application/wifi_2dmap/wf2dmap_obj.h"
#include "application/wifi_2dmap/wf2dmap_scroll.h"

#undef GLOBAL
#ifdef	__WF2DMAP_SCROLL_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
///	�X�N���[�����
//=====================================
typedef struct _WF2DMAP_SCROLL {
	s16 draw_top;		// �\����`����W
	s16 draw_left;		// �\����`�����W
}WF2DMAP_SCROLL;


//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------

// �X�N���[���Ǘ��֐�
GLOBAL void WF2DMAP_SCRContSysMain( WF2DMAP_SCROLL* p_data, const WF2DMAP_OBJWK* cp_objwk );



// �f�[�^�ݒ聕�擾
GLOBAL void WF2DMAP_SCROLLSysDataInit( WF2DMAP_SCROLL* p_sys );
GLOBAL void WF2DMAP_SCROLLSysDataSet( WF2DMAP_SCROLL* p_sys, s16 draw_top, s16 draw_left );
GLOBAL s16 WF2DMAP_SCROLLSysTopGet( const WF2DMAP_SCROLL* cp_sys );
GLOBAL s16 WF2DMAP_SCROLLSysLeftGet( const WF2DMAP_SCROLL* cp_sys );
GLOBAL void WF2DMAP_SCROLLSysTopAdd( WF2DMAP_SCROLL* p_sys, s16 add );
GLOBAL void WF2DMAP_SCROLLSysLeftAdd( WF2DMAP_SCROLL* p_sys, s16 add );


 
#undef	GLOBAL
#endif		// __WF2DMAP_SCROLL_H__

