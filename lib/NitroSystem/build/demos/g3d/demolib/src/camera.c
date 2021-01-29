/*---------------------------------------------------------------------------*
  Project:  NITRO-System - demos - g3d - demolib
  File:     camera.c

  Copyright 2004-2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.9 $
 *---------------------------------------------------------------------------*/

#include "g3d_demolib/system.h"
#include "g3d_demolib/camera.h"

#define	CAMERA_SPEED  		256

#define	MAX_ROTATION		0x3f00				/* 90deg == 0x4000 */
#define	MIN_ROTATION		(-MAX_ROTATION)

#define	MAX_DISTANCE		(150 * FX32_ONE)
#define	MIN_DISTANCE		(  4 * FX32_ONE)


/*---------------------------------------------------------------------------*
  Name:         G3DDemo_InitCamera

  Description:  Vw3Camera構造体を初期化します。

  Arguments:    camera  : G3DDemoCamera構造体へのポインタ。
				posy    : 注視点の高さ。
				distance: 注視点とカメラの距離。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_InitCamera(G3DDemoCamera* camera, fx32 posy, fx32 distance)
{
	camera->persp.fovySin  = FX32_SIN30;
    camera->persp.fovyCos  = FX32_COS30;
    camera->persp.aspect   = FX32_ONE * 4 / 3;
    camera->persp.nearClip = FX32_ONE;
    camera->persp.farClip  = FX32_ONE * 400;

	camera->lookat.target.x	= 0;
	camera->lookat.target.y	= posy;
	camera->lookat.target.z	= 0;

	camera->angleX   = 256 * 20;
	camera->angleY   = 0;
	camera->angleZ   = 0;
	camera->distance = distance;

	G3DDemo_CalcCamera(camera);
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_CalcCamera

  Description:  極座標値からLookAt用パラメータを計算します。

  Arguments:    camera:	G3DDemoCamera構造体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_CalcCamera(G3DDemoCamera* camera)
{
	G3DDemoLookAt* lookat = &camera->lookat;
	G3DDemoPersp*  persp  = &camera->persp;

	fx16 sinx = FX_SinIdx((u16)camera->angleX);
	fx16 cosx = FX_CosIdx((u16)camera->angleX);
	fx16 siny = FX_SinIdx((u16)camera->angleY);
	fx16 cosy = FX_CosIdx((u16)camera->angleY);
	fx16 sinz = FX_SinIdx((u16)camera->angleZ);
	fx16 cosz = FX_CosIdx((u16)camera->angleZ);

	lookat->camPos.x = cosx * camera->distance >> FX32_SHIFT;
	lookat->camPos.y = sinx * camera->distance >> FX32_SHIFT;
	lookat->camPos.z = cosx * camera->distance >> FX32_SHIFT;

	lookat->camPos.x = lookat->target.x + (lookat->camPos.x * siny >> FX32_SHIFT);
	lookat->camPos.y = lookat->target.y + lookat->camPos.y;
	lookat->camPos.z = lookat->target.z + (lookat->camPos.z * cosy >> FX32_SHIFT);

    lookat->camUp.x =  FX_Mul( sinz, cosy );
    lookat->camUp.y =  cosz;
    lookat->camUp.z = -FX_Mul( sinz, siny );
}

/*---------------------------------------------------------------------------*
  Name:         G3DDemo_MoveCamera

  Description:  カメラをパッドで制御します。カメラ行列は求めません。

  Arguments:    camera:	G3DDemoCamera構造体へのポインタ。

  Returns:      なし。
 *---------------------------------------------------------------------------*/
void
G3DDemo_MoveCamera(G3DDemoCamera* camera)
{
  // カメラ上下

	if (G3DDEMO_IS_PAD_PRESS(PAD_KEY_UP))
	{
    	if ((camera->angleX += CAMERA_SPEED) > MAX_ROTATION)
    	{
			camera->angleX = MAX_ROTATION;
		}
	}
	if (G3DDEMO_IS_PAD_PRESS(PAD_KEY_DOWN))
	{
		if ((camera->angleX -= CAMERA_SPEED) < MIN_ROTATION)
		{
			camera->angleX = MIN_ROTATION;
		}
	}


  // カメラ左右
	if (G3DDEMO_IS_PAD_PRESS(PAD_KEY_LEFT))
	{
		camera->angleY -= CAMERA_SPEED;
	}
	if (G3DDEMO_IS_PAD_PRESS(PAD_KEY_RIGHT))
	{
		camera->angleY += CAMERA_SPEED;
	}
#if 0
  // カメラ傾き
	if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_L))
	{
    	if ((camera->angleZ += CAMERA_SPEED) > MAX_ROTATION)
    	{
			camera->angleZ = MAX_ROTATION;
		}
	}
	if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_R))
	{
		if ((camera->angleZ -= CAMERA_SPEED) < MIN_ROTATION)
		{
			camera->angleZ = MIN_ROTATION;
		}
	}
#endif

	// カメラ距離
	if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_L))
	{
		if ((camera->distance += CAMERA_SPEED) > MAX_DISTANCE)
		{
			camera->distance = MAX_DISTANCE;
		}
	}
	if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_R))
	{
		if ((camera->distance -= CAMERA_SPEED) < MIN_DISTANCE)
		{
			camera->distance = MIN_DISTANCE;
		}
	}

#if 0
	// 注視点の高さ
	if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_Y))
	{
		camera->lookat.target.y -= CAMERA_SPEED;
	}
	if (G3DDEMO_IS_PAD_PRESS(PAD_BUTTON_X))
	{
		camera->lookat.target.y += CAMERA_SPEED;
	}
#endif
}
