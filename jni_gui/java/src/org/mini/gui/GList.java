/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_CENTER;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_LEFT;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_MIDDLE;
import static org.mini.glfw.utils.Nutil.NVG_CCW;
import static org.mini.glfw.utils.Nutil.NVG_CW;
import static org.mini.glfw.utils.Nutil.NVG_HOLE;
import static org.mini.glfw.utils.Nutil.nvgArc;
import static org.mini.glfw.utils.Nutil.nvgBeginPath;
import static org.mini.glfw.utils.Nutil.nvgBoxGradient;
import static org.mini.glfw.utils.Nutil.nvgClosePath;
import static org.mini.glfw.utils.Nutil.nvgFill;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFillPaint;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgImagePattern;
import static org.mini.glfw.utils.Nutil.nvgImageSize;
import static org.mini.glfw.utils.Nutil.nvgLineTo;
import static org.mini.glfw.utils.Nutil.nvgLinearGradient;
import static org.mini.glfw.utils.Nutil.nvgMoveTo;
import static org.mini.glfw.utils.Nutil.nvgPathWinding;
import static org.mini.glfw.utils.Nutil.nvgRect;
import static org.mini.glfw.utils.Nutil.nvgRestore;
import static org.mini.glfw.utils.Nutil.nvgRoundedRect;
import static org.mini.glfw.utils.Nutil.nvgSave;
import static org.mini.glfw.utils.Nutil.nvgScissor;
import static org.mini.glfw.utils.Nutil.nvgStroke;
import static org.mini.glfw.utils.Nutil.nvgStrokeColor;
import static org.mini.glfw.utils.Nutil.nvgStrokeWidth;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTextJni;
import static org.mini.glfw.utils.Nutil.nvgTranslate;
import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GList extends GObject {

    String text;
    byte[] text_arr;
    char preicon;
    byte[] preicon_arr = toUtf8("" + ICON_CHEVRON_RIGHT);

    public GList(String text, int left, int top, int width, int height) {
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

        drawDropDown(vg, x, y, w, h);
        int[] images = {0, 0, 0, 0, 0, 0};
        drawThumbnails(vg, 365, y - 30, 160, 300, images, 0.5f);
        return true;
    }

    void drawDropDown(long vg, float x, float y, float w, float h) {
        byte[] bg;

        float cornerRadius = 4.0f;

        bg = nvgLinearGradient(vg, x, y, x, y + h, nvgRGBA(255, 255, 255, 16), nvgRGBA(0, 0, 0, 16));
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + 1, y + 1, w - 2, h - 2, cornerRadius - 1);
        nvgFillPaint(vg, bg);
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + 0.5f, y + 0.5f, w - 1, h - 1, cornerRadius - 0.5f);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 48));
        nvgStroke(vg);

        nvgFontSize(vg, 20.0f);
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 160));
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        byte[] b1 = toUtf8(text);
        nvgTextJni(vg, x + h * 0.3f, y + h * 0.5f, b1, 0, b1.length);

        nvgFontSize(vg, h * 1.3f);
        nvgFontFace(vg, GToolkit.getFontIcon());
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgTextJni(vg, x + w - h * 0.5f, y + h * 0.5f, preicon_arr, 0, preicon_arr.length);
    }

    void drawThumbnails(long vg, float x, float y, float w, float h, int[] images, float t) {
        int nimages = images.length;
        float cornerRadius = 3.0f;
        byte[] shadowPaint, imgPaint, fadePaint;
        float ix, iy, iw, ih;
        float thumb = 60.0f;
        float arry = 30.5f;
        int[] imgw = {0}, imgh = {0};
        float stackh = (nimages / 2) * (thumb + 10) + 10;
        int i;
        float u = (1 + (float) Math.cos(t * 0.5f)) * 0.5f;
        float u2 = (1 - (float) Math.cos(t * 0.2f)) * 0.5f;
        float scrollh, dv;

        nvgSave(vg);
//	nvgClearState(vg);

        // Drop shadow
        shadowPaint = Nutil.nvgBoxGradient(vg, x, y + 4, w, h, cornerRadius * 2, 20, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
        nvgBeginPath(vg);
        nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgPathWinding(vg, NVG_HOLE);
        nvgFillPaint(vg, shadowPaint);
        nvgFill(vg);

        // Window
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgMoveTo(vg, x - 10, y + arry);
        nvgLineTo(vg, x + 1, y + arry - 11);
        nvgLineTo(vg, x + 1, y + arry + 11);
        nvgFillColor(vg, nvgRGBA(200, 200, 200, 255));
        nvgFill(vg);

        nvgSave(vg);
        nvgScissor(vg, x, y, w, h);
        nvgTranslate(vg, 0, -(stackh - h) * u);

        dv = 1.0f / (float) (nimages - 1);

        for (i = 0; i < nimages; i++) {
            float tx, ty, v, a;
            tx = x + 10;
            ty = y + 10;
            tx += (i % 2) * (thumb + 10);
            ty += (i / 2) * (thumb + 10);
            nvgImageSize(vg, images[i], imgw, imgh);
            if (imgw[0] < imgh[0]) {
                iw = thumb;
                ih = iw * (float) imgh[0] / (float) imgw[0];
                ix = 0;
                iy = -(ih - thumb) * 0.5f;
            } else {
                ih = thumb;
                iw = ih * (float) imgw[0] / (float) imgh[0];
                ix = -(iw - thumb) * 0.5f;
                iy = 0;
            }

            v = i * dv;
            a = clampf((u2 - v) / dv, 0, 1);

            if (a < 1.0f) {
                drawSpinner(vg, tx + thumb / 2, ty + thumb / 2, thumb * 0.25f, t);
            }

            imgPaint = nvgImagePattern(vg, tx + ix, ty + iy, iw, ih, 0.0f / 180.0f * (float) Math.PI, images[i], a);
            nvgBeginPath(vg);
            nvgRoundedRect(vg, tx, ty, thumb, thumb, 5);
            nvgFillPaint(vg, imgPaint);
            nvgFill(vg);

            shadowPaint = nvgBoxGradient(vg, tx - 1, ty, thumb + 2, thumb + 2, 5, 3, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
            nvgBeginPath(vg);
            nvgRect(vg, tx - 5, ty - 5, thumb + 10, thumb + 10);
            nvgRoundedRect(vg, tx, ty, thumb, thumb, 6);
            nvgPathWinding(vg, NVG_HOLE);
            nvgFillPaint(vg, shadowPaint);
            nvgFill(vg);

            nvgBeginPath(vg);
            nvgRoundedRect(vg, tx + 0.5f, ty + 0.5f, thumb - 1, thumb - 1, 4 - 0.5f);
            nvgStrokeWidth(vg, 1.0f);
            nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
            nvgStroke(vg);
        }
        nvgRestore(vg);

        // Hide fades
        fadePaint = nvgLinearGradient(vg, x, y, x, y + 6, nvgRGBA(200, 200, 200, 255), nvgRGBA(200, 200, 200, 0));
        nvgBeginPath(vg);
        nvgRect(vg, x + 4, y, w - 8, 6);
        nvgFillPaint(vg, fadePaint);
        nvgFill(vg);

        fadePaint = nvgLinearGradient(vg, x, y + h, x, y + h - 6, nvgRGBA(200, 200, 200, 255), nvgRGBA(200, 200, 200, 0));
        nvgBeginPath(vg);
        nvgRect(vg, x + 4, y + h - 6, w - 8, 6);
        nvgFillPaint(vg, fadePaint);
        nvgFill(vg);

        // Scroll bar
        shadowPaint = nvgBoxGradient(vg, x + w - 12 + 1, y + 4 + 1, 8, h - 8, 3, 4, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 92));
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + w - 12, y + 4, 8, h - 8, 3);
        nvgFillPaint(vg, shadowPaint);
//	nvgFillColor(vg, nvgRGBA(255,0,0,128));
        nvgFill(vg);

        scrollh = (h / stackh) * (h - 8);
        if (scrollh > h - 8) {
            scrollh = h - 8;
        }
        shadowPaint = nvgBoxGradient(vg, x + w - 12 - 1, y + 4 + (h - 8 - scrollh) * u - 1, 8, scrollh, 3, 4, nvgRGBA(220, 220, 220, 255), nvgRGBA(128, 128, 128, 255));
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + w - 12 + 1, y + 4 + 1 + (h - 8 - scrollh) * u, 8 - 2, scrollh - 2, 2);
        nvgFillPaint(vg, shadowPaint);
//	nvgFillColor(vg, nvgRGBA(0,0,0,128));
        nvgFill(vg);

        nvgRestore(vg);
    }

    void drawSpinner(long vg, float cx, float cy, float r, float t) {
        float a0 = 0.0f + t * 6;
        float a1 = (float) Math.PI + t * 6;
        float r0 = r;
        float r1 = r * 0.75f;
        float ax, ay, bx, by;
        byte[] paint;

        nvgSave(vg);

        nvgBeginPath(vg);
        nvgArc(vg, cx, cy, r0, a0, a1, NVG_CW);
        nvgArc(vg, cx, cy, r1, a1, a0, NVG_CCW);
        nvgClosePath(vg);
        ax = cx + (float) Math.cos(a0) * (r0 + r1) * 0.5f;
        ay = cy + (float) Math.sin(a0) * (r0 + r1) * 0.5f;
        bx = cx + (float) Math.cos(a1) * (r0 + r1) * 0.5f;
        by = cy + (float) Math.sin(a1) * (r0 + r1) * 0.5f;
        paint = nvgLinearGradient(vg, ax, ay, bx, by, nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 0, 0, 128));
        nvgFillPaint(vg, paint);
        nvgFill(vg);

        nvgRestore(vg);
    }

    static float clampf(float a, float mn, float mx) {
        return a < mn ? mn : (a > mx ? mx : a);
    }

}
