#===========================================================================================
#
#	050826 Satoshi Nohara
#
#	bat�t�@�C����awk��������������
#
#===========================================================================================
{
	name = $1

	#��O
	#"sp_"����X�N���v�g�t�@�C���͑Ώۂ���O���I
	if( name ~ "sp_" ){
		next
	}

	#��O
	#".dat"��Ώۂɂ���I
	#if( (name ~ "fld") && (name ~ "item") ){
		#sub(/\..*$/,"",name)										#"."�ȍ~���폜(�� fld_item)
		#print "awk -f ev_def.awk " name ".dat"
		#next
	#}

	print "awk -f ev_def.awk " name
}

