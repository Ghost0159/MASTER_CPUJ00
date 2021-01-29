//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *	@file		wf2dmap_common_c.h
 *	@brief
 *	@author	
 *	@data		2007.04.13
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __WF2DMAP_COMMON_C_H__
#define __WF2DMAP_COMMON_C_H__

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
// ����
#define 	WF2DMAP_WAY_C_UP	(0) 
#define 	WF2DMAP_WAY_C_DOWN	(1) 
#define 	WF2DMAP_WAY_C_LEFT	(2) 
#define 	WF2DMAP_WAY_C_RIGHT	(3) 
#define 	WF2DMAP_WAY_C_NUM	(4) 


//�R�}���h
#define 	WF2DMAP_CMD_C_NONE			(0) 	// �Ȃɂ��Ȃ�
#define 	WF2DMAP_CMD_C_TURN			(1) 	// �^�[���R�}���h
#define 	WF2DMAP_CMD_C_WALK			(2) 	// �����R�}���h
#define 	WF2DMAP_CMD_C_RUN			(3) 	// ����R�}���h
#define 	WF2DMAP_CMD_C_BUSY			(4) 	// �Z�������
#define 	WF2DMAP_CMD_C_WALLWALK		(5) 	// �Ǖ��������R�}���h
#define 	WF2DMAP_CMD_C_SLOWWALK		(6) 	// �����������R�}���h
#define 	WF2DMAP_CMD_C_WALK4			(7) 	// �������R�}���h
#define 	WF2DMAP_CMD_C_STAYWALK2		(8) 	// ���̏�����R�}���h
#define 	WF2DMAP_CMD_C_STAYWALK4		(9) 	// ���̏�����R�}���h
#define 	WF2DMAP_CMD_C_STAYWALK8		(10) 	// ���̏�����R�}���h
#define 	WF2DMAP_CMD_C_STAYWALK16	(11) 	// ������肻�̏�����R�}���h
#define 	WF2DMAP_CMD_C_NUM			(12) 

#endif		// __WF2DMAP_COMMON_C_H__

