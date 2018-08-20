using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;

namespace Controleur_Robot
{
    public partial class Principal : Form
    {
        CtrlRobot ctrlRobot;
        Log log;
        int FwMajorVer, FwMinorVer;

        const int IhmMajorVer = 1;
        const int IhmMinorVer = 2;

        bool wdt_flipflop=true;

        public Principal()
        {
            InitializeComponent();

            log = new Log();

            ctrlRobot = new CtrlRobot(log);

            DesactivationElements();
        }

        private void DesactivationElements()
        {
            groupBox2.Enabled = false;
            groupBox3.Enabled = false;
            groupBox4.Enabled = false;
            groupBox5.Enabled = false;
            groupBox6.Enabled = false;
        }

        private void ActivationElements()
        {
            groupBox2.Enabled = true;
            groupBox3.Enabled = true;
            groupBox4.Enabled = true;
            groupBox5.Enabled = true;

            if ((FwMajorVer > 1) ||
                ((FwMajorVer == 1) && (FwMinorVer >= 1)))
            {
                int motorLeftNormal, motorLeftTurbo, motorRightNormal, motorRightTurbo;
 
                groupBox6.Enabled = true;

                ctrlRobot.GetParams(out motorLeftNormal, out motorLeftTurbo, out motorRightNormal, out motorRightTurbo);

                trackBarGaucheNormal.Value = motorLeftNormal;
                trackBarGaucheRapide.Value = motorLeftTurbo;
                trackBarDroitNormal.Value = motorRightNormal;
                trackBarDroitRapide.Value = motorRightTurbo;

                labelGaucheNormal.Text = Convert.ToString(motorLeftNormal);
                labelGaucheRapide.Text = Convert.ToString(motorLeftTurbo);
                labelDroitNormal.Text = Convert.ToString(motorRightNormal);
                labelDroitRapid.Text = Convert.ToString(motorRightTurbo);
            }
        }

        private void buttonDeplacement_Click(object sender, EventArgs e)
        {
            int vitesse;

            vitesse = trackBarVitesse.Value;

            if (sender == buttonArret)
            {
                labelCommandeCourante.Text = "Arret";
                ctrlRobot.SetMotors(CtrlRobot.MOTORS.Arret, CtrlRobot.MOTORS.Arret);
            }
            else if (sender == buttonArriere)
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Arriere normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.ArriereNormal, 
                                        CtrlRobot.MOTORS.ArriereNormal);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Arriere rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.ArriereRapide,
                                        CtrlRobot.MOTORS.ArriereRapide);
                }
            }
            else if (sender == buttonAvant)
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Avant normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantNormal,
                                        CtrlRobot.MOTORS.AvantNormal);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Avant rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantRapide,
                                        CtrlRobot.MOTORS.AvantRapide);
                }
            }
            else if (sender == buttonDroite)
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Droite normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantNormal,
                                        CtrlRobot.MOTORS.Arret);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Droite rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.AvantRapide,
                                        CtrlRobot.MOTORS.Arret);
                }
            }
            else /* Forcement, c'est le gauche ! */
            {
                if (vitesse == 1) /* vitesse normale */
                {
                    labelCommandeCourante.Text = "Gauche normal";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.Arret,
                                        CtrlRobot.MOTORS.AvantNormal);
                }
                else /* vitesse rapide */
                {
                    labelCommandeCourante.Text = "Gauche rapide";
                    ctrlRobot.SetMotors(CtrlRobot.MOTORS.Arret,
                                        CtrlRobot.MOTORS.AvantRapide);
                }
            }
        }

        private void buttonEnvoyer_Click(object sender, EventArgs e)
        {
            /*MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  */
            ctrlRobot.SetMotorsSpeed(trackBarGaucheNormal.Value,
                                     trackBarGaucheRapide.Value,
                                     trackBarDroitNormal.Value,
                                     trackBarDroitRapide.Value);
        }

        private void buttonEnregistrer_Click(object sender, EventArgs e)
        {
            /*MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk); */
            ctrlRobot.RecordParams();
        }

        private void trackBarMoteurs_Scroll(object sender, EventArgs e)
        {
            if (sender == trackBarDroitNormal)
                    labelDroitNormal.Text = Convert.ToString(trackBarDroitNormal.Value);
            else if (sender == trackBarDroitRapide)
                    labelDroitRapid.Text = Convert.ToString(trackBarDroitRapide.Value);
            else if (sender == trackBarGaucheNormal)
                    labelGaucheNormal.Text = Convert.ToString(trackBarGaucheNormal.Value);
            else /* forcement c'est trackBarGaucheRapide */
                    labelGaucheRapide.Text = Convert.ToString(trackBarGaucheRapide.Value);
        }

        private void buttonLed_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  
        }

        private void buttonConnecter_Click(object sender, EventArgs e)
        {
            string portname;
            string[] portlist;
            CtrlRobot.OPENSTATUS status;

            if (ctrlRobot.IsOpen == false)
            {
                portlist = SerialPort.GetPortNames();
                portname = portlist[portlist.Length - 1];
                
                /* Ouverture du port et test de la presence du robot */
                status = ctrlRobot.Open(portname);
                
                if (status == CtrlRobot.OPENSTATUS.TimeoutErr)
                {
                    MessageBox.Show("Aucun robot sur le port " + portname,
                                    "Erreur port serie", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                    return;
                }

                if (status == CtrlRobot.OPENSTATUS.ComErr)
                {
                    MessageBox.Show("Erreur lors de l'ouverture du port " + portname + "\r\n",
                                   "Erreur port serie", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);

                    return;
                }

                /* Si tout s'est bien passé, verification de la version du FW */
                ctrlRobot.GetVersion(out FwMajorVer, out FwMinorVer);

                /* Puis demarrage du robot */
                ctrlRobot.Start();

                /* et lancement du timer de watchdog */
                timerWatchdog.Enabled = true;
                wdt_flipflop = true;

                ActivationElements();

                labelConnecter.Text = "connecté (" + portname + ")";
                buttonConnecter.Text = "Deconnecter";
                timerCommande.Enabled = true;
            }
            else
            {
                timerCommande.Enabled = false;
                /* et arret du timer de watchdog */
                timerWatchdog.Enabled = false;

                ctrlRobot.Stop(); 
                ctrlRobot.Close();

                DesactivationElements();

                labelConnecter.Text = "non connecté";
                buttonConnecter.Text = "Connecter";
            }
        }

        private void updateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("Ne soit pas aussi impatient, petit scarabée !", "Fonction non implementée",
                 MessageBoxButtons.OK, MessageBoxIcon.Asterisk);  
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            About about = new About();
            
            about.SetVersionIHM(Properties.Resources.MAJOR_VERSION, Properties.Resources.MINOR_VERSION);
            about.SetVersionFW(Convert.ToString(FwMajorVer), Convert.ToString(FwMinorVer));
            about.Show();
        }

        private void timerCommande_Tick(object sender, EventArgs e)
        {
            CtrlRobot.VBAT vbat;
            CtrlRobot.SENSOR sensor;

            int odometrieGauche, odometrieDroite;

            /* Recuperation de l'etat de la batterie */
            vbat = ctrlRobot.GetVBat();

            switch (vbat)
            {
                case CtrlRobot.VBAT.Inconnu:
                    labelBatterie.Text = "Inconnu";
                    break;
                case CtrlRobot.VBAT.Vide:
                    labelBatterie.Text = "Vide";
                    break;
                case CtrlRobot.VBAT.Faible:
                    labelBatterie.Text = "Faible";
                    break;
                case CtrlRobot.VBAT.Normal:
                    labelBatterie.Text = "Normale";
                    break;
            }

            /* Recuperation de l'etat du capteur */
            sensor = ctrlRobot.GetSensor();

            switch (sensor)
            {
                case CtrlRobot.SENSOR.Inconnu:
                    labelCapteur.Text = "Inconnu";
                    break;
                case CtrlRobot.SENSOR.Vide:
                    labelCapteur.Text = "Vide";
                    break;
                case CtrlRobot.SENSOR.Present:
                    labelCapteur.Text = "Present";
                    break;
            }

            /* Recuperation de l'odometrie*/
            ctrlRobot.GetOdometrie(out odometrieGauche, out odometrieDroite);

            labelOdoGauche.Text = Convert.ToString(odometrieGauche);
            labelOdoDroit.Text = Convert.ToString(odometrieDroite);
        }

        private void timerWatchdog_Tick(object sender, EventArgs e)
        {
            if (wdt_flipflop == true)
            {
                ctrlRobot.ReloadWdt();
                wdt_flipflop = false;
            }
            else
            {
                wdt_flipflop = true;
            }
        }

        private void logToolStripMenuItem_Click(object sender, EventArgs e)
        {
            log.Show();
        }
    }
}
