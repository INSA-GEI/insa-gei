package my.monitor.model;

import java.awt.Image;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Date;
import javax.swing.ImageIcon;
import my.monitor.model.enumeration.RobotMissionStatus;

public final class MonitorReceive extends Thread {

    MyCommunicationModel model;
    MyImageModel modelImg;
    Image img;
    boolean running;
    char type;
    int size;
    byte[] payload;
    byte[] head;
    boolean DEBUG = true;

    @SuppressWarnings("LeakingThisInConstructor")
    public MonitorReceive(MyCommunicationModel m) {
        this.model = m;
    }

    public void setMonitor(MyCommunicationModel m) {
        this.model = m;
    }

    public void setModelImg(MyImageModel modelImg) {
        this.modelImg = modelImg;
    }

    /**
     * Convert byte array into int
     * @param b the byte array to convert
     * @return the int value
     */
    public int getInt(byte[] b) {
        int i = ((b[1] & 0xFF) << 24);
        i += ((b[2] & 0xFF) << 16);
        i += ((b[3] & 0xFF) << 8);
        i += (b[4] & 0xFF);
        return i;
    }

    public void stopThread() {
        running = false;
    }

    public void run() {
        running = true;

        try {
            System.out.println("Thread receive is running");
            while (running == true) {

                head = new byte[5];
                model.getIs().read(head, 0, 5);

                type = (char) head[0];                                      /* Lecture du type */
                size = getInt(head);                                        /* Lecture de la taille des données */

                if (size == 0) {
                    running = false;                                        /* Si la taille est 0 perte de la comm */
                    break;
                }
                payload = new byte[size];

                int size_read = 0;
                while (size_read < size) {
                    size_read += model.getIs().read(payload, size_read, size - size_read);                                   /* Lecture des données */

                }
                Message message = new Message(" ", " ", Message.reception, new Date());
                if (DEBUG) {
                    System.out.println("Message recu de type " + type);
                }
                if (type == 'V') {                                          /* Données reçues de type video */
                    img = new ImageIcon(payload).getImage();
                    System.out.println(modelImg);
                    modelImg.setImage(new ImageIcon(img));
                    message.setType("image");
                } else if (type == 'P') {                                          /* Données reçues de type position */
                    modelImg.setPosition(new MyPosition(payload));
                    message.setType("position");
                } else if (type == 'R') {
                    int robot_status = ByteBuffer.wrap(payload).getInt();
                    model.setRobotStatus(robot_status);
                    message.setType("état de la communication avec le robot");
                } else if (type == 'B') {
                    int vbat = ByteBuffer.wrap(payload).getInt();
                    model.setBatteryLevel(vbat);
                    message.setType("état de la batterie");
                } else if (type == 'T') { /*Mission accomplie*/
                    int number = ByteBuffer.wrap(payload).getInt();
                    model.setMissionStatus(RobotMissionStatus.MISSION_FINISHED);
                    message.setType("mission terminée");
                } else if (type == 'F') { /* Version */
                    String st = new String(payload);
                    model.setVersion(st);
                    message.setType("version");
                }
                model.setMessage(message);
            }

        } catch (IOException e) {
        }
        model.closeConnexion();
        //modelImg.initDefault();
        System.out.println("Trhead receive is stopped");

    }
}
