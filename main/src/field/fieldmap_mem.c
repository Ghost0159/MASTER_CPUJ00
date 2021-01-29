#include "common.h"
#include "fieldmap_mem.h"

enum
{
	ALLOCTYPE_NON,
	ALLOCTYPE_SOURCE,
	ALLOCTYPE_MEMORY,
};

typedef struct FLD_MAP_MEM_tag
{
	u16 GroundAllocType;
	u16 HeightAllocType;
	
	void *GroundData[4];
	void *HeightData[4];
/**
	void *SeaAnm;
	void *Tex;
*/
}FLD_MAP_MEM;

static u8 FieldGroundData[4][FIELD_GROUND_DATA_SIZE];
//static u8 FieldHeightData[4][FIELD_HEIGHT_DATA_SIZE];

static void ClearFldMapMem( const u8 inIdx,
							const int inGroundMemSize,
							const int inHeightMemSize,
							FMM_PTR mem);


//�������m��
FMM_PTR AllocFldMapMem(const BOOL inHeightDataValid)
{
	u8 i;
	int height_mem_size;
	FMM_PTR mem;
	
	//�ǂݍ��݃G���A�m��
	mem = sys_AllocMemory( HEAPID_FIELD, sizeof(FLD_MAP_MEM) );
	memset( mem, 0, sizeof(FLD_MAP_MEM) );
	
	mem->GroundAllocType = ALLOCTYPE_SOURCE;
	if( inHeightDataValid ){ mem->HeightAllocType = ALLOCTYPE_MEMORY; }
	
	//�������m��
	for(i=0;i<4;i++){
		//�n�`�f�[�^�������m��
		mem->GroundData[i] = (void*)&FieldGroundData[i][0];
		if (mem->GroundData[i] == NULL){
			OS_Printf("%d,�n�`�f�[�^�������m�ێ��s\n",i);
			return NULL;
		}
		if (inHeightDataValid){
			//�����f�[�^�������m��
			mem->HeightData[i] = sys_AllocMemory( HEAPID_FIELD, FIELD_HEIGHT_DATA_SIZE );
//			mem->HeightData[i] = (void*)&FieldHeightData[i][0];
			if (mem->HeightData[i] == NULL){
				OS_Printf("%d,�����f�[�^�������m�ێ��s\n",i);
				return NULL;
			}
			height_mem_size = FIELD_HEIGHT_DATA_SIZE;
		}else{
			mem->HeightData[i] = NULL;
			height_mem_size = 0;
		}
		ClearFldMapMem(i,FIELD_GROUND_DATA_SIZE,height_mem_size,mem);
	}
	
	//�C�A�j���f�[�^�������m��
	//�n�`�e�N�X�`���������m��
	
	return mem;
}

//���������
void FreeFldMapMem(FMM_PTR mem)
{
	int i;
	
	//���������
	for( i = 0; i < 4; i++ ){
		//�n�`�f�[�^���������
		if( mem->GroundAllocType == ALLOCTYPE_MEMORY ){
			sys_FreeMemoryEz( (void *)mem->GroundData[i] );
		}
		mem->GroundData[i] = NULL;
		
		//�����f�[�^�͂Ȃ��Ƃ��i�n���Ƃ��j������̂łm�t�k�k�`�F�b�N����
		if (mem->HeightData[i] != NULL){
			//�����f�[�^���������
			if( mem->HeightAllocType == ALLOCTYPE_MEMORY ){
				sys_FreeMemoryEz( (void *)mem->HeightData[i] );
			}
			mem->HeightData[i] = NULL;
		}
	}
	
	sys_FreeMemoryEz((void *)mem);
	mem = NULL;
}

#if 0	//dp old
void FreeFldMapMem(FMM_PTR mem)
{
	u8 i;
	//���������
	for(i=0;i<4;i++){
		//�n�`�f�[�^���������
		//sys_FreeMemoryEz( (void *)mem->GroundData[i] );
		mem->GroundData[i] = NULL;
		//�����f�[�^�͂Ȃ��Ƃ��i�n���Ƃ��j������̂łm�t�k�k�`�F�b�N����
		if (mem->HeightData[i] != NULL){
			//�����f�[�^���������
			sys_FreeMemoryEz( (void *)mem->HeightData[i] );
			mem->HeightData[i] = NULL;
		}
	}
	sys_FreeMemoryEz((void *)mem);
	mem = NULL;
}
#endif

//�n�`�f�[�^�������ƃ|�C���^���o�C���h
void BindGroundMem(const u8 inIdx, FMM_CONST_PTR mem, void **outPtr)
{
	*outPtr = mem->GroundData[inIdx];
}

//�����f�[�^�������ƃ|�C���^���o�C���h
void BindHeightMem(const u8 inIdx, FMM_CONST_PTR mem, void **outPtr)
{
	*outPtr = mem->HeightData[inIdx];
}

static void ClearFldMapMem(	const u8 inIdx,
							const int inGroundMemSize,
							const int inHeightMemSize,
							FMM_PTR mem)
{
	MI_CpuClear8(mem->GroundData[inIdx],inGroundMemSize);
	MI_CpuClear8(mem->HeightData[inIdx],inHeightMemSize);
}

//�A���P�[�g�^�C�v
#if 0	//���������l�������ꊇ�^�C�v�@�T�C�Y�ł��߂��ċt�ɖ���
FMM_PTR AllocHeapFldMapMem( const BOOL inHeightDataValid )
{
	u8 *buf;
	u32 i,size;
	FMM_PTR mem;
	
	#ifdef DEBUG_ONLY_FOR_kagaya
	u32 b_free,a_free,m_block;
	b_free = sys_GetHeapFreeSize(HEAPID_FIELD);
	m_block = sys_GetHeapFreeSizeBlock(HEAPID_FIELD);
	#endif
	
	size = sizeof(FLD_MAP_MEM) + (FIELD_GROUND_DATA_SIZE * 4);
	if( inHeightDataValid == TRUE ){
		size += FIELD_HEIGHT_DATA_SIZE * 4;
	}
	buf = sys_AllocMemory( HEAPID_FIELD, sizeof(size) );
	GF_ASSERT( buf != NULL );
	MI_CpuClear8( buf, size );
	
	#ifdef DEBUG_ONLY_FOR_kagaya
	a_free = sys_GetHeapFreeSize( HEAPID_FIELD );
	OS_Printf("HeapMapMem AllocSize=%08xH,Free Block=%08xH,Before=%08xH,After=%08xH\n",
		size,m_block,b_free,a_free);
	#endif
	
	mem = (FMM_PTR)buf;
	size = sizeof( FLD_MAP_MEM );
	
	for( i = 0; i < 4; i++ ){
		mem->GroundData[i] = (void*)&buf[size];
		size += FIELD_GROUND_DATA_SIZE;
		if( inHeightDataValid == TRUE ){
			mem->HeightData[i] = (void*)&buf[size];
			size += FIELD_HEIGHT_DATA_SIZE;
		}
	}
	
	return( mem );
}

//�A���P�[�g�^�C�v�@�������J��
void FreeHeapMapMem( FMM_PTR mem )
{
	sys_FreeMemoryEz( mem );
}
#endif

//�A���P�[�g�^�C�v
FMM_PTR AllocHeapFldMapMem( const BOOL inHeightDataValid )
{
	u32 i;
	FMM_PTR mem;
	#ifdef DEBUG_ONLY_FOR_kagaya
	u32 b_free,a_free;
	b_free = sys_GetHeapFreeSize(HEAPID_FIELD);
	#endif
	
	mem = sys_AllocMemory( HEAPID_FIELD, sizeof(FLD_MAP_MEM) );
	memset( mem, 0, sizeof(FLD_MAP_MEM) );
	mem->GroundAllocType = ALLOCTYPE_MEMORY;
	if( inHeightDataValid ){ mem->HeightAllocType = ALLOCTYPE_MEMORY; }
	
	for( i = 0; i < 4; i++ ){
		mem->GroundData[i] = sys_AllocMemory(
				HEAPID_FIELD, FIELD_GROUND_DATA_SIZE );
		MI_CpuClear8( mem->GroundData[i], FIELD_GROUND_DATA_SIZE );
		if( inHeightDataValid == TRUE ){
			mem->HeightData[i] = sys_AllocMemory(
				HEAPID_FIELD, FIELD_HEIGHT_DATA_SIZE );
			MI_CpuClear8( mem->HeightData[i], FIELD_HEIGHT_DATA_SIZE );
		}
	}
	
	#ifdef DEBUG_ONLY_FOR_kagaya
	a_free = sys_GetHeapFreeSize( HEAPID_FIELD );
	OS_Printf("HeapMapMem Before=%08xH,After=%08xH\n",b_free,a_free);
	#endif
	
	return( mem );
}

//--------------------------------------------------------------
/**
 * 2�̒n�`������������
 * @param	mem0	��������FMM_PTR����1
 * @param	mem1	��������FMM_PTR����2
 * @retval
 */
//--------------------------------------------------------------
void ExchangeFldMapMemGround( FMM_PTR mem0, FMM_PTR mem1 )
{
	int i;
	FLD_MAP_MEM mem;
	
	mem = *mem0;
	mem0->GroundAllocType = mem1->GroundAllocType;
	mem1->GroundAllocType = mem.GroundAllocType;
	
	for( i = 0; i < 4; i++ ){
		mem0->GroundData[i] = mem1->GroundData[i];
		mem1->GroundData[i] = mem.GroundData[i];
	}
}

#if 0
//--------------------------------------------------------------
/**
 * �n�`�������̃A���b�N�^�C�v���擾
 * @param	mem
 * @retval	BOOL	TRUE=�A���b�N�������^
 */
//--------------------------------------------------------------
BOOL CheckFldMapMemGroundAlloc( FMM_PTR mem )
{
	if( mem->GroundAllocType == ALLOCTYPE_MEMORY ){
		return( TRUE );
	}
	
	return( FALSE );
}
#endif
