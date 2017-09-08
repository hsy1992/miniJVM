/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

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
            }
            //System.out.println("client process.");
        } catch (Exception e) {
            closed = true;
            System.out.println(e);
        }
    }

    public boolean isClosed() {
        return closed;
    }
}
