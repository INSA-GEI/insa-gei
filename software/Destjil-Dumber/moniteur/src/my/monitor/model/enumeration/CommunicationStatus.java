/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor.model.enumeration;

/**
 *
 * @author pehladik
 */
public enum CommunicationStatus {

    COMMUNICATION_DISCONNECTED ("disconnected"),
    COMMUNICATION_CONNECTED ("connected"),
    COMMUNICATION_FAILURE ("échec"),
    COMMUNICATION_TRYING ("en cours") ;

    private final String description;
    CommunicationStatus (String description){
        this.description = description;
    }

    public String description()   { return description; }
}
