/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.io;

import java.io.IOException;

/**
 *
 * open file for random access
 *
 * filemode : r rw rws rwd
 *
 * @author gust
 */
public class RandomAccessFile extends File {

    boolean flush = false;

    public RandomAccessFile(String ppath, String pmode) {
        System.out.println("pmode:"+pmode);
        this.path = ppath;
        if ("r".equals(pmode)) {
            this.mode = "rb";
        } else if ("rw".equals(pmode)) {
            this.mode = "w+b";
        } else if ("rws".equals(pmode)) {
            this.mode = "w+b";
            flush = true;
        } else if ("rwd".equals(pmode)) {
            this.mode = "w+b";
            flush = true;
        } else {
            this.mode = "rb";
        }
        System.out.println("mode:"+mode);
        filePointer = openFile(path.getBytes(), mode.getBytes());
    }

    public void close() throws IOException {
        closeFile(filePointer);
    }

    public int read(byte[] b, int off, int len) throws IOException {
        return readbuf(filePointer, b, off, len);
    }

    public int write(byte[] b, int off, int len) throws IOException {
        int ret = writebuf(filePointer, b, off, len);
        if (flush) {
            flush0(filePointer);
        }
        return ret;
    }

    public int seek(long pos) throws IOException {
        return seek0(filePointer, pos);
    }

    public int setLength(long length) throws IOException {
        return setLength0(filePointer, length);
    }
}
