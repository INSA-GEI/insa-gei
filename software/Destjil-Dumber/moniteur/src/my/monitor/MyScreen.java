/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package my.monitor;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.geom.Ellipse2D;
import javax.swing.ImageIcon;
import javax.swing.JLabel;

/**
 *
 * @author Piro
 */
public class MyScreen extends JLabel {
    
    private ImageIcon img;
    Point objective = null;

    public MyScreen() {
        super();
    }
    
    @Override
    public void paint(Graphics g){
        Graphics2D g2 = (Graphics2D) g;
        if (img != null) g2.drawImage(img.getImage(), null, null);
        
        if (objective != null) {
            g2.setColor(Color.red);
            g2.draw(new Ellipse2D.Double(objective.x-5, objective.y-5, 10, 10));
        }
    }

    public void setImg(ImageIcon img) {
        this.img = img;
    }

    public void setObjective(Point objective) {
        this.objective = objective;
    }
    
    
}
