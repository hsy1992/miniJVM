/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Hashtable;

/**
 *
 * @author gust
 */
public class GToolkit {

    static Hashtable<Long, GForm> table = new Hashtable();

    static public GForm getForm(long win_ptr) {
        return table.get(win_ptr);
    }

    static public GForm removeForm(long win_ptr) {
        return table.remove(win_ptr);
    }

    static public void putForm(long win_ptr, GForm win) {
        table.put(win_ptr, win);
    }

    /**
     * =================================================================
     * =================================================================
     */

}
