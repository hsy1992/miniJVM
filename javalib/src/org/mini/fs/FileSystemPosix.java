/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.fs;

/**
 *
 * @author gust
 */
public class FileSystemPosix extends FileSystemImpl {

    @Override
    public char getSeparator() {
        return '/';
    }

    @Override
    public char getPathSeparator() {
        return ':';
    }

    @Override
    public int prefixLength(String path) {
        return 0;
    }
}
