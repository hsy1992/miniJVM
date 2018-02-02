package test;

import org.mini.gl.warp.GLFrameBuffer;
import org.mini.gl.warp.GLFrameBufferPainter;
import org.mini.glfw.Glfw;
import org.mini.glfw.GlfwCallbackAdapter;
import org.mini.glfw.utils.Gutil;
import org.mini.gui.GFrame;
import org.mini.gui.GForm;
import static org.mini.gui.GToolkit.getForm;
import static org.mini.nk.NK.nk_button_label;
import static org.mini.nk.NK.nk_false;
import static org.mini.nk.NK.nk_layout_row_dynamic;
import static org.mini.nk.NK.nk_layout_row_static;
import static org.mini.nk.NK.nk_option_label;
import static org.mini.nk.NK.nk_propertyi;
import static org.mini.nk.NK.nk_true;
import static org.mini.nk.NK.nk_window_show;
import org.mini.gui.GFrameContents;
import org.mini.gui.GGraphics;
import org.mini.gui.GImage;
import org.mini.nk.NK;

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

    void t1() {
        GForm win = new GForm("test"/*"test 窗口"*/, 800, 600);
        win.init();
        win.add(new GFrame(/*"demo测试"*/"demo", 30, 30, 500, 400, new NkFrameMain()));
        win.setCallBack(new MyCallBack());
        win.run();
    }

    class MyCallBack extends GlfwCallbackAdapter {

        @Override
        public void key(long window, int key, int scancode, int action, int mods) {
            System.out.println("key:" + key + " action:" + action);
            if (key == Glfw.GLFW_KEY_ESCAPE && action == Glfw.GLFW_PRESS) {
                Glfw.glfwSetWindowShouldClose(window, Glfw.GLFW_TRUE);
            }
            if (key == Glfw.GLFW_KEY_V) {
                if (mods == Glfw.GLFW_MOD_CONTROL) {

                    String string = Glfw.glfwGetClipboardString(window);
                    if (string != null) {
                        System.out.println("Clipboard contains " + string);
                    } else {
                        System.out.println("Clipboard does not contain a string\n");
                    }
                }
            }
        }

        @Override
        public void mouseButton(long window, int button, boolean pressed) {
            String bt = button == Glfw.GLFW_MOUSE_BUTTON_LEFT ? "LEFT" : button == Glfw.GLFW_MOUSE_BUTTON_2 ? "RIGHT" : "OTHER";
            String press = pressed ? "pressed" : "released";
//                System.out.println(bt + " " + mx + " " + my + "  " + press);
        }

        @Override
        public void drop(long window, int count, String[] paths) {
            for (int i = 0; i < count; i++) {
                System.out.println(i + " " + paths[i]);
            }
        }

        @Override
        public void cursorPos(long window, int x, int y) {
        }

        @Override
        public boolean windowClose(long window) {
            System.out.println("byebye");
            return true;
        }

        @Override
        public void windowSize(long window, int width, int height) {
            System.out.println("resize " + width + " " + height);
        }

        @Override
        public void framebufferSize(long window, int x, int y) {
        }
    }

    class NkFrameSub1 implements GFrameContents {

        GImage img;

        @Override
        public void init(GFrame parent) {
            img = new GImage("image4.png");
        }

        @Override
        public void updateContents(long ctx, GFrame parent) {
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_true == nk_button_label(ctx, Gutil.toUtf8("Close\000"))) {
//                System.out.println("close pressed\n");
//                System.out.println(" show sub1 " + Integer.toHexString(parent.getFrameMode()));
                nk_window_show(ctx, parent.getTitleBytes(), NK.NK_HIDDEN);
            }
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

        GFrame sub1;

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
            sub1 = new GFrame(/*"子窗口"*/"sub1", 100, 100, 300, 400, new NkFrameSub1());
            img1 = new GImage(glfb.getTexture(), glfb.getWidth(), glfb.getHeight());
        }

        @Override
        public void updateContents(long ctx, GFrame parent) {

            nk_layout_row_static(ctx, 30f, 80, 1);
            if (nk_true == nk_button_label(ctx, Gutil.toUtf8("Canvas here\000"))) {
                System.out.println("button pressed\n");
                long notice = NK.nk_window_find(ctx, sub1.getTitleBytes());
                if (notice == 0) {
                    GForm main = getForm(ctx);
                    main.add(sub1);
                    System.out.println("add sub1=======================");
                } else {
                    nk_window_show(ctx, sub1.getTitleBytes(), NK.NK_SHOWN);
                    System.out.println("show sub1");
                }
            }
//            nk_layout_row_dynamic(ctx, 30, 3);
//            if (nk_option_label(ctx, Gutil.toUtf8("easy\000"), op == EASY ? nk_true : nk_false) == nk_true) {
//                op = EASY;
//            }
//            if (nk_option_label(ctx, Gutil.toUtf8("mid\000"), op == MID ? nk_true : nk_false) == nk_true) {
//                op = MID;
//            }
//            if (nk_option_label(ctx, Gutil.toUtf8("hard\000"), op == HARD ? nk_true : nk_false) == nk_true) {
//                op = HARD;
//            }
//            nk_layout_row_dynamic(ctx, 22, 1);
//            property = nk_propertyi(ctx, Gutil.toUtf8("在哪里Compression:\000"), 0, property, 100, 10, 1);

            //draw 3d graphics
//            GGraphics g = parent.getGraphics();
//            glfb.render(glfbRender);
//            g.drawImage(img1, 0, 150, 200, 200, 0);
            try {
                Thread.sleep(50);
            } catch (InterruptedException ex) {
            }
        }
    }
}
