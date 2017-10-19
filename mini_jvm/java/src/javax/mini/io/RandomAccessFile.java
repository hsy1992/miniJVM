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
 * <pre>
 *    void t17() {
 *        try {
 *            RandomAccessFile c = new RandomAccessFile("./c.txt", "rw");
 *            c.seek(0);
 *            String r = "这是一个测试";
 *            System.out.println(r);
 *            byte[] carr = r.getBytes("utf-8");
 *            c.write(carr, 0, carr.length);
 *            c.close();
 *            RandomAccessFile c1 = new RandomAccessFile("./c.txt", "r");
 *            c1.seek(0);
 *            byte[] barr = new byte[256];
 *            int len;
 *            len = c1.read(barr, 0, 256);
 *            System.out.println("len=" + len);
 *            c1.close();
 *            String s = new String(barr, 0, len, "utf-8");
 *            System.out.println(s);
 *        } catch (IOException ex) {
 *            System.out.println(ex.getMessage());
 *        }
 *    }
 * </pre>
 *
 * @author gust
 */
public class RandomAccessFile extends File {

    boolean flush = false;

    public RandomAccessFile(String ppath, String pmode) {
        System.out.println("pmode:" + pmode);
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
        filePointer = openFile(path.getBytes(), mode.getBytes());
    }

    public void close() throws IOException {
        closeFile(filePointer);
        filePointer = 0;
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
