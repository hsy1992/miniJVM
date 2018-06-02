/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Iterator;
import org.mini.gl.GL;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_DEPTH_BUFFER_BIT;
import static org.mini.gl.GL.GL_STENCIL_BUFFER_BIT;
import org.mini.glfw.utils.StbFont;
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
import org.mini.glfw.utils.Gutil;
import org.mini.glfw.utils.Nutil;
import static org.mini.glfw.utils.Nutil.NVG_ANTIALIAS;
import static org.mini.glfw.utils.Nutil.NVG_DEBUG;
import static org.mini.glfw.utils.Nutil.NVG_STENCIL_STROKES;
import static org.mini.glfw.utils.Nutil.nvgBeginFrame;
import static org.mini.glfw.utils.Nutil.nvgEndFrame;
import static org.mini.gui.GObject.isInBoundle;
import static org.mini.gui.GToolkit.defaultStyle;
import static org.mini.glfw.Glfw.glfwGetFramebufferWidth;
import static org.mini.glfw.Glfw.glfwGetFramebufferHeight;

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

    int[] unicode_range = {
        0x0020, 0xFFFF,
        0
    };
    //
    double ct, prevt, dt;
    boolean premult;

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

    @Override
    public void init() {

        if (!Glfw.glfwInit()) {
            System.out.println("glfw init error.");
            System.exit(1);
        }
//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //glfwWindowHint(Glfw.GLFW_COCOA_RETINA_FRAMEBUFFER, GL_TRUE);

        win = Glfw.glfwCreateWindow(width, height, Gutil.toUtf8(title), 0, 0);
        if (win == 0) {
            glfwTerminate();
            System.exit(1);
        }
        Glfw.glfwMakeContextCurrent(win);
        glfwSwapInterval(1);
        vg = Nutil.nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        if (vg == 0) {
            System.out.println("Could not init nanovg.\n");

        }
        GToolkit.loadFont(vg);
        setCallBack(new FormCallBack());
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
                display(vg);
                glfwSwapBuffers(win);
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
        Nutil.nvgDeleteGL3(vg);
        glfwTerminate();
        GToolkit.removeForm(vg);
        vg = 0;
    }

    void display(long vg) {

        float pxRatio;
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        winWidth = Glfw.glfwGetWindowWidth(win);
        winHeight = Glfw.glfwGetWindowHeight(win);
        fbWidth = glfwGetFramebufferWidth(win);
        fbHeight = glfwGetFramebufferHeight(win);
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float) fbWidth / (float) winWidth;

        ct = glfwGetTime();
        dt = ct - prevt;
        prevt = ct;
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
        }

        @Override
        public void character(long window, char character) {
            GForm.this.characterEvent(character);
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
                } else { //press event
                    if (focus != null) {
                        focus.mouseButtonEvent(button, pressed, mouseX, mouseY);
                    } else {
                        GForm.this.mouseButtonEvent(button, pressed, mouseX, mouseY);
                    }
                }
                this.button = button;
                mouseLastPressed = cur;
            }
        }

        @Override
        public void scroll(long window, double scrollX, double scrollY) {
            GForm.this.scrollEvent(scrollX, scrollY, mouseX, mouseY);
        }

        @Override
        public void cursorPos(long window, int x, int y) {
            win = window;
            mouseX = x;
            mouseY = y;
            GForm.this.cursorPosEvent(x, y);
        }

        @Override
        public boolean windowClose(long window) {
            return true;
        }

        @Override
        public void windowSize(long window, int width, int height) {
        }

        @Override
        public void framebufferSize(long window, int x, int y) {
            boundle[WIDTH] = x;
            boundle[HEIGHT] = y;
        }

        @Override
        public void drop(long window, int count, String[] paths) {
            GForm.this.dropEvent(count, paths);
        }

        public void error(int error, String description) {
            System.out.println("error: " + error + " message: " + description);
        }
    }

    /**
     * @return the fps
     */
    public float getFps() {
        return fps;
    }

}
