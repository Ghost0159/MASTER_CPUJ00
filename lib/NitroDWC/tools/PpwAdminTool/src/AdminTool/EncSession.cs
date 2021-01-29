/*---------------------------------------------------------------------------*
  Project:  AdminTool

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
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Threading;

namespace NWC
{
    // ���N�G�X�g�p�C���^�t�F�[�X
    interface IRequestContainer
    {
        void hton();
    }

    // ���X�|���X�p�C���^�t�F�[�X
    interface IResponseContainer
    {
        void ntoh();
    }

    // �G���f�B�A���ϊ��Ȃǂ̃��[�e�B���e�B
    class SessionUtil
    {
        // ���ݎ��s���̃V�X�e�������g���G���f�B�A���̏ꍇ�Ƀo�C�g��𔽓]����
        public static byte[] ConvertEndian(byte[] bin)
        {
            if (BitConverter.IsLittleEndian)
            {
                Array.Reverse(bin);
                return bin;
            }
            else
            {
                return bin;
            }
        }

        public static short ntoh(short a)
        {
            byte[] b = BitConverter.GetBytes(a);
            ConvertEndian(b);
            return BitConverter.ToInt16(b, 0);
        }
        public static ushort ntoh(ushort a)
        {
            byte[] b = BitConverter.GetBytes(a);
            ConvertEndian(b);
            return BitConverter.ToUInt16(b, 0);
        }
        public static int ntoh(int a)
        {
            byte[] b = BitConverter.GetBytes(a);
            ConvertEndian(b);
            return BitConverter.ToInt32(b, 0);
        }
        public static uint ntoh(uint a)
        {
            byte[] b = BitConverter.GetBytes(a);
            ConvertEndian(b);
            return BitConverter.ToUInt32(b, 0);
        }
        public static long ntoh(long a)
        {
            byte[] b = BitConverter.GetBytes(a);
            ConvertEndian(b);
            return BitConverter.ToInt64(b, 0);
        }
        public static ulong ntoh(ulong a)
        {
            byte[] b = BitConverter.GetBytes(a);
            ConvertEndian(b);
            return BitConverter.ToUInt64(b, 0);
        }
        public static short hton(short a)
        {
            return ntoh(a);
        }
        public static ushort hton(ushort a)
        {
            return ntoh(a);
        }
        public static int hton(int a)
        {
            return ntoh(a);
        }
        public static uint hton(uint a)
        {
            return ntoh(a);
        }
        public static long hton(long a)
        {
            return ntoh(a);
        }
        public static ulong hton(ulong a)
        {
            return ntoh(a);
        }
    }

    /// <summary>
    /// �Í���DLL���b�p�[
    /// </summary>
    public class EncSession
    {

        enum DWC_END_SESSION_STATE
        {
            CANCELED,		//!< �L�����Z��
            ERROR,		    //!< �G���[����
            INITIAL,		//!< �������
            INITIALIZED,	//!< ��������
            REQUEST,		//!< ���N�G�X�g���s
            GETTING_TOKEN,  //!< �g�[�N���擾��
            GOT_TOKEN,	    //!< �g�[�N���擾����
            SENDING_DATA,	//!< �f�[�^���M��
            COMPLETED		//!< ����
        }

        public enum DWC_ENC_SERVER
        {
            RELEASE,             //!< ���i�p�T�[�o�[�ɐڑ����܂�
            DEBUG,               //!< �J���p�T�[�o�[�ɐڑ����܂�
            TEST                 //!< (�g�p���Ȃ��ł�������)
        }

        private DWC_ENC_SERVER currentServer;
        private string key;
        private string proxyServer;
        public string ProxyServer
        {
            get { return proxyServer; }
            set
            {
                if (value != proxyServer)
                {
                    proxyServer = value;
                    ChangeServer(currentServer);
                }
                else
                {
                    proxyServer = value;
                }
            }
        }


        /// <summary>
        /// ������DWC_Init�֐��ADWCi_EncSessionInitialize�֐����Ă΂�܂��B
        /// </summary>
        [DllImport("dwcenc.dll")]
        protected static extern void DWC_EncSessionInitialize( int server, string initdata );

        /// <summary>
        /// ������ghttpSetProxy�֐����Ă΂�܂��B�Ǘ��c�[����pAPI�B
        /// </summary>
        [DllImport("dwcenc.dll")]
        protected static extern bool DWC_EncSetProxy(string proxyServer);

        /// <summary>
        /// ������DWC_ClearError�֐��ADWCi_EncSessionShutdown�֐��ADWC_Shutdown�֐����Ă΂�܂��B
        /// </summary>
        [DllImport("dwcenc.dll")]
        protected static extern void DWC_EncSessionShutdown();

        /// <summary>
        /// ������DWCi_EncSessionGetAsync�֐����Ă΂�܂��B
        /// </summary>
        [DllImport("dwcenc.dll")]
        protected static extern int DWC_EncSessionGetAsync(string url, int pid, IntPtr data, uint size, int cb);

        /// <summary>
        /// ������DWCi_EncSessionProcess�֐����Ă΂�܂��B
        /// </summary>
        /// <returns></returns>
        [DllImport("dwcenc.dll")]
        protected static extern int DWC_EncSessionProcess();

        /// <summary>
        /// ������DWCi_EncSessionGetResponse�֐����Ă΂�܂��B
        /// </summary>
        [DllImport("dwcenc.dll")]
        protected static extern IntPtr DWC_EncSessionGetResponse(ref uint size);

        /// <summary>
        /// ������DWCi_EncSessionCancel�֐����Ă΂�܂��B
        /// </summary>
        [DllImport("dwcenc.dll")]
        protected static extern void DWC_EncSessionCancel();

        protected static bool DWC_EncSessionInitializeWithProxy(int server, string initdata, string proxyServer)
        {
            DWC_EncSessionInitialize(server, initdata);
            if (proxyServer != null && proxyServer != "")
            {
                return EncSession.DWC_EncSetProxy(proxyServer);
            }
            return true;
        }

        /// <summary>
        /// GameSpy�o�b�N�G���h�Ƃ̒ʐM������������
        /// </summary>
        /// <param name="server">�ڑ���T�[�o</param>
        /// <param name="_key">�Q�[�����ƂɈقȂ�V�[�N���b�g�L�[</param>
        internal EncSession(DWC_ENC_SERVER server, string _key, string _proxyServer)
        {
            currentServer = server;
            key = _key;
            proxyServer = _proxyServer;
            EncSession.DWC_EncSessionInitializeWithProxy((int)server, key, proxyServer);
        }

        ~EncSession()
        {
            EncSession.DWC_EncSessionShutdown();
        }

        /// <summary>
        /// �ڑ���T�[�o��؂�ւ���B�O��Ɠ����T�[�o���w�肵���Ƃ��͐ڑ��������B
        /// </summary>
        /// <param name="server">�ڑ���T�[�o</param>
        /// <returns>�v���L�V�T�[�o�t�H�[�}�b�g����������</returns>
        internal bool ChangeServer(DWC_ENC_SERVER server)
        {
            currentServer = server;
            EncSession.DWC_EncSessionShutdown();
            return EncSession.DWC_EncSessionInitializeWithProxy((int)server, key, proxyServer);
        }

        /// <summary>
        /// �ڑ���T�[�o�A�f�B���N�g����؂�ւ���B�O��Ɠ����T�[�o���w�肵���Ƃ��͐ڑ��������B
        /// </summary>
        /// <param name="server">�ڑ���T�[�o</param>
        /// <param name="_key">�V�[�N���b�g�L�[</param>
        /// <returns>�v���L�V�T�[�o�t�H�[�}�b�g����������</returns>
        internal bool ChangeServer(DWC_ENC_SERVER server, string _key)
        {
            currentServer = server;
            key = _key;
            EncSession.DWC_EncSessionShutdown();
            return EncSession.DWC_EncSessionInitializeWithProxy((int)server, key, proxyServer);
        }

        protected bool WaitForResponse()
        {
            DWC_END_SESSION_STATE state;

            while(true)
            {
                state = (DWC_END_SESSION_STATE)EncSession.DWC_EncSessionProcess();

                switch (state)
                {
                    case DWC_END_SESSION_STATE.COMPLETED:		//!< ����
                        return true;
                    case DWC_END_SESSION_STATE.CANCELED:		//!< �L�����Z��
                    case DWC_END_SESSION_STATE.ERROR:			//!< �G���[����
                        return false;
                    case DWC_END_SESSION_STATE.INITIAL:			//!< �������
                    case DWC_END_SESSION_STATE.INITIALIZED:		//!< ��������
                    case DWC_END_SESSION_STATE.REQUEST:			//!< ���N�G�X�g���s
                    case DWC_END_SESSION_STATE.GETTING_TOKEN:	//!< �g�[�N���擾��
                    case DWC_END_SESSION_STATE.GOT_TOKEN:		//!< �g�[�N���擾����
                    case DWC_END_SESSION_STATE.SENDING_DATA:	//!< �f�[�^���M��
                        break;
                }
                Thread.Sleep(10);
            }
        }

        /// <summary>
        /// �ϒ��̃��N�G�X�g�𑗐M���A���X�|���X�̃|�C���^��Ԃ�
        /// </summary>
        /// <param name="url">�ڑ���p�X</param>
        /// <param name="id">ProfileID</param>
        /// <param name="inPtr">���N�G�X�g���i�[����byte��</param>
        /// <param name="outPtr">���X�|���X���w�����|�C���^</param>
        /// <param name="outSize">���X�|���X�T�C�Y</param>
        internal void Session(string url, int id, byte[] bytes, out IntPtr outPtr, out uint outSize)
        {
            uint realSize = 0;
            int inSize = 0;
            IntPtr inPtr = new IntPtr();

            if (bytes == null || bytes.Length == 0)
            {
                //throw new Exception("���̓f�[�^����ł��B");
            }
            else
            {
                inSize = Marshal.SizeOf(bytes[0]) * bytes.Length;
                inPtr = Marshal.AllocHGlobal(inSize);
            }

            try
            {
                if (bytes.Length != 0)
                {
                    Marshal.Copy(bytes, 0, inPtr, bytes.Length);
                }

                lock (this)
                {
                    EncSession.DWC_EncSessionGetAsync(url, id, inPtr, (uint)inSize, 0);
                    if (!WaitForResponse())
                    {
                        throw new Exception("HTTP�G���[");
                    }
                    outPtr = EncSession.DWC_EncSessionGetResponse(ref realSize);
                }
            }
            finally
            {
                Marshal.FreeHGlobal(inPtr);
            }

            outSize = realSize;
            //MessageBox.Show("PTR: " + resultPtr + " size : " + realSize + " send size: " + Marshal.SizeOf(request));
        }

        /// <summary>
        /// �Œ蒷�̃��N�G�X�g�𑗐M���A���X�|���X�̃|�C���^��Ԃ�
        /// </summary>
        /// <param name="url">�ڑ���p�X</param>
        /// <param name="id">ProfileID</param>
        /// <param name="request">���N�G�X�g�f�[�^</param>
        /// <param name="outPtr">���X�|���X���w�����|�C���^</param>
        /// <param name="outSize">���X�|���X�T�C�Y</param>
        internal void Session(string url, int id, object request, out IntPtr outPtr, out uint outSize)
        {
            uint realSize = 0;
            IntPtr ptr = Marshal.AllocCoTaskMem(Marshal.SizeOf(request));
            Marshal.StructureToPtr(request, ptr, false);

            try
            {
                lock (this)
                {
                    EncSession.DWC_EncSessionGetAsync(url, id, ptr, (uint)Marshal.SizeOf(request), 0);
                    if (!WaitForResponse())
                    {
                        throw new Exception("HTTP�G���[");
                    }
                    outPtr = EncSession.DWC_EncSessionGetResponse(ref realSize);
                }
            }
            finally
            {
                Marshal.FreeCoTaskMem(ptr);
            }
            outSize = realSize;
            //MessageBox.Show("PTR: " + resultPtr + " size : " + realSize + " send size: " + Marshal.SizeOf(request));
        }

        /// <summary>
        /// �Œ蒷�̃��N�G�X�g�𑗐M���A�Œ蒷�̃��X�|���X��Ԃ�
        /// </summary>
        /// <typeparam name="T_REQ">���N�G�X�g�f�[�^�N���X</typeparam>
        /// <typeparam name="T_RES">���X�|���X�f�[�^�N���X</typeparam>
        /// <param name="url">�ڑ���p�X</param>
        /// <param name="pid">ProfileID</param>
        /// <param name="request">���N�G�X�g�f�[�^</param>
        /// <param name="outPtr">���X�|���X���w�����|�C���^</param>
        /// <param name="outSize">���X�|���X�T�C�Y</param>
        /// <returns>���X�|���X�f�[�^�B���X�|���X�T�C�Y���K��ƈقȂ����ꍇ��default�l���Ԃ�B</returns>
        internal T_RES GenericSession<T_REQ, T_RES>(string url, int pid, T_REQ request, out IntPtr outPtr, out uint outSize)
            where T_REQ : IRequestContainer
            where T_RES : IResponseContainer
        {
            T_RES response;

            //request.hton();

            Session(url, pid, request, out outPtr, out outSize);

            if (outSize != Marshal.SizeOf(typeof(T_RES)))
            {
                return default(T_RES);
                //throw new Exception("���X�|���X�T�C�Y���s���ł��B\n" + outSize + " < " + Marshal.SizeOf(typeof(T_RES)));
            }

            response = (T_RES)Marshal.PtrToStructure(outPtr, typeof(T_RES));
            response.ntoh();

            return response;
        }
    }
}
