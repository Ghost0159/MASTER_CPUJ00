//==============================================================================
/**
 * @file	frontier_particle.c
 * @brief	�t�����e�B�A 2D�}�b�v�p�p�[�e�B�N��
 * @author	matsuda
 * @date	2007.06.06(��)
 */
//==============================================================================
#include "common.h"
#include "system/particle.h"
#include "system/arc_tool.h"

#include "frontier_particle.h"
#include "fs_usescript.h"


//==============================================================================
//	�萔��`
//==============================================================================
///�퓬�p�[�e�B�N���̃J�����j�A�ݒ�
#define BP_NEAR			(FX32_ONE)
///�퓬�p�[�e�B�N���̃J�����t�@�[�ݒ�
#define BP_FAR			(FX32_ONE * 900)


//==============================================================================
//	�\���̒�`
//==============================================================================
///�t�����e�B�A�p�[�e�B�N���V�X�e���\����
typedef struct _FRP_WORK{
	PTC_PTR ptc[SPAWORK_MAX];
	u16 heap_id;
}FRP_WORK;


//==============================================================================
//	�v���g�^�C�v�錾
//==============================================================================
static u32 sAllocTex(u32 size, BOOL is4x4comp);
static u32 sAllocTexPalette(u32 size, BOOL is4pltt);
static void Local_ParticleExit(PTC_PTR ptc);



//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���V�X�e��������
 * @param   heap_id		�q�[�vID
 */
//--------------------------------------------------------------
FRP_PTR FRParticle_Init(int heap_id)
{
	FRP_WORK *frp;
	
	frp = sys_AllocMemory(heap_id, sizeof(FRP_WORK));
	MI_CpuClear8(frp, sizeof(FRP_WORK));
	
	frp->heap_id = heap_id;
	
	//�p�[�e�B�N���V�X�e��������
	Particle_SystemWorkInit();
	
	return frp;
}

//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���V�X�e���I��
 * @param   frp		�t�����e�B�A�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void FRParticle_Exit(FRP_PTR frp)
{
	int i;
	void *heap;
	
	for(i = 0; i < SPAWORK_MAX; i++){
		if(frp->ptc[i] != NULL){
			Local_ParticleExit(frp->ptc[i]);
		}
	}
	
	sys_FreeMemoryEz(frp);
}


//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���V�X�e�����쐬���A���\�[�X�̓ǂݍ��݁��o�^
 *
 * @param   frp			�t�����e�B�A�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 * @param   work_id		SPAWORK_???
 * @param   spa_no		SPA�C���f�b�N�X
 *
 * @retval  �������ꂽ�p�[�e�B�N���V�X�e���̃|�C���^
 */
//--------------------------------------------------------------
PTC_PTR FRParticle_SystemCreate(FRP_PTR frp, int work_id, int spa_no, int camera_type)
{
	PTC_PTR ptc;
	void *heap;
	GF_CAMERA_PTR camera_ptr;
	void *resource;
	
	GF_ASSERT(frp->ptc[work_id] == NULL);
	
	//�p�[�e�B�N���V�X�e���쐬
	heap = sys_AllocMemory(frp->heap_id, PARTICLE_LIB_HEAP_SIZE);
	ptc = Particle_SystemCreate(sAllocTex, sAllocTexPalette, heap, 
		PARTICLE_LIB_HEAP_SIZE, TRUE, frp->heap_id);
	camera_ptr = Particle_GetCameraPtr(ptc);
	GFC_SetCameraClip(BP_NEAR, BP_FAR, camera_ptr);
	Particle_CameraTypeSet( ptc ,camera_type );

	//���\�[�X�ǂݍ��݁��o�^
	resource = Particle_ArcResourceLoad(ARC_FRONTIER_PARTICLE, spa_no, frp->heap_id);
	Particle_ResourceSet(ptc, resource, PTC_AUTOTEX_LNK | PTC_AUTOPLTT_LNK, TRUE);
	
	//�Ǘ��̈�ɓo�^
	frp->ptc[work_id] = ptc;
	
	return ptc;
}

//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���V�X�e�����I�����A�Ǘ��̈悩����폜����
 *
 * @param   frp		�t�����e�B�A�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 * @param   work_id	SPAWORK_???
 * @param   ptc		�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
void FRParticle_SystemExit(FRP_PTR frp, int work_id)
{
#if 1
	GF_ASSERT(frp->ptc[work_id] != NULL);
	
	Local_ParticleExit(frp->ptc[work_id]);
	frp->ptc[work_id] = NULL;
#else
	int i;
	
	Local_ParticleExit(ptc);
	
	for(i = 0; i < SPAWORK_MAX; i++){
		if(frp->ptc[i] == ptc){
			frp->ptc[i] = NULL;
			return;
		}
	}
	GF_ASSERT(0);	//�Ǘ��̈�ɓo�^����Ă��Ȃ�PTC���n����Ă���
#endif
}

//--------------------------------------------------------------
/**
 * @brief   �t�����e�B�A�p�p�[�e�B�N���V�X�e���E���C���֐�(�v�Z�E�`�揈���Ȃǂ����s)
 *
 * @param   ptc		�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 * @retval  FALSE:1�������Ă���p�[�e�B�N�����Ȃ�
 */
//--------------------------------------------------------------
int FRParticle_Main(void)
{
	int draw_num;
	
	GF_G3X_Reset();


	if(Particle_GetActionNum() == 0){
		return FALSE;
	}
	

	draw_num = Particle_DrawAll();	//�p�[�e�B�N���`��

	if(draw_num > 0){
		//�p�[�e�B�N���̕`�悪�I�������̂ŁA�Ăу\�t�g�E�F�A�X�v���C�g�p�J�����ɐݒ�
		GF_G3X_Reset();
	}

	Particle_CalcAll();	//�p�[�e�B�N���v�Z

	return TRUE;
}

//--------------------------------------------------------------
/**
 * @brief   PTC�|�C���^���擾����
 *
 * @param   frp			�t�����e�B�A�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 * @param   work_id		SPAWORK_???
 *
 * @retval  PTC�|�C���^
 */
//--------------------------------------------------------------
PTC_PTR FRParticle_PTCPtrGet(FRP_PTR frp, int work_id)
{
	GF_ASSERT(frp->ptc[work_id] != NULL);
	return frp->ptc[work_id];
}

//--------------------------------------------------------------
/**
 * @brief   ��������Ă���p�[�e�B�N���G�~�b�^�S�Ă̏I���҂�
 *
 * @param   frp		�t�����e�B�A�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 *
 * @retval  TRUE:�S�ďI�����Ă���B�@FALSE:1�ȏ�̃G�~�b�^�����삵�Ă���
 */
//--------------------------------------------------------------
BOOL FRParticle_EmitNumGet(FRP_PTR frp)
{
	int i;
	
	for(i = 0; i < SPAWORK_MAX; i++){
		if(frp->ptc[i] != NULL){
			if(Particle_GetEmitterNum(frp->ptc[i]) > 0){
				return FALSE;
			}
		}
	}
	return TRUE;	//�����Ă���G�~�b�^�͂ЂƂ��Ȃ�
}

//==============================================================================
//	
//==============================================================================
//--------------------------------------------------------------
/**
 * @brief   �p�[�e�B�N���V�X�e�����I��������
 * @param   ptc		�p�[�e�B�N���V�X�e�����[�N�ւ̃|�C���^
 */
//--------------------------------------------------------------
static void Local_ParticleExit(PTC_PTR ptc)
{
	void *heap;

	heap = Particle_HeapPtrGet(ptc);
	Particle_SystemExit(ptc);
	sys_FreeMemoryEz(heap);
}

//--------------------------------------------------------------
/**
 * @brief   �e�N�X�`��VRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param   size		�e�N�X�`���T�C�Y
 * @param   is4x4comp	4x4���k�e�N�X�`���ł��邩�ǂ����̃t���O(TRUE=���k�e�N�X�`��)
 *
 * @retval  �ǂݍ��݂��J�n����VRAM�̃A�h���X
 */
//--------------------------------------------------------------
static u32 sAllocTex(u32 size, BOOL is4x4comp)
{
	u32 address;
	NNSGfdTexKey key;
	
	key = NNS_GfdAllocTexVram(size, is4x4comp, 0);
	GF_ASSERT(key != NNS_GFD_ALLOC_ERROR_TEXKEY);
	Particle_LnkTexKeySet(key);		//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	
	address = NNS_GfdGetTexKeyAddr(key);
	OS_TPrintf("vram=%d\n", address);
	return address;
}

//--------------------------------------------------------------
/**
 * @brief	�e�N�X�`���p���b�gVRAM�A�h���X��Ԃ����߂̃R�[���o�b�N�֐�
 *
 * @param	size		�e�N�X�`���T�C�Y
 * @param	is4pltt		4�F�p���b�g�ł��邩�ǂ����̃t���O
 *
 * @retval	�ǂݍ��݂��J�n����VRAM�̃A�h���X
 *
 * direct�`���̃e�N�X�`���̏ꍇ�ASPL_LoadTexPlttByCallbackFunction��
 * �R�[���o�b�N�֐����Ăяo���܂���B
 */
//--------------------------------------------------------------
static u32 sAllocTexPalette(u32 size, BOOL is4pltt)
{
	NNSGfdPlttKey key;
	u32 address;
	
	key = NNS_GfdAllocPlttVram(size, is4pltt, NNS_GFD_ALLOC_FROM_LOW);
	if(key == NNS_GFD_ALLOC_ERROR_PLTTKEY){
		GF_ASSERT(0 && "�p�[�e�B�N���Ńp���b�g�̊m�ۂ��o���܂���I\n");
	}
	Particle_PlttLnkTexKeySet(key);	//�����N�h���X�g���g�p���Ă���̂ŃL�[�����Z�b�g
	address = NNS_GfdGetPlttKeyAddr(key);
	OS_TPrintf("vram=%d\n", address);
	return address;
}
