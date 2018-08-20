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
public class Arena {

    public int x;
    public int y;
    public int angle;
    public int width;
    public int height;

    public Arena(int x, int y, int angle, int width, int height) {
        this.x = x;
        this.y = y;
        this.angle = angle;
        this.width = width;
        this.height = height;
    }

    public Arena(byte[] b) {
        ByteBuffer buf = ByteBuffer.wrap(b);
        this.x = buf.getInt();
        this.y = buf.getInt();
        this.angle = buf.getInt();
        this.width = buf.getInt();
        this.height = buf.getInt();
    }

    public int getAngle() {
        return angle;
    }

    public void setAngle(int angle) {
        this.angle = angle;
    }

    public int getHeight() {
        return height;
    }

    public void setHeight(int height) {
        this.height = height;
    }

    public int getWidth() {
        return width;
    }

    public void setWidth(int width) {
        this.width = width;
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
