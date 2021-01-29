/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g3d
  File:     config.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.24 $
 *---------------------------------------------------------------------------*/

//
// AUTHOR: Kenji Nishida
//

#ifndef NNSG3D_CONFIG_H_
#define NNSG3D_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <nnsys/inline.h>
#define NNS_G3D_INLINE NNS_INLINE

// �A�j���[�V�����o�C�i���̏������֐��ւ̃|�C���^�e�[�u���̃T�C�Y
#ifndef NNS_G3D_ANMFMT_MAX
#define NNS_G3D_ANMFMT_MAX 10
#endif

// �W���C���g�E�}�e���A���E�V�F�C�v�̍ő吔
#ifndef NNS_G3D_SIZE_JNT_MAX
#define NNS_G3D_SIZE_JNT_MAX 64
#endif

#ifndef NNS_G3D_SIZE_MAT_MAX
#define NNS_G3D_SIZE_MAT_MAX 64
#endif

#ifndef NNS_G3D_SIZE_SHP_MAX
#define NNS_G3D_SIZE_SHP_MAX 64
#endif

//
// G3D�����W�I���g���R�}���h�̃o�b�t�@�T�C�Y�i���[�h�j
// �����A�f�t�H���g�ł͊m�ۂ���Ȃ��B
// NNS_G3dGeSetBuffer�Ŋm�ۂł���
//
#ifndef NNS_G3D_SIZE_COMBUFFER
#define NNS_G3D_SIZE_COMBUFFER 192
#endif

//
// �R�����g���O���Ƃ��ꂼ��̃R�[���o�b�N�̗L�����`�F�b�N����
// �R�[�h�������ɂȂ�܂��B
//

//#define NNS_G3D_SBC_CALLBACK_TIMING_A_DISABLE
//#define NNS_G3D_SBC_CALLBACK_TIMING_B_DISABLE
//#define NNS_G3D_SBC_CALLBACK_TIMING_C_DISABLE


//
// �R�����g���O���Ƃ��ꂼ���CG�c�[���p�̃R�[�h�������ɂȂ�܂��B
//

//#define NNS_G3D_MAYA_DISABLE
//#define NNS_G3D_SI3D_DISABLE
//#define NNS_G3D_3DSMAX_DISABLE
//#define NNS_G3D_XSI_DISABLE

//
// �R�����g���O���Ƃ��ꂼ��̃A�j���[�V�����p�̃R�[�h�������ɂȂ�܂��B
//

//#define NNS_G3D_NSBMA_DISABLE
//#define NNS_G3D_NSBTP_DISABLE
//#define NNS_G3D_NSBTA_DISABLE
//#define NNS_G3D_NSBCA_DISABLE
//#define NNS_G3D_NSBVA_DISABLE

// SBC��SHP�R�}���h���Q�Ƃ���f�B�X�p�b�`�e�[�u���̃T�C�Y(������ނ�MAT�o�C�i���t�H�[�}�b�g���T�|�[�g���邽��)
#ifndef NNS_G3D_SIZE_SHP_VTBL_NUM
#define NNS_G3D_SIZE_SHP_VTBL_NUM 4
#endif

// SBC��MAT�R�}���h���Q�Ƃ���f�B�X�p�b�`�e�[�u���̃T�C�Y(������ނ�SHP�o�C�i���t�H�[�}�b�g���T�|�[�g���邽��)
#ifndef NNS_G3D_SIZE_MAT_VTBL_NUM
#define NNS_G3D_SIZE_MAT_VTBL_NUM 4
#endif

// CG�c�[�����̃W���C���g�v�Z���s�����߂̊֐��e�[�u���̃G���g���̐�
#ifndef NNS_G3D_FUNC_SENDJOINTSRT_MAX
#define NNS_G3D_FUNC_SENDJOINTSRT_MAX 3
#endif

// CG�c�[�����̃e�N�X�`���s��v�Z���s�����߂̊֐��e�[�u���̃G���g���̐�
#ifndef NNS_G3D_FUNC_SENDTEXSRT_MAX
#define NNS_G3D_FUNC_SENDTEXSRT_MAX 4
#endif

// �R�����g���O���ƁANNS_G3dGeSendDL�ɂ��f�B�X�v���C���X�g�̓]����
// MI_SendGXCommandAsync�ł͂Ȃ�MI_SendGXCommandAsyncFast���g�p���܂�
//#define NNS_G3D_USE_FASTGXDMA

//
// �E�F�C�e�b�h�G���x���[�v�g�p���ɃL���b�V�����g�p���Ȃ��ꍇ��0���`���Ă���
// �ƃ�������ߖ�ł���B
//
#ifndef NNS_G3D_USE_EVPCACHE
#define NNS_G3D_USE_EVPCACHE 1
#endif


#if defined(NNS_G3D_MAYA_DISABLE) && defined(NNS_G3D_SI3D_DISABLE) && \
    defined(NNS_G3D_3DSMAX_DISABLE) && defined(NNS_G3D_XSI_DISABLE)
#error You cannot disable all of the CG tools for G3D.
#endif

#if (NNS_G3D_FUNC_SENDJOINTSRT_MAX < 3)
#error NNS_G3D_FUNC_SENDJOINTSRT_MAX must be 3 or above.
#endif

#if (NNS_G3D_FUNC_SENDTEXSRT_MAX < 2)
#error NNS_G3D_FUNC_SENDTEXSRT_MAX must be 2 or above.
#endif


#if !defined(NNS_FROM_TOOL)
#include <nitro.h>
#define NNS_G3D_ASSERTMSG     SDK_ASSERTMSG
#define NNS_G3D_ASSERT        SDK_ASSERT
#define NNS_G3D_NULL_ASSERT   SDK_NULL_ASSERT
#define NNS_G3D_WARNING       SDK_WARNING

#else // if !defined(NNS_FROM_TOOL)

#include <nitro_win32.h>
#include <assert.h>
#define NNS_G3D_ASSERTMSG(x, y) assert((x))
#define NNS_G3D_ASSERT(x) assert((x))
#define NNS_G3D_NULL_ASSERT(x) assert(NULL != (x))
//#define NNS_G3D_WARNING(x, ...)

#endif  // if !defined(NNS_FROM_TOOL)

#ifdef __cplusplus
}
#endif

#endif
