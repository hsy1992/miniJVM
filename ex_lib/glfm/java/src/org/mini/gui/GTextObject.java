/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfm.Glfm;
import org.mini.gui.event.GActionListener;
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

    GMenu editMenu;

    boolean selectMode = false;

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

    abstract public String getSelectedText();

    abstract public void deleteSelectedText();

    abstract public void insertTextAtCaret(String str);

    public void doSelectText() {

    }

    public void doSelectAll() {

    }

    public void doCopyClipBoard() {
        String s = getSelectedText();
        if (s != null) {
            Glfm.glfmSetClipBoardContent(s);
        }
    }

    public void doCut() {
        doCopyClipBoard();
        deleteSelectedText();
    }

    public void doPasteClipBoard() {
        String s = Glfm.glfmGetClipBoardContent();
        if (s != null) {
            insertTextAtCaret(s);
        }
    }

    @Override
    public void focusGot(GObject go) {
        Glfm.glfmSetKeyboardVisible(getForm().getWinContext(), true);
    }

    @Override
    public void focusLost(GObject go) {
        Glfm.glfmSetKeyboardVisible(getForm().getWinContext(), false);
    }

    @Override
    public void longTouchedEvent(int x, int y) {
        callEditMenu(this, x, y);
    }

    /**
     * 唤出基于form层的编辑菜单,选中菜单项后消失,失去焦点后消失
     *
     * @param focus
     * @param x
     * @param y
     */
    void callEditMenu(final GTextObject focus, float x, float y) {
        float menuH = 40, menuW = 300;

        float mx = x - menuW / 2;
        if (mx < 10) {
            mx = 10;
        } else if (mx + menuW > focus.getForm().getDeviceWidth()) {
            mx = focus.getForm().getDeviceWidth() - menuW;
        }
        float my = y - 20 - menuH;
        if (my < 20) {
            my = y + 10;
        } else if (my + menuH > focus.getForm().getDeviceHeight()) {
            my = focus.getForm().getDeviceHeight() - menuH;
        }
        final GMenu menu = new GMenu((int) mx, (int) my, (int) menuW, (int) menuH);
        menu.addItem(GLanguage.getString("Select"), null);
        menu.addItem(GLanguage.getString("Copy"), null);
        menu.addItem(GLanguage.getString("Paste"), null);
        menu.addItem(GLanguage.getString("Cut"), null);
        menu.addItem(GLanguage.getString("Select All"), null);
        menu.setActionListener(new GActionListener() {
            GTextObject go = focus;
            GMenu m = menu;

            @Override
            public void action(GObject gobj) {
                int si = m.getSelectIndex();
                switch (si) {
                    case 0: {
                        go.doSelectText();
                        break;
                    }
                    case 1: {
                        go.doCopyClipBoard();
                        disposeEditMenu();
                        break;
                    }
                    case 2: {
                        go.doPasteClipBoard();
                        disposeEditMenu();
                        break;
                    }
                    case 3: {
                        go.doCut();
                        disposeEditMenu();
                        break;
                    }
                    case 4: {
                        go.doSelectAll();
                        break;
                    }
                }

            }
        });

//        menu.setFocusListener(new GFocusChangeListener() {
//            GMenu m = menu;
//
//            @Override
//            public void focusGot(GObject go) {
//            }
//
//            @Override
//            public void focusLost(GObject go) {
//                if (m.getForm() != null) {
//                    m.getForm().remove(m);
//                    editMenu = null;
//                }
//            }
//        });
        focus.getForm().add(menu);
        focus.getForm().setFocus(menu);
        disposeEditMenu();
        editMenu = menu;
    }

    void disposeEditMenu() {
        if (editMenu != null) {
            editMenu.getForm().remove(editMenu);
        }
        editMenu = null;
    }
}
