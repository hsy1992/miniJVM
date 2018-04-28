/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfm.Glfm;
import org.mini.gui.event.GFocusChangeListener;
import static org.mini.nanovg.Gutil.toUtf8;

/**
 *
 * @author Gust
 */
public abstract class GTextObject extends GObject implements GFocusChangeListener {

    String hint;
    byte[] hint_arr;
    StringBuilder textsb = new StringBuilder();
    byte[] text_arr;

    public void setHint(String hint) {
        this.hint = hint;
        hint_arr = toUtf8(hint);
    }

    public String getHint() {
        return hint;
    }

    public void setText(String text) {
        this.textsb.setLength(0);
        this.textsb.append(text);
    }

    public String getText() {
        return textsb.toString();
    }

    public void doSelectText() {

    }

    public void doCopyClipBoard() {

    }

    public void doCut() {

    }

    public void doPasteClipBoard() {

    }

    public void doSelectAll() {

    }

    @Override
    public void focusGot(GObject go) {
        Glfm.glfmSetKeyboardVisible(getForm().getWinContext(), true);
    }

    @Override
    public void focusLost(GObject go) {
        Glfm.glfmSetKeyboardVisible(getForm().getWinContext(), false);
    }
}
