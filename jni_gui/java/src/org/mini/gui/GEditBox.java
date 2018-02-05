/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.Glfw;
import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
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
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTextJni;
import static org.mini.gui.GObject.isInBoundle;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GEditBox extends GObject {

    String hint;
    byte[] hint_arr;
    String text;
    byte[] text_arr;
    char preicon;
    boolean singleMode;//single line mode

    public GEditBox(String text, String hint, int left, int top, int width, int height) {
        setText(text);
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

    public void setText(String text) {
        this.text = text;
        text_arr = toUtf8(text);
    }

    public String getText() {
        return text;
    }

    public void setSingleMode(boolean single) {
        this.singleMode = single;
    }

    @Override
    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {
        int rx = (int) (x - parent.getX());
        int ry = (int) (y - parent.getY());
        if (isInBoundle(boundle, rx, ry)) {
            if (pressed) {
                parent.setFocus(this);

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
            switch (key) {
                case Glfw.GLFW_KEY_BACKSPACE: {
                    String txt = getText();
                    if (txt != null && txt.length() > 0) {
                        txt = txt.substring(0, txt.length() - 1);
                        setText(txt);
                    }
                    break;
                }
                case Glfw.GLFW_KEY_ENTER: {
                    String txt = getText();
                    if (txt != null && txt.length() > 0 && !singleMode) {
                        setText(txt + "\n");
                    }
                    break;
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

        drawEditBox(vg, x, y, w, h);
        return true;
    }

    void drawEditBox(long vg, float x, float y, float w, float h) {

        drawEditBoxBase(vg, x, y, w, h);

        nvgFontSize(vg, textFontSize);
        nvgFontFace(vg, GToolkit.getFontWord());

        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        float[] standard = GToolkit.getFontBoundle(vg);
        float[] text_area = new float[]{x + 5f, y + 5f, w - 10f, h - 10f};
        float dx = text_area[LEFT];
        float dy = text_area[TOP];

        if (singleMode) {
            dy += .5f * text_area[HEIGHT];
        } else {
            dy += .5f * standard[HEIGHT];
        }
        if ((getText() == null || getText().length() <= 0) && parent.getFocus() != this) {
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
            nvgTextJni(vg, dx, dy, hint_arr, 0, hint_arr.length);
        } else {
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));

            float[] bond = new float[4];
            Nutil.nvgTextBoxBoundsJni(vg, text_area[LEFT], text_area[TOP], text_area[WIDTH], text_arr, 0, text_arr.length, bond);
            bond[WIDTH] -= bond[LEFT];
            bond[HEIGHT] -= bond[TOP];
            bond[LEFT] = bond[TOP] = 0;

            if (bond[HEIGHT] > text_area[HEIGHT]) {
                dy -= bond[HEIGHT] - text_area[HEIGHT];
            }
            Nutil.nvgScissor(vg, text_area[LEFT], text_area[TOP], text_area[WIDTH], text_area[HEIGHT]);
            Nutil.nvgTextBoxJni(vg, dx, dy, text_area[WIDTH], text_arr, 0, text_arr.length);
            Nutil.nvgResetScissor(vg);
        }
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
//
//    class InnerTextSpiter {
//
//        float ROW_BOARDER = 5f;
//        //input
//        String src;
//        float[] showArea = new float[4];
//
//        //output
//        List<StringBuilder> list = new ArrayList();
//        int showRows;
//        float rowHeight;
//
//        /**
//         * 通过给定的文本和显示区域，计算可显示的字符串行
//         *
//         * @param vg
//         * @param txt
//         * @param showBoundle
//         */
//        public InnerTextSpiter(long vg, String txt, float[] showBoundle) {
//            src = txt;
//            System.arraycopy(showBoundle, 0, showArea, 0, showBoundle.length);
//            split(vg);
//        }
//
//        public int getTotalRows() {
//            return list.size();
//        }
//
//        public List<String> getRows(int showLineStart) {
//            List<String> rows = new ArrayList();
//            for (int i = 0; i < showRows; i++) {
//                if (i + showLineStart < list.size()) {
//                    rows.add(list.get(i + showLineStart).toString());
//                }
//            }
//            return rows;
//        }
//
//        public int getShowRows() {
//            return showRows;
//        }
//
//        public float getRowHeight() {
//            return rowHeight;
//        }
//
//        public void insert(int pos, String str) {
//            if (pos < 0 || pos > src.length() - 1 || src == null) {
//                return;
//            }
//            src = src.substring(0, pos) + str + src.substring(pos);
//        }
//
//        public void delete(int pos, String str) {
//            if (pos < 0 || pos > src.length() - 1 || src == null) {
//                return;
//            }
//            int idx = src.indexOf(str, pos);
//            if (idx == pos) {
//                src = src.substring(0, pos) + src.substring(pos + str.length());
//            }
//        }
//
//        final void split(long vg) {
//            int lineWidth = 0;
//            StringBuilder sb = new StringBuilder();
//            float[] bond = new float[4];
//            float standardHeigh = GToolkit.getFontBoundle(vg)[HEIGHT];
//            float standardWidth = GToolkit.getFontBoundle(vg)[WIDTH];
//            rowHeight = (standardHeigh + ROW_BOARDER);
//            showRows = (int) (showArea[HEIGHT] / rowHeight);
//            showRows++;
//            for (int i = 0, imax = src.length(); i < imax; i++) {
//                char c = src.charAt(i);
//                int pos = findNextPos(i + 1);
//                if (pos == 0) {
//                    Nutil.nvgTextBounds(vg, 0, 0, toUtf8("" + c), null, bond);//字宽
//                }
//                bond[WIDTH] -= bond[LEFT];
//                bond[HEIGHT] -= bond[TOP];
//                bond[LEFT] = bond[TOP] = 0;
//                float cWidth = bond[WIDTH];
//                if (lineWidth + cWidth > showArea[WIDTH] || c == '\r' || c == '\n') {
//                    list.add(sb);
//                    lineWidth = 0;
//                    sb = new StringBuilder();
//                }
//                sb.append(c);
//                lineWidth += cWidth;
//            }
//            if (sb.length() > 0) {
//                list.add(sb);
//            }
//        }
//
//        int findNextPos(int start) {
//            for (int i = start, imax = src.length(); i < imax; i++) {
//                char ch = src.charAt(i);
//                if ((ch > 128) || (ch == '\n') || (ch == ' ')) {
//                    return i;
//                }
//            }
//            return 0;
//        }
//    }
}
