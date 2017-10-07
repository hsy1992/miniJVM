/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.reflect;

/**
 * 类方法的反射，以mini jvm中的 MethofInfo的实例内存地址进行初始化
 * 初始化中会把内存中的相应变量反射到Method实例中。
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
    
    public int argCnt;//The number of words in the frame used by arguments. Eight-byte arguments use two words; all others use one. 
    public LocalVarTable[] localVarTable;

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

    final native void mapMethod(long mid);
}
