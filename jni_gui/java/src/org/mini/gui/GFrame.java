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
import static org.mini.glfw.utils.Nutil.NVG_HOLE;
import static org.mini.glfw.utils.Nutil.nvgBeginPath;
import static org.mini.glfw.utils.Nutil.nvgBoxGradient;
import static org.mini.glfw.utils.Nutil.nvgFill;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFillPaint;
import static org.mini.glfw.utils.Nutil.nvgFontBlur;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgLineTo;
import static org.mini.glfw.utils.Nutil.nvgLinearGradient;
import static org.mini.glfw.utils.Nutil.nvgMoveTo;
import static org.mini.glfw.utils.Nutil.nvgPathWinding;
import static org.mini.glfw.utils.Nutil.nvgRect;
import static org.mini.glfw.utils.Nutil.nvgRestore;
import static org.mini.glfw.utils.Nutil.nvgRoundedRect;
import static org.mini.glfw.utils.Nutil.nvgSave;
import static org.mini.glfw.utils.Nutil.nvgStroke;
import static org.mini.glfw.utils.Nutil.nvgStrokeColor;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTextJni;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GFrame extends GContainer {

    String title;
    byte[] title_arr;

    byte[] close_arr = {(byte) 0xe2, (byte) 0x9d, (byte) 0x8e, 0};
    float[] close_boundle = new float[4];

    int background_rgba;
    
    GPanel panel =new GPanel();

    float[] title_boundle = new float[4];
    long vg;
    int frameMode;
    boolean closable;

    public GFrame(String title, int left, int top, int width, int height) {
        setTitle(title);
        boundle[LEFT] = left;
        boundle[TOP] = top;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;

        panel.boundle[LEFT] = 2;
        panel.boundle[TOP] = 32;
        panel.boundle[WIDTH] = width - 4;
        panel.boundle[HEIGHT] = height - 34;
        add(panel);
    }

    public void setTitle(String title) {
        title_arr = toUtf8(title);
        this.title = title;
    }

    public String getTitle() {
        return title;
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
    
    public GPanel getPanel(){
        return panel;
    }

    /**
     * @return the closable
     */
    public boolean isClosable() {
        return closable;
    }

    /**
     * @param closable the closable to set
     */
    public void setClosable(boolean closable) {
        this.closable = closable;
    }

    public GForm getForm() {
        return (GForm) parent;
    }

    @Override
    public boolean update(long vg) {
        Nutil.nvgResetScissor(vg);
        this.vg = vg;
        drawWindow(vg, title, boundle[0], boundle[1], boundle[2], boundle[3]);
        super.update(this.vg);
        return true;
    }

    void drawWindow(long vg, String title, float x, float y, float w, float h) {
        float cornerRadius = 3.0f;
        byte[] shadowPaint;
        byte[] headerPaint;

        nvgSave(vg);
//	nvgClearState(vg);

        // Window
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgFillColor(vg, GToolkit.getStyle().getFrameBackground());
        nvgFill(vg);

        // Drop shadow
        shadowPaint = nvgBoxGradient(vg, x, y + 2, w, h, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
        nvgBeginPath(vg);
        nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgPathWinding(vg, NVG_HOLE);
        nvgFillPaint(vg, shadowPaint);
        nvgFill(vg);

        // Header
        headerPaint = nvgLinearGradient(vg, x, y, x, y + 15, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16));
        nvgBeginPath(vg);
        title_boundle[LEFT] = x + 1;
        title_boundle[TOP] = y + 1;
        title_boundle[WIDTH] = w - 2;
        title_boundle[HEIGHT] = 30;
        nvgRoundedRect(vg,
                title_boundle[LEFT],
                title_boundle[TOP],
                title_boundle[WIDTH],
                title_boundle[HEIGHT],
                cornerRadius - 1);
        nvgFillPaint(vg, headerPaint);
        nvgFill(vg);
        nvgBeginPath(vg);
        nvgMoveTo(vg, x + 0.5f, y + 0.5f + 30);
        nvgLineTo(vg, x + 0.5f + w - 1, y + 0.5f + 30);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
        nvgStroke(vg);

        nvgFontSize(vg, GToolkit.getStyle().getTitleFontSize());
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(vg, 2);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
        nvgTextJni(vg, x + w / 2, y + 16 + 1, title_arr, 0, title_arr.length);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, GToolkit.getStyle().getFrameTitleColor());
        nvgTextJni(vg, x + w / 2, y + 16, title_arr, 0, title_arr.length);

        if (closable) {
            nvgFontSize(vg, GToolkit.getStyle().getIconFontSize());
            nvgFontFace(vg, GToolkit.getFontIcon());
            nvgFillColor(vg, nvgRGBA(192, 32, 32, 128));
            nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgTextJni(vg, x + 10, y + 16, close_arr, 0, close_arr.length);
        }
        close_boundle[LEFT] = x + 10;
        close_boundle[TOP] = y + 6;
        close_boundle[WIDTH] = 16;
        close_boundle[HEIGHT] = 16;


        nvgRestore(vg);
    }

    boolean mouseDrag1 = false;
    boolean mouseDrag2 = false;
    int mouseX, mouseY;
    boolean isMoveFrame;

    @Override
    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {

        switch (button) {
            case Glfw.GLFW_MOUSE_BUTTON_1: {//left
                if (pressed) {
                    mouseDrag1 = true;
                    if (isInBoundle(title_boundle, mouseX, mouseY)) {
                        isMoveFrame = true;
                    }
                    if (closable && isInBoundle(close_boundle, x, y)) {
                        parent.remove(this);
                    }
                } else {
                    mouseDrag1 = false;
                    isMoveFrame = false;
                }
                break;
            }
            case Glfw.GLFW_MOUSE_BUTTON_2: {//right
                if (pressed) {
                    mouseDrag2 = true;
                } else {
                    mouseDrag2 = false;
                }
                break;
            }
            case Glfw.GLFW_MOUSE_BUTTON_3: {//middle
                break;
            }
        }
        if (isInBoundle(boundle, x, y)) {
            super.mouseButtonEvent(button, pressed, x, y);
        } else {
            panel.setFocus(null);
        }
    }

    @Override
    public void cursorPosEvent(int x, int y) {
        if (isMoveFrame) {
            boundle[LEFT] += x - mouseX;
            boundle[TOP] += y - mouseY;
            if (boundle[TOP] < 0) {
                boundle[TOP] = 0;
            }
            GForm gform = getForm();
            if (boundle[TOP] + title_boundle[HEIGHT] > gform.boundle[TOP] + gform.boundle[HEIGHT]) {
                boundle[TOP] = gform.boundle[TOP] + gform.boundle[HEIGHT] - title_boundle[HEIGHT];
            }
        }
        mouseX = x;
        mouseY = y;
        super.cursorPosEvent(x, y);
    }

    @Override
    public void scrollEvent(double scrollX, double scrollY, int x, int y) {
        super.scrollEvent(scrollX, scrollY, x, y);
    }
}
