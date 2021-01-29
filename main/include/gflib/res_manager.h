//[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[
/**
 *
 *@file		res_manager.h
 *@brief	���\�[�XID�Ǘ��}�l�[�W���[
 *@author	tomoya takahashi
 *@data		2005.08.29
 *
 */
//]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]
#ifndef __RES_MANAGER_H__
#define __RES_MANAGER_H__

#undef GLOBAL
#ifdef	__RES_MANAGER_H_GLOBAL
#define	GLOBAL	/* */
#else
#define	GLOBAL	extern
#endif

//-----------------------------------------------------------------------------
/**
 *					�萔�錾
*/
//-----------------------------------------------------------------------------
#define RES_MANAGER_ID_NONE	(0xffffffff)	// ID�Ȃ�

//-----------------------------------------------------------------------------
/**
 *					�\���̐錾
*/
//-----------------------------------------------------------------------------
//-------------------------------------
//	
//	���\�[�X�w�b�_�[�|�C���^
//
//	�o�^�f�[�^���܂Ƃ߂��f�[�^�ł�
//	
//=====================================
typedef struct _RES_HEADER* RES_HEADER_PTR;

//---------------------------
//
//	���\�[�X�Ǘ��}�l�[�W���[
//	�Ǘ��f�[�^�|�C���^
//
//===========================
typedef struct _RES_MANAGER* RES_MANAGER_PTR;

//---------------------------
//
//	���\�[�X�Ǘ��f�[�^�|�C���^
//	�ǂݍ��񂾂P�̃��\�[�X�f�[�^��
//	�Ǘ����Ă���|�C���^
//
//===========================
typedef struct _RES_OBJ* RES_OBJ_PTR;

//-----------------------------------------------------------------------------
/**
 *					�v���g�^�C�v�錾
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�Ǘ��}�l�[�W���[���ԕ��쐬
 *
 *@param	num		�f�[�^�Ǘ��e�[�u����
 *
 *@return	RES_MANAGER_PTR		�Ǘ��f�[�^�|�C���^
 *
 * ���Ǘ��e�[�u���̈�̊m��
 * ���Ǘ��e�[�u���̈�̏�����
 *
 */
//-----------------------------------------------------------------------------
GLOBAL RES_MANAGER_PTR RESM_Init(int num, int heap);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�Ǘ��f�[�^�j��
 *
 *@param	resm	���\�[�X�Ǘ��f�[�^
 *
 *@return	none
 *
 *	���ǂݍ��񂾃��\�[�X��j��
 *	���Ǘ��e�[�u���̈��j��
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_Delete(RES_MANAGER_PTR resm);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�c��o�^�\�����擾
 *
 *@param	resm	�擾���郊�\�[�X�Ǘ��f�[�^
 *
 *@return	int		�c��o�^�\��
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL int RESM_GetRest(RES_MANAGER_PTR resm);

//----------------------------------------------------------------------------
/**
 *
 *@brief	ID���d�����Ȃ������擾
 *
 *@param	resm	�擾���郊�\�[�X�Ǘ��f�[�^
 *@param	id		�`�F�b�N����ID
 *
 *@retval	TRUE	�d�����Ȃ�
 *@retval	FALSE	�d������
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL BOOL RESM_CheckID(RES_MANAGER_PTR resm, int id);

//----------------------------------------------------------------------------
/**
 *	
 *@brief	���\�[�X�̓o�^
 *
 *@param	resm	�o�^���郊�\�[�X�Ǘ��f�[�^�|�C���^
 *@param	res		���\�[�X
 *@param	id		�o�^ID	�i���d�����Ӂj
 *
 *@retval	RES_OBJ_PTR	�o�^����
 *@retval	NULL		�o�^���s	�o�^�e�[�u�������t
 *
 * ���p�X�̃��\�[�X��ǂݍ���ŁA�Ǘ��f�[�^�ɐݒ�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL RES_OBJ_PTR RESM_AddResNormal(RES_MANAGER_PTR resm, void* res, int id);

//----------------------------------------------------------------------------
/**
 *	
 *@brief	���\�[�X�̓o�^
 *
 *@param	resm	�o�^���郊�\�[�X�Ǘ��f�[�^�|�C���^
 *@param	p_path	���\�[�X�̃p�X
 *@param	id		�o�^ID	�i���d�����Ӂj
 *@param	heap	���\�[�X�ǂݍ��݂Ɏg�p����q�[�v
 CONST_*
 *@retval	RES_OBJ_PTR	�o�^����
 *@retval	NULL		�o�^���s	�o�^�e�[�u�������tor�p�X���������Ȃ�
 *
 * ���p�X�̃��\�[�X��ǂݍ���ŁA�Ǘ��f�[�^�ɐݒ�
 * 
 */
//-----------------------------------------------------------------------------
GLOBAL RES_OBJ_PTR RESM_AddRes(RES_MANAGER_PTR resm, const char* p_path, int id, int heap);

//----------------------------------------------------------------------------
/**
 *	
 *@brief	�w�b�_�[���g�p�������\�[�X�̓o�^�i�P���j
 *
 *@param	resm	�o�^���郊�\�[�X�Ǘ��f�[�^�|�C���^
 *@param	header	���\�[�X�w�b�_�[�|�C���^
 *@param	no		�o�^����i���o�[
 *@param	heap	���\�[�X�ǂݍ��݂Ɏg�p����q�[�v
 *
 *@retval	RES_OBJ_PTR	�o�^����
 *
 * ���p�X�̃��\�[�X��ǂݍ���ŁA�Ǘ��f�[�^�ɐݒ�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL RES_OBJ_PTR RESM_AddResHd(RES_MANAGER_PTR resm, const RES_HEADER_PTR header, int no, int heap);

//----------------------------------------------------------------------------
/**
 *	
 *@brief	�w�b�_�[���g�p�������\�[�X�̓o�^�i�����j
 *
 *@param	resm	�o�^���郊�\�[�X�Ǘ��f�[�^�|�C���^
 *@param	header	���\�[�X�w�b�_�[�|�C���^
 *@param	heap	���\�[�X�ǂݍ��݂Ɏg�p����q�[�v
 *
 *@return	none
 *
 * ���p�X�̃��\�[�X��ǂݍ���ŁA�Ǘ��f�[�^�ɐݒ�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_AddResHds(RES_MANAGER_PTR resm, const RES_HEADER_PTR header, int heap);

//----------------------------------------------------------------------------
/**
 *	
 *@brief	�w�b�_�[���g�p�������\�[�X�̓o�^�i�����j
 *			�w�b�_�[�̊J�n�v�f�A�I���v�f���w��
 *
 *@param	resm	�o�^���郊�\�[�X�Ǘ��f�[�^�|�C���^
 *@param	header	���\�[�X�w�b�_�[�|�C���^
 *@param	start	�J�n�v�f��
 *@param	num		�o�^�v�f��
 *@param	heap	���\�[�X�ǂݍ��݂Ɏg�p����q�[�v
 *
 *@return	noen
 *
 * ���p�X�̃��\�[�X��ǂݍ���ŁA�Ǘ��f�[�^�ɐݒ�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_AddResHdsEx(RES_MANAGER_PTR resm, const RES_HEADER_PTR header, int start, int num, int heap);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�̔j��
 *
 *@param	resm	�j�����郊�\�[�X���o�^����Ă���Ǘ��f�[�^�|�C���^
 *@param	r_obj	�j�����郊�\�[�X�I�u�W�F
 *
 * ���ǂݍ��񂾃��\�[�X��j�����A�Ǘ��f�[�^��������
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_DeleteRes(RES_MANAGER_PTR resm, RES_OBJ_PTR r_obj);

//----------------------------------------------------------------------------
/**
 *
 *@brief	ID�Ń��\�[�X�̔j��
 *
 *@param	resm	�j�����郊�\�[�X���o�^����Ă���Ǘ��f�[�^�|�C���^
 *@param	id		�j�����郊�\�[�X��ID	(�Ǘ�ID)
 *
 * ���ǂݍ��񂾃��\�[�X��j�����A�Ǘ��f�[�^��������
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_DeleteResID(RES_MANAGER_PTR resm, int id);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�S���\�[�X�̔j��
 *
 *@param	resm	�j�����郊�\�[�X���o�^����Ă���Ǘ��f�[�^�|�C���^
 *
 * ���ǂݍ��񂾃��\�[�X��j�����A�Ǘ��f�[�^��������
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_DeleteAllRes(RES_MANAGER_PTR resm);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�I�u�W�F�N�g�f�[�^�|�C���^���擾
 *
 *@param	resm		���\�[�X�Ǘ��f�[�^
 *@param	id			���\�[�X��ID	(�Ǘ�ID)
 *
 *@retval	NULL		�o�^����Ă��Ȃ�
 *@retval	RES_OBJ_PTR	�o�^����Ă��郊�\�[�X�I�u�W�F�|�C���^
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL RES_OBJ_PTR RESM_GetResObj(RES_MANAGER_PTR resm, int id);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�o�^����Ă��郊�\�[�X�f�[�^���擾
 *
 *@param	r_obj		���\�[�X�I�u�W�F
 *
 *@retval	NULL		�o�^����Ă��Ȃ�
 *@retval	NULL�ȊO	�o�^����Ă��郊�\�[�X�|�C���^
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void* RESM_GetRes(RES_OBJ_PTR r_obj);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�o�^����Ă��郊�\�[�X�f�[�^��ύX
 *
 *@param	r_obj		���\�[�X�I�u�W�F
 *@param	res			�ݒ肷�郊�\�[�X
 *
 *@param	none
 *
 *	���܂ŕێ�����Ă����f�[�^�͔j������܂��B�i����������j
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_SetRes(RES_OBJ_PTR r_obj, void* res);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�o�^����Ă���ID���擾
 *
 *@param	r_obj		���\�[�X�I�u�W�F
 *
 *@retval	int			�Ǘ�ID
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL int RESM_GetID(RES_OBJ_PTR r_obj);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�o�^����Ă���ID��ݒ�
 *
 *@param	resm		���\�[�X�}�l�[�W��
 *@param	r_obj		���\�[�X�I�u�W�F
 *@param	id			�ݒ肷��ID
 *
 *@retval	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_SetID(RES_MANAGER_PTR resm, RES_OBJ_PTR r_obj, int id);

//-----------------------------------------------------------------------------
//
//		*********** �w�b�_�[�p�֐� **************
//
//=============================================================================
//----------------------------------------------------------------------------
/**
 *
 *	�w�b�_�[�t�@�C���`���̐���
 *	����
 *		���\�[�X�t�@�C���p�X,ID(���^�[��)
 *		���\�[�X�t�@�C���p�X,ID(���^�[��)
 *		���\�[�X�t�@�C���p�X,ID(���^�[��)
 *		0,0		// �I�[�R�[�h
 *
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *
 *@brief	�w�b�_�[�T�C�Y���擾
 *
 *@param	none	
 *
 *@return	int		�w�b�_�[�̃o�C�g�T�C�Y
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL int RESM_GetHeaderSize(void);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�w�b�_�[�z�񂩂�v�f���̃f�[�^��Ԃ�
 *
 *@param	tbl		�z��ɂȂ��Ă���w�b�_�[
 *@param	no		�v�f��
 *
 *@return	RES_HEADER_PTR data[no] �̃A�h���X
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL RES_HEADER_PTR RESM_GetTbl(RES_HEADER_PTR tbl, int no);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�w�b�_�[�̓ǂݍ���
 *
 *@param	header			�w�b�_�[�f�[�^�i�[��
 *@param	rom_path		�w�b�_�[�t�@�C���p�X
 *@param	heap			�q�[�vID
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_HeaderLoad(RES_HEADER_PTR header, const char* rom_path, int heap);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�w�b�_�[�̔j��
 *
 *@param	header			�w�b�_�[�f�[�^�f�[�^
 *
 *@return	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void RESM_HeaderFree(RES_HEADER_PTR header);











//-----------------------------------------------------------------------------
/**
 *
 * 
*	�e�N�X�`�����\�[�X����}�l�[�W��
*	�i.nsbtx��p�j
*
*	��̃��\�[�X�}�l�[�W���@�\�ɉ���
*		Vram�]���@VramKey�擾���Ǘ�
*	�̋@�\��ǉ������}�l�[�W���ł��B
*
* �g����
*
*	TEXRESM_Init	�}�l�[�W�����쐬
*	
*		TEXRESM_AddRes�`	���\�[�X��o�^
*			
*			// �����͂����Ȃ悤��
*			TEXRESM_GetRes		�ǂݍ��񂾂܂܂̃��\�[�X���擾
*			TEXRESM_AllocVramKey VramKey�m��
*			TEXRESM_TexLoad		�e�N�X�`���ǂݍ���
*			TEXRESM_CutTex		�e�N�X�`�����f�[�^�j��
*			TEXRESM_GetTexKey	�e�N�X�`���L�[�擾
*			TEXRESM_GetPlttKey	�p���b�g�L�[�擾
*			// 
*
*		TEXRESM_DeleteRes�`	���\�[�X��j��
*
*	TEXRESM_Delete�Ń}�l�[�W��	�j��
* 
*
*/
//-----------------------------------------------------------------------------
//---------------------------
//
//	�e�N�X�`�����\�[�X�Ǘ��}�l�[�W���[
//	�Ǘ��f�[�^�|�C���^
//
//===========================
typedef struct _TEXRES_MANAGER* TEXRES_MANAGER_PTR;
typedef const struct _TEXRES_MANAGER* CONST_TEXRES_MANAGER_PTR;

//---------------------------
//
//	�e�N�X�`�����\�[�X�Ǘ��f�[�^�|�C���^
//	�ǂݍ��񂾂P�̃��\�[�X�f�[�^
//	VramKey
//	�Ǘ����Ă���|�C���^
//
//===========================
typedef struct _TEXRES_OBJ* TEXRES_OBJ_PTR;
typedef const struct _TEXRES_OBJ* CONST_TEXRES_OBJ_PTR;

//-------------------------------------
//	�e�N�X�`���J�b�g�@�L��
//=====================================
enum{
	TEXRESM_TEX_CUT_FALSE,
	TEXRESM_TEX_CUT_TRUE,
};


//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�Ǘ��}�l�[�W���[���ԕ��쐬
 *
 *@param	num		�f�[�^�Ǘ��e�[�u����
 *
 *@return	TEXRES_MANAGER_PTR		�Ǘ��f�[�^�|�C���^
 *
 * ���Ǘ��e�[�u���̈�̊m��
 * ���Ǘ��e�[�u���̈�̏�����
 *
 */
//-----------------------------------------------------------------------------
GLOBAL TEXRES_MANAGER_PTR TEXRESM_Init(int num, int heap);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�Ǘ��f�[�^�j��
 *
 *@param	resm	���\�[�X�Ǘ��f�[�^
 *
 *@return	none
 *
 *	���ǂݍ��񂾃��\�[�X��j��
 *	���Ǘ��e�[�u���̈��j��
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_Delete(TEXRES_MANAGER_PTR resm);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�c��o�^�\�����擾
 *
 *@param	resm	�擾���郊�\�[�X�Ǘ��f�[�^
 *
 *@return	int		�c��o�^�\��
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL int TEXRESM_GetRest(CONST_TEXRES_MANAGER_PTR resm);

//----------------------------------------------------------------------------
/**
 *
 *@brief	ID���d�����Ȃ������擾
 *
 *@param	resm	�擾���郊�\�[�X�Ǘ��f�[�^
 *@param	id		�`�F�b�N����ID
 *
 *@retval	TRUE	�d�����Ȃ�
 *@retval	FALSE	�d������
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL BOOL TEXRESM_CheckID(CONST_TEXRES_MANAGER_PTR resm, int id);

//-----------------------------------------------------------------------------
/**
*		TEXRESM_AddRes�`�֐��S
*
*		�o�^�֐��͕�����ނ���܂��̂ŁA���p���@�ɂ���Ďg�������Ă�������
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	
 *@brief	���\�[�X�̓o�^	�ǂݍ��ݍς݃��\�[�X�̓o�^
 *
 *@param	resm		�o�^���郊�\�[�X�Ǘ��f�[�^�|�C���^
 *@param	res			���\�[�X
 *@param	id			�o�^ID	�i���d�����Ӂj
 *@param	texcut_flg	�e�N�X�`���J�b�g�@�L���@�t���O
 *@param	heap		�e�N�X�`���J�b�g���[�N�@�q�[�v
 *
 *@retval	TEXRES_OBJ_PTR	�o�^����
 *@retval	NULL		�o�^���s	�o�^�e�[�u�������t
 *
 * ���p�X�̃��\�[�X��ǂݍ���ŁA�Ǘ��f�[�^�ɐݒ�
 * ��TEXRESM_Delete�`�֐����g�p����Ə���Ɉ����̃��\�[�X�͔j������܂��B
 *
 */
//-----------------------------------------------------------------------------
GLOBAL TEXRES_OBJ_PTR TEXRESM_AddResNormal(CONST_TEXRES_MANAGER_PTR resm, void* res, int id, u32 texcut_flg, u32 heap);

//----------------------------------------------------------------------------
/**
 *	
 *@brief	���\�[�X�̓o�^��VramKey�̎擾���s��
 *
 *@param	resm	�o�^���郊�\�[�X�Ǘ��f�[�^�|�C���^
 *@param	res		���\�[�X
 *@param	id		�o�^ID	�i���d�����Ӂj
 *@param	texcut_flg	�e�N�X�`���J�b�g�@�L���@�t���O
 *@param	heap	���\�[�X�ǂݍ��݂Ɏg�p����q�[�v
 *
 *@retval	TEXRES_OBJ_PTR	�o�^����
 *@retval	NULL		�o�^���s	�o�^�e�[�u�������t
 *
 * �������̗���
 *	�P�F�p�X�̃��\�[�X��ǂݍ���
 *	�Q�FVramKey���擾
 */
//-----------------------------------------------------------------------------
GLOBAL TEXRES_OBJ_PTR TEXRESM_AddAndAllocVramKeyResNormal(TEXRES_MANAGER_PTR resm, void* res, int id, u32 texcut_flg, int heap);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�̔j��
 *
 *@param	resm	�j�����郊�\�[�X���o�^����Ă���Ǘ��f�[�^�|�C���^
 *@param	r_obj	�j�����郊�\�[�X�I�u�W�F
 *
 * ���ǂݍ��񂾃��\�[�X��j�����A�Ǘ��f�[�^��������
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_DeleteRes(TEXRES_MANAGER_PTR resm, TEXRES_OBJ_PTR r_obj);

//----------------------------------------------------------------------------
/**
 *
 *@brief	ID�Ń��\�[�X�̔j��
 *
 *@param	resm	�j�����郊�\�[�X���o�^����Ă���Ǘ��f�[�^�|�C���^
 *@param	id		�j�����郊�\�[�X��ID	(�Ǘ�ID)
 *
 * ���ǂݍ��񂾃��\�[�X��j�����A�Ǘ��f�[�^��������
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_DeleteResID(TEXRES_MANAGER_PTR resm, int id);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�S���\�[�X�̔j��
 *
 *@param	resm	�j�����郊�\�[�X���o�^����Ă���Ǘ��f�[�^�|�C���^
 *
 * ���ǂݍ��񂾃��\�[�X��j�����A�Ǘ��f�[�^��������
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_DeleteAllRes(TEXRES_MANAGER_PTR resm);

//----------------------------------------------------------------------------
/**
 *
 *@brief	���\�[�X�I�u�W�F�N�g�f�[�^�|�C���^���擾
 *
 *@param	resm		���\�[�X�Ǘ��f�[�^
 *@param	id			���\�[�X��ID	(�Ǘ�ID)
 *
 *@retval	NULL		�o�^����Ă��Ȃ�
 *@retval	TEXRES_OBJ_PTR	�o�^����Ă��郊�\�[�X�I�u�W�F�|�C���^
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL TEXRES_OBJ_PTR TEXRESM_GetResObj(CONST_TEXRES_MANAGER_PTR resm, int id);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�o�^����Ă���ID���擾
 *
 *@param	r_obj		���\�[�X�I�u�W�F
 *
 *@retval	int			�Ǘ�ID
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL int TEXRESM_GetID(CONST_TEXRES_OBJ_PTR r_obj);

//----------------------------------------------------------------------------
/**
 *
 *@brief	�o�^����Ă���ID��ݒ�
 *
 *@param	resm		���\�[�X�}�l�[�W��
 *@param	r_obj		���\�[�X�I�u�W�F
 *@param	id			�ݒ肷��ID
 *
 *@retval	none
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_SetID(TEXRES_MANAGER_PTR resm, TEXRES_OBJ_PTR r_obj, int id);

//----------------------------------------------------------------------------
/**
 * �I�u�W�F�N�g�|�C���^�Ŏ擾
 *
 *@brief	�o�^����Ă���e�N�X�`�����\�[�X�f�[�^���擾
 *
 *@param	r_obj		���\�[�X�I�u�W�F
 *
 *@retval	NULL		�o�^����Ă��Ȃ�
 *@retval	NULL�ȊO	�o�^����Ă���e�N�X�`�����\�[�X�|�C���^
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSG3dResTex* TEXRESM_GetResPTR(CONST_TEXRES_OBJ_PTR r_obj);

//----------------------------------------------------------------------------
/**
 * ID�Ŏ擾
 *
 *@brief	�o�^����Ă���e�N�X�`�����\�[�X�f�[�^���擾
 *
 *@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *@param	id			�o�^ID
 *
 *@retval	NULL		�o�^����Ă��Ȃ�
 *@retval	NULL�ȊO	�o�^����Ă���e�N�X�`�����\�[�X�|�C���^
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSG3dResTex* TEXRESM_GetResID(CONST_TEXRES_MANAGER_PTR resm, int id);

//----------------------------------------------------------------------------
/**
 * ���I�u�W�F�N�g�|�C���^�Ŋm��
 *
 *	@brief	�e�N�X�`���L�[��VramManager����m�ۂ��܂�
 *
 *	@param	r_obj		���\�[�X�I�u�W�F
 *
 *	@return none
 *
 * �m�ۂ���VramKey�i�e�N�X�`���L�[�@�p���b�g�L�[�j
 * ��TEXRESM_DeleteRes�`�֐��Ŕj������܂��B
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_AllocVramKeyPTR( TEXRES_OBJ_PTR r_obj );

//----------------------------------------------------------------------------
/**
 * ��ID�Ŋm��
 *
 *	@brief	�e�N�X�`���L�[��VramManager����m�ۂ��܂�
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *
 *	@return none
 *
 * �m�ۂ���VramKey�i�e�N�X�`���L�[�@�p���b�g�L�[�j
 * ��TEXRESM_DeleteRes�`�֐��Ŕj������܂��B
 *
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_AllocVramKeyID( CONST_TEXRES_MANAGER_PTR resm, int id );

//-----------------------------------------------------------------------------
/**
*		�e�N�X�`���f�[�^��]��������A
*		�e�N�X�`�����f�[�^��j�����֐��S
*
*		TEXRESM_TexLoadAndCutTex�`�֐��͓]����
*							�e�N�X�`�����f�[�^�j���𓯎��ɍs���܂��B
*
*		TEXRESM_TexLoad�`�֐��̓e�N�X�`���]�����s���܂��B
*		TEXRESM_CutTex�`�֐��̓e�N�X�`�����f�[�^�̔j�����s���܂��B
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 * ���I�u�W�F�N�g�|�C���^
 *
 *	@brief	�e�N�X�`�����\�[�X��Vram�ɓ]�����ăe�N�X�`���̎��f�[�^��j��
 *
 *	@param	r_obj	�e�N�X�`�����\�[�X�I�u�W�F
 *
 *	@return	none
 *
 * ���̊֐����ĂԂƃe�N�X�`�����f�[�^�͔j������܂��B
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_TexLoadAndCutTexPTR( TEXRES_OBJ_PTR r_obj );
//----------------------------------------------------------------------------
/**
 * ��ID
 *
 *	@brief	�e�N�X�`�����\�[�X��Vram�ɓ]�����ăe�N�X�`���̎��f�[�^��j��
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *
 *	@return	none
 *
 * ���̊֐����ĂԂƃe�N�X�`�����f�[�^�͔j������܂��B
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_TexLoadAndCutTexID( TEXRES_MANAGER_PTR resm, int id );
//----------------------------------------------------------------------------
/**
 * ���I�u�W�F�N�g�|�C���^
 *
 *	@brief	�e�N�X�`�����\�[�X��Vram�ɓ]��
 *
 *	@param	r_obj	�e�N�X�`�����\�[�X�I�u�W�F
 *
 *	@return	none
 *
 *	TEXRESM_TexLoadAndCutTex�֐���
 *		�E�e�N�X�`���f�[�^�]��
 *		�E�e�N�X�`�����f�[�^�폜
 *	�̂Q�̏����ɕ������֐��ł��B
 * 
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_TexLoadPTR( TEXRES_OBJ_PTR r_obj );
//----------------------------------------------------------------------------
/**
 * ��ID
 *
 *	@brief	�e�N�X�`�����\�[�X��Vram�ɓ]��
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *
 *	@return	none
 *
 *	TEXRESM_TexLoadAndCutTex�֐���
 *		�E�e�N�X�`���f�[�^�]��
 *		�E�e�N�X�`�����f�[�^�폜
 *	�̂Q�̏����ɕ������֐��ł��B
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_TexLoadID( TEXRES_MANAGER_PTR resm, int id );
//----------------------------------------------------------------------------
/**
 * ���I�u�W�F�N�g�|�C���^
 *
 *	@brief	�e�N�X�`�����\�[�X�̎��f�[�^��j��
 *
 *	@param	r_obj	�e�N�X�`�����\�[�X�I�u�W�F
 *
 *	@return	none
 *
 *	TEXRESM_TexLoadAndCutTex�֐���
 *		�E�e�N�X�`���f�[�^�]��
 *		�E�e�N�X�`�����f�[�^�폜
 *	�̂Q�̏����ɕ������֐��ł��B
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_CutTexPTR( TEXRES_OBJ_PTR r_obj );
//----------------------------------------------------------------------------
/**
 * ��ID
 *
 *	@brief	�e�N�X�`�����\�[�X�̎��f�[�^��j��
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *
 *	@return	none
 *
 *	TEXRESM_TexLoadAndCutTex�֐���
 *		�E�e�N�X�`���f�[�^�]��
 *		�E�e�N�X�`�����f�[�^�폜
 *	�̂Q�̏����ɕ������֐��ł��B
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_CutTexID( TEXRES_MANAGER_PTR resm, int id );

//----------------------------------------------------------------------------
/**
 * �I�u�W�F�N�g�|�C���^�Ŏ擾
 *
 *	@brief	�e�N�X�`���L�[�̎擾
 *
 *	@param	r_obj	�e�N�X�`�����\�[�X�I�u�W�F
 *
 *	@return	�e�N�X�`���L�[
 *	
 *	NNSGfdTexKey�̒���
 *		NNS_GFD_ALLOC_ERROR_TEXKEY	VramKey�擾���ĂȂ�
 *		���̑�						VramKey�擾���Ă�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSGfdTexKey TEXRESM_GetTexKeyPTR( CONST_TEXRES_OBJ_PTR r_obj );

//----------------------------------------------------------------------------
/**
 * ID�Ŏ擾
 *
 *	@brief	�e�N�X�`���L�[�̎擾
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *
 *	@return	�e�N�X�`���L�[
 *
 *	NNSGfdTexKey�̒���
 *		NNS_GFD_ALLOC_ERROR_TEXKEY	VramKey�擾���ĂȂ�
 *		���̑�						VramKey�擾���Ă�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSGfdTexKey TEXRESM_GetTexKeyID( CONST_TEXRES_MANAGER_PTR resm, int id );

//----------------------------------------------------------------------------
/**
 * �I�u�W�F�N�g�|�C���^�Ŏ擾
 *
 *	@brief	�e�N�X�`��4x4�L�[�̎擾
 *
 *	@param	r_obj	�e�N�X�`�����\�[�X�I�u�W�F
 *
 *	@return	�e�N�X�`���L�[
 *
 *	NNSGfdTexKey�̒���
 *		NNS_GFD_ALLOC_ERROR_TEXKEY	VramKey�擾���ĂȂ�
 *		���̑�						VramKey�擾���Ă�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSGfdTexKey TEXRESM_GetTex4x4KeyPTR( CONST_TEXRES_OBJ_PTR r_obj );

//----------------------------------------------------------------------------
/**
 * ID�Ŏ擾
 *
 *	@brief	�e�N�X�`��4x4�L�[�̎擾
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *
 *	@return	�e�N�X�`���L�[
 *
 *	NNSGfdTexKey�̒���
 *		NNS_GFD_ALLOC_ERROR_TEXKEY	VramKey�擾���ĂȂ�
 *		���̑�						VramKey�擾���Ă�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSGfdTexKey TEXRESM_GetTex4x4KeyID( CONST_TEXRES_MANAGER_PTR resm, int id );

//----------------------------------------------------------------------------
/**
 * �I�u�W�F�N�g�|�C���^�Ŏ擾
 *
 *	@brief	�p���b�g�L�[�̎擾
 *
 *	@param	r_obj	�e�N�X�`�����\�[�X�I�u�W�F
 *
 *	@return	�p���b�g�L�[
 *
 *	NNSGfdPlttKey�̒���
 *		NNS_GFD_ALLOC_ERROR_PLTTKEY	VramKey�擾���ĂȂ�
 *		���̑�						VramKey�擾���Ă�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSGfdPlttKey TEXRESM_GetPlttKeyPTR( CONST_TEXRES_OBJ_PTR r_obj );

//----------------------------------------------------------------------------
/**
 * ID�Ŏ擾
 *
 *	@brief	�p���b�g�L�[�̎擾
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *
 *	@return	�p���b�g�L�[
 *
 *	NNSGfdPlttKey�̒���
 *		NNS_GFD_ALLOC_ERROR_PLTTKEY	VramKey�擾���ĂȂ�
 *		���̑�						VramKey�擾���Ă�
 *
 */
//-----------------------------------------------------------------------------
GLOBAL NNSGfdPlttKey TEXRESM_GetPlttKeyID( CONST_TEXRES_MANAGER_PTR resm, int id );

//-----------------------------------------------------------------------------
/**
*		�w�肳�ꂽVramKey�ɓ]������֐��ł��B
*
*		TEXRESM_CutTex�`�֐��Ŏ��f�[�^��j�������f�[�^��
*									�]�����邱�Ƃ��ł��܂���B
*/
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 * ���I�u�W�F�N�g�|�C���^
 *
 *	@brief	�w�肳�ꂽVramKey�ʒu�Ƀe�N�X�`����]�����܂��B
 *
 *	@param	r_obj		�e�N�X�`�����\�[�X�I�u�W�F
 *	@param	texkey		�e�N�X�`���L�[
 *	@param	tex4x4key	�e�N�X�`��4x4�L�[
 *	@param	plttkey		�p���b�g�L�[
 *
 *	@return	none
 *
 * ������texkey�Eplttkey�f�[�^�̓e�N�X�`�����\�[�X�}�l�[�W���ł�
 * �Ǘ����܂���̂ŁA�e���m�ہA�j�����Ă��������B
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_TexLoadOfKeyPTR( TEXRES_OBJ_PTR r_obj, NNSGfdTexKey texkey, NNSGfdTexKey tex4x4key, NNSGfdPlttKey plttkey );

//----------------------------------------------------------------------------
/**
 *	��ID
 * 
 *	@brief	�w�肳�ꂽVramKey�ʒu�Ƀe�N�X�`����]�����܂��B
 *
 *	@param	resm		�e�N�X�`�����\�[�X�}�l�[�W��
 *	@param	id			�o�^ID
 *	@param	texkey		�e�N�X�`���L�[
 *	@param	tex4x4key	�e�N�X�`��4x4�L�[
 *	@param	plttkey		�p���b�g�L�[
 *
 *	@return	none
 *
 * ������texkey�Eplttkey�f�[�^�̓e�N�X�`�����\�[�X�}�l�[�W���ł�
 * �Ǘ����܂���̂ŁA�e���m�ہA�j�����Ă��������B
 *
 */
//-----------------------------------------------------------------------------
GLOBAL void TEXRESM_TexLoadOfKeyID( TEXRES_MANAGER_PTR resm, int id, NNSGfdTexKey texkey, NNSGfdTexKey tex4x4key, NNSGfdPlttKey plttkey );




//-----------------------------------------------------------------------------
/**
 *			�c�[���֐�
 */
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------
/**
 *	@brief	���f�����\�[�X����e�N�X�`�����f�[�^�𔲂����T�C�Y���擾����
 *
 *	@param	p_file	���f���̃t�@�C��
 *
 *	@return	�e�N�X�`���f�[�^��j�������T�C�Y
 */
//-----------------------------------------------------------------------------
GLOBAL u32 TEXRESM_TOOL_CutTexDataSizeGet( NNSG3dResFileHeader* p_file );



#undef	GLOBAL
#endif		// __RES_MANAGER_H__
