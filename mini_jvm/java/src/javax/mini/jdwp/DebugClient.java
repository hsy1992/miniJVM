/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import java.io.IOException;
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
    }

    public void process() {
        try {
            session.action();
            byte[] data;
            while ((data = session.getPkg()) != null) {
                Session.print(data);
                processPacket(data);
            }
            System.out.println("client process.");
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
//            System.out.println(req);
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
                            session.putPkg(res.toByteArray());
//                            Session.print(res.toByteArray());
                            System.out.println(res);
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
