/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.gui;

/**
 *
 * @author gust
 */
public interface GFrameContents {
    
    public void init(GFrame parent);

    public abstract void updateContents(long ctx, GFrame parent);
}
