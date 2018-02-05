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
import static org.mini.glfw.utils.Nutil.nvgBeginPath;
import static org.mini.glfw.utils.Nutil.nvgFill;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFillPaint;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgRoundedRect;
import static org.mini.glfw.utils.Nutil.nvgStroke;
import static org.mini.glfw.utils.Nutil.nvgStrokeColor;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTextJni;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GButton extends GObject {

    String text;
    byte[] text_arr;
    char preicon;
    byte[] preicon_arr;
    boolean bt_pressed = false;

    public GButton(String text, int left, int top, int width, int height) {
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
        preicon_arr = toUtf8("" + preicon);
    }

    @Override
    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {
        if (isInBoundle(boundle, x - parent.getX(), y - parent.getY())) {
            if (pressed) {
                bt_pressed = true;
                parent.setFocus(this);
            } else {
                bt_pressed = false;
                if (actionListener != null) {
                    actionListener.action();
                }
            }
        }
    }

    @Override
    public void cursorPosEvent(int x, int y) {
        if (!isInBoundle(boundle, x - parent.getX(), y - parent.getY())) {
            bt_pressed = false;
        }
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

        byte[] bg;

        float cornerRadius = 4.0f;
        float tw = 0, iw = 0;
        float move = 0;
        if (bt_pressed) {
            move = 1;
            bg = Nutil.nvgLinearGradient(vg, x, y + h, x, y, nvgRGBA(255, 255, 255, isBlack(bgColor) ? 16 : 32), nvgRGBA(0, 0, 0, isBlack(bgColor) ? 16 : 32));
        } else {
            bg = Nutil.nvgLinearGradient(vg, x, y, x, y + h, nvgRGBA(255, 255, 255, isBlack(bgColor) ? 16 : 32), nvgRGBA(0, 0, 0, isBlack(bgColor) ? 16 : 32));
        }
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, cornerRadius - 1);
        if (!isBlack(bgColor)) {
            nvgFillColor(vg, bgColor);
            nvgFill(vg);
        }
        nvgFillPaint(vg, bg);
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, cornerRadius - 0.5f);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
        nvgStroke(vg);

        nvgFontSize(vg, 20.0f);
        nvgFontFace(vg, GToolkit.getFontWord());
        tw = Nutil.nvgTextBoundsJni(vg, 0, 0, text_arr, 0, text_arr.length, null);
        if (preicon != 0) {
            nvgFontSize(vg, h * 1.3f);
            nvgFontFace(vg, GToolkit.getFontIcon());

            iw = Nutil.nvgTextBoundsJni(vg, 0, 0, preicon_arr, 0, preicon_arr.length, null);
            iw += h * 0.15f;
        }

        if (preicon != 0) {
            nvgFontSize(vg, h * 1.3f);
            nvgFontFace(vg, GToolkit.getFontIcon());
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 96));
            nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            Nutil.nvgTextJni(vg, x + w * 0.5f - tw * 0.5f - iw * 0.75f, y + h * 0.5f + move, preicon_arr, 0, preicon_arr.length);
        }

        nvgFontSize(vg, textFontSize);
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 160));
        nvgTextJni(vg, x + w * 0.5f - tw * 0.5f + iw * 0.25f, y + h * 0.5f + 1 + move, text_arr, 0, text_arr.length);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));
        nvgTextJni(vg, x + w * 0.5f - tw * 0.5f + iw * 0.25f, y + h * 0.5f + move, text_arr, 0, text_arr.length);
        return true;
    }
// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise

    boolean isBlack(float[] col) {
        int r = 0, g = 1, b = 2, a = 3;
        if (col[r] == 0.0f && col[g] == 0.0f && col[b] == 0.0f && col[a] == 0.0f) {
            return true;
        }
        return false;
    }
}
