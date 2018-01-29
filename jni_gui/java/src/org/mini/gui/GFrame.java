/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.utils.Gutil;
import org.mini.nk.NK;
import static org.mini.nk.NK.NK_WINDOW_BORDER;
import static org.mini.nk.NK.NK_WINDOW_MINIMIZABLE;
import static org.mini.nk.NK.NK_WINDOW_MOVABLE;
import static org.mini.nk.NK.NK_WINDOW_SCALABLE;
import static org.mini.nk.NK.NK_WINDOW_TITLE;
import static org.mini.nk.NK.nk_begin;
import static org.mini.nk.NK.nk_end;
import static org.mini.nk.NK.nk_window_get_canvas;

/**
 *
 * @author gust
 */
public class GFrame extends GObject {

    String title;
    byte[] title_barr;

    GFrameContents frameContents;
    int background_rgba;
    float[] boundle;
    GGraphics g = new GGraphics();
    long ctx;
    int frameMode = NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE
            | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE;

    public GFrame(String title, int left, int top, int width, int height, GFrameContents con) {
        setTitle(title);
        boundle = new float[]{left, top, width, height};
        frameContents = con;
        frameContents.init(this);
    }

    public void setTitle(String title) {
        this.title = title;
        this.title_barr = Gutil.toUtf8(title + "\000");
    }

    public String getTitle() {
        return title;
    }

    public byte[] getTitleBytes() {
        return title_barr;
    }

    public void setFrameMode(int mode) {
        frameMode = mode;
    }

    public int getFrameMode() {
        return frameMode;
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
        int ret = nk_begin(ctx, title_barr, boundle, frameMode);
        if (ret != 0 && frameContents != null) {
            g.frame_bound = NK.nk_window_get_bounds(ctx);
            g.font = getForm(ctx).getFont();
            frameContents.updateContents(ctx, this);
        }
        nk_end(ctx);
        return true;
    }

}
