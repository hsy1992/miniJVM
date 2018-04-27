/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import com.sun.javafx.tk.Toolkit;
import org.mini.glfm.Glfm;
import static org.mini.gui.GTextBox.AREA_DETAIL_ADD;
import static org.mini.nanovg.Gutil.toUtf8;
import static org.mini.gui.GToolkit.nvgRGBA;
import org.mini.nanovg.Nanovg;
import static org.mini.nanovg.Nanovg.NVG_ALIGN_CENTER;
import static org.mini.nanovg.Nanovg.NVG_ALIGN_LEFT;
import static org.mini.nanovg.Nanovg.NVG_ALIGN_MIDDLE;
import static org.mini.nanovg.Nanovg.nvgBeginPath;
import static org.mini.nanovg.Nanovg.nvgBoxGradient;
import static org.mini.nanovg.Nanovg.nvgCreateNVGglyphPosition;
import static org.mini.nanovg.Nanovg.nvgFill;
import static org.mini.nanovg.Nanovg.nvgFillColor;
import static org.mini.nanovg.Nanovg.nvgFillPaint;
import static org.mini.nanovg.Nanovg.nvgFontFace;
import static org.mini.nanovg.Nanovg.nvgFontSize;
import static org.mini.nanovg.Nanovg.nvgNVGglyphPosition_x;
import static org.mini.nanovg.Nanovg.nvgRoundedRect;
import static org.mini.nanovg.Nanovg.nvgTextAlign;
import static org.mini.nanovg.Nanovg.nvgTextGlyphPositionsJni;
import static org.mini.nanovg.Nanovg.nvgTextJni;
import static org.mini.nanovg.Nanovg.nvgTextMetrics;

/**
 *
 * @author gust
 */
public class GInputField extends GObject {

    static public final int BOX_STYLE_EDIT = 0;
    static public final int BOX_STYLE_SEARCH = 1;

    String hint;
    byte[] hint_arr;
    StringBuilder textsb = new StringBuilder();
    byte[] text_arr;
    float[] reset_boundle;
    int text_max = 256;
    int boxStyle = BOX_STYLE_EDIT;
    //
    byte[] search_arr = {(byte) 0xe2, (byte) 0x8c, (byte) 0xa8, 0};
    byte[] reset_arr = toUtf8("" + ICON_CIRCLED_CROSS);
    //
    float[] lineh = {0};
    short[] text_pos;
    //
    int caretIndex;
    int selectStart = -1;//选取开始
    int selectEnd = -1;//选取结束

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

    public void setBoxStyle(int boxStyle) {
        this.boxStyle = boxStyle;
    }

    public void setMaxTextLength(int len) {
        text_max = len;
    }

    public void setText(String text) {
        this.textsb.append(text);
        text_arr = toUtf8(text);
    }

    public String getText() {
        return textsb.toString();
    }

    @Override
    public void touchEvent(int phase, int x, int y) {
        int rx = (int) (x - parent.getX());
        int ry = (int) (y - parent.getY());
        if (isInBoundle(boundle, rx, ry)) {
            if (phase == Glfm.GLFMTouchPhaseEnded) {
                if (isInBoundle(reset_boundle, rx, ry)) {
                    textsb.setLength(0);
                } else {
                    setCaretIndex(getCaretIndex(x, y));
                }
            } else if (actionListener != null) {
                actionListener.action(this);
            }
        }
    }

    public int getCaretIndex(int x, int y) {
        if (text_pos == null) {
            return textsb.length();
        }
        for (int j = 0; j < text_pos.length; j++) {
            //取第 j 个字符的X座标
            float x0 = text_pos[j];
            if (x < x0) {
                return j;
            }
        }
        return text_pos.length;
    }

    @Override
    public void onFocus() {
        Glfm.glfmSetKeyboardVisible(getForm().getWinContext(), true);
    }

    @Override
    public void onUnFocus() {
        Glfm.glfmSetKeyboardVisible(getForm().getWinContext(), false);
    }

    /**
     *
     * @param character
     */
    @Override
    public void characterEvent(String str, int mods) {
        if (parent.getFocus() != this) {
            return;
        }

        for (int i = 0, imax = str.length(); i < imax; i++) {
            char character = str.charAt(i);
            if (character != '\n' && character != '\r' && textsb.length() < text_max) {
                textsb.insert(caretIndex, character);
                setCaretIndex(caretIndex + 1);
            }
        }
    }

    @Override
    public void keyEvent(int key, int action, int mods) {
        if (parent.getFocus() != this) {
            return;
        }
        if (action == Glfm.GLFMKeyActionPressed || action == Glfm.GLFMKeyActionRepeated) {
            if (key == Glfm.GLFMKeyBackspace) {
                if (textsb.length() > 0 && caretIndex > 0) {
                    int[] selectFromTo = getSelected();
                    if (selectFromTo != null) {
                        delectSelect();
                    } else {
                        textsb.delete(caretIndex - 1, caretIndex);
                        setCaretIndex(caretIndex - 1);
                        text_arr = null;
                    }
                }
            }
        }
    }

    /**
     * @param caretIndex the caretIndex to set
     */
    private void setCaretIndex(int caretIndex) {
        if (caretIndex < 0) {
            caretIndex = 0;
        } else if (caretIndex > textsb.length()) {
            caretIndex = textsb.length();
        }
        this.caretIndex = caretIndex;
    }

    int[] getSelected() {
        int select1 = 0, select2 = 0;
        if (selectStart != -1 && selectEnd != -1) {
            select1 = selectStart > selectEnd ? selectEnd : selectStart;
            select2 = selectStart < selectEnd ? selectEnd : selectStart;
            return new int[]{select1, select2};
        }
        return null;
    }

    void delectSelect() {
        int[] sarr = getSelected();
        setCaretIndex(sarr[0]);
        textsb.delete(sarr[0], sarr[1]);
        text_arr = null;
        resetSelect();
    }

    void resetSelect() {
        selectStart = selectEnd = -1;
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
        
        Nanovg.nvgScissor(vg, x, y, w, h);

        byte[] bg;
        float FONT_WIDTH = GToolkit.getStyle().getIconFontWidth();
        float leftIcons = 0.5f;//图标占位宽度
        // Edit
        if (boxStyle == BOX_STYLE_SEARCH) {
            GToolkit.getStyle().drawFieldBoxBase(vg, x, y, w, h);
            nvgFontSize(vg, GToolkit.getStyle().getIconFontSize());
            nvgFontFace(vg, GToolkit.getFontIcon());
            nvgFillColor(vg, GToolkit.getStyle().getHintFontColor());
            nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

            nvgTextJni(vg, x + FONT_WIDTH * 1.5f, y + h * 0.55f, search_arr, 0, search_arr.length);
            leftIcons = 2;
        } else {
            GToolkit.getStyle().drawEditBoxBase(vg, x, y, w, h);
        }

        nvgFontSize(vg, GToolkit.getStyle().getTextFontSize());
        nvgFontFace(vg, GToolkit.getFontWord());

        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

        text_arr = toUtf8(textsb.toString());
        float wordx = x + FONT_WIDTH * leftIcons;
        float wordy = y + boundle[HEIGHT] * 0.5f;
        float text_show_area_x = wordx;
        float text_show_area_w = boundle[WIDTH] - FONT_WIDTH * (leftIcons + 2.5f);
        float text_width = Nanovg.nvgTextBoundsJni(vg, 0, 0, text_arr, 0, text_arr.length, null);
        if (parent.getFocus() != this && (textsb == null || textsb.length() == 0)) {
            if (hint_arr != null) {
                nvgFillColor(vg, GToolkit.getStyle().getHintFontColor());
                nvgTextJni(vg, wordx, wordy, hint_arr, 0, hint_arr.length);
            }
        } else {

            long glyphsHandle = nvgCreateNVGglyphPosition(text_max);
            if (parent.getFocus() == this) {
                nvgTextMetrics(vg, null, null, lineh);
            }

            try {
                if (text_width > text_show_area_w) {
                    wordx -= text_width - text_show_area_w;
                }
                int char_count = nvgTextGlyphPositionsJni(vg, wordx, wordy, text_arr, 0, text_arr.length, glyphsHandle, text_max);

                text_pos = new short[char_count];
                float caretx = 0;
                //确定每个char的位置
                for (int j = 0; j < char_count; j++) {
                    //取第 j 个字符的X座标
                    float x0 = nvgNVGglyphPosition_x(glyphsHandle, j);
                    text_pos[j] = (short) x0;
                    if (caretIndex == j) {
                        caretx = x0;
                    }
                }
                if (caretx == 0) {
                    caretx = wordx + text_width;
                }
                if (parent.getFocus() == this) {
                    GToolkit.drawCaret(vg, caretx, wordy - 0.5f * lineh[0], 1, lineh[0], false);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            Nanovg.nvgDeleteNVGglyphPosition(glyphsHandle);

//            float caret_x = wordx;
//            text_arr = toUtf8(textsb.toString());
//            float text_width = Nanovg.nvgTextBoundsJni(vg, 0, 0, text_arr, 0, text_arr.length, null);
//            float text_show_area_w = w - standard[WIDTH] * 4.5f;
//            float text_show_area_x = wordx;
//            if (text_width > txt_show_area_w) {
//                caret_x = wordx + txt_show_area_w;
//                wordx -= text_width - txt_show_area_w;
//            } else {
//                caret_x = wordx + text_width;
//            }
            nvgFillColor(vg, GToolkit.getStyle().getTextFontColor());
            Nanovg.nvgScissor(vg, text_show_area_x, y, text_show_area_w, h);
            nvgTextJni(vg, wordx, wordy, text_arr, 0, text_arr.length);
            Nanovg.nvgResetScissor(vg);
        }
        nvgFontSize(vg, GToolkit.getStyle().getIconFontSize());
        nvgFontFace(vg, GToolkit.getFontIcon());
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 32));
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgTextJni(vg, x + w - h * 0.55f, y + h * 0.55f, reset_arr, 0, reset_arr.length);
        return true;
    }

}
