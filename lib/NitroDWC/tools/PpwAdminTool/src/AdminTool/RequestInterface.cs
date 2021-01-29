/*---------------------------------------------------------------------------*
  Project:  PpwAdminTool

  Copyright 2008 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

 *---------------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Reflection;
using NWC;
using System.Runtime.Serialization.Formatters.Binary;

namespace AdminTool
{
    /// <summary>
    /// ���ʃC���^�t�F�[�X�N���X
    /// </summary>
    public partial class RequestInterface
    {
        // �萔��`
        public const int MAX_REQUEST_SIZE = 2000;

        public const ushort COMMON_REQUEST_REQUEST_VERSION = 1;
        public const byte COMMON_REQUEST_ROM_VERSION = (byte)Util.RomVersion.PLATINUM;
        public const byte COMMON_REQUEST_LANGUAGE = (byte)Util.Language.JP;

        protected static RequestInterface s_instance = null;
        /// <summary>
        /// �ڑ���p�X
        /// </summary>
        public const string URL_ADMIN_SET_SERVER_STATE = "/web/common/adminSetServerState.asp";
        public const string URL_ADMIN_GET_SERVER_STATE = "/web/common/adminGetServerState.asp";

        /// <summary>
        /// �Ǘ����N�G�X�g�pProfileID
        /// </summary>
        public const int ADMIN_PROFILE_ID = 0;

        /// <summary>
        /// �T�[�o�̏��
        /// </summary>
        public enum ServerState : uint
        {
            OK,
            MAINTENANCE
        }

        public static object DeepCopy(object obj)
        {
            // �f�B�[�v�R�s�[
            BinaryFormatter serializer = new BinaryFormatter();
            object loadedObj;
            using (System.IO.MemoryStream ms = new System.IO.MemoryStream())
            {
                serializer.Serialize(ms, obj);
                ms.Seek(0, SeekOrigin.Begin);
                loadedObj = serializer.Deserialize(ms);
            }
            return loadedObj;
        }

        public static void SaveXML<T>(string filePath, T obj)
        {
            try
            {
                System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(T));
                using (System.IO.FileStream fs = new System.IO.FileStream(filePath, System.IO.FileMode.Create))
                {
                    serializer.Serialize(fs, obj);
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("�������ݎ��ɃG���[���������܂����B" + e.Message);
            }
        }

        // �ǂݍ��߂Ȃ��Ƃ��̓f�t�H���g�l(���null)��Ԃ��B
        public static T LoadXML<T>(string filePath)
        {
            try
            {
                System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(T));
                T loadedObj;
                using (System.IO.FileStream fs = new System.IO.FileStream(filePath, System.IO.FileMode.Open))
                {
                    loadedObj = (T)serializer.Deserialize(fs);
                }
                return loadedObj;
            }
            catch (Exception e)
            {
                MessageBox.Show("�ǂݍ��ݎ��ɃG���[���������܂����B�t�@�C���`�������������m�F���Ă��������B" + e.Message);
                return default(T);
            }
        }

        /// <summary>
        /// ���ʂ̃��N�G�X�g�R���e�i
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        [Serializable()]
        public class RequestContainer : IRequestContainer, ICloneable
        {
            public int id;
            public ushort requestVersion;
            public byte romVersion;
            public byte language;
            public uint macAddr1;
            public ushort macAddr2;
            public ushort reserve1;
            public uint reserve2;

            public RequestContainer(){}

            public RequestContainer(int _id)
            {
                id = _id;
                requestVersion = COMMON_REQUEST_REQUEST_VERSION;
                romVersion = COMMON_REQUEST_ROM_VERSION;
                language = COMMON_REQUEST_LANGUAGE;
                macAddr1 = 0xffff;  // �K���ɃZ�b�g
                macAddr2 = 0xff;    // �K���ɃZ�b�g
            }

            public object Clone()
            {
                return DeepCopy(this);
            }

            virtual public string[] ToStringArray()
            {
                List<string> result = new List<string>();
                FieldInfo[] fis = this.GetType().GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
                foreach (FieldInfo fi in fis)
                {
                    if (fi.GetValue(this) is UInt32[])
                    {
                        UInt32[] vals = (UInt32[])fi.GetValue(this);
                        string[] valsStr = new string[vals.Length];
                        int i = 0;
                        foreach (UInt32 val in vals)
                        {
                            valsStr[i] = val.ToString();
                            i++;
                        }
                        result.Add(fi.Name + " = {" + String.Join(", ", valsStr) + "}");
                    }
                    else
                    {
                        result.Add(fi.Name + " = " + fi.GetValue(this).ToString());
                    }
                }

                return result.ToArray();
            }

            public virtual void hton()
            {
                FieldInfo[] fis = this.GetType().GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
                foreach (FieldInfo fi in fis)
                {
                    // �l�ȃv���p�e�B��S�ăG���f�B�A���ϊ�����
                    /*
                    if (fi.FieldType.Equals(typeof(Int16)) || fi.FieldType.Equals(typeof(Int32)) || fi.FieldType.Equals(typeof(Int64))
                        || fi.FieldType.Equals(typeof(UInt16)) || fi.FieldType.Equals(typeof(UInt32)) || fi.FieldType.Equals(typeof(UInt64)))
                    {
                        fi.SetValue(SessionUtil.hton((fi.GetValue(this).GetType())fi.GetValue(this)));
                    }
                     */
                    if (fi.GetValue(this) is Int16)
                    {
                        fi.SetValue(this, SessionUtil.hton((Int16)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is UInt16)
                    {
                        fi.SetValue(this, SessionUtil.hton((UInt16)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is Int32)
                    {
                        fi.SetValue(this, SessionUtil.hton((Int32)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is UInt32)
                    {
                        fi.SetValue(this, SessionUtil.hton((UInt32)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is Int64)
                    {
                        fi.SetValue(this, SessionUtil.hton((Int64)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is UInt64)
                    {
                        fi.SetValue(this, SessionUtil.hton((UInt64)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is ServerState)
                    {
                        fi.SetValue(this, SessionUtil.hton((uint)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is UInt32[])
                    {
                        UInt32[] vals = (UInt32[])fi.GetValue(this);
                        UInt32[] result = new UInt32[vals.Length];
                        int i = 0;
                        foreach (UInt32 val in vals)
                        {
                            result[i] = SessionUtil.hton(val);
                            i++;
                        }
                        fi.SetValue(this, result);
                    }
                    else
                    {
                        //throw new NotImplementedException("�G���f�B�A���ϊ��ɑΉ����Ă��Ȃ��^�ł��B");
                    }
                }
            }

            internal virtual List<byte> ToByteArray()
            {
                List<byte> bytes = new List<byte>();
                bytes.AddRange(BitConverter.GetBytes(id));
                bytes.AddRange(BitConverter.GetBytes(requestVersion));
                bytes.Add((byte)romVersion);
                bytes.Add((byte)language);
                bytes.AddRange(BitConverter.GetBytes(macAddr1));
                bytes.AddRange(BitConverter.GetBytes(macAddr2));
                bytes.AddRange(BitConverter.GetBytes(reserve1));
                bytes.AddRange(BitConverter.GetBytes(reserve2));
                return bytes;
            }
        }

        /// <summary>
        /// ���ʂ̃��X�|���X�R���e�i
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class ResponseContainer : IResponseContainer, ICloneable
        {
            public uint code;
            //string message;

            public ResponseContainer() { }
            public ResponseContainer(uint code_)
            {
                code = code_;
            }

            public object Clone()
            {
                return DeepCopy(this);
            }

            /// <summary>
            /// �����o���l�b�g���[�N�o�C�g�I�[�_�[����z�X�g�o�C�g�I�[�_�[�ɕϊ�����
            /// </summary>
            public virtual void ntoh()
            {
                FieldInfo[] fis = this.GetType().GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
                foreach (FieldInfo fi in fis)
                {
                    if (fi.GetValue(this) is Int16)
                    {
                        fi.SetValue(this, SessionUtil.ntoh((Int16)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is UInt16)
                    {
                        fi.SetValue(this, SessionUtil.ntoh((UInt16)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is Int32)
                    {
                        fi.SetValue(this, SessionUtil.ntoh((Int32)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is UInt32)
                    {
                        fi.SetValue(this, SessionUtil.ntoh((UInt32)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is Int64)
                    {
                        fi.SetValue(this, SessionUtil.ntoh((Int64)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is UInt64)
                    {
                        fi.SetValue(this, SessionUtil.ntoh((UInt64)fi.GetValue(this)));
                    }
                    else if (fi.GetValue(this) is ServerState)
                    {
                        fi.SetValue(this, SessionUtil.ntoh((uint)fi.GetValue(this)));
                    }
                    else
                    {
                        //throw new NotImplementedException("�G���f�B�A���ϊ��ɑΉ����Ă��Ȃ��^�ł��B");
                    }
                }
            }
        }

        /// <summary>
        /// �T�[�o�̏�ԃZ�b�g���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class AdminSetServerStateRequest : RequestContainer
        {
            ServerState serverState;

            public AdminSetServerStateRequest() { }
            public AdminSetServerStateRequest(int _id, ServerState _serverState)
                : base(_id)
            {
                serverState = _serverState;
            }
        }

        /// <summary>
        /// �T�[�o�̏�ԃZ�b�g���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class AdminSetServerStateResponse : ResponseContainer
        {
        }

        /// <summary>
        /// �T�[�o�̏�Ԏ擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class AdminGetServerStateRequest : RequestContainer
        {
            public AdminGetServerStateRequest() { }
            public AdminGetServerStateRequest(int _id)
                : base(_id)
            {
            }
        }

        /// <summary>
        /// �T�[�o�̏�Ԏ擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class AdminGetServerStateResponse : ResponseContainer
        {
            public ServerState serverState;
        }

        /// <summary>
        /// �ڑ���T�[�o���
        /// </summary>
        public class ServerInfo
        {
            private const string SECRET_KEY = "uLMOGEiiJogofchScpXb000244fd00006015100000005b440e7epokemondpds";
            public static readonly string SECRET_KEY_DEV;

            private const string GAME_NAME = "pokemondpds";
            private const string GAME_NAME_DEV = "ninsake";

            /// <summary>
            /// ���[���A�h���X�擾�ȂǂŒ���HTTP�ڑ����邽�߂̃x�[�X�A�h���X
            /// </summary>
            private const string BASE_URL_GS_RELEASE = "http://gamestats2.gs.nintendowifi.net/" + GAME_NAME + "/";
            private const string BASE_URL_GS_DEBUG = "http://sdkdev.gamespy.com/games/" + GAME_NAME + "/web/";
            private const string BASE_URL_GS_DEV_RELEASE = "http://gamestats2.gs.nintendowifi.net/" + GAME_NAME_DEV + "/";
            private const string BASE_URL_GS_DEV_DEBUG = "http://sdkdev.gamespy.com/games/" + GAME_NAME_DEV + "/web/";

            /// <summary>
            /// �ڑ���̎��
            /// </summary>
            public enum KIND
            {
                DEBUG,
                RELEASE,
                DEV_DEBUG,
                DEV_RELEASE
            }

            private static List<ServerInfo> list;
            private KIND kind;
            public KIND Kind
            {
                get { return kind; }
            }

            private EncSession.DWC_ENC_SERVER server;
            public EncSession.DWC_ENC_SERVER Server
            {
                get { return server; }
            }

            private string key;
            public string Key
            {
                get { return key; }
            }

            private string baseUrl;
            public string BaseUrl
            {
                get { return baseUrl; }
            }

            private NetworkCredential credential;
            public NetworkCredential Credential
            {
                get { return credential; }
            }

            private string name;
            public string Name
            {
                get { return name; }
                set { name = value; }
            }

            static ServerInfo()
            {
                NetworkCredential credential = new NetworkCredential("pokemonco", "pco88437");
                NetworkCredential devCredential = null;
                // �C�V���J���p�T�[�o�̏���ǂݍ���
                try
                {
                    using (StreamReader r = new StreamReader(@"secretkey_dev.txt"))
                    {
                        string[] separator = { System.Environment.NewLine };
                        string text = r.ReadToEnd();
                        string[] lines = text.Split(separator, System.StringSplitOptions.RemoveEmptyEntries);
                        SECRET_KEY_DEV = lines[0];
                        devCredential = new NetworkCredential(lines[1], lines[2]);
                    }
                }
                catch (Exception)
                {
                    SECRET_KEY_DEV = null;
                    devCredential = null;
                }

                list = new List<ServerInfo>();
                list.Add(new ServerInfo(ServerInfo.KIND.DEBUG, EncSession.DWC_ENC_SERVER.DEBUG, SECRET_KEY,
                    BASE_URL_GS_DEBUG, credential, "�f�o�b�O�T�[�o"));
                list.Add(new ServerInfo(ServerInfo.KIND.RELEASE, EncSession.DWC_ENC_SERVER.RELEASE, SECRET_KEY,
                    BASE_URL_GS_RELEASE, credential, "�����[�X�T�[�o"));
                list.Add(new ServerInfo(ServerInfo.KIND.DEV_DEBUG, EncSession.DWC_ENC_SERVER.DEBUG, SECRET_KEY_DEV,
                    BASE_URL_GS_DEV_DEBUG, devCredential, "�J���p�f�o�b�O�T�[�o"));
                list.Add(new ServerInfo(ServerInfo.KIND.DEV_RELEASE, EncSession.DWC_ENC_SERVER.RELEASE, SECRET_KEY_DEV,
                    BASE_URL_GS_DEV_RELEASE, devCredential, "�J���p�����[�X�T�[�o"));
            }

            private ServerInfo(KIND _kind, EncSession.DWC_ENC_SERVER _server, string _key, string _baseUrl, NetworkCredential _credential, string _name)
            {
                kind = _kind;
                server = _server;
                key = _key;
                baseUrl = _baseUrl;
                credential = _credential;
                name = _name;
            }

            /// <summary>
            /// �w�肵����ނ̃T�[�o���𓾂�
            /// </summary>
            /// <param name="kind">�擾�������T�[�o�̎��</param>
            /// <returns>�T�[�o���</returns>
            public static ServerInfo Select(KIND kind)
            {
                return list.Find(delegate(ServerInfo info)
                {
                    return info.kind == kind;
                });
            }
        }

        // �C���X�^���X�ϐ���`
        private ServerInfo currentServer;
        internal ServerInfo CurrentServer
        {
            get { return currentServer; }
        }

        private EncSession encSession;
        public string ProxyServer
        {
            get { return encSession.ProxyServer; }
            set { encSession.ProxyServer = value; }
        }

        /// <summary>
        /// Enc�C���^�t�F�[�X������������
        /// </summary>
        protected RequestInterface(ServerInfo.KIND kind, string _proxyServer)
        {
            currentServer = ServerInfo.Select(kind);
            encSession = new EncSession(currentServer.Server, currentServer.Key, _proxyServer);
        }

        /// <summary>
        /// ���N�G�X�g�p�C���^�t�F�[�X���擾����B���ڗp
        /// </summary>
        /// <param name="kind">�ŏ��ɐڑ�����T�[�o</param>
        /// <returns>RequestInterface�I�u�W�F�N�g</returns>
        public static RequestInterface CreateInstance(ServerInfo.KIND kind, string _proxyServer)
        {
            lock (typeof(RequestInterface))
            {
                if (s_instance == null)
                {
                    s_instance = new RequestInterface(kind, _proxyServer);
                }
                else
                {
                    throw new Exception("���łɃI�u�W�F�N�g�͐�������Ă���̂Őڑ���̎w��͖����ł��B");
                }
            }
            return s_instance;
        }

        /// <summary>
        /// �T�[�o��؂�ւ���
        /// </summary>
        /// <param name="server">�ڑ���T�[�o</param>
        public void ChangeServer(ServerInfo.KIND kind)
        {
            currentServer = ServerInfo.Select(kind);
            encSession.ChangeServer(currentServer.Server, currentServer.Key);
        }

        /// <summary>
        /// ���݂̃T�[�o�ɍĐڑ�����
        /// </summary>
        public void Reconnect()
        {
            encSession.ChangeServer(currentServer.Server, currentServer.Key);
        }

        /// <summary>
        /// �ėp�I�ȃ��N�G�X�g���M���\�b�h
        /// </summary>
        /// <typeparam name="T_REQ">���N�G�X�g�f�[�^�N���X</typeparam>
        /// <typeparam name="T_RES">���X�|���X�f�[�^�N���X</typeparam>
        /// <param name="url">�ڑ���p�X</param>
        /// <param name="request">���N�G�X�g�f�[�^</param>
        /// <param name="message">���X�|���X���b�Z�[�W</param>
        /// <returns>���X�|���X�f�[�^</returns>
        internal T_RES GenericSession<T_REQ, T_RES>(string url, T_REQ request, out string message)
            where T_REQ : RequestContainer
            where T_RES : ResponseContainer, new()
        {
            IntPtr ptr;
            uint size;
            message = "";

            T_RES res = encSession.GenericSession<T_REQ, T_RES>(url, request.id, request, out ptr, out size);
            if (res != default(T_RES))
            {
                // ����I��
                return res;
            }
            else // ���X�|���X�T�C�Y���Ⴄ�Ƃ�
            {
                res = new T_RES();
                // ���X�|���X�R�[�h�����o��
                if (size < Marshal.SizeOf(res.code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B\n" + size + " != " + Marshal.SizeOf(typeof(T_RES)));
                }

                res.code = (uint)Marshal.ReadInt32(ptr);
                //res.ntoh();

                if (res.code == 0)
                {
                    if (size < Marshal.SizeOf(typeof(T_RES)))
                    {
                        // ���X�|���X�T�C�Y���������̂Ƀ��X�|���X�R�[�h0�̂Ƃ�
                        throw new Exception("���X�|���X�T�C�Y���s���ł��B\n" + size + " != " + Marshal.SizeOf(typeof(T_RES)));
                    }
                }
                else
                {
                    // �G���[���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(res.code)), 256);
                }

                // ���X�|���X�T�C�Y���傫���Ƃ��A�G���[�R�[�h���Z�b�g����Ă���Ƃ��͗�O�𓊂�������I��
                return res;
            }
        }
    }
}
