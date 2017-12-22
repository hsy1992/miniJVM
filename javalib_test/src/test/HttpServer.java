/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package test;

import java.io.IOException;
import java.util.Calendar;
import javax.cldc.io.Connector;
import javax.mini.net.ServerSocket;
import javax.mini.net.Socket;

/**
 *
 * @author gust
 */
public class HttpServer {

    public static void main(String args[]) {
        HttpServer f = new HttpServer();
        f.t12();
        f.t13();
    }

    void t12() {

        new Thread(new Runnable() {

            @Override
            public void run() {
                try {
                    ServerSocket srvsock = (ServerSocket) Connector.open("serversocket://:8080");

                    //建一个线程，过5秒钟关掉自己
                    new Thread(new Runnable() {

                        @Override
                        public void run() {
                            try {
                                int MAX = 5;
                                for (int i = 0; i < MAX; i++) {
                                    System.out.println("server would close at " + (MAX - i) + " second later.");
                                    Thread.sleep(1000);
                                }
                                if (srvsock != null) {
                                    srvsock.close();
                                }
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }

                    }).start();
                    System.out.println("server socket listen...");
                    srvsock.listen();
                    while (true) {
                        try {
                            Socket cltsock;
                            try {
                                cltsock = srvsock.accept();
                            } catch (IOException e) {
                                break;
                            }
                            cltsock.setOption(Socket.OP_TYPE_NON_BLOCK, Socket.OP_VAL_NON_BLOCK);
                            System.out.println("accepted client socket:" + cltsock);
                            byte[] buf = new byte[256];
                            StringBuffer tmps = new StringBuffer();
                            int rlen;
                            while ((rlen = cltsock.read(buf, 0, 256)) != -1) {
                                String s = new String(buf, 0, rlen);
                                tmps.append(s);
                                String s1 = tmps.toString();
                                if (s1.indexOf("\n\n") >= 0 || s1.indexOf("\r\n\r\n") >= 0) {
                                    break;
                                }
                            }
                            //System.out.println("RECV: " + tmps.toString());
                            String sbuf = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\nFor mini_jvm test. ( EGLS Beijing co.,ltd)" + Calendar.getInstance().getTime().toString();
                            int sent = 0;
                            while ((sent) < sbuf.length()) {
                                int wlen = cltsock.write(sbuf.getBytes(), sent, sbuf.length() - sent);
                                if (wlen == -1) {
                                    break;
                                }
                                sent += wlen;
                            }
                            cltsock.close();
                            if (false) {
                                break;
                            }
                        } catch (Exception e) {
                            System.out.println(e);
                        }
                    }
                    srvsock.close();
                } catch (Exception e) {
                    System.out.println(e);
                }
            }
        }).start();

    }

    void t13() {
        for (int j = 0; j < 3; j++) {
            
            try {
                Thread.sleep(2000);
                
                Socket conn = (Socket) Connector.open("socket://127.0.0.1:8080");
                conn.setOption(Socket.OP_TYPE_NON_BLOCK, Socket.OP_VAL_NON_BLOCK);
                String request = "GET / HTTP/1.1\r\n\r\n";
                conn.write(request.getBytes(), 0, request.length());
                byte[] rcvbuf = new byte[256];
                int len = 0;
                int zero = 0;
                while (len != -1) {
                    len = conn.read(rcvbuf, 0, 256);
                    if (len == 0) {
                        zero++;
                    }
                    if (zero > 3000000) {
                        break;
                    }
                    for (int i = 0; i < len; i++) {
                        System.out.print((char) rcvbuf[i]);
                    }

                };
                System.out.print("\nend\n");
                conn.close();
            } catch (Exception e) {

            }
        }
    }
}
