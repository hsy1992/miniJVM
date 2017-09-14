/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.reflect;

/**
 *
 * @author gust
 */
public class Field {

    //不可随意改动字段类型及名字，要和native一起改
    public long fieldId;
    public String fieldName;
    public String signature;
    public short accessFlags;

    public Field(long fid) {
        this.fieldId = fid;
        mapField(fieldId);
    }

    public String toString() {
        return Long.toString(fieldId, 16) + "|"
                + fieldName + "|"
                + signature + "|access:"
                + Integer.toHexString(accessFlags) + "|";
    }

    native void mapField(long fid);
}
