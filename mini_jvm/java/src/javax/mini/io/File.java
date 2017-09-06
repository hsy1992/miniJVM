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
    String mode;

    public File(String path) {
        this.path = path;
        fd = new FileDescriptor();
        int ret = loadFD(path.getBytes(), fd);
        if (ret < 0) {
            fd.exists = false;
        } else {
            fd.exists = true;
        }
    }

    protected File() {
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

    public OutputStream getOutputStream(boolean append) throws IOException {
        if (filePointer != 0) {
            closeFile(filePointer);
        }
        filePointer = openFile(path.getBytes(), append ? "a+b".getBytes() : "w+b".getBytes());;

        return new FileOutputStream(filePointer);
    }

    public InputStream getInputStream() throws IOException {
        if (filePointer != 0) {
            closeFile(filePointer);
        }
        filePointer = openFile(path.getBytes(), "rb".getBytes());
        return new FileInputStream(filePointer);
    }

    public class FileInputStream extends InputStream {

        long fileHandle;

        public FileInputStream(long fileHandle) {
            this.fileHandle = fileHandle;
        }

        @Override
        public int read() throws IOException {
            return read0(fileHandle);
        }

        @Override
        public void close() throws IOException {
            closeFile(fileHandle);
            fileHandle = 0;
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

        @Override
        public void close() throws IOException {
            closeFile(fileHandle);
            fileHandle = 0;
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
