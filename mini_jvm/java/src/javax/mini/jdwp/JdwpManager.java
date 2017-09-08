/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

/**
 *
 * @author gust
 */
public class JdwpManager {
    static public DebugServer startJdwp(){
        DebugServer server=new DebugServer();
        server.startServer();
        return server;
    }
}
