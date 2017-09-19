/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import javax.mini.jdwp.vm.JdwpNative;
import javax.mini.jdwp.vm.JdwpThreads;
import java.io.IOException;
import java.util.Random;
import javax.mini.jdwp.type.Location;
import javax.mini.jdwp.constant.ClassStatus;
import javax.mini.jdwp.constant.Command;
import javax.mini.jdwp.constant.CommandSet;
import javax.mini.jdwp.constant.Error;
import javax.mini.jdwp.constant.Tag;
import javax.mini.jdwp.constant.TypeTag;
import javax.mini.jdwp.events.EventManager;
import javax.mini.jdwp.events.EventSet;
import javax.mini.jdwp.net.JdwpPacket;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.ResponsePacket;
import javax.mini.jdwp.net.Session;
import javax.mini.jdwp.reflect.LocalVarTable;
import javax.mini.jdwp.reflect.Method;
import javax.mini.jdwp.reflect.RConst;
import javax.mini.jdwp.reflect.Reference;
import javax.mini.jdwp.reflect.StackFrame;
import javax.mini.jdwp.type.ValueType;
import javax.mini.net.Socket;

/**
 *
 * @author gust
 */
public class DebugClient {

    Socket sock;
    Session session;
    boolean closed = false;

    public DebugClient(Socket psock) {
        sock = psock;
        sock.setOption(Socket.OP_TYPE_NON_BLOCK, Socket.OP_VAL_NON_BLOCK);
        session = new Session(sock);
        JdwpPacket.setReferBytes(JdwpNative.referenceTyepSize());
    }

    public void process() {
        try {
            session.action();
            EventManager.action(session);
            byte[] data;
            while ((data = session.receive()) != null) {
//                Session.print(data);
                processPacket(data);
            }
            //System.out.println("client process.");
        } catch (Exception e) {
            closed = true;
            System.out.println(e);
        }
    }

    public void processPacket(byte[] data) throws IOException {
        JdwpPacket packet = new JdwpPacket(data);
        if (packet.getFlag() == JdwpPacket.REQUEST) {
            RequestPacket req = new RequestPacket(data);
            System.out.println(req);
            switch (req.getCommandSet()) {
                case CommandSet.VirtualMachine: {//set 1

                    switch (req.getCommand()) {

                        case Command.VirtualMachine_Version: {//1.1
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeUTF("MINI_JVM");
                            res.writeInt(0);
                            res.writeInt(1);
                            res.writeUTF("0.1");
                            res.writeUTF("Mini jvm");
                            session.send(res.toByteArray());
//                            System.out.println(res);
                            break;
                        }
                        case Command.VirtualMachine_ClassesBySignature: {//1.2
                            String signature = req.readUTF();
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            String name = signatureToName(signature);
                            Class cl = JdwpNative.getClassByName(name);
                            if (cl == null) {
                                res.writeInt(0);
                            } else {
                                res.writeInt(1);
                                res.writeByte(TypeTag.CLASS);
                                long refid = JdwpNative.referenceId(cl);
                                res.writeRefer(refid);
                                Reference ref = new Reference(refid);
                                res.writeInt(ref.status);
                            }
                            System.out.println("VirtualMachine_ClassesBySignature:" + signature + "," + cl);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_AllClasses: {//1.3
                            Class[] classes = JdwpNative.getClasses();
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeInt(classes.length);
                            for (Class cl : classes) {
                                res.writeByte(Tag.CLASS_OBJECT);
                                res.writeRefer(JdwpNative.referenceId(cl));
                                res.writeUTF(cl.getName());
                                res.writeInt(ClassStatus.VERIFIED);
                            }
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_AllThreads: {//1.4
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            Thread[] threads = JdwpThreads.getThreads();
                            res.writeInt(threads.length - 2);
                            for (Thread t : threads) {
                                if (t == JdwpManager.getServer().getDispacher()
                                        || t == JdwpManager.getServer().getListener()) {//不发jdwp线程
                                    continue;
                                }
                                long tid = JdwpNative.referenceId(t);
                                System.out.println("VirtualMachine_AllThreads:" + t.getName());
                                res.writeRefer(tid);
                            }
                            session.send(res.toByteArray());
                            System.out.println(res);
                            break;
                        }
                        case Command.VirtualMachine_TopLevelThreadGroups: {//1.5
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeInt(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_Dispose: {//1.6
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            session.send(res.toByteArray());
                            closed = true;
                            break;
                        }
                        case Command.VirtualMachine_IDSizes: {//1.7
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeInt(JdwpNative.referenceTyepSize());
                            res.writeInt(JdwpNative.referenceTyepSize());
                            res.writeInt(JdwpNative.referenceTyepSize());
                            res.writeInt(JdwpNative.referenceTyepSize());
                            res.writeInt(JdwpNative.referenceTyepSize());
                            session.send(res.toByteArray());
                            System.out.println(res);
                            break;
                        }
                        case Command.VirtualMachine_Suspend: {//1.8
                            Thread[] threads = JdwpThreads.getThreads();
                            for (Thread t : threads) {
                                if (t == JdwpManager.getServer().getDispacher()
                                        || t == JdwpManager.getServer().getListener()) {//不发jdwp线程
                                    continue;
                                }
                                System.out.println("VirtualMachine_Suspend:" + t.getName());
                                JdwpThreads.suspendThread(t);
                            }
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_Resume: {//1.9
                            Thread[] threads = JdwpThreads.getThreads();
                            for (Thread t : threads) {
                                if (t == JdwpManager.getServer().getDispacher()
                                        || t == JdwpManager.getServer().getListener()) {//不发jdwp线程
                                    continue;
                                }
                                JdwpThreads.resumeThread(t);
                                System.out.println("VirtualMachine_Resume:" + t.getName());
                            }
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_Exit: {//1.10
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_CreateString: {//1.11
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_Capabilities: {//1.12
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeBoolean(false);//canWatchFieldModification
                            res.writeBoolean(false);//canWatchFieldAccess
                            res.writeBoolean(false);//canGetBytecodes
                            res.writeBoolean(false);//canGetSyntheticAttribute
                            res.writeBoolean(false);//canGetOwnedMonitorInfo
                            res.writeBoolean(false);//canGetCurrentContendedMonitor
                            res.writeBoolean(false);//canGetMonitorInfo
                            System.out.println("VirtualMachine_Capabilities");
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_ClassPaths: {//1.13
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_DisposeObjects: {//1.14
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_HoldEvents: {//1.15
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_ReleaseEvents: {//1.16
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_CapabilitiesNew: {//1.17
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_RedefineClasses: {//1.18
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_SetDefaultStratum: {//1.19
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.VirtualMachine_AllClassesWithGeneric: {//1.20
                            System.out.println(req + " not support");
                            break;
                        }

                    }
                    break;
                }

                case CommandSet.ReferenceType: {//set 2
                    switch (req.getCommand()) {
                        case Command.ReferenceType_Signature: {//2.1
                            long refType = req.readRefer();
                            Reference ref = JdwpManager.getReference(refType);
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            String str = "L" + ref.className + ";";
                            System.out.println("ReferenceType_Signature:" + Long.toString(refType, 16) + "," + str);
                            res.writeUTF(str);
                            session.send(res.toByteArray());

                            break;
                        }
                        case Command.ReferenceType_ClassLoader: {//2.2
                            long refType = req.readRefer();
                            Reference ref = JdwpManager.getReference(refType);
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeRefer(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ReferenceType_Modifiers: {//2.3
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_Fields: {//2.4
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_Methods: {//2.5
                            long refType = req.readRefer();
                            Reference ref = JdwpManager.getReference(refType);
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeInt(ref.methods.length);
                            for (int i = 0; i < ref.methods.length; i++) {
                                //System.out.println("method[" + i + "]" + ref.methods[i]);
                                res.writeRefer(ref.methods[i].methodId);
                                res.writeUTF(ref.methods[i].methodName);
                                res.writeUTF(ref.methods[i].signature);
                                res.writeInt(ref.methods[i].accessFlags);
                            }
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ReferenceType_GetValues: {//2.6
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_SourceFile: {//2.7
                            long refType = req.readRefer();
                            Reference ref = (Reference) JdwpManager.getReference(refType);
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeUTF(ref.source);
                            System.out.println("ReferenceType_SourceFile:" + ref.source);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ReferenceType_NestedTypes: {//2.8
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_Status: {//2.9
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_Interfaces: {//2.10
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_ClassObject: {//2.11
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_SourceDebugExtension: {//2.12
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_SignatureWithGeneric: {//2.13
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_FieldsWithGeneric: {//2.14
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ReferenceType_MethodsWithGeneric: {//2.15
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ClassType: {//set 3
                    switch (req.getCommand()) {
                        case Command.ClassType_Superclass: {//3.1
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ClassType_SetValues: {//3.2
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ClassType_InvokeMethod: {//3.3
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ClassType_NewInstance: {//3.4
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ArrayType: {//set 4
                    switch (req.getCommand()) {
                        case Command.ArrayType_NewInstance: {//4.1
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.InterfaceType: {//set 5
                    System.out.println(req + " not support");
                    break;
                }
                case CommandSet.Method: {//set 6
                    switch (req.getCommand()) {
                        case Command.Method_LineTable: {//6.1
                            long refType = req.readRefer();
                            Reference ref = (Reference) JdwpManager.getReference(refType);
                            long methodID = req.readRefer();
                            Method method = ref.getMethod(methodID);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeLong(method.codeStart);
                            res.writeLong(method.codeEnd);
                            res.writeInt(method.lines);
                            System.out.println("Method_LineTable:" + method.codeStart + "," + method.codeEnd + "," + method.lines + " ,arrlen=" + method.lineNum.length);
                            for (int i = 0; i < method.lines; i++) {
                                res.writeLong(method.lineNum[i * 2]);
                                res.writeInt(method.lineNum[i * 2 + 1]);
                            }
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.Method_VariableTable: {//6.2
                            long refType = req.readRefer();
                            long methodID = req.readRefer();
                            Method m = new Method(methodID);

                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(m.argCnt);
                            LocalVarTable[] tab = m.localVarTable;
                            res.writeInt(tab.length);
                            for (int i = 0; i < tab.length; i++) {
                                res.writeLong(tab[i].codeIndex);
                                res.writeUTF(tab[i].name);
                                res.writeUTF(tab[i].signature);
                                res.writeInt(tab[i].length);
                                res.writeInt(tab[i].slot);
                                System.out.println("Method_VariableTable:" + tab[i]);
                            }
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.Method_Bytecodes: {//6.3
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.Method_IsObsolete: {//6.4
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.Method_VariableTableWithGeneric: {//6.5
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.Field: {//set 8
                    break;
                }
                case CommandSet.ObjectReference: {//set 9
                    switch (req.getCommand()) {
                        case Command.ObjectReference_ReferenceType: {//9.1
                            long objid = req.readRefer();
                            Object obj = JdwpNative.referenceObj(objid);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            System.out.println("obj [" + Long.toString(objid, 16) + "].getClass()=" + obj.getClass());
                            res.writeByte(obj.getClass().isArray() ? (byte) TypeTag.ARRAY : obj.getClass().isInterface() ? TypeTag.INTERFACE : TypeTag.CLASS);
                            res.writeRefer(JdwpNative.referenceId(obj.getClass()));
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ObjectReference_GetValues: {//9.2
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ObjectReference_SetValues: {//9.3
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ObjectReference_MonitorInfo: {//9.5
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ObjectReference_InvokeMethod: {//9.6
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ObjectReference_DisableCollection: {//9.7
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ObjectReference_EnableCollection: {//9.8
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ObjectReference_IsCollected: {//9.9
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.StringReference: {//set 10
                    switch (req.getCommand()) {
                        case Command.StringReference_Value: {//10.1
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ThreadReference: {//set 11
                    switch (req.getCommand()) {
                        case Command.ThreadReference_Name: {//11.1
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeUTF(t.getName());
                            System.out.println("ThreadReference_Name:" + t.getName());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Suspend: {//11.2
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            JdwpThreads.suspendThread(t);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Resume: {//11.3
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            JdwpThreads.resumeThread(t);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Status: {//11.4
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JdwpThreads.getStatus(t));
                            res.writeInt(0);
                            System.out.println("ThreadReference_Status:" + t.getName() + "," + JdwpThreads.getStatus(t));
                            session.send(res.toByteArray());
                            break;
                        }

                        case Command.ThreadReference_ThreadGroup: {//11.5
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Frames: {//11.6
                            long thread = req.readRefer();
                            int startFrame = req.readInt();
                            int length = req.readInt();
                            System.out.println("ThreadReference_Frames: startFrame=" + startFrame + ", len=" + length);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);

                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            long rid = JdwpThreads.getTopRuntime(t);
                            javax.mini.jdwp.reflect.StackFrame runtime = new javax.mini.jdwp.reflect.StackFrame(rid);
                            int deepth = runtime.getDeepth();
                            if (length == -1) {//等于-1返回所有剩下的
                                length = deepth - startFrame;
                            }
                            res.writeInt(length);
                            javax.mini.jdwp.reflect.StackFrame r = runtime.getLastSon();
                            //System.out.println("deepth:" + runtime.getDeepth());
                            for (int i = 0; i < deepth; i++) {
                                if (i >= startFrame && i < startFrame + length) {//返回指定层级的runtimeframe
                                    res.writeRefer(r.runtimeId);
                                    Location loc = new Location();
                                    loc.typeTag = Reference.getReferenceTypeTag(r.classId);
                                    loc.classID = r.classId;
                                    loc.methodID = r.methodId;
                                    loc.execIndex = r.pc - r.byteCode;
                                    loc.writeLocation(res);
                                    //System.out.println(loc);
                                }
                                r = r.parent;
                                if (r == null) {
                                    break;
                                }
                            }
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_FrameCount: {//11.7
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JdwpThreads.getFrameCount(t));
                            System.out.println("ThreadReference_FrameCount:" + JdwpThreads.getFrameCount(t));
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_OwnedMonitors: {//11.8
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeUTF(t.getName());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_CurrentContendedMonitor: {//11.9
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeRefer(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Stop: {//11.10
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            long objid = req.readRefer();
                            JdwpThreads.stopThread(t, objid);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Interrupt: {//11.11
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            t.interrupt();
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_SuspendCount: {//11.12
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JdwpThreads.getSuspendCount(t));
                            System.out.println("ThreadReference_SuspendCount:" + t.getName() + "," + JdwpThreads.getSuspendCount(t));
                            session.send(res.toByteArray());
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ThreadGroupReference: {//set 12
                    switch (req.getCommand()) {
                        case Command.ThreadGroupReference_Name: {//12.1
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ThreadGroupReference_Parent: {//12.2
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ThreadGroupReference_Children: {//12.3
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ArrayReference: {//set 13
                    switch (req.getCommand()) {
                        case Command.ArrayReference_Length: {
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ArrayReference_GetValues: {
                            System.out.println(req + " not support");
                            break;
                        }
                        case Command.ArrayReference_SetValues: {
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ClassLoaderReference: {//set 14
                    switch (req.getCommand()) {
                        case Command.ClassLoaderReference_VisibleClasses: {
                            System.out.println(req + " not support");
                            break;
                        }

                    }
                    break;
                }
                case CommandSet.EventRequest: {//set 15
                    switch (req.getCommand()) {
                        case Command.EventRequest_Set: {//15.1
                            EventSet eventSet = EventManager.createEventSet(req);
                            EventManager.putEventSet(eventSet);
                            short ret = eventSet.process();
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            if (ret == Error.NONE) {
                                res.setErrorCode(Error.NONE);
                                res.writeInt(eventSet.getRequestId());
                            } else {
                                res.setErrorCode(ret);
                            }
                            session.send(res.toByteArray());
                            break;
                        }//
                        case Command.EventRequest_Clear: {//15.2
                            byte eventKind = req.readByte();
                            int requestID = req.readInt();
                            System.out.println("EventRequest_Clear:eventKind=" + eventKind + ", requestID=" + requestID);
                            EventSet set = EventManager.getEventSet(requestID);
                            if (set != null) {
                                set.clear();
                                EventManager.removeEventSet(requestID);
                            }
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.EventRequest_ClearAllBreakpoints: {//15.3`
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            Random r = new Random();
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.StackFrame: {//set 16
                    switch (req.getCommand()) {
                        case Command.StackFrame_GetValues: {
                            long thread = req.readRefer();
                            long frame = req.readRefer();
                            StackFrame r = new StackFrame(frame);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            
                            int slots = req.readInt();
                            res.writeInt(slots);
                            for (int i = 0; i < slots; i++) {
                                int slot = req.readInt();
                                byte type=req.readByte();
                                ValueType val = new ValueType(type);
                                JdwpNative.getLocalVal(frame, slot, val);
                                val.write(res);
                                System.out.println("StackFrame_GetValues,thead=" + thread + " , frame=" + frame + ", val=" + val);
                            }
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.StackFrame_SetValues: {
                            long thread = req.readRefer();
                            long frame = req.readRefer();
                            StackFrame r = new StackFrame(frame);
                            int slotValues = req.readInt();
                            for (int i = 0; i < slotValues; i++) {
                                int slot = req.readInt();
                                ValueType val = new ValueType(req);
                                JdwpNative.setLocalVal(frame, slot, val.type, val.value, val.bytes);
                                System.out.println("StackFrame_SetValues,thead=" + thread + " , frame=" + frame + ", val=" + val);
                            }
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.StackFrame_ThisObject: {
                            long thread = req.readRefer();
                            long frame = req.readRefer();
                            StackFrame r = new StackFrame(frame);
                            System.out.println("StackFrame_ThisObject,thead=" + thread + " , frame=" + frame + ", this=" + r.localThis);

                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeRefer(r.localThis);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.StackFrame_PopFrames: {
                            System.out.println(req + " not support");
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ClassObjectReference: {//set 17
                    switch (req.getCommand()) {
                        case Command.ClassObjectReference_ReflectedType: {
                            System.out.println(req + " not support");
                            break;
                        }

                    }
                    break;
                }
                case CommandSet.Event: {//set 64
                    switch (req.getCommand()) {
                        case Command.Event_Composite: {
                            System.out.println(req + " not support");
                            break;
                        }

                    }
                    break;
                }
            }
        } else {
            ResponsePacket res = new ResponsePacket(data);
            switch (res.getErrorCode()) {
                case javax.mini.jdwp.constant.Error.VM_DEAD: {
                    break;
                }
            }
        }
    }

    public boolean isClosed() {
        return closed;
    }

    public String signatureToName(String signature) {
        if (signature.startsWith("L")) {
            signature = signature.substring(1);
        }
        if (signature.endsWith(";")) {
            signature = signature.substring(0, signature.length() - 1);
        }
        return signature;
    }
}
