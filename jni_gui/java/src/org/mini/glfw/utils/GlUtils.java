/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.glfw.utils;

/**
 *
 * @author gust
 */
public class GlUtils {

    /**
     * fill farr into barr and return barr
     * @param farr
     * @param barr
     * @return 
     */
    static public native byte[] f2b(float[] farr, byte[] barr);
}
