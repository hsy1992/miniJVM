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
//    u8 type;//type of array or object or runtime
//    StackFrame *stack;
//    LocalVarItem *localVariables;
//    s32 localvar_count;
//    MethodInfo *methodInfo;
//    Class *clazz;
//    //method bytecode
//    u8 *pc;
//    CodeAttribute *codeAttr;
//    u8 wideMode;
////    Instance *thread;
//    JavaThreadInfo *threadInfo;
//    Runtime *son;//sub method's runtime
//} Runtime;
    public long pc_refer;
    public long son_refer;

    int pc_offset;
    int son_offset;

    public MemRuntime(long obj) {
        memobj = obj;
        pc_offset = 4 * JdwpNative.referenceTyepSize() + 1 + 4;
        son_offset = pc_offset + JdwpNative.referenceTyepSize();
        pc_refer = readRefer(pc_offset);
        son_refer = readRefer(son_offset);
    }
}
