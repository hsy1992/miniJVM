package test;

import org.mini.glfw.Glfw;
import org.mini.glfw.GlfwCallbackAdapter;
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
import static org.mini.nk.NK.nk_window_find;
import static org.mini.nk.NK.nk_window_show;
import org.mini.gui.GFrameContents;
import org.mini.gui.GGraphics;
import org.mini.gui.GImage;

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

    class NkFormContents1 implements GFrameContents {

        final int EASY = 0, MID = 1, HARD = 2;
        int op = EASY;
        int property = 20;
        String sub_form_name = "Notice";
        GImage img;

        @Override
        public void updateContents(long ctx, GFrame parent) {

            nk_layout_row_static(ctx, 30f, 80, 1);
            if (nk_true == nk_button_label(ctx, "button")) {
                System.out.println("button pressed\n");
                long notice = nk_window_find(ctx, sub_form_name);
                if (notice == 0) {
                    GForm main = getForm(ctx);
                    System.out.println("main window:" + main);
//                    main.add(new GFrame(sub_form_name, 100, 100, 300, 400, new GFrameContents() {
//                        @Override
//                        public void updateContents(long ctx, GFrame parent) {
//                            nk_layout_row_static(ctx, 30, 80, 1);
//                            if (nk_true == nk_button_label(ctx, "Ok")) {
//                                System.out.println("Ok pressed\n");
//                            }
//                        }
//                    }));
                } else {
                    nk_window_show(ctx, sub_form_name, 1);
                }
            }
            nk_layout_row_dynamic(ctx, 30, 3);
            if (nk_option_label(ctx, "easy", op == EASY ? nk_true : nk_false) == nk_true) {
                op = EASY;
            }
            if (nk_option_label(ctx, "mid", op == MID ? nk_true : nk_false) == nk_true) {
                op = MID;
            }
            if (nk_option_label(ctx, "hard", op == HARD ? nk_true : nk_false) == nk_true) {
                op = HARD;
            }
            nk_layout_row_dynamic(ctx, 22, 1);
            property = nk_propertyi(ctx, "Compression:", 0, property, 100, 10, 1);

            GGraphics g = parent.getGraphics();

            g.setColor(0x80ff0000);
            g.fillRect(100, 100, 100, 200);
            g.setColor(0xff00ff00);
            g.fillArc(150, 150, 50, 40, 0, 360);
            g.drawLine(0, 0, 300, 400);
            g.drawArc(100, 100, 100, 200, 0, 360);
            if(img==null){
                img=new GImage();
                img.loadImage("image4.png");
            }
            g.drawImage(img, 0, 0, 0);
        }

    }

    void t1() {
        GForm win = new GForm("test", 800, 600);
        win.add(new GFrame("demo", 30, 30, 300, 200, new NkFormContents1()));
        win.setCallBack(new MyCallBack());
    }

    public static void main(String[] args) {
        GuiTest gt = new GuiTest();
        gt.t1();

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

}
