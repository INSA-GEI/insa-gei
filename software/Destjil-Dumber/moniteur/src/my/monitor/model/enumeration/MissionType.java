/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package my.monitor.model.enumeration;

/**
 *
 * @author pehladik
 */
public enum MissionType {
    REACH_COORDINATE    (0),
    REACH_OBJECTIVE     (1),
    STOP                (2);

    private final int id;

    MissionType(int id) {
        this.id  = id;
    }

    public int id () { return id;    }

}
