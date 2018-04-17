/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.gui.GToolkit.nvgRGBA;
import org.mini.gui.event.GActionListener;

/**
 *
 * @author gust
 */
abstract public class GObject {

    public static char ICON_SEARCH = (char) 0x1F50D;
    public static char ICON_CIRCLED_CROSS = 0x2716;
    public static char ICON_CHEVRON_RIGHT = 0xE75E;
    public static char ICON_CHECK = 0x2713;
    public static char ICON_LOGIN = 0xE740;
    public static char ICON_TRASH = 0xE729;
    //
    GContainer parent;

    //object position and width ,height
    float[] boundle = new float[4];
    static final int LEFT = 0;
    static final int TOP = 1;
    static final int WIDTH = 2;
    static final int HEIGHT = 3;

    float[] bgColor;
    float[] color;

    GActionListener actionListener;

    static boolean flush;

    boolean visable = true;

    protected void init() {

    }

    public void flush() {
        flush = true;
    }

    public boolean update(long ctx) {
        return true;
    }

    public void keyEvent(int key, int action, int mods) {
    }

    public void characterEvent(String str, int modifiers) {
    }

    public void touchEvent(int phase, int x, int y) {
    }

    public void clickEvent(int x, int y) {
    }

    public void scrollEvent(double scrollX, double scrollY, int x, int y) {
    }

    public void onFocus() {

    }

    public void onUnFocus() {

    }

    public static boolean isInBoundle(float[] bound, float x, float y) {
        return x >= bound[LEFT] && x <= bound[LEFT] + bound[WIDTH]
                && y >= bound[TOP] && y <= bound[TOP] + bound[HEIGHT];
    }

    public float[] getBoundle() {
        return boundle;
    }

    public GObject getParent() {
        return parent;
    }

    public void setParent(GContainer p) {
        parent = p;
    }

    public float getX() {
        if (parent != null) {
            return parent.getX() + boundle[LEFT];
        }
        return boundle[LEFT];
    }

    public float getY() {
        if (parent != null) {
            return parent.getY() + boundle[TOP];
        }
        return boundle[TOP];
    }

    public float getW() {
        return boundle[WIDTH];
    }

    public float getH() {
        return boundle[HEIGHT];
    }

    /**
     * @return the bgColor
     */
    public float[] getBgColor() {
        return bgColor;
    }

    /**
     * @param bgColor the bgColor to set
     */
    public void setBgColor(int r, int g, int b, int a) {
        bgColor = nvgRGBA((byte) r, (byte) g, (byte) b, (byte) a);
    }

    /**
     * @return the color
     */
    public float[] getColor() {
        return color;
    }

    /**
     * @param color the color to set
     */
    public void setColor(int r, int g, int b, int a) {
        color = nvgRGBA((byte) r, (byte) g, (byte) b, (byte) a);
    }

    /**
     * @return the actionListener
     */
    public GActionListener getActionListener() {
        return actionListener;
    }

    /**
     * @param actionListener the actionListener to set
     */
    public void setActionListener(GActionListener actionListener) {
        this.actionListener = actionListener;
    }

    public void setVisable(boolean v) {
        visable = v;
    }

    public boolean getVisable() {
        return visable;
    }

    public GForm getForm() {
        GObject go = this;
        while ((go = go.parent) != null) {
            if (go instanceof GForm) {
                return (GForm) go;
            }
        }
        return null;
    }
}
