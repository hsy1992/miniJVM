/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp;

import java.util.Hashtable;
import javax.mini.jdwp.reflect.Reference;

/**
 *
 * @author gust
 */
public class JdwpManager {

    private static DebugServer server;
    //存放虚拟机内存中的一些对象
    static Hashtable reflectCache = new Hashtable();

    static public DebugServer startJdwp() {
        server = new DebugServer();
        server.startServer();
        return server;
    }

    /**
     * @return the server
     */
    public static DebugServer getServer() {
        return server;
    }

    public static Reference getReference(long refid) {
        Reference ref = (Reference) reflectCache.get(refid);
        if (ref == null) {
            ref = new Reference(refid);
            reflectCache.put(ref.classId, ref);
        }
        return ref;
    }
}
