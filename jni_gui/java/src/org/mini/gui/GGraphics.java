/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.utils.Gutil;
import org.mini.nk.NK;

/**
 *
 * @author gust
 */
public class GGraphics {

    long brush;
    long font;
    float[] frame_bound;

    int[] curColor = new int[1];
    float[] rect = new float[4];
    float[] clip = new float[4];
    int translateX, translateY;

    public void setColor(int abgr) {
        curColor[0] = abgr;
    }

    public void fillRect(int x, int y, int w, int h) {
        if (brush == 0) {
            return;
        }
        rect[0] = x + frame_bound[0] + translateX;
        rect[1] = y + frame_bound[1] + translateY;
        rect[2] = w;
        rect[3] = h;
        NK.nk_fill_rect(brush, rect, 1, curColor);
    }

    public void fillArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
        if (brush == 0) {
            return;
        }
        x += frame_bound[0] + translateX;
        y += frame_bound[1] + translateY;
        NK.nk_fill_arc(brush, x, y, width, (float) (startAngle * Math.PI / 180), (float) (arcAngle * Math.PI / 180), curColor);
    }

    int thickness = 1;

    public void drawArc(int x, int y, int width, int height, int startAngle, int arcAngle) {
        if (brush == 0) {
            return;
        }
        x += frame_bound[0] + translateX;
        y += frame_bound[1] + translateY;
        NK.nk_stroke_arc(brush, x, y, width, (float) (startAngle * Math.PI / 180), (float) (arcAngle * Math.PI / 180), thickness, curColor);
    }

    public void drawLine(int x1, int y1, int x2, int y2) {
        if (brush == 0) {
            return;
        }
        x1 += frame_bound[0] + translateX;;
        y1 += frame_bound[1] + translateY;;
        x2 += frame_bound[0] + translateX;;
        y2 += frame_bound[1] + translateY;;
        NK.nk_stroke_line(brush, x1, y1, x2, y2, thickness, curColor);
    }

    public void drawString(String str, int x, int y, int anchor) {
        if (brush == 0) {
            return;
        }
        rect[0] = x + frame_bound[0] + translateX;
        rect[1] = y + frame_bound[1] + translateY;
        rect[2] = 10000;
        rect[3] = 10000;
        byte[] b = Gutil.toUtf8(str + "\000");
        int blen = str.length();
        NK.nk_draw_text(brush, rect, b, blen, NK.nk_get_font_handle(font), new int[]{0x80808080}, curColor);
    }

    public void drawSubstring(String str, int offset, int len, int x, int y, int anchor) {
        if (brush == 0) {
            return;
        }
        x += frame_bound[0] + translateX;
        y += frame_bound[1] + translateY;
        str = str.substring(offset, len);
        byte[] b = Gutil.toUtf8(str + "\000");
        int blen = str.length();
        NK.nk_draw_text(brush, rect, b, blen, NK.nk_get_font_handle(font), new int[]{0xff808000}, curColor);
    }

    public void drawChar(char character, int x, int y, int anchor) {
        if (brush == 0) {
            return;
        }
        x += frame_bound[0] + translateX;
        y += frame_bound[1] + translateY;

    }

    public void drawChars(char[] data, int offset, int length, int x, int y, int anchor) {
        if (brush == 0) {
            return;
        }
        x += frame_bound[0] + translateX;
        y += frame_bound[1] + translateY;
    }

    public void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
        if (brush == 0) {
            return;
        }
        x1 += frame_bound[0] + translateX;
        y1 += frame_bound[1] + translateY;
        x2 += frame_bound[0] + translateX;
        y2 += frame_bound[1] + translateY;
        x3 += frame_bound[0] + translateX;
        y3 += frame_bound[1] + translateY;
    }

    public void drawImage(GImage img, int x, int y, int anchor) {
        drawImage(img, x, y, img.getWidth(), img.getHeight(), anchor);
    }

    public void drawImage(GImage img, int x, int y, int w, int h, int anchor) {
        if (brush == 0) {
            return;
        }
        rect[0] = x + frame_bound[0] + translateX;
        rect[1] = y + frame_bound[1] + translateY;
        rect[2] = w;
        rect[3] = h;
        //System.out.println("w=" + w + "  ,h=" + h);
        int color = curColor[0];
        curColor[0] = 0xffffffff;
        NK.nk_draw_image(brush, rect, img.getDataPtr(), curColor);
        curColor[0] = color;
    }

    public void drawRegion(GImage src, int x_src, int y_src, int width, int height, int transform, int x_dest, int y_dest, int anchor) {
        if (brush == 0) {
            return;
        }
        x_src += frame_bound[0] + translateX;
        y_src += frame_bound[1] + translateY;

        x_dest += frame_bound[0] + translateX;
        y_dest += frame_bound[1] + translateY;
    }

    public void drawRGB(int[] rgbData, int offset, int scanlength, int x, int y, int width, int height, boolean processAlpha) {
        if (brush == 0) {
            return;
        }
        x += frame_bound[0] + translateX;
        y += frame_bound[1] + translateY;
    }

    public int getColor() {
        return curColor[0];
    }

    public void copyArea(int x_src, int y_src, int width, int height, int x_dest, int y_dest, int anchor) {
        if (brush == 0) {
            return;
        }
        x_src += frame_bound[0] + translateX;
        y_src += frame_bound[1] + translateY;

        x_dest += frame_bound[0] + translateX;
        y_dest += frame_bound[1] + translateY;
    }

    public void clipRect(int x, int y, int width, int height) {
        if (brush == 0) {
            return;
        }
        x += frame_bound[0] + translateX;
        y += frame_bound[1] + translateY;
    }

    public void translate(int x, int y) {
        translateX = x;
        translateY = y;
    }

}
