/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import java.util.Enumeration;
import java.util.Hashtable;
import javax.mini.util.Iterator;

/**
 *
 * @author gust
 */
public class EventManager {

    static Hashtable events = new Hashtable();
    static int requestId = 100;
    static int commandId = 0;

    public static void action() {
        for (Enumeration e = events.elements(); e.hasMoreElements();) {
            ReqEvent event = (ReqEvent) e.nextElement();
            event.process();
        }
    }

    public static int getRequestId() {
        return requestId++;
    }

    static public int getCommandId() {
        return commandId++;
    }

    static public void putEvent(ReqEvent event) {
        events.put(event.getRequestId(), event);
    }

    static public ReqEvent getEvent(int reqId) {
        return (ReqEvent) events.get(reqId);
    }
}
