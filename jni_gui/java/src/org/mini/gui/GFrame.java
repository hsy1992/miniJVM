/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.utils.Gutil;
import org.mini.nk.NK;
import static org.mini.nk.NK.NK_ANTI_ALIASING_ON;
import static org.mini.nk.NK.NK_WINDOW_BORDER;
import static org.mini.nk.NK.NK_WINDOW_MINIMIZABLE;
import static org.mini.nk.NK.NK_WINDOW_MOVABLE;
import static org.mini.nk.NK.NK_WINDOW_SCALABLE;
import static org.mini.nk.NK.NK_WINDOW_TITLE;
import static org.mini.nk.NK.nk_begin;
import static org.mini.nk.NK.nk_end;
import static org.mini.nk.NK.nk_glfw3_new_frame;
import static org.mini.nk.NK.nk_glfw3_render;
import static org.mini.nk.NK.nk_window_get_canvas;

/**
 *
 * @author gust
 */
public class GFrame extends GObject {

    byte[] title;

    GFrameContents winContents;
    int background_rgba;
    float[] boundle;
    GGraphics g = new GGraphics();
    long ctx;

    public GFrame(String title, int left, int top, int width, int height, GFrameContents con) {
        this.title = Gutil.toUtf8(title + "\000");
        boundle = new float[]{left, top, width, height};
        winContents = con;
    }

    public void setBackground(int rgba) {
        background_rgba = rgba;
    }

    public GForm getForm(long ctx) {
        return GToolkit.getForm(ctx);
    }

    public GGraphics getGraphics() {
        g.brush = nk_window_get_canvas(ctx);
        return g;
    }

    @Override
    public boolean update(long ctx) {
        this.ctx = ctx;
        nk_glfw3_new_frame();
        int ret = nk_begin(ctx, title, boundle, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE
                | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE);
        if (ret != 0 && winContents != null) {
            g.frame_bound = NK.nk_window_get_bounds(ctx);
            g.font = getForm(ctx).getFont();
            winContents.updateContents(ctx, this);
        }
        nk_end(ctx);
        nk_glfw3_render(NK_ANTI_ALIASING_ON);
        return true;
    }

}
