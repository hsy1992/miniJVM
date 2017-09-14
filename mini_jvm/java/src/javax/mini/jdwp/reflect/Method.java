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
public class Method {

    //不可随意改动字段类型及名字，要和native一起改
    public long methodId;
    public String methodName;
    public String signature;
    public String genericSignature = "";
    public short accessFlags;
    public long codeStart;
    public long codeEnd;
    public int lines;
    public short[] lineNum;

    public Method(long mid) {
        this.methodId = mid;
        mapMethod(methodId);
    }

    public String toString() {
        return Long.toString(methodId, 16) + "|"
                + methodName + "|"
                + signature + "|access:"
                + Integer.toHexString(accessFlags) + "|"
                + codeStart + "|"
                + codeEnd + "|lines:"
                + lines;
    }

    native void mapMethod(long mid);
}
