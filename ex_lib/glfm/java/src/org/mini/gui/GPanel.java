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
public class GPanel extends GContainer{
        GInitExtension extinit;

    void init() {
        if(extinit!=null){
            extinit.onInit(this);
        }
    }

    /**
     * @param extinit the extinit to set
     */
    public void setExtinit(GInitExtension extinit) {
        this.extinit = extinit;
    }

}
