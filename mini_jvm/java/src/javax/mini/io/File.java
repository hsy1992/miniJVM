/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.io;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

/**
 *
 * @author gust
 */
public class File {

    FileDescriptor fd;
    long filePointer;
    String path;
    boolean append;

    public File(String path) {
        this(path, false);

    }

    public File(String path, boolean append) {
        this.path = path;
        this.append = append;
        fd = new FileDescriptor();
        int ret = loadFD(path.getBytes(), fd);
        if (ret < 0) {
            fd.exists = false;
        } else {
            fd.exists = true;
        }
    }

    public boolean isFile() {
        return fd.isFile();
    }

    public boolean isDirectory() {
        return fd.isDirectory();
    }

    public boolean exists() {
        return fd.exists;
    }

    public long length() {
        return fd.st_size;
    }

    public String[] list() {
        return listDir(path.getBytes());
    }

    public OutputStream getOutputStream() throws IOException {
        if (filePointer == 0) {
            open();
        }
        return new FileOutputStream(filePointer);
    }

    public InputStream getInputStream() throws IOException {
        if (filePointer == 0) {
            open();
        }
        return new FileInputStream(filePointer);
    }

    void open() throws IOException {
        filePointer = openFile(path.getBytes(), append ? "wb+".getBytes() : "wb".getBytes());
    }

    public class FileInputStream extends InputStream {

        long fileHandle;

        public FileInputStream(long fileHandle) {
            this.fileHandle = fileHandle;
        }

        @Override
        public int read() throws IOException {
            int ret = read0(fileHandle);
            if (ret < 0) {
                throw new IOException("read file error: " + path);
            }
            return ret;
        }

        @Override
        public void close() {
            closeFile(fileHandle);
        }

    }

    public class FileOutputStream extends OutputStream {

        long fileHandle;

        public FileOutputStream(long fileHandle) {
            this.fileHandle = fileHandle;
        }

        @Override
        public void write(int b) throws IOException {
            int ret = write0(fileHandle, b);
            if (ret < 0) {
                throw new IOException("write file error: " + path);
            }
        }

    }

    static native int loadFD(byte[] filePath, FileDescriptor fd);

    static native String[] listDir(byte[] filePath);

    static native long openFile(byte[] filePath, byte[] mode);

    static native int closeFile(long fileHandle);

    static native int flush0(long fileHandle);

    static native int read0(long fileHandle);

    static native int write0(long fileHandle, int b);

    static native int readbuf(long fileHandle, byte[] b, int off, int len);

    static native int writebuf(long fileHandle, byte[] b, int off, int len);

    static native int seek0(long fileHandle, long pos);

    static native int setLength0(long fileHandle, long len);

}
