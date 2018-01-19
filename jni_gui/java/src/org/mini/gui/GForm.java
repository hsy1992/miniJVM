/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.Glfw;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MAJOR;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MINOR;
import static org.mini.glfw.Glfw.glfwWindowHint;
import static org.mini.glfw.Glfw.glfwWindowShouldClose;
import static org.mini.nk.NK.NK_GLFW3_INSTALL_CALLBACKS;
import static org.mini.nk.NK.nk_glfw3_font_stash_begin;
import static org.mini.nk.NK.nk_glfw3_font_stash_end;
import static org.mini.nk.NK.nk_glfw3_init;

/**
 *
 * @author gust
 */
public class GForm extends GContainer implements Runnable {

    Thread main;
    String title;
    int width;
    int height;
    long native_window_ptr;

    public GForm(String title, int width, int height) {
        this.title = title;
        this.width = width;
        this.height = height;
        main = new Thread(this);
        main.start();

    }

    @Override
    public void run() {
        Glfw.glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        long glfwwin = Glfw.glfwCreateWindow(width, height, "hello glfw", 0, 0);
        Glfw.glfwMakeContextCurrent(glfwwin);
        native_window_ptr = nk_glfw3_init(glfwwin, NK_GLFW3_INSTALL_CALLBACKS);
        long[] atlas = {0L};
        nk_glfw3_font_stash_begin(atlas);
        nk_glfw3_font_stash_end();
        GToolkit.putForm(native_window_ptr, this);
        while (!glfwWindowShouldClose(glfwwin)) {
            try {
                update(native_window_ptr);
            } catch (Exception ex) {
            }
        }
        GToolkit.removeForm(native_window_ptr);
        native_window_ptr = 0;
    }

}
