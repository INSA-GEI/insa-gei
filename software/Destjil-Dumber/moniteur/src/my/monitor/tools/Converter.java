/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor.tools;

/**
 *
 * @author pehladik
 */
public class Converter {

    public static byte[] intToByte(int value) {
        byte[] b = new byte[4];
        b[0] = (byte) ((value >> 24) & 0xFF);
        b[1] = (byte) ((value >> 16) &0xFF);
        b[2] = (byte) ((value >> 8) & 0xFF);
        b[3] = (byte) ((value) & 0xFF);
        return b;
    }

    public static void printByteArray(byte[] b, int nbData) {
        if (nbData > b.length) {
            nbData = b.length;
        }
        for (int i = 0; i < nbData; i++) {
            System.out.print(Integer.toHexString(0xFF & b[i]) + " ");
            if (((i + 1) % 25) == 0) {
                System.out.println();
            }
        }
        System.out.println();
    }
}
