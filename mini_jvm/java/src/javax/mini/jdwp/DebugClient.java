/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import java.io.ByteArrayInputStream;
import java.io.DataInputStream;
import java.io.IOException;
import javax.mini.jdwp.analyzer.AnalyzerManager;
import javax.mini.jdwp.net.Packet;
import javax.mini.jdwp.analyzer.PacketAnalyzer;
import javax.mini.jdwp.net.Request;
import javax.mini.jdwp.net.Response;
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
    }

    public void process() {
        try {
            session.action();
            byte[] data;
            while ((data = session.getPkg()) != null) {
                Session.print(data);
                processPacket(data);
            }
            //System.out.println("client process.");
        } catch (Exception e) {
            closed = true;
            System.out.println(e);
        }
    }

    public void processPacket(byte[] data) throws IOException {
        DataInputStream dis = new DataInputStream(new ByteArrayInputStream(data));
        Packet packet = Packet.readPacket(dis, true);
        PacketAnalyzer packetAnalyzer = AnalyzerManager.createPacketAnalyzer(packet);

        if (packet instanceof Request) {
            packetAnalyzer.updateInternalDataModel(packet);
        }
        if (packet instanceof Response) {
            Response resp = (Response) packet;
            if (!resp.isError()) {
                packetAnalyzer.updateInternalDataModel(packet);
            }
        }
        System.out.println("packet :"+packetAnalyzer.getPacketInfo(packet));
    }

    public boolean isClosed() {
        return closed;
    }
}
