/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.net;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import javax.mini.net.Socket;
import javax.mini.util.LinkedList;

/**
 *
 * @author gust
 */
public class Session {

    Socket conn;
    LinkedList rpool = new LinkedList();
    LinkedList spool = new LinkedList();
    String handshake = "JDWP-Handshake";
    boolean firstRead;

    /**
     *
     */
    public Session(Socket sock) {
        this.conn = sock;
        firstRead = true;
        rcvNeed = handshake.length();
    }

    public void action() throws IOException {
        rcvAndDecode();
        sndAndEncode();
    }

    public void putPkg(byte[] data) {
        synchronized (spool) {
            spool.add(data);
        }
    }

    public byte[] getPkg() {
        synchronized (rpool) {
            if (!rpool.isEmpty()) {
                return (byte[]) rpool.removeFirst();
            }
        }
        return null;
    }
    /**
     * 接收数据
     */
    static final int PKG_BYTESSIZE_LEN = 4;// 包 长度定义 为几个字节
    byte[] tmpRbuf = new byte[256];

    int lenNeed = 0;
    byte[] lengthBuf = new byte[4];
    int rcvNeed = 0;
    ByteArrayOutputStream rcvBuf = new ByteArrayOutputStream();

    public void rcvAndDecode() throws IOException {

        while (true) {// 循环接收多个数据包
            if (rcvNeed == 0) {
                if (lenNeed == 0) {
                    lenNeed = PKG_BYTESSIZE_LEN;
                }
                // 取长度
                int r = conn.read(lengthBuf, PKG_BYTESSIZE_LEN - lenNeed, lenNeed);
                if (r < 0) {
                    throw new IOException("read pkg len error");
                } else if (r == 0) {
                    return;
                } else {
                    lenNeed -= r;
                    if (lenNeed == 0) {
                        rcvNeed = ((lengthBuf[0] & 0xff) << 24) | ((lengthBuf[1] & 0xff) << 16) | ((lengthBuf[2] & 0xff) << 8) | (lengthBuf[3] & 0xff);
                        rcvBuf.reset();
                    }
                }
            }
            if (rcvNeed > 0) {
                int r = conn.read(tmpRbuf, 0, rcvNeed);
                if (r < 0) {
                    throw new IOException("read pkg error");
                } else if (r == 0) {
                    return;
                } else {
                    rcvBuf.write(tmpRbuf, 0, r);
                    rcvNeed -= r;
                    if (rcvNeed == 0) {
                        byte[] b = rcvBuf.toByteArray();
                        print(b);
                        if (firstRead) {
                            if (handshake.equals(new String(b))) {
                                putPkg(b);
                                firstRead = false;
                            } else {
                                throw new IOException("none jdwp connection.");
                            }
                        } else {
                            synchronized (rpool) {
                                rpool.add(b);
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * 发送
     */
    int sent;
    private byte[] sndBuf;

    public void sndAndEncode() throws IOException {

        while (true) {// 循环接收多个数据包
            if (sndBuf == null) {
                synchronized (spool) {
                    if (!spool.isEmpty()) {
                        sndBuf = (byte[]) spool.removeFirst();
                    } else {
                        break;
                    }
                }
            }
            if (sndBuf != null) {
                int w = conn.write(sndBuf, sent, sndBuf.length - sent);
                if (w < 0) {
                    throw new IOException("write data error.");
                } else if (w == 0) {
                    break;
                } else {
                    sent += w;
                    if (sent == 0) {
                        sndBuf = null;
                    }
                }

            }

        }
    }

    // ----------------------------------------------------------------------------
    // no comment
    // ----------------------------------------------------------------------------
    static public void print(byte[] b) {
        for (int i = 0; i < b.length; i++) {
            String s = Integer.toHexString(b[i]);
            s = "00" + s;
            String tgt = "";
            for (int j = 0; j < 2; j++) {
                tgt = (s.charAt(s.length() - 1 - j)) + tgt;
            }
            System.out.print(" " + tgt);
        }
        System.out.println();
    }

}
