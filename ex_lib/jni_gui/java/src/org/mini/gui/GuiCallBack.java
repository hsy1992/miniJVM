/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import org.mini.glfw.Glfw;
import static org.mini.glfw.Glfw.GLFW_KEY_ESCAPE;
import static org.mini.glfw.Glfw.GLFW_PRESS;
import static org.mini.glfw.Glfw.GLFW_TRUE;
import static org.mini.glfw.Glfw.glfwSetWindowShouldClose;
import org.mini.glfw.GlfwCallbackAdapter;
import static org.mini.gui.GObject.HEIGHT;
import static org.mini.gui.GObject.WIDTH;

/**
 *
 * @author Gust
 */
public class GuiCallBack extends GlfwCallbackAdapter {

    GForm gform;
    long win;

    int mouseX, mouseY, button;
    long mouseLastPressed;
    int CLICK_PERIOD = 200;

    public void setWindowHandle(long h) {
        win = h;
    }

    public void setForm(GForm gf) {
        gform = gf;
    }

    @Override
    public void key(long window, int key, int scancode, int action, int mods) {
        if (gform == null) {
            return;
        }
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        gform.keyEvent(key, scancode, action, mods);
        gform.flush();
    }

    @Override
    public void character(long window, char character) {
        if (gform == null) {
            return;
        }
        gform.characterEvent(character);
        gform.flush();
    }

    @Override
    public void mouseButton(long window, int button, boolean pressed) {
        if (gform == null) {
            return;
        }
        if (window == win) {
            switch (button) {
                case Glfw.GLFW_MOUSE_BUTTON_1: {//left
                    if (pressed) {
                        gform.focus = gform.findFocus(mouseX, mouseY);
                    } else {
                    }
                    break;
                }
                case Glfw.GLFW_MOUSE_BUTTON_2: {//right
                    if (pressed) {
                        gform.focus = gform.findFocus(mouseX, mouseY);
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
                if (gform.focus != null) {
                    gform.focus.clickEvent(button, mouseX, mouseY);
                } else {
                    gform.clickEvent(button, mouseX, mouseY);
                }
            } else //press event
             if (gform.focus != null) {
                    gform.focus.mouseButtonEvent(button, pressed, mouseX, mouseY);
                } else {
                    gform.mouseButtonEvent(button, pressed, mouseX, mouseY);
                }
            this.button = button;
            mouseLastPressed = cur;
        }
        gform.flush();
    }

    @Override
    public void scroll(long window, double scrollX, double scrollY) {
        if (gform == null) {
            return;
        }
        gform.scrollEvent(scrollX, scrollY, mouseX, mouseY);
        gform.flush();
    }

    @Override
    public void cursorPos(long window, int x, int y) {
        if (gform == null) {
            return;
        }
        if (win == window) {
            mouseX = x;
            mouseY = y;
            gform.cursorPosEvent(x, y);
            gform.flush();
        }
    }

    @Override
    public boolean windowClose(long window) {
        if (gform == null) {
            return true;
        }
        gform.flush();
        return true;
    }

    @Override
    public void windowSize(long window, int width, int height) {
        if (gform == null) {
            return;
        }
        gform.flush();
    }

    @Override
    public void framebufferSize(long window, int x, int y) {
        if (gform == null) {
            return;
        }
        gform.boundle[WIDTH] = x;
        gform.boundle[HEIGHT] = y;
        gform.flush();
    }

    @Override
    public void drop(long window, int count, String[] paths) {
        if (gform == null) {
            return;
        }
        gform.dropEvent(count, paths);
        gform.flush();
    }

    public void error(int error, String description) {
        System.out.println("error: " + error + " message: " + description);
    }
}
