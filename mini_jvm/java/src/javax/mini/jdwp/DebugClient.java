/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import java.io.IOException;
import java.util.Enumeration;
import java.util.Random;
import java.util.Vector;
import javax.mini.jdwp.analyzer.Location;
import javax.mini.jdwp.constant.Command;
import javax.mini.jdwp.constant.CommandSet;
import javax.mini.jdwp.constant.Error;
import javax.mini.jdwp.net.JdwpPacket;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.ResponsePacket;
import javax.mini.jdwp.net.Session;
import javax.mini.jdwp.reflect.JdwpNative;
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
                Session.print(data);
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
                case CommandSet.VirtualMachine: {

                    switch (req.getCommand()) {
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
                        case Command.VirtualMachine_AllThreads: {
                            ResponsePacket res = new ResponsePacket();
                            res.setErrorCode(Error.NONE);
                            res.setId(req.getId());
                            Vector threads = com.egls.jvm.JvmThreads.getThreads();
                            res.writeInt(threads.size());
                            for (Enumeration e = threads.elements(); e.hasMoreElements();) {
                                Thread t = (Thread) e.nextElement();
                                res.writeRefer(t.hashCode());
                            }
                            session.send(res.toByteArray());
                            System.out.println(res);
                            break;
                        }
                    }
                    break;
                }
                case CommandSet.EventRequest: {
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
