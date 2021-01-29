using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace AdminTool
{
    public partial class EditRecordForm : Form
    {
        public List<RequestInterface.LobbyScheduleRecord> orgData;
        public bool endOk = false;
        public bool EndOk
        {
            get { return endOk; }
        }

        public EditRecordForm(List<RequestInterface.LobbyScheduleRecord> dataSource)
        {
            InitializeComponent();
            orgData = dataSource;

            BindingSource bindingSource = new BindingSource();
            bindingSource.AllowNew = true;
            bindingSource.DataSource = typeof(RequestInterface.LobbyScheduleRecord);    // ���̃f�[�^�\�[�X�̋K��̌^���w��

            foreach (RequestInterface.LobbyScheduleRecord item in dataSource)
            {
                bindingSource.Add(item);
            }

            DataGridViewTextBoxColumn text = new DataGridViewTextBoxColumn();
            text.DataPropertyName = "Time"; // �p�u���b�N�ȃv���p�e�B�����w�肵�Ȃ��ƕ\������Ȃ����Ƃɒ���
            text.Name = "����(�b)";
            text.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleRight;
            dataGridView.Columns.Add(text);
            
            DataGridViewComboBoxColumn combo = new DataGridViewComboBoxColumn();
            List<RequestInterface.PPW_LOBBY_TIME_EVENT> list =
                new List<RequestInterface.PPW_LOBBY_TIME_EVENT>
                ((RequestInterface.PPW_LOBBY_TIME_EVENT[])Enum.GetValues(typeof(RequestInterface.PPW_LOBBY_TIME_EVENT)));
            list.RemoveAt(0);   // Enum�v�f�̓��A�ŏ���Lock�����O����B
            combo.DataSource = list;
            combo.DataPropertyName = "TimeEvent";
            combo.Name = "�C�x���g";
            combo.MaxDropDownItems = 20;
            dataGridView.Columns.Add(combo);

            dataGridView.AutoGenerateColumns = false;
            dataGridView.DataSource = bindingSource;
            bindingNavigator.BindingSource = bindingSource;
        }

        private void EditRecordForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (endOk)
            {
                BindingSource bindingSource = (BindingSource)dataGridView.DataSource;
                orgData.Clear();
                foreach (RequestInterface.LobbyScheduleRecord item in bindingSource)
                {
                    orgData.Add(item);
                }
            }
        }

        private void buttonOK_Click(object sender, EventArgs e)
        {
            endOk = true;
            Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void dataGridView_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            ExtendedDataGridView.ShowDataError(sender, e);
            e.Cancel = false;
        }
    }
}