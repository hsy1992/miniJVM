package test;

import org.mini.gl.warp.GLFrameBuffer;
import org.mini.gl.warp.GLFrameBufferPainter;
import org.mini.gui.GButton;
import org.mini.gui.GCheckBox;
import org.mini.gui.GColorSelector;
import org.mini.gui.GEditBox;
import org.mini.gui.GFrame;
import org.mini.gui.GForm;
import org.mini.gui.GFrameContents;
import org.mini.gui.GImage;
import org.mini.gui.GInputField;
import org.mini.gui.GLabel;
import org.mini.gui.GObject;
import org.mini.gui.GSlider;
import org.mini.gui.event.GActionListener;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author gust
 */
public class GuiTest {

    public static void main(String[] args) {
        GuiTest gt = new GuiTest();
        gt.t1();

    }
    GForm win;

    void t1() {
        win = new GForm(/*"GuiTest"*/"test 窗口", 800, 600);
        win.init();
        win.add(new GFrame("demo测试"/*"demo"*/, 50, 50, 300, 400, new NkFrameMain()));
        GFrame sub1 = new GFrame(/*"子窗口"*/"sub1", 100, 100, 300, 400, new NkFrameSub1());
        win.add(sub1);
        win.run();
    }

    class NkFrameSub1 implements GFrameContents {

        GImage img;

        @Override
        public void init(GFrame parent) {
            img = new GImage("image4.png");
            GColorSelector cs = new GColorSelector(0, 10, 30, 200, 200);
            parent.add(cs);
        }

        @Override
        public void updateContents(long vg, GFrame parent) {
//            //canvas
//            GGraphics g = parent.getGraphics();
//
//            g.setColor(0x80ff0000);
//            g.fillRect(100, 200, 200, 100);
//            g.setColor(0xff00ff00);
//            g.fillArc(150, 100, 50, 40, 0, 90);
//            g.drawLine(200, 200, 300, 400);
//            g.drawArc(100, 100, 30, 50, 0, 270);
//
//            g.drawImage(img, 0, 150, 100, 100, 0);
//            g.drawString("测试一下看看here is a text drawing test.", 100, 300, 0);

//            GImage img2 = GForm.getGFont().renderToTexture("张鹏gust zhang", 10);
//            g.drawImage(img2, 120, 150, 0);
        }

    }

    class NkFrameMain implements GFrameContents {

        final int EASY = 0, MID = 1, HARD = 2;
        int op = EASY;
        int property = 20;
        Light light;
        GLFrameBuffer glfb;
        GLFrameBufferPainter glfbRender;
        GImage img1;

        @Override
        public void init(GFrame parent) {
            light = new Light();
            glfb = new GLFrameBuffer(300, 300);
            glfbRender = new GLFrameBufferPainter() {
                @Override
                public void paint() {
//                    light.setCamera();
//                    light.draw();
                }
            };
            int x = 10, y = 40;
            GInputField gif = new GInputField("search", x, y, 280, 25);
            parent.add(gif);
            y += 30;
            GLabel lb1 = new GLabel("Login", x, y, 280, 20);
            parent.add(lb1);
            y += 25;
            GEditBox mail = new GEditBox("Email", x, y, 280, 28);
            parent.add(mail);
            y += 35;
            GEditBox pwd = new GEditBox("Password", x, y, 280, 28);
            parent.add(pwd);
            y += 35;

            GCheckBox cbox = new GCheckBox("Remember me", true, x, y, 140, 28);
            parent.add(cbox);
            GButton sig = new GButton("Sign in", x + 138, y, 140, 28);
            sig.setBgColor(0, 96, 128, 255);
            sig.setIcon(GObject.ICON_LOGIN);
            parent.add(sig);
            y += 35;
            GLabel lb2 = new GLabel("Diameter", x, y, 280, 20);
            parent.add(lb2);
            y += 25;
            //drawEditBoxNum(vg, "123.00", "px", x + 180, y, 100, 28);
            GSlider sli = new GSlider(0.4f, x, y, 170, 28);
            parent.add(sli);
            y += 35;
            GButton bt1 = new GButton("Delete删除", x, y, 160, 28);
            bt1.setBgColor(128, 16, 8, 255);
            bt1.setIcon(GObject.ICON_TRASH);
            parent.add(bt1);
            GButton bt2 = new GButton("Cancel", x + 170, y, 110, 28);
            bt2.setBgColor(0, 0, 0, 0);
            parent.add(bt2);
            img1 = new GImage(glfb.getTexture(), glfb.getWidth(), glfb.getHeight());

            bt1.setActionListener(new GActionListener() {
                @Override
                public void action() {
                    System.out.println("delete something");
                }
            });
        }

        @Override
        public void updateContents(long vg, GFrame parent) {
            try {
                Thread.sleep(30);
            } catch (InterruptedException ex) {
            }
        }
    }
}
