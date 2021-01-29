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
using System.Collections;

namespace AdminTool
{
    /// <summary>
    /// ���ʃC���^�t�F�[�X�N���X
    /// </summary>
    public partial class RequestInterface
    {
        /// <summary>
        /// �ڑ���p�X
        /// </summary>
        // �X�P�W���[��
        public const string URL_ADMIN_SET_LOBBY_SCHEDULE = "/web/enc/lobby/adminSetSchedule.asp";
        public const string URL_ADMIN_APPEND_LOBBY_SCHEDULE = "/web/enc/lobby/adminAppendSchedule.asp";
        public const string URL_ADMIN_GET_LOBBY_SCHEDULE = "/web/enc/lobby/adminGetSchedule.asp";
        public const string URL_GET_LOBBY_SCHEDULE = "/web/enc/lobby/getSchedule.asp";

        // VIP
        public const string URL_ADMIN_SET_LOBBY_VIP = "/web/enc/lobby/adminSetVip.asp";
        public const string URL_ADMIN_GET_LOBBY_VIP = "/web/enc/lobby/adminGetVip.asp";
        public const string URL_GET_LOBBY_VIP = "/web/enc/lobby/getVip.asp";
        
        // �s���`�F�b�N
        public const string URL_ADMIN_CHECK_PROFILE = "/web/enc/lobby/checkProfile.asp";
        
        // �A���P�[�g
        public const string URL_GET_QUESTIONNAIRE = "/web/enc/lobby/getQuestionnaire.asp";
        public const string URL_SUBMIT_QUESTIONNAIRE = "/web/enc/lobby/submitQuestionnaire.asp";
        public const string URL_ADMIN_GET_SPECIAL_WEEK = "/web/enc/lobby/adminGetSpecialWeek.asp";
        public const string URL_ADMIN_SET_SPECIAL_WEEK = "/web/enc/lobby/adminSetSpecialWeek.asp";
        public const string URL_ADMIN_GET_FREE_QUESTION = "/web/enc/lobby/adminGetFreeQuestion.asp";
        public const string URL_ADMIN_SET_FREE_QUESTION = "/web/enc/lobby/adminSetFreeQuestion.asp";
        public const string URL_ADMIN_UPDATE_QUESTIONNAIRE = "/web/enc/lobby/adminUpdateQuestionnaire.asp";
        public const string URL_ADMIN_SET_QUESTION_SERIAL_NO = "/web/enc/lobby/adminSetQuestionSerialNo.asp";
        public const string URL_ADMIN_SET_NEXT_QUESTION_NO = "/web/enc/lobby/adminSetNextQuestionNo.asp";
        public const string URL_ADMIN_SET_QUESTION_NO = "/web/enc/lobby/adminSetQuestionNo.asp";
        public const string URL_ADMIN_SET_SPECIAL_QUESTION_THRESHOLD = "/web/enc/lobby/adminSetSpecialQuestionThreshold.asp";
        public const string URL_ADMIN_SET_DEFAULT_SUMMARIZE_FLAG = "/web/enc/lobby/adminSetDefaultSummarizeFlag.asp";
        public const string URL_ADMIN_INITIALIZE_QUESTIONNAIRE = "/web/enc/lobby/adminInitializeQuestionnaire.asp";

        private LobbySchedules currentSchedules = new LobbySchedules();
        public LobbySchedules CurrentSchedules
        {
            get { return currentSchedules; }
            set
            {
                currentSchedules = value;
                currentSchedules.Sort();
            }
        }


        /// <summary>
        /// ���r�[�X�P�W���[���擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyGetScheduleRequest : RequestContainer
        {
            public LobbyGetScheduleRequest() { }
            public LobbyGetScheduleRequest(int _id)
                : base(_id)
            {
            }
        }

        public enum PPW_LOBBY_TIME_EVENT : int
        {
            LOCK,              ///< �����̃��b�N�B
            NEON_A0,           ///< �����l�I���~��0�B
            NEON_A1,           ///< �����l�I���~��1�B
            NEON_A2,           ///< �����l�I���~��2�B
            NEON_A3,           ///< �����l�I���~��3�B
            NEON_A4,           ///< �����l�I���~��4�B
            NEON_A5,           ///< �����l�I���~��5�B
            NEON_B0,           ///< ���l�I���~��0�B
            NEON_B1,           ///< ���l�I���~��1�B
            NEON_B2,           ///< ���l�I���~��2�B
            NEON_B3,           ///< ���l�I���~��3�B
            NEON_C0,           ///< ���j�������g�~��0�B
            NEON_C1,           ///< ���j�������g�~��1�B
            NEON_C2,           ///< ���j�������g�~��2�B
            NEON_C3,           ///< ���j�������g�~��3�B
            MINIGAME_END,      ///< �~�j�Q�[���I���B
            FIRE_WORKS_START,  ///< �t�@�C�A�[���[�N�X�J�n�B
            FIRE_WORKS_END,    ///< �t�@�C�A�[���[�N�X�I���B
            PARADE,            ///< �p���[�h�B
            CLOSE              ///< �I���B
        }

        public enum PPW_LOBBY_SEASON : int
        {
            NORMAL,
            SPRING,
            SUMMER,
            AUTUMN,
            WINTER
        }

        /// <summary>
        /// ���r�[�X�P�W���[�����R�[�h
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        [Serializable()]
        public class LobbyScheduleRecord : IComparable<LobbyScheduleRecord>
        {
            ///< �C�x���g���N�����鎞��(���C���`�����l�������b�N�����Ƃ�����̎���)�B
            private int time;
            public int Time
            {
                get { return time; }
                set { time = value; }
            }
                           
            ///< �N������C�x���g�B
            private PPW_LOBBY_TIME_EVENT timeEvent;
	        public PPW_LOBBY_TIME_EVENT TimeEvent
	        {
		        get { return timeEvent;}
                set
                {
                    if (!Enum.IsDefined(typeof(PPW_LOBBY_TIME_EVENT), value) || PPW_LOBBY_TIME_EVENT.LOCK == value)
                    {
                        throw new InvalidDataException("" + (int)value + " �̓C�x���g�ɕϊ��ł��܂���B");
                    }
                    timeEvent = value;
                }
	        }


            public LobbyScheduleRecord()
            {
                time = 0;
                timeEvent = PPW_LOBBY_TIME_EVENT.NEON_A0;
            }

            internal ListViewItem[] ToListViewItems()
            {
                List<ListViewItem> list = new List<ListViewItem>();
                list.Add(new ListViewItem(new String[] { "- time", time.ToString() }));
                list.Add(new ListViewItem(new String[] { "  event", timeEvent.ToString() }));
                return list.ToArray();
            }

            internal List<byte> ToByteArray()
            {
                List<byte> bytes = new List<byte>();
                bytes.AddRange(BitConverter.GetBytes(time));
                bytes.AddRange(BitConverter.GetBytes((int)timeEvent));
                return bytes;
            }

            override public string ToString()
            {
                return "- time: " + time.ToString() + "\n  event: " + timeEvent.ToString();
            }

            #region IComparable �����o

            public int CompareTo(LobbyScheduleRecord rhs)
            {
                return (time - rhs.time) * 0xffff + ((int)timeEvent - (int)rhs.timeEvent);
            }

            #endregion
        }

        /// <summary>
        /// ���r�[�X�P�W���[���擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyGetScheduleResponse : ResponseContainer
        {
            public uint lockTime;                           ///< ���C���`�����l�����I�[�v�����Ă��烍�b�N����܂ł̎���(�b)�B
            public uint random;                             ///< �T�[�o�Ő��������32bit�͈͂̃����_���l�B
            public uint roomFlag;                           ///< PPW_LOBBY_ROOM_FLAG�ɑΉ�����e��bit�t���O
            public byte roomType;                           ///< �����̎�ށB
            public byte season;                             ///< �G�ߔԍ��B
            public ushort scheduleRecordNum;                ///< �X�P�W���[�����R�[�h�̐��B
            public List<LobbyScheduleRecord> scheduleRecords = new List<LobbyScheduleRecord>(); ///< �X�P�W���[�����R�[�h

            public LobbyGetScheduleResponse() { }

            internal ListViewItem[] ToListViewItems()
            {
                List<ListViewItem> list = new List<ListViewItem>();
                list.Add(new ListViewItem(new String[] { "lockTime", lockTime.ToString() }));
                list.Add(new ListViewItem(new String[] { "random", random.ToString() }));
                list.Add(new ListViewItem(new String[] { "roomFlag", roomFlag.ToString() }));
                list.Add(new ListViewItem(new String[] { "roomType", roomType.ToString() }));
                list.Add(new ListViewItem(new String[] { "season", season.ToString() }));
                list.Add(new ListViewItem(new String[] { "scheduleRecordNum", scheduleRecordNum.ToString() }));
                foreach (LobbyScheduleRecord scheduleRecord in scheduleRecords)
                {
                    list.AddRange(new List<ListViewItem>(scheduleRecord.ToListViewItems()));
                }
                return list.ToArray();
            }
        }

        /// <summary>
        /// ���r�[�X�P�W���[���ݒ�擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetScheduleRequest : RequestContainer
        {
            public LobbyAdminGetScheduleRequest() { }
            public LobbyAdminGetScheduleRequest(int _id)
                : base(_id)
            {
            }
        }

        /// <summary>
        /// ���r�[�X�P�W���[���ݒ�x�[�X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        [Serializable()]
        public class LobbyAdminScheduleBase
        {
            public byte startMonth;
            public byte startDay;
            public ushort startYear;
            public uint lockTime;                           ///< ���C���`�����l�����I�[�v�����Ă��烍�b�N����܂ł̎���(�b)�B
            public uint roomFlag;                           ///< PPW_LOBBY_ROOM_FLAG�ɑΉ�����e��bit�t���O
            [System.Xml.Serialization.XmlElement("roomRateFire")]   // XML�v�f���ύX
            public byte roomRate1;                        ///< �����̎�ށB
            [System.Xml.Serialization.XmlElement("roomRateWater")]
            public byte roomRate2;                        ///< �����̎�ށB
            [System.Xml.Serialization.XmlElement("roomRateThunder")]
            public byte roomRate3;                        ///< �����̎�ށB
            [System.Xml.Serialization.XmlElement("roomRateGrass")]
            public byte roomRate4;                        ///< �����̎�ށB
            [System.Xml.Serialization.XmlElement("roomRateSecret")]
            public byte roomRate5;                        ///< �����̎�ށB
            public byte season;                             ///< �G�ߔԍ��B
            [System.Xml.Serialization.XmlIgnoreAttribute]   // XML�V���A���C�Y���Ȃ�
            public ushort scheduleRecordNum;                ///< �X�P�W���[�����R�[�h�̐��B

            public LobbyAdminScheduleBase()
            {
                DateTime now = DateTime.Now;
                startMonth = (byte)now.Month;
                startDay = (byte)now.Day;
                startYear = (ushort)now.Year;
                roomRate1 = 20;
                roomRate2 = 20;
                roomRate3 = 20;
                roomRate4 = 20;
                roomRate5 = 20;
            }

            internal List<byte> ToByteArray()
            {
                List<byte> bytes = new List<byte>();
                bytes.Add((byte)(startMonth - 1));  // �T�[�o�ł�0-11��v��
                bytes.Add(startDay);
                bytes.AddRange(BitConverter.GetBytes(startYear));
                bytes.AddRange(BitConverter.GetBytes(lockTime));
                bytes.AddRange(BitConverter.GetBytes(roomFlag));
                bytes.Add(roomRate1);
                bytes.Add(roomRate2);
                bytes.Add(roomRate3);
                bytes.Add(roomRate4);
                bytes.Add(roomRate5);
                bytes.Add(season);
                bytes.AddRange(BitConverter.GetBytes(scheduleRecordNum));
                return bytes;
            }

            internal List<string> ToStringArray()
            {
                List<string> list = new List<string>();
                list.Add(startYear.ToString());
                list.Add(startMonth.ToString());
                list.Add(startDay.ToString());
                list.Add(lockTime.ToString());
                list.Add(roomFlag.ToString());
                list.Add(roomRate1.ToString());
                list.Add(roomRate2.ToString());
                list.Add(roomRate3.ToString());
                list.Add(roomRate4.ToString());
                list.Add(roomRate5.ToString());
                list.Add(season.ToString());
                // ���R�[�h���͎��ۂ�List�̐����g��
                //list.Add(scheduleRecordNum.ToString());
                return list;
            }

            internal bool ChangeData(string tag, string data)
            {
                try
                {
                    Type t = this.GetType();
                    FieldInfo info = t.GetField(tag);
                    // data���Z�b�g�������^�ɃL���X�g���ăZ�b�g
                    info.SetValue(this, Convert.ChangeType(Int32.Parse(data), info.FieldType));
                }
                catch (Exception e)
                {
                    MessageBox.Show(e.Message);
                    return false;
                }
                return true;
            }

            internal bool IsConsistent()
            {
                return roomRate1 + roomRate2 + roomRate3 + roomRate4 + roomRate5 == 100 &&
                    startYear >= 2000 && startYear <= 2200 && startMonth >= 1 && startMonth <= 12 && startDay >= 1 && startDay <= 31;
            }
        }


        /// <summary>
        /// ���r�[�X�P�W���[���ݒ�擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetScheduleResponse : ResponseContainer
        {
            public LobbyAdminScheduleBase scheduleBase;
            public List<LobbyScheduleRecord> scheduleRecords = new List<LobbyScheduleRecord>(); ///< �X�P�W���[�����R�[�h

            public LobbyAdminGetScheduleResponse() { }
        }

        /// <summary>
        /// ���r�[�X�P�W���[���ݒ�Z�b�g���N�G�X�g�̈ꕔ�A�������̂��߂܂Ƃ߂đ���̂Ń��N�G�X�g�I�u�W�F�N�g����͌p�����Ȃ�
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        [System.Xml.Serialization.XmlRoot("LobbySchedule")]
        [Serializable()]
        public class LobbyAdminSetScheduleRequestPart : ICloneable, IComparable<LobbyAdminSetScheduleRequestPart>
        {
            public const int OBJECT_SIZE = 340;
            public LobbyAdminScheduleBase scheduleBase = new LobbyAdminScheduleBase();

            [System.Xml.Serialization.XmlElement("LobbyScheduleRecord", Type = typeof(LobbyScheduleRecord))]
            public List<LobbyScheduleRecord> scheduleRecords = new List<LobbyScheduleRecord>(); ///< �X�P�W���[�����R�[�h

            public DateTime StartDate
            {
                get { return new DateTime(scheduleBase.startYear, scheduleBase.startMonth, scheduleBase.startDay); }
                set
                {
                    scheduleBase.startYear = (ushort)value.Year;
                    scheduleBase.startMonth = (byte)value.Month;
                    scheduleBase.startDay = (byte)value.Day;
                }
            }

            public uint LockTime
            {
                get { return scheduleBase.lockTime; }
                set { scheduleBase.lockTime = value; }
            }

            public uint RoomFlag
            {
                get { return scheduleBase.roomFlag; }
                set { scheduleBase.roomFlag = value; }
            }

            public byte RoomRate1
            {
                get { return scheduleBase.roomRate1; }
                set { scheduleBase.roomRate1 = value; }
            }

            public byte RoomRate2
            {
                get { return scheduleBase.roomRate2; }
                set { scheduleBase.roomRate2 = value; }
            }

            public byte RoomRate3
            {
                get { return scheduleBase.roomRate3; }
                set { scheduleBase.roomRate3 = value; }
            }

            public byte RoomRate4
            {
                get { return scheduleBase.roomRate4; }
                set { scheduleBase.roomRate4 = value; }
            }

            public byte RoomRate5
            {
                get { return scheduleBase.roomRate5; }
                set { scheduleBase.roomRate5 = value; }
            }

            public byte Season
            {
                get { return scheduleBase.season; }
                set { scheduleBase.season = value; }
            }

            public PPW_LOBBY_SEASON EnumSeason
            {
                get { return (PPW_LOBBY_SEASON)scheduleBase.season; }
                set
                {
                    if (!Enum.IsDefined(typeof(PPW_LOBBY_SEASON), value))
                    {
                        throw new InvalidDataException("" + value + " �͋G�߂ɕϊ��ł��܂���B");
                    }
                    scheduleBase.season = (byte)value;
                }
            }


            public ushort ScheduleRecordNum
            {
                get { return (ushort)scheduleRecords.Count; }
                set { }
            }

            // scheduleRecords���R�s�[�p(TSV)�ɃV���A���C�Y
            [System.Xml.Serialization.XmlIgnoreAttribute]   // XML�V���A���C�Y���Ȃ�
            public string ScheduleRecordsText
            {
                get
                {
                    System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(scheduleRecords.GetType());
                    using (System.IO.MemoryStream ms = new System.IO.MemoryStream())
                    {
                        serializer.Serialize(ms, scheduleRecords);

                        // �����߂�
                        ms.Seek(0, SeekOrigin.Begin);

                        using (StreamReader reader = new StreamReader(ms))
                        {
                            // Just read to the end.
                            string result;
                            result = reader.ReadToEnd();
                            result = result.Replace("\r", "");
                            result = result.Replace("\n", "");
                            return result;
                        }
                    }
                }
                set
                {
                    System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(scheduleRecords.GetType());
                    using (System.IO.MemoryStream ms = new System.IO.MemoryStream(Encoding.Unicode.GetBytes(value)))
                    {
                        this.scheduleRecords = (List<LobbyScheduleRecord>)serializer.Deserialize(ms);
                    }
                }
            }

            public LobbyAdminSetScheduleRequestPart()
            {
            }

            public void StoreXML(string filePath)
            {
                System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(LobbyAdminSetScheduleRequestPart));
                using (System.IO.FileStream fs = new System.IO.FileStream(filePath, System.IO.FileMode.Create))
                {
                    serializer.Serialize(fs, this);
                }
            }

            public static LobbyAdminSetScheduleRequestPart LoadXML(string filePath)
            {
                System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(LobbyAdminSetScheduleRequestPart));
                LobbyAdminSetScheduleRequestPart loadedObj;
                using (System.IO.FileStream fs = new System.IO.FileStream(filePath, System.IO.FileMode.Open))
                {
                    loadedObj = (LobbyAdminSetScheduleRequestPart)serializer.Deserialize(fs);
                }
                return loadedObj;
            }

            public static LobbyAdminSetScheduleRequestPart PtrToStructure(IntPtr ptr)
            {
                LobbyAdminSetScheduleRequestPart result = new LobbyAdminSetScheduleRequestPart();

                // �蓮�ō\���̍\�z
                int baseAddr = ptr.ToInt32();
                result.scheduleBase = (LobbyAdminScheduleBase)Marshal.PtrToStructure(ptr, typeof(LobbyAdminScheduleBase));
                result.scheduleBase.startMonth++;   // �T�[�o��0-11��Ԃ��Ă���̂ŏC��

                LobbyScheduleRecord data;

                for (int i = 0; i < result.scheduleBase.scheduleRecordNum; i++)
                {
                    data = (LobbyScheduleRecord)Marshal.PtrToStructure(
                        new IntPtr(baseAddr + Marshal.SizeOf(typeof(LobbyAdminScheduleBase)) + (i * Marshal.SizeOf(typeof(LobbyScheduleRecord)))),
                        typeof(LobbyScheduleRecord));
                    //data.ntoh();
                    result.scheduleRecords.Add(data);
                }
                return result;
            }

            internal List<byte> ToByteArray()
            {
                List<byte> bytes = new List<byte>();
                scheduleBase.scheduleRecordNum = (ushort)scheduleRecords.Count;
                bytes.AddRange(scheduleBase.ToByteArray());
                for (int i = 0; i < 40; i++)
                {
                    if (i < scheduleBase.scheduleRecordNum)
                    {
                        bytes.AddRange(scheduleRecords[i].ToByteArray());
                    }
                    else
                    {
                        // ���܂�͋�f�[�^��������
                        bytes.AddRange(BitConverter.GetBytes((int)0));
                        bytes.AddRange(BitConverter.GetBytes((int)0));
                    }
                }
                return bytes;
            }

            public string[] ToStringArray()
            {
                List<string> list = scheduleBase.ToStringArray();
                list.Add("" + scheduleRecords.Count);
                return list.ToArray();
            }

            internal string ToToolTipText()
            {
                string result = "";
                foreach (LobbyScheduleRecord record in scheduleRecords)
                {
                    result += record.ToString() + "\n";
                }
                return result;
            }

            internal bool ChangeData(string tag, string data)
            {
                return scheduleBase.ChangeData(tag, data);
            }

            internal bool IsConsistent()
            {
                return scheduleBase.IsConsistent() && scheduleRecords.Count <= 40;
            }
            
            #region ICloneable �����o

            public object Clone()
            {
                return DeepCopy(this);
            }

            #endregion

            #region IComparable �����o

            public int CompareTo(LobbyAdminSetScheduleRequestPart rhs)
            {
                // ���t�Ń\�[�g
                int score = (this.scheduleBase.startYear - rhs.scheduleBase.startYear) * 10000
                    + (this.scheduleBase.startMonth - rhs.scheduleBase.startMonth) * 100
                    + (this.scheduleBase.startDay - rhs.scheduleBase.startDay);
                if (score == 0)
                {
                    // ���߂ċN���������͈���\�[�g�ɂȂ�悤��
                    return this.GetHashCode() - rhs.GetHashCode();
                }
                return score;
            }

            #endregion
        }

        /// <summary>
        /// ���r�[�X�P�W���[���ݒ�Z�b�g���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        [Serializable()]
        public class LobbySchedules : ICloneable

        {
            [System.Xml.Serialization.XmlElement("LobbySchedule", Type = typeof(LobbyAdminSetScheduleRequestPart))]
            public List<LobbyAdminSetScheduleRequestPart> lobbySchedules = new List<LobbyAdminSetScheduleRequestPart>();

            public LobbySchedules()
            {
            }

            public LobbySchedules(LobbyAdminSetScheduleRequestPart[] ary)
            {
                lobbySchedules = new List<LobbyAdminSetScheduleRequestPart>(ary);
            }

            public void Sort()
            {
                lobbySchedules.Sort();
            }

            public static LobbyAdminSetScheduleRequestPart[] PtrToStructure(IntPtr ptr, uint outSize)
            {
                if (outSize == 0 || outSize % LobbyAdminSetScheduleRequestPart.OBJECT_SIZE != 0)
                {
                    throw new Exception("invalid size: " + outSize);
                }
                uint itemNum = (uint)(outSize / LobbyAdminSetScheduleRequestPart.OBJECT_SIZE);

                List<LobbyAdminSetScheduleRequestPart> list = new List<LobbyAdminSetScheduleRequestPart>();
                for (uint i = 0; i < itemNum; i++)
                {
                    list.Add(LobbyAdminSetScheduleRequestPart.PtrToStructure(ptr));
                    ptr = new IntPtr(ptr.ToInt32() + LobbyAdminSetScheduleRequestPart.OBJECT_SIZE);
                }
                return list.ToArray();
            }

            // �X�P�W���[�����Ƃ�byte�̃��X�g�ɂ��ĕԂ�
            internal List<List<byte>> ToByteArrays()
            {
                List<List<byte>> result = new List<List<byte>>();
                foreach (LobbyAdminSetScheduleRequestPart item in lobbySchedules)
                {
                    List<byte> bytes = new List<byte>();
                    bytes.AddRange(item.ToByteArray());
                    result.Add(bytes);
                }
                return result;
            }

            internal bool ChangeData(int row, string tag, string data)
            {
                if (row >= lobbySchedules.Count)
                {
                    return false;
                }
                return lobbySchedules[row].ChangeData(tag, data);
            }

            internal void AddNewSchedule()
            {
                lobbySchedules.Add(new LobbyAdminSetScheduleRequestPart());
            }

            internal int GetScheduleHashCode(int index)
            {
                return lobbySchedules[index].GetHashCode();
            }

            internal int FindSchedule(int hash)
            {
                return lobbySchedules.FindIndex(delegate(LobbyAdminSetScheduleRequestPart item)
                {
                    return item.GetHashCode() == hash;
                });
            }

            internal void DeleteSchedule(int index)
            {
                lobbySchedules.RemoveAt(index);
            }

            internal void CopySchedule(int index)
            {
                LobbyAdminSetScheduleRequestPart schedule = lobbySchedules[index];
                lobbySchedules.Insert(index, (LobbyAdminSetScheduleRequestPart)DeepCopy(schedule));
            }

            internal bool IsConsistent()
            {
                if (lobbySchedules.Count == 0)
                {
                    return false;
                }

                // �ŏ��̃X�P�W���[���������̂��̂Ȃ�s��
                LobbyAdminScheduleBase scheduleBase = lobbySchedules[0].scheduleBase;
                DateTime scheduleTime;
                try
                {
                    scheduleTime = new DateTime(scheduleBase.startYear, scheduleBase.startMonth, scheduleBase.startDay);
                }
                catch (ArgumentOutOfRangeException)
                {
                    // �s���ȓ��t
                    return false;
                }

                DateTime currentTime = DateTime.Now;
                if (currentTime.CompareTo(scheduleTime) < 0)
                {
                    return false;
                }

                foreach (LobbyAdminSetScheduleRequestPart schedule in lobbySchedules)
                {
                    if (!schedule.IsConsistent())
                    {
                        return false;
                    }
                }
                return true;
            }

            #region ICloneable �����o

            public object Clone()
            {
                return DeepCopy(this);
            }

            #endregion
        }

        /// <summary>
        /// ���r�[�X�P�W���[���ݒ�Z�b�g���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetScheduleResponse : ResponseContainer
        {
            public LobbyAdminSetScheduleResponse() { }
        }

        /// <summary>
        /// VIP���R�[�h
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyVipRecord
        {
            private int profileId;
            public int ProfileId
            {
                get { return profileId; }
                set { profileId = value; }
            }

            private int key;
            public int Key
            {
                get { return key; }
                set { key = value; }
            }


            public LobbyVipRecord() { }
        }

        /// <summary>
        /// VIP���R�[�h
        /// </summary>
        [Serializable]
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public class LobbyVipSettingRecord : IComparable<LobbyVipSettingRecord>, ICloneable
        {
            private byte startMonth;                         ///< 
            public byte StartMonth
            {
                get { return startMonth; }
                set { startMonth = value; }
            }

            private byte startDay;                           ///< 
            public byte StartDay
            {
                get { return startDay; }
                set { startDay = value; }
            }

            private ushort startYear;                        ///< 
            public ushort StartYear
            {
                get { return startYear; }
                set { startYear = value; }
            }

            public DateTime StartDate
            {
                get { return new DateTime(startYear, startMonth, startDay); }
                set
                {
                    startYear = (ushort)value.Year;
                    startMonth = (byte)value.Month;
                    startDay = (byte)value.Day;
                }
            }

            private byte endMonth;                           ///< 
            public byte EndMonth
            {
                get { return endMonth; }
                set { endMonth = value; }
            }

            private byte endDay;
            public byte EndDay
            {
                get { return endDay; }
                set { endDay = value; }
            }

            private ushort endYear;                          ///< 
            public ushort EndYear
            {
                get { return endYear; }
                set { endYear = value; }
            }

            public DateTime EndDate
            {
                get { return new DateTime(endYear, endMonth, endDay); }
                set
                {
                    endYear = (ushort)value.Year;
                    endMonth = (byte)value.Month;
                    endDay = (byte)value.Day;
                }
            }

            private int profileId;
            public int ProfileId
            {
                get { return profileId; }
                set { profileId = value; }
            }

            private int eventId;
            public int EventId
            {
                get { return eventId; }
                set { eventId = value; }
            }

            public LobbyVipSettingRecord()
            {
                // DGV�g�p���̃f�t�H���g�f�[�^��ݒ�
                DateTime now = DateTime.Now;
                startMonth = (byte)now.Month;
                startDay = (byte)now.Day;
                startYear = (ushort)now.Year;
                now = now.AddDays(1);
                endMonth = (byte)now.Month;
                endDay = (byte)now.Day;
                endYear = (ushort)now.Year;
                profileId = 100000;
                eventId = 0;
            }

            internal List<byte> ToByteArray()
            {
                List<byte> bytes = new List<byte>();
                bytes.Add((byte)(startMonth - 1));  // �T�[�o�ł�0-11��v��
                bytes.Add(startDay);
                bytes.AddRange(BitConverter.GetBytes(startYear));
                bytes.Add((byte)(endMonth - 1));  // �T�[�o�ł�0-11��v��
                bytes.Add(endDay);
                bytes.AddRange(BitConverter.GetBytes(endYear));
                bytes.AddRange(BitConverter.GetBytes(profileId));
                bytes.AddRange(BitConverter.GetBytes(eventId));
                return bytes;
            }

            internal bool IsConsistent()
            {
                DateTime startTime;
                DateTime endTime;
                try
                {
                    startTime = new DateTime(startYear, startMonth, startDay);
                    endTime = new DateTime(endYear, endMonth, endDay);
                }
                catch (ArgumentOutOfRangeException)
                {
                    // �s���ȓ��t
                    return false;
                }
                if (startTime >= endTime)
                {
                    return false;
                }

                return startYear >= 2000 && startYear <= 2200 && startMonth >= 1 && startMonth <= 12 && startDay >= 1 && startDay <= 31
                    && endYear >= 2000 && endYear <= 2200 && endMonth >= 1 && endMonth <= 12 && endDay >= 1 && endDay <= 31
                    && profileId >= 100000;
            }

            #region IComparable<LobbyVipSettingRecord> �����o

            public int CompareTo(LobbyVipSettingRecord other)
            {
                // �J�n�����ɂ���
                return (this.startYear * 10000 + this.startMonth * 100 + this.startDay)
                     - (other.startYear * 10000 + other.startMonth * 100 + other.startDay);
            }

            #endregion

            #region ICloneable �����o

            public object Clone()
            {
                return DeepCopy(this);
            }

            #endregion
        }

        /// <summary>
        /// VIP�擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyGetVipRequest : RequestContainer
        {
            uint trainerId;
            public LobbyGetVipRequest() { }
            public LobbyGetVipRequest(int _id, uint _trainerId)
                : base(_id)
            {
                trainerId = _trainerId;
            }
        }

        /// <summary>
        /// VIP�擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyGetVipResponse : ResponseContainer
        {
            public List<LobbyVipRecord> vipRecords = new List<LobbyVipRecord>();

            public LobbyGetVipResponse() { }
        }

        /// <summary>
        /// VIP�ݒ�擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetVipSettingRequest : RequestContainer
        {
            public LobbyAdminGetVipSettingRequest() { }
            public LobbyAdminGetVipSettingRequest(int _id)
                : base(_id)
            {
            }
        }

        /// <summary>
        /// VIP�ݒ�擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetVipSettingResponse : ResponseContainer
        {
            public List<LobbyVipSettingRecord> vipSettingRecords = new List<LobbyVipSettingRecord>();

            public LobbyAdminGetVipSettingResponse() { }
        }

        /// <summary>
        /// VIP�ݒ�o�^���N�G�X�g
        /// </summary>
        [System.Xml.Serialization.XmlRoot("VipSetting")]
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public class LobbyAdminSetVipSettingRequest : RequestContainer
        {
            public List<LobbyVipSettingRecord> vipSettingRecords = new List<LobbyVipSettingRecord>();

            public LobbyAdminSetVipSettingRequest() { }
            public LobbyAdminSetVipSettingRequest(int _id, List<LobbyVipSettingRecord> src)
                : base(_id)
            {
                vipSettingRecords = src;
            }

            internal override List<byte> ToByteArray()
            {
                // �擪�ɋ��ʃw�b�_��t����
                List<byte> result = base.ToByteArray();

                foreach (LobbyVipSettingRecord item in vipSettingRecords)
                {
                    result.AddRange(item.ToByteArray());
                }
                return result;
            }

            internal bool IsConsistent()
            {
                foreach (LobbyVipSettingRecord item in vipSettingRecords)
                {
                    if (!item.IsConsistent())
                    {
                        return false;
                    }
                }
                return true;
            }

        }

        /// <summary>
        /// VIP�ݒ�o�^���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetVipSettingResponse : ResponseContainer
        {
            public LobbyAdminSetVipSettingResponse() { }
        }



        internal LobbyGetScheduleResponse LobbyGetSchedule(out string message)
        {
            int baseStructureOffset = 20;
            LobbyGetScheduleRequest request = new LobbyGetScheduleRequest(ADMIN_PROFILE_ID);
            LobbyGetScheduleResponse result = new LobbyGetScheduleResponse();
            IntPtr ptr;
            uint outSize;
            message = "";

            //request.hton();

            encSession.Session(URL_GET_LOBBY_SCHEDULE, ADMIN_PROFILE_ID, request, out ptr, out outSize);

            // ���X�|���X�R�[�h�̃`�F�b�N
            if (outSize < Marshal.SizeOf(result.code))
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
            }
            else
            {
                result.code = (uint)Marshal.ReadInt32(ptr);
                //result.ntoh();
                if (result.code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(result.code)), 128);
                    return result;
                }
            }

            if (outSize < baseStructureOffset)
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��BLobbyGetScheduleResponse�\���̂̍Œ�T�C�Y�𖞂����Ă��܂���B");
            }

            // �蓮�ō\���̍\�z
            int baseAddr = ptr.ToInt32();
            result.lockTime = (uint)Marshal.ReadInt32(new IntPtr(baseAddr + 4));
            result.random = (uint)Marshal.ReadInt32(new IntPtr(baseAddr + 8));
            result.roomFlag = (uint)Marshal.ReadInt32(new IntPtr(baseAddr + 12));
            result.roomType = Marshal.ReadByte(new IntPtr(baseAddr + 16));
            result.season = Marshal.ReadByte(new IntPtr(baseAddr + 17));
            result.scheduleRecordNum = (ushort)Marshal.ReadInt16(new IntPtr(baseAddr + 18));
            //result.ntoh();

            LobbyScheduleRecord data;

            // �T�C�Y�`�F�b�N
            if ((outSize - baseStructureOffset) % Marshal.SizeOf(typeof(LobbyScheduleRecord)) == 0 &&
                (outSize - baseStructureOffset) / Marshal.SizeOf(typeof(LobbyScheduleRecord)) == result.scheduleRecordNum)
            {
                for (int i = 0; i < result.scheduleRecordNum; i++)
                {
                    data = (LobbyScheduleRecord)Marshal.PtrToStructure(
                        new IntPtr(baseAddr + baseStructureOffset + (i * Marshal.SizeOf(typeof(LobbyScheduleRecord)))), typeof(LobbyScheduleRecord));
                    //data.ntoh();
                    result.scheduleRecords.Add(data);
                }
            }
            else
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��BPPW_LobbyScheduleRecord�\���̂̔{���ł͂���܂���B");
            }
            return result;
        }

        /// <summary>
        /// �s���`�F�b�N���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminCheckProfileRequest : RequestContainer
        {
            REF_WFLBY_USER_PROFILE profile;

            public LobbyAdminCheckProfileRequest() { }
            public LobbyAdminCheckProfileRequest(int _id, REF_WFLBY_USER_PROFILE profile_)
                : base(_id)
            {
                profile = profile_;
            }

            internal override List<byte> ToByteArray()
            {
                List<byte> result = base.ToByteArray();
                result.AddRange(profile.ToByteArray());
                return result;
            }
        }

        /// <summary>
        /// �s���`�F�b�N���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminCheckProfileResponse : ResponseContainer
        {
            public REF_WFLBY_USER_PROFILE profile;
            public LobbyAdminCheckProfileResponse(uint code, IntPtr ptr, int size)
                : base(code)
            {
                profile = new REF_WFLBY_USER_PROFILE(ptr, size);
            }

            public static LobbyAdminCheckProfileResponse Result(IntPtr ptr, int size, out uint code, out string message)
            {
                code = 0;
                message = "";

                // ���X�|���X�R�[�h�̃`�F�b�N
                if (size < Marshal.SizeOf(code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
                }
                code = (uint)Marshal.ReadInt32(ptr);
                if (code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), 128);
                    return null;
                }

                return new LobbyAdminCheckProfileResponse(code, new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), size - Marshal.SizeOf(code));
            }
        }

        /// <summary>
        /// �A���P�[�g���擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyGetQuestionnaireRequest : RequestContainer
        {
            public LobbyGetQuestionnaireRequest() { }
            public LobbyGetQuestionnaireRequest(int _id)
                : base(_id)
            {
            }
        }

        /// <summary>
        /// �A���P�[�g���擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyGetQuestionnaireResponse : ResponseContainer
        {
            public REF_PPW_LobbyQuestionnaire questionnaire;
            public LobbyGetQuestionnaireResponse(uint code, IntPtr ptr, int size)
                : base(code)
            {
                questionnaire = new REF_PPW_LobbyQuestionnaire(ptr, size);
            }

            public static LobbyGetQuestionnaireResponse Result(IntPtr ptr, int size, out uint code, out string message)
            {
                code = 0;
                message = "";

                // ���X�|���X�R�[�h�̃`�F�b�N
                if (size < Marshal.SizeOf(code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
                }
                code = (uint)Marshal.ReadInt32(ptr);
                if (code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), 128);
                    return null;
                }

                return new LobbyGetQuestionnaireResponse(code, new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), size - Marshal.SizeOf(code));
            }
        }

        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public class LobbySpecialWeekSetting
        {
            public List<REF_PPW_LobbySpecialWeekRecord> records = new List<REF_PPW_LobbySpecialWeekRecord>();
            public LobbySpecialWeekSetting()
            {
            }
            public LobbySpecialWeekSetting(List<REF_PPW_LobbySpecialWeekRecord> records_)
            {
                records = records_;
            }

            internal IEnumerable<byte> ToByteArray()
            {
                List<byte> result = new List<byte>();
                foreach (REF_PPW_LobbySpecialWeekRecord record in records)
                {
                    result.AddRange(record.ToByteArray());
                }
                return result;
            }

            internal bool IsConsistent()
            {
                foreach (REF_PPW_LobbySpecialWeekRecord item in records)
                {
                    if (!item.IsConsistent())
                    {
                        return false;
                    }
                }
                return true;
            }
        }

        /// <summary>
        /// �X�y�V�����E�B�[�N�擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetSpecialWeekRequest : RequestContainer
        {
            public LobbyAdminGetSpecialWeekRequest() { }
            public LobbyAdminGetSpecialWeekRequest(int _id)
                : base(_id)
            {
            }
        }

        /// <summary>
        /// �X�y�V�����E�B�[�N�擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetSpecialWeekResponse : ResponseContainer
        {
            public LobbySpecialWeekSetting specialWeek = new LobbySpecialWeekSetting();
            public LobbyAdminGetSpecialWeekResponse(uint code, IntPtr ptr, int size)
                : base(code)
            {
                if (size % REF_PPW_LobbySpecialWeekRecord.SPECIAL_RECORD_SIZE != 0)
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��BPPW_LobbySpecialWeekRecord�\���̂̔{���ł͂���܂���B");
                }

                while (size != 0)
                {
                    specialWeek.records.Add(new REF_PPW_LobbySpecialWeekRecord(ptr, REF_PPW_LobbySpecialWeekRecord.SPECIAL_RECORD_SIZE));
                    ptr = new IntPtr(ptr.ToInt32() + REF_PPW_LobbySpecialWeekRecord.SPECIAL_RECORD_SIZE);
                    size -= REF_PPW_LobbySpecialWeekRecord.SPECIAL_RECORD_SIZE;
                }
            }

            public static LobbyAdminGetSpecialWeekResponse Result(IntPtr ptr, int size, out uint code, out string message)
            {
                code = 0;
                message = "";

                // ���X�|���X�R�[�h�̃`�F�b�N
                if (size < Marshal.SizeOf(code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
                }
                code = (uint)Marshal.ReadInt32(ptr);
                if (code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), 128);
                    return null;
                }

                return new LobbyAdminGetSpecialWeekResponse(code, new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), size - Marshal.SizeOf(code));
            }
        }


        /// <summary>
        /// �X�y�V�����E�B�[�N�o�^���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public class LobbyAdminSetSpecialWeekRequest : RequestContainer
        {
            public LobbySpecialWeekSetting specialWeek = new LobbySpecialWeekSetting();

            public LobbyAdminSetSpecialWeekRequest() { }
            public LobbyAdminSetSpecialWeekRequest(int _id, LobbySpecialWeekSetting specialWeek_)
                : base(_id)
            {
                specialWeek = specialWeek_;
            }

            internal override List<byte> ToByteArray()
            {
                List<byte> result = base.ToByteArray();
                result.AddRange(specialWeek.ToByteArray());
                return result;
            }

            internal bool IsConsistent()
            {
                return specialWeek.IsConsistent();
            }

        }

        /// <summary>
        /// �X�y�V�����E�B�[�N�o�^���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetSpecialWeekResponse : ResponseContainer
        {
            public LobbyAdminSetSpecialWeekResponse() { }

            public static void Result(IntPtr ptr, int size, out uint code, out string message)
            {
                code = 0;
                message = "";

                // ���X�|���X�R�[�h�̃`�F�b�N
                if (size < Marshal.SizeOf(code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
                }
                code = (uint)Marshal.ReadInt32(ptr);
                if (code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), 128);
                    return;
                }
            }
        }


        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public class LobbyFreeQuestionSetting
        {
            public List<REF_PPW_LobbyFreeQuestion> records = new List<REF_PPW_LobbyFreeQuestion>();
            public LobbyFreeQuestionSetting() { }
            public LobbyFreeQuestionSetting(int language)
            {
                for (int i = 0; i < REF_PPW_LobbySpecialWeekRecord.LOBBY_FREE_QUESTION_NUM; i++)
                {
                    List<string> answers = new List<string>();
                    answers.Add("�����Ƃ�1");
                    answers.Add("�����Ƃ�2");
                    answers.Add("�����Ƃ�3");

                    records.Add(new REF_PPW_LobbyFreeQuestion(REF_PPW_LobbySpecialWeekRecord.LOBBY_FREE_QUESTION_START_NO + i,
                        language,
                        "������Ԃ�",
                        answers));
                }
            }

            public LobbyFreeQuestionSetting(IntPtr ptr, int size)
            {
                while (size != 0)
                {
                    records.Add(new REF_PPW_LobbyFreeQuestion(ptr, REF_PPW_LobbyFreeQuestion.NATIVE_SIZE));
                    ptr = new IntPtr(ptr.ToInt32() + REF_PPW_LobbyFreeQuestion.NATIVE_SIZE);
                    size -= REF_PPW_LobbyFreeQuestion.NATIVE_SIZE;
                }
            }

            public LobbyFreeQuestionSetting(List<REF_PPW_LobbyFreeQuestion> records_)
            {
                records = records_;
            }

            /*
            public LobbyFreeQuestionSetting(int questionNo, int language, DataGridView dgv)
            {
                for (int i = 0; i < dgv.Rows.Count; i++)
                {
                    string cur = "";
                    string questionSentence = "";
                    string answer1 = "";
                    string answer2 = "";
                    string answer3 = "";
                    try
                    {
                        cur = dgv[0, i].Value.ToString();
                        questionSentence = DpwUtil.Tools.UnicodeToPokecode(cur);
                        cur = dgv[1, i].Value.ToString();
                        answer1 = DpwUtil.Tools.UnicodeToPokecode(cur);
                        cur = dgv[2, i].Value.ToString();
                        answer2 = DpwUtil.Tools.UnicodeToPokecode(cur);
                        cur = dgv[3, i].Value.ToString();
                        answer3 = DpwUtil.Tools.UnicodeToPokecode(cur);
                    }
                    catch (Exception e)
                    {
                        MessageBox.Show("�u" + cur + "�v�̓Q�[���������R�[�h�֕ϊ��ł��܂���B");
                        throw e;
                    }
                    List<ushort> questionSentenceList = Util.BytesToShorts(new List<byte>(Encoding.Unicode.GetBytes(questionSentence)), false);
                    List<ushort> answer1List = Util.BytesToShorts(new List<byte>(Encoding.Unicode.GetBytes(answer1)), false);
                    List<ushort> answer2List = Util.BytesToShorts(new List<byte>(Encoding.Unicode.GetBytes(answer2)), false);
                    List<ushort> answer3List = Util.BytesToShorts(new List<byte>(Encoding.Unicode.GetBytes(answer3)), false);
                    List<List<ushort>> answer = new List<List<ushort>>();
                    answer.Add(answer1List);
                    answer.Add(answer2List);
                    answer.Add(answer3List);
                    records.Add(new REF_PPW_LobbyFreeQuestion(questionNo, language, questionSentenceList, answer));
                }
            }
             * */

            internal IEnumerable<byte> ToByteArray()
            {
                List<byte> result = new List<byte>();
                foreach (REF_PPW_LobbyFreeQuestion record in records)
                {
                    result.AddRange(record.ToByteArray());
                }
                return result;
            }

            internal bool IsConsistent()
            {
                foreach (REF_PPW_LobbyFreeQuestion item in records)
                {
                    if (!item.IsConsistent())
                    {
                        return false;
                    }
                }
                return true;
            }
        }

        /// <summary>
        /// �C�ӎ���o�^���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        public class LobbyAdminSetFreeQuestionRequest : RequestContainer
        {
            public LobbyFreeQuestionSetting freeQuestion;

            public LobbyAdminSetFreeQuestionRequest(int _id, LobbyFreeQuestionSetting freeQuestion_)
                : base(_id)
            {
                freeQuestion = freeQuestion_;
            }

            internal override List<byte> ToByteArray()
            {
                List<byte> result = base.ToByteArray();
                result.AddRange(freeQuestion.ToByteArray());
                return result;
            }

            internal bool IsConsistent()
            {
                return freeQuestion.IsConsistent();
            }

        }

        /// <summary>
        /// �C�ӎ���o�^���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetFreeQuestionResponse : ResponseContainer
        {
            public LobbyAdminSetFreeQuestionResponse() { }

            public static void Result(IntPtr ptr, int size, out uint code, out string message)
            {
                code = 0;
                message = "";

                // ���X�|���X�R�[�h�̃`�F�b�N
                if (size < Marshal.SizeOf(code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
                }
                code = (uint)Marshal.ReadInt32(ptr);
                if (code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), 128);
                    return;
                }
            }
        }

        /// <summary>
        /// �C�ӎ���擾���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetFreeQuestionRequest : RequestContainer
        {
            int languageForGet;
            public LobbyAdminGetFreeQuestionRequest() { }
            public LobbyAdminGetFreeQuestionRequest(int _id, int languageForGet_)
                : base(_id)
            {
                languageForGet = languageForGet_;
            }
        }

        /// <summary>
        /// �C�ӎ���擾���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminGetFreeQuestionResponse : ResponseContainer
        {
            public LobbyFreeQuestionSetting freeQuestion;
            public LobbyAdminGetFreeQuestionResponse(uint code, IntPtr ptr, int size)
                : base(code)
            {
                freeQuestion = new LobbyFreeQuestionSetting(ptr, size);
            }

            public static LobbyAdminGetFreeQuestionResponse Result(IntPtr ptr, int size, out uint code, out string message)
            {
                code = 0;
                message = "";

                // ���X�|���X�R�[�h�̃`�F�b�N
                if (size < Marshal.SizeOf(code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
                }
                code = (uint)Marshal.ReadInt32(ptr);
                if (code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), 128);
                    return null;
                }

                return new LobbyAdminGetFreeQuestionResponse(code, new IntPtr(ptr.ToInt32() + Marshal.SizeOf(code)), size - Marshal.SizeOf(code));
            }
        }

        /// <summary>
        /// �A���P�[�g�X�V���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminUpdateQuestionnaireRequest : RequestContainer
        {
            public LobbyAdminUpdateQuestionnaireRequest(int _id)
                : base(_id)
            {
            }
        }

        /// <summary>
        /// �A���P�[�g�X�V���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminUpdateQuestionnaireResponse : ResponseContainer
        {
        }

        /// <summary>
        /// �A���P�[�g��o���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbySubmitQuestionnaireRequest : RequestContainer
        {
            int questionSerialNo;
            int questionNo;
            int answerNo;
            int trainerId;
            int sex;
            ushort country;
            ushort region;

            public LobbySubmitQuestionnaireRequest(int _id, int questionSerialNo_, byte language_, int answerNo_)
                : base(_id)
            {
                questionSerialNo = questionSerialNo_;
                answerNo = answerNo_;
                language = language_;

                // �c��͓K���ɐݒ�
                trainerId = 0;
                sex = 0;
                country = 0;
                region = 0;
            }
        }

        /// <summary>
        /// �A���P�[�g��o���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbySubmitQuestionnaireResponse : ResponseContainer
        {
        }

        /// <summary>
        /// ����ʂ��ԍ��Z�b�g���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetQuestionSerialNoRequest : RequestContainer
        {
            int questionSerialNo;
            public LobbyAdminSetQuestionSerialNoRequest(int _id, int questionSerialNo_)
                : base(_id)
            {
                questionSerialNo = questionSerialNo_;
            }
        }

        /// <summary>
        /// ����ʂ��ԍ��Z�b�g���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetQuestionSerialNoResponse : ResponseContainer
        {
        }

        /// <summary>
        /// ���̎���ԍ��Z�b�g���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetNextQuestionNoRequest : RequestContainer
        {
            int nextQuestionNo;
            public LobbyAdminSetNextQuestionNoRequest(int _id, int nextQuestionNo_)
                : base(_id)
            {
                nextQuestionNo = nextQuestionNo_;
            }
        }

        /// <summary>
        /// ���̎���ԍ��Z�b�g���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetNextQuestionNoResponse : ResponseContainer
        {
        }

        /// <summary>
        /// ����ԍ��Z�b�g���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetQuestionNoRequest : RequestContainer
        {
            int questionNo;
            public LobbyAdminSetQuestionNoRequest(int _id, int questionNo_)
                : base(_id)
            {
                questionNo = questionNo_;
            }
        }

        /// <summary>
        /// ����ԍ��Z�b�g���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetQuestionNoResponse : ResponseContainer
        {
        }

        /// <summary>
        /// �A���P�[�g���������N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminInitializeQuestionnaireRequest : RequestContainer
        {
            public LobbyAdminInitializeQuestionnaireRequest(int _id)
                : base(_id)
            {
            }
        }

        /// <summary>
        /// �A���P�[�g���������X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminInitializeQuestionnaireResponse : ResponseContainer
        {
        }

        /// <summary>
        /// ���ʎ���臒l�Z�b�g���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetSpecialQuestionThresholdRequest : RequestContainer
        {
            int specialQuestionThreshold;
            public LobbyAdminSetSpecialQuestionThresholdRequest(int _id, int specialQuestionThreshold_)
                : base(_id)
            {
                specialQuestionThreshold = specialQuestionThreshold_;
            }
        }

        /// <summary>
        /// ���ʎ���臒l�Z�b�g���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetSpecialQuestionThresholdResponse : ResponseContainer
        {
        }

        /// <summary>
        /// �f�t�H���g�W�v�t���O�Z�b�g�Z�b�g���N�G�X�g
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4, CharSet = CharSet.Ansi)]
        internal class LobbyAdminSetDefaultSummarizeFlagRequest : RequestContainer
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 12)]
            string defaultSummarizeFlag;
            public LobbyAdminSetDefaultSummarizeFlagRequest(int _id, string defaultSummarizeFlag_)
                : base(_id)
            {
                defaultSummarizeFlag = defaultSummarizeFlag_;
            }
        }

        /// <summary>
        /// �f�t�H���g�W�v�t���O�Z�b�g���X�|���X
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 4)]
        internal class LobbyAdminSetDefaultSummarizeFlagResponse : ResponseContainer
        {
        }

        internal LobbySchedules LobbyAdminGetSchedule(out uint code, out string message)
        {
            LobbyAdminGetScheduleRequest request = new LobbyAdminGetScheduleRequest(ADMIN_PROFILE_ID);
            LobbyAdminSetScheduleRequestPart[] result;  // ���X�|���X��LobbyAdminSetScheduleRequest�̔z������
            IntPtr ptr;
            uint outSize;
            message = "";
            const int resultCodeSize = 4;

            //request.hton();

            encSession.Session(URL_ADMIN_GET_LOBBY_SCHEDULE, ADMIN_PROFILE_ID, request, out ptr, out outSize);

            // �T�C�Y�̃`�F�b�N
            if (outSize % LobbyAdminSetScheduleRequestPart.OBJECT_SIZE != resultCodeSize)
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��Bsize: " + outSize);
            }

            code = (uint)Marshal.ReadInt32(ptr);

            if (code != 0)
            {
                // ���b�Z�[�W�̎��o��
                message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + resultCodeSize), 128);
                return null;
            }

            result = LobbySchedules.PtrToStructure(new IntPtr(ptr.ToInt32() + resultCodeSize), outSize - resultCodeSize);

            return new LobbySchedules(result);
        }

        internal void LobbyAdminSetSchedule(LobbySchedules schedules, out uint code, out string message)
        {
            LobbyAdminSetScheduleResponse result = new LobbyAdminSetScheduleResponse();
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";


            List<List<byte>> list = schedules.ToByteArrays();
            if (list.Count == 0)
            {
                throw new Exception("�X�P�W���[������ł��B");
            }

            // ��x�ɕ����̃X�P�W���[�����Z�b�g���邽�߁A�������̃X�P�W���[�����܂Ƃ߂����X�g�����B
            List<List<byte>> appendedList = new List<List<byte>>();
            {
                foreach (List<byte> bytes in list)
                {
                    if (appendedList.Count == 0 || appendedList[appendedList.Count - 1].Count + bytes.Count > RequestInterface.MAX_REQUEST_SIZE)
                    {
                        // �Ō�̗v�f�������ς��ɂȂ����̂ŐV�K���X�g��ǉ�
                        // ���ʃw�b�_��t���ď���������
                        appendedList.Add(new RequestContainer().ToByteArray());
                    }

                    // �f�[�^��ǉ�
                    appendedList[appendedList.Count - 1].AddRange(bytes);
                }
            }

            string url = URL_ADMIN_SET_LOBBY_SCHEDULE;
            foreach (List<byte> bytes in appendedList)
            {
                encSession.Session(url, ADMIN_PROFILE_ID, bytes.ToArray(), out ptr, out outSize);

                // ���X�|���X�R�[�h�̃`�F�b�N
                if (outSize < Marshal.SizeOf(result.code))
                {
                    throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
                }

                code = (uint)Marshal.ReadInt32(ptr);
                //result.ntoh();
                if (code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(result.code)), 128);
                    return;
                }

                // ���ڈȍ~�̓A�y���h�pURL�ɕύX
                url = URL_ADMIN_APPEND_LOBBY_SCHEDULE;
            }
        }

        internal LobbyGetVipResponse LobbyGetVip(uint trainerId, out string message)
        {
            int baseStructureOffset = 4;
            LobbyGetVipRequest request = new LobbyGetVipRequest(ADMIN_PROFILE_ID, trainerId);
            LobbyGetVipResponse result = new LobbyGetVipResponse();
            IntPtr ptr;
            uint outSize;
            message = "";

            encSession.Session(URL_GET_LOBBY_VIP, ADMIN_PROFILE_ID, request, out ptr, out outSize);

            // ���X�|���X�R�[�h�̃`�F�b�N
            if (outSize < Marshal.SizeOf(result.code))
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
            }
            else
            {
                result.code = (uint)Marshal.ReadInt32(ptr);
                if (result.code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(result.code)), 128);
                    return result;
                }
            }

            int baseAddr = ptr.ToInt32();

            long recordNum = (outSize - baseStructureOffset) / Marshal.SizeOf(typeof(LobbyVipRecord));
            if (recordNum == 0)
            {
                MessageBox.Show("����VIP�͂��܂���B");
                return result;
            }

            // �T�C�Y�`�F�b�N
            if ((outSize - baseStructureOffset) % Marshal.SizeOf(typeof(LobbyVipRecord)) == 0)
            {
                LobbyVipRecord data;
                for (int i = 0; i < recordNum; i++)
                {
                    data = (LobbyVipRecord)Marshal.PtrToStructure(
                        new IntPtr(baseAddr + baseStructureOffset + (i * Marshal.SizeOf(typeof(LobbyVipRecord)))), typeof(LobbyVipRecord));
                    result.vipRecords.Add(data);
                }
            }
            else
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��BLobbyVipRecord�\���̂̔{���ł͂���܂���B");
            }
            return result;
        }

        internal LobbyAdminGetVipSettingResponse LobbyAdminGetVipSetting(out string message)
        {
            int baseStructureOffset = 4;
            LobbyAdminGetVipSettingRequest request = new LobbyAdminGetVipSettingRequest(ADMIN_PROFILE_ID);
            LobbyAdminGetVipSettingResponse result = new LobbyAdminGetVipSettingResponse();
            IntPtr ptr;
            uint outSize;
            message = "";

            encSession.Session(URL_ADMIN_GET_LOBBY_VIP, ADMIN_PROFILE_ID, request, out ptr, out outSize);

            // ���X�|���X�R�[�h�̃`�F�b�N
            if (outSize < Marshal.SizeOf(result.code))
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
            }
            else
            {
                result.code = (uint)Marshal.ReadInt32(ptr);
                if (result.code != 0)
                {
                    // ���b�Z�[�W�̎��o��
                    message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(result.code)), 128);
                    return result;
                }
            }

            int baseAddr = ptr.ToInt32();

            long recordNum = (outSize - baseStructureOffset) / Marshal.SizeOf(typeof(LobbyVipSettingRecord));
            if (recordNum == 0)
            {
                MessageBox.Show("�T�[�o�ɓo�^����Ă���VIP�ݒ胊�X�g�͋�ł��B");
                return result;
            }

            // �T�C�Y�`�F�b�N
            if ((outSize - baseStructureOffset) % Marshal.SizeOf(typeof(LobbyVipSettingRecord)) == 0)
            {
                LobbyVipSettingRecord data;
                for (int i = 0; i < recordNum; i++)
                {
                    data = (LobbyVipSettingRecord)Marshal.PtrToStructure(
                        new IntPtr(baseAddr + baseStructureOffset + (i * Marshal.SizeOf(typeof(LobbyVipSettingRecord)))), typeof(LobbyVipSettingRecord));
                    data.StartMonth++;  // ���̓T�[�o����0-11���Ԃ��Ă���̂ŏC��
                    data.EndMonth++;
                    result.vipSettingRecords.Add(data);
                }
            }
            else
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��BLobbyVipSettingRecord�\���̂̔{���ł͂���܂���B");
            }
            return result;
        }

        internal void LobbyAdminSetVipSettingRecord(LobbyAdminSetVipSettingRequest req, out uint code, out string message)
        {
            LobbyAdminSetScheduleResponse result = new LobbyAdminSetScheduleResponse();
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";


            List<byte> bytes = req.ToByteArray();
            if (bytes.Count == 0)
            {
                if (MessageBox.Show("���VIP���X�g��o�^���܂��B��낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
                {
                    return;
                }
            }

            encSession.Session(URL_ADMIN_SET_LOBBY_VIP, ADMIN_PROFILE_ID, bytes.ToArray(), out ptr, out outSize);

            // ���X�|���X�R�[�h�̃`�F�b�N
            if (outSize < Marshal.SizeOf(result.code))
            {
                throw new Exception("���X�|���X�T�C�Y���s���ł��B���X�|���X�R�[�h���܂܂�Ă��܂���B");
            }

            code = (uint)Marshal.ReadInt32(ptr);
            if (code != 0)
            {
                // ���b�Z�[�W�̎��o��
                message = Marshal.PtrToStringAnsi(new IntPtr(ptr.ToInt32() + Marshal.SizeOf(result.code)), 128);
                return;
            }
        }

        internal LobbyAdminCheckProfileResponse LobbyAdminCheckProfile(LobbyAdminCheckProfileRequest req, out uint code, out string message)
        {
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";

            List<byte> bytes = req.ToByteArray();
            encSession.Session(URL_ADMIN_CHECK_PROFILE, ADMIN_PROFILE_ID, bytes.ToArray(), out ptr, out outSize);

            return LobbyAdminCheckProfileResponse.Result(ptr, (int)outSize, out code, out message);
        }

        internal LobbyGetQuestionnaireResponse LobbyGetQuestionnaire(LobbyGetQuestionnaireRequest req, out uint code, out string message)
        {
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";

            List<byte> bytes = req.ToByteArray();
            encSession.Session(URL_GET_QUESTIONNAIRE, ADMIN_PROFILE_ID, bytes.ToArray(), out ptr, out outSize);

            return LobbyGetQuestionnaireResponse.Result(ptr, (int)outSize, out code, out message);
        }

        internal LobbyAdminGetSpecialWeekResponse LobbyAdminGetSpecialWeek(LobbyAdminGetSpecialWeekRequest req, out uint code, out string message)
        {
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";

            List<byte> bytes = req.ToByteArray();
            encSession.Session(URL_ADMIN_GET_SPECIAL_WEEK, ADMIN_PROFILE_ID, bytes.ToArray(), out ptr, out outSize);

            return LobbyAdminGetSpecialWeekResponse.Result(ptr, (int)outSize, out code, out message);
        }

        internal void LobbyAdminSetSpecialWeek(LobbyAdminSetSpecialWeekRequest req, out uint code, out string message)
        {
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";

            List<byte> bytes = req.ToByteArray();
            encSession.Session(URL_ADMIN_SET_SPECIAL_WEEK, ADMIN_PROFILE_ID, bytes.ToArray(), out ptr, out outSize);

            LobbyAdminSetSpecialWeekResponse.Result(ptr, (int)outSize, out code, out message);
        }

        internal void LobbyAdminSetFreeQuestion(LobbyAdminSetFreeQuestionRequest req, out uint code, out string message)
        {
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";

            List<byte> bytes = req.ToByteArray();
            encSession.Session(URL_ADMIN_SET_FREE_QUESTION, ADMIN_PROFILE_ID, bytes.ToArray(), out ptr, out outSize);

            LobbyAdminSetFreeQuestionResponse.Result(ptr, (int)outSize, out code, out message);
        }

        internal LobbyAdminGetFreeQuestionResponse LobbyAdminGetFreeQuestion(LobbyAdminGetFreeQuestionRequest req, out uint code, out string message)
        {
            IntPtr ptr;
            uint outSize;
            code = 0;
            message = "";

            encSession.Session(URL_ADMIN_GET_FREE_QUESTION, ADMIN_PROFILE_ID, req, out ptr, out outSize);

            return LobbyAdminGetFreeQuestionResponse.Result(ptr, (int)outSize, out code, out message);
        }
    }
}
