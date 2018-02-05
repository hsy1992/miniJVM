/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.ArrayList;
import java.util.List;
import org.mini.glfw.Glfw;
import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_LEFT;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_MIDDLE;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_TOP;
import static org.mini.glfw.utils.Nutil.nvgBeginPath;
import static org.mini.glfw.utils.Nutil.nvgBoxGradient;
import static org.mini.glfw.utils.Nutil.nvgCreateNVGglyphPosition;
import static org.mini.glfw.utils.Nutil.nvgCreateNVGtextRow;
import static org.mini.glfw.utils.Nutil.nvgFill;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFillPaint;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgNVGglyphPosition_x;
import static org.mini.glfw.utils.Nutil.nvgRestore;
import static org.mini.glfw.utils.Nutil.nvgRoundedRect;
import static org.mini.glfw.utils.Nutil.nvgSave;
import static org.mini.glfw.utils.Nutil.nvgStroke;
import static org.mini.glfw.utils.Nutil.nvgStrokeColor;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTextBreakLinesJni;
import static org.mini.glfw.utils.Nutil.nvgTextGlyphPositionsJni;
import static org.mini.glfw.utils.Nutil.nvgTextJni;
import static org.mini.glfw.utils.Nutil.nvgTextMetrics;
import static org.mini.gui.GObject.isInBoundle;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GTextBox extends GObject {

    String hint;
    byte[] hint_arr;
    StringBuilder textsb = new StringBuilder();
    byte[] text_arr;
    char preicon;
    boolean singleMode;//single line mode
    //
    float[] lineh = {0};
    int caretIndex;
    short[][] area_detail;
    static final int AREA_DETAIL_ADD = 6;
    static final int AREA_INDEX_START = 4;
    static final int AREA_INDEX_END = 5;
    static final int AREA_X = LEFT;
    static final int AREA_Y = TOP;
    static final int AREA_W = WIDTH;
    static final int AREA_H = HEIGHT;

    public GTextBox(String text, String hint, int left, int top, int width, int height) {
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
        this.textsb.setLength(0);
        this.textsb.append(text);
        text_arr = toUtf8(text);
    }

    public String getText() {
        return textsb.toString();
    }

    public void setSingleMode(boolean single) {
        this.singleMode = single;
    }

    boolean isInArea(short[] bound, float x, float y) {
        return x >= bound[LEFT] && x <= bound[LEFT] + bound[WIDTH]
                && y >= bound[TOP] && y <= bound[TOP] + bound[HEIGHT];
    }

    int getIndexOfString(int x, int y) {
        if (area_detail != null) {
            for (short[] detail : area_detail) {
                if (detail != null) {
                    if (isInArea(detail, x, y)) {
                        for (int i = AREA_DETAIL_ADD, imax = detail.length; i < imax; i++) {
                            int x0 = detail[i];
                            int x1 = (i + 1 < imax) ? detail[i + 1] : detail[AREA_X] + detail[AREA_W];
                            if (x >= x0 && x < x1) {
                                return detail[AREA_INDEX_START] + i - AREA_DETAIL_ADD;
                            }
                        }
                    }
                }
            }
        }
        return -1;
    }

    int[] getCaretPos() {
        if (area_detail != null) {
            for (short[] detail : area_detail) {
                if (detail != null) {
                    if (caretIndex >= detail[AREA_INDEX_START] && caretIndex < detail[AREA_INDEX_END]) {
                        int idx = caretIndex - detail[AREA_INDEX_START];
                        int x = detail[idx];
                        //int x1 = idx < detail[AREA_INDEX_END] ? detail[idx + 1] : detail[AREA_X] + +detail[AREA_W];
                        return new int[]{x, detail[AREA_Y]};
                    }
                }
            }
        }
        return null;
    }

    @Override
    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {
        int rx = (int) (x - parent.getX());
        int ry = (int) (y - parent.getY());
        if (isInBoundle(boundle, rx, ry)) {
            if (pressed) {
                parent.setFocus(this);
                int caret = getIndexOfString(x, y);
                if (caret >= 0) {
                    caretIndex = caret;
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
        textsb.insert(caretIndex, character);
        caretIndex++;
        text_arr = null;
    }

    @Override
    public void keyEvent(int key, int scanCode, int action, int mods) {
        if (parent.getFocus() != this) {
            return;
        }
        if (action == Glfw.GLFW_PRESS || action == Glfw.GLFW_REPEAT) {
            switch (key) {
                case Glfw.GLFW_KEY_BACKSPACE: {
                    if (textsb.length() > 0 && caretIndex > 0) {
                        textsb.delete(caretIndex - 1, caretIndex);
                        caretIndex--;
                        text_arr = null;
                    }
                    break;
                }
                case Glfw.GLFW_KEY_DELETE: {
                    if (textsb.length() > caretIndex) {
                        textsb.delete(caretIndex, caretIndex + 1);
                        text_arr = null;
                    }
                    break;
                }
                case Glfw.GLFW_KEY_ENTER: {
                    String txt = getText();
                    if (txt != null && txt.length() > 0 && !singleMode) {
                        textsb.insert(caretIndex++, "\n");
                        text_arr = null;
                    }
                    break;
                }
                case Glfw.GLFW_KEY_LEFT: {
                    if (textsb.length() > 0 && caretIndex > 0) {
                        caretIndex--;
                    }
                    break;
                }
                case Glfw.GLFW_KEY_RIGHT: {
                    if (textsb.length() > caretIndex) {
                        caretIndex++;
                    }
                    break;
                }
                case Glfw.GLFW_KEY_UP: {
                    int[] pos = getCaretPos();
                    if (pos != null) {
                        int cart = getIndexOfString(pos[0], pos[1] - (int) lineh[0]);
                        if (cart >= 0) {
                            caretIndex = cart;
                        }

                    }
                    break;
                }
                case Glfw.GLFW_KEY_DOWN: {
                    int[] pos = getCaretPos();
                    if (pos != null) {
                        int cart = getIndexOfString(pos[0], pos[1] + (int) lineh[0]);
                        if (cart >= 0) {
                            caretIndex = cart;
                        }

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

        drawTextBox(vg, x, y, w, h);
        return true;
    }

    void drawTextBox(long vg, float x, float y, float w, float h) {
        drawTextBoxBase(vg, x, y, w, h);
        nvgFontSize(vg, textFontSize);
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);

        nvgTextMetrics(vg, null, null, lineh);
        float lineH = lineh[0];

        float[] text_area = new float[]{x + 5f, y + 5f, w - 10f, h - 10f};
        float dx = text_area[LEFT];
        float dy = text_area[TOP];

        if (singleMode) {
            dy += .5f * (text_area[HEIGHT] - lineH);
        } else {
            dy += .2f * lineH;
        }
        if ((getText() == null || getText().length() <= 0) && parent.getFocus() != this) {
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
            nvgTextJni(vg, dx, dy, hint_arr, 0, hint_arr.length);
        } else {//编辑中
            if (text_arr == null) {
                text_arr = toUtf8(textsb.toString());
            }
            int showRows = Math.round(text_area[HEIGHT] / lineH);
            area_detail = new short[showRows][];
            float[] bond = new float[4];
            Nutil.nvgTextBoxBoundsJni(vg, dx, dy, text_area[WIDTH], text_arr, 0, text_arr.length, bond);
            int totalRows = Math.round((bond[HEIGHT] - bond[TOP]) / lineH);
            int posCount = 100;
            int rowCount = 10;
            long rows = nvgCreateNVGtextRow(rowCount);
            long glyphs = nvgCreateNVGglyphPosition(posCount);
            int nrows, i, nglyphs, j, lnum = 0;
            float caretx = 0, px;
            float a;
            float gx = 0, gy = 0;
            int gutter = 0;

            nvgSave(vg);

            // The text break API can be used to fill a large buffer of rows,
            // or to iterate over the text just few lines (or just one) at a time.
            // The "next" variable of the last returned item tells where to continue.
            long ptr = GToolkit.getArrayDataPtr(text_arr);
            int start = 0;
            int end = text_arr.length;
            while ((nrows = nvgTextBreakLinesJni(vg, text_arr, start, end, text_area[WIDTH], rows, rowCount)) != 0) {
                for (i = 0; i < nrows; i++) {
                    if (lnum >= totalRows - showRows) {
//                        boolean hit = mx > dx && mx < (dx + text_area[WIDTH]) && my >= dy && my < (dy + lineH);
                        float row_width = Nutil.nvgNVGtextRow_width(rows, i);
//                    nvgBeginPath(vg);
//                    nvgFillColor(vg, nvgRGBA(255, 255, 255, hit ? 64 : 16));
//                    nvgRect(vg, dx, dy, row_width, lineH);
//                    nvgFill(vg);

                        int starti = (int) (Nutil.nvgNVGtextRow_start(rows, i) - ptr);
                        int endi = (int) (Nutil.nvgNVGtextRow_end(rows, i) - ptr);

                        nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                        nvgTextJni(vg, dx, dy, text_arr, starti, endi);

//                        if (hit) {
//                            caretx = (mx < dx + row_width / 2) ? dx : dx + row_width;
                        caretx = dx;
                        px = dx;
                        nglyphs = nvgTextGlyphPositionsJni(vg, dx, dy, text_arr, starti, endi, glyphs, posCount);
                        int curRow = showRows - (totalRows - lnum);
                        if (area_detail.length < curRow) {
                            int debug = 1;
                        }
                        area_detail[curRow] = new short[AREA_DETAIL_ADD + nglyphs];
                        area_detail[curRow][AREA_X] = (short) dx;
                        area_detail[curRow][AREA_Y] = (short) dy;
                        area_detail[curRow][AREA_W] = (short) text_area[WIDTH];
                        area_detail[curRow][AREA_H] = (short) lineH;
                        area_detail[curRow][AREA_INDEX_START] = (short) starti;
                        area_detail[curRow][AREA_INDEX_END] = (short) endi;
                        for (j = 0; j < nglyphs; j++) {
                            float x0 = nvgNVGglyphPosition_x(glyphs, j);
                            area_detail[curRow][AREA_DETAIL_ADD + i] = (short) x0;
                            float x1 = (j + 1 < nglyphs) ? nvgNVGglyphPosition_x(glyphs, j + 1) : dx + row_width;
                            float gxx = x0 * 0.3f + x1 * 0.7f;
//                                if (mx >= px && mx < gxx) {
//                                    caretx = nvgNVGglyphPosition_x(glyphs, j);
//                                    caretIndex = (int) (starti + j);
//                                }
                            if (caretIndex == starti + j) {
                                caretx = x0;
                            }
                            px = gxx;
                        }

                        if (parent.getFocus() == this) {
                            GToolkit.drawCaret(vg, caretx, dy, 1, lineH);
                        }

                        gutter = lnum + 1;
                        gx = dx - 10;
                        gy = dy + lineH / 2;
//                        }
                        dy += lineH;
                    }
                    lnum++;
                }

                long next = Nutil.nvgNVGtextRow_next(rows, nrows - 1);
                start = (int) (next - ptr);
            }
            nvgRestore(vg);

            Nutil.nvgDeleteNVGtextRow(rows);
            Nutil.nvgDeleteNVGglyphPosition(glyphs);

        }
    }

    void drawTextBox1(long vg, float x, float y, float w, float h) {

        drawTextBoxBase(vg, x, y, w, h);

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
            if (parent.getFocus() == this) {
                nvgTextMetrics(vg, null, null, lineh);
                GToolkit.drawCaret(vg, dx, dy - .5f * lineh[0], 1, lineh[0]);
            }
        }
    }

    public static void drawTextBoxBase(long vg, float x, float y, float w, float h) {
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

//    static class InnerEvent {
//
//        static int MOUSE_CLICK = 0;
//        static int MOUSE_DRAG = 1;
//        static int DELETE_ = 2;
//        static int APPEND_ = 3;
//        int type;
//        int mousex, mousey;
//        String str;
//
//        InnerEvent(int type) {
//            this.type = type;
//        }
//
//        boolean hitMouseClick(float x, float y, float w, float h) {
//            return type == MOUSE_CLICK && mousex >= x && mousex <= x + w && mousey >= y && mousey <= y + h;
//        }
//    }
}
