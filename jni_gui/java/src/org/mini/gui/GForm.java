/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.utils.StbFont;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_FALSE;
import static org.mini.gl.GL.GL_TRUE;
import static org.mini.gl.GL.glClear;
import static org.mini.gl.GL.glViewport;
import org.mini.glfw.Glfw;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MAJOR;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MINOR;
import static org.mini.glfw.Glfw.GLFW_OPENGL_CORE_PROFILE;
import static org.mini.glfw.Glfw.GLFW_OPENGL_FORWARD_COMPAT;
import static org.mini.glfw.Glfw.GLFW_OPENGL_PROFILE;
import static org.mini.glfw.Glfw.GLFW_RESIZABLE;
import static org.mini.glfw.Glfw.glfwPollEvents;
import static org.mini.glfw.Glfw.glfwSwapBuffers;
import static org.mini.glfw.Glfw.glfwSwapInterval;
import static org.mini.glfw.Glfw.glfwTerminate;
import static org.mini.glfw.Glfw.glfwWindowHint;
import static org.mini.glfw.Glfw.glfwWindowShouldClose;
import org.mini.glfw.GlfwCallback;
import org.mini.glfw.utils.Gutil;
import org.mini.nk.NK;
import static org.mini.nk.NK.nk_glfw3_init;
import static org.mini.nk.NK.nk_glfw3_new_frame;
import static org.mini.nk.NK.nk_glfw3_render;
import static org.mini.nk.NK.nk_glfw3_shutdown;
import static org.mini.nk.NK.nk_true;

/**
 *
 * @author gust
 */
public class GForm extends GContainer implements Runnable {

    String title;
    int width;
    int height;
    long win; //glfw win
    long ctx; //nk contex
    GlfwCallback callback;
    long nkfont;
    static StbFont gfont;

    static int MAX_VERTEX_BUFFER = 512 * 1024;
    static int MAX_ELEMENT_BUFFER = 128 * 1024;
    int[] unicode_range = {
        0x0020, 0xFFFF,
        0
    };

    public GForm(String title, int width, int height) {
        this.title = title + "\000";
        this.width = width;
        this.height = height;
    }

    public void setCallBack(GlfwCallback callback) {
        this.callback = callback;
        if (win != 0) {
            Glfw.glfwSetCallback(win, callback);
        }
    }

    static public void setGFont(StbFont pgfont) {
        gfont = pgfont;
    }

    static public StbFont getGFont() {
        return gfont;
    }

    public long getNkFont() {
        return nkfont;
    }

    @Override
    public void init() {

        if (!Glfw.glfwInit()) {
            System.out.println("glfw init error.");
            System.exit(1);
        }
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        win = Glfw.glfwCreateWindow(width, height, Gutil.toUtf8(title), 0, 0);
        if (win == 0) {
            glfwTerminate();
            System.exit(1);
        }
        Glfw.glfwMakeContextCurrent(win);
        glfwSwapInterval(nk_true);
        ctx = nk_glfw3_init(win, NK.NK_GLFW3_INSTALL_CALLBACKS);
//        if (callback != null) {
//            Glfw.glfwSetCallback(win, callback);
//        }
        //字体
//        long[] atlas = {0};
//        NK.nk_glfw3_font_stash_begin(atlas);
//        NK.nk_glfw3_font_stash_end();

        if (gfont == null) {
            gfont = Gutil.getDefaultFont();
        }
//        font = NK.nk_load_font_file("./wqymhei.ttc\000".getBytes(), 15);
        byte[] fontBuffer = gfont.getFontBytes();
        nkfont = NK.nk_load_font_memory(GToolkit.getArrayDataPtr(fontBuffer), fontBuffer.length, 14);
        NK.nk_style_set_font(ctx, NK.nk_get_font_handle(nkfont));
    }

    @Override
    public void run() {
        if (ctx == 0) {
            System.out.println("gl context not inited.");
            return;
        }
        //
        GToolkit.putForm(ctx, this);

        while (!glfwWindowShouldClose(win)) {
            try {
                glfwPollEvents();
                nk_glfw3_new_frame();
                //user define contents
                update(ctx);
                //clear
                glClear(GL_COLOR_BUFFER_BIT);
                //render
                NK.nk_glfw3_render(NK.NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
                glfwSwapBuffers(win);
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
        nk_glfw3_shutdown();
        glfwTerminate();
        GToolkit.removeForm(ctx);
        ctx = 0;
    }

}
