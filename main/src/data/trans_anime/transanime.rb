#----------------------------------------------------------------
#
# �t�B�[���h�e�N�X�`���]���A�j���f�[�^�R���o�[�g
#
# 2007.04.05
# AkitoMori GameFreak inc.
#
#----------------------------------------------------------------

require "CSV"

# �A�j�����ő��`
TEXNAME_LENGTH  = 16	#MAYA���ł̃e�N�X�`���w�薼
TEXPATH_LENGTH  = 32	#���ۂ�imd�̃t�@�C����
ANIME_MAX   	= 18
ANIME_TABLE 	= ANIME_MAX*2


#==================================================
#------------------------------------
# �]���A�j���Ǘ��N���X
#==================================================
class TransAnime
	def initialize
		@texname    = "asase"
		@texpath    = "asase.nsbmd"
		@animetable = [20]
	end

	#------------------------------------
	# �e�N�X�`�����Z�b�g
	def set_texname( name )
		@texname = name
	end

	#------------------------------------
	# �e�N�X�`���p�X�Z�b�g
	def set_texpath( path )
		#�g���q��imd����nsbmd�ɏ��������Ēu��
		@texpath = path.sub(/\.imd/,"\.nsbtx")
	end

	#------------------------------------
	# �A�j���[�V�����e�[�u���Z�b�g
	def set_animetable( idx, no )
		if no==nil then
			#�f�[�^�����̏ꍇ��FF�Ŗ��߂�
			@animetable[idx] = 0xff
		else
			if (idx%2)==0 then
				#�����Ԗڂ̓C���f�b�N�X�Ȃ̂ł��̂܂�
				@animetable[idx] = no.to_i
			else
				#��Ԗڂ̓A�j���J�E���^�Ȃ̂�30/1�ł��邱�Ƃ��l�����Ĕ����ɂ���
				@animetable[idx] = no.to_i/2
			end
		end
	end

	def get_texpath()
		return @texpath
	end

	#------------------------------------
	# �N���X���\��
	def putinfo
		p @texname 
		p @texpath 
		p @animetable
	end

	#------------------------------------
	# �o�̓o�C�i���ɕϊ�
	def pack

		#��U������Array�Ɋi�[���Ȃ����Ă���pack�֐����Ăяo��
		@tmp = [@texname]
		ANIME_TABLE.times{|n|
			@tmp[1+n] = @animetable[n].to_i
		}

		#�o�̓t�H�[�}�b�g�̍쐬
		@outputstr   = sprintf( "a%dc%d",TEXNAME_LENGTH, ANIME_TABLE )

		#�o�C�i���o��
		return @tmp.pack( @outputstr )
	end
end

#-----------------------------------------
# ���C������
#-----------------------------------------

#�������R�Ȃ�������I��
if ARGV.length<3 then
	p "3 parameters are necessary"
	p "ruby transanime.rb <input>.xls <output1>.bin <output2>.scr"
	exit
end

#�O���[�o���Ȃ��
ta = []
i  = 0


#---------CSV��荞��-----------
CSV.foreach(ARGV[0]){|line|

	#---------�P�s�ڔ�΂�-----------
	if line[0]=="�e�N�X�`����" then
		# p "�P�s�ڂȂ̂Ŕ�΂���"
		next
	end
	
	#---------�e�N�X�`�����̓����ĂȂ��s�͔�΂�-----------
	if line[0]==nil then
		next
	end
	
	#---------��荞�݌��ʂ�TransAnime�N���X��-----------
	ta[i] = TransAnime.new
	ta[i].set_texname(line[0])
	ta[i].set_texpath(line[1])

	ANIME_TABLE.times{|n|
		ta[i].set_animetable(n,line[n+2])
	}
	i += 1

}

printf( "�e�N�X�`���w���%d��\n",i )

#p ta[0].pack
#p ta[1].pack
#p ta[2].pack


#---------�A�j���f�[�^�o��-----------
File.open(ARGV[1].to_str,"wb"){|file|
	#�e�N�X�`���o�^�����o��
	buf = []
	buf = [i.to_i]
	file.write buf.pack("V")

	#�e�N�X�`���A�j���f�[�^���o��
	i.times{|n|
		file.write ta[n].pack
	}
}

#---------�A�[�J�C�u�p���X�g�o��-----------
File.open(ARGV[2],"wb"){|file|
	# �ŏ��ɃA�j���e�[�u��
	file.write "\""+ARGV[1]+"\"\n"

	# ���Ƃ̓e�N�X�`���t�@�C���̗���
	i.times{|n|
		file.write "\""+ta[n].get_texpath()+"\"\n"
	}
}
