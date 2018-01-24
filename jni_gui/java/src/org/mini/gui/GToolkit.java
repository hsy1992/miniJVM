/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.io.UnsupportedEncodingException;
import java.util.Hashtable;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.mini.reflect.Array;
import javax.mini.reflect.vm.RefNative;

/**
 *
 * @author gust
 */
public class GToolkit {

    static Hashtable<Long, GForm> table = new Hashtable();

    static public GForm getForm(long ctx) {
        return table.get(ctx);
    }

    static public GForm removeForm(long ctx) {
        return table.remove(ctx);
    }

    static public void putForm(long ctx, GForm win) {
        table.put(ctx, win);
    }

    /**
     * 
     * 返回数组数据区首地址
     * @param array
     * @return 
     */
    public static long getArrayDataPtr(Object array) {
        Array reflect_arr = new Array(RefNative.obj2id(array));
        return reflect_arr.getDataPtr();
    }
    
}
