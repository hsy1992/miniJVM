/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import javax.mini.reflect.Array;
import javax.mini.reflect.vm.RefNative;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.glClear;
import org.mini.glfw.Glfw;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MAJOR;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MINOR;
import static org.mini.glfw.Glfw.glfwPollEvents;
import static org.mini.glfw.Glfw.glfwSwapBuffers;
import static org.mini.glfw.Glfw.glfwSwapInterval;
import static org.mini.glfw.Glfw.glfwTerminate;
import static org.mini.glfw.Glfw.glfwWindowHint;
import static org.mini.glfw.Glfw.glfwWindowShouldClose;
import org.mini.glfw.GlfwCallback;
import org.mini.nk.NK;
import static org.mini.nk.NK.nk_glfw3_init;
import static org.mini.nk.NK.nk_glfw3_shutdown;
import static org.mini.nk.NK.nk_true;

/**
 *
 * @author gust
 */
public class GForm extends GContainer implements Runnable {

    Thread main;
    String title;
    int width;
    int height;
    long win; //glfw win
    long ctx; //nk contex
    GlfwCallback callback;
    long font;

    int[] unicode_range = {
        0x0020, 0xFFFF,
        0
    };

    public GForm(String title, int width, int height) {
        this.title = title;
        this.width = width;
        this.height = height;

        main = new Thread(this);
        main.start();

    }

    public void setCallBack(GlfwCallback callback) {
        this.callback = callback;
        if (win != 0) {
            Glfw.glfwSetCallback(win, callback);
        }
    }

    public long getFont() {
        return font;
    }

    @Override
    public void run() {

        if (!Glfw.glfwInit()) {
            System.out.println("glfw init error.");
            System.exit(1);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        win = Glfw.glfwCreateWindow(width, height, title, 0, 0);
        if (win == 0) {
            glfwTerminate();
            System.exit(1);
        }
        Glfw.glfwMakeContextCurrent(win);
        glfwSwapInterval(nk_true);
        ctx = nk_glfw3_init(win, NK.NK_GLFW3_INSTALL_CALLBACKS);
        if (callback != null) {
            Glfw.glfwSetCallback(win, callback);
        }
        //字体
        
        font = NK.nk_load_font("./wqymhei.ttc\000".getBytes(), 15);
        
        NK.nk_style_set_font(ctx, NK.nk_get_font_handle(font));
        //
        GToolkit.putForm(ctx, this);

        while (!glfwWindowShouldClose(win)) {
            try {
                glfwPollEvents();
                glClear(GL_COLOR_BUFFER_BIT);
                update(ctx);
                glfwSwapBuffers(win);
            } catch (Exception ex) {
            }
        }
        System.out.println("while exit");
        nk_glfw3_shutdown();
        System.out.println("nk_glfw3_shutdown ok");
        glfwTerminate();
        System.out.println("glfwTerminate ok");
        GToolkit.removeForm(ctx);
        ctx = 0;
    }

}
