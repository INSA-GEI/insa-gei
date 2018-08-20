/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package my.monitor.model.enumeration;

/**
 *
 * @author pehladik
 */
public enum MyDirection {
    FORWARD (1),
    BACKWARD (4),
    LEFT (3),
    RIGHT (2),
    STOP (0);

    private final int value;
    MyDirection(int value){
        this.value = value;
    }

    public int value() { return value ; }
}
