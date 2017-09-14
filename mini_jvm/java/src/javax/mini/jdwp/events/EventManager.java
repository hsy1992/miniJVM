/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import java.util.Hashtable;

/**
 *
 * @author gust
 */
public class EventManager {

    Hashtable events = new Hashtable();
    static int requestId;

    public static int getRequestId() {
        return requestId++;
    }

    public void putEvent(ReqEvent event) {
        events.put(event.getRequestId(), event);
    }

    public ReqEvent getEvent(int reqId) {
        return (ReqEvent) events.get(reqId);
    }
}
