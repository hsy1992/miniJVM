package test;

import static jdk.nashorn.internal.runtime.JSType.NULL;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_DEPTH_BUFFER_BIT;
import static org.mini.gl.GL.GL_STENCIL_BUFFER_BIT;
import static org.mini.gl.GL.GL_TRUE;
import static org.mini.gl.GL.glClear;
import static org.mini.gl.GL.glClearColor;
import static org.mini.gl.GL.glViewport;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MAJOR;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MINOR;
import static org.mini.glfw.Glfw.GLFW_KEY_ESCAPE;
import static org.mini.glfw.Glfw.GLFW_MOUSE_BUTTON_2;
import static org.mini.glfw.Glfw.GLFW_MOUSE_BUTTON_LEFT;
import static org.mini.glfw.Glfw.GLFW_OPENGL_CORE_PROFILE;
import static org.mini.glfw.Glfw.GLFW_OPENGL_FORWARD_COMPAT;
import static org.mini.glfw.Glfw.GLFW_OPENGL_PROFILE;
import static org.mini.glfw.Glfw.GLFW_PRESS;
import static org.mini.glfw.Glfw.GLFW_TRUE;
import static org.mini.glfw.Glfw.glfwCreateWindow;
import static org.mini.glfw.Glfw.glfwGetFramebufferSizeH;
import static org.mini.glfw.Glfw.glfwGetFramebufferSizeW;
import static org.mini.glfw.Glfw.glfwGetTime;
import static org.mini.glfw.Glfw.glfwInit;
import static org.mini.glfw.Glfw.glfwMakeContextCurrent;
import static org.mini.glfw.Glfw.glfwPollEvents;
import static org.mini.glfw.Glfw.glfwSetCallback;
import static org.mini.glfw.Glfw.glfwSetWindowShouldClose;
import static org.mini.glfw.Glfw.glfwSwapBuffers;
import static org.mini.glfw.Glfw.glfwTerminate;
import static org.mini.glfw.Glfw.glfwWindowHint;
import static org.mini.glfw.Glfw.glfwWindowShouldClose;
import org.mini.glfw.GlfwCallbackAdapter;
import static org.mini.glfw.utils.Gutil.toUtf8;
import org.mini.glfw.utils.Nutil;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_CENTER;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_LEFT;
import static org.mini.glfw.utils.Nutil.NVG_ALIGN_MIDDLE;
import static org.mini.glfw.utils.Nutil.NVG_ANTIALIAS;
import static org.mini.glfw.utils.Nutil.NVG_CCW;
import static org.mini.glfw.utils.Nutil.NVG_CW;
import static org.mini.glfw.utils.Nutil.NVG_DEBUG;
import static org.mini.glfw.utils.Nutil.NVG_HOLE;
import static org.mini.glfw.utils.Nutil.NVG_STENCIL_STROKES;
import static org.mini.glfw.utils.Nutil.nvgAddFallbackFontId;
import static org.mini.glfw.utils.Nutil.nvgArc;
import static org.mini.glfw.utils.Nutil.nvgBeginFrame;
import static org.mini.glfw.utils.Nutil.nvgBeginPath;
import static org.mini.glfw.utils.Nutil.nvgBoxGradient;
import static org.mini.glfw.utils.Nutil.nvgCircle;
import static org.mini.glfw.utils.Nutil.nvgClosePath;
import static org.mini.glfw.utils.Nutil.nvgCreateFont;
import static org.mini.glfw.utils.Nutil.nvgEndFrame;
import static org.mini.glfw.utils.Nutil.nvgFill;
import static org.mini.glfw.utils.Nutil.nvgFillColor;
import static org.mini.glfw.utils.Nutil.nvgFillPaint;
import static org.mini.glfw.utils.Nutil.nvgFontBlur;
import static org.mini.glfw.utils.Nutil.nvgFontFace;
import static org.mini.glfw.utils.Nutil.nvgFontSize;
import static org.mini.glfw.utils.Nutil.nvgHSLA;
import static org.mini.glfw.utils.Nutil.nvgLineTo;
import static org.mini.glfw.utils.Nutil.nvgLinearGradient;
import static org.mini.glfw.utils.Nutil.nvgMoveTo;
import static org.mini.glfw.utils.Nutil.nvgPathWinding;
import static org.mini.glfw.utils.Nutil.nvgRadialGradient;
import static org.mini.glfw.utils.Nutil.nvgRect;
import static org.mini.glfw.utils.Nutil.nvgRestore;
import static org.mini.glfw.utils.Nutil.nvgRotate;
import static org.mini.glfw.utils.Nutil.nvgRoundedRect;
import static org.mini.glfw.utils.Nutil.nvgSave;
import static org.mini.glfw.utils.Nutil.nvgStroke;
import static org.mini.glfw.utils.Nutil.nvgStrokeColor;
import static org.mini.glfw.utils.Nutil.nvgStrokeWidth;
import static org.mini.glfw.utils.Nutil.nvgText;
import static org.mini.glfw.utils.Nutil.nvgTextAlign;
import static org.mini.glfw.utils.Nutil.nvgTranslate;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author gust
 */
public class TestNanovg {

    long win;

    public static void main(String[] args) {
        TestNanovg gt = new TestNanovg();
        gt.t1();

    }

    void t1() {
        glfwInit();
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//        glfwWindowHint(GLFW_DEPTH_BITS, 16);
//        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
        win = glfwCreateWindow(640, 480, "hello glfw".getBytes(), 0, 0);
        if (win != 0) {
            glfwSetCallback(win, new CallBack());
            glfwMakeContextCurrent(win);
            //glfwSwapInterval(1);

            int w = glfwGetFramebufferSizeW(win);
            int h = glfwGetFramebufferSizeH(win);
            System.out.println("w=" + w + "  ,h=" + h);
            init();
            long last = System.currentTimeMillis(), now;
            int count = 0;
            while (!glfwWindowShouldClose(win)) {

                display();

                glfwPollEvents();
                glfwSwapBuffers(win);
                count++;
                now = System.currentTimeMillis();
                if (now - last > 1000) {
                    System.out.println("fps:" + count);
                    last = now;
                    count = 0;
                }
            }
            glfwTerminate();
        }
    }

    boolean exit = false;
    long curWin;
    int mx, my;

    class CallBack extends GlfwCallbackAdapter {

        @Override
        public void key(long window, int key, int scancode, int action, int mods) {
            System.out.println("key:" + key + " action:" + action);
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }

        @Override
        public void mouseButton(long window, int button, boolean pressed) {
            if (window == curWin) {
                String bt = button == GLFW_MOUSE_BUTTON_LEFT ? "LEFT" : button == GLFW_MOUSE_BUTTON_2 ? "RIGHT" : "OTHER";
                String press = pressed ? "pressed" : "released";
                System.out.println(bt + " " + mx + " " + my + "  " + press);
            }
        }

        @Override
        public void cursorPos(long window, int x, int y) {
            curWin = window;
            mx = x;
            my = y;
        }

        @Override
        public boolean windowClose(long window) {
            System.out.println("byebye");
            return true;
        }

        @Override
        public void windowSize(long window, int width, int height) {
            System.out.println("resize " + width + " " + height);
        }

        @Override
        public void framebufferSize(long window, int x, int y) {
        }
    }

//---------------------------------------------------------------------  
//  
    long vg;
    int font;
    byte[] fontname = "ch".getBytes();
    boolean premult;
    double t, prevt, dt;

    void init() {
        vg = Nutil.nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        if (vg == 0) {
            System.out.println("Could not init nanovg.\n");

        }
        font = nvgCreateFont(vg, fontname, toUtf8("../binary/res/wqymhei.ttc\000"));
        if (font == -1) {
            System.out.println("Could not add font.\n");
        }
        nvgAddFallbackFontId(vg, font, font);
    }

    void display() {

        float pxRatio;
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        winWidth = glfwGetFramebufferSizeW(win);
        winHeight = glfwGetFramebufferSizeH(win);
        fbWidth = glfwGetFramebufferSizeW(win);
        fbHeight = glfwGetFramebufferSizeH(win);
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float) fbWidth / (float) winWidth;

        t = glfwGetTime();
        dt = t - prevt;
        prevt = t;
        // Update and render
        glViewport(0, 0, fbWidth, fbHeight);
        if (premult) {
            glClearColor(0, 0, 0, 0);
        } else {
            glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
        float x, y;
        drawWindow(vg, "Widgets `n Stuff", 50, 50, 300, 400);
        x = 60;
        y = 95;
        drawSearchBox(vg, "Search", x, y, 280, 25);

        drawColorwheel(vg, winWidth - 300, winHeight - 300, 250.0f, 250.0f, (float) t);

        nvgEndFrame(vg);
        try {
            Thread.sleep(10);
        } catch (InterruptedException ex) {
        }

    }
    float NVG_PI = 3.1415926f;

    float[] nvgRGBA(int r, int g, int b, int a) {
        return Nutil.nvgRGBA((byte) r, (byte) g, (byte) b, (byte) a);
    }

    float cosf(float v) {
        return (float) Math.cos(v);
    }

    float sinf(float v) {
        return (float) Math.sin(v);
    }

    void drawColorwheel(long vg, float x, float y, float w, float h, float t) {
        int i;
        float r0, r1, ax, ay, bx, by, cx, cy, aeps, r;
        float hue = (float) Math.sin(t * 0.12f);
        byte[] paint;

        nvgSave(vg);

        /*	nvgBeginPath(vg);
	nvgRect(vg, x,y,w,h);
	nvgFillColor(vg, nvgRGBA(255,0,0,128));
	nvgFill(vg);*/
        cx = x + w * 0.5f;
        cy = y + h * 0.5f;
        r1 = (w < h ? w : h) * 0.5f - 5.0f;
        r0 = r1 - 20.0f;
        aeps = 0.5f / r1;	// half a pixel arc length in radians (2pi cancels out).

        for (i = 0; i < 6; i++) {
            float a0 = (float) i / 6.0f * NVG_PI * 2.0f - aeps;
            float a1 = (float) (i + 1.0f) / 6.0f * NVG_PI * 2.0f + aeps;
            nvgBeginPath(vg);
            nvgArc(vg, cx, cy, r0, a0, a1, NVG_CW);
            nvgArc(vg, cx, cy, r1, a1, a0, NVG_CCW);
            nvgClosePath(vg);
            ax = cx + cosf(a0) * (r0 + r1) * 0.5f;
            ay = cy + sinf(a0) * (r0 + r1) * 0.5f;
            bx = cx + cosf(a1) * (r0 + r1) * 0.5f;
            by = cy + sinf(a1) * (r0 + r1) * 0.5f;
            paint = nvgLinearGradient(vg, ax, ay, bx, by, Nutil.nvgHSLA(a0 / (NVG_PI * 2), 1.0f, 0.55f, (byte) 255), nvgHSLA(a1 / (NVG_PI * 2), 1.0f, 0.55f, (byte) 255));
            nvgFillPaint(vg, paint);
            nvgFill(vg);
        }

        nvgBeginPath(vg);
        nvgCircle(vg, cx, cy, r0 - 0.5f);
        nvgCircle(vg, cx, cy, r1 + 0.5f);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 64));
        nvgStrokeWidth(vg, 1.0f);
        nvgStroke(vg);

        // Selector
        nvgSave(vg);
        nvgTranslate(vg, cx, cy);
        nvgRotate(vg, hue * NVG_PI * 2);

        // Marker on
        nvgStrokeWidth(vg, 2.0f);
        nvgBeginPath(vg);
        nvgRect(vg, r0 - 1, -3, r1 - r0 + 2, 6);
        nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
        nvgStroke(vg);

        paint = nvgBoxGradient(vg, r0 - 3, -5, r1 - r0 + 6, 10, 2, 4, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
        nvgBeginPath(vg);
        nvgRect(vg, r0 - 2 - 10, -4 - 10, r1 - r0 + 4 + 20, 8 + 20);
        nvgRect(vg, r0 - 2, -4, r1 - r0 + 4, 8);
        nvgPathWinding(vg, NVG_HOLE);
        nvgFillPaint(vg, paint);
        nvgFill(vg);

        // Center triangle
        r = r0 - 6;
        ax = (float) Math.cos(120.0f / 180.0f * NVG_PI) * r;
        ay = (float) Math.sin(120.0f / 180.0f * NVG_PI) * r;
        bx = (float) Math.cos(-120.0f / 180.0f * NVG_PI) * r;
        by = (float) Math.sin(-120.0f / 180.0f * NVG_PI) * r;
        nvgBeginPath(vg);
        nvgMoveTo(vg, r, 0);
        nvgLineTo(vg, ax, ay);
        nvgLineTo(vg, bx, by);
        nvgClosePath(vg);
        paint = nvgLinearGradient(vg, r, 0, ax, ay, Nutil.nvgHSLA(hue, 1.0f, 0.5f, (byte) 255), nvgRGBA(255, 255, 255, 255));
        nvgFillPaint(vg, paint);
        nvgFill(vg);
        paint = nvgLinearGradient(vg, (r + ax) * 0.5f, (0 + ay) * 0.5f, bx, by, nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 0, 0, 255));
        nvgFillPaint(vg, paint);
        nvgFill(vg);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 64));
        nvgStroke(vg);

        // Select circle on triangle
        ax = (float) Math.cos(120.0f / 180.0f * NVG_PI) * r * 0.3f;
        ay = (float) Math.sin(120.0f / 180.0f * NVG_PI) * r * 0.4f;
        nvgStrokeWidth(vg, 2.0f);
        nvgBeginPath(vg);
        nvgCircle(vg, ax, ay, 5);
        nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 192));
        nvgStroke(vg);

        paint = nvgRadialGradient(vg, ax, ay, 7, 9, nvgRGBA(0, 0, 0, 64), nvgRGBA(0, 0, 0, 0));
        nvgBeginPath(vg);
        nvgRect(vg, ax - 20, ay - 20, 40, 40);
        nvgCircle(vg, ax, ay, 7);
        nvgPathWinding(vg, NVG_HOLE);
        nvgFillPaint(vg, paint);
        nvgFill(vg);

        nvgRestore(vg);

        nvgRestore(vg);
    }

    void drawWindow(long vg, String title, float x, float y, float w, float h) {
        float cornerRadius = 3.0f;
        byte[] shadowPaint;
        byte[] headerPaint;

        nvgSave(vg);
//	nvgClearState(vg);

        // Window
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgFillColor(vg, nvgRGBA(28, 30, 34, 192));
//	nvgFillColor(vg, nvgRGBA(0,0,0,128));
        nvgFill(vg);

        // Drop shadow
        shadowPaint = nvgBoxGradient(vg, x, y + 2, w, h, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
        nvgBeginPath(vg);
        nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgPathWinding(vg, NVG_HOLE);
        nvgFillPaint(vg, shadowPaint);
        nvgFill(vg);

        // Header
        headerPaint = nvgLinearGradient(vg, x, y, x, y + 15, nvgRGBA(255, 255, 255, 8), nvgRGBA(0, 0, 0, 16));
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x + 1, y + 1, w - 2, 30, cornerRadius - 1);
        nvgFillPaint(vg, headerPaint);
        nvgFill(vg);
        nvgBeginPath(vg);
        nvgMoveTo(vg, x + 0.5f, y + 0.5f + 30);
        nvgLineTo(vg, x + 0.5f + w - 1, y + 0.5f + 30);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
        nvgStroke(vg);

        nvgFontSize(vg, 18.0f);
        nvgFontFace(vg, fontname);
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

        nvgFontBlur(vg, 2);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
        nvgText(vg, x + w / 2, y + 16 + 1, toUtf8(title), null);

        nvgFontBlur(vg, 0);
        nvgFillColor(vg, nvgRGBA(220, 220, 220, 160));
        nvgText(vg, x + w / 2, y + 16, toUtf8(title), null);

        nvgRestore(vg);
    }

    void drawSearchBox(long vg, String text, float x, float y, float w, float h) {
        byte[] bg;
        byte[] icon = new byte[8];
        float cornerRadius = h / 2 - 1;

        // Edit
        bg = nvgBoxGradient(vg, x, y + 1.5f, w, h, h / 2, 5, nvgRGBA(0, 0, 0, 16), nvgRGBA(0, 0, 0, 92));
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x, y, w, h, cornerRadius);
        nvgFillPaint(vg, bg);
        nvgFill(vg);

        /*	nvgBeginPath(vg);
	nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, cornerRadius-0.5f);
	nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
	nvgStroke(vg);*/
        nvgFontSize(vg, h * 1.3f);
        nvgFontFace(vg, fontname);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 64));
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(vg, x + h * 0.55f, y + h * 0.55f, toUtf8("aaa\000"), null);

        nvgFontSize(vg, 20.0f);
        nvgFontFace(vg, fontname);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 32));

        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgText(vg, x + h * 1.05f, y + h * 0.5f, toUtf8(text), null);

        nvgFontSize(vg, h * 1.3f);
        nvgFontFace(vg, fontname);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 32));
        nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
        nvgText(vg, x + w - h * 0.55f, y + h * 0.55f, toUtf8("bbb\000"), null);
    }

}
