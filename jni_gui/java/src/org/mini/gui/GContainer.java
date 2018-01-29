/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

/**
 *
 * @author gust
 */
abstract public class GContainer extends GObject {

    Set<GObject> elements = new HashSet();

    //异步添加删除form
    final List<AddRemoveItem> cache = Collections.synchronizedList(new ArrayList());

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
        }
    }

    public void remove(GObject nko) {
        if (nko != null) {
            cache.add(new AddRemoveItem(AddRemoveItem.REMOVE, nko));
            nko.init();
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

        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            try {
                GObject nko = it.next();
                if (!nko.update(ctx)) {
                    it.remove();
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return true;
    }
}
