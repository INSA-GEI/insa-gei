using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Controleur_Robot
{
    public partial class Log : Form
    {
        public Log()
        {
            InitializeComponent();
        }

        private void buttonFermer_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void buttonEffacer_Click(object sender, EventArgs e)
        {
            textBoxLog.Clear();
        }

        public void WriteLine(string text)
        {
            if (checkBoxVerbose.Checked == false)
            {
                if (text.StartsWith("[ERR]"))
                {
                    textBoxLog.AppendText(text + "\r\n");
                }
            }
            else
            {
                textBoxLog.AppendText(text + "\r\n");
            }
        }
    }
}
