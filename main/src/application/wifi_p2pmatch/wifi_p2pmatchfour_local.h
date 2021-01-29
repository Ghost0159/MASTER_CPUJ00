#ifndef __WIFI_P2PMATCHFOUR_LOCAL_H__
#define __WIFI_P2PMATCHFOUR_LOCAL_H__

#define WIFI_P2PMATCH_NUM_MAX	(4)	// �ő�S�l�}�b�`���O

enum{
	WFP2PMF_CON_NG,		// �ʐMNG
	WFP2PMF_CON_OK,		// �ʐMOK
	WFP2PMF_CON_LOCK,	// ���e���Z����
};

typedef struct _WFP2P_WK WFP2P_WK;


// �ʐM�f�[�^
typedef struct {
	u16 netID;	// �l�b�gID
	u32 flag;	// WFP2PMF_CON_�`
} WFP2PMF_COMM_RESULT;

// VCHAT�f�[�^
typedef struct {
	u8 vchat_now[WIFI_P2PMATCH_NUM_MAX];
} WFP2PMF_COMM_VCHAT;

extern void WFP2PMF_CommResultRecv( WFP2P_WK* p_wk, const WFP2PMF_COMM_RESULT* cp_data );
extern void WFP2PMF_CommStartRecv( WFP2P_WK* p_wk );
extern void WFP2PMF_CommVchatRecv( WFP2P_WK* p_wk, const WFP2PMF_COMM_VCHAT* cp_data );



#endif		// __WIFI_P2PMATCHFOUR_LOCAL_H__
