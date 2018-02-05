/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.glfw.utils.Gutil.toUtf8;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_LEFT;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_MIDDLE;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgText;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GLabel extends GObject {

    String text;
    byte[] text_arr;
    char preicon;

    public GLabel(String text, int left, int top, int width, int height) {
        setText(text);
        boundle[LEFT] = left;
        boundle[TOP] = top;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;
    }

    public void setText(String text) {
        this.text = text;
        text_arr = toUtf8(text);
    }

    public void setIcon(char icon) {
        preicon = icon;
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

        //NVG_NOTUSED(w);
        nvgFontSize(vg, textFontSize);
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 128));

        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgText(vg, x, y + h * 0.5f, text_arr, null);
        return true;
    }

}
