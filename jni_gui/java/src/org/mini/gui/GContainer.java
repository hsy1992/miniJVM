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
            nko.init();
            nko.setParent(null);
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
                    elements.add(ari.go);
                } else {
                    elements.remove(ari.go);
                }
            }
            cache.clear();
        }
        GObject[] arr = elements.toArray(new GObject[elements.size()]);
        for (int i = arr.length - 1; i >= 0; i--) {
            GObject nko = arr[i];
            if (!nko.update(ctx)) {
                elements.remove(nko);
            }
        }

        return true;
    }

    @Override
    public void keyEvent(int key, int scanCode, int action, int mods) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.keyEvent(key, scanCode, action, mods);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void characterEvent(char character) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.characterEvent(character);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void mouseButtonEvent(int button, boolean pressed, int x, int y) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.mouseButtonEvent(button, pressed, x, y);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void cursorPosEvent(int x, int y) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.cursorPosEvent(x, y);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void dropEvent(int count, String[] paths) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.dropEvent(count, paths);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void scrollEvent(double scrollX, double scrollY) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                nko.scrollEvent(scrollX, scrollY);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
