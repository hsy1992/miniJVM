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

    public void loadImage(String filepath) {
        id = Gutil.image_load(filepath);
        nk_image = NK.nk_image_id(id);
        refectArr = new Array(RefNative.obj2id(nk_image));
    }

    public byte[] getNkImage() {
        return nk_image;
    }

    public long getDataPtr() {
        return refectArr.getDataPtr();
    }
}
