/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package my.monitor.model.enumeration;

/**
 *
 * @author pehladik
 */
public enum MyObjective {
    GREEN_AREA ("Green area", 0),
    RED_AREA ("Red area", 1),
    GET_THE_BALL ("Get the ball", 2),
    SIMPLE_MOVEMENT ("Move", 3);

    private final String description;
    private final int id;
    MyObjective (String description, int id){
        this.description = description;
        this.id = id;
    }

    public int id()   { return id; }
}
