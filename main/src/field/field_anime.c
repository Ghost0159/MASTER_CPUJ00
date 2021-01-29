//=============================================================================
/**
 * @file	field_anime.c
 * @bfief	�t�B�[���h�A�j�������i��ɓ]���A�j��)
 * @author	mori GAME FREAK inc.
 *
 *
 */
//=============================================================================

#include "common.h"
#include "system/arc_tool.h"
#include "calctool.h"
#include "str_tool.h"
#include "system/lib_pack.h"
#include "field_anime.h"

#include "fldtanime.naix"

//==============================================================================
// ��`
//==============================================================================
//---- kagaya debug
#ifdef DEBUG_ONLY_FOR_kagaya
//#define DEBUG_ANIME_NAME_PRINT
#endif
//----

#define FIELD_ANIME_MAX			( 16 )	 // �n�`�p�]���A�j���̓���������
#define TEXTURE_NAME_LENGTH		( 16 )	 // �e�N�X�`���w�薼�̍ő咷
#define TEXTURE_ANIME_MAX		( 18 )	 // �����ɓo�^�ł���A�j���[�V�����̐�

#define TEX_ANIME_END_CODE		( 0xff ) // �A�j���f�[�^�̏I�[��`(AnmTbl�p)

// �A�j���f�[�^��`
typedef struct{							// �A�j���p�f�[�^�iROM����ǂݍ��݂܂�)
	char name[TEXTURE_NAME_LENGTH];		// �e�N�X�`���t�@�C����
	u8	 AnmTbl[TEXTURE_ANIME_MAX][2];	// �A�j���p�e�[�u��[0]:�p�^�[���ԍ�  [1]:wait
}FIELD_ANIME;


// �]���A�j���Ǘ����[�N
typedef struct {
	u8  			*texadr;			// ���ɓ]������Ă���e�N�X�`���A�h���X
	int 			texsize;			// �]������e�N�X�`���T�C�Y
	NNSG3dResTex 	*animetex;			// �]������e�N�X�`�����\�[�X�̃|�C���^
	void			*texfile;			// �e�N�X�`���t�@�C���̎���
	FIELD_ANIME		*animetbl;			// �A�j���[�V������`�e�[�u���̃|�C���^
	u16				point,wait;			// ���݂̕\���|�C���^,�E�F�C�g
}FIELD_ANIME_WORK;

// �ǂݍ��񂾃A�j���f�[�^��cast���邽�߂̍\���̒�`
typedef struct{
	u32 num;
	FIELD_ANIME anime_sheet[0];			// num�̎w��Ō��Ƀf�[�^������
}FIELD_ANIME_SHEET;


// �t�B�[���h�]���A�j���p�R���g���[�����[�N
struct _FIELD_ANIME_CONTROL_WORK{
	void 			 	*anime_bindata;	// ROM����ǂݍ��ރA�j���w��f�[�^
	FIELD_ANIME_SHEET   *fa_sheet;
	FIELD_ANIME_WORK faw[FIELD_ANIME_MAX];	// �A�j���p���[�N
};

typedef struct _FIELD_ANIME_CONTROL_WORK FIELD_ANIME_CONTROL_WORK;

//==============================================================================
// ���[�N
//==============================================================================


//==============================================================================
// �֐�
//==============================================================================

static int  AnimeSet(FIELD_ANIME_PTR faw, NNSG3dResTex *tex, int AnimeNo, ARCHANDLE *handle);


//==============================================================================
/**
 * ������
 *
 * @param   none		
 *
 * @retval  FIELD_ANIME_PTR		�A�j���Ǘ��|�C���^	
 */
//==============================================================================
FIELD_ANIME_PTR InitFieldAnime(void)
{
	int i;
	FIELD_ANIME_PTR facw=NULL;
	
	facw = sys_AllocMemory(HEAPID_FIELD,sizeof(FIELD_ANIME_CONTROL_WORK));


	GF_ASSERT(facw);

	for(i=0;i<FIELD_ANIME_MAX;i++){
		facw->faw[i].point    = 0;
		facw->faw[i].wait     = 0;
		facw->faw[i].texadr   = NULL;
		facw->faw[i].animetex = NULL;
		facw->faw[i].animetbl = NULL;
		facw->faw[i].texfile  = NULL;
	}
	
	return facw;
}


//114 892
// 34%


//------------------------------------------------------------------
/**
 * �t�B�[���h�]���A�j���o�^
 *
 * @param   tex			����VRAM�ɓ]���ς݂̃e�N�X�`���Ǘ��|�C���^
 * @param   Tindex		��̃e�N�X�`���̎w��index
 * @param   AnimeNo		�]�����s�������e�N�X�`���A�j���ԍ��i���ꂪ�t�@�C�����ɂȂ�)
 *
 * @retval  int 		�o�^�ł�����0-10 �ł��Ȃ�������-1
 */
//------------------------------------------------------------------
int FieldAnimeSets(FIELD_ANIME_PTR facw, NNSG3dResTex *tex)
{
	int i,result=0;
	ARCHANDLE *handle;

//	OS_Printf("field trans_ anime�o�^�J�n");

	// �t�@�C���n���h���I�[�v��
	handle = ArchiveDataHandleOpen( ARC_FIELD_TRANS_ANIME, HEAPID_FIELD );

	// �A�j���e�[�u���ǂݍ���
	facw->anime_bindata = ArchiveDataLoadAllocByHandle( handle, NARC_fldtanime_tanime_bin, HEAPID_FIELD );
	facw->fa_sheet      = (FIELD_ANIME_SHEET*)facw->anime_bindata;
	
//	OS_Printf("---------------------sheet data anime num = %d\n",facw->fa_sheet->num);
//	for(i=0;i<facw->fa_sheet->num;i++){
//		OS_Printf("------Anime No.%d: %s\n", i,facw->fa_sheet->anime_sheet[i].name);
//	}
	

	
	//�t�@�C���̐������e�N�X�`���������o�^��Ƃ��s��
	for (i=0;i<facw->fa_sheet->num;i++){
		if(AnimeSet(facw, tex, i, handle)>=0){
			result++;
		}
	}

	//�t�@�C���n���h���N���[�Y
	ArchiveDataHandleClose( handle );

//	OS_Printf("field anime num = %d\n",result);
	
	return result;
}


//------------------------------------------------------------------
/**
 * �t�B�[���h�]���A�j���o�^
 *
 * @param   tex			����VRAM�ɓ]���ς݂̃e�N�X�`���Ǘ��|�C���^
 * @param   Tindex		��̃e�N�X�`���̎w��index
 * @param   AnimeNo		�]�����s�������e�N�X�`���A�j���ԍ��i���ꂪ�t�@�C�����ɂȂ�)
 *
 * @retval  int 		�o�^�ł�����0-10 �ł��Ȃ�������-1
 */
//------------------------------------------------------------------
static int AnimeSet(FIELD_ANIME_PTR facw, NNSG3dResTex *tex, int AnimeNo, ARCHANDLE *handle)
{
	int  no,i;
	char filename[40];
	
	//�m�ۂ���ĂȂ��̂ɌĂ΂�Ă邩������Ȃ��̂�
	if(facw==NULL){								
		return -1;
	}

	//�g�p���Ă��Ȃ����[�N������
	for(no=0;no<FIELD_ANIME_MAX;no++){				
		if(facw->faw[no].texfile==NULL){
			break;
		}
	}
	//�o�^MAX �������͒n�`�e�N�X�`����NULL��������o�^���~
	if(no==FIELD_ANIME_MAX || tex==NULL){			
		return -1;
	}

	//�]���ςݒn�`�e�N�X�`������]���ΏۂƂȂ�ꏊ���擾����
	facw->faw[no].texadr   = GetTexStartVRAMAdrByName(tex,facw->fa_sheet->anime_sheet[AnimeNo].name);	
	if (facw->faw[no].texadr == NULL){
		//�e�N�X�`�����Ȃ�������o�^���~
		return -1;			
	}
	
	#ifdef DEBUG_ANIME_NAME_PRINT
	{
		OS_Printf( "FieldTransAnime [%s] Start\n",
			facw->fa_sheet->anime_sheet[AnimeNo].name );
	}
	#endif
	
	facw->faw[no].texsize  = GetTexByteSizeByName(tex,facw->fa_sheet->anime_sheet[AnimeNo].name);

	// �A�j���[�V�����f�[�^�ǂݍ���
	sprintf(filename,"data/fld_anime%d.bin",AnimeNo);				
	facw->faw[no].animetbl = &(facw->fa_sheet->anime_sheet[AnimeNo]);	
	
//	OS_Printf("field_anime animetbl=%08x\n",facw->faw[no].animetbl);
	
	//�]���A�j���e�N�X�`���ǂݍ���
	facw->faw[no].texfile = ArchiveDataLoadAllocByHandle( handle, AnimeNo+1, HEAPID_FIELD );

	facw->faw[no].animetex  = NNS_G3dGetTex((NNSG3dResFileHeader*) facw->faw[no].texfile );

	return no;
}



//==============================================================================
/**
 * �t�B�[���h�p�e�N�X�`���]���A�j���풓�֐�
 *
 * @param   faw		�A�j���Ǘ��|�C���^		
 *
 * @retval  none		
 */
//==============================================================================
void FieldAnimeMain(FIELD_ANIME_PTR facw)
{
	int i;
	
	if(facw==NULL){								//�m�ۂ���ĂȂ��̂ɌĂ΂�Ă邩������Ȃ��̂�
		return;
	}
	
	for(i=0;i<FIELD_ANIME_MAX;i++){				//�g�p���Ă��Ȃ����[�N������
		FIELD_ANIME_WORK *faw = &facw->faw[i];

		if(faw->texfile==NULL){
			continue;							//�����Z�b�g����Ă��Ȃ��ꍇ�̓X���[
		}

		//>>add
		if (faw->animetbl == NULL){
			continue;
		}
		//<<
		
		if(faw->animetbl->AnmTbl[faw->point][1] <= faw->wait){	//�A�j���X�V�^�C�~���O�̎�
			faw->wait = 0;						//�E�F�C�g�N���A
			faw->point++;						//�|�C���g�{�{
			if(faw->animetbl->AnmTbl[faw->point][0]==TEX_ANIME_END_CODE){	//�A�j����`���[�܂ł��Ă���ꍇ�̓��[�v
				faw->point = 0;
			}
			
			#ifdef DEBUG_ANIME_NAME_PRINT
			{
				OS_Printf( "FieldTransAnime [%s] PatternChange %d\n",
					faw->animetbl->name, faw->point );
			}
			#endif
			
			AddVramTransferManager(
					NNS_GFD_DST_3D_TEX_VRAM,		//�]���Z�b�g
					(u32)faw->texadr,
					GetTexStartAdr(faw->animetex,faw->animetbl->AnmTbl[faw->point][0]),
					faw->texsize);	
//			OS_Printf("i=%d, point=%d pat=%04x\n",i,faw[i].point,faw[i].animetbl->AnmTbl[faw[i].point][0]);
		}else{
			faw->wait++;			//�E�F�C�g�X�V
		}
	}
}



//==============================================================================
/**
 * �ʂɃe�N�X�`���]���A�j�������
 *
 * @param   faw		�A�j���Ǘ��|�C���^
 * @param   no		faw[]�̓Y��
 *
 * @retval  none	
 */
//==============================================================================
void FieldAnimeRelease(FIELD_ANIME_PTR facw, int no )
{
	// �e�N�X�`���t�@�C���̉���̓L���b�V���ɐς܂�Ă���̂ōs��Ȃ�
	

	//�m�ۂ���ĂȂ�������I��
	if(facw==NULL){
		return;
	}

	// �e�N�X�`���t�@�C�����
	if(facw->faw[no].texfile){
		sys_FreeMemoryEz(facw->faw[no].texfile);
	}

	// �A�j���[�V������`�e�[�u���w�������
	if(facw->faw[no].animetbl!=NULL){
		facw->faw[no].animetbl = NULL;
	}
	facw->faw[no].point = 0;	//�A�j���p���[�N�̏�����
	facw->faw[no].wait  = 0;
}

//==============================================================================
/**
 * �e�N�X�`���A�j���]���V�X�e���I��
 *
 * @param   faw		�A�j���Ǘ��|�C���^
 *
 * @retval  none
 */
//==============================================================================
void FieldAnimeAllRelease(FIELD_ANIME_PTR facw)
{
	int i;
	
	//�m�ۂ���ĂȂ��̂ɌĂ΂�Ă邩������Ȃ��̂�
	if(facw==NULL){								
		return;
	}	
	// �A�j���o�^�e�[�u���̉��
	for(i=0;i<FIELD_ANIME_MAX;i++){
		FieldAnimeRelease( facw, i );
	}
	
	// �A�j���[�V�����f�[�^���
	sys_FreeMemoryEz(facw->anime_bindata);
	
}

//==============================================================================
/**
 * �t�B�[���h�]���A�j���V�X�e�����
 *
 * @param   facw		�A�j���Ǘ��|�C���^
 *
 * @retval  none		
 */
//==============================================================================
void ReleaseFieldAnime(FIELD_ANIME_PTR facw)
{
	if(facw!=NULL){
		sys_FreeMemoryEz(facw);
		facw = NULL;
	}
}



