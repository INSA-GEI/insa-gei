/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor.controller;

import java.awt.Point;
import my.monitor.model.enumeration.CommunicationStatus;
import my.monitor.model.enumeration.MyDirection;
import my.monitor.model.enumeration.MyMode;
import my.monitor.model.enumeration.RobotMissionStatus;
import my.monitor.model.enumeration.RobotStatus;
import my.monitor.model.enumeration.VideoStatus;

/**
 * This controller implements the required methods and provides the properties
 * necessary to work with the DisplayViewPanel and PropertyViewPanel views. Each of
 * methods in this class can be called upon by the views to update to state of the
 * registered models.
 *
 * @author Robert Eckstein
 */
public class MyController extends AbstractController {

    public static final String COMMUNICATION_STATUS_PROPERTY = "CommStatus";
    public static final String COMMUNICATION_ADRESS_PROPERTY = "Address";
    public static final String COMMUNICATION_PORT_PROPERTY = "Port";
    public static final String CHANGE_MODE_ROBOT_CONTROL = "Mode";
    public static final String MONITOR_BATTERY_PROPERTY = "BatteryLevel";
    public static final String CHANGE_ROBOT_STATUS = "RobotStatus";
    public static final String VIDEO_ARENA_PROPERTY = "Arena";
    public static final String VIDEO_POSITION_PROPERTY = "Position";
    public static final String VIDEO_IMAGE_PROPERTY = "Image";
    public static final String VIDEO_STATUS_PROPERTY = "VideoStatus";
    public static final String VIDEO_POSITION_STATUS_PROPERTY = "PositionComputed";
    public static final String CHANGE_ROBOT_DIRECTION = "Direction";
    public static final String CHANGE_ROBOT_SPEED = "Speed";
    public static final String CHANGE_ROBOT_OBJECTIVE = "Objective";
    public static final String CHANGE_ROBOT_MISSION_STATUS = "MissionStatus";
    public static final String CHANGE_DEMARRAGE = "DemarrageRapide";
    public static final String VERSION = "Version";
    public static final String MESSAGE = "Message";
    
    /**
     * Change Image
     */
    public void changePositionComputed(Boolean newPositionComputed) {
        setModelProperty(VIDEO_POSITION_STATUS_PROPERTY, newPositionComputed);
    }

    public void changeArena(VideoStatus newVideoStatus) {
        setModelProperty(VIDEO_STATUS_PROPERTY, newVideoStatus);
    }

    /**
     * Change Communication
     */
    public void changeCommStatus(CommunicationStatus newCommStatus) {
        setModelProperty(COMMUNICATION_STATUS_PROPERTY, newCommStatus);
    }

    public void changePort(int newPort) {
        setModelProperty(COMMUNICATION_PORT_PROPERTY, newPort);
    }

    public void changeAdress(String newAdress) {
        setModelProperty(COMMUNICATION_ADRESS_PROPERTY, newAdress);
    }

    public void changeMode(MyMode newMode) {
        setModelProperty(CHANGE_MODE_ROBOT_CONTROL, newMode);
    }

    public void changeRobotStatus(RobotStatus newRobotStatus) {
        setModelProperty(CHANGE_ROBOT_STATUS, newRobotStatus);
    }

    public void changeRobotDirection(MyDirection newDirection) {
        setModelProperty(CHANGE_ROBOT_DIRECTION, newDirection);
    }
    
    public void changeRobotSpeed(Integer newSpeed) {
        setModelProperty(CHANGE_ROBOT_SPEED, newSpeed);
    }
    
    public void changeRobotObjective(Point newPoint) {
        setModelProperty(CHANGE_ROBOT_OBJECTIVE, newPoint);
    }
    
    public void changeRobotMissionStatus(RobotMissionStatus newMissionStatus) {
        setModelProperty(CHANGE_ROBOT_MISSION_STATUS, newMissionStatus);
    }
    
    public void changeDemarrageRapide(Boolean newDemarrage) {
        setModelProperty(CHANGE_DEMARRAGE, newDemarrage);
    }
    
    public void changeVersion(String newVersion) {
        setModelProperty(VERSION, newVersion);
    }
    
    public void changeMessage(String newMessage) {
        setModelProperty(MESSAGE, newMessage);
    }
    
}
