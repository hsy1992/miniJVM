/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.reflect;

import java.util.Vector;

/**
 *
 * @author gust
 */
public class Reference {

    //不可随意改动字段类型及名字，要和native一起改
    public long classId;
    public String className;
    public short accessFlags;
    public int fieldCount;
    public int methodCount;
    public String source;

    public Field[] fields;
    public Method[] methods;

    public Reference(long classId) {
        this.classId = classId;
        mapReference(classId);
        fields = new Field[fieldCount];
        for (int i = 0; i < fieldCount; i++) {
            Field field = new Field();
            mapField(field, classId, i);
            fields[i] = (field);
        }
        methods = new Method[methodCount];
        for (int i = 0; i < methodCount; i++) {
            Method method = new Method();
            mapMethod(method, classId, i);
            methods[i] = (method);
        }
    }

    public Method getMethod(long methodId) {
        for (int i = 0; i < methodCount; i++) {
            if (methods[i].methodId == methodId) {
                return methods[i];
            }
        }
        return null;
    }

    public Field getField(long fieldId) {
        for (int i = 0; i < fieldCount; i++) {
            if (fields[i].fieldId == fieldId) {
                return fields[i];
            }
        }
        return null;
    }

    native void mapReference(long classId);

    static native void mapField(Field field, long classId, int index);

    static native void mapMethod(Method field, long classId, int index);
}
