package test;

import org.mini.gl.GL;
import static org.mini.gl.GL.GL_ARRAY_BUFFER;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_FALSE;
import static org.mini.gl.GL.GL_FLOAT;
import static org.mini.gl.GL.GL_FRAGMENT_SHADER;
import static org.mini.gl.GL.GL_STATIC_DRAW;
import static org.mini.gl.GL.GL_TRIANGLES;
import static org.mini.gl.GL.GL_TRUE;
import static org.mini.gl.GL.GL_VERTEX_SHADER;
import static org.mini.gl.GL.glAttachShader;
import static org.mini.gl.GL.glBindBuffer;
import static org.mini.gl.GL.glBindVertexArray;
import static org.mini.gl.GL.glClear;
import static org.mini.gl.GL.glCompileShader;
import static org.mini.gl.GL.glCreateProgram;
import static org.mini.gl.GL.glCreateShader;
import static org.mini.gl.GL.glDrawArrays;
import static org.mini.gl.GL.glEnableVertexAttribArray;
import static org.mini.gl.GL.glGenBuffers;
import static org.mini.gl.GL.glGenVertexArrays;
import static org.mini.gl.GL.glLinkProgram;
import static org.mini.gl.GL.glShaderSource;
import static org.mini.gl.GL.glUseProgram;
import static org.mini.gl.GL.glVertexAttribPointer;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MAJOR;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MINOR;
import static org.mini.glfw.Glfw.GLFW_KEY_ESCAPE;
import static org.mini.glfw.Glfw.GLFW_MOUSE_BUTTON_2;
import static org.mini.glfw.Glfw.GLFW_MOUSE_BUTTON_LEFT;
import static org.mini.glfw.Glfw.GLFW_OPENGL_CORE_PROFILE;
import static org.mini.glfw.Glfw.GLFW_OPENGL_FORWARD_COMPAT;
import static org.mini.glfw.Glfw.GLFW_OPENGL_PROFILE;
import static org.mini.glfw.Glfw.GLFW_PRESS;
import static org.mini.glfw.Glfw.GLFW_TRUE;
import static org.mini.glfw.Glfw.glfwCreateWindow;
import static org.mini.glfw.Glfw.glfwInit;
import static org.mini.glfw.Glfw.glfwMakeContextCurrent;
import static org.mini.glfw.Glfw.glfwPollEvents;
import static org.mini.glfw.Glfw.glfwSetCallback;
import static org.mini.glfw.Glfw.glfwSetWindowShouldClose;
import static org.mini.glfw.Glfw.glfwSwapBuffers;
import static org.mini.glfw.Glfw.glfwTerminate;
import static org.mini.glfw.Glfw.glfwWindowHint;
import static org.mini.glfw.Glfw.glfwWindowShouldClose;
import org.mini.glfw.GlfwCallbackAdapter;
import org.mini.glfm.Gutil;
import static org.mini.glfw.Glfw.glfwGetFramebufferWidth;
import static org.mini.glfw.Glfw.glfwGetFramebufferHeight;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author gust
 */
public class Shader {

    boolean exit = false;
    long curWin;
    int mx, my;

    class CallBack extends GlfwCallbackAdapter {

        @Override
        public void key(long window, int key, int scancode, int action, int mods) {
            System.out.println("key:" + key + " action:" + action);
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }

        @Override
        public void mouseButton(long window, int button, boolean pressed) {
            if (window == curWin) {
                String bt = button == GLFW_MOUSE_BUTTON_LEFT ? "LEFT" : button == GLFW_MOUSE_BUTTON_2 ? "RIGHT" : "OTHER";
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

//---------------------------------------------------------------------  
//  
// init  
//  
// init()函数用于设置我们后面会用到的一些数据.例如顶点信息,纹理等  
//  
    int Triangles = 0, NumVAOs = 1;
    int ArrayBuffer = 0, NumBuffers = 1;
    int vPosition = 0;

    int[] VAOs = new int[NumVAOs];
    int[] Buffers = new int[NumBuffers];

    int NumVertices = 6;

    byte[] s_v = Gutil.toUtf8(
            "#version 330   \n"
            + "attribute vec3 position;\n"
            + "attribute vec2 texcoord;\n"
            + "\n"
            + "varying vec2 vTexcoord;\n"
            + "\n"
            + "void main()\n"
            + "{\n"
            + "    gl_Position = vec4(position, 1.0);\n"
            + "    vTexcoord = texcoord;\n"
            + "}"
    );
//            byte[] s_v = Gutil.toUtf8("#version 330   \n"
//                + "layout(location = 0) in vec4 vPosition;  \n"
//                + "void  \n"
//                + "main()  \n"
//                + " {  \n"
//                + "     gl_Position = vPosition;  \n"
//                + "} \000");
    byte[] s_f = Gutil.toUtf8(
            "#version 330   \n"
            + "precision mediump float;\n"
            + "\n"
            + "uniform sampler2D image;\n"
            + "\n"
            + "varying vec2 vTexcoord;\n"
            + "\n"
            + "void main()\n"
            + "{\n"
            + "    float block = 150.0;\n"
            + "    float delta = 1.0/block;\n"
            + "    vec4 color = vec4(0.0);\n"
            + "    \n"
            + "    float factor[9];\n"
            + "    factor[0] = 0.0947416; factor[1] = 0.118318; factor[2] = 0.0947416;\n"
            + "    factor[3] = 0.118318; factor[4] = 0.147761; factor[5] = 0.118318;\n"
            + "    factor[6] = 0.0947416; factor[7] = 0.118318; factor[8] = 0.0947416;\n"
            + "    \n"
            + "    for (int i = -1; i <= 1; i++) {\n"
            + "        for (int j = -1; j <= 1; j++) {\n"
            + "            float x = max(0.0, vTexcoord.x + float(i) * delta);\n"
            + "            float y = max(0.0, vTexcoord.y + float(i) * delta);\n"
            + "            color += texture2D(image, vec2(x, y)) * factor[(i+1)*3+(j+1)];\n"
            + "        }\n"
            + "    }\n"
            + "    \n"
            + "    gl_FragColor = vec4(vec3(color), 1.0);\n"
            + "}"
    );
//            byte[] s_f = Gutil.toUtf8("#version 330   \n"
//                + "out vec4 fColor;  \n"
//                + "void  \n"
//                + "main()  \n"
//                + "{  \n"
//                + "fColor = vec4(0.0, 0.0, 1.0, 1.0);  \n"
//                + "}  \000");

    void init() {
        glGenVertexArrays(NumVAOs, VAOs, 0);
        glBindVertexArray(VAOs[Triangles]);

        // 我们首先指定了要渲染的两个三角形的位置信息.  
        float[] vertices = new float[]{
            -0.90f, -0.90f, // Triangle 1  
            0.85f, -0.90f,
            -0.90f, 0.85f,
            0.90f, -0.85f, // Triangle 2  
            0.90f, 0.90f,
            -0.85f, 0.90f
        };

        glGenBuffers(NumBuffers, Buffers, 0);
        glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
        GL.glBufferData(GL_ARRAY_BUFFER, (long) (vertices.length * 4), vertices, 0, GL_STATIC_DRAW);

        int vertex_shader, fragment_shader, program;
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, new byte[][]{s_v}, null, 0);
        glCompileShader(vertex_shader);
        byte[] szLog = new byte[1024];
        int[] return_val = {0};
        GL.glGetShaderiv(vertex_shader, GL.GL_COMPILE_STATUS, return_val, 0);
        if (return_val[0] == GL_FALSE) {
            GL.glGetShaderInfoLog(vertex_shader, szLog.length, return_val, 0, szLog);
            System.out.println("Compile Shader fail error :" + new String(szLog, 0, return_val[0]) + "\n");
        }
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, new byte[][]{s_f}, null, 0);
        glCompileShader(fragment_shader);
        GL.glGetShaderiv(fragment_shader, GL.GL_COMPILE_STATUS, return_val, 0);
        if (return_val[0] == GL_FALSE) {
            GL.glGetShaderInfoLog(fragment_shader, szLog.length, return_val, 0, szLog);
            System.out.println("Compile Shader fail error :" + new String(szLog, 0, return_val[0]) + "\n");
        }
        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glUseProgram(program);
        // 最后这部分我们成为shader plumbing,  
        // 我们把需要的数据和shader程序中的变量关联在一起,后面会详细讲述  
        glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, null, 0);
        glEnableVertexAttribArray(vPosition);

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
        glClear(GL_COLOR_BUFFER_BIT);

//          // 2. 发起OpenGL调用来请求渲染你的对象  
        glBindVertexArray(VAOs[Triangles]);
        glDrawArrays(GL_TRIANGLES, 0, NumVertices);

        try {
            Thread.sleep(10);
        } catch (InterruptedException ex) {
        }

    }

    void t1() {
        glfwInit();
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//        glfwWindowHint(GLFW_DEPTH_BITS, 16);
//        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        long win = glfwCreateWindow(640, 480, "hello glfw".getBytes(), 0, 0);
        if (win != 0) {
            glfwSetCallback(win, new CallBack());
            glfwMakeContextCurrent(win);
            //glfwSwapInterval(1);

            int w = glfwGetFramebufferWidth(win);
            int h = glfwGetFramebufferHeight(win);
            System.out.println("w=" + w + "  ,h=" + h);
            init();
            long last = System.currentTimeMillis(), now;
            int count = 0;
            while (!glfwWindowShouldClose(win)) {

                display();

                glfwPollEvents();
                glfwSwapBuffers(win);
                count++;
                now = System.currentTimeMillis();
                if (now - last > 1000) {
                    System.out.println("fps:" + count);
                    last = now;
                    count = 0;
                }
            }
            glfwTerminate();
        }
    }

    public static void main(String[] args) {
        Shader gt = new Shader();
        gt.t1();

    }
}
