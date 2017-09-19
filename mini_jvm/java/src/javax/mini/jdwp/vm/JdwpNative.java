/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.vm;

import javax.mini.jdwp.type.ValueType;

/**
 *
 * @author gust
 */
public class JdwpNative {

    ClassLoader cl;

    public static native int referenceTyepSize();

    public static native long referenceId(Object o);

    public static native Object referenceObj(long objId);

    public static native Class[] getClasses();

    public static native Class getClassByName(String className);

    public static native int setBreakPoint(int setOrClear, byte typeTag, long classID, long methodID, long execIndex);

    public static native int setLocalVal(long frame, int slot, byte type, long value, int bytes);

    public static native int getLocalVal(long frame, int slot, ValueType val);

    public static native int getFieldVal(long objId, long fieldId, ValueType val);
}
