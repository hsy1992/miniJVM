package test;

import java.util.Random;
import org.mini.gl.GL;
import org.mini.glfw.Glfw;
import org.mini.glfw.GlfwCallbackAdapter;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author gust
 */
public class TestGL {

    boolean exit = false;
    long curWin;
    int mx, my;

    class CallBack extends GlfwCallbackAdapter {

        @Override
        public void key(long window, int key, int scancode, int action, int mods) {
            System.out.println("key:" + key + " action:" + action);
            if (key == Glfw.GLFW_KEY_ESCAPE && action == Glfw.GLFW_PRESS) {
                Glfw.glfwSetWindowShouldClose(window, Glfw.GLFW_TRUE);
            }
        }

        @Override
        public void mouseButton(long window, int button, boolean pressed) {
            if (window == curWin) {
                String bt = button == Glfw.GLFW_MOUSE_BUTTON_LEFT ? "LEFT" : button == Glfw.GLFW_MOUSE_BUTTON_2 ? "RIGHT" : "OTHER";
                String press = pressed ? "pressed" : "released";
                System.out.println(bt + " " + mx + " " + my + "  " + press);
            }
        }

        @Override
        public void cursorPos(long window, int x, int y) {
            curWin = window;
            mx = x;
            my = y;
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


    byte[] mask = new byte[]{
        (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, //   这是最下面的一行

        (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
        (byte) 0x03, (byte) 0x80, (byte) 0x01, (byte) 0xC0, //   麻

        (byte) 0x06, (byte) 0xC0, (byte) 0x03, (byte) 0x60, //   烦

        (byte) 0x04, (byte) 0x60, (byte) 0x06, (byte) 0x20, //   的

        (byte) 0x04, (byte) 0x30, (byte) 0x0C, (byte) 0x20, //   初

        (byte) 0x04, (byte) 0x18, (byte) 0x18, (byte) 0x20, //   始

        (byte) 0x04, (byte) 0x0C, (byte) 0x30, (byte) 0x20, //   化

        (byte) 0x04, (byte) 0x06, (byte) 0x60, (byte) 0x20, //   ，

        (byte) 0x44, (byte) 0x03, (byte) 0xC0, (byte) 0x22, //   不

        (byte) 0x44, (byte) 0x01, (byte) 0x80, (byte) 0x22, //   建

        (byte) 0x44, (byte) 0x01, (byte) 0x80, (byte) 0x22, //   议

        (byte) 0x44, (byte) 0x01, (byte) 0x80, (byte) 0x22, //   使

        (byte) 0x44, (byte) 0x01, (byte) 0x80, (byte) 0x22, //   用

        (byte) 0x44, (byte) 0x01, (byte) 0x80, (byte) 0x22,
        (byte) 0x44, (byte) 0x01, (byte) 0x80, (byte) 0x22,
        (byte) 0x66, (byte) 0x01, (byte) 0x80, (byte) 0x66,
        (byte) 0x33, (byte) 0x01, (byte) 0x80, (byte) 0xCC,
        (byte) 0x19, (byte) 0x81, (byte) 0x81, (byte) 0x98,
        (byte) 0x0C, (byte) 0xC1, (byte) 0x83, (byte) 0x30,
        (byte) 0x07, (byte) 0xE1, (byte) 0x87, (byte) 0xE0,
        (byte) 0x03, (byte) 0x3F, (byte) 0xFC, (byte) 0xC0,
        (byte) 0x03, (byte) 0x31, (byte) 0x8C, (byte) 0xC0,
        (byte) 0x03, (byte) 0x3F, (byte) 0xFC, (byte) 0xC0,
        (byte) 0x06, (byte) 0x64, (byte) 0x26, (byte) 0x60,
        (byte) 0x0C, (byte) 0xCC, (byte) 0x33, (byte) 0x30,
        (byte) 0x18, (byte) 0xCC, (byte) 0x33, (byte) 0x18,
        (byte) 0x10, (byte) 0xC4, (byte) 0x23, (byte) 0x08,
        (byte) 0x10, (byte) 0x63, (byte) 0xC6, (byte) 0x08,
        (byte) 0x10, (byte) 0x30, (byte) 0x0C, (byte) 0x08,
        (byte) 0x10, (byte) 0x18, (byte) 0x18, (byte) 0x08,
        (byte) 0x10, (byte) 0x00, (byte) 0x00, (byte) 0x08 // 这是最上面的一行
    };

    float Pi = 3.1415926f;

    void draw1() {
        GL.glClear(GL.GL_COLOR_BUFFER_BIT);

        GL.glEnable(GL.GL_POLYGON_STIPPLE);

        GL.glPolygonStipple(mask, 0);

        GL.glRectf(-0.5f, -0.5f, 0.0f, 0.0f);   // 在左下方绘制一个有镂空效果的正方形

        GL.glDisable(GL.GL_POLYGON_STIPPLE);

        GL.glRectf(0.0f, 0.0f, 0.5f, 0.5f);     // 在右上方绘制一个无镂空效果的正方形

        //GL.glShadeModel(GL.GL_FLAT);
        GL.glClear(GL.GL_COLOR_BUFFER_BIT);
        GL.glBegin(GL.GL_TRIANGLE_FAN);
        {
            GL.glColor3f(1.0f, 1.0f, 1.0f);
            GL.glVertex2f(0.0f, 0.0f);
            for (int i = 0; i <= 8; ++i) {
                GL.glColor3f(i & 0x04, i & 0x02, i & 0x01);
                GL.glVertex2f((float) Math.cos(i * Pi / 4), (float) Math.sin(i * Pi / 4));
            }
        }
        GL.glEnd();
    }

    void draw0() {
        GL.glColor3f(0.f, 1.f, 0.f);
        //GL.glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
        GL.glPointSize(5.f);
        GL.glBegin(GL.GL_POINTS);
        {
            GL.glColor3f(1f, 0.f, 0.f);
            GL.glVertex2f(-0.8f, 0.1f);
            GL.glVertex2f(-0.8f, 0.2f);
        }
        GL.glEnd();
        GL.glBegin(GL.GL_POLYGON);
        {
            float R = 0.2f;
            int n = 20;
            for (int i = 0; i < n; ++i) {
                GL.glVertex2f((float) (R * Math.cos(2 * Math.PI / n * i)), (float) (R * Math.sin(2 * Math.PI / n * i)));
            }
        }
        GL.glEnd();
        GL.glLineWidth(15.f);
        float x, factor = 0.1f;
        GL.glBegin(GL.GL_LINES);
        {
            GL.glVertex2f(-1.0f, 0.0f);
            GL.glVertex2f(1.0f, 0.0f);         // 以上两个点可以画x轴
            GL.glVertex2f(0.0f, -1.0f);
            GL.glVertex2f(0.0f, 1.0f);         // 以上两个点可以画y轴
        }
        GL.glEnd();

        GL.glBegin(GL.GL_LINE_STRIP);
        {
            for (x = -1.0f / factor; x < 1.0f / factor; x += 0.01f) {
                GL.glVertex2f(x * factor, (float) Math.sin(x) * factor);
            }
        }
        GL.glEnd();
    }

    void t1() {
        Glfw.glfwInit();
//        Glfw.glfwWindowHint(Glfw.GLFW_CONTEXT_VERSION_MAJOR, 2);
//        Glfw.glfwWindowHint(Glfw.GLFW_CONTEXT_VERSION_MINOR, 0);
        Glfw.glfwWindowHint(Glfw.GLFW_DEPTH_BITS, 16);
//        Glfw.glfwWindowHint(Glfw.GLFW_TRANSPARENT_FRAMEBUFFER, Glfw.GLFW_TRUE);
        long win = Glfw.glfwCreateWindow(300, 300, "hello glfw", 0, 0);
        if (win != 0) {
            Glfw.glfwSetCallback(win, new CallBack());
            Glfw.glfwMakeContextCurrent(win);
//            Glfw.glfwSwapInterval(1);

            int w = Glfw.glfwGetFramebufferSizeW(win);
            int h = Glfw.glfwGetFramebufferSizeH(win);
            System.out.println("w=" + w + "  ,h=" + h);

            long last = System.currentTimeMillis(), now;
            int count = 0;
            while (!Glfw.glfwWindowShouldClose(win)) {

                draw1();
                /* Timing */
 /* Draw one frame */
//                displayB();

                Glfw.glfwPollEvents();
                Glfw.glfwSwapBuffers(win);

                count++;
                now = System.currentTimeMillis();
                if (now - last > 1000) {
                    System.out.println("fps:" + count);
                    last = now;
                    count = 0;
                }
            }
            Glfw.glfwTerminate();
        }
    }

    public static void main(String[] args) {
        TestGL gt = new TestGL();
        gt.t1();

    }
}
