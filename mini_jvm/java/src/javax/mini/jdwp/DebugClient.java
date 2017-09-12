/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import com.egls.jvm.JvmThreads;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Random;
import javax.mini.jdwp.analyzer.Location;
import javax.mini.jdwp.constant.Command;
import javax.mini.jdwp.constant.CommandSet;
import javax.mini.jdwp.constant.Error;
import javax.mini.jdwp.constant.SuspendStatus;
import javax.mini.jdwp.constant.ThreadStatus;
import javax.mini.jdwp.net.JdwpPacket;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.ResponsePacket;
import javax.mini.jdwp.net.Session;
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

                        case Command.VirtualMachine_Version: {
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            res.writeUTF("MINI_JVM");
                            res.writeInt(0);
                            res.writeInt(1);
                            res.writeUTF("0.1");
                            res.writeUTF("Mini jvm");
                            session.send(res.toByteArray());
                            System.out.println(res);
                            break;
                        }
                        case Command.VirtualMachine_ClassesBySignature: {
                            break;
                        }
                        case Command.VirtualMachine_AllClasses: {
                            break;
                        }
                        case Command.VirtualMachine_AllThreads: {
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            Hashtable threads = com.egls.jvm.JvmThreads.getThreads();
                            res.writeInt(threads.size());
                            for (Enumeration e = threads.keys(); e.hasMoreElements();) {
                                Thread t = (Thread) threads.get(e.nextElement());
                                res.writeRefer(t.hashCode());
                            }
                            session.send(res.toByteArray());
                            System.out.println(res);
                            break;
                        }
                        case Command.VirtualMachine_TopLevelThreadGroups: {
                            break;
                        }
                        case Command.VirtualMachine_Dispose: {
                            break;
                        }
                        case Command.VirtualMachine_IDSizes: {
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
                        case Command.VirtualMachine_Suspend: {
                            break;
                        }
                        case Command.VirtualMachine_Resume: {
                            break;
                        }
                        case Command.VirtualMachine_Exit: {
                            break;
                        }
                        case Command.VirtualMachine_CreateString: {
                            break;
                        }
                        case Command.VirtualMachine_Capabilities: {
                            break;
                        }
                        case Command.VirtualMachine_ClassPaths: {
                            break;
                        }
                        case Command.VirtualMachine_DisposeObjects: {
                            break;
                        }
                        case Command.VirtualMachine_HoldEvents: {
                            break;
                        }
                        case Command.VirtualMachine_ReleaseEvents: {
                            break;
                        }
                        case Command.VirtualMachine_CapabilitiesNew: {
                            break;
                        }
                        case Command.VirtualMachine_RedefineClasses: {
                            break;
                        }
                        case Command.VirtualMachine_SetDefaultStratum: {
                            break;
                        }
                        case Command.VirtualMachine_AllClassesWithGeneric: {
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
                        case Command.ThreadReference_Name: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeUTF(t.getName());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Suspend: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            JvmThreads.suspendThread(t);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Resume: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            JvmThreads.resumeThread(t);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Status: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JvmThreads.getStatus(t));
                            res.writeInt(SuspendStatus.SUSPEND_STATUS_SUSPENDED);
                            session.send(res.toByteArray());
                            break;
                        }

                        case Command.ThreadReference_ThreadGroup: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Frames: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeUTF(t.getName());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_FrameCount: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeInt(JvmThreads.getFrameCount(t));
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_OwnedMonitors: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeUTF(t.getName());
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_CurrentContendedMonitor: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            res.writeRefer(0);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Stop: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            long objid = req.readRefer();
                            JvmThreads.stopThread(t, objid);
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_Interrupt: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
                            t.interrupt();
                            ResponsePacket res = new ResponsePacket();
                            res.setId(req.getId());
                            res.setErrorCode(Error.NONE);
                            session.send(res.toByteArray());
                            break;
                        }
                        case Command.ThreadReference_SuspendCount: {
                            long thread = req.readRefer();
                            Thread t = JvmThreads.getThread((int) thread);
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
                            System.out.println("eventKind=" + eventKind);
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
