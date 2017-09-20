/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.type;

import javax.mini.jdwp.constant.Tag;
import javax.mini.jdwp.net.JdwpPacket;
import javax.mini.jdwp.vm.JdwpNative;

/**
 *
 * @author gust
 */
public class ValueType {

    public byte type;
    public long value;
    public char bytes;

    public ValueType(byte type) {
        this.type = type;
        bytes = getBytes(type);
    }

    public ValueType(JdwpPacket p) {
        type = p.readByte();
        bytes = getBytes(type);
        switch (bytes) {
            case '1':
                value = p.readByte() & 0xff;
                break;
            case '2':
                value = p.readShort() & 0xffff;
                break;
            case '4':
                value = p.readInt() & 0xffffffff;
                break;
            case '8':
                value = p.readLong();
                break;
            case 'R':
                value = p.readLong();
                break;
            case Tag.VOID:
                break;
        }
    }
    
    public void write(JdwpPacket p){
        p.writeByte(type);
        switch (bytes) {
            case '1':
                p.writeByte((byte)value);
                break;
            case '2':
                p.writeShort((short)value);
                break;
            case '4':
                p.writeInt((int)value);
                break;
            case '8':
                p.writeLong(value);
                break;
            case 'R':
                p.writeRefer(value);
                break;
       
        }
    }

    public static char getBytes(byte type) {
        char bytes = '0';
        switch (type) {
            case Tag.BYTE:
            case Tag.BOOLEAN:
                bytes = '1';
                break;
            case Tag.SHORT:
            case Tag.CHAR:
                bytes = '2';
                break;
            case Tag.INT:
            case Tag.FLOAT:
                bytes = '4';
                break;
            case Tag.LONG:
            case Tag.DOUBLE:
                bytes = '8';
                break;
            case Tag.ARRAY:
            case Tag.OBJECT:
            case Tag.STRING:
            case Tag.THREAD:
            case Tag.THREAD_GROUP:
            case Tag.CLASS_LOADER:
            case Tag.CLASS_OBJECT:
                bytes = 'R';
                break;
            case Tag.VOID:
                bytes = '0';
                break;
        }
        return bytes;
    }

    public String toString() {
        return (char) type + "|" + bytes + "|" + Long.toString(value, 16);
    }
}
