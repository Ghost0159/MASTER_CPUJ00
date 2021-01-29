/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib - include - g3d_demolib
  File:     camera.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.6 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_G3DDEMO_CAMERA_H_
#define NNS_G3DDEMO_CAMERA_H_

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif


/*---------------------------------------------------------------------------*
	G3DDemoLookAt構造体

 *---------------------------------------------------------------------------*/
typedef struct
{
	VecFx32		camPos;					// カメラの位置(＝視点)
	VecFx32		target;					// カメラの焦点(＝注視点)
	VecFx32		camUp;					// カメラの上方向
	MtxFx43		matrix;

} G3DDemoLookAt;


/*---------------------------------------------------------------------------*
	G3DDemoPersp構造体

 *---------------------------------------------------------------------------*/
typedef struct
{
	fx32		fovySin;			// 視野角/2の正弦をとった値
    fx32		fovyCos;			// 視野角/2の余弦をとった値
    fx32		aspect;				// アスペクト比
    fx32		nearClip;			// 視点からnearクリップ面までの距離
    fx32		farClip;			// 視点からfarクリップ面までの距離

} G3DDemoPersp;


/*---------------------------------------------------------------------------*
	G3DDemoCamera構造体

 *---------------------------------------------------------------------------*/
typedef struct
{
	G3DDemoPersp	persp;
	G3DDemoLookAt	lookat;

	fx32			distance;
	s16				angleX;
	s16				angleY;
	s16				angleZ;
	s16				padding;

} G3DDemoCamera;


/*---------------------------------------------------------------------------*
	関数プロトタイプ

 *---------------------------------------------------------------------------*/

void G3DDemo_InitCamera(G3DDemoCamera* camera, fx32 posy, fx32 distance);
void G3DDemo_CalcCamera(G3DDemoCamera* camera);
void G3DDemo_MoveCamera(G3DDemoCamera* camera);


#ifdef __cplusplus
}/* extern "C" */
#endif

// NNS_G3DDEMO_CAMERA_H_
#endif
