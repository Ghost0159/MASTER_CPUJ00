##################################################
#		
#	wflby_3dmapobj_data.dat����
#	�O���t�B�b�N�f�[�^�̃e�[�u�����쐬����
#
##################################################

@MODATA_FILE		= undef;		#�f�[�^�t�@�C��
@GRA_NAIX_FILE		= undef;		#�O���t�B�b�NNAIX�t�@�C��
@OUTPUT_BUFF		= undef;		#�����o�������̔z��
$OUTPUT_BUFF_NUM	= 0;			#�����o���o�b�t�@��

$OUTPUT_STRUCT_MEMBER_NUM	= 109;	#�����o���\���̂̃����o��

######�ǂݍ��݃t�@�C��
$INPUT_MODATA		= "wflby_3dmapobj_data.c";
$INPUT_NAIX			= "wifi_lobby.naix";
$OUTPUT_FILE		= "wflby_3dmapobj_data";



#�t�@�C�����J��
&file_open();

#NARC��`�����̔z����쐬����
$return = &makeoutput_buff();
if( $return == -1 ){
	exit(1);
}


#���Ԃɏ����o��
&output_data();

exit(0);


#�t�@�C�����J��
sub file_open{
	open( FILEIN, $INPUT_MODATA );
	@MODATA_FILE = <FILEIN>;
	close( FILEIN );
	
	open( FILEIN, $INPUT_NAIX );
	@GRA_NAIX_FILE = <FILEIN>;
	close( FILEIN );
}

#OUTPUT_BUFF���쐬����
sub makeoutput_buff{

	my( $dat );
	
	$OUTPUT_BUFF_NUM = 0;
	
	foreach $one ( @MODATA_FILE ){
		#�X�y�[�X�A�^�u�A,\r\n��r��
		$one =~ s/[\s\t,\r\n]//g;

		#NARC���������ɓ����Ă��邩�`�F�b�N
		if( $one =~ /NARC/ ){
			#�����ɕϊ����Ĕz��Ɋi�[����
			$dat = &naix_idx_get( $one );
			if( $dat >= 0 ){
				$OUTPUT_BUFF[ $OUTPUT_BUFF_NUM ] = $dat;
				$OUTPUT_BUFF_NUM ++;
			}else{
				return -1;	#��`���Ȃ�
			}
		#��������������ɂ͂����Ă��Ȃ����`�F�b�N
		}elsif( $one =~ /^([0-9]+)/ ){
			$OUTPUT_BUFF[ $OUTPUT_BUFF_NUM ] = $1;
			$OUTPUT_BUFF_NUM ++;
		}
	}
}

#NARC�̒�`�̐�����Ԃ�
sub naix_idx_get{
	my( $word ) = @_;
	my( $count );

	$count = 0;
	while( $count < @GRA_NAIX_FILE ){

		if( $GRA_NAIX_FILE[$count] =~ /$word[^0-9]*([0-9]*)/ ){
#			print( $word." to ".$1."\n" );
			return $1;
		}
		
		$count ++;
	}

	print( "$word������܂���\n" );
	return -1;
}

#�t�@�C���������o��
#�P�̃f�[�^�͂U�ŏo���Ă���̂ŁA���ꂲ�Ƃɏ����o��
sub output_data{

	my($i, $j, $idx, $count, $outfile_no, $outfile_num);

	$count			= 0;
	$outfile_no		= 0;
	$outfile_num	= $OUTPUT_BUFF_NUM / $OUTPUT_STRUCT_MEMBER_NUM;

	for( $i=0; $i<$outfile_num; $i++ ){
		open( FILEOUT, ">".$OUTPUT_FILE."_".$i.".dat" );
		binmode( FILEOUT );
		for( $j=0; $j<$OUTPUT_STRUCT_MEMBER_NUM; $j++ ){

			$idx = ($OUTPUT_STRUCT_MEMBER_NUM*$i)+$j;
			print( FILEOUT pack("I", $OUTPUT_BUFF[$idx]) );
		}
		close( FILEOUT );
	}
	close( FILEOUT );
}
