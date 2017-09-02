/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.eio;

import java.io.IOException;
import javax.cldc.io.Connection;

/**
 *
 * @author gust
 */
public interface Socket extends Connection {

    static final byte OP_TYPE_NON_BLOCK = 0;
    static final byte OP_TYPE_REUSEADDR = 1;
    static final byte OP_TYPE_RCVBUF = 2;
    static final byte OP_TYPE_SNDBUF = 3;

    static final byte OP_VAL_NON_BLOCK = 1;
    static final byte OP_VAL_BLOCK = 0;
    static final byte OP_VAL_NON_REUSEADDR = 1;
    static final byte OP_VAL_REUSEADDR = 0;

//    int available() throws IOException;

    public int write(byte b[], int off, int len)
            throws IOException;

    int read(byte b[], int off, int len)
            throws IOException;

    void setOption(int type, int val);
}
