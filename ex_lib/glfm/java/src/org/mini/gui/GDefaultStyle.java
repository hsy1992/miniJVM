/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

import static org.mini.gui.GToolkit.nvgRGBA;

/**
 *
 * @author gust
 */
public class GDefaultStyle implements GStyle {

    @Override
    public float getTextFontSize() {
        return 18f;
    }

    @Override
    public float getTitleFontSize() {
        return 20f;
    }

    @Override
    public float getIconFontSize() {
        return 35f;
    }

    float[] textFontColor = nvgRGBA(255, 255, 255, 128);

    @Override
    public float[] getTextFontColor() {
        return textFontColor;
    }

    float[] frameBackground = nvgRGBA(28, 30, 34, 192);

    @Override
    public float[] getFrameBackground() {
        return frameBackground;
    }

    float[] frameTitleColor = nvgRGBA(220, 220, 220, 160);

    @Override
    public float[] getFrameTitleColor() {
        return frameTitleColor;
    }

    float[] hintFontColor = nvgRGBA(255, 255, 255, 64);

    @Override
    public float[] getHintFontColor() {
        return hintFontColor;
    }

    float[] editBackground = nvgRGBA(255, 255, 255, 16);

    @Override
    public float[] getEditBackground() {
        return editBackground;
    }

}
