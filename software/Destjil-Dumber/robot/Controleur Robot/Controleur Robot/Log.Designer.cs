namespace Controleur_Robot
{
    partial class Log
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.textBoxLog = new System.Windows.Forms.TextBox();
            this.buttonFermer = new System.Windows.Forms.Button();
            this.buttonEffacer = new System.Windows.Forms.Button();
            this.checkBoxVerbose = new System.Windows.Forms.CheckBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.checkBoxVerbose);
            this.panel1.Controls.Add(this.textBoxLog);
            this.panel1.Controls.Add(this.buttonFermer);
            this.panel1.Controls.Add(this.buttonEffacer);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(292, 266);
            this.panel1.TabIndex = 0;
            // 
            // textBoxLog
            // 
            this.textBoxLog.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxLog.Location = new System.Drawing.Point(3, 3);
            this.textBoxLog.Multiline = true;
            this.textBoxLog.Name = "textBoxLog";
            this.textBoxLog.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBoxLog.Size = new System.Drawing.Size(286, 231);
            this.textBoxLog.TabIndex = 2;
            this.textBoxLog.WordWrap = false;
            // 
            // buttonFermer
            // 
            this.buttonFermer.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonFermer.Location = new System.Drawing.Point(214, 240);
            this.buttonFermer.Name = "buttonFermer";
            this.buttonFermer.Size = new System.Drawing.Size(75, 23);
            this.buttonFermer.TabIndex = 1;
            this.buttonFermer.Text = "&Fermer";
            this.buttonFermer.UseVisualStyleBackColor = true;
            this.buttonFermer.Click += new System.EventHandler(this.buttonFermer_Click);
            // 
            // buttonEffacer
            // 
            this.buttonEffacer.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonEffacer.Location = new System.Drawing.Point(3, 240);
            this.buttonEffacer.Name = "buttonEffacer";
            this.buttonEffacer.Size = new System.Drawing.Size(75, 23);
            this.buttonEffacer.TabIndex = 0;
            this.buttonEffacer.Text = "&Effacer";
            this.buttonEffacer.UseVisualStyleBackColor = true;
            this.buttonEffacer.Click += new System.EventHandler(this.buttonEffacer_Click);
            // 
            // checkBoxVerbose
            // 
            this.checkBoxVerbose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBoxVerbose.AutoSize = true;
            this.checkBoxVerbose.Checked = true;
            this.checkBoxVerbose.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxVerbose.Location = new System.Drawing.Point(84, 244);
            this.checkBoxVerbose.Name = "checkBoxVerbose";
            this.checkBoxVerbose.Size = new System.Drawing.Size(65, 17);
            this.checkBoxVerbose.TabIndex = 3;
            this.checkBoxVerbose.Text = "Verbeux";
            this.checkBoxVerbose.UseVisualStyleBackColor = true;
            // 
            // Log
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 266);
            this.Controls.Add(this.panel1);
            this.Name = "Log";
            this.ShowIcon = false;
            this.Text = "Log";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox textBoxLog;
        private System.Windows.Forms.Button buttonFermer;
        private System.Windows.Forms.Button buttonEffacer;
        private System.Windows.Forms.CheckBox checkBoxVerbose;
    }
}