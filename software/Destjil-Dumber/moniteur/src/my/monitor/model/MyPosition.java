/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package my.monitor.model;

import java.nio.ByteBuffer;

/**
 *
 * @author pehladik
 */
public class MyPosition {
    private int x;
    private int y;
    private int angle;

    public MyPosition(int x, int y, int angle) {
        this.x = x;
        this.y = y;
        this.angle = angle;
    }

    public MyPosition(byte[] b){
        ByteBuffer buf = ByteBuffer.wrap(b);
        x = buf.getInt();
        y = buf.getInt();
        angle = buf.getInt();
    }

    public int getAngle() {
        return angle;
    }

    public void setAngle(int angle) {
        this.angle = angle;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

}
