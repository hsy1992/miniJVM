/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import javax.mini.jdwp.constant.Error;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.Session;

/**
 *
 * @author gust
 */
public class EventSet_MONITOR_WAITED extends EventSet {

    public EventSet_MONITOR_WAITED(RequestPacket req) {
        super(req);
    }

    public short process() {
        for (Mod mod : mods) {

        }
        return Error.NONE;
    }

    public short clear() {
        return Error.NONE;
    }

    @Override
    public void postEvent(Event event, Session session) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
}
