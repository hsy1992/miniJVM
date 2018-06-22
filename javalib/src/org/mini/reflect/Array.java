/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.mini.reflect;

/**
 * 反射一个数组实例，如
 * 
 * int[] a=new int[3];
 * Array rarr = new Array(RefNative.obj2id(a));
 * int v=(int)rarr.getVal(2);
 * 
 * 即可通过rarr 访问内数组成员
 * 
 * @author gust
 */
public class Array {

    //不可随意改动字段类型及名字，要和native一起改
    public long arrayId;
    public byte type;
    char bytes; //'1','2','4','8','R'
    public int length;
    long arr_addr;
    //
    public MemAccess data_ptr;

    public Array(long array) {
        arrayId = array;
        mapArray(arrayId);
        data_ptr = new MemAccess(arr_addr);
        bytes = RConst.getBytes(type);
    }
    
    /**
     * return data start memory address
     * @return 
     */
    public long getDataPtr(){
        return arr_addr;
    }

    public long getVal(int firstIndex) throws IllegalAccessException {

        //int refer_size = JdwpNative.referenceTyepSize();
        switch (bytes) {
            case '1':
                return ((byte) data_ptr.readByte(firstIndex));

            case '2':
                return ((short) data_ptr.readShort(firstIndex));
            case '4':
                return ((int) data_ptr.readInt(firstIndex));
            case '8':
                return (data_ptr.readLong(firstIndex));
            case 'R':
                return (data_ptr.readRefer(firstIndex));
        }
        throw new IllegalAccessException();
    }

    final native void mapArray(long classId);
}
