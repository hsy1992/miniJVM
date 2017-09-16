/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.vm;

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
    
    public static native int setBreakPoint(int setOrClear,byte typeTag,long classID,long methodID,long execIndex);

}
