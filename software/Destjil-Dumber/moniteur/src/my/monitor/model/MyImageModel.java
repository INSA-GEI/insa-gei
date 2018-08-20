/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor.model;

import java.util.Date;
import javax.swing.ImageIcon;
import my.monitor.controller.MyController;
import my.monitor.model.enumeration.Order;
import my.monitor.model.enumeration.VideoStatus;

/**
 *
 * @author pehladik
 */
public class MyImageModel extends AbstractModel {

    // Varibale de débug
    boolean DEBUG = true;
    // Communication
    MyCommunicationModel myCommunicationModel;
    // Données du modèle
    Arena arena = null;
    MyPosition position = null;
    ImageIcon image = null;
    VideoStatus videoStatus;
    Boolean positionComputed;

    public MyImageModel(MyCommunicationModel myCommunicationModel) {
        this.myCommunicationModel = myCommunicationModel;
        this.myCommunicationModel.threadAperiodicReceiver.setModelImg(this);
    }

    public void initDefault() {
        if (DEBUG) {
            System.out.println("Init model");
        }
        setVideoStatus(VideoStatus.VIDEO_STOP);
        setPositionComputed(false);
        myCommunicationModel.setMessage(null);
    }

    public void setPositionComputed(Boolean positionComputed) {
        if (DEBUG) {
            System.out.println("Send compute position");
        }
        Boolean oldPosition = this.positionComputed;
        Order order;

        if (positionComputed.booleanValue()) {
            order = Order.ACTION_COMPUTE_POSITION;
        } else {
            order = Order.ACTION_ABORT_POSITION;
        }
        myCommunicationModel.sendData(order.toByte());
        myCommunicationModel.setMessage(new Message("demande de calcul de la position", " ", Message.envoi, new Date()));

        this.positionComputed = positionComputed;
        firePropertyChange(MyController.VIDEO_POSITION_STATUS_PROPERTY, oldPosition, positionComputed);
    }

    public void setArena(Arena arena) {
        if (DEBUG) {
            System.out.println("Set Arena");
        }
        Arena oldArena = this.arena;
        this.arena = arena;
        firePropertyChange(
                MyController.VIDEO_ARENA_PROPERTY,
                oldArena,
                arena);
    }

    public void setImage(ImageIcon image) {
        if (DEBUG) {
            System.out.println("Set Image");
        }
        ImageIcon oldImage = this.image;
        this.image = image;
        firePropertyChange(
                MyController.VIDEO_IMAGE_PROPERTY,
                oldImage,
                image);
    }

    public void setPosition(MyPosition position) {
        if (DEBUG) {
            System.out.println("Set Position");
        }
        MyPosition oldPosition = this.position;
        this.position = position;
        firePropertyChange(
                MyController.VIDEO_POSITION_PROPERTY,
                oldPosition,
                position);
    }

    public void setVideoStatus(VideoStatus videoStatus) {
        if (DEBUG) {
            System.out.println("Set video status " + videoStatus);
        }
        VideoStatus oldVideoStatus = this.videoStatus;

        Order order;
        switch (videoStatus) {
            case VIDEO_ARENA_CALIBRATION:
                order = Order.ACTION_FIND_ARENA;
                myCommunicationModel.setMessage(new Message("trouver l'arène", " ", Message.envoi, new Date()));
                this.videoStatus = videoStatus;
                break;
            case VIDEO_ARENA_OK:
                order = Order.ACTION_ARENA_OK;
                myCommunicationModel.setMessage(new Message("arène trouvée", " ", Message.envoi, new Date()));
                this.videoStatus = videoStatus;
                break;
            default:
                order = Order.ACTION_CANCEL_ARENA;
                myCommunicationModel.setMessage(new Message("annulation de la recherche de l'arène", " ", Message.envoi, new Date()));
                this.videoStatus = VideoStatus.VIDEO_RUNNING;
                break;
        }
        if (DEBUG) {
            System.out.println("Send order " + order);
        }
        myCommunicationModel.sendData(order.toByte());

        firePropertyChange(
                MyController.VIDEO_STATUS_PROPERTY,
                oldVideoStatus,
                videoStatus);
    }
}
