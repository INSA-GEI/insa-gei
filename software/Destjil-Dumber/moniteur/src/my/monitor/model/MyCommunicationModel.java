/*
 * Modif : 28/03/2012 PEH remise à jour du modèle camera lors de la perte de 
 * connexion
 */
package my.monitor.model;

import java.awt.Point;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Date;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import my.monitor.controller.MyController;
import my.monitor.model.enumeration.CommunicationStatus;
import my.monitor.model.enumeration.MissionType;
import my.monitor.model.enumeration.MyDirection;
import my.monitor.model.enumeration.MyLevel;
import my.monitor.model.enumeration.MyMode;
import my.monitor.model.enumeration.Order;
import my.monitor.model.enumeration.RobotMissionStatus;
import my.monitor.model.enumeration.RobotStatus;
import my.monitor.model.enumeration.VideoStatus;

/**
 *
 * @author pehladik
 */
public class MyCommunicationModel extends AbstractModel {

    /**
     * Varibale de débug
     */
    boolean DEBUG = true;
    /**
     * Données du modèle
     */
    Movement move = new Movement(MyDirection.STOP, 50);
    Arena arena = null;
    Point objective = new Point(-1, -1);
    RobotMissionStatus missionStatus = RobotMissionStatus.NO_MISSION;
    int missionNumber = 0;
    /**
     * Etats du GUI
     */
    Boolean demarrageRapide = false;
    RobotStatus robStatus = RobotStatus.CONNECTED;
    VideoStatus videoStatus = VideoStatus.VIDEO_UNKNOWN;
    MyLevel batteryLevel = MyLevel.UNKNOWN;
    MyMode mode = MyMode.NO_CONTROL;
    String version = "2.1.5 (beta)";
    /**
     * Log
     */
    ArrayList<Message> listMsg = new ArrayList<Message>();
    public static String MESSAGE_NULL = " ";
    public static String MESSAGE_RESET = "RESET";

    public String getVersion() {
        return version;
    }
    /**
     * Communication IP avec le superviseur
     */
    CommunicationStatus commStatus = CommunicationStatus.COMMUNICATION_DISCONNECTED;
    String address;
    Integer port;
    Socket so;
    BufferedOutputStream os;
    BufferedInputStream is;
    MonitorReceive threadAperiodicReceiver;
    MonitorSendPeriodic ms;
    MyImageModel modelImg;
    /**
     * fichier Properties
     */
    FileInputStream fileProperties;
    Properties prop = new Properties();

    public MyCommunicationModel() {
        threadAperiodicReceiver = new MonitorReceive(this);
    }

    public void initDefault(int port, String address) {
        prop = new Properties();
        try {
            fileProperties = new FileInputStream("config.properties");
        } catch (FileNotFoundException ex) {
            Logger.getLogger(MyCommunicationModel.class.getName()).log(Level.SEVERE, null, ex);
            //set the properties value
            prop.setProperty("address", "localhost");
            prop.setProperty("port", "8000");
            try {
                //save properties to project root folder
                prop.store(new FileOutputStream("config.properties"), null);
                fileProperties = new FileInputStream("config.properties");
            } catch (IOException ex1) {
                Logger.getLogger(MyCommunicationModel.class.getName()).log(Level.SEVERE, null, ex1);
            }
        }

        try {
            prop.load(fileProperties);
            String st = prop.getProperty("port");
            setPort(Integer.parseInt(prop.getProperty("port")));
            setAddress(prop.getProperty("address"));
        } catch (IOException ex) {
            Logger.getLogger(MyCommunicationModel.class.getName()).log(Level.SEVERE, null, ex);
        }

        setCommStatus(CommunicationStatus.COMMUNICATION_TRYING);
        setRobotStatus(robStatus.UNKNOWN);
        setBatteryLevel(MyLevel.UNKNOWN);
        setMove(new Movement(MyDirection.FORWARD, 50));
        setDirection(MyDirection.STOP);
        setDemarrageRapide(Boolean.valueOf(false));
        setMessage(null);
    }

    public void setMessage(Message message) {
        if (DEBUG) {
            System.out.println("Màj log " + message);
        }
        Message oldMessage = null;
        
        if (message == null) {
            if (!listMsg.isEmpty()) {
                oldMessage = listMsg.get(listMsg.size() - 1);
                listMsg.clear();
            }
            firePropertyChange(MyController.MESSAGE, oldMessage, message);
        } else {
            if (!listMsg.isEmpty()) {
                oldMessage = listMsg.get(listMsg.size() - 1);
            }
            listMsg.add(message);
            firePropertyChange(MyController.MESSAGE, oldMessage, message);
        }


    }

    public void setCommStatus(CommunicationStatus commStatus) {
        if (DEBUG) {
            System.out.println("Màj communication " + commStatus);
        }
        CommunicationStatus oldCommStatus = this.commStatus;
        if (commStatus == CommunicationStatus.COMMUNICATION_CONNECTED) {
            openConnexion();
        } else if (commStatus == CommunicationStatus.COMMUNICATION_DISCONNECTED) {
            this.commStatus = CommunicationStatus.COMMUNICATION_DISCONNECTED;
            closeConnexion();
        } else {
            this.commStatus = CommunicationStatus.COMMUNICATION_DISCONNECTED;
        }

        if (DEBUG) {
            System.out.println("Tire comm status old:" + oldCommStatus + "new:" + this.commStatus);
        }
        firePropertyChange(MyController.COMMUNICATION_STATUS_PROPERTY, oldCommStatus, this.commStatus);

        /* Demarrage rapide */
        if ((this.commStatus == CommunicationStatus.COMMUNICATION_CONNECTED)
                && (demarrageRapide.booleanValue())) {
            int test = 1;
            while (test == 1) {
                modelImg.setVideoStatus(VideoStatus.VIDEO_ARENA_CALIBRATION);
                test = JOptionPane.showConfirmDialog(null,
                        "Est-ce que l'arène est délimitée par le carré vert ?",
                        "Calibration",
                        JOptionPane.YES_NO_CANCEL_OPTION);
            }
            if (test == 2) {
                modelImg.setVideoStatus(VideoStatus.VIDEO_ARENA_KO);
            } else if (test == 0) {
                modelImg.setVideoStatus(VideoStatus.VIDEO_ARENA_OK);
                modelImg.setPositionComputed(Boolean.valueOf(true));
                setRobotStatus(RobotStatus.WAIT_CONNEXION);
            }
        }
    }

    public void closeConnexion() {
        if (DEBUG) {
            System.out.println("Fermeture de la connexion TCP");
        }
        try {
            so.close();
            os.close();
            is.close();
        } catch (IOException ex) {
            Logger.getLogger(MyCommunicationModel.class.getName()).log(Level.SEVERE, null, ex);
        }
        threadAperiodicReceiver.stopThread();
        //commStatus = CommunicationStatus.COMMUNICATION_DISCONNECTED;
        initDefault(this.port, this.address);
        modelImg.initDefault();
        //ms.stopThread();
    }

    public void openConnexion() {
        if (DEBUG) {
            System.out.println("Ouverture de la connexion TCP avec " + address
                    + " port " + port);
        }
        try {
            so = new Socket(address, port);
            os = new BufferedOutputStream(this.so.getOutputStream());
            is = new BufferedInputStream(this.so.getInputStream());
            commStatus = CommunicationStatus.COMMUNICATION_CONNECTED;
            if (DEBUG) {
                System.out.println("Ouverture réussie");
            }
            threadAperiodicReceiver = new MonitorReceive(this);
            threadAperiodicReceiver.setModelImg(modelImg);
            threadAperiodicReceiver.start();

        } catch (IOException e) {
            commStatus = CommunicationStatus.COMMUNICATION_FAILURE;
            if (DEBUG) {
                System.out.println("Echec");
            }
        }
    }

    public void setAddress(String address) {
        String oldAdress = this.address;
        this.address = address;
        this.prop.setProperty("address", address);
        try {
            prop.store(new FileOutputStream("config.properties"), null);
        } catch (IOException ex) {
            Logger.getLogger(MyCommunicationModel.class.getName()).log(Level.SEVERE, null, ex);
        }
        firePropertyChange(MyController.COMMUNICATION_ADRESS_PROPERTY, oldAdress, address);
    }

    public void setPort(Integer port) {
        if (DEBUG) {
            System.out.println("Màj port");
        }
        Integer oldPort = this.port;
        this.port = port;
        prop.setProperty("port", port.toString());
        try {
            prop.store(new FileOutputStream("config.properties"), null);
        } catch (IOException ex) {
            Logger.getLogger(MyCommunicationModel.class.getName()).log(Level.SEVERE, null, ex);
        }
        firePropertyChange(MyController.COMMUNICATION_PORT_PROPERTY,
                oldPort,
                port);
    }

    /*public void setVideoStatus(VideoStatus videoStatus) {
    this.videoStatus = videoStatus;
    }*/
    /**
     * Gestion du niveau de la batterie
     * @param level
     */
    public MyLevel getBatteryLevel() {
        return batteryLevel;
    }

    public void setBatteryLevel(MyLevel level) {
        if (DEBUG) {
            System.out.println("Màj batterie");
        }
        MyLevel old = batteryLevel;
        batteryLevel = level;
        firePropertyChange(MyController.MONITOR_BATTERY_PROPERTY, old, level);
    }

    public void setBatteryLevel(int level) {
        switch (level) {
            case 0:
                setBatteryLevel(MyLevel.LOW);
                break;
            case 1:
                setBatteryLevel(MyLevel.MEDIUM);
                break;
            case 2:
                setBatteryLevel(MyLevel.HIGH);
                break;
            default:
                setBatteryLevel(MyLevel.UNKNOWN);
                break;
        }
    }

    public void setRobotStatus(RobotStatus newRobotStatus) {
        if (DEBUG) {
            System.out.println("Màj robot status");
        }
        RobotStatus old = this.robStatus;
        this.robStatus = newRobotStatus;
        if ((newRobotStatus == RobotStatus.WAIT_CONNEXION)
                && (old != RobotStatus.CONNECTED)) {
            sendData(Order.ACTION_CONNECT_ROBOT.toByte());
            setMessage(new Message("demande de connexion", " ", Message.envoi, new Date()));
        }
        if (robStatus == RobotStatus.UNKNOWN) {
            setMissionStatus(RobotMissionStatus.NO_MISSION);
            setObjective(new Point(-1, -1));
            setDirection(MyDirection.STOP);
        }
        firePropertyChange(MyController.CHANGE_ROBOT_STATUS, old, robStatus);
    }

    public void setRobotStatus(int robotStatus) {
        switch (robotStatus) {
            case 0:
                setRobotStatus(RobotStatus.CONNECTED);
                break;
            default:
                setRobotStatus(RobotStatus.UNKNOWN);
                break;
        }
    }

    public void sendData(byte[] data) {
        if (DEBUG) {
            System.out.print((char) data[0]);
            for (int i = 1; i < data.length; i++) {
                System.out.print("|" + data[i]);
            }
            System.out.println("");
        }
        if (commStatus == CommunicationStatus.COMMUNICATION_CONNECTED) {
            try {
                os.write(data);
                os.flush();
            } catch (IOException ex) {
                closeConnexion();
            }
        } else {
            System.out.println("Socket fermé");
        }
    }

    public CommunicationStatus getCommStatus() {
        return commStatus;
    }

    public BufferedInputStream getIs() {
        return is;
    }

    public void setIs(BufferedInputStream is) {
        this.is = is;
    }

    public BufferedOutputStream getOs() {
        return os;
    }

    public void setOs(BufferedOutputStream os) {
        this.os = os;
    }

    /*public Arena getArena() {
    return arena;
    }
    
    public void setArena(Arena arena) {
    this.arena = arena;
    }
    
    public void clearArena() {
    arena = null;
    }*/
    public Movement getMove() {
        return move;
    }

    public void setMove(Movement move) {
        this.move = move;
    }

    public void setDirection(MyDirection newDirection) {
        if (DEBUG) {
            System.out.println("Màj direction");
        }
        MyDirection old = this.move.getDirection();
        this.move.setDirection(newDirection);
        /*if (old.value() != newDirection.value()) {
        sendData(move.toByte());
        }*/
        firePropertyChange(MyController.CHANGE_ROBOT_DIRECTION, old, move.getDirection());
    }

    public void clearMove() {
        move.setDirection(MyDirection.STOP);
    }

    public void setModelImg(MyImageModel modelImg) {
        this.modelImg = modelImg;
    }

    public void setSpeed(Integer newSpeed) {
        if (DEBUG) {
            System.out.println("Màj vitesse");
        }
        int old = move.getSpeed();
        move.setSpeed(newSpeed.intValue());
        firePropertyChange(MyController.CHANGE_ROBOT_SPEED, old, newSpeed);
    }

    public void setObjective(Point newPoint) {
        if (DEBUG) {
            System.out.println("Màj objectif");
        }
        Point old = objective;
        objective = newPoint;
        firePropertyChange(MyController.CHANGE_ROBOT_OBJECTIVE, old, objective);
    }

    public void setMissionStatus(RobotMissionStatus newMissionStatus) {
        if (DEBUG) {
            System.out.println("Màj mission status " + newMissionStatus);
        }
        RobotMissionStatus old = missionStatus;
        missionStatus = newMissionStatus;
        if (missionStatus == RobotMissionStatus.MISSION_RUNNING) {
            missionNumber++;
            MyMission mission = new MyMission(missionNumber, MissionType.REACH_COORDINATE);
            mission.setPosition((int) objective.x, (int) objective.y, 0);
            sendData(mission.toByte());
            setMessage(new Message("mission", " ", Message.envoi, new Date()));
        } else if (missionStatus == RobotMissionStatus.MISSION_STOP) {
            MyMission mission = new MyMission(missionNumber, MissionType.STOP);
            mission.setPosition(0, 0, 0);
            sendData(mission.toByte());
            setMessage(new Message("terminer la mission", " ", Message.envoi, new Date()));
        }
        firePropertyChange(MyController.CHANGE_ROBOT_MISSION_STATUS, old, missionStatus);
    }
    /*public Position getPosition() {
    return position;
    }
    
    public void setPosition(Position position) {
    this.position = position;
    }
    
    public RobotStatus getRobStatus() {
    return robStatus;
    }
    
    public void setRobStatus(RobotStatus robStatus) {
    this.robStatus = robStatus;
    }
    
    public ImageIcon getImage() {
    return image;
    }
    
    public void setImage(ImageIcon image) {
    this.image = image;
    }*/

    public MyMode getMode() {
        return mode;
    }

    public void setDemarrageRapide(Boolean newDemarrage) {
        if (DEBUG) {
            System.out.println("Changement de demarrage " + newDemarrage);
        }
        Boolean oldDemarrageRapide = demarrageRapide;
        this.demarrageRapide = newDemarrage;

        firePropertyChange(
                MyController.CHANGE_DEMARRAGE,
                oldDemarrageRapide, newDemarrage);

    }

    public void setMode(MyMode newMode) {
        if (DEBUG) {
            System.out.println("Changement de mode " + newMode.name());
        }
        MyMode oldMode = this.mode;
        this.mode = newMode;

        if (this.mode.name() != MyMode.MANUAL.name()) {
            ms.stopThread();
            clearMove();
        } else {
            ms = new MonitorSendPeriodic(this);
            ms.start();
        }

        firePropertyChange(
                MyController.CHANGE_MODE_ROBOT_CONTROL,
                oldMode, newMode);
    }

    public void setVersion(String newVersion) {
        if (DEBUG) {
            System.out.println("Version " + newVersion);
        }

        String oldVersion = this.version;
        this.version = newVersion;

        firePropertyChange(
                MyController.VERSION,
                oldVersion, newVersion);
    }
}
