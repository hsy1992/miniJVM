package test;

import org.mini.gl.GL;
import static org.mini.gl.GL.GL_AMBIENT;
import static org.mini.gl.GL.GL_AMBIENT_AND_DIFFUSE;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_DEPTH_BUFFER_BIT;
import static org.mini.gl.GL.GL_DEPTH_TEST;
import static org.mini.gl.GL.GL_DIFFUSE;
import static org.mini.gl.GL.GL_EMISSION;
import static org.mini.gl.GL.GL_FALSE;
import static org.mini.gl.GL.GL_LIGHT0;
import static org.mini.gl.GL.GL_LIGHTING;
import static org.mini.gl.GL.GL_POSITION;
import static org.mini.gl.GL.GL_SHININESS;
import static org.mini.gl.GL.GL_SPECULAR;
import static org.mini.gl.GL.GL_TRUE;
import static org.mini.gl.GL.glClear;
import static org.mini.gl.GL.glDepthMask;
import static org.mini.gl.GL.glEnable;
import static org.mini.gl.GL.glLightfv;
import static org.mini.gl.GL.glMaterialf;
import static org.mini.gl.GL.glMaterialfv;
import static org.mini.gl.GL.glPopMatrix;
import static org.mini.gl.GL.glPushMatrix;
import static org.mini.gl.GL.glRotatef;
import static org.mini.gl.GL.glTranslatef;
import org.mini.glfw.Glfw;
import org.mini.glfw.GlfwCallbackAdapter;
import org.mini.glfw.utils.Gutil;
import static test.Gears.angle;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author gust
 */
public class Light {

    boolean exit = false;
    long curWin;
    int mx, my;
    boolean rotate = true;

    class CallBack extends GlfwCallbackAdapter {

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
            if (window == curWin) {
                String bt = button == Glfw.GLFW_MOUSE_BUTTON_LEFT ? "LEFT" : button == Glfw.GLFW_MOUSE_BUTTON_2 ? "RIGHT" : "OTHER";
                String press = pressed ? "pressed" : "released";
                System.out.println(bt + " " + mx + " " + my + "  " + press);
            }
            if (pressed) {
                rotate = false;
            } else {
                rotate = true;
            }
        }

        @Override
        public void drop(long window, int count, String[] paths) {
            for (int i = 0; i < count; i++) {
                System.out.println(i + " " + paths[i]);
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
            GL.glViewport(0, 0, x, y);
        }
    }

    int w, h;

    void t1() {
        Glfw.glfwInit();
        long win = Glfw.glfwCreateWindow(500, 500, "hello glfw", 0, 0);
        if (win != 0) {
            Glfw.glfwSetCallback(win, new CallBack());
            Glfw.glfwMakeContextCurrent(win);
//            Glfw.glfwSwapInterval(1);

            w = Glfw.glfwGetFramebufferSizeW(win);
            h = Glfw.glfwGetFramebufferSizeH(win);
            System.out.println("w=" + w + "  ,h=" + h);

            long last = System.currentTimeMillis(), now;
            int count = 0;
            while (!Glfw.glfwWindowShouldClose(win)) {
                int sleep = 100;

                draw2();
                GL.glLoadIdentity();
                Gutil.drawCood();

                Glfw.glfwPollEvents();
                Glfw.glfwSwapBuffers(win);

                try {
                    Thread.sleep(sleep);
                } catch (InterruptedException ex) {
                }
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
        Light gt = new Light();
        gt.t1();

    }
    //===========================================================
    //===========================================================
    //===========================================================

    Ball red = new Ball(3f, 16, Ball.SOLID);
    Ball blue = new Ball(1f, 10, Ball.SOLID);
    Ball yellow = new Ball(0.5f, 10, Ball.SOLID);

    float cube_angel = 0.f;
    Cube cube = new Cube(0.1f, 0.1f, 0.1f);
    static float angle = 0.f;
    static float angley = 0.f;

    void draw2() {
        // 定义一些材质颜色

        // 清除屏幕
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 创建透视效果视图  
        GL.glMatrixMode(GL.GL_PROJECTION);//对投影矩阵操作  
        GL.glLoadIdentity();//将坐标原点移到中心  
        Gutil.gluPerspective(90.0f, 1.0f, 1.0f, 40.0f);//设置透视投影矩阵  
        GL.glMatrixMode(GL.GL_MODELVIEW);//对模型视景矩阵操作  
        GL.glLoadIdentity();
        Gutil.gluLookAt(0.0, 5.0, -15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);//视点转换  

        float da = 2;
        if (rotate) {
            angle += da;     //递增旋转角度计数器
            angley += da * 12;     //递增旋转角度计数器
        }
        if (angle >= 360.0f) //如果已旋转一周,复位计数器
        {
            angle = 0.0f;
        }

//        GL.glLightModelf(GL.GL_LIGHT_MODEL_AMBIENT, 0.5f);
//        GL.glLightModelf(GL.GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
        // 定义太阳光源，它是一种白色的光源
        {
            float sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
            float sun_light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
            float sun_light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
            float sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

            glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position, 0);
            glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient, 0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse, 0);
            glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular, 0);

            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHTING);
            glEnable(GL_DEPTH_TEST);
        }

        // 定义太阳的材质并绘制太阳
        {
            float sun_mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
            float sun_mat_diffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
            float sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
            float sun_mat_emission[] = {0.8f, 0.8f, 0.0f, 1.0f};
            float sun_mat_shininess = 0.0f;

            glMaterialfv(GL.GL_FRONT, GL_AMBIENT, sun_mat_ambient, 0);
            glMaterialfv(GL.GL_FRONT, GL_DIFFUSE, sun_mat_diffuse, 0);
            glMaterialfv(GL.GL_FRONT, GL_SPECULAR, sun_mat_specular, 0);
            glMaterialfv(GL.GL_FRONT, GL_EMISSION, sun_mat_emission, 0);
            GL.glMaterialf(GL.GL_FRONT, GL_SHININESS, sun_mat_shininess);

            glRotatef(angle, 0.0f, -1.0f, 0.0f);
            red.draw();
//        glutSolidSphere(2.0, 40, 32);
        }

        // 定义地球的材质并绘制地球
        {
            float earth_mat_ambient[] = {0.0f, 0.0f, .5f, 1.0f};
            float earth_mat_diffuse[] = {0.0f, 0.0f, 0.5f, 1.0f};
            float earth_mat_specular[] = {0.0f, 0.0f, .10f, 1.0f};
            float earth_mat_emission[] = {0.0f, 0.0f, 0.2f, 1.0f};
            float earth_mat_shininess = 10.0f;

            glMaterialfv(GL.GL_FRONT, GL_AMBIENT, earth_mat_ambient, 0);
            glMaterialfv(GL.GL_FRONT, GL_DIFFUSE, earth_mat_diffuse, 0);
            glMaterialfv(GL.GL_FRONT, GL_SPECULAR, earth_mat_specular, 0);
            glMaterialfv(GL.GL_FRONT, GL_EMISSION, earth_mat_emission, 0);
            glMaterialf(GL.GL_FRONT, GL_SHININESS, earth_mat_shininess);

            glRotatef(angle, 0.0f, -1.0f, 0.0f);
            glTranslatef(10.0f, 0.0f, 0.0f);
            blue.draw();
            // glutSolidSphere(2.0, 40, 32);
        }

        // 定义月亮的材质并绘制地球
        {
            float moon_mat_ambient[] = {0.3f, 0.1f, 0.0f, 1.0f};
            float moon_mat_diffuse[] = {0.3f, 0.1f, 0.0f, 1.0f};
            float moon_mat_specular[] = {0.1f, 0.0f, 0.0f, 1.0f};
            float moon_mat_emission[] = {0.2f, 0.05f, 0.0f, 1.0f};
            float moon_mat_shininess = 10.0f;

            glMaterialfv(GL.GL_FRONT, GL_AMBIENT, moon_mat_ambient, 0);
            glMaterialfv(GL.GL_FRONT, GL_DIFFUSE, moon_mat_diffuse, 0);
            glMaterialfv(GL.GL_FRONT, GL_SPECULAR, moon_mat_specular, 0);
            glMaterialfv(GL.GL_FRONT, GL_EMISSION, moon_mat_emission, 0);
            glMaterialf(GL.GL_FRONT, GL_SHININESS, moon_mat_shininess);

            glRotatef(angley, 0.0f, -1.0f, 0.0f);
            glTranslatef(2f, 0.0f, 0.0f);
            yellow.draw();
            // glutSolidSphere(2.0, 40, 32);
        }

    }

}
