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
public class StackFrame {

    public long runtimeId;
    public long classId;
    public long sonId;
    public long pc;
    public long byteCode;
    public long methodId;

    public StackFrame son;
    public StackFrame parent;
    public Method method;
    public long[] localVariables;
    public long localThis;

    public StackFrame(long rid) {
        this(rid, null);
    }

    public StackFrame(long rid, StackFrame parent) {
        this.runtimeId = rid;
        this.parent = parent;
        mapRuntime(runtimeId);
        method = new Method(methodId);
        //System.out.println(this);
        if (sonId != 0) {
            son = new StackFrame(sonId, this);
        }
    }

    public StackFrame getLastSon() {
        return son == null ? this : son.getLastSon();
    }

    /**
     * 返回 lastson=0 起的第frameID个runtime
     *
     * @param frameID
     * @return
     */
    public StackFrame getFrameByIndex(long frameID) {
        StackFrame r = getLastSon();
        for (int i = 0; i < frameID; i++) {
            r = r.parent;
        }
        return r;
    }

    public int getDeepth() {
        int deep = 0;
        StackFrame r = this;
        while (r != null) {
            r = r.son;
            deep++;
        }
        deep--;//顶层
        return deep;
    }

    public String toString() {
        return "Runtime:"
                + "|" + Long.toString(runtimeId, 16)
                + "|class:" + Long.toString(classId, 16)
                + "|son:" + Long.toString(sonId, 16)
                + "|pc:" + Long.toString(pc, 16)
                + "|" + Long.toString(byteCode, 16)
                + "|pos:" + (pc - byteCode)
                + "|this:" + Long.toString(localThis, 16)
                + "|" + method.methodName;
    }

    native void mapRuntime(long runtimeId);
}
//typedef struct _Runtime {
//    MethodInfo *methodInfo;
//    Class *clazz;
//    u8 *pc;
//    CodeAttribute *bytecode;//method bytecode
//    JavaThreadInfo *threadInfo;
//    Runtime *son;//sub method's runtime
//    StackFrame *stack;
//    LocalVarItem *localVariables;
//    s32 localvar_count;
//    u8 wideMode;
//} Runtime;
