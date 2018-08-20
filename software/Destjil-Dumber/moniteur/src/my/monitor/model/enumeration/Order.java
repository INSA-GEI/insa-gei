/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package my.monitor.model.enumeration;

import my.monitor.tools.Converter;
/**
 *
 * @author pehladik
 */
public enum Order {
    ACTION_FIND_ARENA (1),
    ACTION_CANCEL_ARENA (2),
    ACTION_ARENA_OK (3),
    ACTION_CONNECT_ROBOT (10),
    ACTION_DISCONNECT_ROBOT (11),
    ACTION_COMPUTE_POSITION (4),
    ACTION_ABORT_POSITION (5),
    ACTION_ABORT_MISSION (6);

    private final int type;
    Order(int type) {
        this.type = type;
    }

    public byte[] toByte(){
        int length = 4;
        byte[] data = new byte[5 + length];
        data[0] = 'A';
        System.arraycopy(Converter.intToByte(length), 0, data, 1, 4);
        System.arraycopy(Converter.intToByte(this.type), 0, data, 5, 4);

        return data;
    }

    public static void main(String[] args)
    {
        Converter.printByteArray(Order.ACTION_ABORT_MISSION.toByte(), 9);
    }

}
