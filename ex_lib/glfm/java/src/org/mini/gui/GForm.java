/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Iterator;
import java.util.Timer;
import java.util.TimerTask;
import org.mini.gl.GL;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_DEPTH_BUFFER_BIT;
import static org.mini.gl.GL.GL_STENCIL_BUFFER_BIT;
import org.mini.nanovg.StbFont;
import static org.mini.gl.GL.glClear;
import static org.mini.gl.GL.glClearColor;
import static org.mini.gl.GL.glViewport;
import org.mini.glfm.Glfm;
import org.mini.glfm.GlfmCallBack;
import org.mini.glfm.GlfmCallBackAdapter;
import org.mini.nanovg.Nanovg;
import static org.mini.nanovg.Nanovg.NVG_ANTIALIAS;
import static org.mini.nanovg.Nanovg.NVG_DEBUG;
import static org.mini.nanovg.Nanovg.NVG_STENCIL_STROKES;
import static org.mini.nanovg.Nanovg.nvgBeginFrame;
import static org.mini.nanovg.Nanovg.nvgEndFrame;

/**
 *
 * @author gust
 */
public class GForm extends GPanel {

    String title;
    int width;
    int height;
    long win; //glfw win
    long vg; //nk contex
    GlfmCallBack callback;
    static StbFont gfont;
    float fps;
    float fpsExpect = 30;

    //
    float pxRatio;
    int winWidth, winHeight;
    int fbWidth, fbHeight;
    //
    boolean premult;

    Timer timer = new Timer();//用于更新画面，UI系统采取按需刷新的原则

    public GForm(String title, int width, int height, long display) {
        this.title = title;
        this.width = width;
        this.height = height;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;
        win = display;

        setCallBack(new FormCallBack());
    }

    public void setCallBack(GlfmCallBack callback) {
        this.callback = callback;
        if (win != 0) {
            Glfm.glfmSetCallBack(win, callback);
        }
    }

    static public void setGFont(StbFont pgfont) {
        gfont = pgfont;
    }

    static public StbFont getGFont() {
        return gfont;
    }

    public long getNvContext() {
        return vg;
    }

    public long getWinContext() {
        return win;
    }

    public int getDeviceWidth() {
        return (int) winWidth;
    }

    public int getDeviceHeight() {
        return (int) winHeight;
    }

    @Override
    void init() {

        if (0 == GL.gladLoadGLES2Loader()) {
            System.out.println("glad something went wrong!\n");
            System.exit(-1);
        }
        vg = Nanovg.nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        if (vg == 0) {
            System.out.println("Could not init nanovg.\n");

        }
        String respath = Glfm.glfmGetResRoot();
        System.setProperty("word_font_path", respath + "/wqymhei.ttc");
        System.setProperty("icon_font_path", respath + "/entypo.ttf");
        System.setProperty("emoji_font_path", respath + "/NotoEmoji-Regular.ttf");
        GToolkit.loadFont(vg);

        fbWidth = Glfm.glfmGetDisplayWidth(win);
        fbHeight = Glfm.glfmGetDisplayHeight(win);
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float) Glfm.glfmGetDisplayScale(win);
        winWidth = (int) (fbWidth / pxRatio);
        winHeight = (int) (fbHeight / pxRatio);
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;

        extinit.onInit(this);
        flush();
    }

    void display(long vg) {

        // Update and render
        glViewport(0, 0, fbWidth, fbHeight);
        if (premult) {
            glClearColor(0, 0, 0, 0);
        } else {
            glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, winWidth, winHeight, pxRatio);
        update(vg);
        changeFocus();
        nvgEndFrame(vg);
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
            if (isInBoundle(nko.getBoundle(), x, y)) {
                setFocus(nko);
                break;
            }
        }
    }

    class FormCallBack extends GlfmCallBackAdapter {

        int mouseX, mouseY, lastX, lastY;
        long mouseLastPressed;
        int CLICK_PERIOD = 200;

        long last, count;

        @Override
        public void mainLoop(long display, double frameTime) {
            long startAt, endAt, cost;
            try {
                startAt = System.currentTimeMillis();
                if (flush) {
                    display(vg);
                    flush = false;
                }
                count++;
                endAt = System.currentTimeMillis();
                cost = endAt - startAt;
                if (cost > 1000) {
                    //System.out.println("fps:" + count);
                    fps = count;
                    last = endAt;
                    count = 0;
                }
                if (cost < 1000 / fpsExpect) {
                    Thread.sleep((long) (1000 / fpsExpect - cost));
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onSurfaceCreated(long display, int width, int height) {
            GForm.this.init();
            flush();
        }

        @Override
        public boolean onKey(long display, int keyCode, int action, int modifiers) {
            System.out.println("keyCode  :" + keyCode + "   action=" + action + "   modifiers=" + modifiers);
            GForm.this.keyEvent(keyCode, action, modifiers);
            flush();
            return true;
        }

        @Override
        public void onCharacter(long window, String str, int modifiers) {
            System.out.println("onCharacter  :" + str + "   mod=" + modifiers);
            GForm.this.characterEvent(str, modifiers);
            flush();
        }

        @Override
        public boolean onTouch(long display, int touch, int phase, double x, double y) {
            x /= Glfm.glfmGetDisplayScale(display);
            y /= Glfm.glfmGetDisplayScale(display);
            lastX = mouseX;
            lastY = mouseY;
            mouseX = (int) x;
            mouseY = (int) y;
//            System.out.println("   touch=" + touch + "   phase=" + phase + "   x=" + x + "   y=" + y);
//            System.out.println("display=" + display + "   win=" + win);
            if (display == win) {

                switch (phase) {
                    case Glfm.GLFMTouchPhaseBegan: {//
                        findSetFocus(mouseX, mouseY);
                        break;
                    }
                    case Glfm.GLFMTouchPhaseEnded: {//
                        findSetFocus(mouseX, mouseY);
                        break;
                    }
                    case Glfm.GLFMTouchPhaseMoved: {//
                        scrollEvent(mouseX - lastX, mouseY - lastY, mouseX, mouseY);
                        break;
                    }
                    case Glfm.GLFMTouchPhaseHover: {//
                        break;
                    }
                }
                boolean pressed = phase == Glfm.GLFMTouchPhaseBegan;
                //click event
                long cur = System.currentTimeMillis();
                if (pressed && cur - mouseLastPressed < CLICK_PERIOD) {
                    if (focus != null) {
                        focus.clickEvent(mouseX, mouseY);
                    } else {
                        GForm.this.clickEvent(mouseX, mouseY);
                    }
                } else //press event
                 if (focus != null) {
                        focus.touchEvent(phase, mouseX, mouseY);
                    } else {
                        GForm.this.touchEvent(phase, mouseX, mouseY);
                    }
                mouseLastPressed = cur;
            }
            flush();
            return true;
        }

        @Override
        public void onSurfaceDestroyed(long window) {

        }

        @Override
        public void onSurfaceResize(long window, int width, int height) {
            GForm.this.boundle[WIDTH] = width;
            GForm.this.boundle[HEIGHT] = height;
            flush();
        }

        public void onSurfaceError(long display, String description) {
            System.out.println("error message: " + description);
            flush();
        }
    }

    TimerTask tt_OnTouch = new TimerTask() {
        public void run() {
            flush = true;
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
