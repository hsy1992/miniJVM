/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

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
import static org.mini.nk.NK.nk_window_get_canvas;

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


    @Override
    public void run() {

        if (!Glfw.glfwInit()) {
            System.out.println("glfw init error.");
            System.exit(1);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        win = Glfw.glfwCreateWindow(width, height, "hello glfw", 0, 0);
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
        long[] atlas = {0L};
        NK.nk_glfw3_font_stash_begin(atlas);
        System.out.println("atlas=" + atlas[0]);
        NK.nk_glfw3_font_stash_end();
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
        nk_glfw3_shutdown();
        glfwTerminate();
        GToolkit.removeForm(ctx);
        ctx = 0;
    }

}
