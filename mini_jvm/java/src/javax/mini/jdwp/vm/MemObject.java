/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.vm;

/**
 * 从某个内存对象起始地址加上偏移量读取数据
 * @author gust
 */
public class MemObject {
    public long memobj;
    
    
    public native byte readByte(int offset);
    public native short readShort(int offset);
    public native int readInt(int offset);
    public native long readLong(int offset);
    public native long readRefer(int offset);
    
}
