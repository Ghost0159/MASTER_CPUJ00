using System;
using System.Collections.Generic;
using System.Text;

namespace AdminTool
{
    /// <summary>
    /// ���X�g�{�b�N�X�̃����o�ɒl���������邽�߂̃N���X
    /// </summary>
    class ListBoxMember
    {
        public const string DISPLAYMEMBER = "Name";
        public const string VALUEMEMBER = "Value";
        private string name;
        private object value;
        private ListBoxMember()
        {
            name = null;
            value = null;
        }
        public ListBoxMember(string _name, object _value)
            : this()
        {
            name = _name;
            value = _value;
        }
        public string Name
        {
            get { return name; }
        }
        public object Value
        {
            get { return value; }
        }
    }
}
