/*---------------------------------------------------------------------------*
  Project:  NitroSDK - OS - demos - cplusplus-1
  File:     new.cpp

  Copyright 2003,2004 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: new.cpp,v $
  Revision 1.4  2004/04/06 11:35:38  yada
  fix header comment

  Revision 1.3  2004/02/20 03:32:18  yasu
  add comments

  Revision 1.2  2004/02/20 00:04:05  yasu
  add comments

  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#include <nitro.h>

#define HEAP_ID 	((OSHeapHandle)0)
#define ARENA_ID	((OSArenaId)OS_ARENA_MAIN)
#define ROUND(n, a)     (((u32) (n) + (a) - 1) & ~((a) - 1))

//----------------------------------------------------------------
//
void* operator new ( std::size_t blocksize )
{
    return OS_AllocFromHeap( ARENA_ID, HEAP_ID, blocksize );
}

//----------------------------------------------------------------
//
void* operator new[] ( std::size_t blocksize )
{
    return OS_AllocFromHeap( ARENA_ID, HEAP_ID, blocksize );
}

//----------------------------------------------------------------
//
void operator delete ( void* block ) throw()
{
    OS_FreeToHeap( ARENA_ID, HEAP_ID, block );
}

//----------------------------------------------------------------
//
void operator delete[] ( void* block ) throw()
{
    OS_FreeToHeap( ARENA_ID, HEAP_ID, block );
}

/*---------------------------------------------------------------------------*
  Name:         NitroStartUp

  Description:  startup before NitroMain()
                 - Initialize memory control system for new()

                 FYI:
                 - Startup fuctions called in following order
                          1) NitroStartUp();
                          2) Global/Static Constructors
                          3) NitroMain();

  Arguments:    None

  Returns:      None
 *---------------------------------------------------------------------------*/
void NitroStartUp(void)
{
    void*    arenaLo;
    void*    arenaHi;

    OS_Init();

    arenaLo = OS_GetArenaLo( ARENA_ID );
    arenaHi = OS_GetArenaHi( ARENA_ID );

    // Create a heap
    arenaLo = OS_InitAlloc( ARENA_ID, arenaLo, arenaHi, 1 );
    OS_SetArenaLo( ARENA_ID, arenaLo );

    // Ensure boundaries are 32B aligned
    arenaLo = (void*)ROUND( arenaLo, 32 );
    arenaHi = (void*)ROUND( arenaHi, 32 );

    // The boundaries given to OSCreateHeap should be 32B aligned
    (void)OS_SetCurrentHeap( ARENA_ID, OS_CreateHeap( ARENA_ID, arenaLo, arenaHi ) );

    // From here on out, OS_Alloc and OS_Free behave like malloc and free respectively
    OS_SetArenaLo( ARENA_ID, arenaLo = arenaHi );
}
