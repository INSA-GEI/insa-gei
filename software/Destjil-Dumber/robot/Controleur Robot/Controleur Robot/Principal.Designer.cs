namespace Controleur_Robot
{
    partial class Principal
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Principal));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.updateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.logToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonConnecter = new System.Windows.Forms.Button();
            this.labelConnecter = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.labelOdoDroit = new System.Windows.Forms.Label();
            this.labelOdoGauche = new System.Windows.Forms.Label();
            this.labelCapteur = new System.Windows.Forms.Label();
            this.labelBatterie = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.labelCommandeCourante = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.trackBarVitesse = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.buttonArriere = new System.Windows.Forms.Button();
            this.buttonArret = new System.Windows.Forms.Button();
            this.buttonDroite = new System.Windows.Forms.Button();
            this.buttonGauche = new System.Windows.Forms.Button();
            this.buttonAvant = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.buttonLed = new System.Windows.Forms.Button();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.buttonEnregistrer = new System.Windows.Forms.Button();
            this.buttonEnvoyer = new System.Windows.Forms.Button();
            this.labelDroitRapid = new System.Windows.Forms.Label();
            this.labelDroitRapide = new System.Windows.Forms.Label();
            this.trackBarDroitRapide = new System.Windows.Forms.TrackBar();
            this.labelDroitNormal = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.trackBarDroitNormal = new System.Windows.Forms.TrackBar();
            this.labelGaucheRapide = new System.Windows.Forms.Label();
            this.label15 = new System.Windows.Forms.Label();
            this.trackBarGaucheRapide = new System.Windows.Forms.TrackBar();
            this.labelGaucheNormal = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.trackBarGaucheNormal = new System.Windows.Forms.TrackBar();
            this.timerCommande = new System.Windows.Forms.Timer(this.components);
            this.timerWatchdog = new System.Windows.Forms.Timer(this.components);
            this.menuStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVitesse)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarDroitRapide)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarDroitNormal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGaucheRapide)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGaucheNormal)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.updateToolStripMenuItem,
            this.logToolStripMenuItem,
            this.aboutToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(698, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // updateToolStripMenuItem
            // 
            this.updateToolStripMenuItem.Name = "updateToolStripMenuItem";
            this.updateToolStripMenuItem.Size = new System.Drawing.Size(90, 20);
            this.updateToolStripMenuItem.Text = "&Mise à jour FW";
            this.updateToolStripMenuItem.Click += new System.EventHandler(this.updateToolStripMenuItem_Click);
            // 
            // logToolStripMenuItem
            // 
            this.logToolStripMenuItem.Name = "logToolStripMenuItem";
            this.logToolStripMenuItem.Size = new System.Drawing.Size(36, 20);
            this.logToolStripMenuItem.Text = "&Log";
            this.logToolStripMenuItem.Click += new System.EventHandler(this.logToolStripMenuItem_Click);
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(77, 20);
            this.aboutToolStripMenuItem.Text = "&A propos de";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonConnecter);
            this.groupBox1.Controls.Add(this.labelConnecter);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(3, 27);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(208, 110);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Connection";
            // 
            // buttonConnecter
            // 
            this.buttonConnecter.Location = new System.Drawing.Point(18, 65);
            this.buttonConnecter.Name = "buttonConnecter";
            this.buttonConnecter.Size = new System.Drawing.Size(169, 32);
            this.buttonConnecter.TabIndex = 2;
            this.buttonConnecter.Text = "Connecter";
            this.buttonConnecter.UseVisualStyleBackColor = true;
            this.buttonConnecter.Click += new System.EventHandler(this.buttonConnecter_Click);
            // 
            // labelConnecter
            // 
            this.labelConnecter.Location = new System.Drawing.Point(71, 23);
            this.labelConnecter.Name = "labelConnecter";
            this.labelConnecter.Size = new System.Drawing.Size(116, 23);
            this.labelConnecter.TabIndex = 1;
            this.labelConnecter.Text = "non connecté";
            this.labelConnecter.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 28);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(29, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Etat:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.labelOdoDroit);
            this.groupBox2.Controls.Add(this.labelOdoGauche);
            this.groupBox2.Controls.Add(this.labelCapteur);
            this.groupBox2.Controls.Add(this.labelBatterie);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.label6);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Location = new System.Drawing.Point(3, 143);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(208, 137);
            this.groupBox2.TabIndex = 2;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Informations";
            // 
            // labelOdoDroit
            // 
            this.labelOdoDroit.AutoSize = true;
            this.labelOdoDroit.Location = new System.Drawing.Point(134, 109);
            this.labelOdoDroit.Name = "labelOdoDroit";
            this.labelOdoDroit.Size = new System.Drawing.Size(53, 13);
            this.labelOdoDroit.TabIndex = 8;
            this.labelOdoDroit.Text = "Pas d\'info";
            // 
            // labelOdoGauche
            // 
            this.labelOdoGauche.AutoSize = true;
            this.labelOdoGauche.Location = new System.Drawing.Point(134, 86);
            this.labelOdoGauche.Name = "labelOdoGauche";
            this.labelOdoGauche.Size = new System.Drawing.Size(53, 13);
            this.labelOdoGauche.TabIndex = 7;
            this.labelOdoGauche.Text = "Pas d\'info";
            // 
            // labelCapteur
            // 
            this.labelCapteur.AutoSize = true;
            this.labelCapteur.Location = new System.Drawing.Point(134, 58);
            this.labelCapteur.Name = "labelCapteur";
            this.labelCapteur.Size = new System.Drawing.Size(53, 13);
            this.labelCapteur.TabIndex = 6;
            this.labelCapteur.Text = "Pas d\'info";
            // 
            // labelBatterie
            // 
            this.labelBatterie.AutoSize = true;
            this.labelBatterie.Location = new System.Drawing.Point(134, 30);
            this.labelBatterie.Name = "labelBatterie";
            this.labelBatterie.Size = new System.Drawing.Size(53, 13);
            this.labelBatterie.TabIndex = 5;
            this.labelBatterie.Text = "Pas d\'info";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(22, 109);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(81, 13);
            this.label7.TabIndex = 4;
            this.label7.Text = "Odometrie droit:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(22, 86);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(97, 13);
            this.label6.TabIndex = 3;
            this.label6.Text = "Odometrie gauche:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(22, 58);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(47, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "Capteur:";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(22, 30);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(46, 13);
            this.label3.TabIndex = 0;
            this.label3.Text = "Batterie:";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.labelCommandeCourante);
            this.groupBox3.Controls.Add(this.label9);
            this.groupBox3.Controls.Add(this.groupBox5);
            this.groupBox3.Controls.Add(this.buttonArriere);
            this.groupBox3.Controls.Add(this.buttonArret);
            this.groupBox3.Controls.Add(this.buttonDroite);
            this.groupBox3.Controls.Add(this.buttonGauche);
            this.groupBox3.Controls.Add(this.buttonAvant);
            this.groupBox3.Location = new System.Drawing.Point(217, 27);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(481, 253);
            this.groupBox3.TabIndex = 3;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Contrôle";
            // 
            // labelCommandeCourante
            // 
            this.labelCommandeCourante.AccessibleName = "";
            this.labelCommandeCourante.AutoSize = true;
            this.labelCommandeCourante.Location = new System.Drawing.Point(380, 209);
            this.labelCommandeCourante.Name = "labelCommandeCourante";
            this.labelCommandeCourante.Size = new System.Drawing.Size(44, 13);
            this.labelCommandeCourante.TabIndex = 10;
            this.labelCommandeCourante.Text = "Aucune";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(266, 209);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(108, 13);
            this.label9.TabIndex = 9;
            this.label9.Text = "Commande courante:";
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.trackBarVitesse);
            this.groupBox5.Controls.Add(this.label5);
            this.groupBox5.Controls.Add(this.label8);
            this.groupBox5.Location = new System.Drawing.Point(333, 23);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(136, 142);
            this.groupBox5.TabIndex = 8;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Vitesse";
            // 
            // trackBarVitesse
            // 
            this.trackBarVitesse.Location = new System.Drawing.Point(22, 24);
            this.trackBarVitesse.Maximum = 2;
            this.trackBarVitesse.Minimum = 1;
            this.trackBarVitesse.Name = "trackBarVitesse";
            this.trackBarVitesse.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBarVitesse.Size = new System.Drawing.Size(45, 112);
            this.trackBarVitesse.TabIndex = 5;
            this.trackBarVitesse.Value = 1;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(73, 33);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(41, 13);
            this.label5.TabIndex = 6;
            this.label5.Text = "Rapide";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(73, 117);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(46, 13);
            this.label8.TabIndex = 7;
            this.label8.Text = "Normale";
            // 
            // buttonArriere
            // 
            this.buttonArriere.Location = new System.Drawing.Point(126, 158);
            this.buttonArriere.Name = "buttonArriere";
            this.buttonArriere.Size = new System.Drawing.Size(84, 50);
            this.buttonArriere.TabIndex = 4;
            this.buttonArriere.Text = "Arriere";
            this.buttonArriere.UseVisualStyleBackColor = true;
            this.buttonArriere.Click += new System.EventHandler(this.buttonDeplacement_Click);
            // 
            // buttonArret
            // 
            this.buttonArret.Location = new System.Drawing.Point(126, 103);
            this.buttonArret.Name = "buttonArret";
            this.buttonArret.Size = new System.Drawing.Size(84, 50);
            this.buttonArret.TabIndex = 3;
            this.buttonArret.Text = "Arret";
            this.buttonArret.UseVisualStyleBackColor = true;
            this.buttonArret.Click += new System.EventHandler(this.buttonDeplacement_Click);
            // 
            // buttonDroite
            // 
            this.buttonDroite.Location = new System.Drawing.Point(216, 103);
            this.buttonDroite.Name = "buttonDroite";
            this.buttonDroite.Size = new System.Drawing.Size(84, 50);
            this.buttonDroite.TabIndex = 2;
            this.buttonDroite.Text = "Droite";
            this.buttonDroite.UseVisualStyleBackColor = true;
            this.buttonDroite.Click += new System.EventHandler(this.buttonDeplacement_Click);
            // 
            // buttonGauche
            // 
            this.buttonGauche.Location = new System.Drawing.Point(36, 103);
            this.buttonGauche.Name = "buttonGauche";
            this.buttonGauche.Size = new System.Drawing.Size(84, 50);
            this.buttonGauche.TabIndex = 1;
            this.buttonGauche.Text = "Gauche";
            this.buttonGauche.UseVisualStyleBackColor = true;
            this.buttonGauche.Click += new System.EventHandler(this.buttonDeplacement_Click);
            // 
            // buttonAvant
            // 
            this.buttonAvant.Location = new System.Drawing.Point(126, 47);
            this.buttonAvant.Name = "buttonAvant";
            this.buttonAvant.Size = new System.Drawing.Size(84, 50);
            this.buttonAvant.TabIndex = 0;
            this.buttonAvant.Text = "Avant";
            this.buttonAvant.UseVisualStyleBackColor = true;
            this.buttonAvant.Click += new System.EventHandler(this.buttonDeplacement_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.buttonLed);
            this.groupBox4.Controls.Add(this.comboBox1);
            this.groupBox4.Controls.Add(this.label11);
            this.groupBox4.Location = new System.Drawing.Point(3, 286);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(208, 154);
            this.groupBox4.TabIndex = 4;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Test led";
            // 
            // buttonLed
            // 
            this.buttonLed.Location = new System.Drawing.Point(18, 91);
            this.buttonLed.Name = "buttonLed";
            this.buttonLed.Size = new System.Drawing.Size(169, 31);
            this.buttonLed.TabIndex = 2;
            this.buttonLed.Text = "Tester led";
            this.buttonLed.UseVisualStyleBackColor = true;
            this.buttonLed.Click += new System.EventHandler(this.buttonLed_Click);
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "Eteinte",
            "Attente",
            "Actif",
            "Batterie faible",
            "Watchdog"});
            this.comboBox1.Location = new System.Drawing.Point(66, 39);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 21);
            this.comboBox1.TabIndex = 1;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(15, 42);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(28, 13);
            this.label11.TabIndex = 0;
            this.label11.Text = "Led:";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.buttonEnregistrer);
            this.groupBox6.Controls.Add(this.buttonEnvoyer);
            this.groupBox6.Controls.Add(this.labelDroitRapid);
            this.groupBox6.Controls.Add(this.labelDroitRapide);
            this.groupBox6.Controls.Add(this.trackBarDroitRapide);
            this.groupBox6.Controls.Add(this.labelDroitNormal);
            this.groupBox6.Controls.Add(this.label17);
            this.groupBox6.Controls.Add(this.trackBarDroitNormal);
            this.groupBox6.Controls.Add(this.labelGaucheRapide);
            this.groupBox6.Controls.Add(this.label15);
            this.groupBox6.Controls.Add(this.trackBarGaucheRapide);
            this.groupBox6.Controls.Add(this.labelGaucheNormal);
            this.groupBox6.Controls.Add(this.label12);
            this.groupBox6.Controls.Add(this.trackBarGaucheNormal);
            this.groupBox6.Location = new System.Drawing.Point(217, 286);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(481, 154);
            this.groupBox6.TabIndex = 5;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Reglage moteurs";
            // 
            // buttonEnregistrer
            // 
            this.buttonEnregistrer.Location = new System.Drawing.Point(350, 91);
            this.buttonEnregistrer.Name = "buttonEnregistrer";
            this.buttonEnregistrer.Size = new System.Drawing.Size(102, 32);
            this.buttonEnregistrer.TabIndex = 13;
            this.buttonEnregistrer.Text = "Enregistrer";
            this.buttonEnregistrer.UseVisualStyleBackColor = true;
            this.buttonEnregistrer.Click += new System.EventHandler(this.buttonEnregistrer_Click);
            // 
            // buttonEnvoyer
            // 
            this.buttonEnvoyer.Location = new System.Drawing.Point(350, 37);
            this.buttonEnvoyer.Name = "buttonEnvoyer";
            this.buttonEnvoyer.Size = new System.Drawing.Size(102, 32);
            this.buttonEnvoyer.TabIndex = 12;
            this.buttonEnvoyer.Text = "Envoyer";
            this.buttonEnvoyer.UseVisualStyleBackColor = true;
            this.buttonEnvoyer.Click += new System.EventHandler(this.buttonEnvoyer_Click);
            // 
            // labelDroitRapid
            // 
            this.labelDroitRapid.Location = new System.Drawing.Point(293, 81);
            this.labelDroitRapid.Name = "labelDroitRapid";
            this.labelDroitRapid.Size = new System.Drawing.Size(40, 23);
            this.labelDroitRapid.TabIndex = 11;
            this.labelDroitRapid.Text = "0";
            this.labelDroitRapid.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelDroitRapide
            // 
            this.labelDroitRapide.AutoSize = true;
            this.labelDroitRapide.Location = new System.Drawing.Point(252, 28);
            this.labelDroitRapide.Name = "labelDroitRapide";
            this.labelDroitRapide.Size = new System.Drawing.Size(61, 13);
            this.labelDroitRapide.TabIndex = 10;
            this.labelDroitRapide.Text = "Droit rapide";
            // 
            // trackBarDroitRapide
            // 
            this.trackBarDroitRapide.Location = new System.Drawing.Point(255, 44);
            this.trackBarDroitRapide.Maximum = 256;
            this.trackBarDroitRapide.Name = "trackBarDroitRapide";
            this.trackBarDroitRapide.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBarDroitRapide.Size = new System.Drawing.Size(45, 104);
            this.trackBarDroitRapide.TabIndex = 9;
            this.trackBarDroitRapide.Scroll += new System.EventHandler(this.trackBarMoteurs_Scroll);
            // 
            // labelDroitNormal
            // 
            this.labelDroitNormal.Location = new System.Drawing.Point(213, 81);
            this.labelDroitNormal.Name = "labelDroitNormal";
            this.labelDroitNormal.Size = new System.Drawing.Size(40, 23);
            this.labelDroitNormal.TabIndex = 8;
            this.labelDroitNormal.Text = "0";
            this.labelDroitNormal.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(174, 28);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(63, 13);
            this.label17.TabIndex = 7;
            this.label17.Text = "Droit normal";
            // 
            // trackBarDroitNormal
            // 
            this.trackBarDroitNormal.Location = new System.Drawing.Point(177, 44);
            this.trackBarDroitNormal.Maximum = 256;
            this.trackBarDroitNormal.Name = "trackBarDroitNormal";
            this.trackBarDroitNormal.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBarDroitNormal.Size = new System.Drawing.Size(45, 104);
            this.trackBarDroitNormal.TabIndex = 6;
            this.trackBarDroitNormal.Scroll += new System.EventHandler(this.trackBarMoteurs_Scroll);
            // 
            // labelGaucheRapide
            // 
            this.labelGaucheRapide.Location = new System.Drawing.Point(128, 81);
            this.labelGaucheRapide.Name = "labelGaucheRapide";
            this.labelGaucheRapide.Size = new System.Drawing.Size(40, 23);
            this.labelGaucheRapide.TabIndex = 5;
            this.labelGaucheRapide.Text = "0";
            this.labelGaucheRapide.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(91, 28);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(77, 13);
            this.label15.TabIndex = 4;
            this.label15.Text = "Gauche rapide";
            // 
            // trackBarGaucheRapide
            // 
            this.trackBarGaucheRapide.Location = new System.Drawing.Point(94, 44);
            this.trackBarGaucheRapide.Maximum = 256;
            this.trackBarGaucheRapide.Name = "trackBarGaucheRapide";
            this.trackBarGaucheRapide.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBarGaucheRapide.Size = new System.Drawing.Size(45, 104);
            this.trackBarGaucheRapide.TabIndex = 3;
            this.trackBarGaucheRapide.Scroll += new System.EventHandler(this.trackBarMoteurs_Scroll);
            // 
            // labelGaucheNormal
            // 
            this.labelGaucheNormal.Location = new System.Drawing.Point(45, 81);
            this.labelGaucheNormal.Name = "labelGaucheNormal";
            this.labelGaucheNormal.Size = new System.Drawing.Size(40, 23);
            this.labelGaucheNormal.TabIndex = 2;
            this.labelGaucheNormal.Text = "0";
            this.labelGaucheNormal.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(6, 28);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(79, 13);
            this.label12.TabIndex = 1;
            this.label12.Text = "Gauche normal";
            // 
            // trackBarGaucheNormal
            // 
            this.trackBarGaucheNormal.Location = new System.Drawing.Point(9, 44);
            this.trackBarGaucheNormal.Maximum = 256;
            this.trackBarGaucheNormal.Name = "trackBarGaucheNormal";
            this.trackBarGaucheNormal.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBarGaucheNormal.Size = new System.Drawing.Size(45, 104);
            this.trackBarGaucheNormal.TabIndex = 0;
            this.trackBarGaucheNormal.Scroll += new System.EventHandler(this.trackBarMoteurs_Scroll);
            // 
            // timerCommande
            // 
            this.timerCommande.Interval = 1000;
            this.timerCommande.Tick += new System.EventHandler(this.timerCommande_Tick);
            // 
            // timerWatchdog
            // 
            this.timerWatchdog.Interval = 500;
            this.timerWatchdog.Tick += new System.EventHandler(this.timerWatchdog_Tick);
            // 
            // Principal
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(698, 444);
            this.Controls.Add(this.groupBox6);
            this.Controls.Add(this.groupBox4);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Principal";
            this.Text = "Controleur Robot";
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVitesse)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarDroitRapide)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarDroitNormal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGaucheRapide)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarGaucheNormal)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem updateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonConnecter;
        private System.Windows.Forms.Label labelConnecter;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label labelOdoDroit;
        private System.Windows.Forms.Label labelOdoGauche;
        private System.Windows.Forms.Label labelCapteur;
        private System.Windows.Forms.Label labelBatterie;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Label labelCommandeCourante;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TrackBar trackBarVitesse;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Button buttonArriere;
        private System.Windows.Forms.Button buttonArret;
        private System.Windows.Forms.Button buttonDroite;
        private System.Windows.Forms.Button buttonGauche;
        private System.Windows.Forms.Button buttonAvant;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.Button buttonLed;
        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.Button buttonEnregistrer;
        private System.Windows.Forms.Button buttonEnvoyer;
        private System.Windows.Forms.Label labelDroitRapid;
        private System.Windows.Forms.Label labelDroitRapide;
        private System.Windows.Forms.TrackBar trackBarDroitRapide;
        private System.Windows.Forms.Label labelDroitNormal;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.TrackBar trackBarDroitNormal;
        private System.Windows.Forms.Label labelGaucheRapide;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TrackBar trackBarGaucheRapide;
        private System.Windows.Forms.Label labelGaucheNormal;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.TrackBar trackBarGaucheNormal;
        private System.Windows.Forms.Timer timerCommande;
        private System.Windows.Forms.Timer timerWatchdog;
        private System.Windows.Forms.ToolStripMenuItem logToolStripMenuItem;
    }
}

