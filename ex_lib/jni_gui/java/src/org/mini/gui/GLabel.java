/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_LEFT;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_MIDDLE;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_TOP;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTextJni;
import static org.mini.glfw.utils.Nutil.nvgTextMetrics;

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

    public final void setText(String text) {
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

        drawText(vg, x, y, w, h);
        return true;
    }

    void drawLabel(long vg, float x, float y, float w, float h) {
        //NVG_NOTUSED(w);
        nvgFontSize(vg, GToolkit.getStyle().getTextFontSize());
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgFillColor(vg, GToolkit.getStyle().getTextFontColor());

        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgTextJni(vg, x, y + h * 0.5f, text_arr, 0, text_arr.length);

    }

    void drawText(long vg, float x, float y, float w, float h) {

        nvgFontSize(vg, GToolkit.getStyle().getTextFontSize());
        nvgFillColor(vg, GToolkit.getStyle().getTextFontColor());
        nvgFontFace(vg, GToolkit.getFontWord());

        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        float[] text_area = new float[]{x + 2f, y + 2f, w - 4f, h - 4f};
        float dx = text_area[LEFT];
        float dy = text_area[TOP];

        float[] bond = new float[4];
        Nutil.nvgTextBoxBoundsJni(vg, text_area[LEFT], text_area[TOP], text_area[WIDTH], text_arr, 0, text_arr.length, bond);
        bond[WIDTH] -= bond[LEFT];
        bond[HEIGHT] -= bond[TOP];
        bond[LEFT] = bond[TOP] = 0;

        if (bond[HEIGHT] > text_area[HEIGHT]) {
            dy -= bond[HEIGHT] - text_area[HEIGHT];
        }
        Nutil.nvgScissor(vg, x, y, w, h);
        Nutil.nvgTextBoxJni(vg, dx, dy, text_area[WIDTH], text_arr, 0, text_arr.length);
        Nutil.nvgResetScissor(vg);
    }

}
