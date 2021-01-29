/*---------------------------------------------------------------------------*
  Project:  NitroDWC - auth - util_wifiidtool
  File:     util_wifiidtool.h

  Copyright 2005 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: util_wifiidtool.h,v $
  Revision 1.1  2005/08/30 01:31:23  nakata
  Add util_wifiidtoos.h to $NITRODWC_ROOT/include/bm.

  Revision 1.9  2005/08/22 12:07:13  sasakit
  WiFiID�����ׂ����ǂ����𔻒肷��֐��̒ǉ��B

  Revision 1.8  2005/08/20 09:34:04  akimaru
  ����J�̕�����util_wifiidtool.h�ɕ���

  Revision 1.7  2005/08/20 07:01:19  sasakit
  �w�b�_�C���N���[�h�K�[�h�̏����𓝈ꂵ���B
  bm/dwc_init.h -> bm/dwc_bm_init.h�ɕύX
  �w�b�_��Copyright�������B
  �\���̖̂��O��Ԃ��ł��邾�����[���ɂ����Â����B
  util_wifiidtool.h��dwc_backup.h�̊֌W���኱�C���B


  $NoKeywords: $
 *---------------------------------------------------------------------------*/
#ifndef DWC_UTIL_WIFIIDTOOL_H_
#define DWC_UTIL_WIFIIDTOOL_H_

#include <nitro.h>

#ifdef __cplusplus
extern "C" {
#endif		// __cplusplus


	/*---------------------------------------------------------------------------*
  	Name:        DWC_Auth_Id

	Description:  UserID���i�[����\����
	u64     	   uId:  �F�؂��ꂽ���[�UID
	u64unAttestationID:  �F�؂���Ă��Ȃ����[�UID    			  
	u8		       flg:  �F�؂��ꂽ���ǂ����������t���O 0:�F�؂���Ă��Ȃ��@1:�F�؂���Ă���
   	Returns:	  �Ȃ�
 	*---------------------------------------------------------------------------*/
	typedef struct{
		u64 uId;
		u64 notAttestedId;
		u32  flg;
	}DWCAuthWiFiId;
	
	/*---------------------------------------------------------------------------*
  	Name:         DWC_Auth_GetId

	Description:  DWC_Auth_Id ���擾����
  	Arguments:    id			
	  

   	Returns:	  �Ȃ�
 	*---------------------------------------------------------------------------*/
	void DWC_Auth_GetId( DWCAuthWiFiId* id );

	/*---------------------------------------------------------------------------*
	  Name:         DWC_Auth_CheckPseudoWiFiID
	
	  Description:  �I�t���C���Ő������鉼��WiFiID����������Ă��邩�ǂ������m�F����
	
	  Arguments:    �Ȃ�
	
	  Returns:      TRUE  : ��������Ă�B
	                FALSE : ��������Ă��Ȃ��B
	 *---------------------------------------------------------------------------*/
	BOOL DWC_Auth_CheckPseudoWiFiID( void );

BOOL DWC_Auth_CheckWiFiIDNeedCreate( void );


#ifdef __cplusplus
}
#endif		// __cplusplus


#endif // DWC_UTIL_WIFIIDTOOL_H_
