/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.reflect;

import java.util.Vector;
import javax.mini.jdwp.constant.TypeTag;
import javax.mini.jdwp.vm.JdwpNative;

/**
 *
 * @author gust
 */
public class Reference {

    //不可随意改动字段类型及名字，要和native一起改
    public long classId;
    public String className;
    public short accessFlags;
    public String source;
    public int status;
    long fieldIds[];
    long methodIds[];

    public Field[] fields;
    public Method[] methods;

    public Reference(long classId) {
        this.classId = classId;
        mapReference(classId);
        fields = new Field[fieldIds.length];
        for (int i = 0; i < fieldIds.length; i++) {
            fields[i] = new Field(fieldIds[i]);
        }
        methods = new Method[methodIds.length];
        for (int i = 0; i < methodIds.length; i++) {
            methods[i] = new Method(methodIds[i]);
        }
    }

    public Method getMethod(long methodId) {
        for (int i = 0; i < methodIds.length; i++) {
            if (methods[i].methodId == methodId) {
                return methods[i];
            }
        }
        return null;
    }

    public Field getField(long fieldId) {
        for (int i = 0; i < fieldIds.length; i++) {
            if (fields[i].fieldId == fieldId) {
                return fields[i];
            }
        }
        return null;
    }

    public static byte getReferenceTypeTag(long classId) {
        Class clazz = JdwpNative.referenceObj(classId).getClass();
        if (clazz.isArray()) {
            return TypeTag.ARRAY;
        } else if (clazz.isInterface()) {
            return TypeTag.INTERFACE;
        } else {
            return TypeTag.CLASS;
        }
    }

    native void mapReference(long classId);

}
