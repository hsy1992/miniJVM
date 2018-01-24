/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.glfw.utils;

import java.io.UnsupportedEncodingException;
import org.mini.gl.GL;
import static org.mini.gl.GL.GL_LINES;
import static org.mini.gl.GL.glBegin;
import static org.mini.gl.GL.glEnd;

/**
 *
 * @author gust
 */
public class Gutil {

    /**
     * fill farr into barr and return barr
     *
     * @param farr
     * @param barr
     * @return
     */
    static public native byte[] f2b(float[] farr, byte[] barr);

    /**
     * load image return opengl GL_TEXTURE_2D id
     *
     * @param pfilename
     * @param para_3slot  new int[3] for image w,h,bit depth
     * @return
     */
    public static native int image_load(String pfilename, int[] para_3slot); //const char*, //struct nk_image/*none_ptr*/ 

    /**
     * vec and matrix
     */
    //vec2, vec3, vec4
    static public native float[] vec_add(float[] result, float[] vec1, float[] vec2);

    //vec2, vec3, vec4
    static public native float[] vec_sub(float[] result, float[] vec1, float[] vec2);

    //vec2, vec3, vec4
    static public native float[] vec_scale(float[] result, float[] vec1, float factor);

    //vec2, vec3, vec4
    static public native float vec_mul_inner(float[] vec1, float[] vec2);

    //vec2, vec3, vec4
    static public native float vec_len(float[] vec1);

    //vec2, vec3, vec4
    static public native float[] vec_normal(float[] result, float[] vec1);

    //vec3, vec4
    static public native float[] vec_mul_cross(float[] result, float[] vec1, float[] vec2);

    //vec3, vec4
    static public native float[] vec_reflect(float[] result, float[] vec1, float[] vec2);

    static public native float[] mat4x4_identity(float[] m1);

    static public native float[] mat4x4_dup(float[] r, float[] m1);

    static public native float[] mat4x4_row(float[] r, float[] m1, int row);

    static public native float[] mat4x4_col(float[] r, float[] m1, int col);

    static public native float[] mat4x4_transpose(float[] r, float[] m1);

    static public native float[] mat4x4_add(float[] r, float[] m1, float[] m2);

    static public native float[] mat4x4_sub(float[] r, float[] m1, float[] m2);

    static public native float[] mat4x4_mul(float[] r, float[] m1, float[] m2);

    static public native float[] mat4x4_mul_vec4(float[] r, float[] m1, float[] vec4);

    static public native float[] mat4x4_from_vec3_mul_outer(float[] r, float[] vec31, float[] vec32);

    static public native float[] mat4x4_translate(float[] r, float x, float y, float z);

    static public native float[] mat4x4_translate_in_place(float[] r, float x, float y, float z);

    static public native float[] mat4x4_scale(float[] r, float[] m1, float factor);

    static public native float[] mat4x4_scale_aniso(float[] r, float[] m1, float x, float y, float z);

    static public native float[] mat4x4_rotate(float[] r, float[] m1, float x, float y, float z, float a);

    static public native float[] mat4x4_rotateX(float[] r, float[] m1, float xa);

    static public native float[] mat4x4_rotateY(float[] r, float[] m1, float ya);

    static public native float[] mat4x4_rotateZ(float[] r, float[] m1, float xa);

    static public native float[] mat4x4_invert(float[] r, float[] m1);

    static public native float[] mat4x4_orthonormalize(float[] r, float[] m1);

    static public native float[] mat4x4_ortho(float[] rm, float l, float r, float b, float t, float n, float f);

    static public native float[] mat4x4_frustum(float[] rm, float l, float r, float b, float t, float n, float f);

    static public native float[] mat4x4_perspective(float[] rm, float y_fov, float aspect, float near, float far);

    static public native float[] mat4x4_look_at(float[] rm, float[] vec3_eye, float[] vec3_center, float[] vec3_up);

    static public void drawCood() {
//        GL.glPointSize(5.f);
        glBegin(GL_LINES);
        GL.glColor3f(1.f, 0, 0);
        GL.glVertex3f(0, 0, 0);
        GL.glVertex3f(1, 0, 0);
        glEnd();
        glBegin(GL_LINES);
        GL.glColor3f(0, 1.f, 0);
        GL.glVertex3f(0, 0, 0);
        GL.glVertex3f(0, 1, 0);
        glEnd();
        glBegin(GL_LINES);
        GL.glColor3f(0, 0, 1.f);
        GL.glVertex3f(0, 0, 0);
        GL.glVertex3f(0, 0, 1);
        glEnd();
    }

    public static byte[] toUtf8(String s) {
        if (s == null) {
            return null;
        }
        byte[] barr = null;
        try {
            barr = s.getBytes("utf-8");
        } catch (UnsupportedEncodingException ex) {
        }
        return barr;
    }
}
