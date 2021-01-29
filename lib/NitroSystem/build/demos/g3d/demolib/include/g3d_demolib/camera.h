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
	G3DDemoLookAt�\����

 *---------------------------------------------------------------------------*/
typedef struct
{
	VecFx32		camPos;					// �J�����̈ʒu(�����_)
	VecFx32		target;					// �J�����̏œ_(�������_)
	VecFx32		camUp;					// �J�����̏����
	MtxFx43		matrix;

} G3DDemoLookAt;


/*---------------------------------------------------------------------------*
	G3DDemoPersp�\����

 *---------------------------------------------------------------------------*/
typedef struct
{
	fx32		fovySin;			// ����p/2�̐������Ƃ����l
    fx32		fovyCos;			// ����p/2�̗]�����Ƃ����l
    fx32		aspect;				// �A�X�y�N�g��
    fx32		nearClip;			// ���_����near�N���b�v�ʂ܂ł̋���
    fx32		farClip;			// ���_����far�N���b�v�ʂ܂ł̋���

} G3DDemoPersp;


/*---------------------------------------------------------------------------*
	G3DDemoCamera�\����

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
	�֐��v���g�^�C�v

 *---------------------------------------------------------------------------*/

void G3DDemo_InitCamera(G3DDemoCamera* camera, fx32 posy, fx32 distance);
void G3DDemo_CalcCamera(G3DDemoCamera* camera);
void G3DDemo_MoveCamera(G3DDemoCamera* camera);


#ifdef __cplusplus
}/* extern "C" */
#endif

// NNS_G3DDEMO_CAMERA_H_
#endif
