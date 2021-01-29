/*---------------------------------------------------------------------------*
  Project:  NitroDWC - base - memfunc
  File:     dwc_memfunc.c

  Copyright 2005-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: dwc_memfunc.c,v $
  Revision 1.5  2007/08/24 10:05:07  takayama
  Realloc�֐��ŐV�����擾�����T�C�Y���ǂݏo���Ă��������C���B

  Revision 1.4  2006/12/28 23:48:43  takayama
  NULL�|�C���^�̉���������ŗ}����悤�ɕύX�Brealloc�̕����͂��̂܂܁B

  Revision 1.3  2006/11/07 06:13:24  nakata
  NULL�|�C���^���J�����悤�Ƃ����ꍇ��GameSpySDK�����Free�Ɍ����Ė�������悤�ɏC��

  Revision 1.2  2005/09/17 07:42:15  sasakit
  realloc�Ń������T�C�Y������������Ƃ��̖����C���B

  Revision 1.1  2005/08/19 06:32:35  sasakit
  �t�@�C���̒ǉ��B

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>
#include <base/dwc_memfunc.h>

/** --------------------------------------------------------------------
  structs
  ----------------------------------------------------------------------*/

// �������[�m�ۂ����Ƃ��̃w�b�_
typedef struct _tagDWCiAllocHeader
{

    u32		signature;	// ���ʎq

    u32		size;		// �m�ۂ����T�C�Y

    u32		dummy[6];	// �_�~�[(�w�b�_�̃T�C�Y��32�o�C�g�ɂ��邽��)

}
DWCiAllocHeader;

#define DWCi_MEMFUNC_SIGNATURE 'DWCM'

/** --------------------------------------------------------------------
  statics
  ----------------------------------------------------------------------*/
static DWCAllocEx s_alloc;
static DWCFreeEx  s_free;


/** --------------------------------------------------------------------
  function prototypes
  ----------------------------------------------------------------------*/
static void*	DWCi_GetAllocateHeader	(void* ptr);
static void*	DWCi_SetAllocateHeader	(void* ptr, u32 size);
static u32		DWCi_GetAllocateSize	(void* ptr);

static void*	DWCi_GetAllocateHeader(void* ptr)
{
    DWCiAllocHeader* header = (DWCiAllocHeader*)(((u8*)ptr) - sizeof(DWCiAllocHeader));

    SDK_ASSERT( header->signature == DWCi_MEMFUNC_SIGNATURE );

    return (void*)header;
}

static void*	DWCi_SetAllocateHeader(void* ptr, u32 size)
{
    DWCiAllocHeader* header = (DWCiAllocHeader*)ptr;

    header->signature = DWCi_MEMFUNC_SIGNATURE;
    header->size = size;

    return (void*)(((u8*)ptr) + sizeof(DWCiAllocHeader));
}

static u32		DWCi_GetAllocateSize(void* ptr)
{
    DWCiAllocHeader* header = DWCi_GetAllocateHeader( ptr );

    SDK_ASSERT( header != NULL );

    return header->size;
}

void
DWC_SetMemFunc( DWCAllocEx alloc, DWCFreeEx free )
{
    s_alloc = alloc;
    s_free  = free;
}

void*
DWC_Alloc( DWCAllocType name,
           u32          size )
{
    return DWC_AllocEx( name, size, 32 );
}

void*
DWC_AllocEx( DWCAllocType name,
             u32          size,
             int          align  )
{
    void* ptr;

    SDK_ASSERTMSG( s_alloc, "DWC_MEMFUNC: Allocator function isn't defined.\n" );
    SDK_ASSERTMSG( (align <= 32) && (align >= -32), "DWC_Alloc() dosen't support to align over 32 bytes order" );

    ptr = s_alloc( name, size + sizeof(DWCiAllocHeader), align );

    if ( ptr == NULL )
        return NULL;

    return DWCi_SetAllocateHeader(ptr, size);
}

void
DWC_Free( DWCAllocType name,
          void*        ptr,
          u32          size )
{
#pragma unused(size)

    SDK_ASSERTMSG( s_free, "DWC_MEMFUNC: Free function isn't defined.\n" );

    if ( ptr == NULL )
    {
        return;
    }

    // �擪�̃|�C���^���擾����
    ptr = DWCi_GetAllocateHeader(ptr);

    s_free( name, ptr, ((DWCiAllocHeader*)ptr)->size + sizeof(DWCiAllocHeader) );
}

void*
DWC_Realloc( DWCAllocType name,
             void*        ptr,
             u32          oldsize,
             u32          newsize )
{
    return DWC_ReallocEx( name, ptr, oldsize, newsize, 32 );
}

void*
DWC_ReallocEx( DWCAllocType name,
               void*        ptr,
               u32          oldsize,
               u32          newsize,
               int          align    )
{
    void* newptr;

    newptr = DWC_AllocEx( name, newsize, align );

    if ( newptr == NULL )
    {
        return NULL;
    }

    if ( ptr != NULL )
    {
        oldsize = DWCi_GetAllocateSize(ptr);

        MI_CpuCopy8( ptr, newptr, (oldsize > newsize) ? newsize : oldsize );

        DWC_Free( name, ptr, oldsize );
    }

    return newptr;
}


void*
DWCi_GsMalloc  (size_t size)
{
    return DWC_Alloc( DWC_ALLOCTYPE_GS, (u32)size );
}
void*
DWCi_GsRealloc (void* ptr, size_t size)
{
    return DWC_Realloc( DWC_ALLOCTYPE_GS, ptr, (u32)size, (u32)size );
}
void
DWCi_GsFree    (void* ptr)
{
    DWC_Free( DWC_ALLOCTYPE_GS, ptr, 0 );
}
void*
DWCi_GsMemalign(size_t boundary, size_t size)
{
    // ���S�ł͂Ȃ����A�K�v�\���Ȏd�l�ƍl������B
    return DWC_AllocEx( DWC_ALLOCTYPE_GS, (u32)size, (int)boundary );
}
