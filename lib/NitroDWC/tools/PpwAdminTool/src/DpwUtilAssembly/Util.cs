using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace AdminTool
{
    public class Util
    {
        public static int FriendCodeToProfileId(UInt64 friendCode)
        {
            return (int)friendCode;
        }

        public static ushort ProfileIdToUserId(int profileId)
        {
            return (ushort)profileId;
        }

        public enum Language
        {
            NONE,
            JP,
            EN,
            FR,
            IT,
            DE,
            RESERVE,
            SP,
            KR
        }

        public static Language IndexToLanguageCode(int index)
        {
            switch(index)
            {
                case 0:
                    return Language.JP;
                case 1:
                    return Language.EN;
                case 2:
                    return Language.FR;
                case 3:
                    return Language.IT;
                case 4:
                    return Language.DE;
                case 5:
                    return Language.SP;
                case 6:
                    return Language.KR;
                default:
                    throw new Exception("����R�[�h���s���ł�");
            }
        }

        public static int LanguageCodeToIndex(Language language)
        {
            switch (language)
            {
                case Language.JP:
                    return 0;
                case Language.EN:
                    return 1;
                case Language.FR:
                    return 2;
                case Language.IT:
                    return 3;
                case Language.DE:
                    return 4;
                case Language.SP:
                    return 5;
                case Language.KR:
                    return 6;
                default:
                    throw new Exception("�s���Ȍ���R�[�h�̃C���f�b�N�X�ł��B");
            }
        }


        public enum RomVersion
        {
            PLATINUM
        }

        public static List<List<string>> TsvTextToArray(string text)
        {
            List<List<string>> result = new List<List<string>>();
            if (string.IsNullOrEmpty(text))
            {
                return result;
            }

            text = text.Replace("\r\n", "\n");
            text = text.Replace('\r', '\n');
            text.TrimEnd(new char[] { '\n' });
            string[] lines = text.Split('\n');

            foreach (string line in lines)
            {
                //�^�u�ŕ���
                result.Add(new List<string>(line.Split('\t')));
            }
            return result;
        }

        // byte��List���G���f�B�A���ˑ���short�ɂ���B�V�X�e���̃G���f�B�A���Ƌt�ɂ���Ƃ���reverse��true�ɂ���B
        public static List<ushort> BytesToUShorts(List<byte> list, bool reverse)
        {
            if(list.Count % 2 == 1)
            {
                throw new Exception("�v�f������ł��B");
            }

            List<ushort> result = new List<ushort>();

            ushort s;
            for (int i = 0; i < list.Count; i += 2)
            {
                if (reverse ^ !BitConverter.IsLittleEndian)
                {
                    // ���]���Ȃ�
                    // !reverse���r�b�O�G���f�B�A��
                    // reverse�����g���G���f�B�A�� �̂Ƃ�
                    s = (ushort)(list[i] << 8 | list[i + 1]);
                }
                else
                {
                    // ���]����
                    s = (ushort)(list[i+1] << 8 | list[i]);
                }
                result.Add(s);
            }
            return result;
        }

        public static List<byte> UShortsToBytes(List<ushort> list, bool reverse)
        {
            List<byte> result = new List<byte>();
            foreach (ushort s in list)
            {
                byte[] bytes = BitConverter.GetBytes(s);    // GetBytes�̓V�X�e���G���f�B�A���ˑ�
                if (reverse)
                {
                    Array.Reverse(bytes);
                }
                result.AddRange(bytes);
            }
            return result;
        }

        public static List<ushort> UnicodeStringToUShortList(string str)
        {
            return Util.BytesToUShorts(new List<byte>(Encoding.Unicode.GetBytes(str)), false);
        }

        public static string Join<T>(List<T> list, string token)
        {
            string result = "";
            for (int i = 0; i < list.Count; i++)
            {
                if (i != 0)
                {
                    result += token;
                }
                result += list[i];
            }
            return result;
        }

        // NULL�����Ƃ��̌�̕������폜�������̂�Ԃ�
        public static string TrimNullString(string str)
        {
            int index = str.IndexOf('\0');
            return str.Remove(index);
        }

        // �|�P�R�[�h�̃��X�g����g�������ꂽUnicode������ɕϊ�����B
        public static string UShortListToUnicodeString(List<ushort> pokeList)
        {
            List<byte> bytes = Util.UShortsToBytes(pokeList, false);
            return Util.TrimNullString(Util.ConvLFToMacro(DpwUtil.Tools.PokecodeToUnicodeWithoutCheckLF(Encoding.Unicode.GetString(bytes.ToArray()))));
        }

        public const string LFString = "<LF>";
	    // <LF>������s�R�[�h�ɕϊ�����
	    public static string ConvMacroToLF(string src)
	    {
		    return src.Replace(LFString, "\n");
	    }

	    // ���s�R�[�h����<LF>�ɕϊ�����
	    public static string ConvLFToMacro(string src)
	    {
		    return src.Replace("\n", LFString);
	    }
    }
}
