#=========================================================
#
#	snd_test_msg.awk�Ő��������tmp.txt����A
#	msgedit�p�̃��b�Z�[�WID��tmp2.txt�ɏo��
#
#=========================================================
BEGIN{
	count = 0
}

{
	#print "msg_snd_name" count
	#count++

	tmp = $0
	#tmp2 = toupper( tmp )
	tmp2 = tolower( tmp )

	print "msg_" tmp2
}


