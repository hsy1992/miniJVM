/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Hashtable;
import javax.mini.reflect.Array;
import javax.mini.reflect.vm.RefNative;
import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
import static org.mini.glfw.utils.Nutil.nvgAddFallbackFontId;
import static org.mini.glfw.utils.Nutil.nvgCreateFont;

/**
 *
 * @author gust
 */
public class GToolkit {

    static Hashtable<Long, GForm> table = new Hashtable();

    static public GForm getForm(long ctx) {
        return table.get(ctx);
    }

    static public GForm removeForm(long ctx) {
        return table.remove(ctx);
    }

    static public void putForm(long ctx, GForm win) {
        table.put(ctx, win);
    }

    /**
     *
     * 返回数组数据区首地址
     *
     * @param array
     * @return
     */
    public static long getArrayDataPtr(Object array) {
        Array reflect_arr = new Array(RefNative.obj2id(array));
        return reflect_arr.getDataPtr();
    }

    public static float[] nvgRGBA(int r, int g, int b, int a) {
        return Nutil.nvgRGBA((byte) r, (byte) g, (byte) b, (byte) a);
    }

    /**
     * 字体部分
     */
    static byte[] font_word = "word".getBytes(), font_icon = "icon".getBytes(), font_emoji = "emoji".getBytes();
    static int font_word_handle, font_icon_handle, font_emoji_handle;
    static boolean fontLoaded = false;
    static byte[] FONT_GLYPH_TEMPLATE = toUtf8("正");

    public static void loadFont(long vg) {
        if (fontLoaded) {
            return;
        }
        font_word_handle = nvgCreateFont(vg, font_word, toUtf8("../../binary/res/wqymhei.ttc\000"));
        if (font_word_handle == -1) {
            System.out.println("Could not add font.\n");
        }
        nvgAddFallbackFontId(vg, font_word_handle, font_word_handle);

        font_icon_handle = nvgCreateFont(vg, font_icon, toUtf8("../../binary/res/entypo.ttf\000"));
        if (font_icon_handle == -1) {
            System.out.println("Could not add font.\n");
        }
        font_emoji_handle = nvgCreateFont(vg, font_emoji, toUtf8("../../binary/res/NotoEmoji-Regular.ttf\000"));
        if (font_emoji_handle == -1) {
            System.out.println("Could not add font.\n");
        }
        fontLoaded = true;
    }

    public static byte[] getFontWord() {
        return font_word;
    }

    public static byte[] getFontIcon() {
        return font_icon;
    }

    public static byte[] getFontEmoji() {
        return font_emoji;
    }

    public static float[] getFontBoundle(long vg) {
        float[] bond = new float[4];
        Nutil.nvgTextBounds(vg, 0, 0, FONT_GLYPH_TEMPLATE, null, bond);
        bond[GObject.WIDTH] -= bond[GObject.LEFT];
        bond[GObject.HEIGHT] -= bond[GObject.TOP];
        bond[GObject.LEFT] = bond[GObject.TOP] = 0;
        return bond;
    }

    /**
     * 风格
     */
    static GStyle defaultStyle = new GDefaultStyle();

    public static GStyle getStyle() {
        return defaultStyle;
    }

    public static void setStyle(GStyle style) {
        defaultStyle = style;
    }
}
