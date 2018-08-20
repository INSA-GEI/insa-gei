/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor.model;

import my.monitor.model.enumeration.MyObjective;
import my.monitor.model.enumeration.MissionType;
import my.monitor.tools.Converter;

/**
 *
 * @author pehladik
 */
public class MyMission {

    public int number;
    public MissionType type;
    public int X = -1;
    public int Y = -1;
    public int orientation = -1;
    public MyObjective objective = MyObjective.SIMPLE_MOVEMENT;

    public MyMission(int number, MissionType type) {
        this.number = number;
        this.type = type;
    }

    public void setPosition(int X, int Y, int orientation) {
        this.X = X;
        this.Y = Y;
        this.orientation = orientation;
    }

    public void setObjective(MyObjective objective) {
        this.objective = objective;
    }

    public byte[] toByte() {
        int length = 24;
        byte[] data = new byte[5 + length];
        data[0] = 'T';
        System.arraycopy(Converter.intToByte(length), 0, data, 1, 4);
        System.arraycopy(Converter.intToByte(this.number), 0, data, 5, 4);
        System.arraycopy(Converter.intToByte(this.type.id()), 0, data, 9, 4);
        System.arraycopy(Converter.intToByte(this.X), 0, data, 13, 4);
        System.arraycopy(Converter.intToByte(this.Y), 0, data, 17, 4);
        System.arraycopy(Converter.intToByte(this.orientation), 0, data, 21, 4);
        System.arraycopy(Converter.intToByte(this.objective.id()), 0, data, 25, 4);

        return data;
    }
}
