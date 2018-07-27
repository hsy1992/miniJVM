/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 *
 * @author gust
 */
abstract public class GContainer extends GObject {

    LinkedList<GObject> elements = new LinkedList();
    GObject focus;

    int menuCount = 0;

    //异步添加删除form
    final List<AddRemoveItem> cache = Collections.synchronizedList(new LinkedList());

    class AddRemoveItem {

        static final int ADD = 0;
        static final int REMOVE = 1;
        int operation;
        GObject go;

        AddRemoveItem(int op, GObject go) {
            operation = op;
            this.go = go;
        }
    }

    /**
     * @return the focus
     */
    public GObject getFocus() {
        return focus;
    }

    /**
     * @param focus the focus to set
     */
    public void setFocus(GObject go) {
        if (this.focus != go) {
            if (focus != null) {
                if (focus.focusListener != null) {
                    focus.focusListener.focusLost(focus);
                }
            }
            this.focus = go;
            if (focus != null) {
                if (focus.focusListener != null) {
                    focus.focusListener.focusGot(focus);
                }
            }
        }
    }

    public void add(GObject nko) {
        if (nko != null) {
            synchronized (cache) {
                cache.add(new AddRemoveItem(AddRemoveItem.ADD, nko));
                nko.init();
                nko.setParent(this);
            }
        }
    }

    public void remove(GObject nko) {
        if (nko != null) {
            synchronized (cache) {
                nko.setParent(null);
                nko.destory();
                cache.add(new AddRemoveItem(AddRemoveItem.REMOVE, nko));
            }
        }
    }

    public boolean isSon(GObject son) {
        return elements.contains(son);
    }

    @Override
    public boolean update(long ctx) {
        synchronized (cache) {
            //菜单加在最前面,focus 在之后,其他组件再在其后

            for (AddRemoveItem ari : cache) {
                if (ari.operation == AddRemoveItem.ADD) {
                    setFocus(ari.go);
                    if (ari.go instanceof GMenu) {
                        menuCount++;
                        elements.addFirst(ari.go);
                    } else {
                        elements.add(menuCount, ari.go);
                    }
                } else {
                    boolean success = elements.remove(ari.go);
                    setFocus(null);
                    if (success && ari.go instanceof GMenu) {
                        menuCount--;
                    }
                }
            }
            cache.clear();
        }
        //如果focus不是第一个，则移到第一个，这样遮挡关系才正确
        if (focus != null && !(focus instanceof GMenu)) {
            elements.remove(focus);
            elements.add(menuCount, focus);
        }
        //更新所有UI组件
        GObject[] arr = elements.toArray(new GObject[elements.size()]);
        for (int i = arr.length - 1; i >= 0; i--) {
            GObject nko = arr[i];
            if (nko.isVisable()) {
                nko.update(ctx);
            }
        }
        return true;
    }

    @Override
    public void keyEvent(int key, int scanCode, int action, int mods) {
        if (focus != null) {
            focus.keyEvent(key, scanCode, action, mods);
        }
    }

    @Override
    public void characterEvent(char character) {
        if (focus != null) {
            focus.characterEvent(character);
        }
    }

    @Override
    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {
        if (focus == null || !isInBoundle(focus.getBoundle(), x - getX(), y - getY())) {
            for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
                try {
                    GObject nko = it.next();
                    if (pressed) {
                        if (isInBoundle(nko.getBoundle(), x - getX(), y - getY())) {
                            setFocus(nko);
                            break;
                        }
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        if (focus != null) {
            focus.mouseButtonEvent(button, pressed, x, y);
        }

    }

    @Override
    public void cursorPosEvent(int x, int y) {
        if (focus != null) {
            focus.cursorPosEvent(x, y);
        }

    }

    @Override
    public void dropEvent(int count, String[] paths) {
        if (focus != null) {
            focus.dropEvent(count, paths);
        }
    }

    @Override
    public void scrollEvent(double scrollX, double scrollY, int x, int y) {
        if (focus != null) {
            focus.scrollEvent(scrollX, scrollY, x, y);
        }
    }

    @Override
    public void clickEvent(int button, int x, int y) {
        if (focus != null) {
            focus.clickEvent(button, x, y);
        }
    }
}
