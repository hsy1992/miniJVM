/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.glfw.utils.Gutil.toUtf8;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_CENTER;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_LEFT;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_MIDDLE;
import static org.mini.glfw.utils.Nutil.nvgBeginPath;
import static org.mini.glfw.utils.Nutil.nvgBoxGradient;
import static org.mini.glfw.utils.Nutil.nvgFill;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFillPaint;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgRoundedRect;
import static org.mini.glfw.utils.Nutil.nvgStroke;
import static org.mini.glfw.utils.Nutil.nvgStrokeColor;
import static org.mini.glfw.utils.Nutil.nvgText;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GEditBox extends GObject {

    String hint;
    byte[] hint_arr;
    char preicon;

    public GEditBox(String hint, int left, int top, int width, int height) {
        setHint(hint);
        boundle[LEFT] = left;
        boundle[TOP] = top;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;
    }

    public void setHint(String hint) {
        this.hint = hint;
        hint_arr = toUtf8(hint);
    }

    /**
     *
     * @param vg
     * @return
     */
    public boolean update(long vg) {
        float x = getX();
        float y = getY();
        float w = getW();
        float h = getH();

        drawEditBox(vg, x, y, w, h);
        return true;
    }

    void drawEditBox(long vg, float x, float y, float w, float h) {

        drawEditBoxBase(vg, x, y, w, h);

        nvgFontSize(vg, 20.0f);
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgText(vg, x + h * 0.3f, y + h * 0.5f, hint_arr, null);
    }

    public static void drawEditBoxBase(long vg, float x, float y, float w, float h) {
        byte[] bg;
        // Edit
        bg = nvgBoxGradient(vg, x + 1, y + 1 + 1.5f, w - 2, h - 2, 3, 4, nvgRGBA(255, 255, 255, 32), nvgRGBA(32, 32, 32, 32));
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, 4 - 1);
        nvgFillPaint(vg, bg);
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, 4 - 0.5f);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
        nvgStroke(vg);
    }
}
