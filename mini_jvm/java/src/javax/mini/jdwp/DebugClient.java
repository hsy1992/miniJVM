/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import javax.mini.jdwp.vm.JdwpNative;
import javax.mini.jdwp.vm.JvmThreads;
import java.io.IOException;
import java.util.Random;
import javax.mini.jdwp.analyzer.Location;
import javax.mini.jdwp.constant.ClassStatus;
import javax.mini.jdwp.constant.Command;
import javax.mini.jdwp.constant.CommandSet;
import javax.mini.jdwp.constant.Error;
import javax.mini.jdwp.constant.SuspendStatus;
import javax.mini.jdwp.constant.Tag;
import javax.mini.jdwp.constant.TypeTag;
import javax.mini.jdwp.net.JdwpPacket;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.ResponsePacket;
import javax.mini.jdwp.net.Session;
import javax.mini.jdwp.vm.MemRuntime;
import javax.mini.net.Socket;
import javax.mini.util.ArrayList;
import javax.mini.util.LinkedList;

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
            byte[] data;
            while ((data = session.receive()) != null) {
//                Session.print(data);
                processPacket(data);
            }
//            System.out.println("client process.");
        } catch (Exception e) {
            closed = true;
            System.out.println(e);
        }
    }

    public void processPacket(byte[] data) throws IOException {
//        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data));
//        Packet packet = Packet.readPacket(dis, true);
//        PacketAnalyzer packetAnalyzer = AnalyzerManager.createPacketAnalyzer(packet);
//
//        if (packet instanceof Request) {
//            packetAnalyzer.updateInternalDataModel(packet);
//        }
//        if (packet instanceof Response) {
//            Response resp = (Response) packet;
//            if (!resp.isError()) {
//                packetAnalyzer.updateInternalDataModel(packet);
//            }
//        }
//        System.out.println("packet :"+packetAnalyzer);
        JdwpPacket packet = new JdwpPacket(data);
        if (packet.getFlag() == JdwpPacket.REQUEST) {
            RequestPacket req = new RequestPacket(data);
            System.out.println(req);
            switch (req.getCommandSet()) {
                case CommandSet.VirtualMachine: {//set 1

                    switch (req.getCommand()) {

                        case Command.VirtualMachine_Version: {//1
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
                        case Command.VirtualMachine_ClassesBySignature: {//2
                            String signature = req.readUTF();
                            System.out.println("VirtualMachine_ClassesBySignature:" + signature);
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeInt(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_AllClasses: {//3
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
                        case Command.VirtualMachine_AllThreads: {//4
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            Thread[] threads = JvmThreads.getThreads();
                            res.writeInt(threads.length - 1);
                            for (Thread t : threads) {
                                if (t == Thread.currentThread()) {
                                    continue;
                                }
                                long tid = JdwpNative.referenceId(t);
                                res.writeRefer(tid);
                                System.out.println("thread id:" + Long.toString(tid, 16));
                            }
                            session.send(res.toByteArray());
                            System.out.println(res);
                            break;
                        }
                        case Command.VirtualMachine_TopLevelThreadGroups: {//5
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeInt(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_Dispose: {//6
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_IDSizes: {//7
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
                        case Command.VirtualMachine_Suspend: {//8
                            Thread[] threads = JvmThreads.getThreads();
                            for (Thread t : threads) {
                                JvmThreads.suspendThread(t);
                            }
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_Resume: {//9
                            Thread[] threads = JvmThreads.getThreads();
                            for (Thread t : threads) {
                                JvmThreads.resumeThread(t);
                            }
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.VirtualMachine_Exit: {//10
                            break;
                        }
                        case Command.VirtualMachine_CreateString: {//11
                            break;
                        }
                        case Command.VirtualMachine_Capabilities: {//12
                            break;
                        }
                        case Command.VirtualMachine_ClassPaths: {//13
                            break;
                        }
                        case Command.VirtualMachine_DisposeObjects: {//14
                            break;
                        }
                        case Command.VirtualMachine_HoldEvents: {//15
                            break;
                        }
                        case Command.VirtualMachine_ReleaseEvents: {//16
                            break;
                        }
                        case Command.VirtualMachine_CapabilitiesNew: {//17
                            break;
                        }
                        case Command.VirtualMachine_RedefineClasses: {//18
                            break;
                        }
                        case Command.VirtualMachine_SetDefaultStratum: {//19
                            break;
                        }
                        case Command.VirtualMachine_AllClassesWithGeneric: {//20
                            break;
                        }

                    }
                    break;
                }

                case CommandSet.ReferenceType: {//set 2
                    switch (req.getCommand()) {
                        case Command.ReferenceType_Signature: {
                            break;
                        }
                        case Command.ReferenceType_ClassLoader: {
                            break;
                        }
                        case Command.ReferenceType_Modifiers: {
                            break;
                        }
                        case Command.ReferenceType_Fields: {
                            break;
                        }
                        case Command.ReferenceType_Methods: {
                            break;
                        }
                        case Command.ReferenceType_GetValues: {
                            break;
                        }
                        case Command.ReferenceType_SourceFile: {
                            break;
                        }
                        case Command.ReferenceType_NestedTypes: {
                            break;
                        }
                        case Command.ReferenceType_Status: {
                            break;
                        }
                        case Command.ReferenceType_Interfaces: {
                            break;
                        }
                        case Command.ReferenceType_ClassObject: {
                            break;
                        }
                        case Command.ReferenceType_SourceDebugExtension: {
                            break;
                        }
                        case Command.ReferenceType_SignatureWithGeneric: {
                            break;
                        }
                        case Command.ReferenceType_FieldsWithGeneric: {
                            break;
                        }
                        case Command.ReferenceType_MethodsWithGeneric: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ClassType: {//set 3
                    switch (req.getCommand()) {
                        case Command.ClassType_Superclass: {
                            break;
                        }
                        case Command.ClassType_SetValues: {
                            break;
                        }
                        case Command.ClassType_InvokeMethod: {
                            break;
                        }
                        case Command.ClassType_NewInstance: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ArrayType: {//set 4
                    switch (req.getCommand()) {
                        case Command.ArrayType_NewInstance: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.InterfaceType: {//set 5
                    break;
                }
                case CommandSet.Method: {//set 6
                    switch (req.getCommand()) {
                        case Command.Method_LineTable: {
                            break;
                        }
                        case Command.Method_VariableTable: {
                            break;
                        }
                        case Command.Method_Bytecodes: {
                            break;
                        }
                        case Command.Method_IsObsolete: {
                            break;
                        }
                        case Command.Method_VariableTableWithGeneric: {
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
                        case Command.ObjectReference_ReferenceType: {
                            long objid = req.readRefer();
                            Object obj = JdwpNative.referenceObj(objid);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeByte(obj.getClass().isArray() ? (byte) Tag.ARRAY : Tag.CLASS_OBJECT);
                            res.writeRefer(JdwpNative.referenceId(obj.getClass()));
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ObjectReference_GetValues: {
                            break;
                        }
                        case Command.ObjectReference_SetValues: {
                            break;
                        }
                        case Command.ObjectReference_MonitorInfo: {
                            break;
                        }
                        case Command.ObjectReference_InvokeMethod: {
                            break;
                        }
                        case Command.ObjectReference_DisableCollection: {
                            break;
                        }
                        case Command.ObjectReference_EnableCollection: {
                            break;
                        }
                        case Command.ObjectReference_IsCollected: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.StringReference: {//set 10
                    switch (req.getCommand()) {
                        case Command.StringReference_Value: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ThreadReference: {//set 11
                    switch (req.getCommand()) {
                        case Command.ThreadReference_Name: {//1
                            long thread = req.readRefer();
                            System.out.println("ThreadReference_Name:" + Long.toString(thread, 16));
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeUTF(t.getName());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Suspend: {//2
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            JvmThreads.suspendThread(t);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Resume: {//3
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            JvmThreads.resumeThread(t);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Status: {//4
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JvmThreads.getStatus(t));
                            res.writeInt(SuspendStatus.SUSPEND_STATUS_SUSPENDED);
                            session.send(res.toByteArray());
                            break;
                        }

                        case Command.ThreadReference_ThreadGroup: {//5
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Frames: {//6
                            long thread = req.readRefer();
                            int startFrame = req.readInt();
                            int length = req.readInt();
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);

                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            JvmThreads.suspendThread(t);
                            long rid = JvmThreads.getTopRuntime(t);
                            MemRuntime runtime = new MemRuntime(rid);
                            int i = 0;
                            LinkedList list = new LinkedList();
                            LinkedList frameid = new LinkedList();
                            while (runtime != null) {
                                long son = runtime.son_refer;
                                i++;
                                if (son == 0) {
                                    runtime = null;
                                } else {
                                    runtime = new MemRuntime(son);
                                    System.out.println(
                                            "method=" + Long.toString(runtime.method_refer, 16)
                                            + ",class=" + Long.toString(runtime.clazz_refer, 16)
                                            + ",pc=" + Long.toString(runtime.pc_refer, 16)
                                            + ",ca=" + Long.toString(runtime.code_refer, 16)
                                            + ",son=" + Long.toString(runtime.son_refer, 16)
                                            + ",idx=" + Long.toString(runtime.pc_refer - runtime.code_refer, 16)
                                    );
                                    Location loc = new Location();
                                    loc.typeTag = TypeTag.CLASS;
                                    loc.classID = runtime.clazz_refer;
                                    loc.methodID = runtime.method_refer;
                                    loc.execIndex = runtime.pc_refer - runtime.code_refer;
                                    list.add(loc);
                                    frameid.add(runtime.pc_refer);
                                    //System.out.println("ThreadReference_Frames:" + runtime + ", " + loc.execIndex);
                                }
                            }
                            res.writeInt(list.size());
                            while (!list.isEmpty()) {
                                Long fid = (Long) frameid.removeLast();
                                res.writeInt((int) fid.longValue());
                                Location loc = (Location) list.removeLast();
                                loc.writeLocation(res);
                            }
                            session.send(res.toByteArray());
                            JvmThreads.resumeThread(t);
                            break;
                        }
                        case Command.ThreadReference_FrameCount: {//7
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JvmThreads.getFrameCount(t));
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_OwnedMonitors: {//8
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeUTF(t.getName());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_CurrentContendedMonitor: {//9
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeRefer(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Stop: {//10
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            long objid = req.readRefer();
                            JvmThreads.stopThread(t, objid);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Interrupt: {//11
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            t.interrupt();
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_SuspendCount: {//12
                            long thread = req.readRefer();
                            Thread t = (Thread) JdwpNative.referenceObj(thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JvmThreads.getSuspendCount(t));
                            session.send(res.toByteArray());
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ThreadGroupReference: {//set 12
                    switch (req.getCommand()) {
                        case Command.ThreadGroupReference_Name: {
                            break;
                        }
                        case Command.ThreadGroupReference_Parent: {
                            break;
                        }
                        case Command.ThreadGroupReference_Children: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ArrayReference: {//set 13
                    switch (req.getCommand()) {
                        case Command.ArrayReference_Length: {
                            break;
                        }
                        case Command.ArrayReference_GetValues: {
                            break;
                        }
                        case Command.ArrayReference_SetValues: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ClassLoaderReference: {//set 14
                    switch (req.getCommand()) {
                        case Command.ClassLoaderReference_VisibleClasses: {
                            break;
                        }

                    }
                    break;
                }
                case CommandSet.EventRequest: {//set 15
                    switch (req.getCommand()) {
                        case Command.EventRequest_Set: {

                            byte eventKind = req.readByte();
                            byte suspend = req.readByte();
                            int numModifiers = req.readInt();
                            //System.out.println("eventKind=" + eventKind);
                            for (int i = 0; i < numModifiers; i++) {
                                int mod = req.readByte();
                                System.out.println("15_1:" + mod);
                                switch (mod) {
                                    case 1: {
                                        int count = req.readInt();
                                        break;
                                    }
                                    case 2: {
                                        int exprId = req.readInt();
                                        break;
                                    }
                                    case 3: {
                                        long threadID = req.readRefer();
                                        break;
                                    }
                                    case 4: {
                                        long clazz = req.readRefer();
                                        break;
                                    }
                                    case 5: {
                                        String classPattern = req.readUTF();
                                        break;
                                    }
                                    case 6: {
                                        String classPattern = req.readUTF();
                                        break;
                                    }
                                    case 7: {
                                        Location location = new Location();
                                        location.typeTag = req.readByte();
                                        location.classID = req.readRefer();
                                        location.methodID = req.readRefer();
                                        location.execIndex = req.readRefer();
                                        break;
                                    }
                                    case 8: {
                                        long referenceTypeID = req.readRefer();
                                        boolean caught = req.readBoolean();
                                        boolean uncaught = req.readBoolean();
                                        break;
                                    }
                                    case 9: {
                                        long referenceTypeID = req.readRefer();
                                        long fieldID = req.readRefer();
                                        break;
                                    }
                                    case 10: {
                                        long threadId = req.readRefer();
                                        int size = req.readInt();
                                        int depth = req.readInt();
                                        break;
                                    }
                                    case 11: {
                                        long instance = req.readRefer();
                                        break;
                                    }
                                    case 12: {
                                        String sourceNamePattern = req.readUTF();
                                        break;
                                    }
                                    default: {
                                        break;
                                    }
                                }
                            }

                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            Random r = new Random();
                            res.setErrorCode(Error.NONE);
                            res.writeInt(Math.abs(r.nextInt()));
                            session.send(res.toByteArray());
                            break;
                        }//
                        case Command.EventRequest_Clear: {
                            break;
                        }
                        case Command.EventRequest_ClearAllBreakpoints: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.StackFrame: {//set 16
                    switch (req.getCommand()) {
                        case Command.StackFrame_GetValues: {
                            break;
                        }
                        case Command.StackFrame_SetValues: {
                            break;
                        }
                        case Command.StackFrame_ThisObject: {
                            break;
                        }
                        case Command.StackFrame_PopFrames: {
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.ClassObjectReference: {//set 17
                    switch (req.getCommand()) {
                        case Command.ClassObjectReference_ReflectedType: {
                            break;
                        }

                    }
                    break;
                }
                case CommandSet.Event: {//set 64
                    switch (req.getCommand()) {
                        case Command.Event_Composite: {
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
}
