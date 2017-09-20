/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.reflect;

/**
 *
 * @author gust
 */
public class MemAccess {
    
    public long memobj;
    
    public MemAccess(long memAddr){
        memobj=memAddr;
    }

    public byte readByte(int offset) {
        return readByte0(memobj, offset);
    }

    public short readShort(int offset) {
        return readShort0(memobj, offset);
    }

    public int readInt(int offset) {
        return readInt0(memobj, offset);
    }

    public long readLong(int offset) {
        return readLong0(memobj, offset);
    }

    public long readRefer(int offset) {
        return readRefer0(memobj, offset);
    }

    static native byte readByte0(long obj, int offset);

    static native short readShort0(long obj, int offset);

    static native int readInt0(long obj, int offset);

    static native long readLong0(long obj, int offset);

    static native long readRefer0(long obj, int offset);
}
