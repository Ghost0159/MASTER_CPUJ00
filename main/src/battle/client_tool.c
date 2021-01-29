
//============================================================================================
/**
 * @file	client_tool.c
 * @bfief	�퓬�N���C�A���g�p�c�[���v���O����
 * @author	HisashiSogabe
 * @date	05.07.21
 */
//============================================================================================

#include "common.h"

#include "system/procsys.h"
#include "system/gamedata.h"

#include "battle/battle_server.h"
#include "battle/client.h"
#include "battle/fight.h"
#include "battle/fight_tool.h"
#include "battle/server_tool.h"
#include "battle/client_tool.h"
#include "battle/ground.h"
#include "battle/scio.h"
#include "battle/tr_ai.h"
#include "battle/battle_id.h"
#include "battle/trtype_def.h"
#include "battle/wazano_def.h"

#include "poketool/poke_tool.h"
#include "poketool/tr_tool.h"
#include "poketool/waza_tool.h"
#include "poketool/monsno.h"

#include "gflib/touchpanel.h"

#include "itemtool/item.h"
#include "itemtool/itemsym.h"
#include "itemtool/myitem.h"

#include "system/arc_tool.h"
#include "system/arc_util.h"
#include "system/snd_tool.h"
#include "system/softsprite.h"
#include "system/gra_tool.h"
#include "system/snd_def.h"

#include "wazaeffect/we_sys.h"

#include "client_def.h"
#include "client_tool_def.h"

#include "b_plist.h"
#include "b_bag.h"
#include "gauge.h"
#include "battle/battle_tcb_pri.h"
#include "system/pm_str.h"
#include "system/msg_ds_icon.h"
#include "system/window.h"
#include "system/brightness.h"

#include "battle/graphic/pl_batt_bg_def.h"

#include "wazaeffect/d_we.h"
#include "wazaeffect/d_tool.h"
#include "wazaeffect/ball_effect.h"
#include "wazaeffect/smptm_statuseffect.h"
#include "battle_input.h"

#include "msgdata/msg_fightmsg_dp.h"
#include "msgdata/msg_atkmsg.h"

#include "pokeanime/p_anm_sys.h"

#include "communication/communication.h"
#include "communication/comm_state.h"

#include "battle_snd_def.h"
#include "client_eff.h"

#include "battle/bc_common.h"
#include "temoti_gauge.h"
#include "system/arc_tool.h"
#include "poketool/pokegra/otherpoke.naix"

#ifdef PM_DEBUG
#include "debug/d_battle.h"
#ifdef DEBUG_ONLY_FOR_nishino
#define	BATTLE_SPEED_UP
#endif
#endif PM_DEBUG

#ifdef PM_DEBUG
#ifdef DEBUG_ONLY_FOR_sogabe
#include "src/battle/skill_intp.h"
//#define	FREEZE_BUG_CHECK			//�t���[�Y�o�O����
//#define	FRONTIER_LOOP_ACT				//�t�����e�B�A�Ń��[�v�`�F�b�N
#endif
#endif PM_DEBUG

#define MSG_WINDOW_FADE		(7)		///<�o�b�O��ʁA�|�P�������X�g��ʂɈڍs����Ƃ��̃��b�Z�[�W�E�C���h�E�̃t�F�[�h�l

//#define	POKE_ANM_OFF

#ifdef FRONTIER_LOOP_ACT
extern	u32	ee_loop_count;
#endif


//-----------------------------------------------------------------------------
//					�v���g�^�C�v�錾
//-----------------------------------------------------------------------------

//�Z�V�[�P���X�n
void	CT_EncountEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_PokemonEncountSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_ENCOUNT_PARAM *pep);
void	CT_PokemonEncountAppearSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_APPEAR_PARAM *pap);
void	CT_PokemonAppearSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_APPEAR_PARAM *pap);
void	CT_PokemonReturnSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_RETURN_PARAM *prp);
void	CT_PokemonInhaleSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_INHALE_PARAM *prp);
void	CT_TrainerEncountSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_ENCOUNT_PARAM *tep);
void	CT_TrainerThrowSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_THROW_PARAM *ttp);
void	CT_TrainerOutSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_TrainerInSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_IN_PARAM *tip);
void	CT_HPGaugeInSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_IN_PARAM *hgip);
void	CT_HPGaugeOutSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_CommandSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,COMMAND_SELECT_PARAM *csp);
void	CT_WazaSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_SELECT_PARAM *wsp);
void	CT_DirSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,DIR_SELECT_PARAM *dsp);
void	CT_ItemSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ITEM_SELECT_PARAM *isp);
void	CT_PokemonSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_SELECT_PARAM *psp);
void	CT_YesNoSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,YES_NO_SELECT_PARAM *ynsp);
void	CT_AttackMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ATTACK_MESSAGE_PARAM *amp);
void	CT_MessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *amp);
void	CT_WazaEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_EFFECT_PARAM *wep);
void	CT_PokemonBlinkSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_HPGaugeCalcSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_CALC_PARAM *hgcp);
void	CT_EXPGaugeCalcSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,EXP_GAUGE_CALC_PARAM *egcp);
void	CT_KizetsuEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,KIZETSU_EFFECT_PARAM *kep);
void	CT_SePlaySet(BATTLE_WORK *bw,CLIENT_PARAM *cp,SE_PLAY_PARAM *spp);
void	CT_FadeOutSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_PokemonVanishOnOff(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_VANISH_ON_OFF *pvoo);
void	CT_HPGaugeStatusSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_STATUS_SET_PARAM *hgssp);
void	CT_TrainerMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_MESSAGE_PARAM *tmp);
void	CT_ModosuMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,MODOSU_MESSAGE_PARAM *mmp);
void	CT_KuridasuMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,KURIDASU_MESSAGE_PARAM *kmp);
void	CT_EncountMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_EncountKuridasuMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ENCOUNT_KURIDASU_MESSAGE_PARAM *ekmp);
void	CT_LevelUpEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_AlertMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ALERT_MESSAGE_PARAM *amp);
void	CT_HPGaugeRefreshSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_REFRESH_PARAM *hgrp);
void	CT_WazaWasureSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_WASURE_PARAM *wwp);
void	CT_MosaicSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,MOSAIC_SET_PARAM *msp);
void	CT_BallGaugeEncountSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp);
void	CT_BallGaugeEncountOut(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp);
void	CT_BallGaugeSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp);
void	CT_BallGaugeOut(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp);
void	CT_SioWaitMessage(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_ChrPop(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_EFFECT_PARAM *wep);
void	CT_SStoOAM(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_OAMtoSS(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_WinLoseMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_EscapeMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ESCAPE_MESSAGE_PARAM *emp);
void	CT_GiveUpMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
void	CT_MigawariChrSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_EFFECT_PARAM *wep);
void	CT_WazaKoukaSESet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZAKOUKA_SE_PARAM *wsp);
void	CT_BGMPlay(BATTLE_WORK *bw,CLIENT_PARAM *cp,BGM_PLAY_PARAM *bpp);

//TCB�n
static	void	TCB_PokemonEncount(TCB_PTR tcb,void *work);
static	void	TCB_PokemonEncountGira(TCB_PTR tcb,void *work);
static	void	TCB_PokemonEncountAppear(TCB_PTR tcb,void *work);
static	void	TCB_PokemonAppear(TCB_PTR tcb,void *work);
static	void	TCB_PokemonReturn(TCB_PTR tcb,void *work);
static	void	TCB_PokemonInhale(TCB_PTR tcb,void *work);
static	void	TCB_TrainerEncount(TCB_PTR tcb,void *work);
static	void	TCB_TrainerThrow(TCB_PTR tcb,void *work);
static	void	TCB_TrainerIn(TCB_PTR tcb,void *work);
static	void	TCB_TrainerOut(TCB_PTR tcb,void *work);
static	void	TCB_HPGaugeIn(TCB_PTR tcb,void *work);
static	void	TCB_HPGaugeOut(TCB_PTR tcb,void *work);
static	void	TCB_CommandSelect(TCB_PTR tcb,void *work);
static	void	TCB_CommandSelectAI(TCB_PTR tcb,void *work);
static	void	TCB_CommandSelectSIO(TCB_PTR tcb,void *work);
static	void	TCB_CommandSelectSafari(TCB_PTR tcb,void *work);
static	void	TCB_CommandSelectPokepark(TCB_PTR tcb,void *work);
static	void	TCB_CommandSelectAuto(TCB_PTR tcb,void *work);
static	void	TCB_WazaSelect(TCB_PTR tcb,void *work);
static	void	TCB_WazaSelectAI(TCB_PTR tcb,void *work);
static	void	TCB_WazaSelectSIO(TCB_PTR tcb,void *work);
static	void	TCB_WazaSelectAuto(TCB_PTR tcb,void *work);
static	void	TCB_DirSelect(TCB_PTR tcb,void *work);
static	void	TCB_DirSelectAI(TCB_PTR tcb,void *work);
static	void	TCB_DirSelectSIO(TCB_PTR tcb,void *work);
static	void	TCB_DirSelectAuto(TCB_PTR tcb,void *work);
static	void	TCB_ItemSelect(TCB_PTR tcb,void *work);
static	void	TCB_ItemSelectAI(TCB_PTR tcb,void *work);
static	void	TCB_ItemSelectSIO(TCB_PTR tcb,void *work);
static	void	TCB_ItemSelectAuto(TCB_PTR tcb,void *work);
static	void	TCB_PokemonSelect(TCB_PTR tcb,void *work);
static	void	TCB_PokemonSelectAI(TCB_PTR tcb,void *work);
static	void	TCB_PokemonSelectSIO(TCB_PTR tcb,void *work);
static	void	TCB_PokemonSelectAuto(TCB_PTR tcb,void *work);
static	void	TCB_YesNoSelect(TCB_PTR tcb,void *work);
static	void	TCB_YesNoSelectAI(TCB_PTR tcb,void *work);
static	void	TCB_YesNoSelectSIO(TCB_PTR tcb,void *work);
static	void	TCB_YesNoSelectAuto(TCB_PTR tcb,void *work);
static	void	TCB_WazaEffect(TCB_PTR tcb,void *work);
static	void	TCB_PokemonBlink(TCB_PTR tcb,void *work);
static	void	TCB_HPGaugeCalc(TCB_PTR tcb,void *work);
static	void	TCB_EXPGaugeCalc(TCB_PTR tcb,void *work);
static	void	TCB_KizetsuEffect(TCB_PTR tcb,void *work);
static	void	TCB_FadeOut(TCB_PTR tcb,void *work);
static	void	TCB_PokemonVanishOnOff(TCB_PTR tcb,void *work);
static	void	TCB_LevelUpEffect(TCB_PTR tcb,void *work);

static	void	TCB_MessageWait(TCB_PTR tcb,void *work);
static	void	TCB_AlertMessageWait(TCB_PTR tcb,void *work);

static	void	TCB_EncountEffect(TCB_PTR tcb,void *work);
static	void	TCB_WazaWasure(TCB_PTR tcb,void *work);
static	void	TCB_MosaicSet(TCB_PTR tcb,void *work);

static	void	TCB_BallGaugeEncountSet(TCB_PTR tcb,void *work);
static	void	TCB_BallGaugeEncountOut(TCB_PTR tcb,void *work);

static	void	TCB_SStoOAM(TCB_PTR tcb,void *work);
static	void	TCB_OAMtoSS(TCB_PTR tcb,void *work);

//�O���֐��c�[���n
void			CT_UISet(BATTLE_WORK *bw,CLIENT_PARAM *cp);
u8				CT_CPClientTypeGet(CLIENT_PARAM *cp);
u8				CT_CPClientBootGet(CLIENT_PARAM *cp);
SOFT_SPRITE		*CT_CPSoftSpriteGet(CLIENT_PARAM *cp);
GAUGE_WORK		*CT_CPGaugeWorkGet(CLIENT_PARAM *cp);
CURSOR_SAVE		*CT_CursorSavePtrGet(CLIENT_PARAM *cp);
void			CT_CursorSaveClear(CLIENT_PARAM *cp);
void			Trainer_EnableSet(CLIENT_PARAM *cp, int enable);

//�O������J�c�[���n
static	void		GaugeObjCreate(BATTLE_WORK *bw,int dir,int level,int hp,int hpmax);
static	void		NumCalc(int num,int *bit,int *buf);
static	SOFT_SPRITE	*CT_SoftSpriteAdd(BATTLE_WORK *bw,SOFT_SPRITE_MANAGER *ssm_p,SOFT_SPRITE_ARC *ssa,
									  int pos_x,int pos_y,int pos_z,int ofs_y,int height,int ofx,int shadow,int client_no,
									  SOFT_SPRITE_ANIME *anm_tbl,SOFT_SPRITE_MOVE_FUNC *move);
static	void		CT_ModosuMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MODOSU_MESSAGE_PARAM *mmp,MESSAGE_PARAM *mp);
static	void		CT_KuridasuMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,KURIDASU_MESSAGE_PARAM *kmp,MESSAGE_PARAM *mp);
static	void		CT_EncountMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *mp);
static	void		CT_EncountKuridasuMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,
													   ENCOUNT_KURIDASU_MESSAGE_PARAM *ekmp,MESSAGE_PARAM *mp);
static	void		CT_WinLoseMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *mp);
static	void		CT_EscapeMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,ESCAPE_MESSAGE_PARAM *emp,MESSAGE_PARAM *mp);
static	void		CT_GiveUpMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *mp);
static	CATS_ACT_PTR	CT_TrainerCLACTResourceLoad(BATTLE_WORK *bw,int type,int trtype,int id,s16 pos_x,s16 pos_y);
static	void		CT_WesCall(BATTLE_WORK *bw,CLIENT_PARAM *cp,WE_SYS_PTR wsp,WAZA_EFFECT_PARAM *wep);
static	void		CT_WazaPokeChangeParamMake(BATTLE_WORK *bw,WAZA_EFFECT_PARAM *wep,
											   WAZA_POKE_CHANGE_PARAM *wpcp,int client_no);

static	void		CT_WazaEffectGaugeShadowOnOffCheck(u8 *gauge,u8 *shadow,int eff_type,int eff_no,u16 waza_no);

static	BOOL		CT_TrainerEncountAnimeEffectCheck(TCB_TRAINER_ENCOUNT *tte,CATS_ACT_PTR cap);
static	void		TEAE_WhiteFadeEffect(TCB_PTR tcb,void *work);
static	void		BattleRecDataErrorStop(BATTLE_WORK *bw);
static	void		BattleRecDataFullStop(BATTLE_WORK *bw);

static	u8			CheckRomCodeTrainerTypeChange(BATTLE_WORK *bw,u8 client_no,u8 trtype);

//-----------------------------------------------------------------------------
//					�萔��`
//-----------------------------------------------------------------------------

//#define	SCROLL_TIME				(33)	///(44)	///<�w�i���X�N���[�����鎞��
#define	SCROLL_SPEED			(4)		///(3)		///<�|�P�������G���J�E���g���Ă���Ƃ��̔w�i�X�N���[���̃X�s�[�h
#define	ENCOUNT_SPEED			(8)		///(6)		///<�|�P�������G���J�E���g���Ă���Ƃ��̃X�s�[�h
//#define	ENCOUNT_START_WAIT		(28)	///(56)	///<�G���J�E���g�G�t�F�N�g���N�����Ă���A�n�ʂ̃X�N���[�����n�܂�܂ł̃E�G�C�g
#define	TRAINER_IN_OUT_SPEED	(5)		///<�g���[�i�[�̏o����X�s�[�h

#define	AFF_SPEED				(0x20)	///<�|�P�����o��G�t�F�N�g�̃|�P�������傫���Ȃ�X�s�[�h
#define	BLINK_WAIT				(2)		///<�_���[�W�u�����N�̊Ԋu
#define	TP_WAIT					(10)	///<�^�b�`�p�l���Ƀ^�b�`���Ă��玟�̃V�[�P���X�Ɉڍs����܂ł̃E�G�C�g
#define	ALERT_WAIT				(40)	///<�A���[�g���b�Z�[�W�̕\���E�G�C�g

#define	POKE_APPEAR_NORMAL_VOL	(127)	//�|�P�����o�ꎞ�̖������ʁi�ʏ�j
#define	POKE_APPEAR_BAD_VOL		(127)	//�|�P�����o�ꎞ�̖������ʁi��Ԉُ��AHP�������j
#define	POKE_KIZETSU_VOL		(127)	//�|�P�����C�⎞�̖�������

#define	POKE_VOICE_PAN_FRONT	(WAZA_SE_PAN_R)		//�|�P�������ʖ�����PAN�ݒ�
#define	POKE_VOICE_PAN_BACK		(WAZA_SE_PAN_L)		//�|�P�����w�ʖ�����PAN�ݒ�

///2�̖ڂ͖��������Ȃ��悤�ɏ����x�点�ēo��
#define ENCOUNT_DOUBLE_APPEAR_WAIT		(12)	//25

//-- �ްٖ��̕ω���̎w��F --//
static const u16 BallHikariColor[] = {
	0x0000,			//dummy
	0x7297,			//�}�X�^�[�{�[��,
	0x3fff,			//�n�C�p�[�{�[��,
	0x7af0,			//�X�[�p�[�{�[��,
	0x7adf,			//�����X�^�[�{�[��,
	0x53d7,			//�T�t�@���{�[��,
	0x67f5,			//�l�b�g�{�[��,
	0x7b2c,			//�_�C�u�{�[��,
	0x2b7e,			//�l�X�g�{�[��,
	0x431f,			//���s�[�g�{�[��,
	0x7bdd,			//�^�C�}�[�{�[��,
	0x2a3f,			//�S�[�W���X�{�[��,
	0x293f,			//�v���~�A�{�[��,
	0x45ce,			//�_�[�N�{�[��,
	0x731f,			//�q�[���{�[��,
	0x7f51,			//�N�C�b�N�{�[��,
	0x151e,			//�v���V���X�{�[��,
};

//============================================================================================
/**
 *	�G���J�E���g�G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	pep		�|�P�����G���J�E���g�p�����[�^�\����
 */
//============================================================================================
void	CT_EncountEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_ENCOUNT_EFFECT	*tee;

	tee=(TCB_ENCOUNT_EFFECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_ENCOUNT_EFFECT));
	tee->bw=bw;
	tee->seq_no=0;
	tee->wait=0;
	tee->sub_disp_evy = 0;
//	GF_BGL_ScrollSet(BattleWorkGF_BGL_INIGet(bw),GF_BGL_FRAME3_M,GF_BGL_SCROLL_X_DEC,SCROLL_SPEED*SCROLL_TIME);
#ifdef FRONTIER_LOOP_ACT
	ee_loop_count++;
	OS_TPrintf("EncountEffect OK:%d���\n",ee_loop_count);
#endif
	TCB_Add(TCB_EncountEffect,tee,0);
}

//============================================================================================
/**
 *	�G���J�E���g�|�P�������Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	pep		�|�P�����G���J�E���g�p�����[�^�\����
 */
//============================================================================================
//extern	void	PokeEditDataSet( ARCHANDLE* handle, SOFT_SPRITE_ANIME *ssanm,u16 mons_no );
void	CT_PokemonEncountSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_ENCOUNT_PARAM *pep)
{
	int	col;
	SOFT_SPRITE_ARC	ssa;
	SOFT_SPRITE_MANAGER	*ssm;
	SOFT_SPRITE_ANIME	ssanm[SS_ANM_SEQ_MAX];
	TCB_POKEMON_ENCOUNT	*tpe;
	int	fight_type;
	u8	ofs_y;
	s8	height;
	s8	ofs_x;
	u8	shadow;

#if 0	//���͉�ʂ̏풓���b�Z�[�W���������p 2006.03.14(��) matsuda
		//���Ȃ��̂ŃR�����g�A�E�g
	{
		STRBUF *nick_buf;
		nick_buf = STRBUF_Create(BUFLEN_POKEMON_NAME, HEAPID_BATTLE);
		STRBUF_SetStringCode(nick_buf, pep->nickname);
	}
#endif
	
	ssm=BattleWorkSoftSpriteManagerGet(bw);
	fight_type=BattleWorkFightTypeGet(bw);

	tpe=(TCB_POKEMON_ENCOUNT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_ENCOUNT));
	tpe->seq_no=0;

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		tpe->dir=PARA_FRONT;
		tpe->gw=BattleWorkGroundWorkGet(bw,GROUND_ENEMY);
		CATS_ObjectPosSetCap(tpe->gw->cap,EncountPosTable[cp->client_type&1][0],GROUND_ENEMY_Y);
	}
	else{
		tpe->dir=PARA_BACK;
		tpe->gw=BattleWorkGroundWorkGet(bw,GROUND_MINE);
		CATS_ObjectPosSetCap(tpe->gw->cap,EncountPosTable[cp->client_type&1][0],GROUND_MINE_Y);
	}
	if(pep->rare){
		col=PARA_RARE;
	}
	else{
		col=PARA_NORMAL;
	}
	PokeGraArcDataGet(&ssa,pep->monsno,pep->sex,tpe->dir,col,pep->form_no,pep->personal_rnd);
	ofs_y=PokeHeightGet(pep->monsno,pep->sex,tpe->dir,pep->form_no,pep->personal_rnd);
	
	///< �A�[�J�C�u����@�����@�e�@�I�t�Z�b�g�@�e�@���擾
	PokeEditData_HeightSet( cp->handle, &height, pep->monsno );
	PokeEditData_ShadowOfsXSet( cp->handle, &ofs_x, pep->monsno );
	PokeEditData_ShadowSizeSet( cp->handle, &shadow, pep->monsno );
	PokeEditData_AnimeSet( cp->handle, &ssanm[0], pep->monsno, cp->client_type );
	
	tpe->ss=cp->poke_ss=CT_SoftSpriteAdd(bw,ssm,&ssa,
										 EncountPosTable[cp->client_type][0],
										 EncountPosTable[cp->client_type][1],
										 EncountPosTable[cp->client_type][2],
										 ofs_y,
										 height,
										 ofs_x,
										 shadow,
										 cp->client_no,&ssanm[0],NULL);

	//���ʂ́A�F��ς���
	if(tpe->dir==PARA_FRONT){
		SoftSpritePalFadeSet(tpe->ss,8,8,0,0x0000);
	}
	
	//�j�ꂽ���E�̃M���e�B�i��
	if(tpe->dir == PARA_FRONT 
			&& (BattleWorkBattleStatusFlagGet(bw) & BATTLE_STATUS_FLAG_HAKAI_GIRA_BATTLE)){
		int offset_y, appear_y;
		
		appear_y = SoftSpriteParaGet(tpe->ss,SS_PARA_POS_Y);
		//�n�ʂ̃X���C�h�ƍ��킹��̂�X����{�ɃI�t�Z�b�gY�����߂�
		offset_y = (GROUND_ENEMY_X - ENCOUNT_X_TYPE_BB) / 2;	// /2 = ���x�𔼕��ɂ���̂ŃI�t�Z�b�g�������ɂ���
		//Ground_EnableSet(tpe->gw, CATS_ENABLE_FALSE);					//�n�ʔ�\��
		SoftSpriteParaSet(tpe->ss,SS_PARA_SHADOW_SIZE,SS_SHADOW_NONE);	//�e��\��
		SoftSpriteParaSet(tpe->ss,SS_PARA_POS_X,APPEAR_X_TYPE_BB);		//X�͓����ʒu
		SoftSpriteParaSet(tpe->ss,SS_PARA_POS_Y,appear_y - offset_y);	//Y�͓��㍂��
	
		tpe->appear_pos=appear_y;
	}
	else{
		tpe->appear_pos=AppearPosTable[cp->client_type][POS_APPEAR];
	}
	tpe->bw=bw;
	tpe->cp=cp;
	tpe->command_code=pep->command_code;
	tpe->client_no=cp->client_no;
	tpe->mons_no=pep->monsno;
	tpe->form_no=pep->form_no;
	tpe->voice=pep->voice;
	tpe->client_type=cp->client_type;
	tpe->chr=PokeSeikakuGetRnd(pep->personal_rnd);
	tpe->rare=pep->rare;
	if(tpe->dir == PARA_FRONT 
			&& (BattleWorkBattleStatusFlagGet(bw) & BATTLE_STATUS_FLAG_HAKAI_GIRA_BATTLE)){
		TCB_Add(TCB_PokemonEncountGira,tpe,0);
	}
	else{
		TCB_Add(TCB_PokemonEncount,tpe,0);
	}

	Snd_PMVoiceDoubleFlagSet( 1 );			///< 2�C���̖���
}

//============================================================================================
/**
 *	�|�P�����o���G�t�F�N�g���Z�b�g�i�G���J�E���g����p�j
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	pap		�|�P�����o���G�t�F�N�g�p�����[�^�\����
 */
//============================================================================================
void	CT_PokemonEncountAppearSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_APPEAR_PARAM *pap)
{
	int	col;
	TCB_POKEMON_APPEAR	*tpa;
	int	fight_type;

	fight_type=BattleWorkFightTypeGet(bw);

	tpa=(TCB_POKEMON_APPEAR *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_APPEAR));

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		tpa->dir=PARA_FRONT;
	}
	else{
		tpa->dir=PARA_BACK;
	}
	if(pap->rare){
		col=PARA_RARE;
	}
	else{
		col=PARA_NORMAL;
	}
	PokeGraArcDataGet(&tpa->ssa,pap->monsno,pap->sex,tpa->dir,col,pap->form_no,pap->personal_rnd);
	tpa->ofs_y=PokeHeightGet(pap->monsno,pap->sex,tpa->dir,pap->form_no,pap->personal_rnd);

	PokeEditData_HeightSet( cp->handle, &tpa->height, pap->monsno );
	PokeEditData_ShadowOfsXSet( cp->handle, &tpa->ofs_x, pap->monsno );
	PokeEditData_ShadowSizeSet( cp->handle, &tpa->shadow, pap->monsno );

	//����ւ����̂ŃJ�[�\���ʒu�L����������
	CT_CursorSaveClear(cp);
	
	tpa->bw=bw;
	tpa->cp=cp;
	tpa->seq_no=0;
	tpa->wait = 0;
	tpa->command_code=pap->command_code;
	tpa->client_no=cp->client_no;
	tpa->mons_no=pap->monsno;
	tpa->form_no=pap->form_no;
	tpa->client_type = cp->client_type;
	tpa->voice=pap->voice;
	tpa->sel_mons_no=pap->sel_mons_no;
	tpa->chr=PokeSeikakuGetRnd(pap->personal_rnd);
	tpa->ballID=pap->ballID;
//	tpa->bms=cp->bms;
	tpa->rare=pap->rare;
	tpa->flag=POKEMON_APPEAR_KURIDASI;
	
	Snd_PMVoiceDoubleFlagSet( 1 );			///< 2�C���̖���
	
	TCB_Add(TCB_PokemonEncountAppear,tpa,0);
}

//============================================================================================
/**
 *	�|�P�����o���G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	pap		�|�P�����o���G�t�F�N�g�p�����[�^�\����
 */
//============================================================================================
void	CT_PokemonAppearSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_APPEAR_PARAM *pap)
{
	int	i;
	int	col;
	TCB_POKEMON_APPEAR	*tpa;
	int	fight_type;

	fight_type=BattleWorkFightTypeGet(bw);

	tpa=(TCB_POKEMON_APPEAR *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_APPEAR));
	tpa->seq_no=0;

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		tpa->dir=PARA_FRONT;
	}
	else{
		tpa->dir=PARA_BACK;
	}
	if(pap->rare){
		col=PARA_RARE;
	}
	else{
		col=PARA_NORMAL;
	}
	PokeGraArcDataGet(&tpa->ssa,pap->monsno,pap->sex,tpa->dir,col,pap->form_no,pap->personal_rnd);
	tpa->ofs_y=PokeHeightGet(pap->monsno,pap->sex,tpa->dir,pap->form_no,pap->personal_rnd);

	PokeEditData_HeightSet( cp->handle, &tpa->height, pap->monsno );
	PokeEditData_ShadowOfsXSet( cp->handle, &tpa->ofs_x, pap->monsno );
	PokeEditData_ShadowSizeSet( cp->handle, &tpa->shadow, pap->monsno );

	//����ւ����̂ŃJ�[�\���ʒu�L����������
	CT_CursorSaveClear(cp);
	
	tpa->bw=bw;
	tpa->cp=cp;
	tpa->command_code=pap->command_code;
	tpa->client_no=cp->client_no;
	tpa->mons_no=pap->monsno;
	tpa->form_no=pap->form_no;
	tpa->client_type = cp->client_type;
	tpa->voice=pap->voice;
	tpa->sel_mons_no=pap->sel_mons_no;
	tpa->chr=PokeSeikakuGetRnd(pap->personal_rnd);
	tpa->ballID=pap->ballID;
	tpa->rare=pap->rare;
	tpa->flag=pap->flag;
	tpa->wait = 0;
	tpa->migawari_flag = pap->migawari_flag;

	for(i=0;i<CLIENT_MAX;i++){
		tpa->wep.mons_no[i]			=pap->wep_mons_no[i];
		tpa->wep.sex[i]				=pap->wep_sex[i];
		tpa->wep.rare[i]			=pap->wep_rare[i];
		tpa->wep.form_no[i]			=pap->wep_form_no[i];
		tpa->wep.personal_rnd[i]	=pap->wep_personal_rnd[i];
	}

	TCB_Add(TCB_PokemonAppear,tpa,0);
}

//============================================================================================
/**
 *	�|�P�����������߂�G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_PokemonReturnSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_RETURN_PARAM *prp)
{
	int					i;
	TCB_POKEMON_RETURN	*tpr;

	tpr=(TCB_POKEMON_RETURN *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_RETURN));
	tpr->bw=bw;
	tpr->cp=cp;
	tpr->ss=cp->poke_ss;
	tpr->command_code=prp->command_code;
	tpr->client_no=cp->client_no;
	tpr->client_type=cp->client_type;
	tpr->seq_no=0;
	tpr->height=prp->height;
	tpr->ballID=prp->ballID;
	tpr->migawari_flag=prp->migawari_flag;

	for(i=0;i<CLIENT_MAX;i++){
		tpr->wep.mons_no[i]			=prp->mons_no[i];
		tpr->wep.sex[i]				=prp->sex[i];
		tpr->wep.rare[i]			=prp->rare[i];
		tpr->wep.form_no[i]			=prp->form_no[i];
		tpr->wep.personal_rnd[i]	=prp->personal_rnd[i];
	}

	TCB_Add(TCB_PokemonReturn,tpr,0);
}

//============================================================================================
/**
 *	�|�P�����������߂�G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_PokemonInhaleSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_INHALE_PARAM *pip)
{
	TCB_POKEMON_INHALE	*tpi;

	tpi=(TCB_POKEMON_INHALE *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_INHALE));
	tpi->bw=bw;
	tpi->ss=cp->poke_ss;
	tpi->command_code=pip->command_code;
	tpi->client_no=cp->client_no;
	tpi->client_type=cp->client_type;
	tpi->seq_no=0;
	tpi->height=pip->height;

	SoftSpritePalFadeSet(tpi->ss,0,16,0,BallHikariColor[pip->ballID]);
	SoftSpriteParaSet(tpi->ss,SS_PARA_SHADOW_AFF_REQ,1);

	TCB_Add(TCB_PokemonInhale,tpi,0);
}

//============================================================================================
/**
 *	�G���J�E���g�g���[�i�[���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	tep		�G���J�E���g�g���[�i�[�p�����[�^�\����
 */
//============================================================================================
void	CT_TrainerEncountSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_ENCOUNT_PARAM *tep)
{
	int	col;
	TCB_TRAINER_ENCOUNT	*tte;
	int	type;

	tte=(TCB_TRAINER_ENCOUNT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_TRAINER_ENCOUNT));
	tte->seq_no=0;

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		tte->dir=PARA_FRONT;
		tte->gw=BattleWorkGroundWorkGet(bw,GROUND_ENEMY);
		CATS_ObjectPosSetCap(tte->gw->cap,EncountPosTable[cp->client_type&1][0],GROUND_ENEMY_Y);
	}
	else{
		tte->dir=PARA_BACK;
		tte->gw=BattleWorkGroundWorkGet(bw,GROUND_MINE);
		CATS_ObjectPosSetCap(tte->gw->cap,EncountPosTable[cp->client_type&1][0],GROUND_MINE_Y);
	}
	if((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_MULTI)||
	  ((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_TAG)&&(cp->client_type&CLIENT_ENEMY_FLAG))){
		type=cp->client_type;
	}
	else{
		type=cp->client_type&1;
	}

	tep->trtype=CheckRomCodeTrainerTypeChange(bw,cp->client_no,tep->trtype);

	tte->cap=cp->cap=CT_TrainerCLACTResourceLoad(bw,type,tep->trtype,cp->client_type,
												 EncountPosTable[type][0],EncountPosTable[type][1]);

	tte->appear_pos=AppearPosTable[type][POS_APPEAR];
	tte->bw=bw;
	tte->command_code=tep->command_code;
	tte->client_no=cp->client_no;
	tte->client_type = cp->client_type;
	tte->scroll_time = 0;
	if((tte->client_type==CLIENT_TYPE_AA)||(tte->client_type==CLIENT_TYPE_A)){
		//��ʃX�N���[���ʒu�̏�����
		GF_BGL_ScrollSet(BattleWorkGF_BGL_INIGet(bw),GF_BGL_FRAME3_M,GF_BGL_SCROLL_X_DEC,SCROLL_SPEED*SCROLL_TIME);
	}
	TCB_Add(TCB_TrainerEncount,tte,0);
}

//============================================================================================
/**
 *	�g���[�i�[������G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_TrainerThrowSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_THROW_PARAM *ttp)
{
	TCB_TRAINER_THROW	*ttt;

	ttt=(TCB_TRAINER_THROW *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_TRAINER_THROW));
	ttt->seq_no=0;

	ttt->bw=bw;
	ttt->command_code=ttp->command_code;
	ttt->type=ttp->type;
	ttt->client_no=cp->client_no;
	ttt->cp=cp;
	if(cp->client_type&CLIENT_ENEMY_FLAG){
		ttt->dir=PARA_FRONT;
		ttt->trtype=0;
	}
	else{
		{
			TRAINER_DATA	*trdata;

			trdata=BattleWorkTrainerDataGet(bw,cp->client_no);
			ttt->dir=PARA_BACK;
			ttt->trtype=TrainerBTrTypeGet(CheckRomCodeTrainerTypeChange(bw,cp->client_no,trdata->tr_type));
		}
	}
	TCB_Add(TCB_TrainerThrow,ttt,0);
}

//============================================================================================
/**
 *	�g���[�i�[��ʊO�G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_TrainerOutSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_TRAINER_OUT	*tto;

	tto=(TCB_TRAINER_OUT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_TRAINER_OUT));
	tto->seq_no=0;

	tto->bw=bw;
	tto->command_code=cp->client_buffer[0];
	tto->client_no=cp->client_no;
	tto->cp=cp;
	if(cp->client_type&CLIENT_ENEMY_FLAG){
		tto->dir=PARA_FRONT;
	}
	else{
		tto->dir=PARA_BACK;
	}

	TCB_Add(TCB_TrainerOut,tto,0);
}

//============================================================================================
/**
 *	�g���[�i�[��ʓ��G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_TrainerInSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_IN_PARAM *tip)
{
	int	col;
	SOFT_SPRITE_ARC	ssa;
	SOFT_SPRITE_MANAGER	*ssm;
	TCB_TRAINER_IN	*tti;
	u32	sex;

	ssm=BattleWorkSoftSpriteManagerGet(bw);

	tti=(TCB_TRAINER_IN *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_TRAINER_IN));
	tti->seq_no=0;

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		tti->dir=PARA_FRONT;
	}
	else{
		tti->dir=PARA_BACK;
	}

	tti->cap=cp->cap=CT_TrainerCLACTResourceLoad(bw,cp->client_type&1,tip->trtype,cp->client_type,
												 SlideInPosTable[cp->client_type&1][0],SlideInPosTable[cp->client_type&1][1]);

	tti->appear_pos=AppearPosTable[cp->client_type&1][tip->pos];
	tti->bw=bw;
	tti->command_code=tip->command_code;
	tti->client_no=cp->client_no;
	TCB_Add(TCB_TrainerIn,tti,0);
}

//============================================================================================
/**
 *	HP�Q�[�W���IN�G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_HPGaugeInSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_IN_PARAM *hgip)
{
	GAUGE_WORK *gauge;
	
	gauge = &cp->gauge;
	MI_CpuClearFast(&gauge->param, sizeof(GAUGE_SUB_PARAM));
	gauge->bw = bw;
	gauge->client_no = cp->client_no;
	gauge->type = Gauge_TypeGet(cp->client_type, BattleWorkFightTypeGet(bw));
	gauge->command_code = hgip->command_code;
	gauge->hp = hgip->hp;
	gauge->hpmax = hgip->hpmax;
	gauge->level = hgip->level;
	gauge->sex = hgip->sex;
	gauge->damage = 0;
	gauge->exp = hgip->now_exp;
	gauge->max_exp = hgip->max_exp;
	gauge->sel_mons_no = hgip->sel_mons_no;
	gauge->status = hgip->status;
	gauge->getball = hgip->get_mark;
	gauge->in_wait = hgip->wait;
	gauge->safari_nokori = hgip->safari_ball;
	
	Gauge_EnableSet(gauge, CATS_ENABLE_FALSE);
	Gauge_PartsDraw(gauge, gauge->hp, GAUGE_DRAW_ALL);

	gauge->tcb = TCB_Add(TCB_HPGaugeIn, gauge, TCBPRI_HP_GAUGE_IN);
}

//============================================================================================
/**
 *	HP�Q�[�W���OUT�G�t�F�N�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_HPGaugeOutSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	GAUGE_WORK *gauge;
	
	gauge=&cp->gauge;
	MI_CpuClearFast(&gauge->param, sizeof(GAUGE_SUB_PARAM));
	gauge->bw=bw;
	gauge->client_no=cp->client_no;
	gauge->command_code=cp->client_buffer[0];

	Gauge_ScrollEffectSet(gauge, GAUGE_SCROLL_OUT);
	gauge->tcb = TCB_Add(TCB_HPGaugeOut, gauge, TCBPRI_HP_GAUGE_IN);
}

//============================================================================================
/**
 *	�R�}���h�I�����N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_CommandSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,COMMAND_SELECT_PARAM *csp)
{
	TCB_COMMAND_SELECT	*tcs;
	int					i,j;

	tcs=(TCB_COMMAND_SELECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_COMMAND_SELECT));
	MI_CpuClearFast(tcs, sizeof(TCB_COMMAND_SELECT));
	
	tcs->seq_no=0;
	tcs->wait=0;
	tcs->bw=bw;
	tcs->command_code=csp->command_code;
	tcs->client_no=cp->client_no;
	tcs->client_type = cp->client_type;
	tcs->gauge = &cp->gauge;
	tcs->sel_mons_no = csp->sel_mons_no;
	tcs->icon_hp = csp->icon_hp;
	tcs->icon_hpmax = csp->icon_hpmax;
	tcs->icon_status = csp->icon_status;
	tcs->no_reshuffle_client = csp->no_reshuffle_client;
	
	for(i=0;i<2;i++){
		for(j=0;j<POKEMON_TEMOTI_MAX;j++){
			tcs->status[i][j]=csp->status[i][j];
		}
	}
	
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		if(csp->status[POKEPARTY_MINE][i] == COMSEL_STATUS_DEAD){
			tcs->motivation[i] = 0;	//�m���̎��͗h�炳�Ȃ��悤�Ƀ��`�x�[�V����0
		}
		else{
			tcs->motivation[i] = csp->motivation[i];
		}
	}

#if 0	//�ړ� 2006.06.30(��)
	if(BattleWorkMineEnemyCheck(bw, cp->client_no) == 0){
		//�Z�I����ʂ̕`��𑁂�����ׂɁA�����Ő�ɋZ�ԍ���PP�Ȃǂ�
		//������ăf�[�^���������ɓW�J���Ă��� 2006.05.12(��) by matsuda
		// ���]�䕔����̎x���̌���Ƃ��Ă܂��B
		BINPUT_WAZA_PARAM wazapara;
		int i;
		for(i = 0; i < WAZA_TEMOTI_MAX; i++){
			wazapara.wazano[i] = csp->wazano[i];
			wazapara.pp[i] = csp->pp[i];
			wazapara.ppmax[i] = csp->ppmax[i];
		}
		BINPUT_WazaParaMemoryDecord(BattleWorkGF_BGL_BIPGet(bw), cp->client_type, &wazapara);
	}
#else
	{
		int i;
		for(i = 0; i < WAZA_TEMOTI_MAX; i++){
			tcs->wazano[i] = csp->wazano[i];
			tcs->pp[i] = csp->pp[i];
			tcs->ppmax[i] = csp->ppmax[i];
		}
	}
#endif

	TCB_Add(cp->ui.UI_CommandSelect,tcs,0);
}

//============================================================================================
/**
 *	�Z�I�����N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_WazaSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_SELECT_PARAM *wsp)
{
	TCB_WAZA_SELECT	*tws;
	int i;
	
	tws=(TCB_WAZA_SELECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_WAZA_SELECT));

	tws->seq_no=0;
	tws->bw=bw;
	tws->command_code=cp->client_buffer[0];
	tws->client_no=cp->client_no;
	tws->client_type=cp->client_type;
	tws->gauge = &cp->gauge;
	tws->sel_mons_no = wsp->sel_mons_no;
	for(i = 0; i < WAZA_TEMOTI_MAX; i++){
		tws->wazano[i] = wsp->wazano[i];
		tws->pp[i] = wsp->pp[i];
		tws->ppmax[i] = wsp->ppmax[i];
	}
	tws->wazabit=wsp->wazabit;

	TCB_Add(cp->ui.UI_WazaSelect,tws,0);
}

//============================================================================================
/**
 *	�U���ΏۑI�����N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_DirSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,DIR_SELECT_PARAM *dsp)
{
	TCB_DIR_SELECT	*tds;
	int i,client_set_max;
	u32	fight_type;
	u8	buf[CLIENT_TYPE_MAX];
	
	tds=(TCB_DIR_SELECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_DIR_SELECT));

	tds->seq_no=0;
	tds->bw=bw;
	tds->command_code=cp->client_buffer[0];
	tds->client_no=cp->client_no;
	tds->client_type = cp->client_type;
	tds->range=dsp->range;
	tds->gauge = &cp->gauge;
	tds->light_flag = dsp->flag;
	
	BattleClientTypeBufMake(bw,&buf[0]);
	client_set_max=BattleWorkClientSetMaxGet(bw);
	fight_type=BattleWorkFightTypeGet(bw);
	for(i=0;i<client_set_max;i++){
		tds->dspp[i]=dsp->dspp[i];
	}

	TCB_Add(cp->ui.UI_DirSelect,tds,0);
}


//============================================================================================
/**
 *	����I�����N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_ItemSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ITEM_SELECT_PARAM *isp)
{
	TCB_ITEM_SELECT	*tis;
	int				i,j;

	tis=(TCB_ITEM_SELECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_ITEM_SELECT));
	tis->tps=(TCB_POKEMON_SELECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_SELECT));
	tis->tps->plist=sys_AllocMemory(HEAPID_BATTLE,sizeof(BPLIST_DATA));
	tis->tps->plist->pp=PokeParty_AllocPartyWork(HEAPID_BATTLE);

	tis->seq_no=0;
	tis->bw=bw;
	tis->command_code=isp->command_code;
	tis->client_no=cp->client_no;
	tis->client_type = cp->client_type;
	tis->enc_double = isp->enc_double;
	tis->waza_vanish = isp->waza_vanish;
	tis->waza_migawari = isp->waza_migawari;
	for(i=0;i<CLIENT_MAX;i++){
		tis->tps->sel_mons_no[i]=isp->sel_mons_no[i];
		for(j=0;j<POKEMON_TEMOTI_MAX;j++){
			tis->list_row[i][j]=isp->list_row[i][j];
		}
		tis->shutout_flag[i]=isp->shutout_flag[i];	// �Z�Ŏg�p�s��
	}

	TCB_Add(cp->ui.UI_ItemSelect,tis,0);
}

//============================================================================================
/**
 *	�|�P�����I�����N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_PokemonSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_SELECT_PARAM *psp)
{
	TCB_POKEMON_SELECT	*tps;
	int					i,j;

	tps=(TCB_POKEMON_SELECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_SELECT));

	tps->seq_no=0;
	tps->bw=bw;
	tps->command_code=psp->command_code;
	tps->client_no=psp->client_no;
	tps->list_mode=psp->list_mode;
	tps->chg_waza=psp->chg_waza;
	tps->double_sel=psp->double_sel;
	tps->item=0;
	tps->no_reshuffle_client=psp->no_reshuffle_client;
	for(i=0;i<CLIENT_MAX;i++){
		tps->sel_mons_no[i]=psp->sel_mons_no[i];
		for(j=0;j<POKEMON_TEMOTI_MAX;j++){
			tps->list_row[i][j]=psp->list_row[i][j];
		}
	}

	TCB_Add(cp->ui.UI_PokemonSelect,tps,0);
}

//============================================================================================
/**
 *	Yes/No�I�����N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_YesNoSelectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,YES_NO_SELECT_PARAM *ynsp)
{
	TCB_YES_NO_SELECT	*tyns;

	tyns=(TCB_YES_NO_SELECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_YES_NO_SELECT));

	tyns->seq_no=0;
	tyns->bw=bw;
	tyns->command_code=ynsp->command_code;
	tyns->client_no=cp->client_no;
	tyns->gauge = &cp->gauge;
	tyns->msg_id=ynsp->msg_id;
	tyns->flag=ynsp->flag;
	tyns->waza_no=ynsp->waza_no;
	tyns->nickname=ynsp->nickname;
	
	TCB_Add(cp->ui.UI_YesNoSelect,tyns,0);
}

//============================================================================================
/**
 *	�A�^�b�N���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	amp		�A�^�b�N���b�Z�[�W�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_AttackMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ATTACK_MESSAGE_PARAM *amp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	msg_m=BattleWorkAttackMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=amp->command_code;
	tmw->client_no=cp->client_no;

	mp.msg_id=ATKMSG_M_000+amp->waza_no*3;
	mp.msg_tag=TAG_NICK;
	mp.msg_para[0]=cp->client_no|(amp->sel_mons_no<<8);

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	mp		���b�Z�[�W�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_MessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *mp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=cp->client_buffer[0];
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	�Z�G�t�F�N�g���N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	wep		�Z�G�t�F�N�g�p�p�����[�^���[�N�\����
 */
//============================================================================================
#if 0
void	CT_WazaEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_EFFECT_PARAM *wep)
{
	TCB_WAZA_EFFECT	*twe;
	WES_CALL_BTL_EX_PARAM wcbep;
	int i;
	int	callID;
	
	twe=(TCB_WAZA_EFFECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_WAZA_EFFECT));

	twe->seq_no=0;
	twe->bw=bw;
	twe->command_code=cp->client_buffer[0];
	twe->client_no=cp->client_no;
	twe->wsp=BattleWorkWE_SYS_PTRGet(bw);
	if((wep->flag==EFFTYPE_WAZA)&&((WT_WazaDataParaGet(wep->waza_no,ID_WTD_flag)&FLAG_PUT_GAUGE)==0)){
		twe->flag=1;
	}
	else{
		twe->flag=0;
	}

	/// �퓬�ɓn���g������
	if(wep->flag==EFFTYPE_WAZA){
		wcbep.eff_arc_id = ARC_WAZA_EFF;
		callID=wep->waza_no;
	}
	else{
		wcbep.eff_arc_id = ARC_WAZA_EFF_SUB;
		callID=wep->data;
	}
	wcbep.bgl  = BattleWorkGF_BGL_INIGet(bw);					///< BGL*
	wcbep.pfd  = BattleWorkPfdGet(bw);							///< PFD*
	wcbep.csp  = BattleWorkCATS_SYS_PTRGet(bw);					///< CSP*
	for(i = 0; i < CLIENT_MAX; i++){
		wcbep.excp[i] = BattleWorkExChrClientParamGet(bw, i);
		wcbep.mons_no[i] = wep->mons_no[i];
		wcbep.sex[i] = wep->sex[i];
		wcbep.rare[i] = wep->rare[i];
		wcbep.form_no[i] = wep->form_no[i];
		wcbep.personal_rnd[i] = wep->personal_rnd[i];
	}
	BattleClientTypeBufMake(bw, &(wcbep.client_type[0]));			///< �N���C�A���gType
	BattleSoftSpriteBufMake(bw, &(wcbep.ss[0]));					///< �\�t�g�X�v���C�g
	wcbep.fight_type = BattleWorkFightTypeGet(bw);					///< �t�@�C�g�^�C�v�擾
	wcbep.perap_voice = BattleWorkPerapVoiceGet(bw, cp->client_no);	///< �y���b�v
	wcbep.bg_area = BattleWorkBGAreaGet(bw);						///< BG�G���A
	wcbep.pal_area = BattleWorkPalAreaGet(bw);						///< PAL�G���A
	
	///< �w�i�f�[�^
	wcbep.haikei_data.arc_index = ARC_BATT_BG;
	wcbep.haikei_data.chr_id	= BATTLE_BG00_NCGR_BIN + BattleWorkBGIDGet(bw);
	wcbep.haikei_data.pal_id	= BATT_BG00_D_NCLR + BattleWorkBGIDGet(bw)*3;
	wcbep.haikei_data.scr_id	= BATTLE_BG00_NSCR_BIN;
	wcbep.haikei_data.pal_start	= 0;
	wcbep.haikei_data.pal_num	= 8;
	
	WES_Call_Ex(twe->wsp, wep, callID, &wcbep);

	TCB_Add(TCB_WazaEffect,twe,0);
}
#else
void	CT_WazaEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_EFFECT_PARAM *wep)
{
	TCB_WAZA_EFFECT	*twe;
	
	twe=(TCB_WAZA_EFFECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_WAZA_EFFECT));

	twe->seq_no=0;
	twe->bw=bw;
	twe->cp=cp;
	twe->command_code=cp->client_buffer[0];
	twe->client_no=cp->client_no;
	twe->wsp=BattleWorkWE_SYS_PTRGet(bw);
	twe->wep=*wep;
	twe->ss=cp->poke_ss;

	if((wep->flag==EFFTYPE_STATUS)&&(wep->data==STATUS_ACTION_DOLL_ON)){
		cp->migawari_flag=1;
	}
	if((wep->flag==EFFTYPE_STATUS)&&(wep->data==STATUS_ACTION_DOLL_OFF)){
		cp->migawari_flag=0;
	}

	CT_WazaEffectGaugeShadowOnOffCheck(&twe->we_on_off_flag,&twe->we_shadow_flag,wep->flag,wep->data,wep->waza_no);

	TCB_Add(TCB_WazaEffect,twe,0);
}
#endif

//============================================================================================
/**
 *	�|�P�����_���[�W�u�����N�A�N�V�������N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_PokemonBlinkSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_POKEMON_BLINK	*tpb;

	tpb=(TCB_POKEMON_BLINK *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_BLINK));

	tpb->seq_no=0;
	tpb->bw=bw;
	tpb->ss=cp->poke_ss;
	tpb->client_no=cp->client_no;
	tpb->wait=0;

	TCB_Add(TCB_PokemonBlink,tpb,0);

}

//============================================================================================
/**
 *	HP�Q�[�W�v�Z���N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_HPGaugeCalcSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_CALC_PARAM *hgcp)
{
	GAUGE_WORK *gauge;
	
	GF_ASSERT(cp->gauge.cap != NULL);
	
	gauge = &cp->gauge;
	MI_CpuClear8(&gauge->param, sizeof(GAUGE_SUB_PARAM));
	gauge->bw = bw;
	gauge->command_code = hgcp->command_code;
	gauge->client_no = cp->client_no;
	gauge->type = Gauge_TypeGet(cp->client_type, BattleWorkFightTypeGet(bw));
	gauge->hp = hgcp->hp;
	gauge->hpmax = hgcp->hpmax;
	gauge->damage = hgcp->damage;
	gauge->level = hgcp->level;
	
	if(hgcp->damage == JIBAKU_HP){
		gauge->hp = 0;
		gauge->damage = 0;
	}
	
	gauge->tcb = TCB_Add(TCB_HPGaugeCalc, gauge, TCBPRI_HP_GAUGE_CALC);
}

//============================================================================================
/**
 *	EXP�Q�[�W�v�Z���N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_EXPGaugeCalcSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,EXP_GAUGE_CALC_PARAM *egcp)
{
	GAUGE_WORK *gauge;
	
	GF_ASSERT(cp->gauge.cap != NULL);
	
	gauge = &cp->gauge;
	MI_CpuClear8(&gauge->param, sizeof(GAUGE_SUB_PARAM));
	gauge->bw = bw;
	gauge->command_code = egcp->command_code;
	gauge->client_no = cp->client_no;

	gauge->exp = egcp->now_exp;
	gauge->max_exp = egcp->max_exp;
	gauge->add_exp = egcp->next_exp - gauge->exp;
	
	if(cp->client_type == CLIENT_TYPE_AA){
		gauge->tcb = TCB_Add(TCB_EXPGaugeCalc, gauge, TCBPRI_HP_GAUGE_CALC);
		return;
	}
	else{
		SCIO_ServerQueClear(gauge->bw, gauge->client_no, gauge->command_code);
	}
}

//============================================================================================
/**
 *	�C��G�t�F�N�g���N��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_KizetsuEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,KIZETSU_EFFECT_PARAM *kep)
{
	TCB_KIZETSU_EFFECT	*tke;
	int					i;

	tke=(TCB_KIZETSU_EFFECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_KIZETSU_EFFECT));

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		tke->dir=PARA_FRONT;
	}
	else{
		tke->dir=PARA_BACK;
	}

	tke->seq_no=0;
	tke->bw=bw;
	tke->cp=cp;
	tke->command_code=kep->command_code;
	tke->client_no=cp->client_no;
	tke->ss=cp->poke_ss;
	tke->monsno=kep->monsno;
	tke->sex=kep->sex;
	tke->form_no=kep->form_no;
	tke->personal_rnd=kep->personal_rnd;
	tke->migawari_flag=kep->migawari_flag;
	tke->henshin_flag=kep->henshin_flag;

	for(i=0;i<CLIENT_MAX;i++){
		tke->wep.mons_no[i]			=kep->wep_mons_no[i];
		tke->wep.sex[i]				=kep->wep_sex[i];
		tke->wep.rare[i]			=kep->wep_rare[i];
		tke->wep.form_no[i]			=kep->wep_form_no[i];
		tke->wep.personal_rnd[i]	=kep->wep_personal_rnd[i];
	}

	TCB_Add(TCB_KizetsuEffect,tke,0);
}

//============================================================================================
/**
 *	�w�肳�ꂽSE��炷
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_SePlaySet(BATTLE_WORK *bw,CLIENT_PARAM *cp,SE_PLAY_PARAM *spp)
{
	int	pan;

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		pan=WAZA_SE_PAN_R;
	}
	else{
		pan=WAZA_SE_PAN_L;
	}

	SCIO_ServerQueClear(bw,cp->client_no,spp->command_code);
	Snd_SePlayPan(spp->se_no,pan);
}

//============================================================================================
/**
 *	��ʃt�F�[�h�A�E�g���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_FadeOutSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_FADE_OUT	*tfo;

	tfo=(TCB_FADE_OUT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_FADE_OUT));

	tfo->seq_no=0;
	tfo->bw=bw;
	tfo->command_code=cp->client_buffer[0];
	tfo->client_no=cp->client_no;

	TCB_Add(TCB_FadeOut,tfo,0);
}

//============================================================================================
/**
 *	�|�P�����̃o�j�b�V���t���O�𐧌�
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 * @param[in]	pvoo	�o�j�b�V���t���O�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_PokemonVanishOnOff(BATTLE_WORK *bw,CLIENT_PARAM *cp,POKEMON_VANISH_ON_OFF *pvoo)
{
	TCB_POKEMON_VANISH_ON_OFF	*tpvoo;
	int							i;

	tpvoo=(TCB_POKEMON_VANISH_ON_OFF *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_POKEMON_VANISH_ON_OFF));

	tpvoo->bw=bw;
	tpvoo->cp=cp;
	tpvoo->command_code=pvoo->command_code;
	tpvoo->client_no=cp->client_no;
	tpvoo->seq_no=0;
	tpvoo->sw=pvoo->sw;
	tpvoo->migawari_flag=pvoo->migawari_flag;

	for(i=0;i<CLIENT_MAX;i++){
		tpvoo->wep.mons_no[i]		=pvoo->wep_mons_no[i];
		tpvoo->wep.sex[i]			=pvoo->wep_sex[i];
		tpvoo->wep.rare[i]			=pvoo->wep_rare[i];
		tpvoo->wep.form_no[i]		=pvoo->wep_form_no[i];
		tpvoo->wep.personal_rnd[i]	=pvoo->wep_personal_rnd[i];
	}

	TCB_Add(TCB_PokemonVanishOnOff,tpvoo,0);
}

//============================================================================================
/**
 *	HP�Q�[�W�ɏ�Ԉُ�A�C�R�����Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_HPGaugeStatusSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_STATUS_SET_PARAM *hgssp)
{
	GF_ASSERT(cp->gauge.cap != NULL);
	
	cp->gauge.status = hgssp->status;
	Gauge_PartsDraw(&cp->gauge, cp->gauge.hp, GAUGE_DRAW_STATUS);
	SCIO_ServerQueClear(bw,cp->client_no,hgssp->command_code);
}

//============================================================================================
/**
 *	�g���[�i�[���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_TrainerMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,TRAINER_MESSAGE_PARAM *tmp)
{
	TCB_MESSAGE_WAIT	*tmw;
	int					tr_id;

	tr_id=BattleWorkTrainerIDGet(bw,cp->client_no);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=tmp->command_code;
	tmw->client_no=cp->client_no;

	tmw->msg_index=TrainerMSG_Print(bw,tr_id,cp->client_no,tmp->type,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	�߂����b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_ModosuMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,MODOSU_MESSAGE_PARAM *mmp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	CT_ModosuMessageParamMake(bw,cp,mmp,&mp);

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=mmp->command_code;
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	�J��o�����b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_KuridasuMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,KURIDASU_MESSAGE_PARAM *kmp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	CT_KuridasuMessageParamMake(bw,cp,kmp,&mp);

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=kmp->command_code;
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	�G���J�E���g���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_EncountMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	CT_EncountMessageParamMake(bw,cp,&mp);

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=CL_ENCOUNT_MESSAGE;
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	�G���J�E���g���̌J��o�����b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_EncountKuridasuMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ENCOUNT_KURIDASU_MESSAGE_PARAM *ekmp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	CT_EncountKuridasuMessageParamMake(bw,cp,ekmp,&mp);

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=ekmp->command_code;
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	���x���A�b�v�G�t�F�N�g��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_LevelUpEffectSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_LEVEL_UP_EFFECT	*tlue;

	tlue=(TCB_LEVEL_UP_EFFECT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_LEVEL_UP_EFFECT));

	tlue->bw=bw;
	tlue->command_code=cp->client_buffer[0];
	tlue->client_no=cp->client_no;
	tlue->seq_no=0;
	tlue->gauge=&cp->gauge;

	TCB_Add(TCB_LevelUpEffect,tlue,0);
}

//============================================================================================
/**
 *	�A���[�g���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_AlertMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ALERT_MESSAGE_PARAM *amp)
{
	TCB_ALERT_MESSAGE_WAIT	*tamw;
	MSGDATA_MANAGER			*msg_m;

	//�A���[�g���b�Z�[�W�͎�������̂ݕ\��
	if(cp->client_boot==CLIENT_BOOT_NORMAL){
		msg_m=BattleWorkFightMsgGet(bw);

		tamw=(TCB_ALERT_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_ALERT_MESSAGE_WAIT));

		tamw->bw=bw;
		tamw->command_code=amp->command_code;
		tamw->client_no=cp->client_no;
		tamw->seq_no=0;

		tamw->msg_index=BattleMSG_Print(bw,msg_m,&amp->mp,BattleWorkConfigMsgSpeedGet(bw));

		TCB_Add(TCB_AlertMessageWait,tamw,0);
	}
	//AI�́A�Ԃ�l�̂�
	else if(cp->client_boot==CLIENT_BOOT_AI){
		SCIO_AlertMessageEndSet(bw,cp->client_no);
		SCIO_ServerQueClear(bw,cp->client_no,amp->command_code);
	}
	else{
		//�ʐM�ΐ펞�́A�Ԃ�l�͂Ȃ�
		if((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_SIO)==0){
			SCIO_AlertMessageEndSet(bw,cp->client_no);
		}
		SCIO_ServerQueClear(bw,cp->client_no,amp->command_code);
	}
}

//============================================================================================
/**
 *	HP�Q�[�W���ĕ`��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_HPGaugeRefreshSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,HP_GAUGE_REFRESH_PARAM *hgrp)
{
	GAUGE_WORK *gauge;
	
	gauge = &cp->gauge;
	MI_CpuClearFast(&gauge->param, sizeof(GAUGE_SUB_PARAM));
	gauge->bw = bw;
	gauge->client_no = cp->client_no;
	gauge->type = Gauge_TypeGet(cp->client_type, BattleWorkFightTypeGet(bw));
	gauge->command_code = hgrp->command_code;
	gauge->hp = hgrp->hp;
	gauge->hpmax = hgrp->hpmax;
	gauge->level = hgrp->level;
	gauge->sex = hgrp->sex;
	gauge->damage = 0;
	gauge->exp = hgrp->now_exp;
	gauge->max_exp = hgrp->max_exp;
	gauge->sel_mons_no = hgrp->sel_mons_no;
	gauge->status = hgrp->status;
	gauge->getball = hgrp->get_mark;
	gauge->safari_nokori = hgrp->safari_ball;
	
	Gauge_PartsDraw(gauge, gauge->hp, GAUGE_DRAW_ALL ^ GAUGE_DRAW_EXP);

	SCIO_ServerQueClear(gauge->bw, gauge->client_no, gauge->command_code);
}

//============================================================================================
/**
 *	�Z�Y�ꏈ�����Ăяo��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_WazaWasureSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_WASURE_PARAM *wwp)
{
	TCB_WAZA_WASURE	*tww;

	tww=(TCB_WAZA_WASURE *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_WAZA_WASURE));

	tww->seq_no=0;
	tww->bw=bw;
	tww->command_code=wwp->command_code;
	tww->client_no=cp->client_no;
	tww->waza_no=wwp->waza_no;
	tww->sel_mons_no=wwp->sel_mons_no;

	TCB_Add(TCB_WazaWasure,tww,0);
}

//============================================================================================
/**
 *	�\�t�g�E�G�A�X�v���C�g�Ƀ��U�C�N��������
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_MosaicSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,MOSAIC_SET_PARAM *msp)
{
	TCB_MOSAIC_SET	*tms;

	tms=(TCB_MOSAIC_SET *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MOSAIC_SET));

	tms->seq_no=0;
	tms->bw=bw;
	tms->ss=cp->poke_ss;
	tms->command_code=msp->command_code;
	tms->client_no=cp->client_no;
	tms->mosaic=msp->mosaic;
	tms->wait=0;
	tms->wait_temp=msp->wait;

	TCB_Add(TCB_MosaicSet,tms,0);
}

//============================================================================================
/**
 *	�{�[���Q�[�W��\���i�G���J�E���g���j
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_BallGaugeEncountSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp)
{
#if 0
	SCIO_ServerQueClear(bw,cp->client_no,bgp->command_code);
#else
	TCB_BALL_GAUGE	*tbg;
	int i;
	
	tbg=(TCB_BALL_GAUGE *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_BALL_GAUGE));

	tbg->seq_no=0;
	tbg->bw=bw;
	tbg->command_code=bgp->command_code;
	tbg->client_no=cp->client_no;
	tbg->client_type=cp->client_type;
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		tbg->status[i] = bgp->status[i];
	}
	
	tbg->encount_change = 0;
	TCB_Add(TCB_BallGaugeEncountSet,tbg,0);
#endif
}

//============================================================================================
/**
 *	�{�[���Q�[�W�������i�G���J�E���g���j
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_BallGaugeEncountOut(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp)
{
#if 0
	SCIO_ServerQueClear(bw,cp->client_no,bgp->command_code);
#else
	TCB_BALL_GAUGE	*tbg;
	int i;
	
	tbg=(TCB_BALL_GAUGE *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_BALL_GAUGE));

	tbg->seq_no=0;
	tbg->bw=bw;
	tbg->command_code=bgp->command_code;
	tbg->client_no=cp->client_no;
	tbg->client_type=cp->client_type;

	//OUT�ɂ͕s�v
//	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
//		tbg->status[i] = bgp->status[i];
//	}
	
	tbg->encount_change = 0;
	TCB_Add(TCB_BallGaugeEncountOut,tbg,0);
#endif
}

//============================================================================================
/**
 *	�{�[���Q�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_BallGaugeSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp)
{
	TCB_BALL_GAUGE	*tbg;
	int i;
	
	tbg=(TCB_BALL_GAUGE *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_BALL_GAUGE));

	tbg->seq_no=0;
	tbg->bw=bw;
	tbg->command_code=bgp->command_code;
	tbg->client_no=cp->client_no;
	tbg->client_type=cp->client_type;
	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
		tbg->status[i] = bgp->status[i];
	}
		
	tbg->encount_change = 1;
	TCB_Add(TCB_BallGaugeEncountSet,tbg,0);
}

//============================================================================================
/**
 *	�{�[���Q�[�W������
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_BallGaugeOut(BATTLE_WORK *bw,CLIENT_PARAM *cp,BALL_GAUGE_PARAM *bgp)
{
#if 0
	SCIO_ServerQueClear(bw,cp->client_no,bgp->command_code);
#else
	TCB_BALL_GAUGE	*tbg;
	int i;
	
	tbg=(TCB_BALL_GAUGE *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_BALL_GAUGE));

	tbg->seq_no=0;
	tbg->bw=bw;
	tbg->command_code=bgp->command_code;
	tbg->client_no=cp->client_no;
	tbg->client_type=cp->client_type;

	//OUT�ɂ͕s�v
//	for(i = 0; i < POKEMON_TEMOTI_MAX; i++){
//		tbg->status[i] = bgp->status[i];
//	}
	
	tbg->encount_change = 1;
	TCB_Add(TCB_BallGaugeEncountOut,tbg,0);
#endif
}

//============================================================================================
/**
 *	�ʐM�ҋ@���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_SioWaitMessage(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	MSGDATA_MANAGER			*msg_m;
	MESSAGE_PARAM			mp;

	//�ʐM�ҋ@���b�Z�[�W�͎�������̂ݕ\��
	if(cp->client_boot==CLIENT_BOOT_NORMAL){
		msg_m=BattleWorkFightMsgGet(bw);

		mp.msg_id=TaikiMsg;
		mp.msg_tag=TAG_NONE;

		BattleMSG_Print(bw,msg_m,&mp,NULL);

		BattleWorkTimeIconSet(bw,TimeWaitIconAdd(BattleWorkGF_BGL_BMPWINGet(bw,TALK_WIN_NO),TALK_WIN_CGX_START));
	}
	SCIO_ServerQueClear(bw,cp->client_no,CL_SIO_WAIT_MESSAGE);
}

//============================================================================================
/**
 *	�ޔ����Ă����L�����𕜋A
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_ChrPop(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_EFFECT_PARAM *wep)
{
	WAZA_POKE_CHANGE_PARAM	wpcp;

	CT_WazaPokeChangeParamMake(bw,wep,&wpcp,cp->client_no);
	HenshinChrPop(&wpcp,HEAPID_BATTLE);

	SCIO_ServerQueClear(bw,cp->client_no,wep->command_code);
}

//--------------------------------------------------------------
/**
 * @brief   �\�t�g�X�v���C�g��OAM�ɕϊ�
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//--------------------------------------------------------------
void	CT_SStoOAM(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_SS_TO_OAM	*tsto;
	
	tsto=(TCB_SS_TO_OAM *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_SS_TO_OAM));

	tsto->seq_no=0;
	tsto->bw=bw;
	tsto->cp=cp;
	tsto->command_code=cp->client_buffer[0];
	tsto->client_no=cp->client_no;
	tsto->ss=cp->poke_ss;

	TCB_Add(TCB_SStoOAM,tsto,0);
}

//--------------------------------------------------------------
/**
 * @brief   OAM���\�t�g�X�v���C�g�ɖ߂�
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//--------------------------------------------------------------
void	CT_OAMtoSS(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_OAM_TO_SS	*tots;
	
	tots=(TCB_OAM_TO_SS *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_OAM_TO_SS));

	tots->seq_no=0;
	tots->bw=bw;
	tots->cp=cp;
	tots->command_code=cp->client_buffer[0];
	tots->client_no=cp->client_no;
	tots->ss=cp->poke_ss;

	TCB_Add(TCB_OAMtoSS,tots,0);
}

//--------------------------------------------------------------
/**
 * @brief   �ʐM�ΐ펞�̏��s���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//--------------------------------------------------------------
void	CT_WinLoseMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	CT_WinLoseMessageParamMake(bw,cp,&mp);

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=CL_WIN_LOSE_MESSAGE;
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//--------------------------------------------------------------
/**
 * @brief   �ʐM�ΐ펞�̂ɂ��郁�b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//--------------------------------------------------------------
void	CT_EscapeMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,ESCAPE_MESSAGE_PARAM *emp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	CT_EscapeMessageParamMake(bw,cp,emp,&mp);

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=CL_ESCAPE_MESSAGE;
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//--------------------------------------------------------------
/**
 * @brief   �o�g���^���[�̃��^�C�A���b�Z�[�W��\��
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//--------------------------------------------------------------
void	CT_GiveUpMessageSet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	TCB_MESSAGE_WAIT	*tmw;
	MSGDATA_MANAGER		*msg_m;
	MESSAGE_PARAM		mp;

	CT_GiveUpMessageParamMake(bw,cp,&mp);

	msg_m=BattleWorkFightMsgGet(bw);

	tmw=(TCB_MESSAGE_WAIT *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TCB_MESSAGE_WAIT));

	tmw->bw=bw;
	tmw->command_code=CL_GIVEUP_MESSAGE;
	tmw->client_no=cp->client_no;

	tmw->msg_index=BattleMSG_Print(bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(bw));

	TCB_Add(TCB_MessageWait,tmw,0);
}

//============================================================================================
/**
 *	�݂����L�������Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_MigawariChrSet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZA_EFFECT_PARAM *wep)
{
	WAZA_POKE_CHANGE_PARAM	wpcp;

	CT_WazaPokeChangeParamMake(bw,wep,&wpcp,cp->client_no);
	MigawariChrSet(&wpcp,HEAPID_BATTLE);

	SCIO_ServerQueClear(bw,cp->client_no,wep->command_code);
}

//============================================================================================
/**
 *	�Z����SE���Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_WazaKoukaSESet(BATTLE_WORK *bw,CLIENT_PARAM *cp,WAZAKOUKA_SE_PARAM *wsp)
{
	int	pan;

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		pan=WAZA_SE_PAN_R;
	}
	else{
		pan=WAZA_SE_PAN_L;
	}

	switch(wsp->kouka){
	case WAZA_HIT_NORMAL:
		Snd_SePlayPan(BSE_HIT_NORMAL,pan);
		break;
	case WAZA_HIT_BATSUGUN:
		Snd_SePlayPan(BSE_HIT_BATSUGUN,pan);
		break;
	case WAZA_HIT_IMAHITOTSU:
		Snd_SePlayPan(BSE_HIT_IMAHITOTSU,pan);
		break;
	}

	SCIO_ServerQueClear(bw,cp->client_no,wsp->command_code);
}

//============================================================================================
/**
 *	�Ȃ��Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 * @param[in]	cp		�N���C�A���g�p�p�����[�^���[�N�\����
 */
//============================================================================================
void	CT_BGMPlay(BATTLE_WORK *bw,CLIENT_PARAM *cp,BGM_PLAY_PARAM *bpp)
{
	Snd_BgmPlay(bpp->bgm_no);

	SCIO_ServerQueClear(bw,cp->client_no,bpp->command_code);
}

//-----------------------------------------------------------------------------
//		TCB�T�u���[�`���Q
//-----------------------------------------------------------------------------
//============================================================================================
/**
 *	�G���J�E���g�|�P��������V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonEncount(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_ENCOUNT	*tpe=(TCB_POKEMON_ENCOUNT *)work;
	WE_SYS_PTR			wsp=BattleWorkWE_SYS_PTRGet(tpe->bw);
	s16					x,y;

	switch(tpe->seq_no){
	case 0:
		tpe->wait=ENCOUNT_START_WAIT;
		tpe->seq_no++;
	case 1:
		if(--tpe->wait){
			break;
		}
		tpe->seq_no++;
	case 2:
		if(tpe->dir==PARA_FRONT){
			CATS_ObjectPosGetCap(tpe->gw->cap,&x,&y);
			if((tpe->client_type==CLIENT_TYPE_BB)||(tpe->client_type==CLIENT_TYPE_B)){
				if(x<GROUND_ENEMY_X){
					CATS_ObjectPosMoveCap(tpe->gw->cap,ENCOUNT_SPEED,0);
				}
				else{
					CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_ENEMY_X,GROUND_ENEMY_Y);
				}
			}
			CATS_ObjectPosGetCap(tpe->gw->cap,&x,&y);
			if(tpe->client_type==CLIENT_TYPE_BB){
				SoftSpriteParaSet(tpe->ss,SS_PARA_POS_X,x);
			}
			else if(tpe->client_type==CLIENT_TYPE_B){
				x=SoftSpriteParaGet(tpe->ss,SS_PARA_POS_X)-x;
				x-=24;
				SoftSpriteParaCalc(tpe->ss,SS_PARA_POS_X,-x);
			}
			else if(tpe->client_type==CLIENT_TYPE_D){
				x=x-SoftSpriteParaGet(tpe->ss,SS_PARA_POS_X);
				x-=16;
				SoftSpriteParaCalc(tpe->ss,SS_PARA_POS_X,x);
			}
			if(SoftSpriteParaGet(tpe->ss,SS_PARA_POS_X)>=tpe->appear_pos){
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpe->ss,1);
#endif
				SoftSpriteParaSet(tpe->ss,SS_PARA_SHADOW_Y_REQ,0);
				SoftSpriteParaSet(tpe->ss,SS_PARA_SHADOW_AFF_REQ,0);
				SoftSpriteParaSet(tpe->ss,SS_PARA_POS_X,tpe->appear_pos);
#ifndef POKE_ANM_OFF
				PokeEditData_PrgAnimeSet( tpe->cp->handle,
										  BattleWorkPokeAnmSysPTRGet(tpe->bw),
										  tpe->ss,
										  tpe->mons_no,
										  tpe->dir,
										  PARA_HFLIP_OFF,
										  tpe->client_no );
#endif
//				PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpe->bw),tpe->ss,tpe->mons_no,tpe->dir,tpe->chr,PARA_HFLIP_OFF,tpe->client_no);

//				Snd_PMVoicePlayEx(tpe->voice,tpe->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,HEAPID_BATTLE);

				{
					u8 wait;
					
					PokeEditData_VoiceWaitSet( tpe->cp->handle, &wait, tpe->mons_no, tpe->client_type );
					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
							 tpe->voice,tpe->mons_no,tpe->form_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
//				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
//								 tpe->voice,tpe->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
				if((tpe->client_type==CLIENT_TYPE_BB)||(tpe->client_type==CLIENT_TYPE_B)){
					CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_ENEMY_X,GROUND_ENEMY_Y);
				}
				SoftSpritePalFadeSet(tpe->ss,8,0,0,0x0000);
				tpe->seq_no++;
			}
		}
		else{
			CATS_ObjectPosGetCap(tpe->gw->cap,&x,&y);
			if((tpe->client_type==CLIENT_TYPE_AA)||(tpe->client_type==CLIENT_TYPE_A)){
				if(x>GROUND_MINE_X){
					CATS_ObjectPosMoveCap(tpe->gw->cap,-ENCOUNT_SPEED,0);
				}
				else{
					CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_MINE_X,GROUND_MINE_Y);
				}
			}
			CATS_ObjectPosGetCap(tpe->gw->cap,&x,&y);
			if(tpe->client_type==CLIENT_TYPE_AA){
				SoftSpriteParaSet(tpe->ss,SS_PARA_POS_X,x);
			}
			else if(tpe->client_type==CLIENT_TYPE_A){
				x=x-SoftSpriteParaGet(tpe->ss,SS_PARA_POS_X);
				x-=24;
				SoftSpriteParaCalc(tpe->ss,SS_PARA_POS_X,x);
			}
			else if(tpe->client_type==CLIENT_TYPE_C){
				x=SoftSpriteParaGet(tpe->ss,SS_PARA_POS_X)-x;
				x-=16;
				SoftSpriteParaCalc(tpe->ss,SS_PARA_POS_X,-x);
			}
			if(SoftSpriteParaGet(tpe->ss,SS_PARA_POS_X)<=tpe->appear_pos){
				SoftSpriteParaSet(tpe->ss,SS_PARA_POS_X,tpe->appear_pos);
//				PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpe->bw),tpe->ss,tpe->mons_no,tpe->dir,tpe->chr,PARA_HFLIP_OFF,tpe->client_no);
				
#ifndef POKE_ANM_OFF
				PokeEditData_PrgAnimeSet( tpe->cp->handle,
										  BattleWorkPokeAnmSysPTRGet(tpe->bw),
										  tpe->ss,
										  tpe->mons_no,
										  tpe->dir,
										  PARA_HFLIP_OFF,
										  tpe->client_no );
#endif
										  
//				Snd_PMVoicePlayEx(tpe->voice,tpe->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,HEAPID_BATTLE);
				{
					u8 wait;
					
					PokeEditData_VoiceWaitSet( tpe->cp->handle, &wait, tpe->mons_no, tpe->client_type );
					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
										 tpe->voice,tpe->mons_no,tpe->form_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
//				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
//								 tpe->voice,tpe->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
				if((tpe->client_type==CLIENT_TYPE_AA)||(tpe->client_type==CLIENT_TYPE_A)){
					CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_MINE_X,GROUND_MINE_Y);
				}
				tpe->seq_no++;
			}
		}
		break;
	case 3:
#ifndef POKE_ANM_OFF
		if((PokeAnm_IsFinished(BattleWorkPokeAnmSysPTRGet(tpe->bw),tpe->client_no)==TRUE)&&
		   (SoftSpriteAnimeEndCheck(tpe->ss)==FALSE)){
#endif
			if(tpe->rare){
				{
					WAZA_EFFECT_PARAM	wep;

					SCIO_EffectDataSet(tpe->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_REA,tpe->client_no,tpe->client_no,NULL);
					CT_WesCall(tpe->bw,tpe->cp,wsp,&wep);
				}
				tpe->seq_no=4;
			}
			else{
				tpe->seq_no=0xff;
			}
#ifndef POKE_ANM_OFF
		}
#endif
		break;
	case 4:
		WES_Executed(wsp);
		if (IsWES_Executed(wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(wsp);						///< Sysmte Reset
			tpe->seq_no=0xff;
		}
		break;
#if 0
	//�X�L�b�v�V�[�P���X
	case 5:
		if(tpe->dir==PARA_FRONT){
#ifndef POKE_ANM_OFF
			SoftSpriteAnimeSet(tpe->ss,1);
#endif
			SoftSpriteParaSet(tpe->ss,SS_PARA_SHADOW_Y_REQ,0);
			SoftSpriteParaSet(tpe->ss,SS_PARA_SHADOW_AFF_REQ,0);
			SoftSpriteParaSet(tpe->ss,SS_PARA_POS_X,tpe->appear_pos);
		
#ifndef POKE_ANM_OFF
			PokeEditData_PrgAnimeSet( tpe->cp->handle,
									  BattleWorkPokeAnmSysPTRGet(tpe->bw),
									  tpe->ss,
									  tpe->mons_no,
									  tpe->dir,
									  PARA_HFLIP_OFF,
									  tpe->client_no );
#endif
									  
//			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpe->bw),tpe->ss,tpe->mons_no,tpe->dir,tpe->chr,PARA_HFLIP_OFF,tpe->client_no);

			{
				u8 wait;
				
				PokeEditData_VoiceWaitSet( tpe->cp->handle, &wait, tpe->mons_no, tpe->client_type );
				
				PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
									 tpe->voice,tpe->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
			}
//			PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
//							 tpe->voice,tpe->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			if((tpe->client_type==CLIENT_TYPE_BB)||(tpe->client_type==CLIENT_TYPE_B)){
				CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_ENEMY_X,GROUND_ENEMY_Y);
			}
			SoftSpritePalFadeSet(tpe->ss,8,0,0,0x0000);
		}
		else{
#ifndef POKE_ANM_OFF
			SoftSpriteAnimeSet(tpe->ss,1);
			PokeEditData_PrgAnimeSet( tpe->cp->handle,
									  BattleWorkPokeAnmSysPTRGet(tpe->bw),
									  tpe->ss,
									  tpe->mons_no,
									  tpe->dir,
									  PARA_HFLIP_OFF,
									  tpe->client_no );
#endif

//			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpe->bw),tpe->ss,tpe->mons_no,tpe->dir,tpe->chr,PARA_HFLIP_OFF,tpe->client_no);
			{
				u8 wait;
				
				PokeEditData_VoiceWaitSet( tpe->cp->handle, &wait, tpe->mons_no, tpe->client_type );
				
				PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
									 tpe->voice,tpe->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
			}
//			PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
//							 tpe->voice,tpe->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			SoftSpriteParaSet(tpe->ss,SS_PARA_POS_X,tpe->appear_pos);
			if((tpe->client_type==CLIENT_TYPE_AA)||(tpe->client_type==CLIENT_TYPE_A)){
				CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_MINE_X,GROUND_MINE_Y);
			}
		}
		tpe->seq_no=3;
		break;
#endif
	default:
		Snd_PMVoiceDoubleFlagSet( 0 );			///< 2�C���̖���
		SCIO_ServerQueClear(tpe->bw,tpe->client_no,tpe->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�G���J�E���g�|�P��������V�[�P���X�pTCB�i�M���e�B�i����G���J�E���g��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonEncountGira(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_ENCOUNT	*tpe=(TCB_POKEMON_ENCOUNT *)work;
	WE_SYS_PTR			wsp=BattleWorkWE_SYS_PTRGet(tpe->bw);
	s16					x,y;

	switch(tpe->seq_no){
	case 0:
		tpe->wait=ENCOUNT_START_WAIT;
		tpe->seq_no++;
	case 1:
		if(--tpe->wait){
			break;
		}
		tpe->seq_no++;
	case 2:
		CATS_ObjectPosGetCap(tpe->gw->cap,&x,&y);
		if((tpe->client_type==CLIENT_TYPE_BB)||(tpe->client_type==CLIENT_TYPE_B)){
			if(x<GROUND_ENEMY_X){
				CATS_ObjectPosMoveCap(tpe->gw->cap,ENCOUNT_SPEED,0);
			}
			else{
				CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_ENEMY_X,GROUND_ENEMY_Y);
			}
		}
		CATS_ObjectPosGetCap(tpe->gw->cap,&x,&y);
		SoftSpriteParaCalc(tpe->ss, SS_PARA_POS_Y, ENCOUNT_SPEED/2);
		if(SoftSpriteParaGet(tpe->ss,SS_PARA_POS_Y)>=tpe->appear_pos){
			SoftSpriteAnimeSet(tpe->ss,1);
			SoftSpriteParaSet(tpe->ss,SS_PARA_SHADOW_Y_REQ,0);
			SoftSpriteParaSet(tpe->ss,SS_PARA_SHADOW_AFF_REQ,0);
			SoftSpriteParaSet(tpe->ss,SS_PARA_POS_Y,tpe->appear_pos);
			PokeEditData_PrgAnimeSet( tpe->cp->handle,
									  BattleWorkPokeAnmSysPTRGet(tpe->bw),
									  tpe->ss,
									  tpe->mons_no,
									  tpe->dir,
									  PARA_HFLIP_OFF,
									  tpe->client_no );
			{
				u8 wait;
					
				PokeEditData_VoiceWaitSet( tpe->cp->handle, &wait, tpe->mons_no, tpe->client_type );
					
				PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpe->bw,tpe->client_no),
									 tpe->voice,tpe->mons_no,tpe->form_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
			}
			CATS_ObjectPosSetCap(tpe->gw->cap,GROUND_ENEMY_X,GROUND_ENEMY_Y);
			SoftSpritePalFadeSet(tpe->ss,8,0,0,0x0000);
			tpe->seq_no++;
		}
		break;
	case 3:
		if((PokeAnm_IsFinished(BattleWorkPokeAnmSysPTRGet(tpe->bw),tpe->client_no)==TRUE)&&
		   (SoftSpriteAnimeEndCheck(tpe->ss)==FALSE)){
			if(tpe->rare){
				{
					WAZA_EFFECT_PARAM	wep;

					SCIO_EffectDataSet(tpe->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_REA,tpe->client_no,tpe->client_no,NULL);
					CT_WesCall(tpe->bw,tpe->cp,wsp,&wep);
				}
				tpe->seq_no=4;
			}
			else{
				tpe->seq_no=0xff;
			}
		}
		break;
	case 4:
		WES_Executed(wsp);
		if (IsWES_Executed(wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(wsp);						///< Sysmte Reset
			tpe->seq_no=0xff;
		}
		break;
	default:
		Snd_PMVoiceDoubleFlagSet( 0 );			///< 2�C���̖���
		SCIO_ServerQueClear(tpe->bw,tpe->client_no,tpe->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�|�P�����o��G�t�F�N�g�{�[���G�t�F�N�g�e�[�u��
 */
//============================================================================================

const	u8	BallMoveEffectTable[]={
	EBMT_THROW_C,		///< 1�C�̎�
	EBMT_STAY_BB,
	EBMT_THROW_L,		///< ���ɓ���������
	EBMT_STAY_B,
	EBMT_THROW_R,		///< �E�ɓ���������
	EBMT_STAY_D,
};

//============================================================================================
/**
 *	�|�P�����o��G�t�F�N�g����V�[�P���X�pTCB�i�G���J�E���g����p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonEncountAppear(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_APPEAR	*tpa=(TCB_POKEMON_APPEAR *)work;

	switch(tpa->seq_no){
	case 0:
		tpa->wait = 0;
//		tpa->ods = NULL;
		#if 1
		{
			tpa->ods = NULL;
			if ( BattleWorkFightTypeGet( tpa->bw ) & FIGHT_TYPE_MULTI ){					
				if ( ( BattleWorkBattleStatusFlagGet( tpa->bw ) & BATTLE_STATUS_FLAG_REC_CHILD )==0 )
				{
					if ( tpa->client_type == CLIENT_TYPE_A ){
						///< �T�[�o�[ �� CLIENT_TYPE_A �̎�
						OS_Printf( " OAM ���Ƃ��� -- 0\n" );
						OS_TPrintf( "CHILD:%08x\n",BattleWorkBattleStatusFlagGet(tpa->bw));
						tpa->ods = OAM_DropAll_Init( tpa->bw, HEAPID_BATTLE );
					}						
				}										
			}
			else {
				if ( ( BattleWorkBattleStatusFlagGet( tpa->bw ) & BATTLE_STATUS_FLAG_REC_CHILD )==0 )
				{
					if ( ( BattleWorkServerFlagGet( tpa->bw ) == 1 && tpa->client_type == CLIENT_TYPE_A ) )
					{
						///< �T�[�o�[ �� CLIENT_TYPE_A �̎�
						OS_Printf( " OAM ���Ƃ��� -- 1\n" );
						tpa->ods = OAM_DropAll_Init( tpa->bw, HEAPID_BATTLE );
					}
					else if ( tpa->client_type == CLIENT_TYPE_AA  )
					{
						///< TYPE_AA = 1vs1 �Ȃ̂ŁA�쐶���g���[�i�[��������
						OS_Printf( " OAM ���Ƃ��� -- 2\n" );
						tpa->ods = OAM_DropAll_Init( tpa->bw, HEAPID_BATTLE );
					}
					else {
						OS_Printf( " OAM ���Ƃ��ĂȂ�\n" );
						OS_Printf( " client_type = %d\n, server = %d\n", tpa->client_type, BattleWorkServerFlagGet( tpa->bw ) );
					}
				}
			}			
		}
		#endif
		tpa->seq_no++;
		break;
		
	case 1:
		{
			TBALL_CUSTOM_PARAM bcp = { 0 };
			
			bcp.client_type = tpa->client_type;	
			bcp.pp = BattleWorkPokemonParamGet(tpa->bw, tpa->client_no, tpa->sel_mons_no);		
			tpa->bes = BallEffect_Init(HEAPID_BATTLE, &bcp);
			BallEffect_ResourceLoad(tpa->bes);
		}
		tpa->seq_no++;
		break;
		
	case 2:
//		if(BM_ModeGet(tpa->bms)!=EBMM_THROW){
		if(BM_ModeGet(tpa->cp->bms)!=EBMM_THROW){
			break;
		}
		
		if (BallEffect_IsLoadEnd(tpa->bes) != TRUE){
			break;
		}
		
//		if (BM_IsOpen(tpa->bms) == TRUE){
		if (BM_IsOpen(tpa->cp->bms) == TRUE){
			if(tpa->client_type == CLIENT_TYPE_C){
				//������2�̖ڂ͖��������Ȃ��悤�ɏ����x�点�ēo��
				//(�G��2�̖ڂ̓{�[�������G�t�F�N�g�̕��ł���Ă���)
				tpa->wait++;
				if(tpa->wait >= ENCOUNT_DOUBLE_APPEAR_WAIT){
					tpa->wait = 0;
				}
				else{
					break;
				}
			}
			
			{	///< �|�P�������� �{���Ȃ�Acase 1 ���]�܂����B
				SOFT_SPRITE_MANAGER	*ssm;
				SOFT_SPRITE_ANIME	ssanm[SS_ANM_SEQ_MAX];

				ssm=BattleWorkSoftSpriteManagerGet(tpa->bw);
			#if 0
			//	PokeAnmDataSet(&ssanm[0],tpa->mons_no);							
			#endif
				PokeEditData_AnimeSet( tpa->cp->handle, &ssanm[0], tpa->mons_no, tpa->client_type );

				tpa->cp->poke_ss=CT_SoftSpriteAdd(tpa->bw,ssm,&tpa->ssa,
												  AppearPosTable[tpa->client_type][POS_APPEAR],
												  EncountPosTable[tpa->client_type][1],
												  EncountPosTable[tpa->client_type][2],
												  tpa->ofs_y,
												  tpa->height,
												  tpa->ofs_x,
												  tpa->shadow,
												  tpa->client_no,&ssanm[0],NULL);
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_APPEAR_X_S);
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_APPEAR_Y_S);
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_Y_REQ,0);
				SoftSpriteParaSet(tpa->cp->poke_ss, SS_PARA_VANISH, TRUE);
			}
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,16,0,BallHikariColor[tpa->ballID]);
			SoftSpriteParaSet(tpa->cp->poke_ss, SS_PARA_VANISH, FALSE);
			BallEffect_ExecutedEz(tpa->bes);
			if(tpa->dir==PARA_FRONT){
				Snd_SePlayPan(BSE_BALL_OPEN,WAZA_SE_PAN_R);
			}
			else{
				Snd_SePlayPan(BSE_BALL_OPEN,WAZA_SE_PAN_L);
			}
			if ( tpa->ods ){
				OAM_DropAll_End( tpa->ods );
				tpa->ods = NULL;
			}
			tpa->seq_no++;
		}
		break;
		
	case 3:
//		if(BM_EndCheck(tpa->bms) != TRUE){
		if( BM_EndCheck( tpa->cp->bms ) != TRUE ){
			tpa->seq_no++;
		}
		
	case 4:
		if((SoftSpriteParaGet(tpa->cp->poke_ss,SS_PARA_AFF_X)==AFF_APPEAR_X_B)
		&&(BallEffect_EndCheck(tpa->bes) == FALSE)){
			if(tpa->dir==PARA_FRONT){
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
#endif
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_AFF_REQ,0);
				
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
			//	PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
			//					 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
			else{
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
#endif
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
			//	PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
			//					 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
#ifndef POKE_ANM_OFF
			PokeEditData_PrgAnimeSet( tpa->cp->handle,
									  BattleWorkPokeAnmSysPTRGet( tpa->bw ),
									  tpa->cp->poke_ss,
									  tpa->mons_no,
									  tpa->dir,
									  PARA_HFLIP_OFF,
									  tpa->client_no );
#endif
//			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpa->bw),
//							  tpa->cp->poke_ss,tpa->mons_no,tpa->dir,tpa->chr,PARA_HFLIP_OFF,tpa->client_no);
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,0,0,BallHikariColor[tpa->ballID]);
			tpa->seq_no = 5;
		}
		else if(SoftSpriteParaGet(tpa->cp->poke_ss,SS_PARA_AFF_X)>=AFF_APPEAR_X_B){
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_APPEAR_X_B);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_APPEAR_Y_B);	
			if(tpa->dir==PARA_FRONT){
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
#endif
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_AFF_REQ,0);
				
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
				
//				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
//								 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
			else{
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);				
#endif
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
				
//				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
//								 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
			
#ifndef POKE_ANM_OFF
			PokeEditData_PrgAnimeSet( tpa->cp->handle,
									  BattleWorkPokeAnmSysPTRGet( tpa->bw ),
									  tpa->cp->poke_ss,
									  tpa->mons_no,
									  tpa->dir,
									  PARA_HFLIP_OFF,
									  tpa->client_no );
#endif
						  
//			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpa->bw),
//							  tpa->cp->poke_ss,tpa->mons_no,tpa->dir,tpa->chr,PARA_HFLIP_OFF,tpa->client_no);
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,0,1,BallHikariColor[tpa->ballID]);
			tpa->seq_no = 5;
		}
		else {
			SoftSpriteParaCalc(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_SPEED);
			SoftSpriteParaCalc(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_SPEED);
			SoftSpriteOYCalc(tpa->cp->poke_ss,tpa->height);
		}
		break;
	case 5:
		if(BallEffect_EndCheck(tpa->bes) == FALSE){
			tpa->seq_no = 6;
		}
		break;
	case 6:
#ifndef POKE_ANM_OFF
		if((PokeAnm_IsFinished(BattleWorkPokeAnmSysPTRGet(tpa->bw),tpa->client_no)==TRUE)&&
		   (SoftSpriteAnimeEndCheck(tpa->cp->poke_ss)==FALSE)){
#endif
//			BM_Delete(tpa->bms);
			BM_Delete(tpa->cp->bms);
			tpa->cp->bms=NULL;
			BallEffect_FreeMemory(tpa->bes);
			if(tpa->rare){
				{
					WAZA_EFFECT_PARAM	wep;

					tpa->wsp=WES_Create(HEAPID_BATTLE);
					SCIO_EffectDataSet(tpa->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_REA,tpa->client_no,tpa->client_no,NULL);
					CT_WesCall(tpa->bw,tpa->cp,tpa->wsp,&wep);
				}
				tpa->seq_no=7;
			}
			else{
				tpa->seq_no=0xff;
			}
#ifndef POKE_ANM_OFF
		}
#endif
		break;
	case 7:
		WES_Executed(tpa->wsp);
		if (IsWES_Executed(tpa->wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(tpa->wsp);						///< Sysmte Reset
			WES_Delete(tpa->wsp);						///< Sysmte Delete
			tpa->seq_no=0xff;
		}
		break;
	default:
		Snd_PMVoiceDoubleFlagSet( 0 );			///< 2�C���̖���
		SCIO_ServerQueClear(tpa->bw,tpa->client_no,tpa->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
/*
	switch(tpa->seq_no){
	case 0:
		if(BM_ModeGet(tpa->bms)!=EBMM_THROW){
			break;
		}
		if(BM_EndCheck(tpa->bms)==TRUE){
			break;
		}
		BM_Delete(tpa->bms);
		{
			SOFT_SPRITE_MANAGER	*ssm;
			SOFT_SPRITE_ANIME	ssanm[SS_ANM_SEQ_MAX];

			ssm=BattleWorkSoftSpriteManagerGet(tpa->bw);
//			PokeAnmDataSet(&ssanm[0],tpa->mons_no);

			tpa->cp->poke_ss=CT_SoftSpriteAdd(tpa->bw,ssm,&tpa->ssa,
											  AppearPosTable[tpa->client_type][POS_APPEAR],
											  EncountPosTable[tpa->client_type][1],
											  EncountPosTable[tpa->client_type][2],
											  tpa->ofs_y,
											  tpa->height,
											  tpa->client_no,&ssanm[0],NULL);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_APPEAR_X_S);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_APPEAR_Y_S);
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,16,0,BallHikariColor[tpa->ballID]);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_Y_REQ,0);

			tpa->seq_no++;
		}
		break;
	case 1:
		{
			TBALL_CUSTOM_PARAM bcp = { 0 };

			bcp.client_type = tpa->client_type;	
			bcp.pp = BattleWorkPokemonParamGet(tpa->bw, tpa->client_no, tpa->sel_mons_no);		
			tpa->bes = BallEffect_Init(HEAPID_BATTLE, &bcp);
			BallEffect_ResourceLoad(tpa->bes);

			tpa->seq_no++;
		}
		break;
	case 2:
		BallEffect_ExecutedEz(tpa->bes);
		Snd_SePlay(BSE_BALL_OPEN);
		tpa->seq_no++;
		break;
	case 3:
		if((SoftSpriteParaGet(tpa->cp->poke_ss,SS_PARA_AFF_X)==AFF_APPEAR_X_B)
		&&(BallEffect_EndCheck(tpa->bes) == FALSE)){
			if(tpa->dir==PARA_FRONT){
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_AFF_REQ,0);
				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
								 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,HEAPID_BATTLE);
			}
			else{
				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
								 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,HEAPID_BATTLE);
			}
			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpa->bw),
							  tpa->cp->poke_ss,tpa->mons_no,tpa->dir,tpa->chr,PARA_HFLIP_OFF,tpa->client_no);
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,0,0,BallHikariColor[tpa->ballID]);
			tpa->seq_no = 6;
		}
		else if(SoftSpriteParaGet(tpa->cp->poke_ss,SS_PARA_AFF_X)>=AFF_APPEAR_X_B){
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_APPEAR_X_B);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_APPEAR_Y_B);	
			if(tpa->dir==PARA_FRONT){
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_AFF_REQ,0);
				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
								 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,HEAPID_BATTLE);
			}
			else{
				PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
								 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,HEAPID_BATTLE);
			}
			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpa->bw),
							  tpa->cp->poke_ss,tpa->mons_no,tpa->dir,tpa->chr,PARA_HFLIP_OFF,tpa->client_no);
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,0,1,BallHikariColor[tpa->ballID]);
			tpa->seq_no = 4;
		}
		else {
			SoftSpriteParaCalc(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_SPEED);
			SoftSpriteParaCalc(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_SPEED);
			SoftSpriteOYCalc(tpa->cp->poke_ss,tpa->height);
		}
		break;
	case 4:
		if(BallEffect_EndCheck(tpa->bes) == FALSE){
			tpa->seq_no = 5;
		}
		break;
	case 5:
		if((PokeAnm_IsFinished(BattleWorkPokeAnmSysPTRGet(tpa->bw),tpa->client_no)==TRUE)&&
		   (SoftSpriteAnimeEndCheck(tpa->cp->poke_ss)==FALSE)){
			SCIO_ServerQueClear(tpa->bw,tpa->client_no,tpa->command_code);
			BallEffect_FreeMemory(tpa->bes);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
	}
*/
}
//============================================================================================
/**
 *	�|�P�����o��G�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonAppear(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_APPEAR	*tpa=(TCB_POKEMON_APPEAR *)work;
	WE_SYS_PTR			wsp=BattleWorkWE_SYS_PTRGet(tpa->bw);

	switch(tpa->seq_no){
	case 0:
		{
			///< �{�[��OAM�ǂݍ���
			TBALL_MOVE_DATA bmd;

			bmd.type=BallMoveEffectTable[tpa->client_type];
			bmd.heap_id=HEAPID_BATTLE;
			bmd.id=tpa->client_no;
			bmd.ball_id=tpa->ballID;
			bmd.csp=BattleWorkCATS_SYS_PTRGet(tpa->bw);
			bmd.pfd=BattleWorkPfdGet(tpa->bw);
			bmd.bg_pri = BM_BG_PRI_BATTLE;
			bmd.surface = 0;

			if(tpa->flag==POKEMON_APPEAR_GET_FAILED){
				bmd.mode=EBMM_FAILURE;		///< ���s��p
			}
			else{
				bmd.mode=EBMM_THROW;
			}

			tpa->bms=BM_Init(&bmd);
		}
		{
			///< �|�P�����ǂݍ���
			SOFT_SPRITE_MANAGER	*ssm;
			SOFT_SPRITE_ANIME	ssanm[SS_ANM_SEQ_MAX];

			ssm=BattleWorkSoftSpriteManagerGet(tpa->bw);
		#if 0
		//	PokeAnmDataSet(&ssanm[0],tpa->mons_no);
		#endif
			PokeEditData_AnimeSet( tpa->cp->handle, &ssanm[0], tpa->mons_no, tpa->client_type );

			tpa->cp->poke_ss=CT_SoftSpriteAdd(tpa->bw,ssm,&tpa->ssa,
											  AppearPosTable[tpa->client_type][POS_APPEAR],
											  EncountPosTable[tpa->client_type][1],
											  EncountPosTable[tpa->client_type][2],
											  tpa->ofs_y,
											  tpa->height,
											  tpa->ofs_x,
											  tpa->shadow,
											  tpa->client_no,&ssanm[0],NULL);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_APPEAR_X_S);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_APPEAR_Y_S);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_Y_REQ,0);
			SoftSpriteParaSet(tpa->cp->poke_ss, SS_PARA_VANISH, TRUE);
		}
		{
			///< OAM���Ƃ�
			tpa->ods = NULL;
			tpa->ods = OAM_DropAll_Init(tpa->bw, HEAPID_BATTLE);
		}
		tpa->seq_no++;
		break;
		
	case 1:
		{
			///< �{�[�� �p�[�e�B�N���ǂݍ���
			TBALL_CUSTOM_PARAM bcp = { 0 };
			
			bcp.client_type = tpa->client_type;	
			bcp.pp = BattleWorkPokemonParamGet(tpa->bw, tpa->client_no, tpa->sel_mons_no);	
			bcp.ball_id = tpa->ballID;
				
			tpa->bes = BallEffect_Init(HEAPID_BATTLE, &bcp);
			BallEffect_ResourceLoad(tpa->bes);
		}
		tpa->seq_no++;
		break;
		
	case 2:
		if((BM_ModeGet(tpa->bms)!=EBMM_THROW)&&
		   (tpa->flag==POKEMON_APPEAR_KURIDASI)){
			break;
		}
		
		if (BallEffect_IsLoadEnd(tpa->bes) != TRUE){
			break;
		}
		
		if (BM_IsOpen(tpa->bms) == TRUE){
			if ( tpa->ods ){
				OAM_DropAll_End(tpa->ods);
			}
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,16,0,BallHikariColor[tpa->ballID]);
			SoftSpriteParaSet(tpa->cp->poke_ss, SS_PARA_VANISH, FALSE);
			BallEffect_ExecutedEz(tpa->bes);
			if(tpa->dir==PARA_FRONT){
				Snd_SePlayPan(BSE_BALL_OPEN,WAZA_SE_PAN_R);
			}
			else{
				Snd_SePlayPan(BSE_BALL_OPEN,WAZA_SE_PAN_L);
			}
			tpa->seq_no++;
		}
		break;
		
	case 3:
		if(BM_EndCheck(tpa->bms) != TRUE){
			tpa->seq_no++;
		}
		
	case 4:
		if((SoftSpriteParaGet(tpa->cp->poke_ss,SS_PARA_AFF_X)==AFF_APPEAR_X_B)
		&&(BallEffect_EndCheck(tpa->bes) == FALSE)){
			if(tpa->dir==PARA_FRONT){
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
#endif
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_AFF_REQ,0);
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}

			//	PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
			//					 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
			else{	
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
#endif
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}

			//	PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
			//					 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
			
#ifndef POKE_ANM_OFF
			PokeEditData_PrgAnimeSet( tpa->cp->handle,
									  BattleWorkPokeAnmSysPTRGet( tpa->bw ),
									  tpa->cp->poke_ss,
									  tpa->mons_no,
									  tpa->dir,
									  PARA_HFLIP_OFF,
									  tpa->client_no );
#endif								  
//			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpa->bw),
//							  tpa->cp->poke_ss,tpa->mons_no,tpa->dir,tpa->chr,PARA_HFLIP_OFF,tpa->client_no);
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,0,0,BallHikariColor[tpa->ballID]);
			tpa->seq_no = 5;
		}
		else if(SoftSpriteParaGet(tpa->cp->poke_ss,SS_PARA_AFF_X)>=AFF_APPEAR_X_B){
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_APPEAR_X_B);
			SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_APPEAR_Y_B);	
			if(tpa->dir==PARA_FRONT){
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
#endif
				SoftSpriteParaSet(tpa->cp->poke_ss,SS_PARA_SHADOW_AFF_REQ,0);
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
			//	PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
			//					 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_FRONT,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
			else{
#ifndef POKE_ANM_OFF
				SoftSpriteAnimeSet(tpa->cp->poke_ss,1);
#endif
				{
					u8 wait;					
					PokeEditData_VoiceWaitSet( tpa->cp->handle, &wait, tpa->mons_no, tpa->client_type );					
					PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
										 tpa->voice,tpa->mons_no,tpa->form_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE, wait);
				}
			//	PokeVoicePlaySet(BattleWorkPerapVoiceGet(tpa->bw,tpa->client_no),
			//					 tpa->voice,tpa->mons_no,POKE_VOICE_PAN_BACK,POKE_APPEAR_NORMAL_VOL,NULL,HEAPID_BATTLE);
			}
			
#ifndef POKE_ANM_OFF
			PokeEditData_PrgAnimeSet( tpa->cp->handle,
									  BattleWorkPokeAnmSysPTRGet( tpa->bw ),
									  tpa->cp->poke_ss,
									  tpa->mons_no,
									  tpa->dir,
									  PARA_HFLIP_OFF,
									  tpa->client_no );
#endif
						  
//			PokePrgAnmDataSet(BattleWorkPokeAnmSysPTRGet(tpa->bw),
//							  tpa->cp->poke_ss,tpa->mons_no,tpa->dir,tpa->chr,PARA_HFLIP_OFF,tpa->client_no);
			SoftSpritePalFadeSet(tpa->cp->poke_ss,16,0,1,BallHikariColor[tpa->ballID]);
			tpa->seq_no = 5;
		}
		else {
			SoftSpriteParaCalc(tpa->cp->poke_ss,SS_PARA_AFF_X,AFF_SPEED);
			SoftSpriteParaCalc(tpa->cp->poke_ss,SS_PARA_AFF_Y,AFF_SPEED);
			SoftSpriteOYCalc(tpa->cp->poke_ss,tpa->height);
		}
		break;
	case 5:
		if(BallEffect_EndCheck(tpa->bes) == FALSE){
			tpa->seq_no = 6;
		}
		break;
	case 6:
#ifndef POKE_ANM_OFF
		if((PokeAnm_IsFinished(BattleWorkPokeAnmSysPTRGet(tpa->bw),tpa->client_no)==TRUE)&&
		   (SoftSpriteAnimeEndCheck(tpa->cp->poke_ss)==FALSE)){
#endif
			BM_Delete(tpa->bms);
			BallEffect_FreeMemory(tpa->bes);
			if(tpa->rare){
				{
					WAZA_EFFECT_PARAM	wep;

					SCIO_EffectDataSet(tpa->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_REA,tpa->client_no,tpa->client_no,NULL);
					CT_WesCall(tpa->bw,tpa->cp,wsp,&wep);
				}
				tpa->seq_no=7;
			}
			else{
				tpa->seq_no=8;
			}
#ifndef POKE_ANM_OFF
		}
#endif
		break;
	case 7:
	case 9:
	case 11:
		WES_Executed(wsp);
		if (IsWES_Executed(wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(wsp);					///< Sysmte Reset
			tpa->seq_no++;
		}
		break;
	case 8:
		if(tpa->migawari_flag){
			{
				WAZA_EFFECT_PARAM		wep;
	
				SCIO_EffectDataSet(tpa->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_OUT,tpa->client_no,tpa->client_no,NULL);
				CT_WesCall(tpa->bw,tpa->cp,wsp,&wep);
			}
			tpa->seq_no++;
		}
		else{
			tpa->seq_no=0xff;
		}
		break;
	case 10:
		{
			WAZA_POKE_CHANGE_PARAM	wpcp;
			WAZA_EFFECT_PARAM		wep;

			CT_WazaPokeChangeParamMake(tpa->bw,&tpa->wep,&wpcp,tpa->client_no);
			MigawariChrSet(&wpcp,HEAPID_BATTLE);

			SCIO_EffectDataSet(tpa->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_IN,tpa->client_no,tpa->client_no,NULL);
			CT_WesCall(tpa->bw,tpa->cp,wsp,&wep);
			tpa->cp->migawari_flag=1;
		}
		tpa->seq_no++;
		break;
	default:
		SCIO_ServerQueClear(tpa->bw,tpa->client_no,tpa->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�|�P�����������߂�G�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonReturn(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_RETURN	*tpr=(TCB_POKEMON_RETURN *)work;
	WE_SYS_PTR			wsp=BattleWorkWE_SYS_PTRGet(tpr->bw);

	switch(tpr->seq_no){
	case 0:
		if(tpr->migawari_flag){
			{
				WAZA_EFFECT_PARAM	wep;

				SCIO_EffectDataSet(tpr->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_OUT,tpr->client_no,tpr->client_no,NULL);
				CT_WesCall(tpr->bw,tpr->cp,wsp,&wep);
			}
			tpr->seq_no++;
		}
		else{
			tpr->seq_no=4;
		}
		break;

	case 2:
		{
			WAZA_POKE_CHANGE_PARAM	wpcp;
			WAZA_EFFECT_PARAM		wep;

			CT_WazaPokeChangeParamMake(tpr->bw,&tpr->wep,&wpcp,tpr->client_no);
			HenshinChrPop(&wpcp,HEAPID_BATTLE);

			SCIO_EffectDataSet(tpr->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_IN,tpr->client_no,tpr->client_no,NULL);
			CT_WesCall(tpr->bw,tpr->cp,wsp,&wep);
			tpr->cp->migawari_flag=0;
		}
		tpr->seq_no++;
		break;
	//�������̃G�t�F�N�g�I���҂�
	case 1:
	case 3:
		WES_Executed(wsp);
		if (IsWES_Executed(wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(wsp);						///< Sysmte Reset
			tpr->seq_no++;
		}
		break;
	case 4:
		if(tpr->client_type&CLIENT_ENEMY_FLAG){
			{
				TBALL_MOVE_DATA bmd;
	
				bmd.type=BallMoveEffectTable[tpr->client_type];
				bmd.heap_id=HEAPID_BATTLE;
				bmd.mode=EBMM_CLOSE;
				bmd.id=tpr->client_no;
				bmd.ball_id=tpr->ballID;
				bmd.csp=BattleWorkCATS_SYS_PTRGet(tpr->bw);
				bmd.pfd=BattleWorkPfdGet(tpr->bw);
				bmd.bg_pri = BM_BG_PRI_BATTLE;
				bmd.surface = 0;
				tpr->bms=BM_Init(&bmd);
			}
		}
		else {
			///< client type = ������
			{
				TBALL_MOVE_DATA bmd;
				
				int bm_param[] = { EBMT_STAY_AA, EBMT_STAY_BB,
								   EBMT_STAY_A, EBMT_STAY_B, EBMT_STAY_C, EBMT_STAY_D };
	
				bmd.type=bm_param[tpr->client_type];
				bmd.heap_id=HEAPID_BATTLE;
				bmd.mode=EBMM_CLOSE;
				bmd.id=tpr->client_no;
				bmd.ball_id=tpr->ballID;
				bmd.csp=BattleWorkCATS_SYS_PTRGet(tpr->bw);
				bmd.pfd=BattleWorkPfdGet(tpr->bw);
				bmd.bg_pri = BM_BG_PRI_BATTLE;
				bmd.surface = 0;
				tpr->bms=BM_Init(&bmd);
			}
		}
		tpr->seq_no=5;
		SoftSpritePalFadeSet(tpr->ss,0,16,0,BallHikariColor[tpr->ballID]);
		switch(tpr->client_type){
		case CLIENT_TYPE_AA:
		case CLIENT_TYPE_A:
		case CLIENT_TYPE_C:
			Snd_SePlayPan(BSE_BALL_OPEN,WAZA_SE_PAN_L);
			break;
		case CLIENT_TYPE_BB:
		case CLIENT_TYPE_B:
		case CLIENT_TYPE_D:
			Snd_SePlayPan(BSE_BALL_OPEN,WAZA_SE_PAN_R);
			break;
		}
		break;
	case 5:
		if(SoftSpritePalFadeExist(tpr->ss)==FALSE){
			tpr->seq_no++;
		}
		break;
	case 6:
		SoftSpriteParaCalc(tpr->ss,SS_PARA_AFF_X,-AFF_SPEED);
		SoftSpriteParaCalc(tpr->ss,SS_PARA_AFF_Y,-AFF_SPEED);
		SoftSpriteOYCalc(tpr->ss,tpr->height);
		if(SoftSpriteParaGet(tpr->ss,SS_PARA_AFF_X)<=AFF_APPEAR_X_S){
			SoftSpriteDel(tpr->ss);
			if(tpr->client_type&CLIENT_ENEMY_FLAG){
				tpr->seq_no=7;
			}
			else{
			///	tpr->seq_no=8;		///< �߂�G�t�F�N�g���o��̂ŏI���҂�����
				tpr->seq_no=7;
			}
		}
		break;
	case 7:
		if(BM_EndCheck(tpr->bms)==FALSE){
			BM_Delete(tpr->bms);
			tpr->seq_no=8;
		}
		break;
	case 8:
		SCIO_ServerQueClear(tpr->bw,tpr->client_no,tpr->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�|�P�����z�����ރG�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonInhale(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_INHALE	*tpi=(TCB_POKEMON_INHALE *)work;

	switch(tpi->seq_no){
	case 0:
		SoftSpriteParaCalc(tpi->ss,SS_PARA_AFF_X,-AFF_SPEED);
		SoftSpriteParaCalc(tpi->ss,SS_PARA_AFF_Y,-AFF_SPEED);
		if(tpi->height){
			tpi->height--;
			SoftSpriteParaCalc(tpi->ss,SS_PARA_POS_Y,-1);
		}
		if(SoftSpriteParaGet(tpi->ss,SS_PARA_AFF_X)<=AFF_APPEAR_X_S){
			SoftSpriteDel(tpi->ss);
			tpi->seq_no++;
		}
		break;
	case 1:
		SCIO_ServerQueClear(tpi->bw,tpi->client_no,tpi->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}
//============================================================================================
/**
 *	�G���J�E���g�g���[�i�[����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
enum{
	SEQ_TE_INIT=0,
	SEQ_TE_WAIT,
	SEQ_TE_MOVE,
	SEQ_TE_ANM_WAIT,
	SEQ_TE_END,
};
static	void	TCB_TrainerEncount(TCB_PTR tcb,void *work)
{
	TCB_TRAINER_ENCOUNT	*tte=(TCB_TRAINER_ENCOUNT *)work;
	s16					x,y,tr_x,tr_y;
	u32					fight_type;

	fight_type=BattleWorkFightTypeGet(tte->bw);

	if((tte->scroll_time<SCROLL_TIME)&&(tte->seq_no>=2)&&
	  ((tte->client_type==CLIENT_TYPE_AA)||(tte->client_type==CLIENT_TYPE_A))){
		GF_BGL_ScrollSet(BattleWorkGF_BGL_INIGet(tte->bw),GF_BGL_FRAME3_M,GF_BGL_SCROLL_X_INC,SCROLL_SPEED);
		tte->scroll_time++;
	}

	switch(tte->seq_no){
	case SEQ_TE_INIT:
		//3�ڂ̃A�j���[�V���������݂���Ȃ�΁A������G���J�E���g���̃A�j���[�V�����Ƃ��čĐ�����
		if(CATS_ObjectAnimeSeqNumGetCap(tte->cap)>2){
			CATS_ObjectAnimeFrameSetCap(tte->cap,0);
			CATS_ObjectAnimeSeqSetCap(tte->cap,2);
		}
		tte->wait=ENCOUNT_START_WAIT;
		tte->seq_no=SEQ_TE_WAIT;
	case SEQ_TE_WAIT:
		if(--tte->wait){
			break;
		}
		tte->seq_no=SEQ_TE_MOVE;
	case SEQ_TE_MOVE:
		CATS_ObjectPosGetCap(tte->gw->cap,&x,&y);
		CATS_ObjectPosGetCap(tte->cap,&tr_x,&tr_y);
		if(tte->dir==PARA_FRONT){
			if((tte->client_type==CLIENT_TYPE_BB)||(tte->client_type==CLIENT_TYPE_B)){
				if(x<GROUND_ENEMY_X){
					CATS_ObjectPosMoveCap(tte->gw->cap,ENCOUNT_SPEED,0);
				}
				else{
					CATS_ObjectPosSetCap(tte->gw->cap,GROUND_ENEMY_X,GROUND_ENEMY_Y);
				}
			}
			CATS_ObjectPosGetCap(tte->gw->cap,&x,&y);
			if((tte->client_type==CLIENT_TYPE_BB)||
			  ((tte->client_type==CLIENT_TYPE_B)&&
			  ((fight_type==FIGHT_TYPE_2vs2_TRAINER)||
			   (fight_type==FIGHT_TYPE_BATTLE_TOWER_2vs2)||
			   (fight_type==FIGHT_TYPE_2vs2_SIO)))){
				CATS_ObjectPosSetCap(tte->cap,x,tr_y);
			}
			else if(tte->client_type==CLIENT_TYPE_B){
				x=tr_x-x;
				x-=24;
				CATS_ObjectPosMoveCap(tte->cap,-x,0);
			}
			else if(tte->client_type==CLIENT_TYPE_D){
				x=x-tr_x;
				x-=16;
				CATS_ObjectPosMoveCap(tte->cap,x,0);
			}
			CATS_ObjectPosGetCap(tte->cap,&tr_x,&tr_y);
			if(tr_x>=tte->appear_pos){
				CATS_ObjectPosSetCap(tte->cap,tte->appear_pos,tr_y);
				if((tte->client_type==CLIENT_TYPE_BB)||(tte->client_type==CLIENT_TYPE_B)){
					CATS_ObjectPosSetCap(tte->gw->cap,GROUND_ENEMY_X,GROUND_ENEMY_Y);
				}
				//�A�j���[�V�����f�[�^�̂���L���������A�j�����X�^�[�g
				if(CATS_ObjectAnimeSeqNumGetCap(tte->cap)>1){
					CATS_ObjectAnimeFrameSetCap(tte->cap,0);
					CATS_ObjectAnimeSeqSetCap(tte->cap,1);
					tte->eff_flag=0;
					tte->seq_no=SEQ_TE_ANM_WAIT;
				}
				else{
					tte->seq_no=SEQ_TE_END;
				}
			}
		}
		else{
			if((tte->client_type==CLIENT_TYPE_AA)||(tte->client_type==CLIENT_TYPE_A)){
				if(x>GROUND_MINE_X){
					CATS_ObjectPosMoveCap(tte->gw->cap,-ENCOUNT_SPEED,0);
				}
				else{
					CATS_ObjectPosSetCap(tte->gw->cap,GROUND_MINE_X,GROUND_MINE_Y);
				}
			}
			CATS_ObjectPosGetCap(tte->gw->cap,&x,&y);
			if((tte->client_type==CLIENT_TYPE_AA)||
			  ((tte->client_type==CLIENT_TYPE_A)&&
			  ((fight_type==FIGHT_TYPE_2vs2_TRAINER)||
			   (fight_type==FIGHT_TYPE_BATTLE_TOWER_2vs2)||
			   (fight_type==FIGHT_TYPE_TAG_BATTLE)||
			   (fight_type==FIGHT_TYPE_2vs2_SIO)))){
				CATS_ObjectPosSetCap(tte->cap,x,tr_y);
			}
			else if(tte->client_type==CLIENT_TYPE_A){
				x=x-tr_x;
				x-=24;
				CATS_ObjectPosMoveCap(tte->cap,x,0);
			}
			else if(tte->client_type==CLIENT_TYPE_C){
				x=tr_x-x;
				x-=16;
				CATS_ObjectPosMoveCap(tte->cap,-x,0);
			}
			CATS_ObjectPosGetCap(tte->cap,&tr_x,&tr_y);
			if(tr_x<=tte->appear_pos){
				CATS_ObjectPosSetCap(tte->cap,tte->appear_pos,tr_y);
				if((tte->client_type==CLIENT_TYPE_AA)||(tte->client_type==CLIENT_TYPE_A)){
					CATS_ObjectPosSetCap(tte->gw->cap,GROUND_MINE_X,GROUND_MINE_Y);
				}
				tte->seq_no=SEQ_TE_END;
			}
		}
		break;
	case SEQ_TE_ANM_WAIT:
		if(CATS_ObjectAnimeActiveCheckCap(tte->cap)==TRUE){
			if(CT_TrainerEncountAnimeEffectCheck(tte,tte->cap)==FALSE){
				break;
			}
		}
		tte->seq_no=SEQ_TE_END;
	case SEQ_TE_END:
		SCIO_ServerQueClear(tte->bw,tte->client_no,tte->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�g���[�i�[������G�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
enum{
	SEQTT_TRAINER_THROW_INIT=0,
	SEQTT_TRAINER_THROW_START,
	SEQTT_TRAINER_THROW_OUT,
	SEQTT_TRAINER_THROW_ACT,
	SEQTT_TRAINER_THROW_ACT_WAIT,
	SEQTT_TRAINER_THROW_ACT_END,
	SEQTT_TRAINER_THROW_END,
};

static	void	TCB_TrainerThrow(TCB_PTR tcb,void *work)
{
	TCB_TRAINER_THROW	*ttt=(TCB_TRAINER_THROW *)work;
	s16					x,y;
	int					frame;
	u32					fight_type;
	s16					BallOfs[][6][2]={
										{
											//�j��l��
											{0x0000,0x0080},
											{-34, 4},
											{-28,-11},
											{50,-12},
									//		{50,-12},
											{0x7fff,0x7fff},	
											{0x7fff,0x7fff},	
										},
										{
											//����l��
											{0x0000,0x0080},
											{-34, 4},
											{-28,-11},
											{50,-12},
											{50,-12},
											{0x7fff,0x7fff},
										},
										{
											//���C�o��
											{0x0000,0x0080},
											{-34, 4},
											{-28,-11},
											{50,-12},
											{0x7fff,0x7fff},
											{0x7fff,0x7fff},
										},
										{
											//���~
											{0x0000,0x0080},
											{-33,-6},
											{-28,-18},
											{50,-36},
											{0x7fff,0x7fff},
										},
										{
											//�Q��
											{0x0000,0x0080},
											{-33,-2},
											{-28,-18},
											{50,-28},
											{0x7fff,0x7fff},
											{0x7fff,0x7fff},
										},
										{
											//�}�C
											{0x0000,0x0080},
											{-34, 4},
											{-28,-11},
											{50,-12},
											{0x7fff,0x7fff},
											{0x7fff,0x7fff},
										},
										{
											//�o�N
											{0x0000,0x0080},
											{-34, 4},
											{-28,-11},
											{50,-12},
											{0x7fff,0x7fff},
											{0x7fff,0x7fff},
										},
										{
											//�~��
											{0x0000,0x0080},
											{-34, 4},
											{-23, -1},
											{50, -2},
											{0x7fff,0x7fff},
											{0x7fff,0x7fff},
										},
										{
											//�j��l��DP
											{0x0000,0x0080},
											{-34, 4},
											{-28,-11},
											{50,-12},
											{50,-12},
											{0x7fff,0x7fff},	
										},
										{
											//����l��DP
											{0x0000,0x0080},
											{-34, 4},
											{-28,-11},
											{50,-12},
											{50,-12},
											{0x7fff,0x7fff},
										},
									};

	fight_type=BattleWorkFightTypeGet(ttt->bw);

	switch(ttt->seq_no){
	case SEQTT_TRAINER_THROW_INIT:
		if(ttt->type==TT_TYPE_ENCOUNT){
			ttt->seq_no=SEQTT_TRAINER_THROW_START;
		}
		else{
			{
				TBALL_MOVE_DATA bmd;

				switch(ttt->type){
				default:
				case TT_TYPE_SAFARI_BALL:
					bmd.type=EBMT_CAPTURE_BB;		///< �G�ɓ�����
					ttt->throw_type=EBMM_CAPTURE;
					bmd.ball_id=ITEM_SAFARIBOORU;
					break;
				case TT_TYPE_SAFARI_STONE:
					bmd.type=EBMT_THROW_E0_STONE;	///< �G�ɓ�����@��
					ttt->throw_type=EBMM_THROW;
					bmd.ball_id=BALL_EFF_STONE;
					break;
				case TT_TYPE_SAFARI_FOOD:
					bmd.type=EBMT_THROW_E0_FOOD;	///< �G�ɓ�����@�G�T
					ttt->throw_type=EBMM_THROW;
					bmd.ball_id=BALL_EFF_FOOD;
					break;
				case TT_TYPE_PARK_BALL:
					bmd.type=EBMT_CAPTURE_BB;		///< �G�ɓ�����
					ttt->throw_type=EBMM_CAPTURE;
					bmd.ball_id=BALL_EFF_PARK_BALL;
					break;
				}
				bmd.heap_id=HEAPID_BATTLE;
				bmd.mode=EBMM_FREE;
				bmd.id=ttt->client_no;
				bmd.csp=BattleWorkCATS_SYS_PTRGet(ttt->bw);
				bmd.pfd=BattleWorkPfdGet(ttt->bw);
				bmd.surface = 0;
				bmd.bw=ttt->bw;
	
				ttt->cp->bms=BM_Init(&bmd);
				BM_BallSoftPriSet(ttt->cp->bms,100);
				BM_BallBgPriSet(ttt->cp->bms,2);
				BM_BallAnimeFlagSet(ttt->cp->bms,FALSE);

				CATS_ObjectAnimeFrameSetCap(ttt->cp->cap,0);
				CATS_ObjectAnimeSeqSetCap(ttt->cp->cap,1);
				CATS_ObjectAutoAnimeSetCap(ttt->cp->cap,TRUE);
				ttt->seq_no=SEQTT_TRAINER_THROW_ACT;
			}
		}
		break;
	case SEQTT_TRAINER_THROW_START:
		if(ttt->dir==PARA_FRONT){
			CATS_ObjectPosMoveCap(ttt->cp->cap,TRAINER_IN_OUT_SPEED,0);
			CATS_ObjectPosGetCap(ttt->cp->cap,&x,&y);
			if((x>=BALL_THROW_X_R-48)&&(BM_ModeGet(ttt->cp->bms)!=EBMM_THROW)){
				BM_BallEnable(ttt->cp->bms,CATS_ENABLE_TRUE);
				BM_ModeChange(ttt->cp->bms,EBMM_THROW);
				if((fight_type&FIGHT_TYPE_2vs2)&&
				  ((fight_type&FIGHT_TYPE_MULTI)==0)){
					{
						CLIENT_PARAM *cp;

						cp=BattleWorkClientParamGet(ttt->bw,BattleWorkPartnerClientNoGet(ttt->bw,ttt->client_no));
						BM_BallEnable(cp->bms,CATS_ENABLE_TRUE);
						BM_ModeChange(cp->bms,EBMM_THROW);
						BM_BallWaitSet(cp->bms, ENCOUNT_DOUBLE_APPEAR_WAIT);
					}
				}
			}
			else if(x>=FRAME_OUT_X_R){
				{
					CATS_RES_PTR	crp;
	
					crp=BattleWorkCATS_RES_PTRGet(ttt->bw);
	
					CATS_ActorPointerDelete_S(ttt->cp->cap);
					ttt->cp->cap=NULL;
					//���\�[�X�폜
					CATS_FreeResourceChar(crp,CHARID_ENCOUNT_TRAINER1+ttt->cp->client_type);
					CATS_FreeResourcePltt(crp,PLTTID_ENCOUNT_TRAINER1+ttt->cp->client_type);
					CATS_FreeResourceCell(crp,CELLID_ENCOUNT_TRAINER1+ttt->cp->client_type);
					CATS_FreeResourceCellAnm(crp,CELLANMID_ENCOUNT_TRAINER1+ttt->cp->client_type);
				}
				ttt->seq_no=SEQTT_TRAINER_THROW_END;
			}
		}
		else{
			CATS_ObjectAnimeFrameSetCap(ttt->cp->cap,0);
			CATS_ObjectAnimeSeqSetCap(ttt->cp->cap,1);
			CATS_ObjectAutoAnimeSetCap(ttt->cp->cap,TRUE);
			ttt->seq_no=SEQTT_TRAINER_THROW_OUT;
		}
		break;
	case SEQTT_TRAINER_THROW_OUT:
		CATS_ObjectPosMoveCap(ttt->cp->cap,-TRAINER_IN_OUT_SPEED,0);
		CATS_ObjectPosGetCap(ttt->cp->cap,&x,&y);
		if(ttt->cp->bms){
			frame=CATS_ObjectAnimeFrameGetCap(ttt->cp->cap);
			if(BallOfs[ttt->trtype][frame][0]!=0x7fff){
				BM_BallEnable(ttt->cp->bms,CATS_ENABLE_TRUE);
				BM_BallPosSet(ttt->cp->bms,x+BallOfs[ttt->trtype][frame][0],y+BallOfs[ttt->trtype][frame][1]);
				if((frame==3)&&(BM_ModeGet(ttt->cp->bms)!=EBMM_THROW)){
					BM_ModeChange(ttt->cp->bms,EBMM_THROW);
					BM_BallAnimeFlagSet(ttt->cp->bms,TRUE);
					if((fight_type&FIGHT_TYPE_2vs2)&&
					  ((fight_type&FIGHT_TYPE_MULTI)==0)){
						{
							CLIENT_PARAM *cp;

							cp=BattleWorkClientParamGet(ttt->bw,BattleWorkPartnerClientNoGet(ttt->bw,ttt->client_no));
							BM_BallPosSet(cp->bms,x+BallOfs[ttt->trtype][frame][0],y+BallOfs[ttt->trtype][frame][1]);
							BM_BallEnable(cp->bms,CATS_ENABLE_TRUE);
							BM_ModeChange(cp->bms,EBMM_THROW);
							BM_BallAnimeFlagSet(cp->bms,TRUE);
						}
					}
				}
			}
		}
		if(x<=FRAME_OUT_X_L){
			{
				CATS_RES_PTR	crp;

				crp=BattleWorkCATS_RES_PTRGet(ttt->bw);

				CATS_ActorPointerDelete_S(ttt->cp->cap);
				ttt->cp->cap=NULL;
				//���\�[�X�폜
				CATS_FreeResourceChar(crp,CHARID_ENCOUNT_TRAINER1+ttt->cp->client_type);
				CATS_FreeResourcePltt(crp,PLTTID_ENCOUNT_TRAINER1+ttt->cp->client_type);
				CATS_FreeResourceCell(crp,CELLID_ENCOUNT_TRAINER1+ttt->cp->client_type);
				CATS_FreeResourceCellAnm(crp,CELLANMID_ENCOUNT_TRAINER1+ttt->cp->client_type);
			}
			ttt->seq_no=SEQTT_TRAINER_THROW_END;
		}
		break;
	case SEQTT_TRAINER_THROW_ACT:
		CATS_ObjectPosGetCap(ttt->cp->cap,&x,&y);
		frame=CATS_ObjectAnimeFrameGetCap(ttt->cp->cap);
		if(frame==4){
			ttt->wait=8;
			ttt->seq_no=SEQTT_TRAINER_THROW_ACT_WAIT;
		}
		else if(ttt->cp->bms!=NULL){
			if(BallOfs[ttt->trtype][frame][0]!=0x7fff){
				BM_BallPosSet(ttt->cp->bms,x+BallOfs[ttt->trtype][frame][0],y+BallOfs[ttt->trtype][frame][1]);
				if((frame==3)&&(BM_ModeGet(ttt->cp->bms)!=ttt->throw_type)){
					BM_ModeChange(ttt->cp->bms,ttt->throw_type);
					BM_BallBgPriSet(ttt->cp->bms,BM_BG_PRI_BATTLE);
					BM_BallAnimeFlagSet(ttt->cp->bms,TRUE);
					if(ttt->throw_type!=EBMM_CAPTURE){
						Snd_SePlay(BSE_SAFARI_THROW);
					}
				}
			}
		}
		break;
	case SEQTT_TRAINER_THROW_ACT_WAIT:
		if(--ttt->wait==0){
			if(ttt->throw_type==EBMM_CAPTURE){
				ttt->seq_no=SEQTT_TRAINER_THROW_END;
			}
			else{
				ttt->seq_no=SEQTT_TRAINER_THROW_ACT_END;
			}
		}
		break;
	case SEQTT_TRAINER_THROW_ACT_END:
		if(BM_EndCheck(ttt->cp->bms)==TRUE){
			break;
		}
		BM_Delete(ttt->cp->bms);
		ttt->cp->bms=NULL;
		ttt->seq_no=SEQTT_TRAINER_THROW_END;
		break;
	case SEQTT_TRAINER_THROW_END:
		SCIO_ServerQueClear(ttt->bw,ttt->client_no,ttt->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�g���[�i�[��ʊO�G�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_TrainerOut(TCB_PTR tcb,void *work)
{
	TCB_TRAINER_OUT	*tto=(TCB_TRAINER_OUT *)work;
	s16				tr_x;
	s16				tr_y;

	switch(tto->seq_no){
	case 0:
		if(tto->dir==PARA_FRONT){
			CATS_ObjectPosMoveCap(tto->cp->cap,TRAINER_IN_OUT_SPEED,0);
			CATS_ObjectPosGetCap(tto->cp->cap,&tr_x,&tr_y);
			if(tr_x>=FRAME_OUT_X_R){
				tto->seq_no++;
			}
		}
		else{
			CATS_ObjectPosMoveCap(tto->cp->cap,-TRAINER_IN_OUT_SPEED,0);
			CATS_ObjectPosGetCap(tto->cp->cap,&tr_x,&tr_y);
			if(tr_x<=FRAME_OUT_X_L){
				tto->seq_no++;
			}
		}
		break;
	case 1:
		{
			CATS_RES_PTR	crp;

			crp=BattleWorkCATS_RES_PTRGet(tto->bw);

			CATS_ActorPointerDelete_S(tto->cp->cap);
			tto->cp->cap=NULL;
			//���\�[�X�폜
			CATS_FreeResourceChar(crp,CHARID_ENCOUNT_TRAINER1+tto->cp->client_type);
			CATS_FreeResourcePltt(crp,PLTTID_ENCOUNT_TRAINER1+tto->cp->client_type);
			CATS_FreeResourceCell(crp,CELLID_ENCOUNT_TRAINER1+tto->cp->client_type);
			CATS_FreeResourceCellAnm(crp,CELLANMID_ENCOUNT_TRAINER1+tto->cp->client_type);
		}
		SCIO_ServerQueClear(tto->bw,tto->client_no,tto->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�g���[�i�[��ʓ��G�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_TrainerIn(TCB_PTR tcb,void *work)
{
	TCB_TRAINER_IN	*tti=(TCB_TRAINER_IN *)work;
	s16				x,y;

	switch(tti->seq_no){
	case 0:
		CATS_ObjectPosGetCap(tti->cap,&x,&y);
		if(tti->dir==PARA_BACK){
			x+=TRAINER_IN_OUT_SPEED;
			if(x>=tti->appear_pos){
				x=tti->appear_pos;
				tti->seq_no++;
			}
		}
		else{
			x-=TRAINER_IN_OUT_SPEED;
			if(x<=tti->appear_pos){
				x=tti->appear_pos;
				tti->seq_no++;
			}
		}
		CATS_ObjectPosSetCap(tti->cap,x,y);
		break;
	case 1:
		SCIO_ServerQueClear(tti->bw,tti->client_no,tti->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	HP�Q�[�W���IN�G�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_HPGaugeIn(TCB_PTR tcb,void *work)
{
	GAUGE_WORK *gauge = work;
	
	switch(gauge->param.seq_no){
	case 0:
		if(gauge->in_wait > 0){
			gauge->in_wait--;
			break;
		}

		Gauge_ScrollEffectSet(gauge, GAUGE_SCROLL_IN);
		Gauge_EnableSet(gauge, CATS_ENABLE_TRUE);
		gauge->param.seq_no++;
		break;
	case 1:
		if(gauge->eff_wait_in_out == TRUE){
			gauge->param.seq_no++;
		}
		break;
	default:
		SCIO_ServerQueClear(gauge->bw, gauge->client_no, gauge->command_code);
		gauge->tcb = NULL;
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	HP�Q�[�W���IN�G�t�F�N�g����V�[�P���X�pTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_HPGaugeOut(TCB_PTR tcb,void *work)
{
	GAUGE_WORK *gauge = work;
	
	switch(gauge->param.seq_no){
	case 0:
		if(gauge->eff_wait_in_out == TRUE){
			gauge->param.seq_no++;
		}
		break;
	default:
		SCIO_ServerQueClear(gauge->bw, gauge->client_no, gauge->command_code);
		gauge->tcb = NULL;
		TCB_Delete(tcb);
		//Gauge_ActorResourceDel(gauge);
		Gauge_EnableSet(gauge, CATS_ENABLE_FALSE);
		break;
	}
}

//============================================================================================
/**
 *	�R�}���h�I��TCB�p�V�[�P���X�i���o�[
 */
//============================================================================================
enum{	//�V�[�P���X�ԍ�
	CSSEQ_FAST_LOAD,
	CSSEQ_COMMAND_INIT,
	CSSEQ_COMMAND_MSG_WAIT,
	CSSEQ_COMMAND_INIT_WAIT,
	
	CSSEQ_COMMAND_SELECT_INIT,
	CSSEQ_COMMAND_SELECT,
	CSSEQ_COMMAND_SELECT_EFFWAIT,

	CSSEQ_COMMAND_SELECT_POKE_PARK_EFFWAIT,
	
	CSSEQ_FINISH,

#ifdef PM_DEBUG
	CSSEQ_DEBUG_TOOL_INIT,
	CSSEQ_DEBUG_TOOL_MAIN,
#endif
};

//============================================================================================
/**
 *	�R�}���h�I��TCB�i�v���C���[�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_CommandSelect(TCB_PTR tcb,void *work)
{
	TCB_COMMAND_SELECT	*tcs;
	GF_BGL_INI	*bgl;
	BI_PARAM_PTR bip;
	CLIENT_PARAM		*cp;
	u32					fight_type;
	int partner_client;
	GAUGE_WORK *partner_gauge;
	
	tcs=(TCB_COMMAND_SELECT *)work;
	bgl=BattleWorkGF_BGL_INIGet(tcs->bw);
	bip = BattleWorkGF_BGL_BIPGet(tcs->bw);
	cp=BattleWorkClientParamGet(tcs->bw,tcs->client_no);
	fight_type=BattleWorkFightTypeGet(tcs->bw);
	
	partner_client = BattleWorkPartnerClientNoGet(tcs->bw, tcs->client_no);
	if(partner_client != tcs->client_no){
		partner_gauge = BattleWorkGaugeWorkGet(tcs->bw, partner_client);
	}
	else{
		partner_gauge = NULL;
	}

#ifdef PM_DEBUG
#ifndef DEBUG_PRINT_BATTLE
	if((sys.cont&(PAD_BUTTON_L|PAD_BUTTON_Y))==(PAD_BUTTON_L|PAD_BUTTON_Y)){
		DebugSoftSpritePosPut(tcs->bw);
	}
#endif
#endif PM_DEBUG
	
	switch(tcs->seq_no){
	case CSSEQ_FAST_LOAD:
#ifdef FREEZE_BUG_CHECK
		OS_TPrintf("CSSEQ_FAST_LOAD\n");
#endif FREEZE_BUG_CHECK
		{
			//�Z�I����ʂ̕`��𑁂�����ׂɁA�����Ő�ɋZ�ԍ���PP�Ȃǂ�
			//������ăf�[�^���������ɓW�J���Ă��� 2006.05.12(��) by matsuda
			// ���]�䕔����̎x���̌���Ƃ��Ă܂��B
			BINPUT_WAZA_PARAM wazapara;
			int i;
			for(i = 0; i < WAZA_TEMOTI_MAX; i++){
				wazapara.wazano[i] = tcs->wazano[i];
				wazapara.pp[i] = tcs->pp[i];
				wazapara.ppmax[i] = tcs->ppmax[i];
			}
			BINPUT_WazaParaMemoryDecord(BattleWorkGF_BGL_BIPGet(tcs->bw), 
				tcs->client_type, &wazapara);
		}
		tcs->seq_no++;
		//break;
	case CSSEQ_COMMAND_INIT:
#ifdef FREEZE_BUG_CHECK
		OS_TPrintf("CSSEQ_COMMAND_INIT\n");
#endif FREEZE_BUG_CHECK
		{
			MSGDATA_MANAGER		*msg_m;
			MESSAGE_PARAM		mp;

			msg_m=BattleWorkFightMsgGet(tcs->bw);

			//�ߊl�f��
			if(fight_type&FIGHT_TYPE_GET_DEMO){
				switch(BattleWorkDemoSeqNoGet(tcs->bw)){
				case 0:
					mp.msg_tag=TAG_NICK;
					mp.msg_para[0]=tcs->client_no|(tcs->sel_mons_no<<8);
					mp.msg_id=DousuruMsg;
					BattleMSG_Print(tcs->bw,msg_m,&mp,0);
					tcs->seq_no = CSSEQ_COMMAND_INIT_WAIT;
					break;
				case 1:
					mp.msg_tag=TAG_NONE;
					mp.msg_id=GetdemoSupportMMsg1+BattleWorkMySexGet(tcs->bw,CLIENT_NO_MINE);
					tcs->index=BattleMSG_Print(tcs->bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(tcs->bw));
					tcs->seq_no = CSSEQ_COMMAND_MSG_WAIT;
					break;
				}
			}
			else{
				if(fight_type&FIGHT_TYPE_SAFARI){
					mp.msg_tag=TAG_TRNAME;
					mp.msg_para[0]=BattleWorkTrainerIDGet(tcs->bw,tcs->client_no);
					mp.msg_id=SafariDousuruMsg;
				}
				else if(fight_type&FIGHT_TYPE_POKE_PARK){
					mp.msg_tag=TAG_TRNAME;
					mp.msg_para[0]=BattleWorkTrainerIDGet(tcs->bw,tcs->client_no);
					mp.msg_id=ParkDousuruMsg;
				}
				else{
					mp.msg_tag=TAG_NICK;
					mp.msg_para[0]=tcs->client_no|(tcs->sel_mons_no<<8);
					mp.msg_id=DousuruMsg;
				}
				BattleMSG_Print(tcs->bw,msg_m,&mp,0);
				tcs->seq_no = CSSEQ_COMMAND_INIT_WAIT;
			}
		}
		break;
	case CSSEQ_COMMAND_MSG_WAIT:
#ifdef FREEZE_BUG_CHECK
		OS_TPrintf("CSSEQ_COMMAND_MSG_WAIT\n");
#endif FREEZE_BUG_CHECK
		if(GF_MSG_PrintEndCheck(tcs->index)==0){
			tcs->seq_no = CSSEQ_COMMAND_INIT_WAIT;
		}
		break;
	case CSSEQ_COMMAND_INIT_WAIT:
#ifdef FREEZE_BUG_CHECK
		OS_TPrintf("CSSEQ_COMMAND_INIT_WAIT\n");
#endif FREEZE_BUG_CHECK
		Gauge_AnimeStart(tcs->gauge);
		Poke_SinCurveSet(cp, tcs->bw);
		tcs->seq_no = CSSEQ_COMMAND_SELECT_INIT;
	case CSSEQ_COMMAND_SELECT_INIT:		///<��ʍ쐬
#ifdef FREEZE_BUG_CHECK
		OS_TPrintf("CSSEQ_COMMAND_SELECT_INIT\n");
#endif FREEZE_BUG_CHECK
		if(BINPUT_EffectEndCheck(bip) == FALSE){
			break;
		}

		{
			BINPUT_SCENE_COMMAND bsc;
			
			ARCHANDLE* hdl_bg;
			ARCHANDLE* hdl_obj;
			
			hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
			hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
			
			bsc.client_no = tcs->client_no;
			bsc.client_type = tcs->client_type;
			bsc.sel_mons_no = tcs->sel_mons_no;
			bsc.icon_hp = tcs->icon_hp;
			bsc.icon_hpmax = tcs->icon_hpmax;
			bsc.icon_status = tcs->icon_status;
			bsc.cancel_escape = BattleWorkEscCanCheck(tcs->bw,tcs->client_no,tcs->no_reshuffle_client);
			if(cp->bip_flag){
				BINPUT_BackFadeReq(bip, BINPUT_BACKFADE_DARK);
				if(BattleWorkBattleStatusFlagGet(tcs->bw) & BATTLE_STATUS_FLAG_FIRST_BATTLE){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_FIGHTONLY, FALSE, &bsc);
				}
				else if(BattleWorkFightTypeGet(tcs->bw) & FIGHT_TYPE_POKE_PARK){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_PARK, FALSE, &bsc);
				}
				else if(BattleWorkFightTypeGet(tcs->bw) & FIGHT_TYPE_SAFARI){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_A_SAFARI, FALSE, &bsc);
				}
				else if(bsc.client_type == CLIENT_TYPE_C 
						&& (BattleWorkFightTypeGet(tcs->bw) & FIGHT_TYPE_MULTI) == 0){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_A_2, FALSE, &bsc);
				}
				else{
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_A, FALSE, &bsc);
				}
				BattleWorkCommandSelectFlagSet(tcs->bw, SLIDE_IN_AFTER);
			}
			else{
				if(BattleWorkBattleStatusFlagGet(tcs->bw) & BATTLE_STATUS_FLAG_FIRST_BATTLE){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_COMMAND_IN_FIGHTONLY, FALSE, &bsc);
				}
				else if(BattleWorkFightTypeGet(tcs->bw) & FIGHT_TYPE_POKE_PARK){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_COMMAND_IN_PARK, FALSE, &bsc);
				}
				else if(BattleWorkFightTypeGet(tcs->bw) & FIGHT_TYPE_SAFARI){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_COMMAND_IN_SAFARI, FALSE, &bsc);
				}
				else if(bsc.client_type != CLIENT_TYPE_C){
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_COMMAND_IN, FALSE, &bsc);
				}
				else{
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_COMMAND_IN_2, FALSE, &bsc);
				}
				cp->bip_flag=1;
			}
			BINPUT_TemotiMotivationParamSet(bip, tcs->motivation);
			BINPUT_StockBallAnimeUpdate(bip, 
				tcs->status[POKEPARTY_MINE], tcs->status[POKEPARTY_ENEMY]);
			BINPUT_StockBallON(bip);
			
			ArchiveDataHandleClose( hdl_bg );
			ArchiveDataHandleClose( hdl_obj );
		}
		
		if(partner_gauge != NULL){
			Gauge_ColorBlackSet(partner_gauge);
		}
		
		tcs->seq_no = CSSEQ_COMMAND_SELECT;
		break;
	case CSSEQ_COMMAND_SELECT:
#ifdef FREEZE_BUG_CHECK
		OS_TPrintf("CSSEQ_COMMAND_SELECT\n");
#endif FREEZE_BUG_CHECK
		//HP���l���[�h�A�o�[���[�h�ؑ�(���K�ŋ@�\�ł��I�f�o�b�O�p����Ȃ���I)
		if(sys.trg & PAD_BUTTON_START){
			CLIENT_PARAM	*cp;
			int i;
			for(i=0;i<BattleWorkClientSetMaxGet(tcs->bw);i++){
				cp=BattleWorkClientParamGet(tcs->bw,i);
				Gauge_NumModeChange(&cp->gauge);
			}
		}
		
#ifdef PM_DEBUG

		//HP�\�����l�؂�ւ�
		if(sys.trg & PAD_BUTTON_SELECT){
			CLIENT_PARAM	*cp;
			int i;
			for(i=0;i<BattleWorkClientSetMaxGet(tcs->bw);i++){
				cp=BattleWorkClientParamGet(tcs->bw,i);
				Gauge_DebugHPPutFlagFlip(&cp->gauge);
				Gauge_PartsDraw(&cp->gauge, cp->gauge.hp, GAUGE_DRAW_ALL);
			}
		}
		//�퓬���f�o�b�O�p�p�����[�^�ύX�c�[��
		if(((sys.cont&(PAD_BUTTON_L|PAD_BUTTON_R))==(PAD_BUTTON_L|PAD_BUTTON_R))&&
			(BattleWorkServerFlagGet(tcs->bw))){
			tcs->seq_no=CSSEQ_DEBUG_TOOL_INIT;
			return;
		}
#endif

		tcs->tp_ret = BINPUT_TouchCheck(bip);
#ifdef FRONTIER_LOOP_ACT
		tcs->tp_ret = SELECT_ESCAPE_COMMAND;
#endif FRONTIER_LOOP_ACT
		if(tcs->tp_ret != RECT_HIT_NONE){
			tcs->wait=TP_WAIT;
			Snd_SePlay(BSE_TP_DECIDE);
			tcs->seq_no = CSSEQ_COMMAND_SELECT_EFFWAIT;
		}
		break;
	case CSSEQ_COMMAND_SELECT_EFFWAIT:
#ifdef FREEZE_BUG_CHECK
		OS_TPrintf("CSSEQ_COMMAND_SELECT_EFFWAIT\n");
#endif FREEZE_BUG_CHECK
		if(BINPUT_EffectEndCheck(bip) == TRUE
				|| tcs->tp_ret == SELECT_FIGHT_COMMAND){	//�u���������v�̏ꍇ�͎��̉�ʍ\�z�������o����悤�ɃG�t�F�N�g�̏I����҂����ɐi�߂�
			switch(tcs->tp_ret){
			case SELECT_FIGHT_COMMAND:
				if(BattleWorkFightTypeGet(tcs->bw)&(FIGHT_TYPE_SAFARI|FIGHT_TYPE_POKE_PARK)){
					tcs->seq_no=CSSEQ_COMMAND_SELECT_POKE_PARK_EFFWAIT;
				}
				break;
			case SELECT_ITEM_COMMAND:
				{
					ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
					ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WALL, FALSE, NULL);
					BINPUT_BackFadeReq(bip, BINPUT_BACKFADE_LIGHT);
					Gauge_ColorNormalSet(partner_gauge);
					Gauge_AnimeStop(tcs->gauge);
					Poke_SinCurveStop(cp);
					ArchiveDataHandleClose( hdl_bg );
					ArchiveDataHandleClose( hdl_obj );			
				}
				break;
			case SELECT_POKEMON_COMMAND:
				{
					ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
					ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WALL, FALSE, NULL);
					BINPUT_BackFadeReq(bip, BINPUT_BACKFADE_LIGHT);
					Gauge_ColorNormalSet(partner_gauge);
					Gauge_AnimeStop(tcs->gauge);
					Poke_SinCurveStop(cp);
					ArchiveDataHandleClose( hdl_bg );
					ArchiveDataHandleClose( hdl_obj );			
				}
				break;
			case SELECT_ESCAPE_COMMAND:
				{
					ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
					ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
#ifdef PM_DEBUG
					if(sys.cont&PAD_BUTTON_L){
						if((fight_type&FIGHT_TYPE_SIO)&&((fight_type&FIGHT_TYPE_TOWER)==0)){
							;
						}
						else{
							tcs->tp_ret = SELECT_DEBUG_WIN_ESCAPE_COMMAND;
						}
					}
					else if((sys.cont&PAD_BUTTON_R)&&((fight_type&FIGHT_TYPE_SIO)==0)){
						tcs->tp_ret = SELECT_DEBUG_ESCAPE_COMMAND;
					}
					else if((BINPUT_CancelEscapeFlagGet(bip)==TRUE)){
						Gauge_ColorNormalSet(partner_gauge);
						tcs->tp_ret = SELECT_CANCEL;
					}
#ifdef FRONTIER_LOOP_ACT
					tcs->tp_ret = SELECT_DEBUG_WIN_ESCAPE_COMMAND;
#endif
	//				else if((tcs->client_type == CLIENT_TYPE_C)&&
	//					   ((fight_type&FIGHT_TYPE_MULTI)==0)){
	//					Gauge_ColorNormalSet(partner_gauge);
	//					tcs->tp_ret = SELECT_CANCEL;
	//				}
	#else
					if((BINPUT_CancelEscapeFlagGet(bip)==TRUE)){
						Gauge_ColorNormalSet(partner_gauge);
						tcs->tp_ret = SELECT_CANCEL;
					}
	//				if((tcs->client_type == CLIENT_TYPE_C)&&
	//				  ((fight_type&FIGHT_TYPE_MULTI)==0)){
	//					Gauge_ColorNormalSet(partner_gauge);
	//					tcs->tp_ret = SELECT_CANCEL;
	//				}
#endif
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WALL, FALSE, NULL);
					ArchiveDataHandleClose( hdl_bg );
					ArchiveDataHandleClose( hdl_obj );			
				}
				break;
#ifdef PM_DEBUG
			case SELECT_DEBUG_ESCAPE_COMMAND:
				{
					ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
					ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );

					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WALL, FALSE, NULL);
					ArchiveDataHandleClose( hdl_bg );
					ArchiveDataHandleClose( hdl_obj );			
				}
				break;
#endif
			default:
				GF_ASSERT_MSG(0 , "�R�}���h���삪�ݒ肳��Ă��܂���\n");
				break;
			}
			
			BINPUT_StockBallOFF(bip);
			tcs->seq_no = CSSEQ_FINISH;
		}
		break;
	case CSSEQ_COMMAND_SELECT_POKE_PARK_EFFWAIT:
		if(BINPUT_EffectEndCheck(bip) == TRUE ){
			ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
			ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
			
			BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WALL, FALSE, NULL);
			BINPUT_BackFadeReq(bip, BINPUT_BACKFADE_LIGHT);
			Gauge_ColorNormalSet(partner_gauge);
			Gauge_AnimeStop(tcs->gauge);
			Poke_SinCurveStop(cp);
			BINPUT_StockBallOFF(bip);
			tcs->seq_no = CSSEQ_FINISH;
			
			ArchiveDataHandleClose( hdl_bg );
			ArchiveDataHandleClose( hdl_obj );			
		}
		break;
	case CSSEQ_FINISH:
		if(BINPUT_BackFadeExeCheck(bip) == TRUE){
			SCIO_CommandSelectReturnSet(tcs->bw,tcs->client_no,tcs->tp_ret);
			SCIO_ServerQueClear(tcs->bw,tcs->client_no,tcs->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
#ifdef PM_DEBUG
#ifndef DEBUG_PRINT_BATTLE
//�퓬���f�o�b�O�p�p�����[�^�ύX�c�[��
	case CSSEQ_DEBUG_TOOL_INIT:
		DebugBattleInit(tcs->bw);
		tcs->seq_no=CSSEQ_DEBUG_TOOL_MAIN;
		break;
	case CSSEQ_DEBUG_TOOL_MAIN:
		{
			BOOL	flag=FALSE;
			if(DebugBattleMain(tcs->bw,&flag)==TRUE){
				if(flag==FALSE){
					tcs->seq_no=CSSEQ_COMMAND_SELECT;
				}
				else{
					tcs->tp_ret = SELECT_DEBUG_ESCAPE_COMMAND;
					tcs->seq_no=CSSEQ_COMMAND_SELECT_EFFWAIT;
				}
			}
		}
		break;
#endif
#endif PM_DEBUG
	}
}

//============================================================================================
/**
 *	�R�}���h�I��TCB�iAI�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_CommandSelectAI(TCB_PTR tcb,void *work)
{
	TCB_COMMAND_SELECT	*tcs=(TCB_COMMAND_SELECT *)work;
	int					command;

	//�^�b�`�p�l���̃{�^���̃X���C�h�C�����I�����Ă��Ȃ��Ƃ��́AAI�v�Z���s��Ȃ��i������������ƃK�N�����߁j
//	if(BattleWorkCommandSelectFlagGet(tcs->bw)==SLIDE_IN_BEFORE){
//		return;
//	}

//�v���`�i�ł�Wifi��AI���ڂ����̂ŁA���̏����͕K�v�Ȃ��͂�
#if 0
#ifdef PM_DEBUG
	if(BattleWorkBattleStatusFlagGet(tcs->bw)&BATTLE_STATUS_FLAG_AUTO_BATTLE){
		//�ʐM�ڑ��ł́A�q�@����AI���̂��Ă��Ȃ��̂ŁA�����ōU����I��
//		if(CommStateIsWifiConnect()){
		if(BattleWorkFightTypeGet(tcs->bw)&FIGHT_TYPE_SIO){
			command=SELECT_FIGHT_COMMAND;
			SCIO_CommandSelectReturnSet(tcs->bw,tcs->client_no,command);

			SCIO_ServerQueClear(tcs->bw,tcs->client_no,tcs->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
			return;
		}
	}
#endif PM_DEBUG
#endif

	command=ClientAICommandSelectAI(tcs->bw,tcs->client_no);
	SCIO_CommandSelectReturnSet(tcs->bw,tcs->client_no,command);

	SCIO_ServerQueClear(tcs->bw,tcs->client_no,tcs->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�R�}���h�I��TCB�i�ʐM�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_CommandSelectSIO(TCB_PTR tcb,void *work)
{
	TCB_COMMAND_SELECT	*tcs=(TCB_COMMAND_SELECT *)work;

	SCIO_ServerQueClear(tcs->bw,tcs->client_no,tcs->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�T�t�@���p�������e�[�u��
 */
//============================================================================================
static	const	u8	SafariEscapeRateTable[][2]={
		{10,40},
		{10,35},
		{10,30},
		{10,25},
		{10,20},
		{10,15},
		{10,10},
		{15,10},
		{20,10},
		{25,10},
		{30,10},
		{35,10},
		{40,10},
};

//============================================================================================
/**
 *	�R�}���h�I��TCB�i�T�t�@���p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_CommandSelectSafari(TCB_PTR tcb,void *work)
{
	TCB_COMMAND_SELECT	*tcs=(TCB_COMMAND_SELECT *)work;
	int					rate;
	u16					mons_no;
	u8					escape_count;

	escape_count=BattleWorkSafariEscapeCountGet(tcs->bw);
	mons_no=ST_ServerPokemonServerParamGet(BattleWorkServerParamGet(tcs->bw),tcs->client_no,ID_PSP_monsno,NULL);
	rate=PokePersonalParaGet(mons_no,ID_PER_escape);

	rate=rate*SafariEscapeRateTable[escape_count][0]/SafariEscapeRateTable[escape_count][1];

	if((BattleWorkRandGet(tcs->bw)%255)<=rate){
		SCIO_CommandSelectReturnSet(tcs->bw,tcs->client_no,SELECT_SAFARI_ESCAPE);
	}
	else{
		SCIO_CommandSelectReturnSet(tcs->bw,tcs->client_no,SELECT_SAFARI_YOUSUMI);
	}

	SCIO_ServerQueClear(tcs->bw,tcs->client_no,tcs->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�R�}���h�I��TCB�i�|�P�p�[�N�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_CommandSelectPokepark(TCB_PTR tcb,void *work)
{
	TCB_COMMAND_SELECT	*tcs=(TCB_COMMAND_SELECT *)work;

	SCIO_CommandSelectReturnSet(tcs->bw,tcs->client_no,SELECT_POKEPARK_ESCAPE);
	SCIO_ServerQueClear(tcs->bw,tcs->client_no,tcs->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�R�}���h�I��TCB�i�ΐ�^��Đ��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_CommandSelectAuto(TCB_PTR tcb,void *work)
{
	TCB_COMMAND_SELECT	*tcs=(TCB_COMMAND_SELECT *)work;
	REC_DATA			data;

	//�^��f�[�^�̐��������`�F�b�N
	if(BattleWorkBattlePlay(tcs->bw,tcs->client_no,&data)==TRUE){
		BattleRecDataFullStop(tcs->bw);
	}
	//�R�}���h���Ȃ�
	if((!data)||
	//�R�}���h���͈͊O
	   (data>SELECT_ESCAPE_COMMAND)||
	//�R�}���h���o�b�O�i�A�C�e�����g�p�ł���FIGHT_TYPE�͘^�悵�Ȃ��j
	   (data==SELECT_ITEM_COMMAND)){
		BattleRecDataErrorStop(tcs->bw);
	}

	SCIO_CommandSelectReturnSet(tcs->bw,tcs->client_no,data);
	SCIO_ServerQueClear(tcs->bw,tcs->client_no,tcs->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�Z�I��TCB�i�v���C���[�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_WazaSelect(TCB_PTR tcb,void *work)
{
	TCB_WAZA_SELECT	*tws=(TCB_WAZA_SELECT *)work;
	GF_BGL_INI		*bgl=BattleWorkGF_BGL_INIGet(tws->bw);
	BI_PARAM_PTR bip;
	int partner_client;
	GAUGE_WORK *partner_gauge;
	CLIENT_PARAM *cp;
	
	bip = BattleWorkGF_BGL_BIPGet(tws->bw);
	cp=BattleWorkClientParamGet(tws->bw,tws->client_no);

	partner_client = BattleWorkPartnerClientNoGet(tws->bw, tws->client_no);
	if(partner_client != tws->client_no){
		partner_gauge = BattleWorkGaugeWorkGet(tws->bw, partner_client);
	}
	else{
		partner_gauge = NULL;
	}

	switch(tws->seq_no){
	case 0:		///<��ʍ쐬
		if(BINPUT_EffectEndCheck(bip) == FALSE){
			break;
		}
		
		{
			MSGDATA_MANAGER		*msg_m;
			MESSAGE_PARAM		mp;

			msg_m=BattleWorkFightMsgGet(tws->bw);
			mp.msg_tag=TAG_NICK;
			mp.msg_para[0]=tws->client_no|(tws->sel_mons_no<<8);
			mp.msg_id=DousuruMsg;
			BattleMSG_Print(tws->bw,msg_m,&mp,0);
		}
		BINPUT_StockBallOFF(bip);
		{
			BINPUT_SCENE_WAZA bsw;
			int i;
			ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
			ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );

			for(i = 0; i < WAZA_TEMOTI_MAX; i++){
				bsw.wazano[i] = tws->wazano[i];
				bsw.pp[i] = tws->pp[i];
				bsw.ppmax[i] = tws->ppmax[i];
			}
			bsw.client_type = tws->client_type;

			BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WAZA, FALSE, &bsw);
			ArchiveDataHandleClose( hdl_bg );
			ArchiveDataHandleClose( hdl_obj );			
		}
		tws->seq_no++;
		break;
	case 1:

#ifdef PM_DEBUG
		///< �Z�G�t�F�N�g�f�o�b�O���j���[�N��
		///< OVERLAY �� AI_TO_SKILL ���N�����āAseq_no = 4�ɂ��� �� OVERLAY��߂����߂̉��V�[�P���X
		if (sys.trg & PAD_BUTTON_START){
			tws->seq_no = 4;
			FightOverlaySwitch( tws->bw, FIGHT_OVERLAY_AI_TO_SKILL );
			DebugWazaEffectInit(HEAPID_BATTLE, tws->bw, tcb, TCB_WazaSelect);
			return;
		}
#endif

		tws->tp_ret = BINPUT_TouchCheck(bip);
		if(tws->tp_ret!=RECT_HIT_NONE){
			Snd_SePlay(BSE_TP_DECIDE);
			tws->seq_no++;
		}
		break;
	case 2:
		if(1){//BINPUT_EffectEndCheck(bip) == TRUE){
			if(tws->tp_ret != SELECT_CANCEL){
				if((BattleWorkFightTypeGet(tws->bw) & FIGHT_TYPE_2vs2) == 0){
					BINPUT_BackFadeReq(bip, BINPUT_BACKFADE_LIGHT);
					Gauge_ColorNormalSet(partner_gauge);
					Gauge_AnimeStop(tws->gauge);
					Poke_SinCurveStop(cp);
				}
			}
			SCIO_WazaSelectReturnSet(tws->bw,tws->client_no,tws->tp_ret);
			tws->seq_no++;
		}
		break;
	case 3:
//		BINPUT_CreateBG(bip, BINPUT_TYPE_WALL, FALSE, NULL);
		tws->seq_no++;
		//break;
	default:
		if(BINPUT_BackFadeExeCheck(bip) == TRUE){
			//�ߊl�f���p�ɃV�[�P���X�i���o�[��i�߂Ă���
			BattleWorkDemoSeqNoSet(tws->bw,1);
			SCIO_ServerQueClear(tws->bw,tws->client_no,tws->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;

#ifdef PM_DEBUG
	///< ����TCB�ɕ��A�����Ƃ��� SKILL_TO_AI�����s���邽��
	///< ���̂悤�ȋC�����̈�������ɂȂ��Ă܂�����B
	case 4:
		FightOverlaySwitch( tws->bw, FIGHT_OVERLAY_SKILL_TO_AI );
		tws->seq_no = 1;
		break;
#endif
	}
}

//============================================================================================
/**
 *	�Z�I��TCB�iAI�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_WazaSelectAI(TCB_PTR tcb,void *work)
{
	TCB_WAZA_SELECT	*tws=(TCB_WAZA_SELECT *)work;
	SERVER_PARAM	*sp;
	int				tp_ret;
	u32				fight_type;

	fight_type=BattleWorkFightTypeGet(tws->bw);
	sp=BattleWorkServerParamGet(tws->bw);

//�v���`�i�ł�Wifi��AI���ڂ����̂ŁA���̏����͕K�v�Ȃ��͂�
#if 0
#ifdef PM_DEBUG
	if(BattleWorkBattleStatusFlagGet(tws->bw)&BATTLE_STATUS_FLAG_AUTO_BATTLE){
		//�ʐM�ڑ��ł́A�q�@����AI���̂��Ă��Ȃ��̂ŁA�����_���ŋZ��I��
//		if(CommStateIsWifiConnect()){
		if(fight_type&FIGHT_TYPE_SIO){
			{
				int	i;
				int	waza[WAZA_TEMOTI_MAX];
				int	wazacnt;
	
				wazacnt=0;
	
				for(i=0;i<WAZA_TEMOTI_MAX;i++){
					if((tws->wazabit&No2Bit(i))==0){
						waza[wazacnt]=i+1;
						wazacnt++;
					}
				}
				tp_ret=waza[BattleWorkRandGet(tws->bw)%wazacnt];
			}
			//�U���Ώۂ̑���iDirSelectAI�ő�������j
			ST_ServerParamDataPut(tws->bw,sp,ID_SP_dir_select_client,tws->client_no,1);

			SCIO_WazaSelectReturnSet(tws->bw,tws->client_no,tp_ret);

			SCIO_ServerQueClear(tws->bw,tws->client_no,tws->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
			return;
		}
	}
#endif PM_DEBUG
#endif

	if((fight_type&FIGHT_TYPE_AI_CALC)||
	   (BattleWorkBattleStatusFlagGet(tws->bw)&BATTLE_STATUS_FLAG_FIRST_BATTLE)||
	   (BattleWorkMineEnemyCheck(tws->bw,tws->client_no)==0)){
//�v���`�i�ł�Wifi��AI���ڂ����̂ŁA���̏����͕K�v�Ȃ��͂�
#if 0
		//Wifi�̎��ɂ����ɂ���̂͂��������̂ŁAASSERT���o��
		GF_ASSERT(!CommStateIsWifiConnect());
#endif
		tp_ret=WazaAIMain(tws->bw,tws->client_no);
		//AI�v�Z���r���Ȃ̂ŁA�ēxAI���R�[������
		switch(tp_ret){
		//AI�v�Z���r���Ȃ̂ŁA�ēxAI���R�[������
		case WAZA_AI_CALC_CONTINUE:
			return;
			break;
		default:
			tp_ret++;
			break;
		}
	}
	else{
		{
			int	i;
			int	waza[WAZA_TEMOTI_MAX];
			int	wazacnt;

			wazacnt=0;

			for(i=0;i<WAZA_TEMOTI_MAX;i++){
				if((tws->wazabit&No2Bit(i))==0){
					waza[wazacnt]=i+1;
					wazacnt++;
				}
			}
			tp_ret=waza[BattleWorkRandGet(tws->bw)%wazacnt];
		}
		//�U���Ώۂ̑���iDirSelectAI�ő�������j
		ST_ServerParamDataPut(tws->bw,sp,ID_SP_dir_select_client,tws->client_no,
							  ST_ServerDefenceClientSet(tws->bw,sp,tws->client_no,tws->wazano[tp_ret-1],1,NULL));
	}

	SCIO_WazaSelectReturnSet(tws->bw,tws->client_no,tp_ret);

	SCIO_ServerQueClear(tws->bw,tws->client_no,tws->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�Z�I��TCB�i�ʐM�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_WazaSelectSIO(TCB_PTR tcb,void *work)
{
	TCB_WAZA_SELECT	*tws=(TCB_WAZA_SELECT *)work;

	SCIO_ServerQueClear(tws->bw,tws->client_no,tws->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�Z�I��TCB�i�ΐ�^��Đ��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_WazaSelectAuto(TCB_PTR tcb,void *work)
{
	TCB_WAZA_SELECT	*tws=(TCB_WAZA_SELECT *)work;
	REC_DATA		data;

	//�^��f�[�^�̐��������`�F�b�N
	if(BattleWorkBattlePlay(tws->bw,tws->client_no,&data)==TRUE){
		BattleRecDataFullStop(tws->bw);
	}
	//�R�}���h���Ȃ�
	if((!data)||
	//�R�}���h���͈͊O
	   (data>WAZA_TEMOTI_MAX)){
		BattleRecDataErrorStop(tws->bw);
	}
	else{
		//�Z�i���o�[�̐��������`�F�b�N
		{
			u16	waza_no;

			waza_no=ST_ServerPokemonServerParamGet(BattleWorkServerParamGet(tws->bw),tws->client_no,ID_PSP_waza1+(data-1),NULL);

			if((!waza_no)||
			   (waza_no>WAZANO_MAX)){
				BattleRecDataErrorStop(tws->bw);
			}
		}
	}

	SCIO_WazaSelectReturnSet(tws->bw,tws->client_no,data);
	SCIO_ServerQueClear(tws->bw,tws->client_no,tws->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�U���ΏۑI��TCB�i�v���C���[�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_DirSelect(TCB_PTR tcb,void *work)
{
	TCB_DIR_SELECT	*tds=(TCB_DIR_SELECT *)work;
	GF_BGL_INI		*bgl=BattleWorkGF_BGL_INIGet(tds->bw);
	BI_PARAM_PTR bip;
	int partner_client;
	GAUGE_WORK *partner_gauge;
	CLIENT_PARAM *cp;

	cp=BattleWorkClientParamGet(tds->bw,tds->client_no);
	bip = BattleWorkGF_BGL_BIPGet(tds->bw);

	partner_client = BattleWorkPartnerClientNoGet(tds->bw, tds->client_no);
	if(partner_client != tds->client_no){
		partner_gauge = BattleWorkGaugeWorkGet(tds->bw, partner_client);
	}
	else{
		partner_gauge = NULL;
	}

	switch(tds->seq_no){
	case 0:		///<��ʍ쐬
		if(BINPUT_EffectEndCheck(bip) == FALSE){
			break;
		}

		{
			BINPUT_SCENE_POKE bsp;
			int i;
			ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
			ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
			
			for(i = 0; i < CLIENT_MAX; i++){
				bsp.dspp[i] = tds->dspp[i];
			}
			bsp.client_type = tds->client_type;
			bsp.pokesele_type = BINPUT_RangeBgTypeGet(tds->range, tds->client_type);
			
			BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_POKE, FALSE, &bsp);
			ArchiveDataHandleClose( hdl_bg );
			ArchiveDataHandleClose( hdl_obj );			
		}
		
		tds->seq_no++;
		break;
	case 1:
		tds->tp_ret = BINPUT_TouchCheck(bip);
		if(tds->tp_ret != RECT_HIT_NONE){
			Snd_SePlay(BSE_TP_DECIDE);
			tds->seq_no++;
		}
		break;
	case 2:
		if(1){//BINPUT_EffectEndCheck(bip) == TRUE){
			if(tds->tp_ret != SELECT_CANCEL){
				Gauge_AnimeStop(tds->gauge);
				Poke_SinCurveStop(cp);
				Gauge_ColorNormalSet(partner_gauge);
				if(tds->light_flag == TRUE){
					BINPUT_BackFadeReq(bip, BINPUT_BACKFADE_LIGHT);
				}
			}
			tds->seq_no++;
		}
		break;
	case 3:
//		BINPUT_CreateBG(bip, BINPUT_TYPE_WALL, FALSE, NULL);
		tds->seq_no++;
		//break;
	default:
		if(BINPUT_BackFadeExeCheck(bip) == TRUE){
			{
				u8	buf[CLIENT_TYPE_MAX];
				u32	fight_type;
				int	ret;

				ret=tds->tp_ret;
				fight_type=BattleWorkFightTypeGet(tds->bw);

				if(ret != SELECT_CANCEL){
					BattleClientNoBufMake(tds->bw,&buf[0]);
					if(fight_type&FIGHT_TYPE_2vs2){
						ret=buf[CLIENT_TYPE_A+(tds->tp_ret-1)]+1;
					}
					else{
						ret=buf[CLIENT_TYPE_AA+(tds->tp_ret-1)]+1;
					}
				}

				SCIO_DirSelectReturnSet(tds->bw,tds->client_no,ret);
				SCIO_ServerQueClear(tds->bw,tds->client_no,tds->command_code);
				sys_FreeMemoryEz(work);
				TCB_Delete(tcb);
			}
		}
		break;
	}
}

//============================================================================================
/**
 *	�U���ΏۑI��TCB�iAI�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_DirSelectAI(TCB_PTR tcb,void *work)
{
	TCB_DIR_SELECT	*tds=(TCB_DIR_SELECT *)work;
	SERVER_PARAM	*sp;
	int	tp_ret;

	sp=BattleWorkServerParamGet(tds->bw);

//	tp_ret=ST_ServerDefenceClientSet(tds->bw,BattleWorkServerParamGet(tds->bw),tds->client_no,0,1,tds->range)+1;
	tp_ret=ST_ServerParamDataGet(tds->bw,sp,ID_SP_dir_select_client,tds->client_no)+1;
	GF_ASSERT_MSG(tp_ret<5,"tp_ret���s���ł�:%d\n",tp_ret);

	SCIO_DirSelectReturnSet(tds->bw,tds->client_no,tp_ret);

	SCIO_ServerQueClear(tds->bw,tds->client_no,tds->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�U���ΏۑI��TCB�i�ʐM�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_DirSelectSIO(TCB_PTR tcb,void *work)
{
	TCB_DIR_SELECT	*tds=(TCB_DIR_SELECT *)work;

	SCIO_ServerQueClear(tds->bw,tds->client_no,tds->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�U���ΏۑI��TCB�i�ΐ�^��Đ��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_DirSelectAuto(TCB_PTR tcb,void *work)
{
	TCB_DIR_SELECT	*tds=(TCB_DIR_SELECT *)work;
	REC_DATA		data;

	//�^��f�[�^�̐��������`�F�b�N
	if(BattleWorkBattlePlay(tds->bw,tds->client_no,&data)==TRUE){
		BattleRecDataFullStop(tds->bw);
	}
	//�R�}���h���Ȃ�
	if((!data)||
	//�R�}���h���͈͊O
	   (data>CLIENT_MAX)){
		BattleRecDataErrorStop(tds->bw);
	}
	else{
		{
			int	def_client=data-1;

			//�Z�̌��ʔ͈͂ɂ���Đ��������`�F�b�N
			switch(tds->range){
			case RANGE_NORMAL:		//�����ȊO��3��
				//�������w�肵�Ă�����s���f�[�^
				if(tds->client_no==def_client){
					BattleRecDataErrorStop(tds->bw);
				}
				break;
			case RANGE_TUBOWOTUKU:	//�ڂ�����p
				//��������I�����Ă��Ȃ��Ȃ�s���f�[�^
				if(BattleWorkMineEnemyCheck(tds->bw,tds->client_no)!=
				   BattleWorkMineEnemyCheck(tds->bw,def_client)){
					BattleRecDataErrorStop(tds->bw);
				}
				break;
			//�������`�F�b�N�̕K�v�Ȃ�
			case RANGE_NONE:		//�I���Ȃ�
			case RANGE_RANDOM:		//�����_��
			case RANGE_DOUBLE:		//�Q�̂ɓ�����
			case RANGE_TRIPLE:		//�����ȊO3�̂ɓ�����
			case RANGE_MINE:		//�����I��
			case RANGE_MINE_DOUBLE:	//�����Q�̑I��
			case RANGE_FIELD:		//��ɂ�����
			case RANGE_MAKIBISHI:	//�܂��т���p
			case RANGE_TEDASUKE:	//�Ă�������p
			case RANGE_SAKIDORI:	//�����ǂ��p
				break;
			}
		}
	}

	SCIO_DirSelectReturnSet(tds->bw,tds->client_no,data);
	SCIO_ServerQueClear(tds->bw,tds->client_no,tds->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	����I���V�[�P���X�i���o�[
 */
//============================================================================================
enum{
	SEQ_ITEM_SEL_INIT=0,
	SEQ_ITEM_SEL_WAIT,
	SEQ_ITEM_SEL_TASK_SET,
	SEQ_ITEM_SEL_MAIN,
	SEQ_ITEM_SEL_POKE_LIST_INIT,
	SEQ_ITEM_SEL_POKE_LIST_MAIN,
	SEQ_ITEM_SEL_END_INIT,
	SEQ_ITEM_SEL_END_WAIT,
	SEQ_ITEM_SEL_END,
	SEQ_ITEM_SEL_USE_MSG,
	SEQ_ITEM_SEL_USE_MSG_WAIT,
	SEQ_ITEM_SEL_BATTLE_EFFECT_INIT1,
	SEQ_ITEM_SEL_BATTLE_EFFECT_WAIT1,
	SEQ_ITEM_SEL_BATTLE_EFFECT_INIT2,
	SEQ_ITEM_SEL_BATTLE_EFFECT_WAIT2,
	SEQ_ITEM_SEL_BATTLE_EFFECT_MSG,
	SEQ_ITEM_SEL_BATTLE_EFFECT_MSG_WAIT,
	SEQ_ITEM_SEL_HPRCV_EFFECT_INIT1,
	SEQ_ITEM_SEL_HPRCV_EFFECT_WAIT1,
	SEQ_ITEM_SEL_HPRCV_EFFECT_INIT2,
	SEQ_ITEM_SEL_HPRCV_EFFECT_WAIT2,
	SEQ_ITEM_SEL_HPRCV_EFFECT_INIT3,
	SEQ_ITEM_SEL_HPRCV_EFFECT_WAIT3,
	SEQ_ITEM_SEL_HPRCV_EFFECT_MSG,
	SEQ_ITEM_SEL_HPRCV_EFFECT_MSG_WAIT,
	SEQ_ITEM_SEL_CONDRCV_EFFECT_INIT1,
	SEQ_ITEM_SEL_CONDRCV_EFFECT_WAIT1,
	SEQ_ITEM_SEL_CONDRCV_EFFECT_INIT2,
	SEQ_ITEM_SEL_CONDRCV_EFFECT_WAIT2,
	SEQ_ITEM_SEL_CONDRCV_EFFECT_MSG,
	SEQ_ITEM_SEL_CONDRCV_EFFECT_MSG_WAIT
};

enum{
	NEMURI_RCV=0,
	DOKU_RCV,
	YAKEDO_RCV,
	KOORI_RCV,
	MAHI_RCV,
	KONRAN_RCV,
	MEROMERO_RCV
};

//============================================================================================
/**
 *	����I��TCB�i�v���C���[�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_ItemSelect(TCB_PTR tcb,void *work)
{
	TCB_ITEM_SELECT	*tis=(TCB_ITEM_SELECT *)work;
	PALETTE_FADE_PTR	pfd;

	pfd=BattleWorkPfdGet(tis->bw);
	
	switch(tis->seq_no){
	case SEQ_ITEM_SEL_INIT:		//�o�b�O��ʂւ̃t�F�[�h
		//BIP����������܂��ɕK�v�ȏ�񂾂��擾
		tis->decend_key = BINPUT_CursorDecendGet(BattleWorkGF_BGL_BIPGet(tis->bw));
		
		MSG_DsIconFlashReq(BattleWorkMsgIconGet(tis->bw),MSG_DSI_REQ_STOP);
		PaletteFadeReq(pfd,PF_BIT_MAIN_ALL,0x0c00,BATTLE_BAGLIST_FADE_SPEED,0,MSG_WINDOW_FADE,0x0000);
		PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,BATTLE_BAGLIST_FADE_SPEED,0,16,0x0000);
		tis->seq_no++;
		break;
	case SEQ_ITEM_SEL_WAIT:
		if(PaletteFadeCheck(pfd)==0){	//�P�x�ύX�t�F�[�h�I���҂����ĐF�X�폜
			{
				FightScreenTransition(tis->bw);

				tis->bag_buf=sys_AllocMemory(HEAPID_BATTLE,sizeof(BBAG_DATA));
				memset( tis->bag_buf, 0, sizeof(BBAG_DATA) );
				tis->bag_buf->bw=tis->bw;
				tis->bag_buf->myst=BattleWorkMyStatusGet(tis->bw,tis->client_no);
				tis->bag_buf->heap=HEAPID_BATTLE;
				tis->bag_buf->end_flg=0;
				tis->bag_buf->myitem=BattleWorkMyItemGet(tis->bw);
				tis->bag_buf->client_no=tis->client_no;
				tis->bag_buf->cursor_flg=tis->decend_key;
				tis->bag_buf->enc_double=tis->enc_double;
				tis->bag_buf->waza_vanish=tis->waza_vanish;
				tis->bag_buf->waza_migawari=tis->waza_migawari;
				tis->bag_buf->skill_item_use=tis->shutout_flag[tis->client_no];	// �Z�Ŏg�p�s��
				BattleBag_TaskAdd( tis->bag_buf );
				tis->seq_no=SEQ_ITEM_SEL_MAIN;
			}
		}
		break;
	case SEQ_ITEM_SEL_TASK_SET:
		tis->bag_buf->cursor_flg=tis->decend_key;
		BattleBag_TaskAdd( tis->bag_buf );
		tis->seq_no++;
	case SEQ_ITEM_SEL_MAIN:
		if(tis->bag_buf->end_flg){
			tis->bag_buf->end_flg=0;
			tis->decend_key = tis->bag_buf->cursor_flg;
			if(tis->bag_buf->ret_item){
				switch(tis->bag_buf->ret_page){
				case BBAG_POKE_STRCV:	// ��ԉ񕜃|�P�b�g
				case BBAG_POKE_HPRCV:	// HP�񕜃|�P�b�g
					tis->seq_no=SEQ_ITEM_SEL_POKE_LIST_INIT;
					break;
				case BBAG_POKE_BATTLE:	// �퓬�p�|�P�b�g
				case BBAG_POKE_BALL:	// �{�[���|�P�b�g
					tis->seq_no=SEQ_ITEM_SEL_END_INIT;
					break;
				}
			}
			else{
				tis->seq_no=SEQ_ITEM_SEL_END_INIT;
			}
		}
		break;
	//�|�P�������X�g��ʂ�
	case SEQ_ITEM_SEL_POKE_LIST_INIT:
		{
			int				i;
			int				cnt=0;
			int				client_no;
			POKEPARTY		*poke_party=BattleWorkPokePartyGet(tis->bw,tis->client_no);
			POKEMON_PARAM	*pp;

			if((BattleWorkFightTypeGet(tis->bw)&FIGHT_TYPE_2vs2)&&
			  ((BattleWorkFightTypeGet(tis->bw)&FIGHT_TYPE_MULTI)==0)){
				client_no=tis->client_no&1;
			}
			else{
				client_no=tis->client_no;
			}

			PokeParty_Init(tis->tps->plist->pp,TEMOTI_POKEMAX);

			for(i=0;i<PokeParty_GetPokeCount(poke_party);i++){
				pp=BattleWorkPokemonParamGet(tis->bw,client_no,tis->list_row[client_no][i]);
				PokeParty_Add(tis->tps->plist->pp,pp);
				tis->tps->plist->list_row[i]=tis->list_row[client_no][i];
			}

			tis->tps->plist->bw = tis->bw;
			tis->tps->plist->heap = HEAPID_BATTLE;
			tis->tps->plist->sel_poke = 0;
			tis->tps->plist->end_flg = 0;
			tis->tps->plist->chg_waza=0;
			tis->tps->plist->mode=BPL_MODE_ITEMUSE;	// �ʏ�̃|�P�����I��
			tis->tps->plist->item=tis->bag_buf->ret_item;
			tis->tps->plist->bag_page=tis->bag_buf->ret_page;
			tis->tps->plist->client_no=tis->bag_buf->client_no;
			tis->tps->plist->cursor_flg = tis->decend_key;
			tis->tps->plist->fight_poke1 = tis->tps->sel_mons_no[tis->client_no];
			tis->tps->plist->fight_poke2 = tis->tps->sel_mons_no[BattleWorkPartnerClientNoGet(tis->bw,tis->client_no)];
			if(tis->client_type==CLIENT_TYPE_C){
				tis->tps->plist->skill_item_use[0]=tis->shutout_flag[BattleWorkPartnerClientNoGet(tis->bw,tis->client_no)];
				tis->tps->plist->skill_item_use[1]=tis->shutout_flag[tis->client_no];	// �Z�Ŏg�p�s��
			}
			else{
				tis->tps->plist->skill_item_use[0]=tis->shutout_flag[tis->client_no];	// �Z�Ŏg�p�s��
				tis->tps->plist->skill_item_use[1]=tis->shutout_flag[BattleWorkPartnerClientNoGet(tis->bw,tis->client_no)];
			}

			BattlePokeList_TaskAdd( tis->tps->plist );
			tis->seq_no++;
		}
		break;
	case SEQ_ITEM_SEL_POKE_LIST_MAIN:
		if(tis->tps->plist->end_flg){
			tis->decend_key = tis->tps->plist->cursor_flg;
			tis->tps->plist->end_flg=0;
			if(tis->tps->plist->sel_poke==BPL_SEL_EXIT){
				tis->seq_no=SEQ_ITEM_SEL_TASK_SET;
			}
			else{
				tis->seq_no=SEQ_ITEM_SEL_END_INIT;
			}
		}
		break;
	case SEQ_ITEM_SEL_END_INIT:
		FightScreenRecover(tis->bw);
		BINPUT_CursorDecendSet(BattleWorkGF_BGL_BIPGet(tis->bw), tis->decend_key);
		PaletteFadeReq(pfd,PF_BIT_MAIN_ALL,0x0c00,BATTLE_BAGLIST_FADE_SPEED,MSG_WINDOW_FADE,0,0x0000);
		PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,BATTLE_BAGLIST_FADE_SPEED,16,0,0x0000);
		tis->seq_no++;
		break;
	case SEQ_ITEM_SEL_END_WAIT:
		if(PaletteFadeCheck(pfd)==0){	//�P�x�ύX�t�F�[�h�I���҂����ĐF�X�폜
			MSG_DsIconFlashReq(BattleWorkMsgIconGet(tis->bw),MSG_DSI_REQ_START);
	
			if(tis->bag_buf->ret_item){
				tis->seq_no=SEQ_ITEM_SEL_USE_MSG;
				switch(tis->bag_buf->ret_page){
				case BBAG_POKE_STRCV:	// ��ԉ񕜃|�P�b�g
					if((tis->bag_buf->ret_item==ITEM_GENKINOKAKERA)||
					   (tis->bag_buf->ret_item==ITEM_GENKINOKATAMARI)){
						tis->seq_no=SEQ_ITEM_SEL_END;
					}
					else if(((tis->tps->plist->sel_poke<2)&&
						    ((BattleWorkFightTypeGet(tis->bw)==FIGHT_TYPE_2vs2_TRAINER)||
							 (BattleWorkFightTypeGet(tis->bw)==FIGHT_TYPE_TAG_BATTLE)))||
						(tis->tps->plist->sel_poke<1)){
						if(tis->bag_buf->ret_item==ITEM_KAIHUKUNOKUSURI){
							if(BattleWorkConfigWazaEffectOnOffCheck(tis->bw)==TRUE){
								tis->next_seq_no=SEQ_ITEM_SEL_HPRCV_EFFECT_INIT1;
							}
							else{
								Snd_SePlayPan(BSE_KAIHUKU,WAZA_SE_PAN_L);
								tis->next_seq_no=SEQ_ITEM_SEL_HPRCV_EFFECT_INIT3;
							}
						}
						else{
							if(BattleWorkConfigWazaEffectOnOffCheck(tis->bw)==TRUE){
								tis->next_seq_no=SEQ_ITEM_SEL_CONDRCV_EFFECT_INIT1;
							}
							else{
								Snd_SePlayPan(BSE_KAIHUKU,WAZA_SE_PAN_L);
								tis->next_seq_no=SEQ_ITEM_SEL_CONDRCV_EFFECT_MSG;
							}
						}
					}
					else{
						tis->seq_no=SEQ_ITEM_SEL_END;
					}
					break;
				case BBAG_POKE_HPRCV:	// HP�񕜃|�P�b�g
					if((((tis->tps->plist->sel_poke<2)&&
					    ((BattleWorkFightTypeGet(tis->bw)==FIGHT_TYPE_2vs2_TRAINER)||
						 (BattleWorkFightTypeGet(tis->bw)==FIGHT_TYPE_TAG_BATTLE)))||
						 (tis->tps->plist->sel_poke<1))&&
						 (ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_HP_RCV,HEAPID_BATTLE))){
						if(BattleWorkConfigWazaEffectOnOffCheck(tis->bw)==TRUE){
							tis->next_seq_no=SEQ_ITEM_SEL_HPRCV_EFFECT_INIT1;
						}
						else{
							Snd_SePlayPan(BSE_KAIHUKU,WAZA_SE_PAN_L);
							tis->next_seq_no=SEQ_ITEM_SEL_HPRCV_EFFECT_INIT3;
						}
					}
					else{
						tis->seq_no=SEQ_ITEM_SEL_END;
					}
					break;
				case BBAG_POKE_BATTLE:	// �퓬�p�|�P�b�g
					if((tis->bag_buf->ret_item==ITEM_PIPPININGYOU)||
					   (tis->bag_buf->ret_item==ITEM_ENEKONOSIPPO)){
						tis->seq_no=SEQ_ITEM_SEL_END;
					}
					else{
						if(tis->bag_buf->ret_item==ITEM_EFEKUTOGAADO){
							if(BattleWorkConfigWazaEffectOnOffCheck(tis->bw)==TRUE){
								tis->next_seq_no=SEQ_ITEM_SEL_BATTLE_EFFECT_INIT2;
							}
							else{
								Snd_SePlayPan(BSE_KAIHUKU,WAZA_SE_PAN_L);
								tis->next_seq_no=SEQ_ITEM_SEL_BATTLE_EFFECT_MSG;
							}
						}
						else{
							if(BattleWorkConfigWazaEffectOnOffCheck(tis->bw)==TRUE){
								tis->next_seq_no=SEQ_ITEM_SEL_BATTLE_EFFECT_INIT1;
							}
							else{
								Snd_SePlayPan(BSE_KAIHUKU,WAZA_SE_PAN_L);
								tis->next_seq_no=SEQ_ITEM_SEL_BATTLE_EFFECT_MSG;
							}
						}
					}
					break;
				case BBAG_POKE_BALL:	// �{�[���|�P�b�g
					tis->seq_no=SEQ_ITEM_SEL_END;
					break;
				}
			}
			else{
				tis->seq_no=SEQ_ITEM_SEL_END;
			}
			//�A�C�e���g�p�G�t�F�N�g���o����悤�ɃI�[�o�[���C�؂�ւ�
			if(tis->seq_no!=SEQ_ITEM_SEL_END){
				FightOverlaySwitch(tis->bw,FIGHT_OVERLAY_AI_TO_SKILL);
			}
		}
		break;
	case SEQ_ITEM_SEL_END:
		{
			ITEM_SELECT_RETURN_PARAM	isrp;

			if(tis->bag_buf->ret_item==0){
				isrp.item_no=SELECT_CANCEL;
			}
			else{
				isrp.item_no=tis->bag_buf->ret_item;
				isrp.page=tis->bag_buf->ret_page;
				if((tis->bag_buf->ret_page==BBAG_POKE_STRCV)||
				   (tis->bag_buf->ret_page==BBAG_POKE_HPRCV)){
					isrp.sel_mons_no=SELECT_POKE_1+tis->tps->plist->list_row[tis->tps->plist->sel_poke];
				}
			}
			SCIO_ItemSelectReturnSet(tis->bw,tis->client_no,isrp);
			SCIO_ServerQueClear(tis->bw,tis->client_no,tis->command_code);
			sys_FreeMemoryEz(tis->tps->plist->pp);
			sys_FreeMemoryEz(tis->tps->plist);
			sys_FreeMemoryEz(tis->tps);
			sys_FreeMemoryEz(tis->bag_buf);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;

	//�A�C�e���g�p���b�Z�[�W
	case SEQ_ITEM_SEL_USE_MSG:
		{
			MSGDATA_MANAGER		*msg_m;
			MESSAGE_PARAM		mp;

			mp.msg_id=Dougu_UseMsg;
			mp.msg_tag=TAG_ITEM;
			mp.msg_para[0]=tis->bag_buf->ret_item;

			msg_m=BattleWorkFightMsgGet(tis->bw);
			tis->msg_index=BattleMSG_Print(tis->bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(tis->bw));
			tis->wait=MSG_WAIT;

			tis->seq_no++;
		}
		break;
	case SEQ_ITEM_SEL_USE_MSG_WAIT:
		if(GF_MSG_PrintEndCheck(tis->msg_index)==0){
			if(--tis->wait==0){
				tis->seq_no=tis->next_seq_no;
			}
		}
		break;
	
	//�퓬�A�C�e���g�p�G�t�F�N�g
	case SEQ_ITEM_SEL_BATTLE_EFFECT_INIT1:
		{
			WAZA_EFFECT_PARAM	wep;

			SCIO_EffectDataSet(tis->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_ITEM_TR,tis->client_no,tis->client_no,NULL);
			CT_WesCall(tis->bw,BattleWorkClientParamGet(tis->bw,tis->client_no),BattleWorkWE_SYS_PTRGet(tis->bw),&wep);
		}
		tis->seq_no++;
		break;

	case SEQ_ITEM_SEL_HPRCV_EFFECT_INIT1:
	case SEQ_ITEM_SEL_CONDRCV_EFFECT_INIT1:
		{
			WAZA_EFFECT_PARAM	wep;
			int					client_no;

			client_no=tis->tps->plist->sel_poke*2;
			SCIO_EffectDataSet(tis->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_ITEM_TR,client_no,client_no,NULL);
			CT_WesCall(tis->bw,BattleWorkClientParamGet(tis->bw,client_no),BattleWorkWE_SYS_PTRGet(tis->bw),&wep);
		}
		tis->seq_no++;
		break;

	//�X�e�[�^�X�㏸�G�t�F�N�g
	case SEQ_ITEM_SEL_BATTLE_EFFECT_INIT2:
		{
			WAZA_EFFECT_PARAM	wep;

			if(tis->bag_buf->ret_item==ITEM_EFEKUTOGAADO){
				SCIO_EffectDataSet(tis->bw,NULL,&wep,EFFTYPE_WAZA,NULL,tis->client_no,tis->client_no,WAZANO_SIROIKIRI);
				CT_WesCall(tis->bw,BattleWorkClientParamGet(tis->bw,tis->client_no),BattleWorkWE_SYS_PTRGet(tis->bw),&wep);
			}
			else if(tis->bag_buf->ret_item==ITEM_KURITHIKATTAA){
				SCIO_EffectDataSet(tis->bw,NULL,&wep,EFFTYPE_WAZA,NULL,tis->client_no,tis->client_no,WAZANO_KIAIDAME);
				CT_WesCall(tis->bw,BattleWorkClientParamGet(tis->bw,tis->client_no),BattleWorkWE_SYS_PTRGet(tis->bw),&wep);
			}
			else{
				SCIO_EffectDataSet(tis->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_UP,tis->client_no,tis->client_no,NULL);
				CT_WesCall(tis->bw,BattleWorkClientParamGet(tis->bw,tis->client_no),BattleWorkWE_SYS_PTRGet(tis->bw),&wep);
			}
			tis->seq_no++;
		}
		break;
	case SEQ_ITEM_SEL_BATTLE_EFFECT_MSG:
		{
			MSGDATA_MANAGER		*msg_m;
			MESSAGE_PARAM		mp;

			mp.msg_id=Dougu_StatusUpMsg;
			mp.msg_tag=TAG_NICK_STAT;
			mp.msg_para[0]=tis->client_no|(tis->tps->sel_mons_no[tis->client_no]<<8);
			switch(tis->bag_buf->ret_item){
			case ITEM_PURASUPAWAA:
				mp.msg_para[1]=COND_POW;
				break;
			case ITEM_DHIFENDAA:
				mp.msg_para[1]=COND_DEF;
				break;
			case ITEM_SUPIIDAA:
				mp.msg_para[1]=COND_AGI;
				break;
			case ITEM_YOKUATAARU:
				mp.msg_para[1]=COND_HIT;
				break;
			case ITEM_SUPESYARUAPPU:
				mp.msg_para[1]=COND_SPEPOW;
				break;
			case ITEM_SUPESYARUGAADO:
				mp.msg_para[1]=COND_SPEDEF;
				break;
			case ITEM_EFEKUTOGAADO:
				mp.msg_id=Dougu_SiroikiriMsg;
				mp.msg_tag=TAG_NONE;
				break;
			case ITEM_KURITHIKATTAA:
				mp.msg_id=Dougu_KiaidameMsg;
				mp.msg_tag=TAG_NICK;
				break;
			}
			msg_m=BattleWorkFightMsgGet(tis->bw);
			tis->msg_index=BattleMSG_Print(tis->bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(tis->bw));
			tis->wait=MSG_WAIT;
		}
		tis->seq_no++;
		break;

	//HP�񕜃G�t�F�N�g
	//��Ԉُ�񕜃G�t�F�N�g
	case SEQ_ITEM_SEL_HPRCV_EFFECT_INIT2:
	case SEQ_ITEM_SEL_CONDRCV_EFFECT_INIT2:
		{
			WAZA_EFFECT_PARAM	wep;
			int					client_no;

			client_no=tis->tps->plist->sel_poke*2;
			SCIO_EffectDataSet(tis->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_RECOVER,client_no,client_no,NULL);
			CT_WesCall(tis->bw,BattleWorkClientParamGet(tis->bw,client_no),BattleWorkWE_SYS_PTRGet(tis->bw),&wep);
		}
		tis->seq_no++;
		break;
	case SEQ_ITEM_SEL_HPRCV_EFFECT_INIT3:
		{
			GAUGE_WORK		*gauge;
			POKEMON_PARAM	*pp;
			int				client_no;
			int				sel_mons_no;

			client_no=tis->tps->plist->sel_poke*2;
			sel_mons_no=tis->tps->plist->list_row[tis->tps->plist->sel_poke];
	
			gauge = BattleWorkGaugeWorkGet(tis->bw,client_no);
			MI_CpuClear8(&gauge->param, sizeof(GAUGE_SUB_PARAM));
			gauge->type = Gauge_TypeGet(BattleWorkClientTypeGet(tis->bw,client_no),BattleWorkFightTypeGet(tis->bw));

			pp=BattleWorkPokemonParamGet(tis->bw,client_no,sel_mons_no);
			gauge->hp = PokeParaGet(pp,ID_PARA_hp,NULL)-tis->tps->plist->hp_rcv;
			gauge->hpmax = PokeParaGet(pp,ID_PARA_hpmax,NULL);
			gauge->damage = tis->tps->plist->hp_rcv;
			if(PokeParaGet(pp,ID_PARA_condition,NULL)==0){
				gauge->status = 0;
			}

			Gauge_InitCalcHP(gauge, gauge->damage);
		}
		tis->seq_no++;
		break;
	case SEQ_ITEM_SEL_HPRCV_EFFECT_WAIT3:
		{
			GAUGE_WORK		*gauge;
			int				client_no;

			client_no=tis->tps->plist->sel_poke*2;
			gauge = BattleWorkGaugeWorkGet(tis->bw,client_no);

			if(Gauge_CalcHP(gauge)==-1){
				Gauge_PartsDraw(gauge,NULL,GAUGE_DRAW_STATUS);
				tis->seq_no++;
			}
		}
		break;
	case SEQ_ITEM_SEL_HPRCV_EFFECT_MSG:
		{
			MSGDATA_MANAGER		*msg_m;
			MESSAGE_PARAM		mp;
			int					client_no;

			msg_m=BattleWorkFightMsgGet(tis->bw);
			client_no=tis->tps->plist->sel_poke*2;

			if(tis->tps->plist->hp_rcv){
				mp.msg_id=Dougu_HpkaihukuMsg;
				mp.msg_tag=TAG_NICK_NUM;
				mp.msg_para[0]=client_no|(tis->tps->sel_mons_no[client_no]<<8);
				mp.msg_para[1]=tis->tps->plist->hp_rcv;
			}
			else{
				mp.msg_id=Dougu_JoutaiKaihukuMsg;
				mp.msg_tag=TAG_NICK;
				mp.msg_para[0]=client_no|(tis->tps->sel_mons_no[client_no]<<8);
			}

			tis->msg_index=BattleMSG_Print(tis->bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(tis->bw));
			tis->wait=MSG_WAIT;

			tis->seq_no++;
		}
		break;

	case SEQ_ITEM_SEL_CONDRCV_EFFECT_MSG:
		{
			GAUGE_WORK		*gauge;
			int				client_no;
			MSGDATA_MANAGER	*msg_m;
			MESSAGE_PARAM	mp;
			int				item_prm;
			int				rcv_cnt=0;
			POKEMON_PARAM	*pp;
			int				sel_mons_no;

			client_no=tis->tps->plist->sel_poke*2;
			gauge = BattleWorkGaugeWorkGet(tis->bw,client_no);
			sel_mons_no=tis->tps->plist->list_row[tis->tps->plist->sel_poke];
			pp=BattleWorkPokemonParamGet(tis->bw,client_no,sel_mons_no);

			if(PokeParaGet(pp,ID_PARA_condition,NULL)==0){
				gauge->status = 0;
			}
			Gauge_PartsDraw(gauge, gauge->hp, GAUGE_DRAW_STATUS);

			mp.msg_tag=TAG_NICK;
			mp.msg_para[0]=client_no|(tis->tps->sel_mons_no[client_no]<<8);

			// �˂ނ��
			if(ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_SLEEP_RCV,HEAPID_BATTLE)){
				item_prm=NEMURI_RCV;
				rcv_cnt++;
			}
			// �ŉ�
			if(ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_POISON_RCV,HEAPID_BATTLE)){
				item_prm=DOKU_RCV;
				rcv_cnt++;
			}
			// �Ώ���
			if(ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_BURN_RCV,HEAPID_BATTLE)){
				item_prm=YAKEDO_RCV;
				rcv_cnt++;
			}
			// �X��
			if(ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_ICE_RCV,HEAPID_BATTLE)){
				item_prm=KOORI_RCV;
				rcv_cnt++;
			}
			// ��჉�
			if(ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_PARALYZE_RCV,HEAPID_BATTLE)){
				item_prm=MAHI_RCV;
				rcv_cnt++;
			}
			// ������
			if(ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_PANIC_RCV,HEAPID_BATTLE)){
				item_prm=KONRAN_RCV;
				rcv_cnt++;
			}
			// ����������
			if(ItemParamGet(tis->bag_buf->ret_item,ITEM_PRM_MEROMERO_RCV,HEAPID_BATTLE)){
				item_prm=MEROMERO_RCV;
				rcv_cnt++;
			}
			if(rcv_cnt!=1){
				mp.msg_id=Dougu_NandemonaosiMsg;
			}
			else{
				switch(item_prm){
				case NEMURI_RCV:
					mp.msg_id=Dougu_NemurikaihukuMsg;
					break;
				case DOKU_RCV:
					mp.msg_id=Dougu_DokukaihukuMsg;
					break;
				case YAKEDO_RCV:
					mp.msg_id=Dougu_YakedokaihukuMsg;
					break;
				case KOORI_RCV:
					mp.msg_id=Dougu_KoorikaihukuMsg;
					break;
				case MAHI_RCV:
					mp.msg_id=Dougu_MahikaihukuMsg;
					break;
				case KONRAN_RCV:
					mp.msg_id=Dougu_KonrankaihukuMsg;
					break;
				case MEROMERO_RCV:
					mp.msg_id=Dougu_MeromerokaihukuMsg;
					break;
				}
			}
			msg_m=BattleWorkFightMsgGet(tis->bw);
			tis->msg_index=BattleMSG_Print(tis->bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(tis->bw));
			tis->wait=MSG_WAIT;
		}
		tis->seq_no++;
		break;

	//�G�t�F�N�g�I���҂�
	case SEQ_ITEM_SEL_BATTLE_EFFECT_WAIT1:
	case SEQ_ITEM_SEL_BATTLE_EFFECT_WAIT2:
	case SEQ_ITEM_SEL_HPRCV_EFFECT_WAIT1:
	case SEQ_ITEM_SEL_HPRCV_EFFECT_WAIT2:
	case SEQ_ITEM_SEL_CONDRCV_EFFECT_WAIT1:
	case SEQ_ITEM_SEL_CONDRCV_EFFECT_WAIT2:
		WES_Executed(BattleWorkWE_SYS_PTRGet(tis->bw));
		if (IsWES_Executed(BattleWorkWE_SYS_PTRGet(tis->bw)) == FALSE){		///< Effect Commond End Check
			WES_Reset(BattleWorkWE_SYS_PTRGet(tis->bw));					///< Sysmte Reset
			tis->seq_no++;
		}
		break;

	//���b�Z�[�W�I���҂�
	case SEQ_ITEM_SEL_BATTLE_EFFECT_MSG_WAIT:
	case SEQ_ITEM_SEL_HPRCV_EFFECT_MSG_WAIT:
	case SEQ_ITEM_SEL_CONDRCV_EFFECT_MSG_WAIT:
		if(GF_MSG_PrintEndCheck(tis->msg_index)==0){
			if(--tis->wait==0){
				//�A�C�e���g�p�G�t�F�N�g���o����悤�ɐ؂�ւ����I�[�o�[���C�����ɖ߂�
				FightOverlaySwitch(tis->bw,FIGHT_OVERLAY_SKILL_TO_AI);
				tis->seq_no=SEQ_ITEM_SEL_END;
			}
		}
		break;

	}
}

//============================================================================================
/**
 *	����I��TCB�iAI�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_ItemSelectAI(TCB_PTR tcb,void *work)
{
	TCB_ITEM_SELECT				*tis=(TCB_ITEM_SELECT *)work;
	ITEM_SELECT_RETURN_PARAM	isrp;

	//�I�������A�C�e���́AAI�p�̃��[�N�ɕێ�����Ă���̂ŁA�����ł́A�_�~�[�̒l�����Ă���
	isrp.item_no=1;
	SCIO_ItemSelectReturnSet(tis->bw,tis->client_no,isrp);
	SCIO_ServerQueClear(tis->bw,tis->client_no,tis->command_code);
	sys_FreeMemoryEz(tis->tps->plist->pp);
	sys_FreeMemoryEz(tis->tps->plist);
	sys_FreeMemoryEz(tis->tps);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	����I��TCB�i�ʐM�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_ItemSelectSIO(TCB_PTR tcb,void *work)
{
	TCB_ITEM_SELECT	*tis=(TCB_ITEM_SELECT *)work;

	SCIO_ServerQueClear(tis->bw,tis->client_no,tis->command_code);
	sys_FreeMemoryEz(tis->tps->plist->pp);
	sys_FreeMemoryEz(tis->tps->plist);
	sys_FreeMemoryEz(tis->tps);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	����I��TCB�i�ΐ�^��Đ��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_ItemSelectAuto(TCB_PTR tcb,void *work)
{
	TCB_ITEM_SELECT				*tis=(TCB_ITEM_SELECT *)work;
	ITEM_SELECT_RETURN_PARAM	isrp;
	REC_DATA					data;

	if(BattleWorkBattlePlay(tis->bw,tis->client_no,&data)==TRUE){
		BattleRecDataFullStop(tis->bw);
	}
	isrp.item_no=data;
	if(BattleWorkBattlePlay(tis->bw,tis->client_no,&data)==TRUE){
		BattleRecDataFullStop(tis->bw);
	}
	isrp.item_no|=(data<<8);
	if(BattleWorkBattlePlay(tis->bw,tis->client_no,&data)==TRUE){
		BattleRecDataFullStop(tis->bw);
	}
	isrp.page=data&0x0f;
	isrp.sel_mons_no=(data&0xf0)>>8;

	SCIO_ItemSelectReturnSet(tis->bw,tis->client_no,isrp);
	SCIO_ServerQueClear(tis->bw,tis->client_no,tis->command_code);
	sys_FreeMemoryEz(tis->tps->plist->pp);
	sys_FreeMemoryEz(tis->tps->plist);
	sys_FreeMemoryEz(tis->tps);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�|�P�����I��TCB�i�v���C���[�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonSelect(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_SELECT	*tps=(TCB_POKEMON_SELECT *)work;
	PALETTE_FADE_PTR	pfd;

	pfd=BattleWorkPfdGet(tps->bw);

	switch(tps->seq_no){
	case 0:		//���X�g��ʂւ̃t�F�[�h
		//���b�Z�[�W�E�C���h���N���A
		{
			GF_BGL_BMPWIN	*win=BattleWorkGF_BGL_BMPWINGet(tps->bw,TALK_WIN_NO);

			GF_BGL_BmpWinDataFill(win,0xff);
			GF_BGL_BmpWinCgxOn(win);
		}

		//BIP����������܂��ɕK�v�ȏ�񂾂��擾
		tps->decend_key = BINPUT_CursorDecendGet(BattleWorkGF_BGL_BIPGet(tps->bw));

		MSG_DsIconFlashReq(BattleWorkMsgIconGet(tps->bw),MSG_DSI_REQ_STOP);
		PaletteFadeReq(pfd,PF_BIT_MAIN_ALL,0x0c00,BATTLE_BAGLIST_FADE_SPEED,0,MSG_WINDOW_FADE,0x0000);
		PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,BATTLE_BAGLIST_FADE_SPEED,0,16,0x0000);
		tps->seq_no++;
		break;
	case 1:
		if(PaletteFadeCheck(pfd)==0){	//�P�x�ύX�t�F�[�h�I���҂����ĐF�X�폜
			{
				int				i;
				int				cnt=0;
				int				client_no;
				int				cli1,cli2;
				POKEPARTY		*poke_party;
				POKEMON_PARAM	*pp,*pp2;

				FightScreenTransition(tps->bw);

				tps->plist=sys_AllocMemory(HEAPID_BATTLE,sizeof(BPLIST_DATA));
				tps->plist->pp=PokeParty_AllocPartyWork(HEAPID_BATTLE);

				if(((BattleWorkFightTypeGet(tps->bw)&(FIGHT_TYPE_SIO|FIGHT_TYPE_MULTI))==(FIGHT_TYPE_SIO|FIGHT_TYPE_MULTI))||
					(BattleWorkFightTypeGet(tps->bw)==FIGHT_TYPE_BATTLE_TOWER_AI_MULTI)){
					if(BattleWorkClientTypeGet(tps->bw,tps->client_no)==CLIENT_TYPE_A){
						cli1=tps->client_no;
						cli2=BattleWorkPartnerClientNoGet(tps->bw,tps->client_no);
					}
					else{
						cli1=BattleWorkPartnerClientNoGet(tps->bw,tps->client_no);
						cli2=tps->client_no;
					}

					pp=PokemonParam_AllocWork(HEAPID_BATTLE);
					for(i=0;i<POKEMON_TEMOTI_MAX;i++){
						PokeParty_Add(tps->plist->pp,pp);
					}
					sys_FreeMemoryEz(pp);
			
					for(i=0;i<BattleWorkPokeCountGet(tps->bw,cli1);i++){
						pp=BattleWorkPokemonParamGet(tps->bw,cli1,tps->list_row[cli1][i]);
						pp2=PokeParty_GetMemberPointer(tps->plist->pp,i*2);
						PokeCopyPPtoPP(pp,pp2);
						tps->plist->list_row[i*2]=tps->list_row[cli1][i];
					}

					for(i=0;i<BattleWorkPokeCountGet(tps->bw,cli2);i++){
						pp=BattleWorkPokemonParamGet(tps->bw,cli2,tps->list_row[cli2][i]);
						pp2=PokeParty_GetMemberPointer(tps->plist->pp,i*2+1);
						PokeCopyPPtoPP(pp,pp2);
						tps->plist->list_row[i*2+1]=tps->list_row[cli2][i];
					}
					if(BattleWorkClientTypeGet(tps->bw,tps->client_no)==CLIENT_TYPE_C){
						tps->plist->sel_poke = 1;
					}
					else{
						tps->plist->sel_poke = 0;
					}
				}
				else{
					if((BattleWorkFightTypeGet(tps->bw)&FIGHT_TYPE_2vs2)&&
					  ((BattleWorkFightTypeGet(tps->bw)&FIGHT_TYPE_MULTI)==0)){
						client_no=tps->client_no&1;
					}
					else{
						client_no=tps->client_no;
					}
					if(BattleWorkClientTypeGet(tps->bw,tps->client_no)==CLIENT_TYPE_C){
						tps->plist->sel_poke = 1;
					}
					else{
						tps->plist->sel_poke = 0;
					}

					poke_party=BattleWorkPokePartyGet(tps->bw,tps->client_no);
			
					for(i=0;i<PokeParty_GetPokeCount(poke_party);i++){
						pp=BattleWorkPokemonParamGet(tps->bw,client_no,tps->list_row[client_no][i]);
						PokeParty_Add(tps->plist->pp,pp);
						tps->plist->list_row[i]=tps->list_row[client_no][i];
					}
				}

				tps->plist->bw = tps->bw;
				tps->plist->heap = HEAPID_BATTLE;
				tps->plist->end_flg = 0;
				tps->plist->chg_waza=tps->chg_waza;
				tps->plist->double_sel=tps->double_sel;
				tps->plist->mode=tps->list_mode;
				tps->plist->item=tps->item;
				tps->plist->client_no=tps->client_no;
				tps->plist->cursor_flg=tps->decend_key;
				if((tps->no_reshuffle_client&No2Bit(tps->client_no))==0){
					tps->plist->fight_poke1 = tps->sel_mons_no[tps->client_no];
				}
				else{
					tps->plist->fight_poke1 = NO_SEL_MONS;
				}
				if(BattleWorkFightTypeGet(tps->bw)&FIGHT_TYPE_MULTI){
					tps->plist->fight_poke2 = NO_SEL_MONS;
				}
				else if((tps->no_reshuffle_client&No2Bit(BattleWorkPartnerClientNoGet(tps->bw,tps->client_no)))==0){
					tps->plist->fight_poke2 = tps->sel_mons_no[BattleWorkPartnerClientNoGet(tps->bw,tps->client_no)];
				}
				else{
					tps->plist->fight_poke2 = NO_SEL_MONS;
				}
				
				BattlePokeList_TaskAdd( tps->plist );
				tps->seq_no++;
			}
		}
		break;
	case 2:
		if(tps->plist->end_flg){
			FightScreenRecover(tps->bw);
			BINPUT_CursorDecendSet(BattleWorkGF_BGL_BIPGet(tps->bw), tps->plist->cursor_flg);
			PaletteFadeReq(pfd,PF_BIT_MAIN_ALL,0x0c00,BATTLE_BAGLIST_FADE_SPEED,MSG_WINDOW_FADE,0,0x0000);
			PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,BATTLE_BAGLIST_FADE_SPEED,16,0,0x0000);
			tps->seq_no++;
		}
		break;
	case 3:
		if(PaletteFadeCheck(pfd)==0){	//�P�x�ύX�t�F�[�h�I���҂����ĐF�X�폜
			MSG_DsIconFlashReq(BattleWorkMsgIconGet(tps->bw),MSG_DSI_REQ_START);
			if(tps->plist->sel_poke==BPL_SEL_EXIT){
				SCIO_PokemonSelectReturnSet(tps->bw,tps->client_no,SELECT_CANCEL);
			}
			else{
				SCIO_PokemonSelectReturnSet(tps->bw,tps->client_no,SELECT_POKE_1+tps->plist->list_row[tps->plist->sel_poke]);
			}
			SCIO_ServerQueClear(tps->bw,tps->client_no,tps->command_code);
			sys_FreeMemoryEz(tps->plist->pp);
			sys_FreeMemoryEz(tps->plist);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
	}
}

//============================================================================================
/**
 *	�|�P�����I��TCB�iAI�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonSelectAI(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_SELECT	*tps=(TCB_POKEMON_SELECT *)work;
	int					pos;
	POKEPARTY			*poke_party;
	POKEMON_PARAM		*pp;
	int					client_no,client_pair_no;
	u32					fight_type;

	fight_type=BattleWorkFightTypeGet(tps->bw);

	client_no=tps->client_no;
	if((fight_type&FIGHT_TYPE_TAG)||
	   (fight_type&FIGHT_TYPE_MULTI)){
		client_pair_no=client_no;
	}
	else{
		client_pair_no=BattleWorkPartnerClientNoGet(tps->bw,client_no);
	}

//�v���`�i�ł�Wifi��AI���ڂ����̂ŁA���̏����͕K�v�Ȃ��͂�
#if 0
#ifdef PM_DEBUG
	if(BattleWorkBattleStatusFlagGet(tps->bw)&BATTLE_STATUS_FLAG_AUTO_BATTLE){
		//�ʐM�ڑ��ł́A�q�@����AI���̂��Ă��Ȃ��̂ŁA�擪����T�[�`����HP�̂���|�P�������J��o��
//		if(CommStateIsWifiConnect()){
		if(fight_type&FIGHT_TYPE_SIO){
			poke_party=BattleWorkPokePartyGet(tps->bw,tps->client_no);
			for(pos=0;pos<PokeParty_GetPokeCount(poke_party);pos++){
				pp=BattleWorkPokemonParamGet(tps->bw,tps->client_no,pos);
				if((PokeParaGet(pp,ID_PARA_hp,NULL))&&
				   (tps->sel_mons_no[client_no]!=pos)&&
				   (tps->sel_mons_no[client_pair_no]!=pos)){
					break;
				}
			}
			SCIO_PokemonSelectReturnSet(tps->bw,tps->client_no,SELECT_POKE_1+pos);
			SCIO_ServerQueClear(tps->bw,tps->client_no,tps->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
			return;
		}
	}
#endif PM_DEBUG
#endif

	pos=ClientAIReshuffleSelMonsNoGet(tps->bw,client_no);
	if(pos==6){
		pos=ClientAIPokeSelectAI(tps->bw,client_no);
		//AI�����߂�Ȃ���ΐ擪����T�[�`����HP�̂���|�P�������J��o��
		if(pos==6){
			poke_party=BattleWorkPokePartyGet(tps->bw,tps->client_no);
			for(pos=0;pos<PokeParty_GetPokeCount(poke_party);pos++){
				pp=BattleWorkPokemonParamGet(tps->bw,tps->client_no,pos);
				if((PokeParaGet(pp,ID_PARA_hp,NULL))&&
				   (tps->sel_mons_no[client_no]!=pos)&&
				   (tps->sel_mons_no[client_pair_no]!=pos)){
					break;
				}
			}
		}
	}
#ifdef PM_DEBUG
#ifndef DEBUG_PRINT_BATTLE
	if(DebugBattlePokemonReshuffle(tps->bw,tps->client_no,&pos)==FALSE){
		return;
	}
#endif
#endif
	SCIO_PokemonSelectReturnSet(tps->bw,tps->client_no,SELECT_POKE_1+pos);
	SCIO_ServerQueClear(tps->bw,tps->client_no,tps->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�|�P�����I��TCB�i�ʐM�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonSelectSIO(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_SELECT	*tps=(TCB_POKEMON_SELECT *)work;

	SCIO_ServerQueClear(tps->bw,tps->client_no,tps->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�|�P�����I��TCB�i�ΐ�^��Đ��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonSelectAuto(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_SELECT	*tps=(TCB_POKEMON_SELECT *)work;
	REC_DATA			data;

	//�^��f�[�^�̐��������`�F�b�N
	if(BattleWorkBattlePlay(tps->bw,tps->client_no,&data)==TRUE){
		BattleRecDataFullStop(tps->bw);
	}
	//�R�}���h���Ȃ�
	if((!data)||
	//���ł�
	//�R�}���h���͈͊O
	   (data>POKEMON_TEMOTI_MAX)){
		BattleRecDataErrorStop(tps->bw);
	}
	else{
		//�I�������|�P�����̐��������`�F�b�N
		{
			POKEPARTY		*poke_party;
			POKEMON_PARAM	*pp;
			int				sel_mons_no=data-1;
	
			poke_party=BattleWorkPokePartyGet(tps->bw,tps->client_no);
				
			//���łɑI�����Ă���|�P�������w��
			if((sel_mons_no==tps->sel_mons_no[tps->client_no])||
			   //(sel_mons_no==tps->sel_mons_no[BattleWorkPartnerClientNoGet(tps->bw,tps->client_no)])||
			   (sel_mons_no==tps->double_sel)){
				BattleRecDataErrorStop(tps->bw);
			}
			//���݂��Ȃ��ꏊ���w��
			else if(data>PokeParty_GetPokeCount(poke_party)){
				BattleRecDataErrorStop(tps->bw);
			}
			else{
				pp=BattleWorkPokemonParamGet(tps->bw,tps->client_no,data-1);
				//�C�₵�Ă���|�P�������w��
				if(PokeParaGet(pp,ID_PARA_hp,NULL)==0){
					BattleRecDataErrorStop(tps->bw);
				}
				//�^�}�S��I��
				if(PokeParaGet(pp,ID_PARA_monsno_egg,NULL)==MONSNO_TAMAGO){
					BattleRecDataErrorStop(tps->bw);
				}
			}
		}
	}

	SCIO_PokemonSelectReturnSet(tps->bw,tps->client_no,data);
	SCIO_ServerQueClear(tps->bw,tps->client_no,tps->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	Yes/No�I��TCB�i�v���C���[�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_YesNoSelect(TCB_PTR tcb,void *work)
{
	TCB_YES_NO_SELECT	*tyns = work;
	GF_BGL_INI	*bgl=BattleWorkGF_BGL_INIGet(tyns->bw);
	BI_PARAM_PTR bip = BattleWorkGF_BGL_BIPGet(tyns->bw);
	int partner_client;
	GAUGE_WORK *partner_gauge;
	CLIENT_PARAM *cp;
	
	enum{	//�V�[�P���X�ԍ�
		CSSEQ_ESCAPE_SELECT_INIT,
		CSSEQ_ESCAPE_SELECT_WAIT,
		CSSEQ_ESCAPE_SELECT,
		CSSEQ_ESCAPE_SELECT_EFFWAIT,
		
		CSSEQ_FINISH,
	};

	cp=BattleWorkClientParamGet(tyns->bw,tyns->client_no);
	partner_client = BattleWorkPartnerClientNoGet(tyns->bw, tyns->client_no);
	if(partner_client != tyns->client_no){
		partner_gauge = BattleWorkGaugeWorkGet(tyns->bw, partner_client);
	}
	else{
		partner_gauge = NULL;
	}
	
	switch(tyns->seq_no){
	case CSSEQ_ESCAPE_SELECT_INIT:
		if(BINPUT_EffectEndCheck(bip) == FALSE){
			break;
		}

		if(tyns->msg_id){
			{
				GF_BGL_BMPWIN		*win;
				MSGDATA_MANAGER		*msg_m;
				MESSAGE_PARAM		mp;
	
				msg_m=BattleWorkFightMsgGet(tyns->bw);
				if(tyns->flag==YNTYPE_NICK_NAME){
					mp.msg_tag=TAG_NICK|TAG_NO_DIR;
					mp.msg_para[0]=tyns->nickname;
				}
				else{
					mp.msg_tag=TAG_NONE;
				}
				mp.msg_id=tyns->msg_id;
				tyns->msg_index=BattleMSG_Print(tyns->bw,msg_m,&mp,BattleWorkConfigMsgSpeedGet(tyns->bw));
			}
		}
		tyns->seq_no = CSSEQ_ESCAPE_SELECT_WAIT;
		break;
	case CSSEQ_ESCAPE_SELECT_WAIT:
		if((GF_MSG_PrintEndCheck(tyns->msg_index)==0)||(tyns->msg_id==NULL)){
			
			{
				ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
				ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
				BINPUT_SCENE_YESNO bsy;
				
				BINPUT_StockBallOFF(bip);
				
				bsy.waza_no = tyns->waza_no;
				switch(tyns->flag){
				case YNTYPE_YES_NO:
				case YNTYPE_NICK_NAME:
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_YESNO, FALSE, &bsy);
					break;
				case YNTYPE_WASURE:
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WASURERU, FALSE, &bsy);
					break;
				case YNTYPE_AKIRAME:
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_AKIRAMERU, FALSE, &bsy);
					break;
				case YNTYPE_NEXT_POKEMON:
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_NEXT_POKEMON, FALSE, &bsy);
					break;
				case YNTYPE_CHANGE_POKEMON:
					BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_CHANGE_POKEMON, FALSE, &bsy);
					break;
				default:
					GF_ASSERT(0);
					break;
				}
				tyns->seq_no = CSSEQ_ESCAPE_SELECT;
				
				ArchiveDataHandleClose( hdl_bg );
				ArchiveDataHandleClose( hdl_obj );			
			}
		}
		break;
	case CSSEQ_ESCAPE_SELECT:
		tyns->tp_ret = BINPUT_TouchCheck(bip);
		if(tyns->tp_ret != RECT_HIT_NONE){
			Snd_SePlay(BSE_TP_DECIDE);
			tyns->seq_no = CSSEQ_ESCAPE_SELECT_EFFWAIT;
		}
		break;
	case CSSEQ_ESCAPE_SELECT_EFFWAIT:
		if(BINPUT_EffectEndCheck(bip) == TRUE){
			{
				ARCHANDLE* hdl_bg  = ArchiveDataHandleOpen( ARC_BATT_BG,  HEAPID_BATTLE ); 
				ARCHANDLE* hdl_obj = ArchiveDataHandleOpen( ARC_BATT_OBJ, HEAPID_BATTLE );
				
				Gauge_AnimeStop(tyns->gauge);
				Poke_SinCurveStop(cp);
				Gauge_ColorNormalSet(partner_gauge);
				BINPUT_StockBallOFF(bip);
				BINPUT_CreateBG(hdl_bg, hdl_obj, bip, BINPUT_TYPE_WALL, FALSE, NULL);
				if(tyns->tp_ret == TRUE){
					BINPUT_BackFadeReq(bip, BINPUT_BACKFADE_LIGHT);
				}
				//tyns->tp_ret = SELECT_ESCAPE_COMMAND;
				tyns->seq_no = CSSEQ_FINISH;
				
				ArchiveDataHandleClose( hdl_bg );
				ArchiveDataHandleClose( hdl_obj );			
			}
		}
		break;

	case CSSEQ_FINISH:
		if(BINPUT_BackFadeExeCheck(bip) == TRUE){
			SCIO_CommandSelectReturnSet(tyns->bw,tyns->client_no,tyns->tp_ret);
			SCIO_ServerQueClear(tyns->bw,tyns->client_no,tyns->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
	}
}

//============================================================================================
/**
 *	Yes/No�I��TCB�iAI�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_YesNoSelectAI(TCB_PTR tcb,void *work)
{
	TCB_YES_NO_SELECT	*tyns=(TCB_YES_NO_SELECT *)work;

	SCIO_ServerQueClear(tyns->bw,tyns->client_no,tyns->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	Yes/No�I��TCB�i�ʐM�p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_YesNoSelectSIO(TCB_PTR tcb,void *work)
{
	TCB_YES_NO_SELECT	*tyns=(TCB_YES_NO_SELECT *)work;

	SCIO_ServerQueClear(tyns->bw,tyns->client_no,tyns->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	Yes/No�I��TCB�i�ΐ�^��Đ��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_YesNoSelectAuto(TCB_PTR tcb,void *work)
{
	TCB_YES_NO_SELECT	*tyns=(TCB_YES_NO_SELECT *)work;
	REC_DATA			data;

	//�^��f�[�^�̐��������`�F�b�N
	if(BattleWorkBattlePlay(tyns->bw,tyns->client_no,&data)==TRUE){
		BattleRecDataFullStop(tyns->bw);
	}
	//�R�}���h���Ȃ�
	if((!data)||
	//�R�}���h���͈͊O
	  ((data!=SELECT_CANCEL)&&(data!=TRUE))){
		BattleRecDataErrorStop(tyns->bw);
	}

	SCIO_CommandSelectReturnSet(tyns->bw,tyns->client_no,data);
	SCIO_ServerQueClear(tyns->bw,tyns->client_no,tyns->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
}

//============================================================================================
/**
 *	�Z�G�t�F�N�gTCB
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
enum{
	SEQ_WE_MIGAWARI_CHECK=0,
	SEQ_WE_MIGAWARI2POKE_WAIT,
	SEQ_WE_POKE_IN_SET,
	SEQ_WE_POKE_IN_WAIT,
	SEQ_WE_WAZA_EFFECT,
	SEQ_WE_WAZA_EFFECT_WAIT,
	SEQ_WE_POKE2MIGAWARI_SET,
	SEQ_WE_POKE2MIGAWARI_WAIT,
	SEQ_WE_MIGAWARI_IN_SET,
	SEQ_WE_MIGAWARI_IN_WAIT,
	SEQ_WE_WAZA_EFFECT_END
};
static	void	TCB_WazaEffect(TCB_PTR tcb,void *work)
{
	TCB_WAZA_EFFECT	*twe=(TCB_WAZA_EFFECT *)work;

	switch(twe->seq_no){
	case SEQ_WE_MIGAWARI_CHECK:
		if((twe->wep.migawari_flag)&&
		   (twe->wep.flag==EFFTYPE_WAZA)&&
		   (SoftSpriteParaGet(twe->ss,SS_PARA_VANISH)==0)){
			{
				WAZA_EFFECT_PARAM	wep;

				SCIO_EffectDataSet(twe->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_OUT,twe->client_no,twe->client_no,NULL);
				CT_WesCall(twe->bw,twe->cp,twe->wsp,&wep);
			}
			twe->seq_no=SEQ_WE_MIGAWARI2POKE_WAIT;
		}
		else{
			twe->seq_no=SEQ_WE_WAZA_EFFECT;
		}
		break;

	case SEQ_WE_POKE_IN_SET:
		{
			WAZA_POKE_CHANGE_PARAM	wpcp;
			WAZA_EFFECT_PARAM		wep;

			CT_WazaPokeChangeParamMake(twe->bw,&twe->wep,&wpcp,twe->client_no);
			HenshinChrPop(&wpcp,HEAPID_BATTLE);

			SCIO_EffectDataSet(twe->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_IN,twe->client_no,twe->client_no,NULL);
			CT_WesCall(twe->bw,twe->cp,twe->wsp,&wep);
			twe->cp->migawari_flag=0;
		}
		twe->seq_no++;
		break;

	case SEQ_WE_WAZA_EFFECT:
		BattleWorkPinchSEFlagSet(twe->bw,2);
		if(twe->we_on_off_flag){
			BattleWorkGaugeOff(twe->bw);
		}
		if(twe->we_shadow_flag){
			SoftSpriteSSSystemFlagSet(BattleWorkSoftSpriteManagerGet(twe->bw),SS_SYSTEM_FLAG_SHADOW_OFF);
		}
		CT_WesCall(twe->bw,twe->cp,twe->wsp,&twe->wep);
		twe->seq_no++;
		break;

	case SEQ_WE_POKE2MIGAWARI_SET:
		BattleWorkPinchSEFlagSet(twe->bw,0);
		if(twe->we_on_off_flag){
			BattleWorkGaugeOn(twe->bw);
		}
		if(twe->we_shadow_flag){
			SoftSpriteSSSystemFlagReset(BattleWorkSoftSpriteManagerGet(twe->bw),SS_SYSTEM_FLAG_SHADOW_OFF);
		}
		if((twe->wep.migawari_flag)&&
		   (twe->wep.flag==EFFTYPE_WAZA)&&
		   (SoftSpriteParaGet(twe->ss,SS_PARA_VANISH)==0)){
			{
				WAZA_EFFECT_PARAM	wep;

				SCIO_EffectDataSet(twe->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_OUT,twe->client_no,twe->client_no,NULL);
				CT_WesCall(twe->bw,twe->cp,twe->wsp,&wep);
			}
			twe->seq_no=SEQ_WE_POKE2MIGAWARI_WAIT;
		}
		else{
			twe->seq_no=SEQ_WE_WAZA_EFFECT_END;
		}
		break;

	case SEQ_WE_MIGAWARI_IN_SET:
		{
			WAZA_POKE_CHANGE_PARAM	wpcp;
			WAZA_EFFECT_PARAM		wep;

			CT_WazaPokeChangeParamMake(twe->bw,&twe->wep,&wpcp,twe->client_no);
			MigawariChrSet(&wpcp,HEAPID_BATTLE);

			SCIO_EffectDataSet(twe->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_IN,twe->client_no,twe->client_no,NULL);
			CT_WesCall(twe->bw,twe->cp,twe->wsp,&wep);
			twe->cp->migawari_flag=1;
		}
		twe->seq_no++;
		break;

	case SEQ_WE_WAZA_EFFECT_END:
		SCIO_ServerQueClear(twe->bw,twe->client_no,twe->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;

	//�������̃G�t�F�N�g�I���҂�
	case SEQ_WE_MIGAWARI2POKE_WAIT:
	case SEQ_WE_POKE_IN_WAIT:
	case SEQ_WE_WAZA_EFFECT_WAIT:
	case SEQ_WE_POKE2MIGAWARI_WAIT:
	case SEQ_WE_MIGAWARI_IN_WAIT:
		WES_Executed(twe->wsp);
		if (IsWES_Executed(twe->wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(twe->wsp);						///< Sysmte Reset
			twe->seq_no++;
		}
		break;
	}
}

//============================================================================================
/**
 *	�|�P�����_���[�W�u�����N
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonBlink(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_BLINK	*tpb=(TCB_POKEMON_BLINK *)work;
	int					vanish;

	if(tpb->seq_no<6){
		if(tpb->wait==0){
			tpb->wait=BLINK_WAIT;
			tpb->seq_no++;
			vanish=SoftSpriteParaGet(tpb->ss,SS_PARA_VANISH);
			vanish^=1;
			SoftSpriteParaSet(tpb->ss,SS_PARA_VANISH,vanish);
		}
		else{
			tpb->wait--;
		}
	}
	else{
		vanish=0;
		SoftSpriteParaSet(tpb->ss,SS_PARA_VANISH,vanish);
		SCIO_ServerQueClear(tpb->bw,tpb->client_no,CL_POKEMON_BLINK);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
	}
}

//============================================================================================
/**
 *	HP�Q�[�W�v�Z
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_HPGaugeCalc(TCB_PTR tcb,void *work)
{
	GAUGE_WORK *gauge = work;
	int calc_hp;

	switch(gauge->param.seq_no){
	case 0:
//		Gauge_PartsDraw(gauge, GAUGE_DRAW_HPBAR | GAUGE_DRAW_HPNUM);
		Gauge_InitCalcHP(gauge, gauge->damage);
		gauge->param.seq_no++;
		//break;
	case 1:
		calc_hp = Gauge_CalcHP(gauge);
		if(calc_hp == -1){
			gauge->param.seq_no++;
		}
		break;
	default:
		SCIO_ServerQueClear(gauge->bw, gauge->client_no, gauge->command_code);
		gauge->tcb = NULL;
		TCB_Delete(tcb);
		return;
	}
}

//============================================================================================
/**
 *	EXP�Q�[�W�v�Z
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
///�Œ�ł����̃t���[������EXP�㏸SE��炵�Ă���~�߂�
#define HP_GAUGE_SE_TIME		(8)
static	void	TCB_EXPGaugeCalc(TCB_PTR tcb,void *work)
{
	GAUGE_WORK *gauge = work;
	int calc_exp;
	
	switch(gauge->param.seq_no){
	case 0:
		gauge->se_wait = 0;
		Snd_SePlay(BSE_EXP_GET);
		Gauge_InitCalcEXP(gauge, gauge->add_exp);
		gauge->param.seq_no++;
		//break;
	case 1:
		if(gauge->se_wait < HP_GAUGE_SE_TIME){
			gauge->se_wait++;
		}
		
		calc_exp = Gauge_CalcEXP(gauge);
		if(calc_exp == -1){
			if(gauge->se_wait >= HP_GAUGE_SE_TIME){
				Snd_SeStopBySeqNo(BSE_EXP_GET, 0);
				gauge->param.seq_no = 100;
			}
			else{
				gauge->param.seq_no++;
			}
		}
		break;
	case 2:
		gauge->se_wait++;
		if(gauge->se_wait >= HP_GAUGE_SE_TIME){
			Snd_SeStopBySeqNo(BSE_EXP_GET, 0);
			gauge->param.seq_no = 100;
		}
		break;
	default:
		SCIO_ServerQueClear(gauge->bw, gauge->client_no, gauge->command_code);
		gauge->tcb = NULL;
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�C��G�t�F�N�g
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_KizetsuEffect(TCB_PTR tcb,void *work)
{
	TCB_KIZETSU_EFFECT	*tke=(TCB_KIZETSU_EFFECT *)work;
	WE_SYS_PTR			wsp=BattleWorkWE_SYS_PTRGet(tke->bw);
	int	pos_y;
	int	height;

	switch(tke->seq_no){
	case 0:
		if(tke->migawari_flag){
			{
				WAZA_EFFECT_PARAM	wep;

				SCIO_EffectDataSet(tke->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_OUT,tke->client_no,tke->client_no,NULL);
				CT_WesCall(tke->bw,tke->cp,wsp,&wep);
			}
			tke->seq_no++;
		}
		else{
			tke->seq_no=4;
		}
		break;

	case 2:
		{
			WAZA_POKE_CHANGE_PARAM	wpcp;
			WAZA_EFFECT_PARAM		wep;

			CT_WazaPokeChangeParamMake(tke->bw,&tke->wep,&wpcp,tke->client_no);
			HenshinChrPop(&wpcp,HEAPID_BATTLE);

			SCIO_EffectDataSet(tke->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_IN,tke->client_no,tke->client_no,NULL);
			CT_WesCall(tke->bw,tke->cp,wsp,&wep);
			tke->cp->migawari_flag=0;
		}
		tke->seq_no++;
		break;
	//�������̃G�t�F�N�g�I���҂�
	case 1:
	case 3:
		WES_Executed(wsp);
		if (IsWES_Executed(wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(wsp);						///< Sysmte Reset
			tke->seq_no++;
		}
		break;
	case 4:
		{
			int	pan;
		//	u8	wait;
		//	u8	client_type;

			if(tke->dir==PARA_FRONT){
				pan=POKE_VOICE_PAN_FRONT;
			}
			else{
				pan=POKE_VOICE_PAN_BACK;
			}
			
		//	client_type = BattleWorkClientTypeGet( tke->bw, tke->client_no );
		//	PokeEditData_VoiceWaitSet( tke->cp->handle, &wait, tke->monsno, client_type );					
			PokeVoiceWaitPlaySet(BattleWorkPerapVoiceGet(tke->bw,tke->client_no),
								 PV_POKEDOWN,tke->monsno,tke->form_no,pan,POKE_KIZETSU_VOL,tke->henshin_flag,HEAPID_BATTLE, 0 );
		//	PokeVoicePlaySet(BattleWorkPerapVoiceGet(tke->bw,tke->client_no),
		//					 PV_POKEDOWN,tke->monsno,pan,POKE_KIZETSU_VOL,tke->henshin_flag,HEAPID_BATTLE);
			tke->seq_no++;
		}
	case 5:
		if(Snd_PMVoicePlayCheck()==FALSE){
			tke->seq_no++;
		}
		break;
	case 6:
		if(tke->dir==PARA_FRONT){
			Snd_SePlayPan(BSE_POKE_DEAD,WAZA_SE_PAN_R);
		}
		else{
			Snd_SePlayPan(BSE_POKE_DEAD,WAZA_SE_PAN_L);
		}
		if(SoftSpriteParaGet(tke->ss,SS_PARA_SHADOW_HEIGHT)>0){
			tke->seq_no=7;
		}
		else{
			tke->seq_no=8;
		}
		break;
	case 7:
		height=SoftSpriteParaGet(tke->ss,SS_PARA_SHADOW_HEIGHT);
		height-=8;
		if(height<0){
			height=0;
		}
		SoftSpriteParaSet(tke->ss,SS_PARA_SHADOW_HEIGHT,height);
		if(height){
			break;
		}
		tke->seq_no++;
	case 8:
		height=PokeHeightGet(tke->monsno,tke->sex,tke->dir,tke->form_no,tke->personal_rnd);
		height=SOFT_SPRITE_SIZE_Y-height;
		SoftSpriteVisibleSet(tke->ss,0,0,SOFT_SPRITE_SIZE_X,height);
		tke->seq_no++;
		break;
	case 9:
		pos_y=SoftSpriteParaGet(tke->ss,SS_PARA_POS_Y);
		pos_y+=8;
		SoftSpriteParaSet(tke->ss,SS_PARA_POS_Y,pos_y);
		height=SoftSpriteParaGet(tke->ss,SS_PARA_OFS_SIZE_Y);
		height-=8;
		if(height<0){
			height=0;
		}
		SoftSpriteParaSet(tke->ss,SS_PARA_OFS_SIZE_Y,height);
		if(height==0){
			SoftSpriteDel(tke->ss);
			tke->seq_no++;
		}
		break;
	case 10:
		SCIO_ServerQueClear(tke->bw,tke->client_no,tke->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	��ʃt�F�[�h�G�t�F�N�g
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_FadeOut(TCB_PTR tcb,void *work)
{
	TCB_FADE_OUT	*tfo=(TCB_FADE_OUT *)work;
	PALETTE_FADE_PTR	pfd;
	SOFT_SPRITE_MANAGER	*ssm_p;

	pfd=BattleWorkPfdGet(tfo->bw);
	ssm_p=BattleWorkSoftSpriteManagerGet(tfo->bw);

	switch(tfo->seq_no){
	case 0:
		MSG_DsIconFlashReq(BattleWorkMsgIconGet(tfo->bw),MSG_DSI_REQ_STOP);
		PaletteFadeReq(pfd,PF_BIT_NORMAL_ALL,0xffff,1,0,16,0x0000);
		SoftSpritePalFadeSetAll(ssm_p,0,16,0,0x0000);
		Snd_BgmFadeOut(0,16);
		tfo->seq_no++;
	case 1:
		if(PaletteFadeCheck(pfd)==0){
			tfo->seq_no++;
		}
		break;
	case 2:
		SCIO_ServerQueClear(tfo->bw,tfo->client_no,tfo->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�|�P�����o�j�b�V������^�X�N
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_PokemonVanishOnOff(TCB_PTR tcb,void *work)
{
	TCB_POKEMON_VANISH_ON_OFF	*tpvoo;
	WE_SYS_PTR					wsp;

	tpvoo=(TCB_POKEMON_VANISH_ON_OFF *)work;
	wsp=BattleWorkWE_SYS_PTRGet(tpvoo->bw);

	switch(tpvoo->seq_no){
	case 0:
		if(tpvoo->cp->poke_ss){
			SoftSpriteParaSet(tpvoo->cp->poke_ss,SS_PARA_VANISH,tpvoo->sw);
			if(tpvoo->sw==SW_VANISH_ON){
				tpvoo->seq_no=0xff;
			}
			else{
				tpvoo->seq_no++;
			}
		}
		else{
			tpvoo->seq_no=0xff;
		}
		break;
	case 1:
		if((tpvoo->migawari_flag)&&(tpvoo->cp->migawari_flag==0)){
			{
				WAZA_EFFECT_PARAM		wep;
	
				SCIO_EffectDataSet(tpvoo->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_OUT,
								   tpvoo->client_no,tpvoo->client_no,NULL);
				CT_WesCall(tpvoo->bw,tpvoo->cp,wsp,&wep);
			}
			tpvoo->seq_no++;
		}
		else{
			tpvoo->seq_no=0xff;
		}
		break;
	case 3:
		{
			WAZA_POKE_CHANGE_PARAM	wpcp;
			WAZA_EFFECT_PARAM		wep;

			CT_WazaPokeChangeParamMake(tpvoo->bw,&tpvoo->wep,&wpcp,tpvoo->client_no);
			MigawariChrSet(&wpcp,HEAPID_BATTLE);

			SCIO_EffectDataSet(tpvoo->bw,NULL,&wep,EFFTYPE_STATUS,STATUS_EFF_MIGAWARI_IN,
							   tpvoo->client_no,tpvoo->client_no,NULL);
			CT_WesCall(tpvoo->bw,tpvoo->cp,wsp,&wep);
			tpvoo->cp->migawari_flag=1;
		}
		tpvoo->seq_no++;
		break;
	case 2:
	case 4:
		WES_Executed(wsp);
		if (IsWES_Executed(wsp) == FALSE){		///< Effect Commond End Check
			WES_Reset(wsp);					///< Sysmte Reset
			tpvoo->seq_no++;
		}
		break;
	default:
		SCIO_ServerQueClear(tpvoo->bw,tpvoo->client_no,tpvoo->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	���x���A�b�v�G�t�F�N�g
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_LevelUpEffect(TCB_PTR tcb,void *work)
{
	TCB_LEVEL_UP_EFFECT	*tlue=(TCB_LEVEL_UP_EFFECT *)work;

	switch(tlue->seq_no){
	case 0:
		Gauge_LevelUpEffectStart(tlue->gauge,&tlue->flag);
		Snd_SePlay(BSE_EXP_MAX);
		tlue->seq_no++;
		break;
	case 1:
		if(tlue->flag==TRUE){
			SCIO_ServerQueClear(tlue->bw,tlue->client_no,tlue->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
	}
}

//============================================================================================
/**
 *	���b�Z�[�W�\���܂�
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_MessageWait(TCB_PTR tcb,void *work)
{
#ifdef BATTLE_SPEED_UP
	TCB_MESSAGE_WAIT	*tmw=(TCB_MESSAGE_WAIT *)work;

	if(DebugBattleFlag&DBF_SPEEDUP){
		SCIO_ServerQueClear(tmw->bw,tmw->client_no,tmw->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
	}
	else{
		if(BattleWorkTimeIconGet(tmw->bw)){
			TimeWaitIconDel(BattleWorkTimeIconGet(tmw->bw));
			BattleWorkTimeIconSet(tmw->bw,NULL);
		}
	
		if(GF_MSG_PrintEndCheck(tmw->msg_index)==0){
			SCIO_ServerQueClear(tmw->bw,tmw->client_no,tmw->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
	}
#else
	TCB_MESSAGE_WAIT	*tmw=(TCB_MESSAGE_WAIT *)work;

	if(BattleWorkTimeIconGet(tmw->bw)){
		TimeWaitIconDel(BattleWorkTimeIconGet(tmw->bw));
		BattleWorkTimeIconSet(tmw->bw,NULL);
	}

	if(GF_MSG_PrintEndCheck(tmw->msg_index)==0){
		SCIO_ServerQueClear(tmw->bw,tmw->client_no,tmw->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
	}
#endif
}

//============================================================================================
/**
 *	���b�Z�[�W�\���܂��i�A���[�g���b�Z�[�W��p�j
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_AlertMessageWait(TCB_PTR tcb,void *work)
{
	TCB_ALERT_MESSAGE_WAIT	*tamw=(TCB_ALERT_MESSAGE_WAIT *)work;

	switch(tamw->seq_no){
	case 0:
		if(GF_MSG_PrintEndCheck(tamw->msg_index)==0){
			tamw->seq_no++;
			tamw->wait=0;
		}
		break;
	case 1:
		if(++tamw->wait==ALERT_WAIT){
			SCIO_AlertMessageEndSet(tamw->bw,tamw->client_no);
			SCIO_ServerQueClear(tamw->bw,tamw->client_no,tamw->command_code);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
	}
}

//============================================================================================
/**
 *	�G���J�E���g�G�t�F�N�g
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	const	u8	EncountEffectTable[]={
	ECE_SPA_GRAVEL_001,			//����
	ECE_SPA_SANDS_001,			//���l
	ECE_SPA_GRASS_001,			//�Ő�
	ECE_SPA_POOL_BALL_001,		//�����܂�
	ECE_SPA_ROCK_SURFACE_001,	//�┧
	ECE_SPA_CAVE_001,			//���A
	ECE_SPA_SNOWFIELD_001,		//�ጴ
	ECE_SPA_WATER_001,			//����
	ECE_SPA_ON_THE_ICE_001,		//�X��
	ECE_SPA_FLOOR_001,			//��
	ECE_SPA_BOG_001,			//���n
	ECE_SPA_FLOOR_001,			//�݂苴�i����ȍ~�́A���݂��Ȃ��Ȃ������ǔO�̂��ߓ�������̂Ȃ��G�t�F�N�g���Ăԁj
	ECE_SPA_FLOOR_001,			//�l�V���n��A	�v���`�i����ǉ�
	ECE_SPA_FLOOR_001,			//�l�V���n��B
	ECE_SPA_FLOOR_001,			//�l�V���n��C
	ECE_SPA_FLOOR_001,			//�l�V���n��D
	ECE_SPA_FLOOR_001,			//�`�����s�I���n��
	ECE_SPA_ON_THE_ICE_001,		//�j�ꂽ���E
	ECE_SPA_FLOOR_001,			//�o�g���^���[
	ECE_SPA_FLOOR_001,			//�o�g���t�@�N�g���[
	ECE_SPA_FLOOR_001,			//�o�g�����[���b�g
	ECE_SPA_FLOOR_001,			//�o�g���L���b�X��
	ECE_SPA_FLOOR_001,			//�o�g���X�e�[�W
	ECE_SPA_FLOOR_001,			//����ȊO
};

static	const	int	EncountAfterColor[]={
	0x7fff,		//BG_ID_BASIC			//����
	0x7fff,		//BG_ID_SEA				//�C
	0x7fff,		//BG_ID_CITY			//�X
	0x7fff,		//BG_ID_FOREST			//�X��
	0x7fff,		//BG_ID_MOUNTAIN		//�R�x
	0x7fff,		//BG_ID_SNOW_MOUNTAIN	//��R
	0x7fff,		//BG_ID_ROOM_A			//�����`
	0x7fff,		//BG_ID_ROOM_B			//�����a
	0x7fff,		//BG_ID_ROOM_C			//�����b
	0x0000,		//BG_ID_CAVE_A			//���A�`
	0x0000,		//BG_ID_CAVE_B			//���A�a
	0x0000,		//BG_ID_CAVE_C			//���A�b
	0x7fff,		//BG_ID_BIG_FOUR_A		//�l�V��A	�v���`�i����ǉ�
	0x7fff,		//BG_ID_BIG_FOUR_B		//�l�V��B
	0x7fff,		//BG_ID_BIG_FOUR_C		//�l�V��C
	0x7fff,		//BG_ID_BIG_FOUR_D		//�l�V��D
	0x7fff,		//BG_ID_BIG_CHAMPION	//�`�����s�I��
	0x7fff,		//BG_ID_HAKAI			//�j�ꂽ���E
	0x7fff,		//BG_ID_TOWER			//�o�g���^���[
	0x7fff,		//BG_ID_FACTORY			//�o�g���t�@�N�g���[
	0x7fff,		//BG_ID_ROULETTE		//�o�g�����[���b�g
	0x7fff,		//BG_ID_CASTLE			//�o�g���L���b�X��
	0x7fff,		//BG_ID_STAGE			//�o�g���X�e�[�W
};


enum{
	SEQ_ECE_INIT=0,
	SEQ_ECE_MAIN,
	SEQ_ECE_END_CHECK,
	SEQ_ECE_END,
	SEQ_ECE_CANCEL_WITH_PARTICLE,
	SEQ_ECE_CANCEL
};

static	void	TCB_EncountEffect(TCB_PTR tcb,void *work)
{
	TCB_ENCOUNT_EFFECT	*tee=(TCB_ENCOUNT_EFFECT *)work;
	GF_BGL_INI			*bgl;
	PALETTE_FADE_PTR	pfd;
	int					ground_id;
	int					col;


	bgl=BattleWorkGF_BGL_INIGet(tee->bw);
	pfd=BattleWorkPfdGet(tee->bw);
	ground_id=BattleWorkGroundIDGet(tee->bw);
	col=EncountAfterColor[BattleWorkBGIDGet(tee->bw)];

//	if((tee->wait<SCROLL_TIME)&&(tee->seq_no<SEQ_ECE_END)){
//		GF_BGL_ScrollSet(bgl,GF_BGL_FRAME3_M,GF_BGL_SCROLL_X_INC,SCROLL_SPEED);
//	}

	tee->wait++;

	switch(tee->seq_no){
	case SEQ_ECE_INIT:
		{
			TEC_ADD_PARAM	tap;

			tee->ecem=ECEM_Init(HEAPID_BATTLE,NULL);
			
			tap.eff_no=EncountEffectTable[ground_id];
			tap.camera_type=GF_CAMERA_ORTHO;
			tee->ece[0]=ECEM_Load(tee->ecem,&tap);

			tap.eff_no=EncountEffectTable[ground_id]+1;
			tap.camera_type=GF_CAMERA_ORTHO;
			tee->ece[1]=ECEM_Load(tee->ecem,&tap);

			ECE_Call(tee->ece[0]);
			
			Snd_SePlay(BECEFF_GRASS_01);

			tee->seq_no = SEQ_ECE_MAIN;
		}
	//	break;
	
	case SEQ_ECE_MAIN:
//		OS_Printf( " wait = %d\n", tee->wait );
//		if(tee->wait==20){
		if(tee->wait==10){
			PaletteFadeReq(pfd,PF_BIT_MAIN_BG,0xf3ff,0,0,16,col);
			PaletteFadeReq(pfd,PF_BIT_MAIN_OBJ,BATTLE_MAIN_OBJPAL_FADEBIT,0,0,16,col);
		}
		
		//�T�u��ʂ������甒�փt�F�[�h������ 2008.02.09(�y) matsuda
		if(tee->wait >= 10 && tee->sub_disp_evy < 16){
			u16 i, rgb;
			u16 *dest_buf;
			tee->sub_disp_evy++;
			if(tee->sub_disp_evy > 16){
				tee->sub_disp_evy = 16;
			}
			dest_buf = PaletteWorkTransWorkGet(pfd, FADE_SUB_BG);
			for(i = 0; i < 256; i++){
				rgb = FADE_CHANGE(0, 0x1f, tee->sub_disp_evy);
				dest_buf[i] = rgb | (rgb << 5) | (rgb << 10);
			}
		}
		
//		if(tee->wait==41){
		if(tee->wait==20){
			ECE_Call(tee->ece[1]);
		}
//		if(tee->wait==46){					///< SE�J�n�̃^�C�~���O
		if(tee->wait==23){					///< SE�J�n�̃^�C�~���O
			Snd_SePlay(BECEFF_GRASS_02);
		}
//		if(tee->wait==56){
		if(tee->wait==28){
			MI_CpuCopy16(BattleWorkPushBGPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_BG),0x20*7);

			if(BattleWorkFightTypeGet(tee->bw)==FIGHT_TYPE_2vs2_YASEI){
				MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*5);
			}
			else if(BattleWorkFightTypeGet(tee->bw)&FIGHT_TYPE_2vs2){
				MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*7);
			}
			else if(BattleWorkFightTypeGet(tee->bw)&FIGHT_TYPE_TRAINER){
				MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*5);
			}
			else{
				MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*4);
			}
			PaletteFadeReq(pfd,PF_BIT_MAIN_BG,0xf3ff,0,16,0,col);
			PaletteFadeReq(pfd,PF_BIT_MAIN_OBJ,BATTLE_MAIN_OBJPAL_FADEBIT,0,16,0,col);
			PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,0,16,0,col);
		}
//		if(tee->wait==100){
		if(tee->wait>=50){
			PaletteFadeReq(pfd,PF_BIT_MAIN_BG,0x0c00,0,16,0,0x0000);
			tee->seq_no++;
		}
		break;
	case SEQ_ECE_END_CHECK:
		if(ECE_EndCheck(tee->ece[1])==FALSE){
			ECEM_Free(tee->ecem);
			tee->seq_no++;
		}
		break;
	case SEQ_ECE_END:
		//���b�Z�[�W�E�C���h�EDS�A�C�R���p���b�g�A�j���X�^�[�g
		MSG_DsIconFlashReq(BattleWorkMsgIconGet(tee->bw),MSG_DSI_REQ_START);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	case SEQ_ECE_CANCEL_WITH_PARTICLE:
		ECEM_Skip(tee->ecem);
		ECEM_Free(tee->ecem);
		tee->seq_no++;
	case SEQ_ECE_CANCEL:
		MI_CpuCopy16(BattleWorkPushBGPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_BG),0x20*7);
		if(BattleWorkFightTypeGet(tee->bw)==FIGHT_TYPE_2vs2_YASEI){
			MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*5);
		}
		else if(BattleWorkFightTypeGet(tee->bw)&FIGHT_TYPE_2vs2){
			MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*7);
		}
		else if(BattleWorkFightTypeGet(tee->bw)&FIGHT_TYPE_TRAINER){
			MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*5);
		}
		else{
			MI_CpuCopy16(BattleWorkPushOBJPaletteGet(tee->bw),PaletteWorkDefaultWorkGet(pfd,FADE_MAIN_OBJ),0x20*4);
		}
		PaletteFadeReq(pfd,PF_BIT_MAIN_BG,0xffff,0,0,0,0x7fff);
		PaletteFadeReq(pfd,PF_BIT_MAIN_OBJ,BATTLE_MAIN_OBJPAL_FADEBIT,0,0,0,0xffff);
		PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,0,0,0,0x7fff);
		tee->seq_no=SEQ_ECE_END;
		break;
	}
}

//============================================================================================
/**
 *	�Z�Y�ꏈ��
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_WazaWasure(TCB_PTR tcb,void *work)
{
	TCB_WAZA_WASURE		*tww=(TCB_WAZA_WASURE *)work;
	PALETTE_FADE_PTR	pfd;

	pfd=BattleWorkPfdGet(tww->bw);

	switch(tww->seq_no){
	case 0:		//���X�g��ʂւ̃t�F�[�h
//		MSG_DsIconFlashReq(BattleWorkMsgIconGet(tww->bw),MSG_DSI_REQ_STOP);
//		PaletteFadeReq(pfd,PF_BIT_MAIN_ALL,0x0c00,0,0,MSG_WINDOW_FADE,0x0000);
		PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,BATTLE_BAGLIST_FADE_SPEED,0,16,0x0000);
		tww->seq_no++;
		break;
	case 1:
		if(PaletteFadeCheck(pfd)==0){	//�P�x�ύX�t�F�[�h�I���҂����ĐF�X�폜
			FightScreenTransition(tww->bw);

			tww->plist=sys_AllocMemory(HEAPID_BATTLE,sizeof(BPLIST_DATA));
			tww->plist->pp=BattleWorkPokePartyGet(tww->bw,tww->client_no);
			tww->plist->bw=tww->bw;
			tww->plist->heap=HEAPID_BATTLE;
			tww->plist->sel_poke=tww->sel_mons_no;
			tww->plist->chg_waza=tww->waza_no;
			tww->plist->end_flg=0;
			tww->plist->double_sel=0;
			tww->plist->mode=BPL_MODE_WAZASET;		// �Z�Y��
			tww->plist->item=0;
			tww->plist->client_no=tww->client_no;
			tww->plist->cursor_flg=0;
				
			BattlePokeList_TaskAdd( tww->plist );
			tww->seq_no++;
		}
		break;
	case 2:
		if(tww->plist->end_flg){
			FightScreenRecover(tww->bw);
//			PaletteFadeReq(pfd,PF_BIT_MAIN_ALL,0x0c00,0,MSG_WINDOW_FADE,0,0x0000);
			PaletteFadeReq(pfd,PF_BIT_SUB_ALL,0xffff,BATTLE_BAGLIST_FADE_SPEED,16,0,0x0000);
			tww->seq_no++;
		}
		break;
	case 3:
		if(PaletteFadeCheck(pfd)==0){	//�P�x�ύX�t�F�[�h�I���҂����ĐF�X�폜
//			MSG_DsIconFlashReq(BattleWorkMsgIconGet(tww->bw),MSG_DSI_REQ_START);
			if(tww->plist->sel_wp==BPL_SEL_WP_CANCEL){
				SCIO_PokemonSelectReturnSet(tww->bw,tww->client_no,SELECT_CANCEL);
			}
			else{
				SCIO_PokemonSelectReturnSet(tww->bw,tww->client_no,tww->plist->sel_wp+1);
			}
			SCIO_ServerQueClear(tww->bw,tww->client_no,tww->command_code);
			sys_FreeMemoryEz(tww->plist);
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
	}
}

//============================================================================================
/**
 *	�\�t�g�E�G�A�X�v���C�g�Ƀ��U�C�N��������
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_MosaicSet(TCB_PTR tcb,void *work)
{
	TCB_MOSAIC_SET	*tms=(TCB_MOSAIC_SET *)work;
	int				mosaic;

	switch(tms->seq_no){
	case 0:
		if(tms->wait==tms->wait_temp){
			tms->wait=0;
			mosaic=SoftSpriteParaGet(tms->ss,SS_PARA_MOSAIC);
			if(mosaic>tms->mosaic){
				mosaic--;
			}
			else if(mosaic<tms->mosaic){
				mosaic++;
			}
			SoftSpriteParaSet(tms->ss,SS_PARA_MOSAIC,mosaic);
			if(mosaic==tms->mosaic){
				tms->seq_no++;
			}
		}
		else{
			tms->wait++;
		}
		break;
	case 1:
		SCIO_ServerQueClear(tms->bw,tms->client_no,tms->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�莝���Q�[�W�F�G���J�E���gIN
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_BallGaugeEncountSet(TCB_PTR tcb,void *work)
{
	TCB_BALL_GAUGE	*tbg=(TCB_BALL_GAUGE *)work;
	TEMOTI_SIDE side;
	BALL_IN_TYPE ball_in_type;
	
	if(BattleWorkMineEnemyCheck(tbg->bw, tbg->client_no) == 0){
		side = TEMOTI_MINE;
	}
	else{
		side = TEMOTI_ENEMY;
	}

	switch(tbg->seq_no){
	case 0:
		{
			TEMOTIGAUGE_PTR tg;
			BALL_IN_TYPE ball_in_type;
			BALL_POS_TYPE pos_type;
			
			if(tbg->encount_change == 0){
				ball_in_type = BALL_IN_TYPE_ENCOUNT;
				pos_type = BALL_POS_MIDDLE;
			}
			else{
				ball_in_type = BALL_IN_TYPE_CHANGE;
				switch(tbg->client_type){
				case CLIENT_TYPE_B:
					pos_type = BALL_POS_HIGH;
					break;
				case CLIENT_TYPE_D:
					pos_type = BALL_POS_LOW;
					break;
				default:
					pos_type = BALL_POS_MIDDLE;
					break;
				}
			}
			
			tg = TEMOTI_InEffectSet(tbg->status, side, ball_in_type, pos_type,
				BattleWorkCATS_SYS_PTRGet(tbg->bw), BattleWorkCATS_RES_PTRGet(tbg->bw));
			BattleWorkGF_BGL_TGSet(tbg->bw, side, tg);
		}
		tbg->seq_no++;
		break;
	case 1:
		if(TEMOTI_InEffectEndCheck(BattleWorkGF_BGL_TGGet(tbg->bw, side)) == TRUE){
//�����OUT��			TEMOTI_EffectFree(tg);
			tbg->seq_no++;
		}
		break;
	default:
		SCIO_ServerQueClear(tbg->bw,tbg->client_no,tbg->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�莝���Q�[�W�F�G���J�E���gOUT
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_BallGaugeEncountOut(TCB_PTR tcb,void *work)
{
	TCB_BALL_GAUGE	*tbg=(TCB_BALL_GAUGE *)work;
	TEMOTI_SIDE side;
	TEMOTIGAUGE_PTR tg;
	ARROW_OUT_TYPE arrow_out_type;
	BALL_OUT_TYPE ball_out_type;
	
	if(BattleWorkMineEnemyCheck(tbg->bw, tbg->client_no) == 0){
		side = TEMOTI_MINE;
	}
	else{
		side = TEMOTI_ENEMY;
	}
	tg = BattleWorkGF_BGL_TGGet(tbg->bw, side);
	
	switch(tbg->seq_no){
	case 0:
		if(tbg->encount_change == 0){
			arrow_out_type = ARROW_OUT_TYPE_SCROLL;
			ball_out_type = BALL_OUT_TYPE_ENCOUNT;
		}
		else{
			arrow_out_type = ARROW_OUT_TYPE_SPOT;
			ball_out_type = BALL_OUT_TYPE_CHANGE;
		}
		
		TEMOTI_OutEffectSet(tg, arrow_out_type, ball_out_type);
		tbg->seq_no++;
		break;
	case 1:
		if(TEMOTI_OutEffectEndCheck(tg) == TRUE){
			TEMOTI_EffectFree(tg);
			BattleWorkGF_BGL_TGSet(tbg->bw, side, NULL);
			tbg->seq_no++;
		}
		break;
	default:
		SCIO_ServerQueClear(tbg->bw,tbg->client_no,tbg->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	�\�t�g�X�v���C�g��OAM�ϊ�
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
static	void	TCB_SStoOAM(TCB_PTR tcb,void *work)
{
	TCB_SS_TO_OAM	*tsto=(TCB_SS_TO_OAM *)work;
	
	switch(tsto->seq_no){
	case 0:
		GF_ASSERT(tsto->cp->ods == NULL);
		tsto->cp->ods = OAM_Drop_Init(tsto->bw, HEAPID_BATTLE, tsto->client_no);
		tsto->seq_no++;
		break;
		
	default:
		SCIO_ServerQueClear(tsto->bw,tsto->client_no,tsto->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
}

//============================================================================================
/**
 *	OAM���\�t�ƃX�v���C�g�ϊ�
 *
 * @param[in]	tcb		TCB�V�X�e�����[�N
 * @param[in]	work	���[�N�G���A�|�C���^
 */
//============================================================================================
#define TOTS_DELETE_WAIT	(5)		///< �E�F�C�g
static	void	TCB_OAMtoSS(TCB_PTR tcb,void *work)
{
	TCB_OAM_TO_SS	*tots=(TCB_OAM_TO_SS *)work;
	
	GF_ASSERT(tots->cp->ods != NULL);
	
	///< B�̈ʒu�̃|�P�����́AD��SSP�Ƃ̃v���C�I���e�B�̊֌W�ł���������̂͂܂���
	if (OAM_Drop_ClientTypeGet(tots->cp->ods) == CLIENT_TYPE_B){
		if (tots->seq_no < TOTS_DELETE_WAIT){
			tots->seq_no++;
			return;
		}
	}
	
	OAM_Drop_End(tots->cp->ods, tots->client_no);
	tots->cp->ods = NULL;
	
	SCIO_ServerQueClear(tots->bw,tots->client_no,tots->command_code);
	sys_FreeMemoryEz(work);
	TCB_Delete(tcb);
	
#if 0
	switch(tots->seq_no){
	case 0:
		GF_ASSERT(tots->cp->ods != NULL);

		///< B�̈ʒu�̃|�P�����́AD��SSP�Ƃ̃v���C�I���e�B�̊֌W�ł���������̂͂܂���
		if (OAM_Drop_ClientTypeGet(tots->cp->ods) != CLIENT_TYPE_B){
			OAM_Drop_End(tots->cp->ods, tots->client_no);
			tots->cp->ods = NULL;
			tots->seq_no = 0xFF;
			break;
		}
		tots->seq_no++;
		break;
	
	case 1:
		tots->seq_no++;
		break;
	
	case 2:
		tots->seq_no++;
		break;
		
	case 3:
		OAM_Drop_End(tots->cp->ods, tots->client_no);
		tots->cp->ods = NULL;
		tots->seq_no++;
		break;
		
	default:
		SCIO_ServerQueClear(tots->bw,tots->client_no,tots->command_code);
		sys_FreeMemoryEz(work);
		TCB_Delete(tcb);
		break;
	}
#endif
}

//-----------------------------------------------------------------------------
//		�O���֐��p�c�[���Q
//-----------------------------------------------------------------------------
//============================================================================================
/**
 *	���[�U�C���^�t�F�[�X�Z�b�g
 *
 * @param[in]	bw		�퓬�V�X�e�����[�N�|�C���^
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
void	CT_UISet(BATTLE_WORK *bw,CLIENT_PARAM *cp)
{
	u32	fight_type=BattleWorkFightTypeGet(bw);
	u32	battle_status_flag=BattleWorkBattleStatusFlagGet(bw);

#ifdef PM_DEBUG
	if(BattleWorkBattleStatusFlagGet(bw)&BATTLE_STATUS_FLAG_AUTO_BATTLE){
		if(fight_type&FIGHT_TYPE_SIO){
			if(cp->client_type&CLIENT_ENEMY_FLAG){
				cp->ui.UI_CommandSelect=&TCB_CommandSelectSIO;
				cp->ui.UI_WazaSelect=&TCB_WazaSelectSIO;
				cp->ui.UI_DirSelect=&TCB_DirSelectSIO;
				cp->ui.UI_ItemSelect=&TCB_ItemSelectSIO;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelectSIO;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelectSIO;
				cp->client_boot=CLIENT_BOOT_SIO;
			}
			else{
				cp->ui.UI_CommandSelect=&TCB_CommandSelectAI;
				cp->ui.UI_WazaSelect=&TCB_WazaSelectAI;
				cp->ui.UI_DirSelect=&TCB_DirSelectAI;
				cp->ui.UI_ItemSelect=&TCB_ItemSelectAI;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAI;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAI;
				cp->client_boot=CLIENT_BOOT_AI;
			}
		}
		else{
			cp->ui.UI_CommandSelect=&TCB_CommandSelectAI;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectAI;
			cp->ui.UI_DirSelect=&TCB_DirSelectAI;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectAI;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAI;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAI;
			cp->client_boot=CLIENT_BOOT_AI;
		}
		return;
	}
#endif

	if(fight_type&FIGHT_TYPE_AI){
		if(cp->client_no){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectAI;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectAI;
			cp->ui.UI_DirSelect=&TCB_DirSelectAI;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectAI;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAI;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAI;
			cp->client_boot=CLIENT_BOOT_AI;
		}
		else{
			if(battle_status_flag&BATTLE_STATUS_FLAG_REC_BATTLE){
				cp->ui.UI_CommandSelect=&TCB_CommandSelectAuto;
				cp->ui.UI_WazaSelect=&TCB_WazaSelectAuto;
				cp->ui.UI_DirSelect=&TCB_DirSelectAuto;
				cp->ui.UI_ItemSelect=&TCB_ItemSelectAuto;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAuto;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAuto;
				cp->client_boot=CLIENT_BOOT_NORMAL;
			}
			else{
				cp->ui.UI_CommandSelect=&TCB_CommandSelect;
				cp->ui.UI_WazaSelect=&TCB_WazaSelect;
				cp->ui.UI_DirSelect=&TCB_DirSelect;
				cp->ui.UI_ItemSelect=&TCB_ItemSelect;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelect;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelect;
				cp->client_boot=CLIENT_BOOT_NORMAL;
			}
		}
	}
	else if((fight_type&(FIGHT_TYPE_MULTI|FIGHT_TYPE_TOWER))==(FIGHT_TYPE_MULTI|FIGHT_TYPE_TOWER)){
		if(cp->client_type&CLIENT_ENEMY_FLAG){
			if(BattleWorkServerFlagGet(bw)){
				cp->ui.UI_CommandSelect=&TCB_CommandSelectAI;
				cp->ui.UI_WazaSelect=&TCB_WazaSelectAI;
				cp->ui.UI_DirSelect=&TCB_DirSelectAI;
				cp->ui.UI_ItemSelect=&TCB_ItemSelectAI;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAI;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAI;
				cp->client_boot=CLIENT_BOOT_AI;
			}
			else{
				cp->ui.UI_CommandSelect=&TCB_CommandSelectSIO;
				cp->ui.UI_WazaSelect=&TCB_WazaSelectSIO;
				cp->ui.UI_DirSelect=&TCB_DirSelectSIO;
				cp->ui.UI_ItemSelect=&TCB_ItemSelectSIO;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelectSIO;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelectSIO;
				cp->client_boot=CLIENT_BOOT_SIO;
			}
		}
		else if(battle_status_flag&BATTLE_STATUS_FLAG_REC_BATTLE){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectAuto;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectAuto;
			cp->ui.UI_DirSelect=&TCB_DirSelectAuto;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectAuto;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAuto;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAuto;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
		else if(cp->client_type!=BattleWorkClientTypeGet(bw,BattleWorkCommIDGet(bw)*2)){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectSIO;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectSIO;
			cp->ui.UI_DirSelect=&TCB_DirSelectSIO;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectSIO;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectSIO;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectSIO;
			cp->client_boot=CLIENT_BOOT_SIO;
		}
		else{
			cp->ui.UI_CommandSelect=&TCB_CommandSelect;
			cp->ui.UI_WazaSelect=&TCB_WazaSelect;
			cp->ui.UI_DirSelect=&TCB_DirSelect;
			cp->ui.UI_ItemSelect=&TCB_ItemSelect;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelect;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelect;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
	}
	else if(fight_type&FIGHT_TYPE_MULTI){
		if(battle_status_flag&BATTLE_STATUS_FLAG_REC_BATTLE){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectAuto;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectAuto;
			cp->ui.UI_DirSelect=&TCB_DirSelectAuto;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectAuto;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAuto;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAuto;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
		else if(cp->client_type!=BattleWorkClientTypeGet(bw,BattleWorkCommIDGet(bw))){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectSIO;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectSIO;
			cp->ui.UI_DirSelect=&TCB_DirSelectSIO;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectSIO;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectSIO;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectSIO;
			cp->client_boot=CLIENT_BOOT_SIO;
		}
		else{
			cp->ui.UI_CommandSelect=&TCB_CommandSelect;
			cp->ui.UI_WazaSelect=&TCB_WazaSelect;
			cp->ui.UI_DirSelect=&TCB_DirSelect;
			cp->ui.UI_ItemSelect=&TCB_ItemSelect;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelect;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelect;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
	}
	else if(fight_type&FIGHT_TYPE_SIO){
		if(battle_status_flag&BATTLE_STATUS_FLAG_REC_BATTLE){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectAuto;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectAuto;
			cp->ui.UI_DirSelect=&TCB_DirSelectAuto;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectAuto;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAuto;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAuto;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
		else if(cp->client_type&CLIENT_ENEMY_FLAG){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectSIO;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectSIO;
			cp->ui.UI_DirSelect=&TCB_DirSelectSIO;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectSIO;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectSIO;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectSIO;
			cp->client_boot=CLIENT_BOOT_SIO;
		}
		else{
			cp->ui.UI_CommandSelect=&TCB_CommandSelect;
			cp->ui.UI_WazaSelect=&TCB_WazaSelect;
			cp->ui.UI_DirSelect=&TCB_DirSelect;
			cp->ui.UI_ItemSelect=&TCB_ItemSelect;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelect;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelect;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
	}
	else if(fight_type&FIGHT_TYPE_POKE_PARK){
		if(cp->client_type&CLIENT_ENEMY_FLAG){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectPokepark;
			cp->ui.UI_WazaSelect=NULL;
			cp->ui.UI_DirSelect=NULL;
			cp->ui.UI_ItemSelect=NULL;
			cp->ui.UI_PokemonSelect=NULL;
			cp->ui.UI_YesNoSelect=NULL;
			cp->client_boot=CLIENT_BOOT_AI;
		}
		else{
			cp->ui.UI_CommandSelect=&TCB_CommandSelect;
			cp->ui.UI_WazaSelect=NULL;
			cp->ui.UI_DirSelect=NULL;
			cp->ui.UI_ItemSelect=NULL;
			cp->ui.UI_PokemonSelect=NULL;
			cp->ui.UI_YesNoSelect=NULL;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
	}
	else if(fight_type&FIGHT_TYPE_SAFARI){
		if(cp->client_type&CLIENT_ENEMY_FLAG){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectSafari;
			cp->ui.UI_WazaSelect=NULL;
			cp->ui.UI_DirSelect=NULL;
			cp->ui.UI_ItemSelect=NULL;
			cp->ui.UI_PokemonSelect=NULL;
			cp->ui.UI_YesNoSelect=NULL;
			cp->client_boot=CLIENT_BOOT_AI;
		}
		else{
			cp->ui.UI_CommandSelect=&TCB_CommandSelect;
			cp->ui.UI_WazaSelect=NULL;
			cp->ui.UI_DirSelect=NULL;
			cp->ui.UI_ItemSelect=NULL;
			cp->ui.UI_PokemonSelect=NULL;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelect;
			cp->client_boot=CLIENT_BOOT_NORMAL;
		}
	}
	else{
		if(cp->client_type&CLIENT_ENEMY_FLAG){
			cp->ui.UI_CommandSelect=&TCB_CommandSelectAI;
			cp->ui.UI_WazaSelect=&TCB_WazaSelectAI;
			cp->ui.UI_DirSelect=&TCB_DirSelectAI;
			cp->ui.UI_ItemSelect=&TCB_ItemSelectAI;
			cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAI;
			cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAI;
			cp->client_boot=CLIENT_BOOT_AI;
		}
		else{
			if(battle_status_flag&BATTLE_STATUS_FLAG_REC_BATTLE){
				cp->ui.UI_CommandSelect=&TCB_CommandSelectAuto;
				cp->ui.UI_WazaSelect=&TCB_WazaSelectAuto;
				cp->ui.UI_DirSelect=&TCB_DirSelectAuto;
				cp->ui.UI_ItemSelect=&TCB_ItemSelectAuto;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelectAuto;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelectAuto;
				cp->client_boot=CLIENT_BOOT_NORMAL;
			}
			else{
				cp->ui.UI_CommandSelect=&TCB_CommandSelect;
				cp->ui.UI_WazaSelect=&TCB_WazaSelect;
				cp->ui.UI_DirSelect=&TCB_DirSelect;
				cp->ui.UI_ItemSelect=&TCB_ItemSelect;
				cp->ui.UI_PokemonSelect=&TCB_PokemonSelect;
				cp->ui.UI_YesNoSelect=&TCB_YesNoSelect;
				cp->client_boot=CLIENT_BOOT_NORMAL;
			}
		}
	}
}

//============================================================================================
/**
 *	client_type�擾�֐�
 *
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
u8	CT_CPClientTypeGet(CLIENT_PARAM *cp)
{
	return cp->client_type;
}

//============================================================================================
/**
 *	client_boot�擾�֐�
 *
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
u8	CT_CPClientBootGet(CLIENT_PARAM *cp)
{
	return cp->client_boot;
}

//============================================================================================
/**
 *	soft_sprite�擾�֐�
 *
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
SOFT_SPRITE	*CT_CPSoftSpriteGet(CLIENT_PARAM *cp)
{
	if(cp->poke_ss){
		return cp->poke_ss;
	}
	else{
		return cp->tr_ss;
	}
}

//============================================================================================
/**
 *	gauge_work�擾�֐�
 *
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
GAUGE_WORK		*CT_CPGaugeWorkGet(CLIENT_PARAM *cp)
{
	return &cp->gauge;
}

//--------------------------------------------------------------
/**
 * @brief   CURSOR_SAVE���[�N�̃|�C���^�擾
 *
 * @param   cp				�N���C�A���g���[�N�|�C���^
 *
 * @retval  CURSOR_SAVE���[�N�̃|�C���^
 */
//--------------------------------------------------------------
CURSOR_SAVE * CT_CursorSavePtrGet(CLIENT_PARAM *cp)
{
	return &cp->cursor_save;
}

//--------------------------------------------------------------
/**
 * @brief   CURSOR_SAVE���[�N������������
 * @param   cp		�N���C�A���g���[�N�|�C���^
 */
//--------------------------------------------------------------
void CT_CursorSaveClear(CLIENT_PARAM *cp)
{
	MI_CpuClear8(&cp->cursor_save, sizeof(CURSOR_SAVE));
}

//--------------------------------------------------------------
/**
 * @brief   �g���[�i�[�A�N�^�[�̕\���ݒ�
 * @param   cp		�N���C�A���g���[�N�ւ̃|�C���^
 * @param   enable	CATS_ENABLE_TRUE(�\��) or CATS_ENABLE_FALSE(��\��)
 */
//--------------------------------------------------------------
void Trainer_EnableSet(CLIENT_PARAM *cp, int enable)
{
	if(cp->cap == NULL){
		return;
	}
	
	CATS_ObjectEnableCap(cp->cap, enable);
}

//-----------------------------------------------------------------------------
//		�O������J�֐�
//-----------------------------------------------------------------------------
//============================================================================================
/**
 *	SoftSprite�̃Z�b�g�ƋZ�G�t�F�N�g�p�L�����W�J�ƃp�����[�^�擾������֐�
 *
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
static	SOFT_SPRITE	*CT_SoftSpriteAdd(BATTLE_WORK *bw,SOFT_SPRITE_MANAGER *ssm_p,SOFT_SPRITE_ARC *ssa,
							  int pos_x,int pos_y,int pos_z,int ofs_y,int height,int ofx,int shadow,int client_no,
							  SOFT_SPRITE_ANIME *anm_tbl,SOFT_SPRITE_MOVE_FUNC *move)
{
	SOFT_SPRITE	*ss;
	u8			*oam_temp=ExChrParamExChrBufferGet(BattleWorkExChrParamGet(bw),client_no);
	int			type;
	int			dir;
	
	type=BattleWorkClientTypeGet(bw,client_no);
	
	if (type & CLIENT_ENEMY_FLAG){
		dir = PARA_FRONT;
	}
	else {
		dir = PARA_BACK;
	}

	Ex_ChangesInto_BattlePokeData(ssa->arc_no,ssa->index_chr,HEAPID_BATTLE,oam_temp, ssa->personal_rnd, FALSE, dir, ssa->strike_mons);
	ExChrParamArcIDSet(BattleWorkExChrParamGet(bw),client_no,ssa->arc_no);
	ExChrParamPalIndexSet(BattleWorkExChrParamGet(bw),client_no,ssa->index_pal);
	ExChrParamHeightSet(BattleWorkExChrParamGet(bw),client_no,ofs_y);

	ss=SoftSpriteAddNum(ssm_p,ssa,pos_x,pos_y+ofs_y,pos_z,client_no,client_no,anm_tbl,move);

	if(type&CLIENT_ENEMY_FLAG){
		if(type>CLIENT_TYPE_BB){
			type=type>>1;
		}
		SoftSpriteParaSet(ss,SS_PARA_SHADOW_REQ,type);
		SoftSpriteParaSet(ss,SS_PARA_SHADOW_SIZE,shadow);
		SoftSpriteParaSet(ss,SS_PARA_SHADOW_Y,(pos_y+ofs_y)+(SHADOW_OFS-ofs_y));
		SoftSpriteParaSet(ss,SS_PARA_SHADOW_OX,ofx);
		SoftSpriteParaSet(ss,SS_PARA_SHADOW_OY,SHADOW_OFS-ofs_y);
		SoftSpriteParaSet(ss,SS_PARA_SHADOW_HEIGHT,height);
	}

	return ss;
}

//============================================================================================
/**
 *	FightType�ƃ��b�Z�[�W�\������Client����߂����b�Z�[�W�̃p�����[�^�𐶐�����
 *
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
static	void	CT_ModosuMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MODOSU_MESSAGE_PARAM *mmp,MESSAGE_PARAM *mp)
{
	if(cp->client_type&CLIENT_ENEMY_FLAG){
		if(BattleWorkFightTypeGet(bw)&FIGHT_TYPE_SIO){
			mp->msg_id=TuushinIrekaeMsg;
			mp->msg_tag=TAG_TRNAME_NICK;
			mp->msg_para[0]=cp->client_no;
			mp->msg_para[1]=cp->client_no|(mmp->sel_mons_no<<8);
		}
		else{
			mp->msg_id=IrekaeMsg2;
			mp->msg_tag=TAG_TRTYPE_TRNAME_NICK;
			mp->msg_para[0]=cp->client_no;
			mp->msg_para[1]=cp->client_no;
			mp->msg_para[2]=cp->client_no|(mmp->sel_mons_no<<8);
		}
	}
	else{
		//�󂯂��_���[�W�ȂǂŁA���b�Z�[�W��ω�
		if(((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_2vs2)==0)&&
		   ((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_SIO)==0)){
			if(mmp->rate==0){
				mp->msg_id=IrekaeMsg_0;
			}
			else if(mmp->rate<25){
				mp->msg_id=IrekaeMsg_1;
			}
			else if(mmp->rate<50){
				mp->msg_id=IrekaeMsg_4;
			}
			else if(mmp->rate<75){
				mp->msg_id=IrekaeMsg_2;
			}
			else{
				mp->msg_id=IrekaeMsg_3;
			}
		}
		else{
			mp->msg_id=IrekaeMsg_1;
		}
		mp->msg_tag=TAG_NICK;
		mp->msg_para[0]=cp->client_no|(mmp->sel_mons_no<<8);
	}
}

//============================================================================================
/**
 *	FightType�ƃ��b�Z�[�W�\������Client����J��o�����b�Z�[�W�̃p�����[�^�𐶐�����
 *
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 */
//============================================================================================
static	void	CT_KuridasuMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,KURIDASU_MESSAGE_PARAM *kmp,MESSAGE_PARAM *mp)
{
	if(cp->client_type&CLIENT_ENEMY_FLAG){
		if((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_SIO)==0){
			mp->msg_id=KuridashiMsg;
			mp->msg_tag=TAG_TRTYPE_TRNAME_NICK;
			mp->msg_para[0]=cp->client_no;
			mp->msg_para[1]=cp->client_no;
			mp->msg_para[2]=cp->client_no|(kmp->sel_mons_no<<8);
		}
		else{
			mp->msg_id=TuushinKuridashiMsg;
			mp->msg_tag=TAG_TRNAME_NICK;
			mp->msg_para[0]=cp->client_no;
			mp->msg_para[1]=cp->client_no|(kmp->sel_mons_no<<8);
		}
	}
	else{
		//�^�����_���[�W�ȂǂŁA���b�Z�[�W��ω�
		if(((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_2vs2)==0)&&
		   ((BattleWorkFightTypeGet(bw)&FIGHT_TYPE_SIO)==0)){
			if(kmp->rate<100){
				mp->msg_id=AppearMsg_3;
			}
			else if(kmp->rate<325){
				mp->msg_id=AppearMsg_4;
			}
			else if(kmp->rate<550){
				mp->msg_id=AppearMsg_2;
			}
			else if(kmp->rate<775){
				mp->msg_id=AppearMsg_1;
			}
			else{
				mp->msg_id=AppearMsg_0;
			}
		}
		else{
			mp->msg_id=AppearMsg_0;
		}
		mp->msg_tag=TAG_NICK;
		mp->msg_para[0]=cp->client_no|(kmp->sel_mons_no<<8);
	}
}

//============================================================================================
/**
 *	�G���J�E���g���b�Z�[�W�̃p�����[�^�𐶐�����
 *
 * @param[in]	bw		�퓬�V�X�e�����[�N
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 * @param[in]	mp		���b�Z�[�W�p�����[�^
 */
//============================================================================================
static	void		CT_EncountMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *mp)
{
	u32	fight_type;
	int	cli1_no;
	int	cli2_no;

	fight_type=BattleWorkFightTypeGet(bw);

	if(fight_type&FIGHT_TYPE_2vs2){
		cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_B);
		cli2_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_D);
	}
	else{
		cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_BB);
		cli2_no=cli1_no;
	}

	if(fight_type&FIGHT_TYPE_SIO){
		if(fight_type&FIGHT_TYPE_TOWER){
			mp->msg_id=Trainer2Msg;
			mp->msg_tag=TAG_TRTYPE_TRNAME_TRTYPE_TRNAME;
			mp->msg_para[0]=cli1_no;
			mp->msg_para[1]=cli1_no;
			mp->msg_para[2]=cli2_no;
			mp->msg_para[3]=cli2_no;
		}
		else if(fight_type&FIGHT_TYPE_MULTI){
			mp->msg_id=TuushinMultiTrainerEncountMsg;
			mp->msg_tag=TAG_TRNAME_TRNAME;
			mp->msg_para[0]=cli1_no;
			mp->msg_para[1]=cli2_no;
		}
		else{
			mp->msg_id=TuushinTrainerEncountMsg;
			mp->msg_tag=TAG_TRNAME;
			mp->msg_para[0]=cli1_no;
		}
	}
	else{
		if((fight_type&FIGHT_TYPE_TAG)||
		   (fight_type&FIGHT_TYPE_MULTI)){
			mp->msg_id=Trainer2Msg;
			mp->msg_tag=TAG_TRTYPE_TRNAME_TRTYPE_TRNAME;
			mp->msg_para[0]=cli1_no;
			mp->msg_para[1]=cli1_no;
			mp->msg_para[2]=cli2_no;
			mp->msg_para[3]=cli2_no;
		}
		else{
			mp->msg_id=TrainerEncountMsg;
			mp->msg_tag=TAG_TRTYPE_TRNAME;
			mp->msg_para[0]=cli1_no;
			mp->msg_para[1]=cli1_no;
		}
	}
}

//============================================================================================
/**
 *	�G���J�E���g���̌J��o�����b�Z�[�W�̃p�����[�^�𐶐�����
 *
 * @param[in]	bw		�퓬�V�X�e�����[�N
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 * @param[in]	ekmp	�G���J�E���g���̌J��o�����b�Z�[�W�p�����[�^
 * @param[in]	mp		���b�Z�[�W�p�����[�^
 */
//============================================================================================
static	void		CT_EncountKuridasuMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,
													   ENCOUNT_KURIDASU_MESSAGE_PARAM *ekmp,MESSAGE_PARAM *mp)
{
	u32	fight_type;
	int	cli1_no;
	int	cli2_no;

	fight_type=BattleWorkFightTypeGet(bw);

	if(cp->client_type&CLIENT_ENEMY_FLAG){
		if(fight_type&FIGHT_TYPE_2vs2){
			cli1_no=cp->client_no;
			cli2_no=BattleWorkPartnerClientNoGet(bw,cp->client_no);
		}
		else{
			cli1_no=cp->client_no;
			cli2_no=cli1_no;
		}
	
		if(fight_type&FIGHT_TYPE_SIO){
			if(fight_type&FIGHT_TYPE_TOWER){
				mp->msg_id=KuridashiMsg4;
				mp->msg_tag=TAG_TRTYPE_TRNAME_NICKx2;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no;
				mp->msg_para[2]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[3]=cli2_no;
				mp->msg_para[4]=cli2_no;
				mp->msg_para[5]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else if(fight_type&FIGHT_TYPE_MULTI){
				mp->msg_id=TuushinKuridashiMultiMsg;
				mp->msg_tag=TAG_TRNAME_NICK_TRNAME_NICK;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[2]=cli2_no;
				mp->msg_para[3]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else if(fight_type&FIGHT_TYPE_2vs2){
				mp->msg_id=TuushinKuridashi2vs2Msg;
				mp->msg_tag=TAG_TRNAME_NICK_NICK;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[2]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else{
				mp->msg_id=TuushinKuridashiMsg;
				mp->msg_tag=TAG_TRNAME_NICK;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
			}
		}
		else{
			if((fight_type&FIGHT_TYPE_TAG)||
			   (fight_type&FIGHT_TYPE_MULTI)){
				mp->msg_id=KuridashiMsg4;
				mp->msg_tag=TAG_TRTYPE_TRNAME_NICKx2;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no;
				mp->msg_para[2]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[3]=cli2_no;
				mp->msg_para[4]=cli2_no;
				mp->msg_para[5]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else if(fight_type&FIGHT_TYPE_2vs2){
				mp->msg_id=Kuridashi2vs2Msg;
				mp->msg_tag=TAG_TRTYPE_TRNAME_NICK_NICK;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no;
				mp->msg_para[2]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[3]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else{
				mp->msg_id=KuridashiMsg;
				mp->msg_tag=TAG_TRTYPE_TRNAME_NICK;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no;
				mp->msg_para[2]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
			}
		}
	}
	else{
		if(fight_type&FIGHT_TYPE_SIO){
			{
				u8	sio_id=BattleWorkCommIDGet(bw);

				if(fight_type&FIGHT_TYPE_MULTI){
					switch(BattleWorkCommStandNoGet(bw,sio_id)){
					case 0:
					case 3:
						cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_C);
						cli2_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_A);
						break;
					case 1:
					case 2:
						cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_A);
						cli2_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_C);
						break;
					}
				}
				else if(fight_type&FIGHT_TYPE_2vs2){
					cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_A);
					cli2_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_C);
				}
				else{
					cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_AA);
					cli2_no=cli1_no;
				}
			}
		}
		else if(fight_type&FIGHT_TYPE_MULTI){
			cli1_no=BattleWorkPartnerClientNoGet(bw,cp->client_no);
			cli2_no=cp->client_no;
		}
		else if(fight_type&FIGHT_TYPE_2vs2){
			cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_A);
			cli2_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_C);
		}
		else{
			cli1_no=cp->client_no;
			cli2_no=cli1_no;
		}
	
		if(fight_type&FIGHT_TYPE_SIO){
			if(fight_type&FIGHT_TYPE_MULTI){
				mp->msg_id=TuushinAppearMultiMsg;
				mp->msg_tag=TAG_TRNAME_NICK_NICK;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[2]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else if(fight_type&FIGHT_TYPE_2vs2){
				mp->msg_id=Appear2vs2Msg;
				mp->msg_tag=TAG_NICK_NICK;
				mp->msg_para[0]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[1]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else{
				mp->msg_id=AppearMsg_0;
				mp->msg_tag=TAG_NICK;
				mp->msg_para[0]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
			}
		}
		else{
			if(fight_type&FIGHT_TYPE_MULTI){
				mp->msg_id=AppearMsg3;
				mp->msg_tag=TAG_TRTYPE_TRNAME_NICK_NICK;
				mp->msg_para[0]=cli1_no;
				mp->msg_para[1]=cli1_no;
				mp->msg_para[2]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[3]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else if(fight_type&FIGHT_TYPE_2vs2){
				mp->msg_id=Appear2vs2Msg;
				mp->msg_tag=TAG_NICK_NICK;
				mp->msg_para[0]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
				mp->msg_para[1]=cli2_no|(ekmp->sel_mons_no[cli2_no]<<8);
			}
			else{
				mp->msg_id=AppearMsg_0;
				mp->msg_tag=TAG_NICK;
				mp->msg_para[0]=cli1_no|(ekmp->sel_mons_no[cli1_no]<<8);
			}
		}
	}
}

//============================================================================================
/**
 *	�ʐM�ΐ펞�̏��s���b�Z�[�W�̃p�����[�^�𐶐�����
 *
 * @param[in]	bw		�퓬�V�X�e�����[�N
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 * @param[in]	mp		���b�Z�[�W�p�����[�^
 */
//============================================================================================
static	void	CT_WinLoseMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *mp)
{
	u32	fight_type;
	int	cli1_no;
	int	cli2_no;
	int	win_lose_flag;

	fight_type=BattleWorkFightTypeGet(bw);
	win_lose_flag=BattleWorkWinLoseGet(bw);

	if(fight_type&FIGHT_TYPE_2vs2){
		cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_B);
		cli2_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_D);
	}
	else{
		cli1_no=BattleWorkClientNoGet(bw,CLIENT_TYPE_BB);
		cli2_no=cli1_no;
	}

	switch(win_lose_flag){
	case FIGHT_WIN:
		if(fight_type&FIGHT_TYPE_MULTI){
			mp->msg_id=TuushinWinMsg2;
			mp->msg_tag=TAG_TRNAME_TRNAME;
			mp->msg_para[0]=cli1_no;
			mp->msg_para[1]=cli2_no;
		}
		else{
			mp->msg_id=TuushinWinMsg;
			mp->msg_tag=TAG_TRNAME;
			mp->msg_para[0]=cli1_no;
		}
		break;
	case FIGHT_LOSE:
		if(fight_type&FIGHT_TYPE_MULTI){
			mp->msg_id=TuushinLoseMsg2;
			mp->msg_tag=TAG_TRNAME_TRNAME;
			mp->msg_para[0]=cli1_no;
			mp->msg_para[1]=cli2_no;
		}
		else{
			mp->msg_id=TuushinLoseMsg;
			mp->msg_tag=TAG_TRNAME;
			mp->msg_para[0]=cli1_no;
		}
		break;
	case FIGHT_DRAW:
		if(fight_type&FIGHT_TYPE_MULTI){
			mp->msg_id=TuushinDrawMsg2;
			mp->msg_tag=TAG_TRNAME_TRNAME;
			mp->msg_para[0]=cli1_no;
			mp->msg_para[1]=cli2_no;
		}
		else{
			mp->msg_id=TuushinDrawMsg;
			mp->msg_tag=TAG_TRNAME;
			mp->msg_para[0]=cli1_no;
		}
		break;
	}
}

//============================================================================================
/**
 *	�ʐM�ΐ펞�̂ɂ��郁�b�Z�[�W�̃p�����[�^�𐶐�����
 *
 * @param[in]	bw		�퓬�V�X�e�����[�N
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 * @param[in]	emp		�����郁�b�Z�[�W�p�����[�^
 * @param[in]	mp		���b�Z�[�W�p�����[�^
 */
//============================================================================================
static	void	CT_EscapeMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,ESCAPE_MESSAGE_PARAM *emp,MESSAGE_PARAM *mp)
{
	int	client_no;
	int	mine_escape=0;
	int	enemy_escape=0;

//	GF_ASSERT(emp->escape_flag!=0);
	
	for(client_no=0;client_no<BattleWorkClientSetMaxGet(bw);client_no++){
		if(emp->escape_flag&No2Bit(client_no)){
			if(BattleWorkMineEnemyCheck(bw,client_no)){
				enemy_escape++;
			}
			else{
				mine_escape++;
			}
		}
	}

	if((mine_escape)&&(enemy_escape)){
		mp->msg_id=EscapeMsg;
		mp->msg_tag=TAG_NONE;
		BattleWorkWinLoseSet(bw,FIGHT_DRAW|FIGHT_ESCAPE_SELECT|FIGHT_ESCAPE_SELECT_PAUSE);
	}
	else if(mine_escape){
		mp->msg_id=EscapeMsg;
		mp->msg_tag=TAG_NONE;
		BattleWorkWinLoseSet(bw,FIGHT_LOSE|FIGHT_ESCAPE_SELECT|FIGHT_ESCAPE_SELECT_PAUSE);
	}
	else{
		if(BattleWorkFightTypeGet(bw)&FIGHT_TYPE_MULTI){
			mp->msg_id=TuushinEscapeMsg2;
			mp->msg_tag=TAG_TRNAME_TRNAME;
			mp->msg_para[0]=BattleWorkClientNoGet(bw,CLIENT_TYPE_B);
			mp->msg_para[1]=BattleWorkClientNoGet(bw,CLIENT_TYPE_D);
		}
		else if(BattleWorkFightTypeGet(bw)&FIGHT_TYPE_2vs2){
			mp->msg_id=TuushinEscapeMsg;
			mp->msg_tag=TAG_TRNAME;
			mp->msg_para[0]=BattleWorkClientNoGet(bw,CLIENT_TYPE_B);
		}
		else{
			mp->msg_id=TuushinEscapeMsg;
			mp->msg_tag=TAG_TRNAME;
			mp->msg_para[0]=BattleWorkClientNoGet(bw,CLIENT_TYPE_BB);
		}
		BattleWorkWinLoseSet(bw,FIGHT_WIN|FIGHT_ESCAPE_SELECT|FIGHT_ESCAPE_SELECT_PAUSE);
	}
#ifdef PM_DEBUG
#ifndef DEBUG_PRINT_BATTLE
	if(emp->escape_flag&0x80){
		debug_battle_work|=DBW_RESTART;
	}
#endif
#endif PM_DEBUG

}

//============================================================================================
/**
 *	�o�g���^���[�̃��^�C�A���b�Z�[�W�̃p�����[�^�𐶐�����
 *
 * @param[in]	bw		�퓬�V�X�e�����[�N
 * @param[in]	cp		�N���C�A���g���[�N�|�C���^
 * @param[in]	mp		���b�Z�[�W�p�����[�^
 */
//============================================================================================
static	void	CT_GiveUpMessageParamMake(BATTLE_WORK *bw,CLIENT_PARAM *cp,MESSAGE_PARAM *mp)
{
	mp->msg_id=GiveUpMsg2;
	mp->msg_tag=TAG_TRNAME;

	if(BattleWorkFightTypeGet(bw)&FIGHT_TYPE_SIO){
		if(BattleWorkCommStandNoGet(bw,BattleWorkCommIDGet(bw))){
			mp->msg_para[0]=BattleWorkClientNoGet(bw,CLIENT_TYPE_C);
		}
		else{
			mp->msg_para[0]=BattleWorkClientNoGet(bw,CLIENT_TYPE_A);
		}
	}
	else{
		mp->msg_para[0]=cp->client_no;
	}
	BattleWorkWinLoseSet(bw,FIGHT_LOSE|FIGHT_ESCAPE_SELECT|FIGHT_ESCAPE_SELECT_PAUSE);
}

//============================================================================================
/**
 *	�g���[�i�[�L������CLACT���\�[�X�̃��[�h
 *
 * @param[in]	bw		�퓬�V�X�e���\����
 * @param[in]	type	client_type
 * @param[in]	trtype	�g���[�i�[�^�C�v
 * @param[in]	id		CLACT���\�[�XID�ւ̃I�t�Z�b�g
 * @param[in]	pos_x	�����ʒuX
 * @param[in]	pos_x	�����ʒuY
 */
//============================================================================================
static	CATS_ACT_PTR	CT_TrainerCLACTResourceLoad(BATTLE_WORK *bw,int type,int trtype,int id,s16 pos_x,s16 pos_y)
{
	CATS_SYS_PTR		csp;
	CATS_RES_PTR		crp;
	PALETTE_FADE_PTR	pfd;
	CATS_ACT_PTR		cap;	
	int					dir;

	csp=BattleWorkCATS_SYS_PTRGet(bw);
	crp=BattleWorkCATS_RES_PTRGet(bw);
	pfd=BattleWorkPfdGet(bw);

	if(type&CLIENT_ENEMY_FLAG){
		dir=PARA_FRONT;
	}
	else{
		dir=PARA_BACK;
	}

	cap=TrCLACTSet(csp,crp,pfd,pos_x,pos_y,trtype,dir,id,HEAPID_BATTLE);

	return cap;
}

//============================================================================================
/**
 *	�g���[�i�[�L������CLACT���\�[�X�̃��[�h
 *
 * @param[in]	bw		�퓬�V�X�e���\����
 * @param[in]	type	client_type
 * @param[in]	trtype	�g���[�i�[�^�C�v
 * @param[in]	id		CLACT���\�[�XID�ւ̃I�t�Z�b�g
 */
//============================================================================================
static	void	CT_WesCall(BATTLE_WORK *bw,CLIENT_PARAM *cp,WE_SYS_PTR wsp,WAZA_EFFECT_PARAM *wep)
{
	WES_CALL_BTL_EX_PARAM wcbep;
	int i;
	int	callID;

	/// �퓬�ɓn���g������
	if(wep->flag==EFFTYPE_WAZA){
		wcbep.eff_arc_id = ARC_WAZA_EFF;
		callID=wep->waza_no;
	}
	else{
		wcbep.eff_arc_id = ARC_WAZA_EFF_SUB;
		callID=wep->data;
	}
	wcbep.bgl  = BattleWorkGF_BGL_INIGet(bw);					///< BGL*
	wcbep.pfd  = BattleWorkPfdGet(bw);							///< PFD*
	wcbep.csp  = BattleWorkCATS_SYS_PTRGet(bw);					///< CSP*
	for(i = 0; i < CLIENT_MAX; i++){
		wcbep.excp[i] = BattleWorkExChrClientParamGet(bw, i);
		wcbep.mons_no[i] = wep->mons_no[i];
		wcbep.sex[i] = wep->sex[i];
		wcbep.rare[i] = wep->rare[i];
		wcbep.form_no[i] = wep->form_no[i];
		wcbep.personal_rnd[i] = wep->personal_rnd[i];
		wcbep.waza_kouka[i] = wep->waza_kouka[i];
	}
	BattleClientTypeBufMake(bw, &(wcbep.client_type[0]));			///< �N���C�A���gType
	BattleSoftSpriteBufMake(bw, &(wcbep.ss[0]));					///< �\�t�g�X�v���C�g
	wcbep.fight_type = BattleWorkFightTypeGet(bw);					///< �t�@�C�g�^�C�v�擾
	wcbep.perap_voice = BattleWorkPerapVoiceGet(bw, cp->client_no);	///< �y���b�v
	wcbep.bg_area = BattleWorkBGAreaGet(bw);						///< BG�G���A
	wcbep.pal_area = BattleWorkPalAreaGet(bw);						///< PAL�G���A
	
	///< �w�i�f�[�^
	wcbep.haikei_data.arc_index = ARC_BATT_BG;
	wcbep.haikei_data.chr_id	= BATTLE_BG00_NCGR_BIN + BattleWorkBGIDGet(bw);
	wcbep.haikei_data.pal_id	= BATT_BG00_D_NCLR + BattleWorkBGIDGet(bw)*3+BattleWorkTimeZoneOffsetGet(bw);
	wcbep.haikei_data.scr_id	= BATTLE_BG00_NSCR_BIN;
	wcbep.haikei_data.pal_start	= 0;
	wcbep.haikei_data.pal_num	= 8;
	
	WES_Call_Ex(wsp, wep, callID, &wcbep);
}

//============================================================================================
/**
 *	WAZA_POKE_CHANGE_PARAM�\���̂̐���
 *
 * @param[in]	bw			�퓬�V�X�e���\����
 * @param[in]	wep			WAZA_POKE_CHANGE_PARAM�\���̂𐶐����邽�߂̃p�����[�^
 * @param[in]	wpcp		��������WAZA_POKE_CHANGE_PARAM�\���̃f�[�^�i�[��
 * @param[in]	client_no	��������ClientNo
 */
//============================================================================================
static	void	CT_WazaPokeChangeParamMake(BATTLE_WORK *bw,WAZA_EFFECT_PARAM *wep,WAZA_POKE_CHANGE_PARAM *wpcp,int client_no)
{
	int	i;

	wpcp->at_client_no=client_no;
	wpcp->df_client_no=client_no;

	for(i=0;i<CLIENT_MAX;i++){
		wpcp->excp[i]			=BattleWorkExChrClientParamGet(bw,i);
		wpcp->mons_no[i]		=wep->mons_no[i];
		wpcp->sex[i]			=wep->sex[i];
		wpcp->rare[i]			=wep->rare[i];
		wpcp->form_no[i]		=wep->form_no[i];
		wpcp->personal_rnd[i]	=wep->personal_rnd[i];
	}
	BattleClientTypeBufMake(bw, &(wpcp->client_type[0]));			///< �N���C�A���gType
	BattleSoftSpriteBufMake(bw, &(wpcp->ss[0]));					///< �\�t�g�X�v���C�g
}

//============================================================================================
/**
 *	�G�t�F�N�g��������HP�Q�[�W�Ɖe�̃I���I�t�𐧌�
 *
 * @param[out]	gauge		HP�Q�[�WON/OFF�t���O
 * @param[out]	shadow		�eON/OFF�t���O
 * @param[in]	eff_type	�G�t�F�N�g���
 * @param[in]	eff_no		�G�t�F�N�g�i���o�[
 * @param[in]	waza_no		�Z�G�t�F�N�g���̋Z�i���o�[
 */
//============================================================================================
static	void	CT_WazaEffectGaugeShadowOnOffCheck(u8 *gauge,u8 *shadow,int eff_type,int eff_no,u16 waza_no)
{
	if(eff_type==EFFTYPE_WAZA){
		if((WT_WazaDataParaGet(waza_no,ID_WTD_flag)&FLAG_PUT_GAUGE)==0){
			gauge[0]=1;
		}
		else{
			gauge[0]=0;
		}
		if(WT_WazaDataParaGet(waza_no,ID_WTD_flag)&FLAG_DEL_SHADOW){
			shadow[0]=1;
		}
		else{
			shadow[0]=0;
		}
	}
	else{
		switch(eff_no){
		case STATUS_WEATHER_MIST:
		case STATUS_WEATHER_RAIN:
		case STATUS_WEATHER_ICE:
		case STATUS_WEATHER_SUN:
		case STATUS_WEATHER_SAND:
		case STATUS_DAMAGE_AKUMU:
		case STATUS_DAMAGE_YADORIGI:
		case STATUS_DAMAGE_MAKITUKU:
		case STATUS_DAMAGE_HONOO:
		case STATUS_DAMAGE_KARA:
		case STATUS_DAMAGE_SUNA:
			gauge[0]=1;
			shadow[0]=0;
			break;
		case STATUS_DAMAGE_MAGUMA:
		case STATUS_DAMAGE_UZUSIO:
			gauge[0]=1;
			shadow[0]=1;
			break;
		default:
			gauge[0]=0;
			shadow[0]=0;
			break;
		}
	}
}

//============================================================================================
/**
 *	�g���[�i�[�A�j���ɐݒ肳��Ă���G�t�F�N�g�̔����`�F�b�N
 *
 * @param[in]	tte		�g���[�i�[�G���J�E���g�G�t�F�N�g���[�N�\���̂ւ̃|�C���^
 * @param[in]	cap		�g���[�i�[�L����CLACT�\����
 *
 * @retval	FALSE �A�j���[�V�����p���@TRUE�@�A�j���[�V�����r���ł��T�[�o�ɏI���Ɠ`����i��ɐi�܂���j
 */
//============================================================================================
enum{
	TEAE_WHITE_FADE_EFF=1,
	TEAE_ANM_END=0xfff,
};
static	BOOL	CT_TrainerEncountAnimeEffectCheck(TCB_TRAINER_ENCOUNT *tte,CATS_ACT_PTR cap)
{
	TEAE_WORK	*tw;
	int			attr;
	BOOL		ret=FALSE;

	//���łɃG�t�F�N�g���N�����Ă���Ȃ�A�������Ȃ�
//	if(tte->eff_flag){
//		return;
//	}

	attr=CATS_ObjectUserAttrNowAnimFrameGet(tte->cap);

	switch(attr){
	case TEAE_WHITE_FADE_EFF:
		if(tte->eff_flag==0){
			tte->eff_flag=1;
			tw=(TEAE_WORK *)sys_AllocMemory(HEAPID_BATTLE,sizeof(TEAE_WORK));
			tw->seq_no=0;
			tw->work=0;
			TCB_Add(TEAE_WhiteFadeEffect,tw,0);
		}
		break;
	case TEAE_ANM_END:
		ret=TRUE;
		break;
	default:
		if(((attr&0xf00)==0x100)&&(attr&0xff)){
			CATS_ObjectAnimeFrameSetCap(tte->cap,0);
			CATS_ObjectAnimeSeqSetCap(tte->cap,(attr&0x0ff)-1);
			ret=TRUE;
		}
		break;
	}

	return ret;
}

//============================================================================================
/**
 *	��ʔ��t�F�[�h�G�t�F�N�g
 */
//============================================================================================
static	void	TEAE_WhiteFadeEffect(TCB_PTR tcb,void *work)
{
	TEAE_WORK	*tw=(TEAE_WORK *)work;

	switch(tw->seq_no){
	case 0:
		ChangeBrightnessRequest(4,BRIGHTNESS_WHITE,BRIGHTNESS_NORMAL,PLANEMASK_ALL&(PLANEMASK_BG1^0xffff),MASK_MAIN_DISPLAY);
		tw->seq_no++;
		break;
	case 1:
		if(IsFinishedBrightnessChg(MASK_MAIN_DISPLAY)==TRUE){
			ChangeBrightnessRequest(4,BRIGHTNESS_NORMAL,BRIGHTNESS_WHITE,PLANEMASK_ALL&(PLANEMASK_BG1^0xffff),MASK_MAIN_DISPLAY);
			tw->seq_no++;
		}
		break;
	case 2:
		if(IsFinishedBrightnessChg(MASK_MAIN_DISPLAY)==TRUE){
			sys_FreeMemoryEz(work);
			TCB_Delete(tcb);
		}
		break;
	}
}

//============================================================================================
/**
 *	�f�[�^�G���[�ɂ��^��Đ����f����
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 */
//============================================================================================
static	void	BattleRecDataErrorStop(BATTLE_WORK *bw)
{
	BattleWorkBattleRecStopFlagSet(bw,BI_SYSMSG_ERROR);
	ST_BattleRecordStopCheck(bw,BattleWorkServerParamGet(bw));
}

//============================================================================================
/**
 *	�f�[�^MAX�ɂ��^��Đ����f����
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 */
//============================================================================================
static	void	BattleRecDataFullStop(BATTLE_WORK *bw)
{
	BattleWorkBattleRecStopFlagSet(bw,BI_SYSMSG_DATAFULL);
	ST_BattleRecordStopCheck(bw,BattleWorkServerParamGet(bw));
}

//============================================================================================
/**
 *	�����R�[�h���`�F�b�N���ēK���ȃg���[�i�[�^�C�v�ɕϊ�����
 *
 * @param[in]	bw		�퓬�V�X�e���p���[�N�\����
 */
//============================================================================================
static	u8	CheckRomCodeTrainerTypeChange(BATTLE_WORK *bw,u8 client_no,u8 trtype)
{
	if(BattleWorkFightTypeGet(bw)&FIGHT_TYPE_SIO){
		//�����R�[�h���O�̏ꍇ�́A�N���C�A���g��DP�Ȃ̂ŁA�g���[�i�[�^�C�v��ύX����
		if((MyStatus_GetRomCode(BattleWorkMyStatusGet(bw,client_no))==0)&&(trtype<=TRTYPE_GIRL)){
			trtype+=TRTYPE_BOY_DP;
		}
	}
	return trtype;
}

