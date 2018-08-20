/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor.model;

import my.monitor.model.enumeration.MyDirection;
import my.monitor.tools.Converter;

/**
 *
 * @author pehladik
 */
public class Movement {

    private MyDirection direction;
    private int speed;

    public Movement(MyDirection direction, int speed) {
        this.direction = direction;
        this.speed = speed;
    }

    public byte[] toByte() {
        int length = 8;
        byte[] data = new byte[5 + length];
        data[0] = 'M';
        System.arraycopy(Converter.intToByte(length), 0, data, 1, 4);
        System.arraycopy(Converter.intToByte(this.direction.value()), 0, data, 5, 4);
        System.arraycopy(Converter.intToByte(this.speed), 0, data, 9, 4);
        return data;
    }

    public MyDirection getDirection() {
        return direction;
    }

    public void setDirection(MyDirection direction) {
        this.direction = direction;
    }

    public int getSpeed() {
        return speed;
    }

    public void setSpeed(int speed) {
        this.speed = speed;
    }
}
