package test;

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

    int Triangles = 0, NumVAOs = 1;
    int ArrayBuffer = 0, NumBuffers = 1;
    int vPosition = 0;

    int[] VAOs = new int[NumVAOs];
    int[] Buffers = new int[NumBuffers];

    int NumVertices = 6;

//---------------------------------------------------------------------  
//  
// init  
//  
// init()函数用于设置我们后面会用到的一些数据.例如顶点信息,纹理等  
//  
    void init() {
        GL.glGenVertexArrays(NumVAOs, VAOs, 0);
        GL.glBindVertexArray(VAOs[Triangles]);

        // 我们首先指定了要渲染的两个三角形的位置信息.  
        float[] vertices = new float[]{
            -0.90f, -0.90f, // Triangle 1  
            0.85f, -0.90f,
            -0.90f, 0.85f,
            0.90f, -0.85f, // Triangle 2  
            0.90f, 0.90f,
            -0.85f, 0.90f
        };

        GL.glGenBuffers(NumBuffers, Buffers, 0);
        GL.glBindBuffer(GL.GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
        GL.glBufferData(GL.GL_ARRAY_BUFFER, vertices.length * 4,
                vertices, 0, GL.GL_STATIC_DRAW);

//        // 然后使用了必需的vertex和fragment shaders  
//        ShaderInfo shaders[] = {
//            {GL_VERTEX_SHADER, "triangles.vert"},
//            {GL_FRAGMENT_SHADER, "triangles.frag"},
//            {GL_NONE, NULL}
//        };
        int vertex_shader, fragment_shader, program;
        vertex_shader = GL.glCreateShader(GL.GL_VERTEX_SHADER);
        String s0 = "#version 330 core  \n"
                + "layout(location = 0) in vec4 vPosition;  \n"
                + "void  \n"
                + "main()  \n"
                + " {  \n"
                + "     gl_Position = vPosition;  \n"
                + "} \000";
        GL.glShaderSource(vertex_shader, s0.getBytes());
        GL.glCompileShader(vertex_shader);

        String s1 = "#version 330 core  \n"
                + "out vec4 fColor;  \n"
                + "void  \n"
                + "main()  \n"
                + "{  \n"
                + "fColor = vec4(0.0, 0.0, 1.0, 1.0);  \n"
                + "}  \000";
        fragment_shader = GL.glCreateShader(GL.GL_FRAGMENT_SHADER);
        GL.glShaderSource(fragment_shader, s1.getBytes());
        GL.glCompileShader(fragment_shader);

        program = GL.glCreateProgram();
        GL.glAttachShader(program, vertex_shader);
        GL.glAttachShader(program, fragment_shader);
        GL.glLinkProgram(program);

//        // LoadShaders()是我们自定义(这里没有给出)的一个函数,  
//        // 用于简化为GPU准备shaders的过程,后面会详细讲述  
//        int program = LoadShaders(shaders);
//        GL.glUseProgram(program);
        // 最后这部分我们成为shader plumbing,  
        // 我们把需要的数据和shader程序中的变量关联在一起,后面会详细讲述  
        GL.glVertexAttribPointer(vPosition, 2, GL.GL_FLOAT, GL.GL_FALSE, 0, 0);
        GL.glEnableVertexAttribArray(vPosition);
    }

//---------------------------------------------------------------------  
//  
// display  
//  
// 这个函数是真正进行渲染的地方.它调用OpenGL的函数来请求数据进行渲染.  
// 几乎所有的display函数都会进行下面的三个步骤.  
//  
    void display() {
        // 1. 调用glClear()清空窗口  
        GL.glClear(GL.GL_COLOR_BUFFER_BIT);

        // 2. 发起OpenGL调用来请求渲染你的对象  
        GL.glBindVertexArray(VAOs[Triangles]);
        GL.glDrawArrays(GL.GL_TRIANGLES, 0, NumVertices);

    }

    void draw() {
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

            //init();
            long last = System.currentTimeMillis(), now;
            int count = 0;
            while (!Glfw.glfwWindowShouldClose(win)) {

                //display();
                draw();

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
