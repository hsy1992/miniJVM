/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.io;

import java.io.IOException;

/**
 *
 * @author gust
 */
public class RandomAccessFile extends File {

    public RandomAccessFile(String path) {
        super(path);
    }

    public RandomAccessFile(String path, boolean append) {
        super(path, append);
    }

    public void open() throws IOException {
        super.open();
    }

    void close() throws IOException {
        closeFile(filePointer);
    }

    int read(byte[] b, int off, int len) throws IOException {
        return readbuf(filePointer, b, off, len);
    }

    int write(byte[] b, int off, int len) throws IOException {
        return writebuf(filePointer, b, off, len);
    }

    int seek(long pos) throws IOException {
        return seek0(filePointer, pos);
    }

    int setLength(long length) throws IOException {
        return setLength0(filePointer, length);
    }
}
