
using System;
using System.Drawing;
using System.Windows.Forms;

/// <summary>
/// ���X�g�r���[��ŃA�C�e����ҏW���邽�߂̃e�L�X�g�{�b�N�X
/// </summary>
public class ListViewInputBox : TextBox
{
    public class InputEventArgs : EventArgs
    {
        private string orgText;
        public string OrgText
        {
            get { return orgText; }
            set { orgText = value; }
        }

        private string newText;
        public string NewText
        {
            get { return newText; }
            set { newText = value; }
        }

        private int row;
        public int Row
        {
            get { return row; }
            set { row = value; }
        }

        private int column;
        public int Column
        {
            get { return column; }
            set { column = value; }
        }

        private bool canceled;
        public bool Canceled
        {
            get { return canceled; }
            set { canceled = value; }
        }

        private bool mouseEnd;
        public bool MouseEnd
        {
            get { return mouseEnd; }
            set { mouseEnd = value; }
        }

    }

    public delegate void InputEventHandler(object sender, InputEventArgs e);

    //�C�x���g�f���Q�[�g�̐錾
    public event InputEventHandler FinishInput;

    private InputEventArgs args = new InputEventArgs();
    private bool finished = false;
    
    /// <summary>
    /// 
    /// </summary>
    /// <param name="parent">�ΏۂƂȂ�ListView�R���g���[��</param>
    /// <param name="info">�q�b�g�e�X�g����</param>
    public ListViewInputBox(ListView parent, int row, int column)
        : base()
    {
        Initialize(parent, row, column);
    }

    /// <summary>
    /// 
    /// </summary>
    /// <param name="parent">�ΏۂƂȂ�ListView�R���g���[��</param>
    /// <param name="info">�q�b�g�e�X�g����</param>
    public ListViewInputBox(ListView parent, ListViewHitTestInfo info)
        : base()
    {
        int column = info.Item.SubItems.IndexOf(info.SubItem);
        int row = info.Item.Index;
        Initialize(parent, row, column);
    }

    private void Initialize(ListView parent, int row, int column)
    {
        ListViewItem item = parent.Items[row];

        args.OrgText = item.SubItems[column].Text;
        args.NewText = item.SubItems[column].Text;
        args.Row = row;
        args.Column = column;

        int left = 0;
        for (int i = 0; i < column; i++)
        {
            left += parent.Columns[i].Width;
        }

        int width;
        // index��0�̎��͍s�S�̂̕����擾���Ă��܂��̂�index1��Left�𕝂Ƃ���
        if (column == 0 && item.SubItems.Count >= 1)
        {
            width = item.SubItems[1].Bounds.Left;
        }
        else
        {
            width = item.SubItems[column].Bounds.Width;
        }
        int height = item.SubItems[column].Bounds.Height;

        this.Parent = parent;
        this.Size = new Size(width, height);
        this.FontHeight -= 3;
        this.Left = left;
        this.Top = item.Position.Y - 1;
        this.Text = item.SubItems[column].Text;
        this.LostFocus += new EventHandler(textbox_LostFocus);
        this.ImeMode = ImeMode.NoControl;
        this.Multiline = false;
        this.KeyDown += new KeyEventHandler(textbox_KeyDown);
        this.TextAlign = HorizontalAlignment.Right;
        this.Focus();
    }

    void Finish(string new_name)
    {
        finished = true;
        this.Hide();    // KeyDown�ŏI�������ꍇ�A������LostFocus���Ă΂�邱�Ƃɒ���
        args.NewText = new_name;
        FinishInput(this, args);
    }

    void textbox_KeyDown(object sender, KeyEventArgs e)
    {
        if (finished)
        {
            return;
        }

        if (e.KeyCode == Keys.Enter)
        {
            args.Canceled = false;
            args.MouseEnd = false;
            Finish(this.Text);
        }
        else if (e.KeyCode == Keys.Escape)
        {
            args.Canceled = true;
            args.MouseEnd = false;
            Finish(args.OrgText);
        }
    }

    void textbox_LostFocus(object sender, EventArgs e)
    {
        if (finished)
        {
            return;
        }

        args.Canceled = false;
        args.MouseEnd = true;
        Finish(this.Text);
    }
}
