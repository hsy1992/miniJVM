/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import java.util.Iterator;
import static org.mini.gl.GL.GL_COLOR_BUFFER_BIT;
import static org.mini.gl.GL.GL_DEPTH_BUFFER_BIT;
import static org.mini.gl.GL.GL_STENCIL_BUFFER_BIT;
import org.mini.nanovg.StbFont;
import static org.mini.gl.GL.glClear;
import static org.mini.gl.GL.glClearColor;
import static org.mini.gl.GL.glViewport;
import org.mini.glfm.Glfm;
import static org.mini.glfm.Glfm.GLFMColorFormatRGBA8888;
import static org.mini.glfm.Glfm.GLFMDepthFormatNone;
import static org.mini.glfm.Glfm.GLFMMultisampleNone;
import static org.mini.glfm.Glfm.GLFMRenderingAPIOpenGLES2;
import static org.mini.glfm.Glfm.GLFMStencilFormatNone;
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
public class GForm extends GContainer {

    String title;
    int width;
    int height;
    long win; //glfw win
    long vg; //nk contex
    GlfmCallBack callback;
    static StbFont gfont;
    float fps;

    int[] unicode_range = {
        0x0020, 0xFFFF,
        0
    };
    //
    boolean premult;

    public GForm(String title, int width, int height, long display) {
        this.title = title;
        this.width = width;
        this.height = height;
        boundle[WIDTH] = width;
        boundle[HEIGHT] = height;
        win = display;
    }

    public void setCallBack(GlfmCallBack callback) {
        this.callback = callback;
        if (win != 0) {
            Glfm.glfmSetCallback(win, callback);
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

        GToolkit.loadFont(vg);
        Glfm.glfmSetDisplayConfig(win,
                GLFMRenderingAPIOpenGLES2,
                GLFMColorFormatRGBA8888,
                GLFMDepthFormatNone,
                GLFMStencilFormatNone,
                GLFMMultisampleNone);

        vg = Nanovg.nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
        if (vg == 0) {
            System.out.println("Could not init nanovg.\n");

        }
        setCallBack(new FormCallBack());

    }

//    public void paint() {
//        if (vg == 0) {
//            System.out.println("gl context not inited.");
//            return;
//        }
//        //
//        GToolkit.putForm(vg, this);
//        long last = System.currentTimeMillis(), now;
//        int count = 0;
//        while (!glfwWindowShouldClose(win)) {
//            try {
//                glfwPollEvents();
//                //user define contents
//                display(vg);
//                glfwSwapBuffers(win);
//                count++;
//                now = System.currentTimeMillis();
//                if (now - last > 1000) {
//                    //System.out.println("fps:" + count);
//                    fps = count;
//                    last = now;
//                    count = 0;
//                }
//            } catch (Exception ex) {
//                ex.printStackTrace();
//            }
//        }
//        Nanovg.nvgDeleteGL2(vg);
//        glfwTerminate();
//        GToolkit.removeForm(vg);
//        vg = 0;
//    }
    void display(long vg) {

        float pxRatio;
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        winWidth = Glfm.glfmGetDisplayWidth(win);
        winHeight = Glfm.glfmGetDisplayHeight(win);
        // Calculate pixel ration for hi-dpi devices.
        pxRatio = (float) Glfm.glfmGetDisplayScale(win);
        fbWidth = (int) (winWidth * pxRatio);
        fbHeight = (int) (winHeight * pxRatio);

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

    class FormCallBack extends GlfmCallBackAdapter {

        int mouseX, mouseY, button;
        long mouseLastPressed;
        int CLICK_PERIOD = 200;

        @Override
        public void mainLoop(long display, double frameTime) {
            display(vg);
        }

        @Override
        public void onSurfaceCreated(long display, int width, int height) {
            GForm.this.init();
        }

        @Override
        public boolean onKey(long display, int keyCode, int action, int modifiers) {
            GForm.this.keyEvent(keyCode, action, modifiers);
            return true;
        }

        @Override
        public void onCharacter(long window, String str, int modifiers) {
            GForm.this.characterEvent(str, modifiers);
        }

//        @Override
//        public boolean onTouch(long display, int touch, int phase, double x, double y) {
//            if (display == win) {
//                switch (button) {
//                    case Glfw.GLFW_MOUSE_BUTTON_1: {//left
//                        if (pressed) {
//                            findSetFocus(mouseX, mouseY);
//                        } else {
//                        }
//                        break;
//                    }
//                    case Glfw.GLFW_MOUSE_BUTTON_2: {//right
//                        if (pressed) {
//                            findSetFocus(mouseX, mouseY);
//                        } else {
//                        }
//                        break;
//                    }
//                    case Glfw.GLFW_MOUSE_BUTTON_3: {//middle
//                        break;
//                    }
//                }
//                //click event
//                long cur = System.currentTimeMillis();
//                if (pressed && cur - mouseLastPressed < CLICK_PERIOD && this.button == button) {
//                    if (focus != null) {
//                        focus.clickEvent(button, mouseX, mouseY);
//                    } else {
//                        GForm.this.clickEvent(button, mouseX, mouseY);
//                    }
//                } else { //press event
//                    if (focus != null) {
//                        focus.touchEvent(button, pressed, mouseX, mouseY);
//                    } else {
//                        GForm.this.touchEvent(button, pressed, mouseX, mouseY);
//                    }
//                }
//                this.button = button;
//                mouseLastPressed = cur;
//            }
//        }
//
//        @Override
//        public void scroll(long window, double scrollX, double scrollY) {
//            GForm.this.scrollEvent(scrollX, scrollY, mouseX, mouseY);
//        }
//
//        @Override
//        public void cursorPos(long window, int x, int y) {
//            win = window;
//            mouseX = x;
//            mouseY = y;
//            GForm.this.cursorPosEvent(x, y);
//        }
//
//        @Override
//        public boolean windowClose(long window) {
//            return true;
//        }
//
//        @Override
//        public void windowSize(long window, int width, int height) {
//        }
//
//        @Override
//        public void framebufferSize(long window, int x, int y) {
//            boundle[WIDTH] = x;
//            boundle[HEIGHT] = y;
//        }
//
//        @Override
//        public void drop(long window, int count, String[] paths) {
//            GForm.this.dropEvent(count, paths);
//        }
//
//        public void error(int error, String description) {
//            System.out.println("error: " + error + " message: " + description);
//        }
    }

    /**
     * @return the fps
     */
    public float getFps() {
        return fps;
    }

}
