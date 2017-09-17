/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import java.util.Enumeration;
import java.util.Hashtable;
import javax.mini.jdwp.constant.EventKind;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.Session;
import javax.mini.util.LinkedList;

/**
 *
 * @author gust
 */
public class EventManager {

    static Hashtable<Integer, EventSet> eventSets = new Hashtable();
    static int requestId = 100;
    static int commandId = 0;
    static LinkedList<Event> events = new LinkedList();

    public static void action(Session session) {
        Event event;
        while((event=getEvent())!=null){
            for(Enumeration<EventSet> e=eventSets.elements();e.hasMoreElements();){
                EventSet set=e.nextElement();
                if(set.eventKind==event.eventKind){
                    set.postEvent(event,session);
                    System.out.println("send event composiet :"+event);
                }
            }
        }
    }

    public static int getRequestId() {
        return requestId++;
    }

    static public int getCommandId() {
        return commandId++;
    }

    static public void putEventSet(EventSet event) {
        eventSets.put(event.getRequestId(), event);
    }

    static public void removeEventSet(int id) {
        eventSets.remove(id);
    }

    static public EventSet getEventSet(int eventKind) {
        return eventSets.get(eventKind);
    }

    static public void putEvent(Event e) {
        synchronized (events) {
            events.add(e);
        }
    }

    static public Event getEvent() {
        Event e;
        synchronized (events) {
            e = events.poll();
        }
        return e;
    }

    static public EventSet createEventSet(RequestPacket req) {
        byte eventKind = req.readByte();
        EventSet eventSet = null;
        switch (eventKind) {
            case EventKind.VM_DISCONNECTED: {
                eventSet = new EventSet_VM_DISCONNECTED(req, eventKind);
                break;
            }
            case EventKind.VM_START: {
                eventSet = new EventSet_VM_START(req, eventKind);
                break;
            }
            case EventKind.THREAD_DEATH: {
                eventSet = new EventSet_THREAD_DEATH(req, eventKind);
                break;
            }
            case EventKind.SINGLE_STEP: {
                eventSet = new EventSet_SINGLE_STEP(req, eventKind);
                break;
            }
            case EventKind.BREAKPOINT: {
                eventSet = new EventSet_BREAKPOINT(req, eventKind);
                break;
            }
            case EventKind.FRAME_POP: {
                eventSet = new EventSet_FRAME_POP(req, eventKind);
                break;
            }
            case EventKind.EXCEPTION: {
                eventSet = new EventSet_EXCEPTION(req, eventKind);
                break;
            }
            case EventKind.USER_DEFINED: {
                eventSet = new EventSet_USER_DEFINED(req, eventKind);
                break;
            }
            case EventKind.THREAD_START: {
                eventSet = new EventSet_THREAD_START(req, eventKind);
                break;
            }
            case EventKind.CLASS_PREPARE: {
                eventSet = new EventSet_CLASS_PREPARE(req, eventKind);
                break;
            }
            case EventKind.CLASS_UNLOAD: {
                eventSet = new EventSet_CLASS_UNLOAD(req, eventKind);
                break;
            }
            case EventKind.CLASS_LOAD: {
                eventSet = new EventSet_CLASS_LOAD(req, eventKind);
                break;
            }
            case EventKind.FIELD_ACCESS: {
                eventSet = new EventSet_FIELD_ACCESS(req, eventKind);
                break;
            }
            case EventKind.FIELD_MODIFICATION: {
                eventSet = new EventSet_FIELD_MODIFICATION(req, eventKind);
                break;
            }
            case EventKind.EXCEPTION_CATCH: {
                eventSet = new EventSet_EXCEPTION_CATCH(req, eventKind);
                break;
            }
            case EventKind.METHOD_ENTRY: {
                eventSet = new EventSet_METHOD_ENTRY(req, eventKind);
                break;
            }
            case EventKind.METHOD_EXIT: {
                eventSet = new EventSet_METHOD_EXIT(req, eventKind);
                break;
            }
            case EventKind.METHOD_EXIT_WITH_RETURN_VALUE: {
                eventSet = new EventSet_METHOD_EXIT_WITH_RETURN_VALUE(req, eventKind);
                break;
            }
            case EventKind.VM_DEATH: {
                eventSet = new EventSet_VM_DEATH(req, eventKind);
                break;
            }
            default: {
                eventSet = new EventSet_UNKNOW(req, eventKind);
                break;
            }
        }
        eventSet.eventKind = eventKind;
        return eventSet;
    }

//    static public short clearEvent(byte eventKind, int requestID) {
//        EventSet set = getEventSet(requestID);
//        EventManager.removeEventSet(requestID);
//        short ret = Error.NONE;
//        ret=set.clear();
//        return ret;
//    }
}
