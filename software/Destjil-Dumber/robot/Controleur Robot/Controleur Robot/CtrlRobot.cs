using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;

namespace Controleur_Robot
{
    public class CtrlRobot: SerialPort 
    {
        private const string CMD_PING="p";
        private const string CMD_SET_MOTORS="m";
        private const string CMD_START_UNSECURE = "u";
        private const string CMD_START_WATCHDOG="W";
        private const string CMD_RELOAD_WATCHDOG="w";
        private const string CMD_GET_SENSOR="s";
        private const string CMD_GET_VBAT="v";
        private const string CMD_GET_ODO="o";
        private const string CMD_RESET="r";
        private const string CMD_VERSION = "V";
        private const string CMD_LED = "l";
        private const string CMD_SET_MOTORS_LEFT_SPEED_NORMAL = "Y";
        private const string CMD_SET_MOTORS_LEFT_SPEED_TURBO = "y";
        private const string CMD_SET_MOTORS_RIGHT_SPEED_NORMAL = "Z";
        private const string CMD_SET_MOTORS_RIGHT_SPEED_TURBO = "z";
        private const string CMD_RECORD_PARAMS = "R";
        private const string CMD_GET_PARAMS = "G";

        private const int MAX_RETRY = 3;

        private const char ANS_OK = 'o';
        private const char ANS_OK_UPPERCASE = 'O';
        private const char ANS_UNKNOWN_CMD='c';
        private const char ANS_UNKNOWN_CMD_UPPERCASE='C';
        private const char ANS_INVALID_PARAMS='e';
        private const char ANS_INVALID_PARAMS_UPPERCASE='E';

        private Log log;

        public enum MOTORS
        {
            Arret=0,
            AvantNormal=1,
            AvantRapide=2,
            ArriereNormal=-1,
            ArriereRapide=-2
        };

        public enum VBAT
        {
            Normal=2,
            Faible=1,
            Vide=0,
            Inconnu=-1
        };

        public enum OPENSTATUS
        { 
            Success,
            TimeoutErr,
            ComErr
        };

        public enum CMD_STATUS
        {
            Success,
            InvalidCmd,
            InvalidParams,
            Timeout
        };

        public enum SENSOR
        {
            Inconnu=-1,
            Vide=0,
            Present=1
        };

        public CtrlRobot()
        {
            this.log = null;
        }

        public CtrlRobot(Log log)
        {
            if (log != null)
            {
                this.log = log;
            }
        }

        new public OPENSTATUS Open()
        {
            OPENSTATUS etat;

            try
            {
                Open(SerialPort.GetPortNames()[0]);
                etat = OPENSTATUS.Success;
            }
            catch (Exception)
            {
                etat = OPENSTATUS.ComErr;
            }

            return etat;
        }

        public OPENSTATUS Open(string PortName)
        {
            OPENSTATUS etat;

            base.NewLine = "\n";
            base.ReadBufferSize = 100;
            base.WriteBufferSize = 100;
            base.ReadTimeout = 50;
            base.WriteTimeout = SerialPort.InfiniteTimeout;

            base.BaudRate = 9600;
            base.DataBits = 8;
            base.Parity = Parity.None;
            base.PortName = PortName;

            try
            {
                base.Open();

                etat = OPENSTATUS.Success;

                Print("Port " + PortName + " ouvert (" + Convert.ToString(DateTime.UtcNow) + ")");
            }
            catch (Exception)
            {
                etat = OPENSTATUS.ComErr;

                Print("[ERR] Echec lors de l'ouverture du port " + PortName + " (" + Convert.ToString(DateTime.UtcNow) + ")");
            }

            if (etat == OPENSTATUS.Success)
            {
                // Test si un robot est bien present (commande ping)
                if (Ping() != CMD_STATUS.Success)
                {
                    etat = OPENSTATUS.TimeoutErr;

                    Print("[ERR] Echec lors de la recherche d'un robot sur le port " + PortName + " (" + Convert.ToString(DateTime.UtcNow) + ")");
                    Close();
                }
            }

            return etat;
        }

        public new void Close()
        {
            base.Close();

            Print ("Port " + PortName + " fermé (" + Convert.ToString(DateTime.UtcNow) + ")");
        }

        /*
         * Print
         * Permet d'enregistrer le log des actions faites sur la liaison serie
         */
        private void Print(string text)
        {
            if (this.log != null)
            {
                log.WriteLine(text);
            }
        }

        /*
         * SendCommand
         * Envoi une commande au robot
         * Forme 1 : Retourne la reponse
         */
        private CMD_STATUS SendCommand(string command, out string answer)
        {
	        int nb_retry=0;
            bool cmd_succeed=false;
            string ans;
            CMD_STATUS status = CMD_STATUS.Success;

            ans = "";

            do
            {
                cmd_succeed = true;
                DiscardInBuffer();
                DiscardOutBuffer();

                /* Envoi de la commande */
                Print("Envoi de la commande: " + command);
                base.WriteLine(command);

                /* Recuperation de la reponse */
                try
                {
                    ans = base.ReadLine();
                }
                catch (TimeoutException)
                {
                    /* Probleme de timeout */
                    Print("[ERR] Timeout lors de la reponse à la commande " + command);
                    nb_retry++;
                    cmd_succeed = false;
                }
            }
            while ((nb_retry < MAX_RETRY) && (cmd_succeed == false));

	        if (nb_retry >= MAX_RETRY)
	        {
		        //throw new System.TimeoutException("La commande a dépassé son nombre maximal de re-essais");
                Print("[ERR] Timeout: La commande a dépassé son nombre maximal de re-essais");
                answer = ans;
                return CMD_STATUS.Timeout;
	        }

            Print("Reponse recue: " + ans);

	        /* Analyse de la reponse */
            if (ans.Length < 1) ans = Convert.ToString(ANS_UNKNOWN_CMD);

            switch (ans[0])
	        {
	        case ANS_UNKNOWN_CMD:
	        case ANS_UNKNOWN_CMD_UPPERCASE:
		        Print ("[ERR] Attention: commande inconnue ou non reconnue");
                status = CMD_STATUS.InvalidCmd;
		        //throw new System.NotImplementedException("La commande " + command + " n'est pas supportée");
		        break;
	        case ANS_INVALID_PARAMS:
	        case ANS_INVALID_PARAMS_UPPERCASE:
		        Print ("[ERR] Attention: parametres invalides ou non reconnus");
                status = CMD_STATUS.InvalidParams;
		        //throw new System.ArgumentException("Les parametres de la commande " + command + " ne sont pas valides");
		        break;
	        case ANS_OK:
	        case ANS_OK_UPPERCASE:
		        Print ("OK");
		        break;
	        default:
		        break;
	        }

            answer = ans;

            return status;
        }

        /*
         * SendCommand
         * Envoi une commande au robot
         * Forme 2 : Ne retourne pas la reponse
         */
        private CMD_STATUS SendCommand(string command)
        {
	        int nb_retry=0;
            bool cmd_succeed=true;
            CMD_STATUS status=CMD_STATUS.Success;

            string answer="";

	        do
	        {
                cmd_succeed = true;
                DiscardInBuffer();
                DiscardOutBuffer();

		        /* Envoi de la commande */
                Print("Envoi de la commande: " + command);
                base.WriteLine(command);
                
                /* Recuperation de la reponse */
                try
                {
                    answer = base.ReadLine();
                }
                catch (TimeoutException)
                {
                    /* Probleme de timeout */
                    Print("[ERR] Timeout lors de la reponse à la commande " + command); 
                    nb_retry ++;
                    cmd_succeed = false;
                }
	        }
            while ((nb_retry < MAX_RETRY) && (cmd_succeed == false)) ;

	        if (nb_retry >= MAX_RETRY)
	        {
		        //throw new System.TimeoutException("La commande a dépassé son nombre maximal de re-essais");
                Print("[ERR] Timeout: La commande a dépassé son nombre maximal de re-essais");
                return CMD_STATUS.Timeout;
	        }

	        Print ("reponse recue: " + answer);

	        /* Analyse de la reponse */
            if (answer.Length < 1) answer = Convert.ToString(ANS_UNKNOWN_CMD);

	        switch (answer[0])
	        {
	        case ANS_UNKNOWN_CMD:
	        case ANS_UNKNOWN_CMD_UPPERCASE:
		        Print ("[ERR] Attention: commande inconnue ou non reconnue");
                status = CMD_STATUS.InvalidCmd;
		        //throw new System.NotImplementedException("La commande " + command + " n'est pas supportée");
		        break;
	        case ANS_INVALID_PARAMS:
	        case ANS_INVALID_PARAMS_UPPERCASE:
		        Print ("[ERR] Attention: parametres invalides ou non reconnus");
                status = CMD_STATUS.InvalidParams;
		        //throw new System.ArgumentException("Les parametres de la commande " + command + " ne sont pas valides");
		        break;
	        case ANS_OK:
	        case ANS_OK_UPPERCASE:
		        Print ("OK");
		        break;
	        default:
		        break;
	        }

            return status;
        }

        /*
         * Commande PING
         * Teste la presence du robot
         */
        public CMD_STATUS Ping()
        {
            return SendCommand(CMD_PING);  
        }

        /*
         * Commande START_WATCHDOG
         * Demarre le watchdog du robot (et fait clignoter la led)
         */
        public CMD_STATUS Start()
        {
	        return SendCommand(CMD_START_UNSECURE);
        }

        /*
         * Commande RESET
         * Arrete le watchdog du robot (et allume la led en continu)
         */
        public CMD_STATUS Stop()
        {
            return SendCommand(CMD_RESET);
        }

        /*
         * Commande SET_MOTORS
         * Commande les moteurs
         */
        public CMD_STATUS SetMotors(MOTORS moteurGauche, MOTORS moteurDroit)
        {
            int valMoteurGauche, valMoteurDroit;

            valMoteurGauche = Convert.ToInt32(moteurGauche);
            valMoteurDroit = Convert.ToInt32(moteurDroit);

            return SendCommand(CMD_SET_MOTORS + "=" + valMoteurGauche + "," + valMoteurDroit);
        }

        /*
         * Commande CMD_RELOAD_WATCHDOG
         * Relacne le Watchdog
         */
        public CMD_STATUS ReloadWdt()
        {
            return SendCommand(CMD_RELOAD_WATCHDOG);
        }

        /*
         * Commande CMD_GET_SENSOR
         * Recupere l'etat du capteur
         */
        public SENSOR GetSensor()
        {
            SENSOR etat;
            string answer;
            string[] items;

            int valeur;

            if (SendCommand(CMD_GET_SENSOR, out answer)== CMD_STATUS.Success)
            {
                items = answer.Split(':');

                try
                {
                    valeur = Convert.ToInt32(items[1]);
                }
                catch (Exception e)
                {
                    Print("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                    valeur = -1;
                }
            }
            else
            {
                valeur = -1;
            }

            switch (valeur)
            {
                case 0:
                    etat = SENSOR.Vide;
                    break;
                case 1:
                    etat = SENSOR.Present;
                    break;
                case -1:
                    etat = SENSOR.Inconnu;
                    break;
                default:
                    //throw new System.ArgumentException("Reponse inconnue: " + answer);
                    Print("[ERR] Attention: reponse inconnue (" + answer + ")");
                    etat = SENSOR.Inconnu;
                    break;
            }

            return etat;
        }

        /*
         * Commande CMD_GET_ODO
         * Recupere l'odometrie du robot
         */
        public void GetOdometrie(out int odometrieGauche, out int odometrieDroite)
        {
            string answer;
            string[] items;

            odometrieGauche = 0;
            odometrieDroite = 0;

            if (SendCommand(CMD_GET_ODO, out answer) == CMD_STATUS.Success)
            {
                items = answer.Split(new Char[] { ':', ',' });

                try
                {
                    odometrieGauche = Convert.ToInt32(items[1]);
                    odometrieDroite = Convert.ToInt32(items[2]);
                }
                catch (Exception e)
                {
                    Print("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                    odometrieGauche = 0;
                    odometrieDroite = 0;
                }
            }
        }

        /*
         * Commande CMD_GET_VBAT
         * Recupere l'etat de la batterie
         */
        public VBAT GetVBat()
        {
            string answer;
            string[] items;
            int valeur;
            VBAT etat;

            if (SendCommand(CMD_GET_VBAT, out answer) == CMD_STATUS.Success)
            {
                items = answer.Split(':');

                try
                {
                    valeur = Convert.ToInt32(items[1]);
                }
                catch (Exception e)
                {
                    Print("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                    valeur = -1;
                }
            }
            else
            {
                valeur = -1;
            }

            switch (valeur)
            {
                case 0:
                    etat = VBAT.Vide;
                    break;
                case 1:
                    etat = VBAT.Faible;
                    break;
                case 2:
                    etat = VBAT.Normal;
                    break;
                case -1:
                    etat = VBAT.Inconnu;
                    break;
                default:
                    //throw new System.ArgumentException("Reponse inconnue: " + answer);
                    Print("[ERR] Attention: Reponse inconnue (" + answer + ")");
                    etat = VBAT.Inconnu;
                    break;
            }

            return etat;
        }

        /*
         * Commande CMD_GET_VERSION
         * Recupere le numero de version du firmware du robot
         */
        public void GetVersion(out int versionMajor, out int versionMinor)
        {
            string answer;
            string[] items;

            if (SendCommand(CMD_VERSION, out answer) == CMD_STATUS.Success)
            {
                items = answer.Split(new char[]{':',','});

                try
                {
                    versionMajor = Convert.ToInt32(items[1]);
                    versionMinor = Convert.ToInt32(items[2]);
                }
                catch (Exception e)
                {
                    Print("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                    versionMajor = -1;
                    versionMinor = -1;
                }
            }
            else
            {
                versionMajor = -1;
                versionMinor = -1;
            }
        }

        /*
         * Commande CMD_SET_MOTORS_SPEED
         * Regle les vitesses de rotation des moteurs
         */
        public CMD_STATUS SetMotorsSpeed(int motorLeftNormal, int motorLeftTurbo,
                                         int motorRightNormal, int motorRightTurbo)
        {
            CMD_STATUS status;
            int re_essai = -1;
            const int MAX_RE_ESSAI = 3;

            if ((motorLeftNormal < 0) || (motorLeftTurbo < 0) ||
                (motorRightNormal < 0) || (motorRightTurbo < 0))
            {
                throw new System.ArgumentOutOfRangeException();
            }

            if ((motorLeftNormal > 255) || (motorLeftTurbo > 255) ||
                (motorRightNormal > 255) || (motorRightTurbo > 255))
            {
                throw new System.ArgumentOutOfRangeException();
            }

            do
            {
                status=SendCommand(CMD_SET_MOTORS_LEFT_SPEED_NORMAL + "=" + motorLeftNormal);
            }
            while ((re_essai<MAX_RE_ESSAI) && 
                   ((status == CMD_STATUS.InvalidCmd) || (status == CMD_STATUS.InvalidParams)));

            if (status != CMD_STATUS.Success) return status;

            do
            {
                status=SendCommand(CMD_SET_MOTORS_LEFT_SPEED_TURBO + "=" + motorLeftTurbo);
            }
            while ((re_essai<MAX_RE_ESSAI) && 
                   ((status == CMD_STATUS.InvalidCmd) || (status == CMD_STATUS.InvalidParams)));

            if (status != CMD_STATUS.Success) return status;

            do
            {
                status=SendCommand(CMD_SET_MOTORS_RIGHT_SPEED_NORMAL + "=" + motorRightNormal);
            }
            while ((re_essai<MAX_RE_ESSAI) && 
                   ((status == CMD_STATUS.InvalidCmd) || (status == CMD_STATUS.InvalidParams)));

            if (status != CMD_STATUS.Success) return status;

            do
            {
                status=SendCommand(CMD_SET_MOTORS_RIGHT_SPEED_TURBO + "=" + motorRightTurbo);
            }
            while ((re_essai<MAX_RE_ESSAI) && 
                   ((status == CMD_STATUS.InvalidCmd) || (status == CMD_STATUS.InvalidParams)));

            return status;
        }

        /*
         * Commande CMD_RECORD_PARAMS
         * Enregistre les parametres en EEPROM
         */
        public CMD_STATUS RecordParams()
        {
            return SendCommand(CMD_RECORD_PARAMS);
        }

        /*
         * commande CMD_GET_PARAMS
         * Recupere l'etat actuel des parametres
         */
        public void GetParams(out int motorLeftNormal, out int motorLeftTurbo,
                              out int motorRightNormal, out int motorRightTurbo)
        {
            string answer;
            string[] items;

            if (SendCommand(CMD_GET_PARAMS, out answer) == CMD_STATUS.Success)
            {
                items = answer.Split(new char[] { ':', ',' });

                try
                {
                    motorLeftNormal = Convert.ToInt32(items[1]);
                    motorLeftTurbo = Convert.ToInt32(items[2]);
                    motorRightNormal = Convert.ToInt32(items[3]);
                    motorRightTurbo = Convert.ToInt32(items[4]);
                }
                catch (Exception e)
                {
                    Print("[ERR] Probleme lors de l'analyse de la reponse " + answer + "\r\nException: " + e);
                    motorLeftNormal = 0;
                    motorLeftTurbo = 0;
                    motorRightNormal = 0;
                    motorRightTurbo = 0;
                }
            }
            else
            {
                motorLeftNormal = 0;
                motorLeftTurbo = 0;
                motorRightNormal = 0;
                motorRightTurbo = 0;
            }
        }
    }
}
