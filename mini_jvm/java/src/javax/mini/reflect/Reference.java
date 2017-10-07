/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.reflect;

import javax.mini.reflect.vm.RefNative;

/**
 *
        Class cla = "xxx".getClass();
        Reference ref = new Reference(RefNative.obj2id(cla));
        System.out.println("ref.name=" + ref.className);
        try {
            String s = (String) cla.newInstance();
            System.out.println(s);
        } catch (InstantiationException ex) {
        } catch (IllegalAccessException ex) {
        }
 * 
 * @author gust
 */
public class Reference {

    public static final byte CLASS = 1; //ReferenceType is a class.
    public static final byte INTERFACE = 2; //ReferenceType is an interface.
    public static final byte ARRAY = 3; //ReferenceType is an array.

    //不可随意改动字段类型及名字，要和native一起改
    public long classId;
    public long superclass;
    public String className;
    public short accessFlags;
    public String source;
    public int status;
    long fieldIds[];
    long methodIds[];
    public long interfaces[];
    public long classObj;//类对象

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
        Class clazz = RefNative.id2obj(classId).getClass();
        if (clazz.isArray()) {
            return ARRAY;
        } else if (clazz.isInterface()) {
            return INTERFACE;
        } else {
            return CLASS;
        }
    }

    final native void mapReference(long classId);

}
