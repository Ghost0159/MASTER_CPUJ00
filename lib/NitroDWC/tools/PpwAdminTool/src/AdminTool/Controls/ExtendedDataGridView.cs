using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace AdminTool
{
    class ExtendedDataGridView : DataGridView
    {
        public ExtendedDataGridView()
            : base()
        {
            KeyDown +=new KeyEventHandler(ExtendedDataGridView_KeyDown);
            CellContextMenuStripNeeded += new DataGridViewCellContextMenuStripNeededEventHandler(ExtendedDataGridView_CellContextMenuStripNeeded);
            CellFormatting += new DataGridViewCellFormattingEventHandler(ExtendedDataGridView_CellFormatting);
        }

        void ExtendedDataGridView_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;

            // �V�K�s�̑O�i�F���O���[�ɂ���
            if (dgv.Rows[e.RowIndex].IsNewRow)
            {
                e.CellStyle.ForeColor = Color.Gray;
            }
        }

        void ExtendedDataGridView_KeyDown(object sender, KeyEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;
            if (dgv.CurrentCell == null)
            {
                return;
            }

            if (e.Control)
            {
                switch (e.KeyCode)
                {
                    // �y�[�X�g
                    case Keys.V:
                        List<List<string>> stringArray = Util.TsvTextToArray(Clipboard.GetText());
                        SetStringArray(stringArray);
                        this.Refresh();
                        break;
                }
            }
        }

        void ExtendedDataGridView_CellContextMenuStripNeeded(object sender, DataGridViewCellContextMenuStripNeededEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;

            if (e.ColumnIndex < 0 || e.RowIndex < 0)
            {
                return;
            }

            // �P�ɑI������
            dgv.ClearSelection();
            dgv[e.ColumnIndex, e.RowIndex].Selected = true;
        }

        public void SetStringArray(List<List<string>> stringArray)
        {
            int startRow = CurrentCell.RowIndex;
            int startColumn = CurrentCell.ColumnIndex;
            DataGridViewCell cell = SelectMostTopLeftCell(SelectedCells);
            startRow = cell.RowIndex;
            startColumn = cell.ColumnIndex;
            for (int i = 0; i < stringArray.Count && i + startRow < Rows.Count; i++)
            {
                if (Rows[startRow + i].IsNewRow)
                {
                    MessageBox.Show("�V�K�s(*�����Ă���s)�ɂ̓y�[�X�g�ł��܂���B");
                    continue;
                }

                for (int j = 0; j < stringArray[i].Count && j + startColumn < Rows[startRow + i].Cells.Count; j++)
                {
                    object orgValue = this[startColumn + j, startRow + i].Value;

                    if (this[startColumn + j, startRow + i].ReadOnly)
                    {
                        // ���[�h�I�����[�ɐݒ肳��Ă���B
                        MessageBox.Show("�ǂݎ���p�̃Z���ɂ̓y�[�X�g�ł��܂���B");
                        return;
                    }

                    if (stringArray[i][j] == "")
                    {
                        // �I��͈͂������`�ł͂Ȃ�
                        MessageBox.Show("�����`�łȂ��I��͈͂̃R�s�[���y�[�X�g�ɂ͑Ή����Ă��܂���B");
                        return;
                    }

                    this[startColumn + j, startRow + i].Value = stringArray[i][j];

                    // ���̓f�[�^�������������̗��R�ŃZ�b�g����Ă��Ȃ������ꍇ�͒��f����B�\���e�L�X�g���r
                    if (this[startColumn + j, startRow + i].FormattedValue.ToString() != stringArray[i][j])
                    {
                        // ���͑O�̒l�ɖ߂�
                        // DataError�C�x���g�n���h����e.Cancel = false������Ɖ��L�͕K�v�Ȃ�
                        //this[startColumn + j, startRow + i].Value = orgValue;
                        return;
                    }
                }
            }
        }

        // ��ԍ���̃Z����Ԃ�
        // ��`�I���ɑΉ��ACtrl�����Ȃ���̕����I���͓K���ɑΉ�
        public static DataGridViewCell SelectMostTopLeftCell(DataGridViewSelectedCellCollection dataGridViewSelectedCellCollection)
        {
            int? minRow = null;
            int? minColumn = null;
            DataGridViewCell resultCell = null;
            foreach (DataGridViewCell cell in dataGridViewSelectedCellCollection)
            {
                if (minRow == null || cell.RowIndex < minRow)
                {
                    minRow = cell.RowIndex;
                }
            }

            foreach (DataGridViewCell cell in dataGridViewSelectedCellCollection)
            {
                if (minRow == cell.RowIndex)
                {
                    if (minColumn == null || cell.ColumnIndex < minColumn)
                    {
                        minColumn = cell.ColumnIndex;
                        resultCell = cell;
                    }
                }
            }
            return resultCell;
        }

        public static void ShowDataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            DataGridView dgv = (DataGridView)sender;
            MessageBox.Show("���͂������̓y�[�X�g���ꂽ�f�[�^�`�������̃Z���̌`���ƈ�v���Ă��܂���B\n"
                + "�Z���̈ʒu: " + (e.RowIndex + 1) + "�s�ځA�u" + dgv.Columns[e.ColumnIndex].Name + "�v�J����\n"
                + "�ڍ׏��: " + e.Exception.Message);
        }

    }
}
