/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author gust
 */
abstract public class GContainer extends GObject {

    List<GObject> elements = new ArrayList();

    //异步添加删除form
    final List<GObject> add_cache = Collections.synchronizedList(new ArrayList());
    final List<GObject> remove_cache = Collections.synchronizedList(new ArrayList());
    public synchronized void add(GObject nko) {
        add_cache.add(nko);
    }

    public synchronized void remove(GObject nko) {
        remove_cache.remove(nko);
    }

    @Override
    public boolean update(long ctx) {
        synchronized (add_cache) {
            elements.addAll(add_cache);
            add_cache.clear();
        }
        synchronized (remove_cache) {
            elements.removeAll(remove_cache);
            remove_cache.clear();
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
