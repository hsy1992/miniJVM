/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Iterator;
import java.util.Timer;
import java.util.TimerTask;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_DEPTH_BUFFER_BIT;
import static org.mini.gl.GL.GL_STENCIL_BUFFER_BIT;
import org.mini.nanovg.StbFont;
import static org.mini.gl.GL.GL_TRUE;
import static org.mini.gl.GL.glClear;
import static org.mini.gl.GL.glClearColor;
import static org.mini.gl.GL.glViewport;
import org.mini.glfw.Glfw;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MAJOR;
import static org.mini.glfw.Glfw.GLFW_CONTEXT_VERSION_MINOR;
import static org.mini.glfw.Glfw.GLFW_OPENGL_CORE_PROFILE;
import static org.mini.glfw.Glfw.GLFW_OPENGL_FORWARD_COMPAT;
import static org.mini.glfw.Glfw.GLFW_OPENGL_PROFILE;
import static org.mini.glfw.Glfw.glfwPollEvents;
import static org.mini.glfw.Glfw.glfwSwapBuffers;
import static org.mini.glfw.Glfw.glfwSwapInterval;
import static org.mini.glfw.Glfw.glfwTerminate;
import static org.mini.glfw.Glfw.glfwWindowHint;
import static org.mini.glfw.Glfw.glfwWindowShouldClose;
import org.mini.nanovg.Gutil;
import static org.mini.glfw.Glfw.glfwGetFramebufferWidth;
import static org.mini.glfw.Glfw.glfwGetFramebufferHeight;
import static org.mini.gui.GObject.LEFT;
import static org.mini.gui.GObject.flush;
import static org.mini.gui.GToolkit.nvgRGBA;
import org.mini.nanovg.Nanovg;
import static org.mini.nanovg.Nanovg.NVG_ALIGN_MIDDLE;
import static org.mini.nanovg.Nanovg.NVG_ANTIALIAS;
import static org.mini.nanovg.Nanovg.NVG_DEBUG;
import static org.mini.nanovg.Nanovg.NVG_STENCIL_STROKES;
import static org.mini.nanovg.Nanovg.nvgBeginFrame;
import static org.mini.nanovg.Nanovg.nvgCreateGL3;
import static org.mini.nanovg.Nanovg.nvgDeleteGL3;
import static org.mini.nanovg.Nanovg.nvgEndFrame;
import static org.mini.nanovg.Nanovg.nvgFillColor;
import static org.mini.nanovg.Nanovg.nvgFontFace;
import static org.mini.nanovg.Nanovg.nvgFontSize;
import static org.mini.nanovg.Nanovg.nvgTextAlign;

/**
 *
 * @author gust
 */
public class GForm extends GPanel implements Runnable {

    String title;
    int width;
    int height;
    long win; //glfw win
    long vg; //nk contex
    GuiCallBack callback;
    static StbFont gfont;
    float fps;
    float fpsExpect = 30;

    float pxRatio;
    int fbWidth, fbHeight;

    int[] unicode_range = {
        0x0020, 0xFFFF,
        0
    };
    //

    Timer timer = new Timer(true);//用于更新画面，UI系统采取按需刷新的原则

    public GForm(String title, int width, int height, GuiCallBack ccb) {
        this.title = title;
        this.width = width;
        this.height = height;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;
        //set inner jpanel
        super.setLocation(0, 0);
        super.setSize(width, height);

        callback = ccb;
    }

    public GuiCallBack getCallBack() {
        return this.callback;
    }

    static public void setGFont(StbFont pgfont) {
        gfont = pgfont;
    }

    static public StbFont getGFont() {
        return gfont;
    }

    public long getGLContext() {
        return vg;
    }

    public int getDeviceWidth() {
        return (int) width;
    }

    public int getDeviceHeight() {
        return (int) height;
    }

    public long getNvContext() {
        return vg;
    }

    public long getWinContext() {
        return win;
    }

    @Override
    public void init() {

        if (!Glfw.glfwInit()) {
            System.out.println("glfw init error.");
            System.exit(1);
        }
        String osname = System.getProperty("os.name");
        if (osname != null && osname.contains("Mac")) {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(Glfw.GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);
        }
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        win = Glfw.glfwCreateWindow(width, height, Gutil.toUtf8(title), 0, 0);
        if (win == 0) {
            glfwTerminate();
            System.exit(1);
        }
        Glfw.glfwMakeContextCurrent(win);
        glfwSwapInterval(1);
        vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        if (vg == 0) {
            System.out.println("Could not init nanovg.\n");

        }
        GToolkit.loadFont(vg);

        callback.setWindowHandle(win);
        callback.setForm(this);
        Glfw.glfwSetCallback(win, callback);

        width = Glfw.glfwGetWindowWidth(win);
        height = Glfw.glfwGetWindowHeight(win);
        fbWidth = glfwGetFramebufferWidth(win);
        fbHeight = glfwGetFramebufferHeight(win);
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float) fbWidth / (float) width;

    }

    @Override
    public void run() {
        if (vg == 0) {
            System.out.println("gl context not inited.");
            return;
        }
        //
        GToolkit.putForm(vg, this);
        long last = System.currentTimeMillis(), now;
        int count = 0;

        long startAt, endAt, cost;
        while (!glfwWindowShouldClose(win)) {
            try {
                startAt = System.currentTimeMillis();
                glfwPollEvents();
                //user define contents
                if (GObject.flushReq()) {
                    display(vg);
                    glfwSwapBuffers(win);
                }
                count++;
                now = System.currentTimeMillis();
                if (now - last > 1000) {
                    //System.out.println("fps:" + count);
                    fps = count;
                    last = now;
                    count = 0;
                }

                endAt = System.currentTimeMillis();
                cost = endAt - startAt;
                if (cost < 1000 / fpsExpect) {
                    Thread.sleep((long) (1000 / fpsExpect - cost));
                }
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        }
        nvgDeleteGL3(vg);
        glfwTerminate();
        GToolkit.removeForm(vg);
        vg = 0;
    }

    void display(long vg) {

        // Update and render
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, width, height, pxRatio);
        Nanovg.nvgResetScissor(vg);
        Nanovg.nvgScissor(vg, 0, 0, width, height);
        update(vg);
        changeFocus();
        nvgEndFrame(vg);
    }

    void drawDebugInfo(long vg) {
        float font_size = 15;
        nvgFontSize(vg, font_size);
        nvgFontFace(vg, GToolkit.getFontWord());
        nvgTextAlign(vg, Nanovg.NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

        GuiCallBack cb = (GuiCallBack) callback;
        float dx = 10, dy = 40;
        byte[] b;
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));

        b = Gutil.toUtf8("touch x:" + cb.mouseX);
        Nanovg.nvgTextJni(vg, dx, dy, b, 0, b.length);
        dy += font_size;
        b = Gutil.toUtf8("touch y:" + cb.mouseY);
        Nanovg.nvgTextJni(vg, dx, dy, b, 0, b.length);
        dy += font_size;
        if (focus != null) {
            b = Gutil.toUtf8("focus x:" + focus.boundle[LEFT] + " y:" + focus.boundle[TOP] + " w:" + focus.boundle[WIDTH] + " h:" + focus.boundle[HEIGHT]);
            Nanovg.nvgTextJni(vg, dx, dy, b, 0, b.length);
        }
    }

    void changeFocus() {
        if (focus != null && elements.peek() != focus) {
            elements.remove(focus);
            elements.addFirst(focus);
        }
    }

    void findSetFocus(int x, int y) {
        for (Iterator<GObject> it = elements.iterator(); it.hasNext();) {
            GObject nko = it.next();
            if (nko.isInArea(x, y)) {
                focus = nko;
                setFocus(nko);
                break;
            }
        }
    }

    TimerTask tt_OnTouch = new TimerTask() {
        public void run() {
            flush();
        }
    };

    void tt_setupOnTouch() {
        timer.schedule(tt_OnTouch, 0L);//, (long) (1000 / fpsExpect));
    }

    /**
     * @return the fps
     */
    public float getFps() {
        return fps;
    }

    public void setFps(float fps) {
        fpsExpect = fps;
    }

}
