using System;
using System.Reflection;

namespace AdminTool
{
    public class DpwUtil
    {
        // ������r���h����ɂ�dpwutil�̓o�^���K�v�B
        // regsvr32 dpwutil.dll
        // DPWUTILLib�̎Q�Ɛݒ�ŕ�����true�ɂ��Ă���̂ŁA�z�z��œo�^������K�v�͂Ȃ�
        public static DPWUTILLib.Tools Tools = new DPWUTILLib.Tools();
        public static DPWUTILLib.PokemonChecker PokemonChecker = new DPWUTILLib.PokemonChecker();
        public static DPWUTILLib.Base64CoDec Base64CoDec = new DPWUTILLib.Base64CoDec();
        public static DPWUTILLib.BinaryCoDec BinaryCoDec = new DPWUTILLib.BinaryCoDec();
        public static DPWUTILLib.Encryptor Encryptor = new DPWUTILLib.Encryptor();
    }
}