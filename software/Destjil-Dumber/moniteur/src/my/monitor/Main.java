/**
 * 26/03/2012 : remise à jour du panel Image lors de la perte de connexion
 * 28/03/2012 : ajout du démarrage rapide et du TextBox Version dans le panel
 * de connexion du robot
 * 17/04/2012 : correction connection distante
 * TODO : paneau de log avec l'option debug
 */

package my.monitor;

import java.awt.Dimension;
import java.awt.Insets;
import javax.swing.JFrame;
import javax.swing.WindowConstants;
import my.monitor.controller.MyController;
import my.monitor.model.MyCommunicationModel;
import my.monitor.model.MyImageModel;

public class Main {

    public static void main(String[] args) {
        MyCommunicationModel model = new MyCommunicationModel();
        MyImageModel modelImg = new MyImageModel(model);
        model.setModelImg(modelImg);

        MyController controller = new MyController();

        MyCommunicationPanel view = new MyCommunicationPanel(controller);
        MyImagePanel viewImage = new MyImagePanel(controller);
        MyComRobotPanel viewComRobot = new MyComRobotPanel(controller);
        MyControlRobotPanel viewControl = new MyControlRobotPanel(controller);
        MyLoggerPanel viewLogger = new MyLoggerPanel(controller);

        controller.addView(view);
        controller.addView(viewImage);
        controller.addView(viewComRobot);
        controller.addView(viewControl);
        controller.addView(viewLogger);

        controller.addModel(model);
        controller.addModel(modelImg);

        modelImg.initDefault();
        model.initDefault(8000, "localhost");

        JFrame displayFrame = new JFrame("Moniteur DeStijl (v"+ model.getVersion() + ")");
        displayFrame.getContentPane().setPreferredSize(new Dimension(1110, 710));
        displayFrame.getContentPane().setLayout(null);
        displayFrame.setResizable(false);
        int padding = 10;

        displayFrame.getContentPane().add(view);
        Insets insets = displayFrame.getContentPane().getInsets();
        Dimension size = view.getPreferredSize();
        view.setBounds(padding+insets.left, padding+insets.top, size.width, size.height);

        displayFrame.getContentPane().add(viewComRobot);
        size = viewComRobot.getPreferredSize();
        viewComRobot.setBounds(padding+ 680 + insets.left, padding+insets.top, size.width, size.height);

        displayFrame.getContentPane().add(viewImage);
        size = viewImage.getPreferredSize();
        viewImage.setBounds(padding+insets.left, padding+100 + insets.top, size.width, size.height);

        displayFrame.getContentPane().add(viewControl);
        size = viewControl.getPreferredSize();
        viewControl.setBounds(padding+680 + insets.left, padding+100+ insets.top, size.width, size.height);
        
        displayFrame.getContentPane().add(viewLogger);
        size = viewControl.getPreferredSize();
        viewLogger.setBounds(padding+680 + insets.left, padding+440+ insets.top, size.width, size.height);
        
        displayFrame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        displayFrame.pack();
        viewComRobot.setVisible(false);
        viewImage.setVisible(false);
        viewControl.setVisible(false);
        viewLogger.setVisible(true);

        displayFrame.setVisible(true);

    }
}
