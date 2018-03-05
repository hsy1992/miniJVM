package test;

import java.util.Random;
import org.mini.gl.warp.GLFrameBuffer;
import org.mini.gl.warp.GLFrameBufferPainter;
import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
import org.mini.gui.GButton;
import org.mini.gui.GCheckBox;
import org.mini.gui.GColorSelector;
import org.mini.gui.GTextBox;
import org.mini.gui.GFrame;
import org.mini.gui.GForm;
import org.mini.gui.GImage;
import org.mini.gui.GInputField;
import org.mini.gui.GLabel;
import org.mini.gui.GList;
import org.mini.gui.GObject;
import org.mini.gui.GPanel;
import org.mini.gui.GScrollBar;
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
        win = new GForm(/*"GuiTest"*/"登录 窗口", 800, 600);
        win.init();
        long vg = win.getGLContext();
        GFrame gframe = new GFrame("Github"/*"demo"*/, 50, 50, 300, 500);
        init(gframe.getPanel(), vg);
        win.add(gframe);
        win.run();
    }

    final int EASY = 0, MID = 1, HARD = 2;
    int op = EASY;
    int property = 20;
    Light light;
    GLFrameBuffer glfb;
    GLFrameBufferPainter glfbRender;
    GImage img1;

    public void init(GPanel parent, long vg) {
        light = new Light();
        glfb = new GLFrameBuffer(300, 300);
        glfbRender = new GLFrameBufferPainter() {
            @Override
            public void paint() {
//                    light.setCamera();
//                    light.draw();
            }
        };
        int x = 8, y = 10;
        GInputField gif = new GInputField("", "search", x, y, 280, 25);
        parent.add(gif);
        y += 30;
        GLabel lb1 = new GLabel("Login", x, y, 280, 20);
        parent.add(lb1);
        y += 25;
        GTextBox mail = new GTextBox("", "Email", x, y, 280, 28);
        mail.setSingleMode(true);
        parent.add(mail);
        y += 35;
        GTextBox pwd = new GTextBox("", "Password", x, y, 280, 28);
        pwd.setSingleMode(true);
        parent.add(pwd);
        y += 35;
        String conttxt = "This is longer chunk of text.\n  \n  Would have used lorem ipsum but she    was busy jumping over the lazy dog with the fox and all the men who came to the aid of the party.";
        conttxt += "I test the program ,there are two window , one window left a button that open the other window, the other left a button for close self.\n"
                + "\n"
                + "the issue maybe related with font , if i use nuklear defult font , the bug nerver show , but i am using chinese font (google android system default font), the bug frequently occure. the app memory using about 180M with default font in macos, use chinese font it would be 460M, is that nuklear load all glyph? but it's not the cause of bug .\n"
                + "\n"
                + "i have a reference that using stb_truetype, follow code is a stbtt test case , the code using chinese font ,that var byteOffset is -64 , out of the allocated bitmap memory . but i 'm not sure there is a same issue, only a note.";
        GTextBox cont = new GTextBox(conttxt, "Contents", x, y, 280, 188);
        parent.add(cont);
        y += 195;

        GCheckBox cbox = new GCheckBox("Remember me", true, x, y, 140, 28);
        parent.add(cbox);
        GButton sig = new GButton("Sign in", x + 138, y, 140, 28);
        sig.setBgColor(0, 96, 128, 255);
        sig.setIcon(GObject.ICON_LOGIN);
        parent.add(sig);
        sig.setActionListener(new GActionListener() {
            @Override
            public void action() {
                Random ran = new Random();
                GFrame sub1 = new GFrame(/*"子窗口"*/"颜色选择", 400 + ran.nextInt(100), 50 + ran.nextInt(100), 300, 400);
                GPanel panel = sub1.getPanel();
                init1(panel, vg);
                sub1.setClosable(true);
                win.add(sub1);
            }
        });
        y += 35;
        GLabel lb2 = new GLabel("Diameter", x, y, 280, 20);
        parent.add(lb2);
        y += 25;
        //drawEditBoxNum(vg, "123.00", "px", x + 180, y, 100, 28);
        GScrollBar sli = new GScrollBar(0.4f, GScrollBar.HORIZONTAL, x, y, 170, 28);
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

    public void updateContents(long vg, GFrame parent) {
        try {
            Thread.sleep(30);
        } catch (InterruptedException ex) {
        }
    }

    GImage img;
    GList list;

    public void init1(GPanel parent, long vg) {
        img = new GImage("./image4.png");
//            GColorSelector cs = new GColorSelector(0, 50, 30, 200, 200);
//            parent.add(cs);

        list = new GList(10, 10, 280, 30);
        parent.add(list);
        if (list.getImages() == null) {
            int i = Nutil.nvgCreateImage(vg, toUtf8("./image4.png"), 0);
            list.setItems(new int[]{i, i, i, i, i, i, i, i, i, i},
                    new String[]{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",});

        }
    }

    public void updateContents1(long vg, GFrame parent) {

    }

}
