/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.nk.NK.NK_WINDOW_BORDER;
import static org.mini.nk.NK.NK_WINDOW_MINIMIZABLE;
import static org.mini.nk.NK.NK_WINDOW_MOVABLE;
import static org.mini.nk.NK.NK_WINDOW_SCALABLE;
import static org.mini.nk.NK.NK_WINDOW_TITLE;
import static org.mini.nk.NK.nk_begin;
import static org.mini.nk.NK.nk_end;

/**
 *
 * @author gust
 */
public class GWindow extends GObject {

    String title;
    int x;
    int y;
    int w;
    int h;
    GWindowContents winContents;
    int background_rgba;

    public GWindow(String title, int left, int top, int width, int height, GWindowContents con) {
        this.title = title;
        this.x = left;
        this.y = top;
        this.w = width;
        this.h = height;
        winContents = con;
    }

    void setBackground(int rgba) {
        background_rgba = rgba;
    }

    @Override
    public boolean update(long win_ptr) {
        int ret = nk_begin(win_ptr, title, new float[]{x, y, w, h}, NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE);
        if (ret != 0 && winContents != null) {
            winContents.updateContents(win_ptr, this);
        }
        nk_end(win_ptr);
        return true;
    }

}
