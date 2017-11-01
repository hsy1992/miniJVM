/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.io;

/**
 *
 * @author gust
 */
public class FileDescriptor {

    /* FIFO */
    static public final int _S_IFIFO = 0x1000;
    /* Character */
    static public final int _S_IFCHR = 0x2000;
    /* Directory */
    static public final int _S_IFDIR = 0x4000;
    /* Regular */
    static public final int _S_IFREG = 0x8000;

    static public final int _S_IFMT = 0xF000;
    boolean exists;

    int st_dev;       //文件的设备编号
    short st_ino;       //节点
    short st_mode;      //文件的类型和存取的权限
    short st_nlink;     //连到该文件的硬连接数目，刚建立的文件值为1
    short st_uid;       //用户ID
    short st_gid;       //组ID
    short st_rdev;      //(设备类型)若此文件为设备文件，则为其设备编号
    long st_size;      //文件字节数(文件大小)
    long st_atime;     //最后一次访问时间
    long st_mtime;     //最后一次修改时间
    long st_ctime;     //最后一次改变时间(指属性)

    public boolean isFile() {
        return (st_mode & _S_IFMT) == _S_IFREG;
    }

    public boolean isDirectory() {
        return (st_mode & _S_IFMT) == _S_IFDIR;
    }
}
