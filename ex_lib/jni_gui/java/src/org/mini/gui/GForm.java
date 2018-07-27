/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Iterator;
import java.util.Timer;
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
import static org.mini.glfw.Glfw.GLFW_KEY_ESCAPE;
import static org.mini.glfw.Glfw.GLFW_OPENGL_CORE_PROFILE;
import static org.mini.glfw.Glfw.GLFW_OPENGL_FORWARD_COMPAT;
import static org.mini.glfw.Glfw.GLFW_OPENGL_PROFILE;
import static org.mini.glfw.Glfw.GLFW_PRESS;
import static org.mini.glfw.Glfw.GLFW_TRUE;
import static org.mini.glfw.Glfw.glfwGetTime;
import static org.mini.glfw.Glfw.glfwPollEvents;
import static org.mini.glfw.Glfw.glfwSetWindowShouldClose;
import static org.mini.glfw.Glfw.glfwSwapBuffers;
import static org.mini.glfw.Glfw.glfwSwapInterval;
import static org.mini.glfw.Glfw.glfwTerminate;
import static org.mini.glfw.Glfw.glfwWindowHint;
import static org.mini.glfw.Glfw.glfwWindowShouldClose;
import org.mini.glfw.GlfwCallback;
import org.mini.glfw.GlfwCallbackAdapter;
import org.mini.nanovg.Gutil;
import static org.mini.gui.GObject.isInBoundle;
import static org.mini.glfw.Glfw.glfwGetFramebufferWidth;
import static org.mini.glfw.Glfw.glfwGetFramebufferHeight;
import static org.mini.nanovg.Nanovg.NVG_ANTIALIAS;
import static org.mini.nanovg.Nanovg.NVG_DEBUG;
import static org.mini.nanovg.Nanovg.NVG_STENCIL_STROKES;
import static org.mini.nanovg.Nanovg.nvgBeginFrame;
import static org.mini.nanovg.Nanovg.nvgCreateGL3;
import static org.mini.nanovg.Nanovg.nvgDeleteGL3;
import static org.mini.nanovg.Nanovg.nvgEndFrame;

/**
 *
 * @author gust
 */
public class GForm extends GContainer implements Runnable {

    String title;
    int width;
    int height;
    long win; //glfw win
    long vg; //nk contex
    GlfwCallback callback;
    static StbFont gfont;
    float fps;

    float pxRatio;
    int fbWidth, fbHeight;

    int[] unicode_range = {
        0x0020, 0xFFFF,
        0
    };
    //

    Timer timer = new Timer();//用于更新画面，UI系统采取按需刷新的原则

    public GForm(String title, int width, int height) {
        this.title = title;
        this.width = width;
        this.height = height;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;
    }

    public void setCallBack(GlfwCallback callback) {
        this.callback = callback;
        if (win != 0) {
            Glfw.glfwSetCallback(win, callback);
        }
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
        setCallBack(new FormCallBack());

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
        while (!glfwWindowShouldClose(win)) {
            try {
                glfwPollEvents();
                //user define contents
//                if (GObject.flushReq()) {
                display(vg);
                glfwSwapBuffers(win);
//                }
                count++;
                now = System.currentTimeMillis();
                if (now - last > 1000) {
                    //System.out.println("fps:" + count);
                    fps = count;
                    last = now;
                    count = 0;
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
                focus = nko;
                setFocus(nko);
                break;
            }
        }
    }

    class FormCallBack extends GlfwCallbackAdapter {

        int mouseX, mouseY, button;
        long mouseLastPressed;
        int CLICK_PERIOD = 200;

        @Override
        public void key(long window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            GForm.this.keyEvent(key, scancode, action, mods);
            GForm.this.flush();
        }

        @Override
        public void character(long window, char character) {
            GForm.this.characterEvent(character);
            GForm.this.flush();
        }

        @Override
        public void mouseButton(long window, int button, boolean pressed) {
            if (window == win) {
                switch (button) {
                    case Glfw.GLFW_MOUSE_BUTTON_1: {//left
                        if (pressed) {
                            findSetFocus(mouseX, mouseY);
                        } else {
                        }
                        break;
                    }
                    case Glfw.GLFW_MOUSE_BUTTON_2: {//right
                        if (pressed) {
                            findSetFocus(mouseX, mouseY);
                        } else {
                        }
                        break;
                    }
                    case Glfw.GLFW_MOUSE_BUTTON_3: {//middle
                        break;
                    }
                }
                //click event
                long cur = System.currentTimeMillis();
                if (pressed && cur - mouseLastPressed < CLICK_PERIOD && this.button == button) {
                    if (focus != null) {
                        focus.clickEvent(button, mouseX, mouseY);
                    } else {
                        GForm.this.clickEvent(button, mouseX, mouseY);
                    }
                } else //press event
                 if (focus != null) {
                        focus.mouseButtonEvent(button, pressed, mouseX, mouseY);
                    } else {
                        GForm.this.mouseButtonEvent(button, pressed, mouseX, mouseY);
                    }
                this.button = button;
                mouseLastPressed = cur;
            }
            GForm.this.flush();
        }

        @Override
        public void scroll(long window, double scrollX, double scrollY) {
            GForm.this.scrollEvent(scrollX, scrollY, mouseX, mouseY);
            GForm.this.flush();
        }

        @Override
        public void cursorPos(long window, int x, int y) {
            win = window;
            mouseX = x;
            mouseY = y;
            GForm.this.cursorPosEvent(x, y);
            GForm.this.flush();
        }

        @Override
        public boolean windowClose(long window) {
            GForm.this.flush();
            return true;
        }

        @Override
        public void windowSize(long window, int width, int height) {
            GForm.this.flush();
        }

        @Override
        public void framebufferSize(long window, int x, int y) {
            boundle[WIDTH] = x;
            boundle[HEIGHT] = y;
            GForm.this.flush();
        }

        @Override
        public void drop(long window, int count, String[] paths) {
            GForm.this.dropEvent(count, paths);
            GForm.this.flush();
        }

        public void error(int error, String description) {
            System.out.println("error: " + error + " message: " + description);
            GForm.this.flush();
        }
    }

    /**
     * @return the fps
     */
    public float getFps() {
        return fps;
    }

}
