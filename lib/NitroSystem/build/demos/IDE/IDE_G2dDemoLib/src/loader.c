/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - IDE - IDE_G2dDemoLib
  File:     loader.c

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/
#include "g2d_demolib.h"
#include <nnsys/g2d/g2d_Softsprite.h>
#include <nnsys/g2d/g2d_Load.h>


/*---------------------------------------------------------------------------*
  Name:         LoadFileToMainMem_

  Description:  バッファを確保してファイルを読み込みます。

  Arguments:    pFname:        読み込むファイル名

  Returns:      読み込みに成功するとファイルの内容を格納しているバッファへの
                ポインタを返します。このバッファは不必要になったときに 
                G2DDemo_Free で開放しなければなりません。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
static void* LoadFileToMainMem_( const char* pFname )
{
    FSFile  file;
    void*   pFile = NULL;
    BOOL    bSuccess;

    SDK_NULL_ASSERT( pFname );

    FS_InitFile( &file );
    bSuccess = FS_OpenFile( &file, pFname );
    
    if( bSuccess )
    {
    	const u32 szFile    = FS_GetLength( &file );


        pFile = G2DDemo_Alloc( szFile );
        SDK_NULL_ASSERT( pFile );

        if( szFile != (u32)FS_ReadFile( &file, pFile, (s32)szFile ) )
        {
            G2DDemo_Free( pFile );
            pFile = NULL;
        }

        bSuccess = FS_CloseFile( &file );
        SDK_ASSERT( bSuccess );
    }else{
        OS_Warning(" Can't find the file : %s ", pFname );
    }

    return pFile;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCER

  Description:  内部でバッファを確保して NCER ファイルからセルデータバンクを
                読み込みます。
                関数成功後、セルデータバンクが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppCellBank: セルデータバンクを格納したバッファへのポインタを
                            受け取るポインタ。
                pFname:     セルデータバンクが格納されているNCERファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCER( NNSG2dCellDataBank** ppCellBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCellBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCellBank( pFile, ppCellBank ) )
        {
            NNS_G2dPrintCellBank( *ppCellBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNANR

  Description:  内部でバッファを確保して NANR ファイルからアニメーション
                データバンクを読み込みます。
                関数成功後、アニメーションデータバンクが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppAnimBank: アニメーションデータバンクを格納したバッファへの
                            ポインタを受け取るポインタ。
                pFname:     アニメーションデータバンクが格納されている
                            NANR ファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNANR( NNSG2dAnimBankData** ppAnimBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppAnimBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedAnimBank( pFile, ppAnimBank ) )
        {
            NNS_G2dPrintAnimBank( *ppAnimBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}
/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNMCR

  Description:  内部でバッファを確保して NMCR ファイルからマルチセルデータ
                バンクを読み込みます。
                関数成功後、マルチセルデータバンクが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppMCBank:   マルチセルデータバンクを格納したバッファへの
                            ポインタを受け取るポインタ。
                pFname:     マルチセルデータバンクが格納されている
                            NMCR ファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNMCR( NNSG2dMultiCellDataBank** ppMCBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppMCBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedMultiCellBank( pFile, ppMCBank ) )
        {
            NNS_G2dPrintMultiCellBank ( *ppMCBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNMAR

  Description:  内部でバッファを確保して NMAR ファイルからマルチセル
                アニメーションデータバンクを読み込みます。
                関数成功後、マルチアニメーションデータバンクが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppMCABank:  マルチセルアニメーションデータバンクを格納した
                            バッファへのポインタを受け取るポインタ。
                pFname:     マルチセルアニメーションデータバンクが
                            格納されている NMAR ファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNMAR( NNSG2dMultiCellAnimBankData** ppMCABank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppMCABank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedMCAnimBank( pFile, ppMCABank ) )
        {
            NNS_G2dPrintAnimBank( *ppMCABank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCGR

  Description:  内部でバッファを確保して NCGR ファイルからキャラクタ形式の
                キャラクタデータを読み込みます。
                関数成功後、キャラクタデータが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppCharData: キャラクタデータを格納したバッファへのポインタを
                            受け取るポインタ。
                pFname:     キャラクタデータが格納されているNCGRファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCGR( NNSG2dCharacterData** ppCharData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCharData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCharacterData( pFile, ppCharData ) )
        {
            NNS_G2dPrintCharacterData( *ppCharData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCGR

  Description:  内部でバッファを確保して NCGR ファイルからキャラクタ形式の
                キャラクタデータを読み込みます。
                関数成功後、キャラクタデータが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppCharData:     キャラクタデータを格納したバッファへのポインタを
                                受け取るポインタ。
                ppCharPosInfo:  キャラクタ読み込み位置データを格納したバッファへのポインタを
                                受け取るポインタ。
                
                pFname:     キャラクタデータが格納されているNCGRファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCGREx
( 
    NNSG2dCharacterData**       ppCharData, 
    NNSG2dCharacterPosInfo**    ppCharPosInfo,
    const char*                 pFname 
)
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCharData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCharacterData( pFile, ppCharData ) )
        {
            NNS_G2dPrintCharacterData( *ppCharData );
            
            if( NNS_G2dGetUnpackedCharacterPosInfo( pFile, ppCharPosInfo ) )
            {
                NNS_G2dPrintCharacterPosInfo( *ppCharPosInfo );   
                return pFile;
            }
        }    
    }
    
    G2DDemo_Free( pFile );
    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCLR

  Description:  内部でバッファを確保して NCLR ファイルからパレットデータを
                読み込みます。
                関数成功後、パレットデータが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppPltData: パレットデータを格納したバッファへのポインタを
                            受け取るポインタ。
                pFname:     パレットデータが格納されているNCLRファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCLR( NNSG2dPaletteData** ppPltData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppPltData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedPaletteData( pFile, ppPltData ) )
        {
            NNS_G2dPrintPaletteData( *ppPltData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNENR

  Description:  内部でバッファを確保して NENR ファイルからエンティティデータ
                バンクを読み込みます。
                関数成功後、エンティティデータバンクが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppEntityBank: エンティティデータバンクを格納したバッファへの
                            ポインタを受け取るポインタ。
                pFname:     エンティティデータバンクが格納されている
                            NENR ファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNENR( NNSG2dEntityDataBank** ppEntityBank, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppEntityBank );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedEntityBank( pFile, ppEntityBank ) )
        {
            NNS_G2dPrintEntityDataBank( *ppEntityBank );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNCBR

  Description:  内部でバッファを確保して NCBR ファイルからビットマップ形式の
                キャラクタデータを読み込みます。
                関数成功後、キャラクタデータが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppCharData: キャラクタデータを格納したバッファへのポインタを
                            受け取るポインタ。
                pFname:     キャラクタデータが格納されているNCBRファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNCBR( NNSG2dCharacterData** ppCharData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppCharData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedCharacterData( pFile, ppCharData ) )
        {
            NNS_G2dPrintCharacterData( *ppCharData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
  Name:         G2DDemo_LoadNSCR

  Description:  内部でバッファを確保して NCBR ファイルからビットマップ形式の
                キャラクタデータを読み込みます。
                関数成功後、キャラクタデータが不要になった時に
                G2DDemo_Free( 返り値 ) でバッファを開放する必要があります。

  Arguments:    ppCharData: キャラクタデータを格納したバッファへのポインタを
                            受け取るポインタ。
                pFname:     キャラクタデータが格納されているNCBRファイル名。

  Returns:      読み込みに成功するとファイルデータが格納されている
                内部で確保したバッファへのポインタを返します。
                読み込みに失敗した場合は NULL を返します。
 *---------------------------------------------------------------------------*/
void* G2DDemo_LoadNSCR( NNSG2dScreenData** ppScreenData, const char* pFname )
{
    void*   pFile = NULL;

    SDK_NULL_ASSERT( ppScreenData );
    SDK_NULL_ASSERT( pFname );

    pFile = LoadFileToMainMem_( pFname );
    if( pFile != NULL )
    {
        if( NNS_G2dGetUnpackedScreenData( pFile, ppScreenData ) )
        {
            NNS_G2dPrintScreenData( *ppScreenData );
            return pFile;
        }

        G2DDemo_Free( pFile );
    }

    return NULL;
}
