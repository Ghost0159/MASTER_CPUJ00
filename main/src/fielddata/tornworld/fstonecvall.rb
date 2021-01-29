##################################################################
#	fstonecvall.rb
#	��Ԃꂽ�������@��ѐ΃f�[�^
#	��ѐ΃f�[�^�t�@�C���S�ẴR���o�[�g�ƈꗗ����
#	fstonecvall.rb ��������ꗗ�t�@�C����
##################################################################

#���C�u����
require "fstonecv.rb"

#=================================================================
#	��ѐ΃t�@�C���f�B���N�g���R���o�[�g
#	list �������ݐ�t�@�C��
#	return 1 == ERROR
#=================================================================
def fstonecv_dir( list )
	ret = 0
	count = 1
	convert = "../../../convert/exceltool/excelcommaconv"
	dir = Dir.open( Dir.pwd + "/tw_fstone/" )			# dir path
	winpath = dir.path								
	winpath = winpath.sub( /\/cygdrive\/c\//, "c:/" )	# win path
	
	list.printf( "TW_DATA_FSTONE_LIST=" )				# label
	
	dir.each{ |name|									# xls cv write list
		if( name.include?("\.xls") )
			binname = name.sub( /\.xls/, "\.bin" )
			txtname = name.sub( /\.xls/, "\.txt" )
			list.printf( " \\\n" )
			list.printf( "\t%s", winpath+binname )		# �t���p�X
			
			system( convert, winpath+name )				# xls -> txt
			
			ret = fstonecv_conv( winpath+txtname )		# �o�C�i���ϊ�
			if( ret == 1 )
				break
			end
			
			count = count + 1
		end
	}
	
	#�f�B���N�g�������
	dir.close
	
	#�t�@�C�����o��
	printf( "tw fstonefile count %d\n", count-1 )
	
	return ret
end

#=================================================================
#	��ѐ΃t�@�C���S�ăR���o�[�g
#=================================================================
name = ARGV[0]
if( name == @nil )
	printf( "ERROR fstonecvall.rb \"ListFileName\"\n" )
	exit 1
end

list = File.open( name, "w" )
ret = fstonecv_dir( list )
list.close
if( ret == 1 )
	File.delete( name )
end
