/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.fs;

import java.io.File;
import java.io.FileSystem;
import java.io.IOException;
import java.io.UnsupportedEncodingException;

/**
 *
 * @author gust
 */
public class FileSystemImpl extends FileSystem {

    @Override
    public char getSeparator() {
        return ':';
    }

    @Override
    public char getPathSeparator() {
        return '/';
    }

    @Override
    public String normalize(String path) {
        return path;
    }

    @Override
    public int prefixLength(String path) {
        return 0;
    }

    @Override
    public String getDefaultParent() {
        byte[] pathbuf = new byte[512];
        InnerFile.getcwd(pathbuf);
        int size = 0;
        while (pathbuf[size++] != 0) {
        }
        return new String(pathbuf, 0, size);
    }

    @Override
    public boolean isAbsolute(File f) {
        throw new RuntimeException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    private String getFullPath(String path) {
        byte[] full = new byte[512];
        InnerFile.fullpath(full, InnerFile.getPathBytes(path));
        int size = 0;
        while (full[size++] != 0) {
        }
        return new String(full, 0, size);
    }

    @Override
    public String resolve(File f) {
        return getFullPath(f.getPath());
    }

    @Override
    public String resolve(String parent, String child) {
        return getFullPath(parent + "/" + child);
    }

    @Override
    public String canonicalize(String path) throws IOException {
        return getFullPath(path);
    }

    @Override
    public int getBooleanAttributes(File f) {
        int att = 0;
        InnerFileStat ifa = new InnerFileStat();
        InnerFile.loadFS(InnerFile.getPathBytes(f.getPath()), ifa);
        att |= ifa.isDirectory() ? BA_DIRECTORY : 0;
        att |= ifa.exists ? BA_EXISTS : 0;
        att |= ifa.isFile() ? BA_REGULAR : 0;
        return att;
    }

    @Override
    public boolean checkAccess(File f, boolean write) {
        InnerFileStat ifa = new InnerFileStat();
        InnerFile.loadFS(InnerFile.getPathBytes(f.getPath()), ifa);
        return write ? ifa.canWrite() : ifa.canRead();
    }

    @Override
    public long getLastModifiedTime(File f) {
        InnerFileStat ifa = new InnerFileStat();
        InnerFile.loadFS(InnerFile.getPathBytes(f.getPath()), ifa);
        return ifa.st_mtime;
    }

    @Override
    public long getLength(File f) {
        InnerFileStat ifa = new InnerFileStat();
        InnerFile.loadFS(InnerFile.getPathBytes(f.getPath()), ifa);
        return ifa.st_size;
    }

    @Override
    public boolean createFileExclusively(String pathname) throws IOException {
        InnerFileStat ifa = new InnerFileStat();
        InnerFile.loadFS(InnerFile.getPathBytes(pathname), ifa);
        if (ifa.exists) {
            throw new IOException("file exists.");
        }
        long fd = InnerFile.openFile(InnerFile.getPathBytes(pathname), "w".getBytes());
        if (fd != 0) {
            InnerFile.closeFile(fd);
            return true;
        }
        return false;
    }

    @Override
    public boolean delete(File f) {
        return false;
    }

    @Override
    public boolean deleteOnExit(File f) {
        return false;
    }

    @Override
    public String[] list(File f) {
        return InnerFile.listDir(InnerFile.getPathBytes(f.getPath()));
    }

    @Override
    public boolean createDirectory(File f) {
        throw new RuntimeException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public boolean rename(File f1, File f2) {
        throw new RuntimeException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public boolean setLastModifiedTime(File f, long time) {
        throw new RuntimeException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public boolean setReadOnly(File f) {
        throw new RuntimeException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public File[] listRoots() {
        File f = new File("/");
        String[] strs = list(f);
        if (strs != null) {
            File[] files = new File[strs.length];
            for (int i = 0, iLen = files.length; i < iLen; i++) {
                files[i] = new File(strs[i]);
            }
            return files;
        }
        return new File[0];
    }

    @Override
    public int compare(File f1, File f2) {
        return f1.getAbsolutePath().compareTo(f2.getAbsolutePath());
    }

    @Override
    public int hashCode(File f) {
        return f.getAbsolutePath().hashCode();
    }

}
