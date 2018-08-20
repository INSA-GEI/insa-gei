/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor.model;

import java.util.Date;

/**
 *
 * @author Piro
 */
public class Message {
    
    String type;
    String name;
    int sens;
    Date date;
    
    static public int reception = 0;
    static public int envoi = 1;

    public Message(String type, String name, int sens, Date date) {
        this.type = type;
        this.name = name;
        this.sens = sens;
        this.date = date;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getSens() {
        return sens;
    }

    public void setSens(int sens) {
        this.sens = sens;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }
    
    
    
}
