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
public class Runtime {
    public long runtimeId;
    public long classId;
    public long sonId;
    public long pc;
    public long byteCode;
    
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