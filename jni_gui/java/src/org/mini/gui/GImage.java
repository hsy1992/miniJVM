/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import javax.mini.reflect.Array;
import javax.mini.reflect.vm.RefNative;
import org.mini.glfw.utils.Gutil;
import org.mini.nk.NK;

/**
 *
 * @author gust
 */
public class GImage {

    byte[] nk_image;
    int id;
    Array refectArr;
    int[] w_h_d = new int[3];

    public GImage(int textureid, int w, int h) {
        id = textureid;
        w_h_d[0] = w;
        w_h_d[1] = h;
        nk_image = NK.nk_image_id(id);
        refectArr = new Array(RefNative.obj2id(nk_image));
    }

    public GImage(String filepath) {
        id = Gutil.image_load(filepath, w_h_d);
        nk_image = NK.nk_image_id(id);
        refectArr = new Array(RefNative.obj2id(nk_image));
    }

    public byte[] getNkImage() {
        return nk_image;
    }

    public int getWidth() {
        return w_h_d[0];
    }

    public int getHeight() {
        return w_h_d[1];
    }

    public int getBitDepth() {
        return w_h_d[2];
    }

    public long getDataPtr() {
        return refectArr.getDataPtr();
    }
}
