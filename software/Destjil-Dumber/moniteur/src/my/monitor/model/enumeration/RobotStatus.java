/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package my.monitor.model.enumeration;

/**
 *
 * @author pehladik
 */
public enum RobotStatus {
    UNKNOWN,
    WAIT_CONNEXION,
    CONNECTED,
    ROBOT_STATUS_ERR_NO_FILE,
    ROBOT_STATUS_ERR_TIMEOUT,
    ROBOT_STATUS_ERR_UNKNOWN_CMD,
    ROBOT_STATUS_ERR_INVALID_PARAMS,
    ROBOT_STATUS_ERR_WDT_EXPIRED,
    DISCONNECTED,
    ROBOT_STATUS_ERR_SELECT;

}
