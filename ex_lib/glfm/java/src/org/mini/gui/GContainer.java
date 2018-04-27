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
import org.mini.glfm.Glfm;

/**
 *
 * @author gust
 */
abstract public class GContainer extends GObject {

    LinkedList<GObject> elements = new LinkedList();
    GObject focus;

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
                focus.onUnFocus();
            }
            this.focus = go;
            if (focus != null) {
                focus.onFocus();
            }
        }
    }

    public void add(GObject nko) {
        if (nko != null) {
            cache.add(new AddRemoveItem(AddRemoveItem.ADD, nko));
            nko.init();
            nko.setParent(this);
        }
    }

    public void remove(GObject nko) {
        if (nko != null) {
            cache.add(new AddRemoveItem(AddRemoveItem.REMOVE, nko));
        }
    }

    public boolean isSon(GObject son) {
        return elements.contains(son);
    }

    @Override
    public boolean update(long ctx) {
        synchronized (cache) {
            for (AddRemoveItem ari : cache) {
                if (ari.operation == AddRemoveItem.ADD) {
                    elements.addFirst(ari.go);
                    setFocus(ari.go);
                } else {
                    elements.remove(ari.go);
                    setFocus(null);
                }
            }
            cache.clear();
        }
        //如果focus不是第一个，则移到第一个，这样遮挡关系才正确
        if (focus != null && elements.peek() != focus) {
            elements.remove(focus);
            elements.addFirst(focus);
        }
        //更新所有UI组件
        GObject[] arr = elements.toArray(new GObject[elements.size()]);
        for (int i = arr.length - 1; i >= 0; i--) {
            GObject nko = arr[i];
            if (nko.getVisable()) {
                if (!nko.update(ctx)) {
                    elements.remove(nko);
                }
            }
        }
        return true;
    }

    @Override
    public void keyEvent(int key, int action, int mods) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.keyEvent(key, action, mods);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void characterEvent(String str, int mods) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.characterEvent(str, mods);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void touchEvent(int phase, int x, int y) {
        if (focus == null || !isInBoundle(focus.getBoundle(), x - getX(), y - getY())) {
            for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
                try {
                    GObject nko = it.next();
                    if (phase == Glfm.GLFMTouchPhaseBegan) {
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
            focus.touchEvent(phase, x, y);
        }
    }

    @Override
    public void scrollEvent(double scrollX, double scrollY, int x, int y) {
        if (focus != null) {
            focus.scrollEvent(scrollX, scrollY, x, y);
        } else {
            for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
                try {
                    GObject nko = it.next();
                    nko.scrollEvent(scrollX, scrollY, x, y);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    public void inertiaEvent(double x1, double y1, double x2, double y2, long moveTime) {

        if (focus != null) {
            focus.inertiaEvent(x1, y1, x2, y2, moveTime);
        } else {
            for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
                try {
                    GObject nko = it.next();
                    nko.inertiaEvent(x1, y1, x2, y2, moveTime);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    public void longTouchedEvent(int x, int y) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.longTouchedEvent(x, y);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
