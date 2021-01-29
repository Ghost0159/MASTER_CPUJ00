##################################################################
#	gndcvall.rb
#	��Ԃꂽ�������@�C�x���g�f�[�^�A
#	�J�����A�N�V�����f�[�^�t�@�C���S�ẴR���o�[�g�ƈꗗ����
#	evflist.rb ��������ꗗ�t�@�C����
#	�O�� "exec" "system"
##################################################################

#���C�u����
require "gndcv.rb"	#�f�[�^�R���o�[�g

#�萔

#=================================================================
#	�n�`�t�@�C���R���o�[�g
#	list �������ݐ�t�@�C��
#	return 0=����I��
#=================================================================
def gnd_make_gnddata( list )
	ret = 0
	count = 1
	convert = "../../../convert/exceltool/excelcommaconv"
	idx = "./tw_arc_attr.naix"
	dir = Dir.open( Dir.pwd + "/tw_ground/" )			# tw_ground path
	winpath = dir.path								
	winpath = winpath.sub( /\/cygdrive\/c\//, "c:/" )	# win path
	
	list.printf( "TW_GNDDATA_GNDLIST =" )				# TW_GNDDATA_GNDLIST
	
	dir.each{ |name|									# xls conv write list
		if( name.include?("\.xls") )
			binname = name.sub( /\.xls/, "\.bin" )
			txtname = name.sub( /\.xls/, "\.txt" )
			list.printf( " \\\n" )						# \
			list.printf( "\t%s", winpath+binname )		# �t���p�X
			
			system( convert, winpath+name )				# xls -> txt
			
			ret = gndcv_txtfileconv( winpath+txtname, idx )	# �o�C�i���ϊ�
			if( ret == 1 )
				break
			end
			
			count = count + 1
		end
	}
	
	#�f�B���N�g�������
	dir.close
	
	#�t�@�C�����o��
	printf( "tw gndfile count %d\n", count-1 )
	
	return ret
end

#=================================================================
#	�C�x���g�t�@�C���R���o�[�g
#=================================================================
path = ARGV[0]
if( path == @nil )
	printf( "ERROR gndcvall.rb \"ListFileName\"\n" )
	exit 1
end

list = File.open( path, "w" )

ret = gnd_make_gnddata( list )

list.close

if( ret == 1 )			#�G���[
	File.delete( path )
end
