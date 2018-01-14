//package test;
//
//import static java.lang.Math.cos;
//import static java.lang.Math.sin;
//import static org.mini.gl.GL.GL_LINE_LOOP;
//import static org.mini.gl.GL.GL_MODELVIEW;
//import static org.mini.gl.GL.GL_PROJECTION;
//import static org.mini.gl.GL.GL_QUADS;
//import static org.mini.gl.GL.glBegin;
//import static org.mini.gl.GL.glColor3f;
//import static org.mini.gl.GL.glEnd;
//import static org.mini.gl.GL.glLoadIdentity;
//import static org.mini.gl.GL.glMatrixMode;
//import static org.mini.gl.GL.glOrtho;
//import static org.mini.gl.GL.glVertex3f;
//import static org.mini.gl.GL.glViewport;
//
//class Ball {
//
//    Point[] mx;
//    float pi = 3.1415926f;
//    public static final int SOLID = 3000;
//    public static final int WIRE = 3001;
//
//    float tx,ty,tz;
////typedef int int;
//    class Point {
//
//        float x;
//        float y;
//        float z;
//    };
//    int w, h, mode;
//
//    public Ball(float radius, int slices, int mode) {
//        w = 2 * slices;
//        h = slices;
//        this.mode = mode;
//        mx = getPointMatrix(radius, slices);
//    }
//    
//    void translate(float tx,float ty,float tz){
//        this.tx=tx;
//        this.ty=ty;
//        this.tz=tz;
//    }
//
//
//    void reshape(int w, int h) {
//        glViewport(0, 0, (int) w, (int) h);
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
//        glOrtho(0.0, 500, 0.0, 500, -500, 500);
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
//    }
//
//    int getPoint(float radius, float a, float b, Point p) {
//        p.x = (float) (radius * sin(a * pi / 180.0f) * cos(b * pi / 180.0f));
//        p.y = (float) (radius * sin(a * pi / 180.0f) * sin(b * pi / 180.0f));
//        p.z = (float) (radius * cos(a * pi / 180.0f));
//        return 1;
//    }
//
//    void drawSlice(Point p1, Point p2, Point p3, Point p4, int mode) {
//        switch (mode) {
//            case SOLID:
//                glBegin(GL_QUADS);
//                break;
//            case WIRE:
//                glBegin(GL_LINE_LOOP);
//                break;
//        }
//        glColor3f(1, 0, 0);
//        glVertex3f(p1.x, p1.y, p1.z);
//        glVertex3f(p2.x, p2.y, p2.z);
//        glVertex3f(p3.x, p3.y, p3.z);
//        glVertex3f(p4.x, p4.y, p4.z);
//        glEnd();
//    }
//
//    Point[] getPointMatrix(float radius, int slices) {
//        int i, j, w = 2 * slices, h = slices;
//        float a = 0.0f, b = 0.0f;
//        float hStep = 180.0f / (h - 1);
//        float wStep = 360.0f / w;
//        int length = w * h;
//        Point[] matrix;
//        matrix = new Point[length];
//        for (a = 0.0f, i = 0; i < h; i++, a += hStep) {
//            for (b = 0.0f, j = 0; j < w; j++, b += wStep) {
//                matrix[i * w + j] = new Point();
//                getPoint(radius, a, b, matrix[i * w + j]);
//            }
//        }
//        return matrix;
//    }
//
//    int drawSphere() {
//        int i = 0, j = 0;
//        for (; i < h - 1; i++) {
//            for (j = 0; j < w - 1; j++) {
//                drawSlice(mx[i * w + j], mx[i * w + j + 1], mx[(i + 1) * w + j + 1], mx[(i + 1) * w + j], mode);
//            }
//            drawSlice(mx[i * w + j], mx[i * w], mx[(i + 1) * w], mx[(i + 1) * w + j], mode);
//        }
//
//        return 1;
//    }
//}
package test;

import static java.lang.Math.cos;
import static java.lang.Math.sin;
import static org.mini.gl.GL.GL_LINE_LOOP;
import static org.mini.gl.GL.GL_MODELVIEW;
import static org.mini.gl.GL.GL_PROJECTION;
import static org.mini.gl.GL.GL_QUADS;
import static org.mini.gl.GL.glBegin;
import static org.mini.gl.GL.glColor3f;
import static org.mini.gl.GL.glEnd;
import static org.mini.gl.GL.glLoadIdentity;
import static org.mini.gl.GL.glMatrixMode;
import static org.mini.gl.GL.glOrtho;
import static org.mini.gl.GL.glVertex3f;
import static org.mini.gl.GL.glViewport;
import org.mini.glfw.utils.Gutil;

class Ball {

    float[][] mx;
    float pi = 3.1415926f;
    public static final int SOLID = 3000;
    public static final int WIRE = 3001;
    final int x = 0, y = 1, z = 2;

    float[] trans = new float[3];
//typedef int int;

    class Point {

        float x;
        float y;
        float z;
    };
    int w, h, mode;

    public Ball(float radius, int slices, int mode) {
        w = 2 * slices;
        h = slices;
        this.mode = mode;
        mx = getPointMatrix(radius, slices);
    }

    void translate(float tx, float ty, float tz) {
        trans[x] = tx;
        trans[y] = ty;
        trans[z] = tz;
    }

    void reshape(int w, int h) {
        glViewport(0, 0, (int) w, (int) h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, 500, 0.0, 500, -500, 500);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    int getPoint(float radius, float a, float b, float[] p) {
        p[x] = (float) (radius * sin(a * pi / 180.0f) * cos(b * pi / 180.0f));
        p[y] = (float) (radius * sin(a * pi / 180.0f) * sin(b * pi / 180.0f));
        p[z] = (float) (radius * cos(a * pi / 180.0f));
        return 1;
    }

    void drawSlice(float[] p1, float[] p2, float[] p3, float[] p4, int mode) {
        switch (mode) {
            case SOLID:
                glBegin(GL_QUADS);
                break;
            case WIRE:
                glBegin(GL_LINE_LOOP);
                break;
        }
        p1 = Gutil.vec_add(new float[3], p1, trans);
        p2 = Gutil.vec_add(new float[3], p2, trans);
        p3 = Gutil.vec_add(new float[3], p3, trans);
        p4 = Gutil.vec_add(new float[3], p4, trans);
        //glColor3f(1, 0, 0);
        glVertex3f(p1[x], p1[y], p1[z]);
        glVertex3f(p2[x], p2[y], p2[z]);
        glVertex3f(p3[x], p3[y], p3[z]);
        glVertex3f(p4[x], p4[y], p4[z]);
        glEnd();
    }

    float[][] getPointMatrix(float radius, int slices) {
        int i, j, w = 2 * slices, h = slices;
        float a = 0.0f, b = 0.0f;
        float hStep = 180.0f / (h - 1);
        float wStep = 360.0f / w;
        int length = w * h;
        float[][] matrix;
        matrix = new float[length][];
        for (a = 0.0f, i = 0; i < h; i++, a += hStep) {
            for (b = 0.0f, j = 0; j < w; j++, b += wStep) {
                matrix[i * w + j] = new float[3];
                getPoint(radius, a, b, matrix[i * w + j]);
            }
        }
        return matrix;
    }

    int drawSphere() {
        int i = 0, j = 0;
        for (; i < h - 1; i++) {
            for (j = 0; j < w - 1; j++) {
                drawSlice(mx[i * w + j], mx[i * w + j + 1], mx[(i + 1) * w + j + 1], mx[(i + 1) * w + j], mode);
            }
            drawSlice(mx[i * w + j], mx[i * w], mx[(i + 1) * w], mx[(i + 1) * w + j], mode);
        }

        return 1;
    }
}
