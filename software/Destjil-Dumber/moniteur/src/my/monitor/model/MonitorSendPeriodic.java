package my.monitor.model;

import java.util.Date;


public final class MonitorSendPeriodic extends Thread {

    MyCommunicationModel model;
    boolean running;
    byte[] msg;

    public MonitorSendPeriodic(MyCommunicationModel m) {
        this.model = m;
    }

    public void setMonitor(MyCommunicationModel monitor) {
        this.model = monitor;
    }

    public void stopThread() {
        this.running = false;
    }

    public void run() {
        running = true;
        System.out.println("Thread send is runnig");
        while (running == true) {

            if (model.getMove() != null) {
                model.sendData(model.getMove().toByte());
                model.setMessage(new Message("mouvement", " ", Message.envoi, new Date()));
            }

            try {
                sleep(500);
            } catch (InterruptedException p) {
            }
        }
        System.out.println("Thread send is stopped");
    }
}
