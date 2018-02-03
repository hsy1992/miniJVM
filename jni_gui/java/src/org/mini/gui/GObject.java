/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.gui.GToolkit.nvgRGBA;
import org.mini.gui.event.GActionListener;
import org.mini.gui.event.GStateListener;

/**
 *
 * @author gust
 */
abstract public class GObject {

    public static char ICON_SEARCH = (char) 0x1F50D;
    public static char ICON_CIRCLED_CROSS = 0x2716;
    public static char ICON_CHEVRON_RIGHT = 0xE75E;
    public static char ICON_CHECK = 0x2713;
    public static char ICON_CHECK_NOT = 0x2714;
    public static char ICON_LOGIN = 0xE740;
    public static char ICON_TRASH = 0xE729;
    //
    GObject parent;

    //object position and width ,height
    float[] boundle = new float[4];
    static int LEFT = 0;
    static int TOP = 1;
    static int WIDTH = 2;
    static int HEIGHT = 3;

    float[] bgColor;
    float[] color;

    GActionListener actionListener;
    GStateListener stateListener;

    public void init() {

    }

    public boolean update(long ctx) {
        return true;
    }

    public void keyEvent(int key, int scanCode, int action, int mods) {
    }

    public void characterEvent(char character) {
    }

    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {
    }

    public void cursorPosEvent(int x, int y) {
    }

    public void dropEvent(int count, String[] paths) {
    }

    public void scrollEvent(double scrollX, double scrollY) {
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

    public void setParent(GObject p) {
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

    /**
     * @return the stateListener
     */
    public GStateListener getStateListener() {
        return stateListener;
    }

    /**
     * @param stateListener the stateListener to set
     */
    public void setStateListener(GStateListener stateListener) {
        this.stateListener = stateListener;
    }
}
