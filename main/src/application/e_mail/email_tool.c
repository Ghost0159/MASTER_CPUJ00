//==============================================================================
/**
 * @file	email_tool.c
 * @brief	E���[���F�c�[����
 * @author	matsuda
 * @date	2007.10.22(��)
 */
//==============================================================================
#include "common.h"
#include <dwc.h>
#include "libdpw/dpw_tr.h"
#include "gflib/strcode.h"
#include "system/procsys.h"
#include "savedata/config.h"
#include "gflib/strbuf.h"
#include "gflib/strbuf_family.h"

#include "application/email_main.h"


//==============================================================================
//	ASCII�R�[�h�ϊ��e�[�u��
//==============================================================================
///STRCODE, ASCII�R�[�h�ϊ��e�[�u��(E���[���Ŏg�p���镶���̂�)
static const struct{
	u16 str_code;
	char ascii_code;
	u8 padding;
}AsciiConvertTbl[] = {
	{n0_,'0'},
	{n1_,'1'},
	{n2_,'2'},
	{n3_,'3'},
	{n4_,'4'},
	{n5_,'5'},
	{n6_,'6'},
	{n7_,'7'},
	{n8_,'8'},
	{n9_,'9'},
	{period_,'.'},
	{bou_,'-'},
	{under_,'_'},
	{anpersand_,'&'},
	{atmark_,'@'},
	{A__,'A'},
	{B__,'B'},
	{C__,'C'},
	{D__,'D'},
	{E__,'E'},
	{F__,'F'},
	{G__,'G'},
	{H__,'H'},
	{I__,'I'},
	{J__,'J'},
	{K__,'K'},
	{L__,'L'},
	{M__,'M'},
	{N__,'N'},
	{O__,'O'},
	{P__,'P'},
	{Q__,'Q'},
	{R__,'R'},
	{S__,'S'},
	{T__,'T'},
	{U__,'U'},
	{V__,'V'},
	{W__,'W'},
	{X__,'X'},
	{Y__,'Y'},
	{Z__,'Z'},
	{a__,'a'},
	{b__,'b'},
	{c__,'c'},
	{d__,'d'},
	{e__,'e'},
	{f__,'f'},
	{g__,'g'},
	{h__,'h'},
	{i__,'i'},
	{j__,'j'},
	{k__,'k'},
	{l__,'l'},
	{m__,'m'},
	{n__,'n'},
	{o__,'o'},
	{p__,'p'},
	{q__,'q'},
	{r__,'r'},
	{s__,'s'},
	{t__,'t'},
	{u__,'u'},
	{v__,'v'},
	{w__,'w'},
	{x__,'x'},
	{y__,'y'},
	{z__,'z'},
};


//--------------------------------------------------------------
/**
 * @brief   STRCODE��ASCII�R�[�h�ɕϊ�
 *
 * @param   src			�ϊ������b�Z�[�W
 * @param   dest		�ϊ����ASCII�R�[�h�����
 * @param   heap_id		�q�[�vID
 */
//--------------------------------------------------------------
void Email_Strcode_to_Ascii(STRBUF *src, char *dest, int heap_id)
{
	STRCODE *code;
	int pos = 0, i;
	
	code = sys_AllocMemory(heap_id, sizeof(STRCODE) * EMAIL_ADDRESS_LEN_SIZE);
	
	STRBUF_GetStringCode(src, code, EMAIL_ADDRESS_LEN_SIZE);
	while(1){
		if(code[pos] == EOM_){
			dest[pos] = '\0';
			break;
		}
		for(i = 0; i < NELEMS(AsciiConvertTbl); i++){
			if(AsciiConvertTbl[i].str_code == code[pos]){
				dest[pos] = AsciiConvertTbl[i].ascii_code;
				break;
			}
		}
		GF_ASSERT(i < NELEMS(AsciiConvertTbl));	//�q�b�g���Ȃ��������܂܂�Ă���
		pos++;
	}
	
	OS_Printf( "mail address %s\n", dest );
	
	sys_FreeMemoryEz(code);
}

//--------------------------------------------------------------
/**
 * @brief   ASCII�R�[�h��STRCODE�ɕϊ�
 *
 * @param   src			�ϊ������b�Z�[�W
 * @param   dest		�ϊ����STRCODE�����
 * @param   heap_id		�q�[�vID
 */
//--------------------------------------------------------------
void Email_Ascii_to_Strcode(char *src, STRBUF *dest, int heap_id)
{
	STRCODE *code;
	int pos = 0, i;
	
	code = sys_AllocMemory(heap_id, sizeof(STRCODE) * EMAIL_ADDRESS_LEN_SIZE);
	
	while(1){
		if(src[pos] == '\0'){
			code[pos] = EOM_;
			break;
		}
		for(i = 0; i < NELEMS(AsciiConvertTbl); i++){
			if(AsciiConvertTbl[i].ascii_code == src[pos]){
				code[pos] = AsciiConvertTbl[i].str_code;
				break;
			}
		}
		GF_ASSERT(i < NELEMS(AsciiConvertTbl));	//�q�b�g���Ȃ��������܂܂�Ă���
		pos++;
	}
	
	STRBUF_SetStringCode(dest, code);
	sys_FreeMemoryEz(code);
}

