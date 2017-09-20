/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.reflect;

import javax.mini.jdwp.constant.Tag;
import javax.mini.jdwp.net.JdwpPacket;
import javax.mini.jdwp.type.ValueType;
import javax.mini.jdwp.vm.JdwpNative;

/**
 *
 * @author gust
 */
public class Array {

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
        bytes = ValueType.getBytes(type);
    }

    public void write(JdwpPacket p, int firstIndex, int length) {
        p.writeByte(type);
        int refer_size = JdwpNative.referenceTyepSize();
        for (int i = 0; i < length; i++) {
            switch (bytes) {
                case '1':
                    p.writeByte((byte) data_ptr.readByte(firstIndex + i));
                    break;
                case '2':
                    p.writeShort((short) data_ptr.readShort(firstIndex + i * 2));
                    break;
                case '4':
                    p.writeInt((int) data_ptr.readInt(firstIndex + i * 4));
                    break;
                case '8':
                    p.writeLong(data_ptr.readByte(firstIndex + i * 8));
                    break;
                case 'R':
                    p.writeRefer(data_ptr.readByte(firstIndex + i * refer_size));
                    break;
            }
        }
    }

    native void mapArray(long classId);
}
