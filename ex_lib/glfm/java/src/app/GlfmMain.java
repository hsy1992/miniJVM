/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package app;

import java.util.Random;
import javax.cldc.io.Connector;
import javax.mini.net.Socket;
import org.mini.gl.warp.GLFrameBuffer;
import org.mini.gl.warp.GLFrameBufferPainter;
import org.mini.glfm.Glfm;
import static org.mini.glfm.Glfm.GLFMColorFormatRGBA8888;
import static org.mini.glfm.Glfm.GLFMDepthFormatNone;
import static org.mini.glfm.Glfm.GLFMMultisampleNone;
import static org.mini.glfm.Glfm.GLFMRenderingAPIOpenGLES2;
import static org.mini.glfm.Glfm.GLFMStencilFormatNone;
import org.mini.gui.GButton;
import org.mini.gui.GCanvas;
import org.mini.gui.GCheckBox;
import org.mini.gui.GColorSelector;
import org.mini.gui.GForm;
import org.mini.gui.GFrame;
import org.mini.gui.GGraphics;
import org.mini.gui.GImage;
import org.mini.gui.GInitExtension;
import org.mini.gui.GInputField;
import org.mini.gui.GLabel;
import org.mini.gui.GList;
import org.mini.gui.GObject;
import org.mini.gui.GPanel;
import org.mini.gui.GScrollBar;
import org.mini.gui.GTextBox;
import org.mini.gui.event.GActionListener;
import static org.mini.nanovg.Gutil.toUtf8;
import org.mini.nanovg.Nanovg;

/**
 *
 * @author gust
 */
public class GlfmMain {

    public static void main(String[] args) {
    }

    static public void glinit(long display) {

        Glfm.glfmSetDisplayConfig(display,
                GLFMRenderingAPIOpenGLES2,
                GLFMColorFormatRGBA8888,
                GLFMDepthFormatNone,
                GLFMStencilFormatNone,
                GLFMMultisampleNone);
        GForm form;
        form = new GForm(/*"GuiTest"*/"登录 窗口", 800, 600, display);
        form.setExtinit(new MyInit(form));
        form.setFps(30f);
        //t13();
    }

    static void t13() {
        try {
            Socket conn = (Socket) Connector.open("socket://baidu.com:80");
            conn.setOption(Socket.OP_TYPE_NON_BLOCK, Socket.OP_VAL_NON_BLOCK);
            String request = "GET / HTTP/1.1\r\n\r\n";
            conn.write(request.getBytes(), 0, request.length());
            byte[] rcvbuf = new byte[256];
            int len = 0;
            while (len != -1) {
                len = conn.read(rcvbuf, 0, 256);
                for (int i = 0; i < len; i++) {
                    System.out.print((char) rcvbuf[i]);
                }
                System.out.print("\n");
            };
        } catch (Exception e) {

        }
    }
}

class MyInit implements GInitExtension {

    GForm form;

    public MyInit(GForm gf) {
        form = gf;
    }

    @Override
    public void onInit(GObject owner) {
        long vg = form.getNvContext();
        GFrame gframe = new GFrame("Github"/*"demo"*/, 50, 50, 300, 500);
        init(gframe.getPanel(), vg);
        form.add(gframe);
    }

    public void init(GPanel parent, final long vg) {
//        light = new Light();

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
        String conttxt = "子窗口This is longer chunk of text.\n  \n  Would have used lorem ipsum but she    was busy jumping over the lazy dog with the fox and all the men who came to the aid of the party.";
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
                GFrame sub1 = new GFrame(/*"子窗口"*/"颜色选择", 40 + ran.nextInt(100), 50 + ran.nextInt(100), 300, 400);
                GPanel panel = sub1.getPanel();
                init1(panel, vg);
                sub1.setClosable(true);
                form.add(sub1);
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

        bt1.setActionListener(new GActionListener() {
            @Override
            public void action() {
                System.out.println("delete something");
            }
        });
    }

    GImage img;
    GList list;

    public void init1(GPanel parent, long vg) {
        img = new GImage("./image4.png");

        int x = 10, y = 10;
        list = new GList(x, y, 280, 30);
        parent.add(list);
        if (list.getImages() == null) {
            int i = Nanovg.nvgCreateImage(vg, toUtf8("./image4.png"), 0);
            list.setItems(new int[]{i, i, i, i, i, i, i, i, i, i},
                    new String[]{"A", "B", "C", "D", "E", "F", "G", "H", "I", "J",});
        }
        y += 40;
        parent.add(new TestCanvas(x, y, 280, 150));
        y += 160;
        GColorSelector cs = new GColorSelector(0, x, y, 130, 130);
        parent.add(cs);

    }

    class TestCanvas extends GCanvas {

        GLFrameBuffer glfb;
        GLFrameBufferPainter glfbRender;
        GImage img3D;

        public TestCanvas(int x, int y, int w, int h) {
            super(x, y, w, h);
//            glfb = new GLFrameBuffer(300, 300);
//            glfbRender = new GLFrameBufferPainter() {
//                @Override
//                public void paint() {
//                    light.setCamera();
//                    light.draw();
//                }
//            };
//            img3D = new GImage(glfb.getTexture(), glfb.getWidth(), glfb.getHeight());
        }

        int pos = 0, delta = 1;

        public void paint(GGraphics g) {
            g.setColor(0xff000000);
            g.fillRect(0, 0, (int) getW(), (int) getH());
            g.setColor(0xff0000ff);
            g.drawLine(0, 100, 100, 100);
            pos += delta;
            if (pos > 50) {
                delta = -1;
            }
            if (pos < 0) {
                delta = 1;
            }
            g.drawString("this is a canvas", pos, 50, GGraphics.TOP | GGraphics.LEFT);
//            glfb.render(glfbRender);
//            g.drawImage(img3D, 0, 0, 100, 100, GGraphics.TOP | GGraphics.LEFT);
        }
    }
}
