/*---------------------------------------------------------------------------*
  Project:  NITRO-System - include - nnsys - g2d
  File:     g2d_CullingUtility.h

  Copyright 2004-2007 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Revision: 1.3 $
 *---------------------------------------------------------------------------*/

#ifndef NNS_G2D_CULLINGUTILITY_H_
#define NNS_G2D_CULLINGUTILITY_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <nitro.h>
#include <nnsys/g2d/fmt/g2d_Vec_data.h>

/*---------------------------------------------------------------------------*
  Name:         NNS_G2dIsInViewCircle

  Description:  ���̈�������肵�܂��i�~�j
                
                
  Arguments:    pvUL:            ���̈� ��`�i����j
                pvSize:          ���̈� ��`�i�T�C�Y�j
                pos:             �Ώە� �ʒu
                boundingR:      ���E�~ ���a      
                
                
  Returns:      ���̈����
  
 *---------------------------------------------------------------------------*/
BOOL NNS_G2dIsInViewCircle( const NNSG2dFVec2* pvUL, const NNSG2dFVec2* pvSize, const NNSG2dFVec2* pos, fx32 boundingR );

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif// NNS_G2D_CULLINGUTILITY_H_
