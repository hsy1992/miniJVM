/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.Glfw;
import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
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
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTextJni;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GInputField extends GObject {

    String hint;
    byte[] hint_arr;
    String text;
    byte[] text_arr;
    float[] reset_boundle;
    //
    byte[] search_arr = toUtf8("" + ICON_SEARCH);
    byte[] reset_arr = toUtf8("" + ICON_CIRCLED_CROSS);

    public GInputField(String text, String hint, int left, int top, int width, int height) {
        setHint(hint);
        boundle[LEFT] = left;
        boundle[TOP] = top;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;
        reset_boundle = new float[]{left + width - height, top, height, height};
    }

    public void setHint(String hint) {
        this.hint = hint;
        hint_arr = toUtf8(hint);
    }

    public void setText(String text) {
        this.text = text;
        text_arr = toUtf8(text);
    }

    public String getText() {
        return text;
    }

    @Override
    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {
        int rx = (int) (x - parent.getX());
        int ry = (int) (y - parent.getY());
        if (isInBoundle(boundle, rx, ry)) {
            if (pressed) {
                parent.setFocus(this);
                if (isInBoundle(reset_boundle, rx, ry)) {
                    setText("");
                }
            } else {
                if (actionListener != null) {
                    actionListener.action();
                }
            }
        }
    }

    /**
     *
     * @param character
     */
    @Override
    public void characterEvent(char character) {
        if (parent.getFocus() != this) {
            return;
        }
        if (text == null) {
            text = "";
        }
        setText(text += character);
    }

    @Override
    public void keyEvent(int key, int scanCode, int action, int mods) {
        if (parent.getFocus() != this) {
            return;
        }
        if (action == Glfw.GLFW_PRESS || action == Glfw.GLFW_REPEAT) {
            if (key == Glfw.GLFW_KEY_BACKSPACE) {
                String txt = getText();
                if (txt != null && txt.length() > 0) {
                    txt = txt.substring(0, txt.length() - 1);
                    setText(txt);
                }
            }
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
        float cornerRadius = h / 2 - 1;

        // Edit
        bg = nvgBoxGradient(vg, x, y + 1.5f, w, h, h / 2, 5, nvgRGBA(0, 0, 0, 16), nvgRGBA(0, 0, 0, 92));
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgFillPaint(vg, bg);
        nvgFill(vg);

//        nvgBeginPath(vg);
//        nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, cornerRadius - 0.5f);
//        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
//        nvgStroke(vg);
        nvgFontSize(vg, h * 1.3f);
        nvgFontFace(vg, GToolkit.getFontIcon());
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgTextJni(vg, x + h * 0.55f, y + h * 0.55f, search_arr, 0, search_arr.length);

        nvgFontSize(vg, textFontSize);
        nvgFontFace(vg, GToolkit.getFontWord());

        float[] standard = GToolkit.getFontBoundle(vg);
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        float wordx = x + standard[WIDTH] * 2;
        float wordy = y + h * 0.5f;
        if (parent.getFocus() != this && (text == null || text.length() == 0)) {
            if (hint_arr != null) {
                nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
                nvgTextJni(vg, wordx, wordy, hint_arr, 0, hint_arr.length);
            }
        } else {
            float text_width = 0;
            float txt_show_area_x = 0;
            float txt_show_area_w = 0;
            if (text_arr != null) {
                text_width = Nutil.nvgTextBoundsJni(vg, 0, 0, text_arr, 0, text_arr.length, null);
                txt_show_area_w = w - standard[WIDTH] * 4.5f;
                txt_show_area_x = wordx;
                if (text_width > txt_show_area_w) {
                    wordx -= text_width - txt_show_area_w;
                }
                nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));
                Nutil.nvgScissor(vg, txt_show_area_x, y, txt_show_area_w, h);
                nvgTextJni(vg, wordx, wordy, text_arr, 0, text_arr.length);
                Nutil.nvgResetScissor(vg);
            }
            Nutil.nvgRect(vg, wordx + txt_show_area_w, wordy, 10, h);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 200));
        }
        nvgFontSize(vg, h * 1.3f);
        nvgFontFace(vg, GToolkit.getFontIcon());
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 32));
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgTextJni(vg, x + w - h * 0.55f, y + h * 0.55f, reset_arr, 0, reset_arr.length);
        return true;
    }

}
