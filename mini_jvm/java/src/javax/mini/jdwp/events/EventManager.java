/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import java.util.Enumeration;
import java.util.Hashtable;
import javax.mini.util.LinkedList;

/**
 *
 * @author gust
 */
public class EventManager {

    static Hashtable eventSet = new Hashtable();
    static int requestId = 100;
    static int commandId = 0;
    static LinkedList events = new LinkedList();

    public static void action() {
        for (Enumeration e = eventSet.elements(); e.hasMoreElements();) {
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

    static public void putEventSet(ReqEvent event) {
        eventSet.put(event.getRequestId(), event);
    }

    static public ReqEvent getEventSet(int eventKind) {
        return (ReqEvent) events.get(eventKind);
    }

    static public void putEvent(Event e) {
        synchronized (events) {
            events.add(e);
        }
    }

    static public Event getEvent() {
        Event e;
        synchronized (events) {
            e = (Event) events.removeFirst();
        }
        return e;
    }
}
