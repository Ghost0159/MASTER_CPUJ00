#====================================================================================
#
#	tr_bgm.xls���R���o�[�g
#
#	TRTYPE���王���Ȃ��Ђ��ς�e�[�u���𐶐�����
#
#	06.06.01 Satoshi Nohara
#
#====================================================================================
BEGIN{
	print "const static u16 trtype_bgm_tbl[][2] = {"
	count = 0
}

{
	if( $1 ~ "TRTYPE" ){
		type = toupper( $1 )
		bgm  = toupper( $2 )
		sub( ".MID", "", bgm )
		sub( "MUS_DP", "", bgm )
		print "\t{ " type ",\t\t" "SEQ" bgm " },"
		count++
	}
}

END{
	print "};"
	print "#define TRTYPE_BGM_TBL_MAX\t(" count ")"
}
