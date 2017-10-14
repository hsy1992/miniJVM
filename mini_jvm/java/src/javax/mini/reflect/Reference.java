/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.reflect;

import javax.mini.reflect.vm.RefNative;

/**
 *
 * Class cla = "xxx".getClass(); Reference ref = new
 * Reference(RefNative.obj2id(cla)); System.out.println("ref.name=" +
 * ref.className); try { String s = (String) cla.newInstance();
 * System.out.println(s); } catch (InstantiationException ex) { } catch
 * (IllegalAccessException ex) { }
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

    public Method getMethod(String methodName, String methodSignature) {
        for (int i = 0; i < methods.length; i++) {
            Method m = methods[i];
            if (m.methodName.equals(methodName) && m.signature.equals(methodSignature)) {
                return methods[i];
            }
        }
        return null;
    }

    public Method getMethod(String methodName, Class<?>... parameterTypes) {
        for (int i = 0; i < methods.length; i++) {
            Method m = methods[i];
            if (m.methodName.equals(methodName)) {
                boolean found = true;
                Class[] paras = m.getParameterTypes();
                if (parameterTypes.length == paras.length) {
                    for (int j = 0; j < parameterTypes.length; j++) {
                        if (paras[j] != parameterTypes[j]) {
                            found = false;
                        }
                    }
                } else {
                    found = false;
                }
                if (found) {
                    return methods[i];
                }
            }
        }
        return null;
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

    static public Class getClassBySignature(String s) {
        switch (s.charAt(0)) {
            case 'S':
                return RefNative.getClassByName("java/lang/Short");
            case 'C':
                return RefNative.getClassByName("java/lang/Character");
            case 'B':
                return RefNative.getClassByName("java/lang/Byte");
            case 'I':
                return RefNative.getClassByName("java/lang/Integer");
            case 'F':
                return RefNative.getClassByName("java/lang/Float");
            case 'Z':
                return RefNative.getClassByName("java/lang/Boolean");
            case 'D':
                return RefNative.getClassByName("java/lang/Double");
            case 'J':
                return RefNative.getClassByName("java/lang/Long");
            case 'L':
                s = s.substring(1, s.length() - 1);
                return RefNative.getClassByName(s);
            default:
                return RefNative.getClassByName(s);
        }
    }

    final native void mapReference(long classId);

}
