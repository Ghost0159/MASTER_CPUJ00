{
	name = $0 
	sub( "SEQ_", "", name )

	#"SE_"�ɑΉ�
	sub( "_", "-", name )

	#"DP_"�ɑΉ�
	sub( "_", "-", name )

	#"_"�ɑΉ�
	sub( "_", "-", name )
	sub( "_", "-", name )
	sub( "_", "-", name )
	sub( "_", "-", name )

	print name
}
