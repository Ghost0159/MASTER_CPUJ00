//=============================================================================
/**
 * @file	gf_date.h
 * @brief	年・月・日・曜日のアクセス処理
 * @author	by mori GAME FREAK Inc.
 * @date	2006.05.11
 */
//=============================================================================

#ifndef	__GF_DATE_H__
#define	__GF_DATE_H__
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// 作った理由はNitroSDKのRTCDate構造体が「年・月・日・曜日」の４つのメンバにu32を
// ひとつづつ充てていたからです。最大でも0-99なのでu8で十分。
// inline関数で宣言する事で、使用した人のソースにコードが展開されます。

typedef u32 GF_DATE;

// 年・月・日・
// 曜日をパック（曜日はRTCWeek列挙型をNitroSDKから参照してください　日曜=0）



//------------------------------------------------------------------
/**
 * @brief   年・月・日・曜日のセット(入っている曜日が正しいかはチェックしてません）
 */
//------------------------------------------------------------------
static inline GF_DATE GFDate_Set( u32 year, u32 month, u32 day, int week)
{
	return (year<<24) | ((month&0xff)<<16) | ((day&0xff)<<8) | week;
}

//------------------------------------------------------------------
/**
 * @brief   NitroSDKの構造体からGF_DATEに変換
 */
//------------------------------------------------------------------
static inline GF_DATE RTCDate2GFDate( RTCDate *RTCDate)
{
	return GFDate_Set( RTCDate->year, RTCDate->month, RTCDate->day, RTCDate->week);
}


//------------------------------------------------------------------
/**
 * @brief   年を返す
 */
//------------------------------------------------------------------
static inline u8 GFDate_GetYear( GF_DATE gfDate )
{
	return gfDate>>24;
}

//------------------------------------------------------------------
/**
 * @brief   月を返す
 */
//------------------------------------------------------------------
static inline u8 GFDate_GetMonth( GF_DATE gfDate )
{
	return (gfDate>>16) & 0xff;
}

//------------------------------------------------------------------
/**
 * @brief   日を返す
 */
//------------------------------------------------------------------
static inline u8 GFDate_GetDay( GF_DATE gfDate )
{
	return (gfDate>>8) & 0xff;
}

//------------------------------------------------------------------
/**
 * @brief   曜日を返す（正しいかチェックはしていません）
 */
//------------------------------------------------------------------
static inline u8 GFDate_GetWeek( GF_DATE gfDate )
{
	return gfDate & 0xff;
}

//------------------------------------------------------------------
/**
 * @brief   GF_DATEをNitroSDKの構造体に変換
 */
//------------------------------------------------------------------
static inline void GFDate2RTCDate( GF_DATE src, RTCDate *des )
{
	des->year  = GFDate_GetYear(  src );
	des->month = GFDate_GetMonth( src );
	des->day   = GFDate_GetDay(   src );
	des->week  = GFDate_GetWeek(  src );
	
}


#endif	//__GF_DATE_H__
