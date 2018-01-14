/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package test;

import org.mini.gl.GL;
import static org.mini.gl.GL.glGetError;

/**
 *
 * @author gust
 */
public class Ball {

    final float PI = 3.1415926535f;
    final float PI2 = 3.1415926535f * 2;
    final int width = 600, height = 600;

    int uStepsNum = 50, vStepNum = 50;

    class Point {

        Point() {
        }

        ;
        Point(double a, double b, double c) {
            x = a;
            y = b;
            z = c;
        }
        ;
    double x;
        double y;
        double z;
    };

    Point getPoint(double u, double v) {
        double x = Math.sin(PI * v) * Math.cos(PI2 * u);
        double y = Math.sin(PI * v) * Math.sin(PI2 * u);
        double z = Math.cos(PI * v);
        return new Point(x, y, z);
    }

    void drawWire() {
        double ustep = 1 / (double) uStepsNum, vstep = 1 / (double) vStepNum;
        double u = 0, v = 0;
        //绘制下端三角形组
        for (int i = 0; i < uStepsNum; i++) {
            GL.glBegin(GL.GL_LINE_LOOP);
            Point a = getPoint(0, 0);
            GL.glVertex3d(a.x, a.y, a.z);
            Point b = getPoint(u, vstep);
            GL.glVertex3d(b.x, b.y, b.z);
            Point c = getPoint(u + ustep, vstep);
            GL.glVertex3d(c.x, c.y, c.z);
            u += ustep;
            GL.glEnd();
        }
        //绘制中间四边形组
        u = 0;
        v = vstep;
        for (int i = 1; i < vStepNum - 1; i++) {
            for (int j = 0; j < uStepsNum; j++) {
                GL.glBegin(GL.GL_LINE_LOOP);
                Point a = getPoint(u, v);
                Point b = getPoint(u + ustep, v);
                Point c = getPoint(u + ustep, v + vstep);
                Point d = getPoint(u, v + vstep);
                GL.glVertex3d(a.x, a.y, a.z);
                GL.glVertex3d(b.x, b.y, b.z);
                GL.glVertex3d(c.x, c.y, c.z);
                GL.glVertex3d(d.x, d.y, d.z);
                u += ustep;
                GL.glEnd();
            }
            v += vstep;
        }
        //绘制下端三角形组
        u = 0;
        for (int i = 0; i < uStepsNum; i++) {
            GL.glBegin(GL.GL_LINE_LOOP);
            Point a = getPoint(0, 1);
            Point b = getPoint(u, 1 - vstep);
            Point c = getPoint(u + ustep, 1 - vstep);
            GL.glVertex3d(a.x, a.y, a.z);
            GL.glVertex3d(b.x, b.y, b.z);
            GL.glVertex3d(c.x, c.y, c.z);
            GL.glEnd();
        }
    }

    void drawBall() {
        System.out.println("  error:" + glGetError());
        GL.glMatrixMode(GL.GL_MODELVIEW);
        GL.glLoadIdentity();
        GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
        GL.glColor3f(1.0f, 0.0f, 0.0f);
        GL.glPointSize(1.0f);
        GL.glRotated(30, 1, 0, 0);
        GL.glRotated(60, 0, 1, 0);
        GL.glRotated(90, 0, 0, 1);
        drawWire();
    }
}
