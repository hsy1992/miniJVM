/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.vm;

/**
 *
 *
 * @author gust
 */
public class MemRuntime extends MemObject {


    
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
    
    public long method_refer;
    public long clazz_refer;
    public long pc_refer;
    public long code_refer;
    public long son_refer;

    int method_offset;
    int clazz_offset;
    int pc_offset;
    int code_offset;
    int son_offset;

    public MemRuntime(long obj) {
        memobj = obj;
        method_offset = 0;
        clazz_offset = method_offset+ JdwpNative.referenceTyepSize();
        pc_offset = clazz_offset+ JdwpNative.referenceTyepSize() ;
        code_offset = pc_offset + JdwpNative.referenceTyepSize();
        son_offset = code_offset + 2 * JdwpNative.referenceTyepSize();
        method_refer = readRefer(method_offset);
        clazz_refer = readRefer(clazz_offset);
        pc_refer = readRefer(pc_offset);
        code_refer = readRefer(code_offset);
        son_refer = readRefer(son_offset);
    }
}
