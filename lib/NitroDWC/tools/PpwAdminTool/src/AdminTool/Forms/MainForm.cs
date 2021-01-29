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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Net;
using System.Net.Cache;
using System.Diagnostics;
using System.Collections;
using System.Runtime.InteropServices;

namespace AdminTool
{
    public partial class MainForm : Form
    {
        private RequestInterface requestInterface;
        private const string defaultStatusText = "Ready";
        List<List<string>> keywordIndexes = new List<List<string>>();
        Properties.Settings settings = Properties.Settings.Default;
        private const string EDIT_SCHEDULE_COLUMN_TEXT = "�C�x���g��";

        List<ExtendedDataGridView> freeQuestionDGVs = new List<ExtendedDataGridView>();

        public MainForm()
        {
            InitializeComponent();

            try
            {
                string proxyServer = settings.ProxyServer;
                requestInterface = RequestInterface.CreateInstance(RequestInterface.ServerInfo.KIND.DEBUG, proxyServer);
            }
            catch (Exception e)
            {
                MessageBox.Show("�������Ɏ��s���܂����B\n" + e.ToString());
                throw new Exception("�I�����܂�");
            }
            toolStripStatusLabel.Text = defaultStatusText;

            if (RequestInterface.ServerInfo.SECRET_KEY_DEV == null)
            {
                ToolStripMenuItemDevDebugServer.Enabled = false;
                ToolStripMenuItemDevReleaseServer.Enabled = false;
                ToolStripMenuItemDebugServer_Click(null, null);
            }
            else
            {
                // �J���p�T�[�o���g����Ƃ��͊J���p�T�[�o�ɐڑ�����
                ToolStripMenuItemDevDebugServer_Click(null, null);
            }

            // �X�P�W���[���p�f�[�^�O���b�h�̃J������������
            {
                CalendarColumn text = new CalendarColumn();
                text.DataPropertyName = "StartDate";
                text.Name = "�J�n��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                text.MinimumWidth = 90;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "LockTime";
                text.Name = "���b�N����";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "RoomFlag";
                text.Name = "�t���O";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "RoomRate1";
                text.Name = "������";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "RoomRate2";
                text.Name = "������";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "RoomRate3";
                text.Name = "������";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "RoomRate4";
                text.Name = "������";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "RoomRate5";
                text.Name = "�鑮��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewComboBoxColumn text = new DataGridViewComboBoxColumn();
                text.DataSource = Enum.GetValues(typeof(RequestInterface.PPW_LOBBY_SEASON));
                text.DataPropertyName = "EnumSeason";
                text.Name = "�G��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                text.MinimumWidth = 80;
                dataGridViewSchedule.Columns.Add(text);
            }
            {
                DataGridViewButtonColumn text = new DataGridViewButtonColumn();
                text.DataPropertyName = "ScheduleRecordsText";
                text.Name = EDIT_SCHEDULE_COLUMN_TEXT;
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSchedule.Columns.Add(text);

            }
            dataGridViewSchedule.AutoGenerateColumns = false;
            RefreshScheduleRecordDGV(requestInterface.CurrentSchedules);

            // �N���C�A���g�f�[�^�擾�pVIP�f�[�^�O���b�h�̃J������������
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "ProfileId";
                text.Name = "ProfileID";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewVipRecord.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "Key";
                text.Name = "Key";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewVipRecord.Columns.Add(text);
            }
            dataGridViewVipRecord.AutoGenerateColumns = false;
            RefreshVipRecordDGV(new List<RequestInterface.LobbyVipRecord>());


            // VIP�ݒ�p�f�[�^�O���b�h�̃J������������
            {
                CalendarColumn text = new CalendarColumn();
                text.DataPropertyName = "StartDate";
                text.Name = "�J�n��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                text.MinimumWidth = 90;
                dataGridViewVipSettingRecord.Columns.Add(text);
            }
            {
                CalendarColumn text = new CalendarColumn();
                text.DataPropertyName = "EndDate";
                text.Name = "�I����";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                text.MinimumWidth = 90;
                dataGridViewVipSettingRecord.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "ProfileId";
                text.Name = "ProfileID";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewVipSettingRecord.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "EventId";
                text.Name = "�C�x���gID";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewVipSettingRecord.Columns.Add(text);
            }
            dataGridViewVipSettingRecord.AutoGenerateColumns = false;
            RefreshVipSettingRecordDGV(new List<RequestInterface.LobbyVipSettingRecord>());



            // �X�y�V�����E�B�[�N�ݒ�pDGV��������
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionSerialNo";
                text.Name = "����ʂ��ԍ�";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNoJp";
                text.Name = "����No���{";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNoUs";
                text.Name = "����No�p��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNoFr";
                text.Name = "����No�t�����X";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNoIt";
                text.Name = "����No�C�^���A";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNoDe";
                text.Name = "����No�h�C�c";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNoSp";
                text.Name = "����No�X�y�C��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNoKr";
                text.Name = "����No�؍�";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "MultiLanguageSummarizeIdJp";
                text.Name = "�����ޓ��{";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "MultiLanguageSummarizeIdUs";
                text.Name = "�����މp��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "MultiLanguageSummarizeIdFr";
                text.Name = "�����ރt�����X";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "MultiLanguageSummarizeIdIt";
                text.Name = "�����ރC�^���A";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "MultiLanguageSummarizeIdDe";
                text.Name = "�����ރh�C�c";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "MultiLanguageSummarizeIdSp";
                text.Name = "�����ރX�y�C��";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "MultiLanguageSummarizeIdKr";
                text.Name = "�����ފ؍�";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                dataGridViewSpecialWeek.Columns.Add(text);
            }
            dataGridViewSpecialWeek.AutoGenerateColumns = false;
            RefreshSpecialWeekRecordDGV(new List<REF_PPW_LobbySpecialWeekRecord>());

            dataGridViewFreeQuestionJp.Tag = Util.Language.JP; // �^�O�ɂ͌���R�[�h���Z�b�g
            dataGridViewFreeQuestionEn.Tag = Util.Language.EN;
            dataGridViewFreeQuestionFr.Tag = Util.Language.FR;
            dataGridViewFreeQuestionIt.Tag = Util.Language.IT;
            dataGridViewFreeQuestionDe.Tag = Util.Language.DE;
            dataGridViewFreeQuestionSp.Tag = Util.Language.SP;
            dataGridViewFreeQuestionKr.Tag = Util.Language.KR;
            freeQuestionDGVs.Add(dataGridViewFreeQuestionJp);
            freeQuestionDGVs.Add(dataGridViewFreeQuestionEn);
            freeQuestionDGVs.Add(dataGridViewFreeQuestionFr);
            freeQuestionDGVs.Add(dataGridViewFreeQuestionIt);
            freeQuestionDGVs.Add(dataGridViewFreeQuestionDe);
            freeQuestionDGVs.Add(dataGridViewFreeQuestionSp);
            freeQuestionDGVs.Add(dataGridViewFreeQuestionKr);

            {
                foreach (ExtendedDataGridView dgv in freeQuestionDGVs)
                {
                    InitializeFreeQuestionDGV(dgv);
                    RefreshFreeQuestionDGV(dgv, new RequestInterface.LobbyFreeQuestionSetting((int)dgv.Tag));
                }
            }

            // �����t�p�P��f�[�^�ǂݍ���
            try
            {
                keywordIndexes.Add(new List<string>(File.ReadAllLines("data/password_jp.txt", System.Text.Encoding.GetEncoding("SJIS"))));
                keywordIndexes.Add(new List<string>(File.ReadAllLines("data/password_en.txt", System.Text.Encoding.UTF8)));
                keywordIndexes.Add(new List<string>(File.ReadAllLines("data/password_fr.txt", System.Text.Encoding.UTF8)));
                keywordIndexes.Add(new List<string>(File.ReadAllLines("data/password_it.txt", System.Text.Encoding.UTF8)));
                keywordIndexes.Add(new List<string>(File.ReadAllLines("data/password_de.txt", System.Text.Encoding.UTF8)));
                keywordIndexes.Add(new List<string>(File.ReadAllLines("data/password_sp.txt", System.Text.Encoding.UTF8)));
                keywordIndexes.Add(new List<string>(File.ReadAllLines("data/password_kr.txt", System.Text.Encoding.UTF8)));
            }
            catch (Exception exp)
            {
                MessageBox.Show("�����t�f�[�^�̓ǂݍ��ݒ��ɃG���[���������܂����B" + exp.Message
                    + "\n�f�o�b�O����ɂ�data�t�H���_������ꏊ���v���W�F�N�g�̃v���p�e�B���f�o�b�O�̍�ƃf�B���N�g���Ő�΃p�X�w�肵�Ă��������B");
                throw new Exception("�I�����܂�");
            }

            // �f�t�H���g�W�v�t���O�̏�����
            checkedListBoxDefaultSummerizeLanguage.SetItemChecked(Util.LanguageCodeToIndex(Util.Language.JP), true);
        }

        private void InitializeFreeQuestionDGV(ExtendedDataGridView dgv)
        {
            // �C�ӎ���ݒ�pDGV��������
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionNo";
                text.Name = "����ԍ�";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
                text.ReadOnly = true;
                dgv.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "QuestionSentence";
                text.Name = "����";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dgv.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "Answer1";
                text.Name = "��1";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dgv.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "Answer2";
                text.Name = "��2";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dgv.Columns.Add(text);
            }
            {
                DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
                text.DataPropertyName = "Answer3";
                text.Name = "��3";
                text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleLeft;
                dgv.Columns.Add(text);
            }
            dgv.AutoGenerateColumns = false;
            dgv.DataError += new DataGridViewDataErrorEventHandler(dataGridViewFreeQuestion_DataError);
            dgv.CellFormatting += new DataGridViewCellFormattingEventHandler(dataGridViewFreeQuestion_CellFormatting);
        }

        void dataGridViewFreeQuestion_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            ExtendedDataGridView.ShowDataError(sender, e);
            e.Cancel = false;
        }

        /// <summary>
        /// �񓯊������L�q�p
        /// </summary>
        private delegate void DelegateProc();
        /// <summary>
        /// �񓯊������p���i�[�N���X
        /// </summary>
        private class CommunicationInfo
        {
            public DelegateProc asynchronousProc;   // �񓯊��ɂ��鏈��
            public DelegateProc successedProc;      // ����������Ɏ��s���鏈��
            public DelegateProc failedProc;         // �������s��Ɏ��s���鏈��
            public DelegateProc finallyProc;        // ���������ɂ�����炸�Ō�Ɏ��s���鏈��
            public DelegateProc timerEndProc;       // �������Ԍv���I���p����
            public Exception exp;                   // �������ɔ���������O���i�[����
            public CommunicationInfo(DelegateProc _asynchronousProc, DelegateProc _successedProc, DelegateProc _failedProc, DelegateProc _finallyProc)
            {
                asynchronousProc = _asynchronousProc;
                successedProc = _successedProc;
                failedProc = _failedProc;
                finallyProc = _finallyProc;
                timerEndProc = null;
                exp = null;
            }
        }

        private bool CallAsynchronousProc(DelegateProc asynchronousProc)
        {
            return CallAsynchronousProc(asynchronousProc, null, null, null);
        }

        private bool CallAsynchronousProc(DelegateProc asynchronousProc, DelegateProc successedProc)
        {
            return CallAsynchronousProc(asynchronousProc, successedProc, null, null);
        }

        private bool CallAsynchronousProc(DelegateProc asynchronousProc, DelegateProc successedProc, DelegateProc failedProc)
        {
            return CallAsynchronousProc(asynchronousProc, successedProc, failedProc, null);
        }

        /// <summary>
        /// �񓯊��ŒʐM�������J�n����
        /// </summary>
        /// <param name="asynchronousProc">�񓯊��ōs���ʐM����</param>
        /// <param name="successedProc">�������ɍs������</param>
        /// <param name="failedProc">���s���ɍs������</param>
        /// <param name="finallyProc">�K���Ō�Ɏ��s���鏈��</param>
        /// <returns>True:�J�n���ꂽ�B False:�J�n����Ȃ������B</returns>
        private bool CallAsynchronousProc(DelegateProc asynchronousProc, DelegateProc successedProc, DelegateProc failedProc, DelegateProc finallyProc)
        {
            if (backgroundWorkerThread.IsBusy)
            {
                if (failedProc != null)
                {
                    failedProc();
                }
                if (finallyProc != null)
                {
                    finallyProc();
                }
                MessageBox.Show("�O��̏��������s���ł��B", "�G���[");
                return false;
            }

            // �����[�X�T�[�o�Ƀ��N�G�X�g�𑗂��Ă������̊m�F
            if (requestInterface.CurrentServer.Kind == RequestInterface.ServerInfo.KIND.RELEASE)
            {
                if (MessageBox.Show("�����[�X�T�[�o�Ƀ��N�G�X�g�𑗂�܂��B�{���ɂ�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
                {
                    if (failedProc != null)
                    {
                        failedProc();
                    }
                    if (finallyProc != null)
                    {
                        finallyProc();
                    }
                    return false;
                }
            }

            CommunicationInfo info = new CommunicationInfo(asynchronousProc, successedProc, failedProc, finallyProc);
            Stopwatch sw = new Stopwatch();
            toolStripStatusLabel.Text = "�ʐM��";
            Update();

            sw.Start();

            info.timerEndProc = delegate
            {
                sw.Stop();
                string text = defaultStatusText;
                if (!sw.IsRunning)
                {
                    text += " " + sw.ElapsedMilliseconds + "ms";
                }
                toolStripStatusLabel.Text = text;
            };

            backgroundWorkerThread.RunWorkerAsync(info);

            return true;
        }

        private void backgroundWorkerGetServerState_DoWork(object sender, DoWorkEventArgs e)
        {
            CommunicationInfo info = (CommunicationInfo)e.Argument;
            try
            {
                info.asynchronousProc();
            }
            catch (Exception exp)
            {
                info.exp = exp;
            }
            e.Result = info;
        }

        private void backgroundWorkerGetServerState_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            CommunicationInfo info = (CommunicationInfo)e.Result;

            info.timerEndProc();

            if (info.exp != null)
            {
                if (info.failedProc != null)
                {
                    info.failedProc();
                }
            }
            else
            {
                if (info.successedProc != null)
                {
                    info.successedProc();
                }
            }

            if (info.finallyProc != null)
            {
                info.finallyProc();
            }

            if (info.exp != null)
            {
#if DEBUG
                MessageBox.Show("�ʐM���������͒ʐM�������ɃG���[���������܂����B\n" + info.exp.ToString(), "�G���[");
#else
                MessageBox.Show("�ʐM���������͒ʐM�������ɃG���[���������܂����B\n" + info.exp.ToString(), "�G���[");
                //MessageBox.Show("�ʐM���������͒ʐM�������ɃG���[���������܂����B\n", "�G���[");
#endif
                // �Đڑ�����
                requestInterface.Reconnect();
            }
        }

        private void ToolStripMenuItemExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ToolStripMenuItemVersion_Click(object sender, EventArgs e)
        {
            string appName = global::AdminTool.Properties.Resources.AppName;
            string version = global::AdminTool.Properties.Resources.VersionString;
            MessageBox.Show(appName + " ver " + version, "�o�[�W�������");
        }

        private void ToolStripMenuItemDebugServer_Click(object sender, EventArgs e)
        {
            requestInterface.ChangeServer(RequestInterface.ServerInfo.KIND.DEBUG);
            ToolStripMenuItemDebugServer.Checked = true;
            ToolStripMenuItemReleaseServer.Checked = false;
            ToolStripMenuItemDevDebugServer.Checked = false;
            ToolStripMenuItemDevReleaseServer.Checked = false;
            Text = global::AdminTool.Properties.Resources.AppName + " - " + requestInterface.CurrentServer.Name;
        }

        private void ToolStripMenuItemReleaseServer_Click(object sender, EventArgs e)
        {
            requestInterface.ChangeServer(RequestInterface.ServerInfo.KIND.RELEASE);
            ToolStripMenuItemDebugServer.Checked = false;
            ToolStripMenuItemReleaseServer.Checked = true;
            ToolStripMenuItemDevDebugServer.Checked = false;
            ToolStripMenuItemDevReleaseServer.Checked = false;
            Text = global::AdminTool.Properties.Resources.AppName + " - " + requestInterface.CurrentServer.Name;
        }

        private void ToolStripMenuItemDevDebugServer_Click(object sender, EventArgs e)
        {
            requestInterface.ChangeServer(RequestInterface.ServerInfo.KIND.DEV_DEBUG);
            ToolStripMenuItemDebugServer.Checked = false;
            ToolStripMenuItemReleaseServer.Checked = false;
            ToolStripMenuItemDevDebugServer.Checked = true;
            ToolStripMenuItemDevReleaseServer.Checked = false;
            Text = global::AdminTool.Properties.Resources.AppName + " - " + requestInterface.CurrentServer.Name;
        }

        private void ToolStripMenuItemDevReleaseServer_Click(object sender, EventArgs e)
        {
            requestInterface.ChangeServer(RequestInterface.ServerInfo.KIND.DEV_RELEASE);
            ToolStripMenuItemDebugServer.Checked = false;
            ToolStripMenuItemReleaseServer.Checked = false;
            ToolStripMenuItemDevDebugServer.Checked = false;
            ToolStripMenuItemDevReleaseServer.Checked = true;
            Text = global::AdminTool.Properties.Resources.AppName + " - " + requestInterface.CurrentServer.Name;
        }

        private void buttonGetScheduleExecute_Click(object sender, EventArgs e)
        {
            RequestInterface.LobbyGetScheduleResponse res = null;
            string message = "";

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                res = requestInterface.LobbyGetSchedule(out message);
            },
            delegate
            {
                // ������
                if (res.code == 0)
                {
                    textBoxAdminResult.Text = "code: " + res.code;
                    listViewUserSchedule.Items.Clear();
                    listViewUserSchedule.Items.AddRange(res.ToListViewItems());
                }
                else
                {
                    textBoxAdminResult.Text = "code: " + res.code + ", message:" + message;
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void buttonGetVipExecute_Click(object sender, EventArgs e)
        {
            RequestInterface.LobbyGetVipResponse res = null;
            string message = "";
            uint trainerId = 0;

            try
            {
                trainerId = UInt32.Parse(textBoxVipClientTrainerId.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("�t�����h�R�[�h��������ProfileID�������̓��[�UID�����������͂���Ă��܂���B");
                return;
            }

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                res = requestInterface.LobbyGetVip(trainerId, out message);
            },
            delegate
            {
                // ������
                if (res.code == 0)
                {
                    textBoxAdminResult.Text = "code: " + res.code;
                    RefreshVipRecordDGV(res.vipRecords);
                }
                else
                {
                    textBoxAdminResult.Text = "code: " + res.code + ", message:" + message;
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void RefreshScheduleRecordDGV(RequestInterface.LobbySchedules src)
        {
            BindingSource bindingSource = new BindingSource();
            bindingSource.AllowNew = true;
            bindingSource.DataSource = src.lobbySchedules;
            dataGridViewSchedule.DataSource = bindingSource;
            bindingNavigatorSchedule.BindingSource = bindingSource;
        }

        private void RefreshVipRecordDGV(List<RequestInterface.LobbyVipRecord> src)
        {
            BindingSource bindingSource = new BindingSource();
            bindingSource.AllowNew = false;
            bindingSource.DataSource = src;
            dataGridViewVipRecord.DataSource = bindingSource;
        }

        private void RefreshVipSettingRecordDGV(List<RequestInterface.LobbyVipSettingRecord> src)
        {
            BindingSource bindingSource = new BindingSource();
            bindingSource.AllowNew = true;
            //src.Sort();
            bindingSource.DataSource = src;
            dataGridViewVipSettingRecord.DataSource = bindingSource;
            bindingNavigatorVipSettingRecord.BindingSource = bindingSource;
        }


        private void RefreshSpecialWeekRecordDGV(List<REF_PPW_LobbySpecialWeekRecord> src)
        {
            BindingSource bindingSource = new BindingSource();
            bindingSource.AllowNew = true;
            bindingSource.DataSource = src;
            dataGridViewSpecialWeek.DataSource = bindingSource;
            bindingNavigatorSpecialWeek.BindingSource = bindingSource;
        }

        private void RefreshFreeQuestionDGV(DataGridView dgv, RequestInterface.LobbyFreeQuestionSetting lobbyFreeQuestionSetting)
        {
            BindingSource bindingSource = new BindingSource();
            bindingSource.AllowNew = false;
            bindingSource.DataSource = lobbyFreeQuestionSetting.records;
            dgv.DataSource = bindingSource;
        }

        private void dataGridViewVipSettingRecord_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            dataGridViewVipSettingRecord.CommitEdit(DataGridViewDataErrorContexts.Commit);
            DataGridView dgv = (DataGridView)sender;
            BindingSource src = (BindingSource)dataGridViewVipSettingRecord.DataSource;
            List<RequestInterface.LobbyVipSettingRecord> list = (List<RequestInterface.LobbyVipSettingRecord>)src.DataSource;
            RequestInterface.LobbyVipSettingRecord temp = null;

            int newRowIndex = dgv.NewRowIndex;
            if (newRowIndex >= 0 && newRowIndex < list.Count)
            {
                // �V�K�s�̓\�[�g���Ȃ����ߑҔ�
                temp = list[newRowIndex];
                list.RemoveAt(newRowIndex);
            }

            list.Sort();
            
            if (temp != null)
            {
                list.Add(temp);
            }
            
            RefreshVipSettingRecordDGV(list);
        }

        private void dataGridViewVipSettingRecord_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            ExtendedDataGridView.ShowDataError(sender, e);
            e.Cancel = false;
        }

        public int GenerateVipKey(uint clientTrainerId, int vipPid, int eventId)
        {
            // �C�x���gID��0�̂Ƃ��͍����t����
            if (eventId == 0)
            {
                return 0;
            }
            List<byte> bytes = new List<byte>();
            bytes.AddRange(BitConverter.GetBytes((ushort)clientTrainerId));         // ����2byte
            bytes.AddRange(BitConverter.GetBytes(Util.ProfileIdToUserId(vipPid)));  // ����2byte
            bytes.AddRange(BitConverter.GetBytes(eventId));

            string base64string = Convert.ToBase64String(bytes.ToArray());
            byte[] byteBase64Array = Encoding.Unicode.GetBytes(base64string);
            List<byte> slimByteBase64Array = new List<byte>();
            // ���ʂȌ��Ԃ��󂢂Ă�̂ŏk�߂�
            for (int i = 0; i < byteBase64Array.Length; i++)
            {
                if (i % 2 != 0)
                {
                    continue;
                }
                slimByteBase64Array.Add(byteBase64Array[i]);
            }

            System.Security.Cryptography.SHA1 sha = new System.Security.Cryptography.SHA1CryptoServiceProvider();
            byte[] hash = sha.ComputeHash(slimByteBase64Array.ToArray());
            int key = hash[0] << 24 | hash[5] << 16 | hash[10] << 8 | hash[15];
            // �L�[��0�̂Ƃ��͍����t�������Ӗ�����̂�1�ɒu��������
            if (key == 0)
            {
                key = 1;
            }
            return key;
        }

        private void buttonVipConfirmExecute_Click(object sender, EventArgs e)
        {
            ShowPassword();
        }

        private void ShowPassword()
        {

            uint clientTrainerId = 0;
            int vipPid = 0;
            int eventId = 0;

            try
            {
                clientTrainerId = UInt32.Parse(textBoxVipConfirmClientTrainerId.Text);
                string vipFrinedCodeText = textBoxVipConfirmVipPid.Text.Replace("-", "");
                if (vipFrinedCodeText.Length > 12)
                {
                    throw new Exception("�������傫�����܂��B");
                }
                vipPid = Util.FriendCodeToProfileId(UInt64.Parse(vipFrinedCodeText));
                eventId = Int32.Parse(textBoxVipConfirmEventId.Text);
            }
            catch (Exception)
            {
                labelVipConfirmWarning.Visible = true;
                return;
            }
            labelVipConfirmWarning.Visible = false;

            int key = GenerateVipKey(clientTrainerId, vipPid, eventId);

            textBoxVipConfirmKey.Text = key.ToString();
            byte[] keyBytes = BitConverter.GetBytes(key);
            textBoxVipConfirmA.Text = keyBytes[0].ToString();
            textBoxVipConfirmB.Text = keyBytes[1].ToString();
            textBoxVipConfirmC.Text = keyBytes[2].ToString();
            textBoxVipConfirmD.Text = keyBytes[3].ToString();
            int _1 = keyBytes[3] + keyBytes[0];
            int _2 = keyBytes[0] + keyBytes[1];
            int _3 = keyBytes[1] + keyBytes[2];
            int _4 = keyBytes[2] + keyBytes[3];
            textBoxVipConfirm1.Text = _1.ToString();
            textBoxVipConfirm2.Text = _2.ToString();
            textBoxVipConfirm3.Text = _3.ToString();
            textBoxVipConfirm4.Text = _4.ToString();

            textBoxVipConfirmPasswordJP.Text = GeneratePassword(_1, _2, _3, _4, Util.Language.JP);
            textBoxVipConfirmPasswordEN.Text = GeneratePassword(_1, _2, _3, _4, Util.Language.EN);
            textBoxVipConfirmPasswordFR.Text = GeneratePassword(_1, _2, _3, _4, Util.Language.FR);
            textBoxVipConfirmPasswordIT.Text = GeneratePassword(_1, _2, _3, _4, Util.Language.IT);
            textBoxVipConfirmPasswordDE.Text = GeneratePassword(_1, _2, _3, _4, Util.Language.DE);
            textBoxVipConfirmPasswordSP.Text = GeneratePassword(_1, _2, _3, _4, Util.Language.SP);
            textBoxVipConfirmPasswordKR.Text = GeneratePassword(_1, _2, _3, _4, Util.Language.KR);
        }

        private string GeneratePassword(int _1, int _2, int _3, int _4, Util.Language language)
        {
            if ((_1 | _2 | _3 | _4) == 0)
            {
                return "�C�x���gID��0�̂��ߍ����t�͂���܂���B";
            }
            int languageIndex = Util.LanguageCodeToIndex(language);
            int wordNum = keywordIndexes[(int)languageIndex].Count;

            if (wordNum == 0)
            {
                return "�P�ꃊ�X�g����ł��B";
            }
            return keywordIndexes[(int)languageIndex][_1 % wordNum] + "  "
            + keywordIndexes[(int)languageIndex][_2 % wordNum] + "  "
            + keywordIndexes[(int)languageIndex][_3 % wordNum] + "  "
            + keywordIndexes[(int)languageIndex][_4 % wordNum];
        }

        private void textBoxTransFriendCode_TextChanged(object sender, EventArgs e)
        {
            UInt64 friendCode = 0;
            try
            {
                string text = textBoxTransFriendCode.Text;
                if (text == "")
                {
                    return;
                }
                text = text.Replace("-", "");
                if (text.Length > 12)
                {
                    labelTransInvalidFriendCode.Visible = true;
                    return;
                }
                friendCode = UInt64.Parse(text);
            }
            catch (Exception)
            {
                labelTransInvalidFriendCode.Visible = true;
                return;
            }

            labelTransInvalidFriendCode.Visible = false;
            textBoxTransProfileId.Text = Util.FriendCodeToProfileId(friendCode).ToString();
        }

        private void textBoxTransProfileId_TextChanged(object sender, EventArgs e)
        {
            int profileId = 0;
            try
            {
                string text = textBoxTransProfileId.Text;
                if (text == "")
                {
                    return;
                }
                profileId = Int32.Parse(text);
            }
            catch (Exception)
            {
                labelTransInvalidProfileId.Visible = true;
                return;
            }

            if (profileId < 100000)
            {
                labelTransInvalidProfileId.Visible = true;
                return;
            }

            labelTransInvalidProfileId.Visible = false;
            textBoxTransUserId.Text = Util.ProfileIdToUserId(profileId).ToString();
        }

        private void textBoxVipConfirmClientPid_TextChanged(object sender, EventArgs e)
        {
            ShowPassword();
        }

        private void textBoxVipConfirmVipPid_TextChanged(object sender, EventArgs e)
        {
            try
            {
                UInt64 friendCode = UInt64.Parse(((TextBox)sender).Text.Replace("-", ""));
                Int32 profileId = Util.FriendCodeToProfileId(friendCode);
                UInt16 userId = Util.ProfileIdToUserId(profileId);
                textBoxVipConfirmVipPid_UserId.Text = userId.ToString();
            }
            catch (Exception)
            {
                // �Ȃɂ����Ȃ�
            }
            ShowPassword();
        }

        private void textBoxVipConfirmEventId_TextChanged(object sender, EventArgs e)
        {
            ShowPassword();
        }

        private void textBoxVipConfirmClientPid_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                ((TextBox)sender).SelectAll();
            }
        }

        private void textBoxVipConfirmVipPid_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                ((TextBox)sender).SelectAll();
            }
        }

        private void textBoxVipConfirmEventId_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                ((TextBox)sender).SelectAll();
            }
        }

        private void ToolStripMenuItemProxySetting_Click(object sender, EventArgs e)
        {
            // �q�X�g����������ΐ���
            if (settings.ProxyServerHistory == null)
            {
                settings.ProxyServerHistory = new System.Collections.Specialized.StringCollection();
            }
            ProxySettingForm form = new ProxySettingForm(requestInterface.ProxyServer, settings.ProxyServerHistory);
            form.ShowDialog();
            if (form.EndOk)
            {
                settings.ProxyServer = form.ResultProxyServer;
                requestInterface.ProxyServer = form.ResultProxyServer;

                // �q�X�g���ɒǉ�
                if (form.ResultProxyServer != "" && !settings.ProxyServerHistory.Contains(form.ResultProxyServer))
                {
                    settings.ProxyServerHistory.Add(form.ResultProxyServer);
                }
            }
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            settings.Save();
        }

        private void dataGridViewSchedule_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;

            // �w�b�_�̃N���b�N�͖���
            if (e.RowIndex < 0)
            {
                return;
            }

            // "�C�x���g��"��Ȃ�΁A�{�^�����N���b�N���ꂽ
            if (dgv.Columns[e.ColumnIndex].Name == EDIT_SCHEDULE_COLUMN_TEXT)
            {
                // �X�P�W���[�����R�[�h�ҏW�p�t�H�[���\��
                RequestInterface.LobbySchedules schedule = requestInterface.CurrentSchedules;
                BindingSource src = (BindingSource)dgv.DataSource;
                bool isNewRow = dataGridViewSchedule.Rows[e.RowIndex].IsNewRow;
                if (isNewRow)
                {
                    // �V�K�s�̏�Ԃ��ƕҏW��ɍs�������Ă��܂��̂ň�[�f�^�b�`���Ď蓮�ōs��ǉ�����B
                    dgv.DataSource = null;
                    schedule.lobbySchedules.Add(new RequestInterface.LobbyAdminSetScheduleRequestPart());
                    RefreshScheduleRecordDGV(schedule);
                }
                List<RequestInterface.LobbyScheduleRecord> records = requestInterface.CurrentSchedules.lobbySchedules[e.RowIndex].scheduleRecords;
                EditRecordForm form = new EditRecordForm(records);
                form.ShowDialog();
                if (!form.EndOk)
                {
                    // �蓮�Œǉ������Ō�̍s���폜����B
                    if(isNewRow)
                    {
                        dgv.DataSource = null;
                        schedule.lobbySchedules.RemoveAt(schedule.lobbySchedules.Count - 1);
                        RefreshScheduleRecordDGV(schedule);
                    }
                    return;
                }

                records.Sort();
                RefreshScheduleRecordDGV(schedule);
            }
        }

        private void dataGridViewSchedule_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;
            
            // �Z���̗���m�F
            if (e.ColumnIndex >= 0 && e.RowIndex >= 0
                && dgv.Columns[e.ColumnIndex].Name == EDIT_SCHEDULE_COLUMN_TEXT
                && (e.PaintParts & DataGridViewPaintParts.ContentForeground) == DataGridViewPaintParts.ContentForeground)
            {
                string countText = "0";
                if (requestInterface.CurrentSchedules.lobbySchedules.Count > e.RowIndex)
                {
                    countText = requestInterface.CurrentSchedules.lobbySchedules[e.RowIndex].scheduleRecords.Count.ToString();
                }
                string text = countText + ":�ҏW...";

                // �O�i�ȊO���`�悳���悤�ɂ���
                DataGridViewPaintParts paintParts = e.PaintParts & ~DataGridViewPaintParts.ContentForeground;

                // �Z����`�悷��
                e.Paint(e.ClipBounds, paintParts);

                // �X�P�W���[���ҏW�p�ɕ\���e�L�X�g�������I�ɕ`�悷��B
                e.Graphics.DrawString(text, dgv.Font, new SolidBrush(Color.DarkSlateBlue), new Point(e.CellBounds.Left+3, e.CellBounds.Top+4));

                // �`�悪�����������Ƃ�m�点��
                e.Handled = true;
            }
        }

        private void dataGridViewSchedule_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            ExtendedDataGridView.ShowDataError(sender, e);
            e.Cancel = false;
        }

        private void dataGridViewSchedule_ColumnHeaderMouseClick(object sender, DataGridViewCellMouseEventArgs e)
        {
            dataGridViewSchedule.CommitEdit(DataGridViewDataErrorContexts.Commit);
            RequestInterface.LobbySchedules schedules = requestInterface.CurrentSchedules;
            RequestInterface.LobbyAdminSetScheduleRequestPart temp = null;
            int newRowIndex = dataGridViewSchedule.NewRowIndex;
            if (newRowIndex >= 0 && newRowIndex < schedules.lobbySchedules.Count)
            {
                // �V�K�s�̓\�[�g���Ȃ����ߑҔ�
                temp = schedules.lobbySchedules[newRowIndex];
                schedules.lobbySchedules.RemoveAt(newRowIndex);
            }

            schedules.Sort();

            if (temp != null)
            {
                schedules.lobbySchedules.Add(temp);
            }
            RefreshScheduleRecordDGV(schedules);
        }

        private void toolStripButtonLoadScheduleSetting_Click(object sender, EventArgs e)
        {
            openFileDialogSettingXML.Filter = "Lobby Schedule files (*.xml)|*.xml|All files (*.*)|*.*";
            openFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (openFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                RequestInterface.LobbySchedules temp = RequestInterface.LoadXML<RequestInterface.LobbySchedules>(openFileDialogSettingXML.FileName);
                if (temp == null)
                {
                    return;
                }
                requestInterface.CurrentSchedules = temp;
                RefreshScheduleRecordDGV(requestInterface.CurrentSchedules);
            }
        }

        private void toolStripButtonSaveScheduleSetting_Click(object sender, EventArgs e)
        {
            dataGridViewSchedule.CommitEdit(DataGridViewDataErrorContexts.Commit);
            saveFileDialogSettingXML.Filter = "Lobby Schedule files (*.xml)|*.xml|All files (*.*)|*.*";
            saveFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (saveFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                RequestInterface.SaveXML<RequestInterface.LobbySchedules>(saveFileDialogSettingXML.FileName, requestInterface.CurrentSchedules);
            }
        }

        private void toolStripButtonCopySchedule_Click(object sender, EventArgs e)
        {
            if (dataGridViewSchedule.SelectedRows.Count == 0)
            {
                MessageBox.Show("��ԍ��̕���(�J�n�N�̍����̗�)���N���b�N���ĕ����������s��I�����Ă��������B\n"
                    + "Ctrl�������Ȃ���N���b�N���邱�Ƃŕ����̃X�P�W���[���𕡐��ł��܂��B");
                return;
            }

            List<RequestInterface.LobbyAdminSetScheduleRequestPart> copiedScheudles = new List<RequestInterface.LobbyAdminSetScheduleRequestPart>();

            // �I������Ă���s�𒲂ׂ�
            foreach (DataGridViewRow row in dataGridViewSchedule.SelectedRows)
            {
                if (row.IsNewRow)
                {
                    MessageBox.Show("�V�K�s(*�����Ă���s)�͕����ł��܂���B");
                    return;
                }
                copiedScheudles.Add((RequestInterface.LobbyAdminSetScheduleRequestPart)requestInterface.CurrentSchedules.lobbySchedules[row.Index].Clone());
            }

            copiedScheudles.Sort();

            requestInterface.CurrentSchedules.lobbySchedules.AddRange(copiedScheudles);
            RefreshScheduleRecordDGV(requestInterface.CurrentSchedules);
        }

        private void toolStripButtonAdminGetScheduleExecute_Click(object sender, EventArgs e)
        {
            if (requestInterface.CurrentSchedules.lobbySchedules.Count != 0)
            {
                if (MessageBox.Show("���ݕҏW���̃X�P�W���[����j�����ăT�[�o����ǂݍ���ł�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
                {
                    return;
                }
            }

            RequestInterface.LobbySchedules res = null;
            uint code = 0;
            string message = "";

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                res = requestInterface.LobbyAdminGetSchedule(out code, out message);
            },
            delegate
            {
                // ������
                if (code == 0)
                {
                    textBoxAdminResult.Text = "code: " + code;
                    requestInterface.CurrentSchedules = res;
                    RefreshScheduleRecordDGV(requestInterface.CurrentSchedules);
                }
                else
                {
                    textBoxAdminResult.Text = "code: " + code + ", message:" + message;
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void toolStripButtonAdminSetScheduleExecute_Click(object sender, EventArgs e)
        {
            dataGridViewSchedule.CommitEdit(DataGridViewDataErrorContexts.Commit);
            RequestInterface.LobbySchedules schedules = requestInterface.CurrentSchedules;
            string message = "";
            uint code = 0;

            RequestInterface.LobbySchedules clonedSchedules = (RequestInterface.LobbySchedules)schedules.Clone();

            int newRowIndex = dataGridViewSchedule.NewRowIndex;
            if (newRowIndex >= 0 && newRowIndex < clonedSchedules.lobbySchedules.Count)
            {
                // �V�K�s���폜
                clonedSchedules.lobbySchedules.RemoveAt(newRowIndex);
            }

            clonedSchedules.Sort();

            if (clonedSchedules.lobbySchedules.Count == 0)
            {
                MessageBox.Show("�X�P�W���[������ł��B");
                return;
            }

            if (!clonedSchedules.IsConsistent())
            {
                MessageBox.Show("�s���ȃX�P�W���[��������܂��B�Ԃ��Ȃ��Ă���X�P�W���[�����C�����Ă��������B\n"
                              + "�E�����̍��v��100�ɂȂ�悤�ɂ��Ă��������B\n"
                              + "�E�J�n����2000�N����2200�N�̊Ԃɂ��Ă��������B\n"
                              + "�E��Ԗڂ̃X�P�W���[���̊J�n�������݂��ߋ��ɂ��Ă��������B\n"
                              + "�E�C�x���g����40�ȉ��ɂ��Ă��������B");
                return;
            }

            if (MessageBox.Show("���̃X�P�W���[�����T�[�o�ɐݒ肵�Ă�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
            {
                return;
            }

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                requestInterface.LobbyAdminSetSchedule(clonedSchedules, out code, out message);
            },
            delegate
            {
                // ������
                textBoxAdminResult.Text = "code: " + code + (code == 0 ? "" : ", message:" + message);
                if (code == 0)
                {
                    MessageBox.Show("�������܂����B");
                }
                else
                {
                    MessageBox.Show("���s���܂����B");
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void toolStripButtonLoadVipSetting_Click(object sender, EventArgs e)
        {
            openFileDialogSettingXML.Filter = "Vip Setting files (*.xml)|*.xml|All files (*.*)|*.*";
            openFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (openFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                RequestInterface.LobbyAdminSetVipSettingRequest req = RequestInterface.LoadXML<RequestInterface.LobbyAdminSetVipSettingRequest>(openFileDialogSettingXML.FileName);
                if (req == null)
                {
                    return;
                }
                RefreshVipSettingRecordDGV(req.vipSettingRecords);
            }
        }

        private void toolStripButtonSaveVipSetting_Click(object sender, EventArgs e)
        {
            dataGridViewVipSettingRecord.CommitEdit(DataGridViewDataErrorContexts.Commit);
            saveFileDialogSettingXML.Filter = "Vip Setting files (*.xml)|*.xml|All files (*.*)|*.*";
            saveFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (saveFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                // ���݂̐ݒ�����N�G�X�g�Ƃ��Ď擾����XML��
                BindingSource src = (BindingSource)dataGridViewVipSettingRecord.DataSource;
                RequestInterface.LobbyAdminSetVipSettingRequest temp =
                    new RequestInterface.LobbyAdminSetVipSettingRequest(RequestInterface.ADMIN_PROFILE_ID,
                        (List<RequestInterface.LobbyVipSettingRecord>)src.DataSource);
                RequestInterface.SaveXML<RequestInterface.LobbyAdminSetVipSettingRequest>(saveFileDialogSettingXML.FileName, temp);
            }
        }

        private void toolStripButtonCopyVipRecords_Click(object sender, EventArgs e)
        {
            if (dataGridViewVipSettingRecord.SelectedRows.Count == 0)
            {
                MessageBox.Show("��ԍ��̕���(�J�n�N�̍����̗�)���N���b�N���ĕ����������s��I�����Ă��������B\n"
                    + "Ctrl�������Ȃ���N���b�N���邱�Ƃŕ����̃X�P�W���[���𕡐��ł��܂��B");
                return;
            }

            BindingSource src = (BindingSource)dataGridViewVipSettingRecord.DataSource;
            List<RequestInterface.LobbyVipSettingRecord> list = (List<RequestInterface.LobbyVipSettingRecord>)src.DataSource;
            List<RequestInterface.LobbyVipSettingRecord> copiedVips = new List<RequestInterface.LobbyVipSettingRecord>();

            // �I������Ă���s�𒲂ׂ�
            foreach (DataGridViewRow row in dataGridViewVipSettingRecord.SelectedRows)
            {
                if (row.IsNewRow)
                {
                    MessageBox.Show("�V�K�s(*�����Ă���s)�͕����ł��܂���B");
                    return;
                }
                copiedVips.Add((RequestInterface.LobbyVipSettingRecord)list[row.Index].Clone());
            }

            copiedVips.Sort();

            list.AddRange(copiedVips);
            RefreshVipSettingRecordDGV(list);
        }

        private void toolStripButtonAdminGetVipSettingRecordExecute_Click(object sender, EventArgs e)
        {
            BindingSource src = (BindingSource)dataGridViewVipSettingRecord.DataSource;
            List<RequestInterface.LobbyVipSettingRecord> list = (List<RequestInterface.LobbyVipSettingRecord>)src.DataSource;
            if (list.Count != 0)
            {
                if (MessageBox.Show("���ݕҏW����VIP�ݒ胊�X�g��j�����ăT�[�o����ǂݍ���ł�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
                {
                    return;
                }
            }

            RequestInterface.LobbyAdminGetVipSettingResponse res = null;
            string message = "";

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                res = requestInterface.LobbyAdminGetVipSetting(out message);
            },
            delegate
            {
                // ������
                if (res.code == 0)
                {
                    textBoxAdminResult.Text = "code: " + res.code;
                    RefreshVipSettingRecordDGV(res.vipSettingRecords);
                }
                else
                {
                    textBoxAdminResult.Text = "code: " + res.code + ", message:" + message;
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void toolStripButtonAdminSetVipSettingExecute_Click(object sender, EventArgs e)
        {
            dataGridViewVipSettingRecord.CommitEdit(DataGridViewDataErrorContexts.Commit);
            BindingSource src = (BindingSource)dataGridViewVipSettingRecord.DataSource;
            List<RequestInterface.LobbyVipSettingRecord> list = (List<RequestInterface.LobbyVipSettingRecord>)src.DataSource;
            List<RequestInterface.LobbyVipSettingRecord> clonedList = new List<RequestInterface.LobbyVipSettingRecord>(list);

            // �V�K�s�͍폜
            int newRowIndex = dataGridViewVipSettingRecord.NewRowIndex;
            if (newRowIndex >= 0 && newRowIndex < list.Count)
            {
                clonedList.RemoveAt(newRowIndex);
            }

            RequestInterface.LobbyAdminSetVipSettingRequest req =
                new RequestInterface.LobbyAdminSetVipSettingRequest(RequestInterface.ADMIN_PROFILE_ID, clonedList);

            string message = "";
            uint code = 0;

            if (!req.IsConsistent())
            {
                MessageBox.Show("�s����VIP���R�[�h������܂��B�Ԃ��Ȃ��Ă���VIP���R�[�h���C�����Ă��������B\n"
                              + "�E�J�n���ƏI������2000�N����2200�N�̊Ԃɂ��Ă��������B\n"
                              + "�E�J�n�����I�������O�ɂ��Ă��������B\n"
                              + "�EProfileID��10���ȏ�ɂ��Ă��������B");
                return;
            }

            if (MessageBox.Show("����VIP�ݒ胊�X�g���T�[�o�ɐݒ肵�Ă�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
            {
                return;
            }

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                requestInterface.LobbyAdminSetVipSettingRecord(req, out code, out message);
            },
            delegate
            {
                // ������
                textBoxAdminResult.Text = "code: " + code + (code == 0 ? "" : ", message:" + message);
                if (code == 0)
                {
                    MessageBox.Show("�������܂����B");
                }
                else
                {
                    MessageBox.Show("���s���܂����B");
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void dataGridViewSchedule_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;

            if (dgv.Rows[e.RowIndex].IsNewRow)
            {
                return;
            }

            if (!requestInterface.CurrentSchedules.lobbySchedules[e.RowIndex].IsConsistent())
            {
                // ���������Ƃ�Ă��Ȃ��s��Ԃ�����
                e.CellStyle.BackColor = Color.LightPink;
            }
        }

        private void dataGridViewVipSettingRecord_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;

            if (dgv.Rows[e.RowIndex].IsNewRow)
            {
                return;
            }

            BindingSource src = (BindingSource)dgv.DataSource;
            List<RequestInterface.LobbyVipSettingRecord> list = (List<RequestInterface.LobbyVipSettingRecord>)src.DataSource;
            if (!list[e.RowIndex].IsConsistent())
            {
                // ���������Ƃ�Ă��Ȃ��s��Ԃ�����
                e.CellStyle.BackColor = Color.LightPink;
            }
        }

        private void buttonCheckProfile_Click(object sender, EventArgs e)
        {
            List<UInt16> list;

            Int32 userId = 0;
            UInt32 trainerId = 0;
            string name = null;
            byte sex = 0;
            byte language = 0;
            UInt16 trainerType = 0;
            UInt16 country = 0;
            byte region = 0;
            byte romCode = 0;

            try
            {
                userId = Int32.Parse(textBoxCheckProfileUserId.Text);
                trainerId = UInt32.Parse(textBoxCheckProfileTrainerId.Text);
                try
                {
                    name = DpwUtil.Tools.UnicodeToPokecode(textBoxCheckProfileName.Text);
                }
                catch (Exception)
                {
                    throw new Exception("�|�P�����R�[�h�ɕϊ��ł��Ȃ�������������܂����B");
                }
                sex = byte.Parse(textBoxCheckProfileSex.Text);
                language = byte.Parse(textBoxCheckProfileLanguage.Text);
                trainerType = UInt16.Parse(textBoxCheckProfileTrainerType.Text);
                country = UInt16.Parse(textBoxCheckProfileCountry.Text);
                region = byte.Parse(textBoxCheckProfileRegion.Text);
                romCode = byte.Parse(textBoxCheckProfileRomCode.Text);

                list = Util.BytesToUShorts(new List<byte>(Encoding.Unicode.GetBytes(name)), false);
            }
            catch (Exception err)
            {
                MessageBox.Show("���͂��ꂽ�p�����[�^���s���ł��B" + err.Message);
                return;
            }

            REF_WFLBY_USER_PROFILE profile = new REF_WFLBY_USER_PROFILE(userId, trainerId, list, sex, language, trainerType, country, region, romCode);
            RequestInterface.LobbyAdminCheckProfileRequest req = new RequestInterface.LobbyAdminCheckProfileRequest(RequestInterface.ADMIN_PROFILE_ID, profile);
            RequestInterface.LobbyAdminCheckProfileResponse res = null;

            string message = "";
            uint code = 0;

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                res = requestInterface.LobbyAdminCheckProfile(req, out code, out message);
            },
            delegate
            {
                // ������
                textBoxAdminResult.Text = "code: " + code + (code == 0 ? "" : ", message:" + message);
                if (code == 0)
                {
                    if (res == null)
                    {
                        throw new Exception("�T�[�o����Ԃ��ꂽ�f�[�^���s���ł��B");
                    }

                    REF_WFLBY_USER_PROFILE resultProfile = res.profile;

                    textBoxCheckProfileUserIdResult.Text = resultProfile.userid.ToString();
                    textBoxCheckProfileTrainerIdResult.Text = resultProfile.trainerid.ToString();
                    List<byte> bytes = Util.UShortsToBytes(resultProfile.name, false);
                    textBoxCheckProfileNameResult.Text = DpwUtil.Tools.PokecodeToUnicode(Encoding.Unicode.GetString(bytes.ToArray()));
                    textBoxCheckProfileSexResult.Text = resultProfile.sex.ToString();
                    textBoxCheckProfileLanguageResult.Text = resultProfile.region_code.ToString();
                    textBoxCheckProfileTrainerTypeResult.Text = resultProfile.tr_type.ToString();
                    textBoxCheckProfileCountryResult.Text = resultProfile.nation.ToString();
                    textBoxCheckProfileRegionResult.Text = resultProfile.area.ToString();
                    textBoxCheckProfileRomCodeResult.Text = resultProfile.rom_code.ToString();
                }
                else
                {
                    MessageBox.Show("�s���`�F�b�N�ŕs���ȃp�����[�^�����o����܂����B");
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void buttonGetQuestionnaire_Click(object sender, EventArgs e)
        {
            RequestInterface.LobbyGetQuestionnaireRequest req = new RequestInterface.LobbyGetQuestionnaireRequest(RequestInterface.ADMIN_PROFILE_ID);
            RequestInterface.LobbyGetQuestionnaireResponse res = null;
            string message = "";
            uint code = 0;

            try
            {
                req.language = byte.Parse(textBoxGetQuestionnaireLanguage.Text);
            }
            catch (Exception)
            {
                MessageBox.Show("����R�[�h�����������͂���Ă��܂���B");
                return;
            }

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                res = requestInterface.LobbyGetQuestionnaire(req, out code, out message);
            },
            delegate
            {
                // ������
                if (code == 0)
                {
                    textBoxAdminResult.Text = "code: " + code;
                    textBoxQuestionnaireInfo.Text = res.questionnaire.ToString();
                    //RefreshVipRecordDGV(res.vipRecords);
                }
                else
                {
                    textBoxAdminResult.Text = "code: " + code + ", message:" + message;
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void toolStripButtonAdminGetSpecialWeekSetting_Click(object sender, EventArgs e)
        {
            BindingSource src = (BindingSource)dataGridViewSpecialWeek.DataSource;
            List<REF_PPW_LobbySpecialWeekRecord> list = (List<REF_PPW_LobbySpecialWeekRecord>)src.DataSource;
            if (list.Count != 0)
            {
                if (MessageBox.Show("���ݕҏW���̃X�y�V�����E�B�[�N�ݒ胊�X�g��j�����ăT�[�o����ǂݍ���ł�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
                {
                    return;
                }
            }

            RequestInterface.LobbyAdminGetSpecialWeekRequest req = new RequestInterface.LobbyAdminGetSpecialWeekRequest(RequestInterface.ADMIN_PROFILE_ID);
            RequestInterface.LobbyAdminGetSpecialWeekResponse res = null;
            uint code = 0;
            string message = "";

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                res = requestInterface.LobbyAdminGetSpecialWeek(req, out code, out message);
            },
            delegate
            {
                // ������
                if (code == 0)
                {
                    textBoxAdminResult.Text = "code: " + code;
                    RefreshSpecialWeekRecordDGV(res.specialWeek.records);
                }
                else
                {
                    textBoxAdminResult.Text = "code: " + code + ", message:" + message;
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void toolStripButtonAdminSetSpecialWeekSetting_Click(object sender, EventArgs e)
        {
            dataGridViewSpecialWeek.CommitEdit(DataGridViewDataErrorContexts.Commit);
            BindingSource src = (BindingSource)dataGridViewSpecialWeek.DataSource;
            List<REF_PPW_LobbySpecialWeekRecord> list = (List<REF_PPW_LobbySpecialWeekRecord>)src.DataSource;
            List<REF_PPW_LobbySpecialWeekRecord> clonedList = new List<REF_PPW_LobbySpecialWeekRecord>(list);
            RequestInterface.LobbySpecialWeekSetting specialWeek = new RequestInterface.LobbySpecialWeekSetting(clonedList);

            // �V�K�s�͍폜
            int newRowIndex = dataGridViewSpecialWeek.NewRowIndex;
            if (newRowIndex >= 0 && newRowIndex < list.Count)
            {
                clonedList.RemoveAt(newRowIndex);
            }

            RequestInterface.LobbyAdminSetSpecialWeekRequest req =
                new RequestInterface.LobbyAdminSetSpecialWeekRequest(RequestInterface.ADMIN_PROFILE_ID, specialWeek);

            string message = "";
            uint code = 0;

            if (!req.IsConsistent())
            {
                MessageBox.Show("�s���ȃX�y�V�����E�B�[�N���R�[�h������܂��B�Ԃ��Ȃ��Ă���X�y�V�����E�B�[�N���R�[�h���C�����Ă��������B\n"
                              + "�E����ԍ���0-59��������1000-1009�ɂ��Ă��������B\n");
                return;
            }

            if (MessageBox.Show("���̃X�y�V�����E�B�[�N�ݒ胊�X�g���T�[�o�ɐݒ肵�Ă�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
            {
                return;
            }

            CallAsynchronousProc(delegate
            {
                // ���s����葱��
                requestInterface.LobbyAdminSetSpecialWeek(req, out code, out message);
            },
            delegate
            {
                // ������
                textBoxAdminResult.Text = "code: " + code + (code == 0 ? "" : ", message:" + message);
                if (code == 0)
                {
                    MessageBox.Show("�������܂����B");
                }
                else
                {
                    MessageBox.Show("���s���܂����B");
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void toolStripButtonLoadSpecialWeekSetting_Click(object sender, EventArgs e)
        {
            openFileDialogSettingXML.Filter = "Special Week Setting files (*.xml)|*.xml|All files (*.*)|*.*";
            openFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (openFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                RequestInterface.LobbySpecialWeekSetting req = RequestInterface.LoadXML<RequestInterface.LobbySpecialWeekSetting>(openFileDialogSettingXML.FileName);
                if (req == null)
                {
                    return;
                }
                RefreshSpecialWeekRecordDGV(req.records);
            }
        }

        private void toolStripButtonSaveSpecialWeek_Click(object sender, EventArgs e)
        {
            dataGridViewSpecialWeek.CommitEdit(DataGridViewDataErrorContexts.Commit);
            saveFileDialogSettingXML.Filter = "Special Week files (*.xml)|*.xml|All files (*.*)|*.*";
            saveFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (saveFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                BindingSource src = (BindingSource)dataGridViewSpecialWeek.DataSource;
                RequestInterface.LobbySpecialWeekSetting temp =
                    new RequestInterface.LobbySpecialWeekSetting((List<REF_PPW_LobbySpecialWeekRecord>)src.DataSource);
                RequestInterface.SaveXML<RequestInterface.LobbySpecialWeekSetting>(saveFileDialogSettingXML.FileName, temp);
            }
        }

        private void toolStripButtonCopySpecialWeekRecord_Click(object sender, EventArgs e)
        {
            
            if (dataGridViewSpecialWeek.SelectedRows.Count == 0)
            {
                MessageBox.Show("��ԍ��̕���(�J�n�N�̍����̗�)���N���b�N���ĕ����������s��I�����Ă��������B\n"
                    + "Ctrl�������Ȃ���N���b�N���邱�Ƃŕ����̃X�P�W���[���𕡐��ł��܂��B");
                return;
            }

            BindingSource src = (BindingSource)dataGridViewSpecialWeek.DataSource;
            List<REF_PPW_LobbySpecialWeekRecord> list = (List<REF_PPW_LobbySpecialWeekRecord>)src.DataSource;
            List<REF_PPW_LobbySpecialWeekRecord> copiedVips = new List<REF_PPW_LobbySpecialWeekRecord>();

            // �I������Ă���s�𒲂ׂ�
            foreach (DataGridViewRow row in dataGridViewSpecialWeek.SelectedRows)
            {
                if (row.IsNewRow)
                {
                    MessageBox.Show("�V�K�s(*�����Ă���s)�͕����ł��܂���B");
                    return;
                }
                copiedVips.Add((REF_PPW_LobbySpecialWeekRecord)RequestInterface.DeepCopy(list[row.Index]));
            }

            copiedVips.Sort();

            list.AddRange(copiedVips);
            RefreshSpecialWeekRecordDGV(list);
        }

        private void buttonSetFreeQuestion_Click(object sender, EventArgs e)
        {
            List<RequestInterface.LobbyAdminSetFreeQuestionRequest> requests = new List<RequestInterface.LobbyAdminSetFreeQuestionRequest>();
            foreach (ExtendedDataGridView dgv in freeQuestionDGVs)
            {
                dgv.CommitEdit(DataGridViewDataErrorContexts.Commit);
                BindingSource src = (BindingSource)dgv.DataSource;
                List<REF_PPW_LobbyFreeQuestion> list = (List<REF_PPW_LobbyFreeQuestion>)src.DataSource;
                List<REF_PPW_LobbyFreeQuestion> clonedList = new List<REF_PPW_LobbyFreeQuestion>(list);
                RequestInterface.LobbyFreeQuestionSetting freeQuestion = new RequestInterface.LobbyFreeQuestionSetting(clonedList);

                // �V�K�s�͍폜
                int newRowIndex = dgv.NewRowIndex;
                if (newRowIndex >= 0 && newRowIndex < list.Count)
                {
                    clonedList.RemoveAt(newRowIndex);
                }

                RequestInterface.LobbyAdminSetFreeQuestionRequest req =
                    new RequestInterface.LobbyAdminSetFreeQuestionRequest(RequestInterface.ADMIN_PROFILE_ID, freeQuestion);

                if (!req.IsConsistent())
                {
                    MessageBox.Show("�s���ȔC�ӎ��⃌�R�[�h������܂��B�Ԃ��Ȃ��Ă���C�ӎ��⃌�R�[�h���C�����Ă��������B\n"
                                  + "���╶��108�����A�񓚂�16�����܂łł��B\n"
                                  + "(�������A���{��̏ꍇ�͎��╶36�����A��8�����܂łɂ��Ă��������B)");
                    return;
                }
                requests.Add(req);
            }

            if (MessageBox.Show("���̔C�ӎ���ݒ胊�X�g���T�[�o�ɐݒ肵�Ă�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
            {
                return;
            }

            List<string> messages = new List<string>();
            List<uint> codes = new List<uint>();
            CallAsynchronousProc(delegate
            {
                string message = "";
                uint code = 0;
                foreach (RequestInterface.LobbyAdminSetFreeQuestionRequest req in requests)
                {
                    // ���s����葱��
                    requestInterface.LobbyAdminSetFreeQuestion(req, out code, out message);
                    messages.Add(message);
                    codes.Add(code);
                }
            },
            delegate
            {
                // ������
                textBoxAdminResult.Text = "code: " + Util.Join(codes, ", ");
                if (codes.TrueForAll(delegate(uint code)
                {
                    return code == 0;
                }))
                {
                    MessageBox.Show("�������܂����B");
                }
                else
                {
                    MessageBox.Show("���s���܂����B");
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void buttonGetFreeQuestion_Click(object sender, EventArgs e)
        {
            List<RequestInterface.LobbyAdminGetFreeQuestionRequest> requests = new List<RequestInterface.LobbyAdminGetFreeQuestionRequest>();
            foreach (ExtendedDataGridView dgv in freeQuestionDGVs)
            {
                RequestInterface.LobbyAdminGetFreeQuestionRequest req =
                    new RequestInterface.LobbyAdminGetFreeQuestionRequest(RequestInterface.ADMIN_PROFILE_ID, (int)dgv.Tag);

                requests.Add(req);
            }

            if (MessageBox.Show("���ݕҏW���̔C�ӎ����j�����ăT�[�o����ǂݍ���ł�낵���ł����H", "�m�F", MessageBoxButtons.YesNo) != DialogResult.Yes)
            {
                return;
            }

            List<RequestInterface.LobbyAdminGetFreeQuestionResponse> responses = new List<RequestInterface.LobbyAdminGetFreeQuestionResponse>();
            List<string> messages = new List<string>();
            List<uint> codes = new List<uint>();
            CallAsynchronousProc(delegate
            {
                string message = "";
                uint code = 0;
                foreach (RequestInterface.LobbyAdminGetFreeQuestionRequest req in requests)
                {
                    // ���s����葱��
                    responses.Add(requestInterface.LobbyAdminGetFreeQuestion(req, out code, out message));
                    messages.Add(message);
                    codes.Add(code);
                }
            },
            delegate
            {
                // ������
                textBoxAdminResult.Text = "code: " + Util.Join(codes, ", ");
                if (codes.TrueForAll(delegate(uint code)
                {
                    return code == 0;
                }))
                {
                    foreach (RequestInterface.LobbyAdminGetFreeQuestionResponse res in responses)
                    {
                        // ����R�[�h����v����DGV��T��
                        ExtendedDataGridView matched = freeQuestionDGVs.Find(delegate(ExtendedDataGridView elem)
                        {
                            return (int)elem.Tag == res.freeQuestion.records[0].Language;
                        });
                        if (matched == null)
                        {
                            throw new Exception("�Y�����錾��R�[�h��DGV��������܂���B");
                        }
                        RefreshFreeQuestionDGV(matched, res.freeQuestion);
                    }
                    MessageBox.Show("�������܂����B");
                }
                else
                {
                    MessageBox.Show("���s���܂����B");
                }
            },
            delegate
            {
                // ���s��
            });
        }

        private void dataGridViewSpecialWeek_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;

            if (dgv.Rows[e.RowIndex].IsNewRow)
            {
                return;
            }

            BindingSource src = (BindingSource)dgv.DataSource;
            List<REF_PPW_LobbySpecialWeekRecord> list = (List<REF_PPW_LobbySpecialWeekRecord>)src.DataSource;
            if (!list[e.RowIndex].IsConsistent())
            {
                // ���������Ƃ�Ă��Ȃ��s��Ԃ�����
                e.CellStyle.BackColor = Color.LightPink;
            }
        }

        void dataGridViewFreeQuestion_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;

            if (dgv.Rows[e.RowIndex].IsNewRow)
            {
                return;
            }

            BindingSource src = (BindingSource)dgv.DataSource;
            List<REF_PPW_LobbyFreeQuestion> list = (List<REF_PPW_LobbyFreeQuestion>)src.DataSource;
            if (!list[e.RowIndex].IsConsistent())
            {
                // ���������Ƃ�Ă��Ȃ��s��Ԃ�����
                e.CellStyle.BackColor = Color.LightPink;
            }
        }

        private void buttonQuestionnaireUpdate_Click(object sender, EventArgs e)
        {
            string message = "";

            RequestInterface.LobbyAdminUpdateQuestionnaireRequest req = new RequestInterface.LobbyAdminUpdateQuestionnaireRequest(RequestInterface.ADMIN_PROFILE_ID);
            RequestInterface.LobbyAdminUpdateQuestionnaireResponse res = null;
            
            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbyAdminUpdateQuestionnaireRequest, RequestInterface.LobbyAdminUpdateQuestionnaireResponse>
                    (RequestInterface.URL_ADMIN_UPDATE_QUESTIONNAIRE, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

        private void buttonSubmitQuestionnaire_Click(object sender, EventArgs e)
        {
            string message = "";
            int profileId = 100000;
            int serialNo = 0;
            byte language = 1;
            int answerNo = 0;

            try
            {
                profileId = int.Parse(textBoxSubmitQuestionnaireProfileId.Text);
                serialNo = int.Parse(textBoxSubmitQuestionnaireSerialNo.Text);
                language = byte.Parse(textBoxSubmitQuestionnaireLanguage.Text);
                answerNo = int.Parse(textBoxSubmitQuestionnaireAnswerNo.Text);
            }
            catch (Exception err)
            {
                MessageBox.Show("���͂��ꂽ�����񂪕s���ł��B\n" + err.Message);
            }

            RequestInterface.LobbySubmitQuestionnaireRequest req = new RequestInterface.LobbySubmitQuestionnaireRequest(profileId, serialNo, language, answerNo);
            RequestInterface.LobbySubmitQuestionnaireResponse res = null;

            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbySubmitQuestionnaireRequest, RequestInterface.LobbySubmitQuestionnaireResponse>
                    (RequestInterface.URL_SUBMIT_QUESTIONNAIRE, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

        private void dataGridViewSpecialWeek_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            ExtendedDataGridView.ShowDataError(sender, e);
            e.Cancel = false;
        }

        private void buttonLoadFreeQuestionSetting_Click(object sender, EventArgs e)
        {
            openFileDialogSettingXML.Filter = "Free Question Setting files (*.xml)|*.xml|All files (*.*)|*.*";
            openFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (openFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                List<RequestInterface.LobbyFreeQuestionSetting> list = RequestInterface.LoadXML<List<RequestInterface.LobbyFreeQuestionSetting>>(openFileDialogSettingXML.FileName);
                if (list == null)
                {
                    return;
                }

                if(list.Count != freeQuestionDGVs.Count)
                {
                    MessageBox.Show("�ǂݍ��񂾐ݒ�t�@�C���ƌ��݂̃v���O�����͌݊���������܂���B");
                    return;
                }

                int i = -1;
                foreach (RequestInterface.LobbyFreeQuestionSetting item in list)
                {
                    i++;
                    RefreshFreeQuestionDGV(freeQuestionDGVs[i], item);
                }
            }
        }

        private void buttonSaveFreeQuestionSetting_Click(object sender, EventArgs e)
        {
            dataGridViewVipSettingRecord.CommitEdit(DataGridViewDataErrorContexts.Commit);
            saveFileDialogSettingXML.Filter = "Free Question Setting files (*.xml)|*.xml|All files (*.*)|*.*";
            saveFileDialogSettingXML.InitialDirectory = Directory.GetCurrentDirectory();
            if (saveFileDialogSettingXML.ShowDialog() == DialogResult.OK)
            {
                List<RequestInterface.LobbyFreeQuestionSetting> settingList = new List<RequestInterface.LobbyFreeQuestionSetting>();
                foreach (ExtendedDataGridView dgv in freeQuestionDGVs)
                {
                    dgv.CommitEdit(DataGridViewDataErrorContexts.Commit);
                    BindingSource src = (BindingSource)dgv.DataSource;
                    List<REF_PPW_LobbyFreeQuestion> list = (List<REF_PPW_LobbyFreeQuestion>)src.DataSource;
                    RequestInterface.LobbyFreeQuestionSetting freeQuestion = new RequestInterface.LobbyFreeQuestionSetting(list);
                    settingList.Add(freeQuestion);
                }

                RequestInterface.SaveXML <List<RequestInterface.LobbyFreeQuestionSetting>>(saveFileDialogSettingXML.FileName, settingList);
            }
        }

        private void buttonSetQuestionSerialNo_Click(object sender, EventArgs e)
        {
            string message = "";
            int questionSerialNo = 0;

            try
            {
                questionSerialNo = int.Parse(textBoxSetQuestionSerialNo.Text);
            }
            catch (Exception err)
            {
                MessageBox.Show("���͂��ꂽ�����񂪕s���ł��B\n" + err.Message);
            }

            RequestInterface.LobbyAdminSetQuestionSerialNoRequest req = new RequestInterface.LobbyAdminSetQuestionSerialNoRequest(RequestInterface.ADMIN_PROFILE_ID, questionSerialNo);
            RequestInterface.LobbyAdminSetQuestionSerialNoResponse res = null;

            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbyAdminSetQuestionSerialNoRequest, RequestInterface.LobbyAdminSetQuestionSerialNoResponse>
                    (RequestInterface.URL_ADMIN_SET_QUESTION_SERIAL_NO, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

        private void buttonSetNextQuestionNo_Click(object sender, EventArgs e)
        {
            string message = "";
            int nextQuestionNo = 0;

            try
            {
                nextQuestionNo = int.Parse(textBoxSetNextQuestionNo.Text);
            }
            catch (Exception err)
            {
                MessageBox.Show("���͂��ꂽ�����񂪕s���ł��B\n" + err.Message);
            }

            RequestInterface.LobbyAdminSetNextQuestionNoRequest req = new RequestInterface.LobbyAdminSetNextQuestionNoRequest(RequestInterface.ADMIN_PROFILE_ID, nextQuestionNo);
            RequestInterface.LobbyAdminSetNextQuestionNoResponse res = null;

            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbyAdminSetNextQuestionNoRequest, RequestInterface.LobbyAdminSetNextQuestionNoResponse>
                    (RequestInterface.URL_ADMIN_SET_NEXT_QUESTION_NO, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

        private void buttonSetQuestionNo_Click(object sender, EventArgs e)
        {
            string message = "";
            int questionNo = 0;

            try
            {
                questionNo = int.Parse(textBoxSetQuestionNo.Text);
            }
            catch (Exception err)
            {
                MessageBox.Show("���͂��ꂽ�����񂪕s���ł��B\n" + err.Message);
            }

            RequestInterface.LobbyAdminSetQuestionNoRequest req = new RequestInterface.LobbyAdminSetQuestionNoRequest(RequestInterface.ADMIN_PROFILE_ID, questionNo);
            RequestInterface.LobbyAdminSetQuestionNoResponse res = null;

            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbyAdminSetQuestionNoRequest, RequestInterface.LobbyAdminSetQuestionNoResponse>
                    (RequestInterface.URL_ADMIN_SET_QUESTION_NO, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

        private void buttonInitializeQuestionnaire_Click(object sender, EventArgs e)
        {
            string message = "";

            RequestInterface.LobbyAdminInitializeQuestionnaireRequest req = new RequestInterface.LobbyAdminInitializeQuestionnaireRequest(RequestInterface.ADMIN_PROFILE_ID);
            RequestInterface.LobbyAdminInitializeQuestionnaireResponse res = null;

            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbyAdminInitializeQuestionnaireRequest, RequestInterface.LobbyAdminInitializeQuestionnaireResponse>
                    (RequestInterface.URL_ADMIN_INITIALIZE_QUESTIONNAIRE, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

        private void buttonSetSpecialQuestionThreshold_Click(object sender, EventArgs e)
        {
            string message = "";
            int questionNo = 0;

            try
            {
                questionNo = int.Parse(textBoxSetSpecialQuestionThreshold.Text);
            }
            catch (Exception err)
            {
                MessageBox.Show("���͂��ꂽ�����񂪕s���ł��B\n" + err.Message);
            }

            RequestInterface.LobbyAdminSetSpecialQuestionThresholdRequest req = new RequestInterface.LobbyAdminSetSpecialQuestionThresholdRequest(RequestInterface.ADMIN_PROFILE_ID, questionNo);
            RequestInterface.LobbyAdminSetSpecialQuestionThresholdResponse res = null;

            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbyAdminSetSpecialQuestionThresholdRequest, RequestInterface.LobbyAdminSetSpecialQuestionThresholdResponse>
                    (RequestInterface.URL_ADMIN_SET_SPECIAL_QUESTION_THRESHOLD, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

        private void buttonSetDefaultSummerizeLanguage_Click(object sender, EventArgs e)
        {
            string message = "";
            // ���M����i�K�ōŌ�ɏI�[�����邪�A�ǂ����p�f�B���O�Ȃ̂Ŗ��Ȃ�
            char[] summarizeFlagArray = { '\u0000', '\u0000', '\u0000', '\u0000', '\u0000', '\u0000', '\u0000', '\u0000', '\u0000', '\u0000', '\u0000', '\u0000'};

            CheckedListBox.CheckedIndexCollection list = checkedListBoxDefaultSummerizeLanguage.CheckedIndices;

            foreach (int index in list)
            {
                summarizeFlagArray[(int)Util.IndexToLanguageCode(index)] = '\u0001';
            }

            RequestInterface.LobbyAdminSetDefaultSummarizeFlagRequest req = new RequestInterface.LobbyAdminSetDefaultSummarizeFlagRequest(RequestInterface.ADMIN_PROFILE_ID, new string(summarizeFlagArray));
            RequestInterface.LobbyAdminSetDefaultSummarizeFlagResponse res = null;

            CallAsynchronousProc(delegate
            {
                res = requestInterface.GenericSession<RequestInterface.LobbyAdminSetDefaultSummarizeFlagRequest, RequestInterface.LobbyAdminSetDefaultSummarizeFlagResponse>
                    (RequestInterface.URL_ADMIN_SET_DEFAULT_SUMMARIZE_FLAG, req, out message);
            }, delegate
            {
                textBoxAdminResult.Text = "code: " + res.code + (res.code != 0 ? ", message:" + message : "");
            });
        }

    }
}