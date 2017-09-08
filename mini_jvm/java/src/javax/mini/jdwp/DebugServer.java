/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import javax.cldc.io.Connector;
import javax.mini.net.ServerSocket;
import javax.mini.net.Socket;
import javax.mini.util.ArrayList;
import javax.mini.util.Iterator;
import javax.mini.util.List;

/**
 *
 * @author gust
 */
public class DebugServer {

    static int SERVER_PORT = 8000;
    List clients = new ArrayList();
    Listener listener;
    Dispacher dispacher;
    boolean exit = false;
    ServerSocket servSock;

    public void startServer() {
        listener = new Listener();
        listener.start();
        dispacher = new Dispacher();
        dispacher.start();
    }

    class Listener extends Thread {

        public void run() {
            try {
                servSock = (ServerSocket) Connector.open("serversocket://:" + SERVER_PORT, Connector.READ_WRITE);
                System.out.println("jdwp listening in " + SERVER_PORT);
            } catch (Exception e) {
                System.out.println(e);
            }
            while (!exit) {
                try {
                    Socket sock = servSock.accept();
                    DebugClient dc = new DebugClient(sock);
                    clients.add(dc);
                } catch (Exception e) {
                    exit=true;
                    System.out.println(e);
                }
            }
        }
    }

    class Dispacher extends Thread {

        public void run() {
            while (!exit) {
                try {
                    for (Iterator it = clients.iterator(); it.hasNext();) {
                        DebugClient dc = (DebugClient) it.next();
                        dc.process();
                        if (dc.isClosed()) {
                            it.remove();
                        }
                    }
                } catch (Exception e) {
                    System.out.println(e);
                }
            }
        }
    }
}
