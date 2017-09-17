/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import javax.mini.jdwp.constant.ClassStatus;
import javax.mini.jdwp.constant.Command;
import javax.mini.jdwp.constant.CommandSet;
import javax.mini.jdwp.constant.Error;
import javax.mini.jdwp.constant.TypeTag;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.Session;
import javax.mini.jdwp.reflect.Reference;
import javax.mini.jdwp.vm.JdwpNative;

/**
 * 断点是一个开关型的，一次设一次取消
 *
 * @author gust
 */
public class EventSet_BREAKPOINT extends EventSet {

    static int SET = 1;
    static int CLEAR = 0;

    public EventSet_BREAKPOINT(RequestPacket req, byte eventKind) {
        super(req, eventKind);
    }

    public short process() {
        short ret = Error.NONE;
        for (Mod m : mods) {
            if (m instanceof ModLocationOnly) {
                ModLocationOnly modLoc = (ModLocationOnly) m;
                ret = (short) JdwpNative.setBreakPoint(SET, modLoc.loc.typeTag, modLoc.loc.classID, modLoc.loc.methodID, modLoc.loc.execIndex);
            }
        }
        return ret;
    }

    public short clear() {
        short ret = Error.NONE;
        for (Mod m : mods) {
            if (m instanceof ModLocationOnly) {
                ModLocationOnly modLoc = (ModLocationOnly) m;
                ret = (short) JdwpNative.setBreakPoint(CLEAR, modLoc.loc.typeTag, modLoc.loc.classID, modLoc.loc.methodID, modLoc.loc.execIndex);
            }
        }
        return ret;
    }

    @Override
    public void postEvent(Event event, Session session) {
        for (Mod mod : mods) {
            System.out.println("breakpoint1," + mod + " , mod instanceof ModLocationOnly=" + (mod instanceof ModLocationOnly));
            if (mod instanceof ModLocationOnly) {
                ModLocationOnly m = (ModLocationOnly) mod;
                System.out.println("breakpoint2," + m);
                System.out.println("m.loc="+m.loc);
                System.out.println("e.loc="+event.loc);
                if (m.loc.equals(event.loc)) {
                    System.out.println("breakpoint3," + event.loc);
                    RequestPacket req = new RequestPacket();
                    req.setId(EventManager.getCommandId());
                    req.setCommandSet(CommandSet.Event);
                    req.setCommand(Command.Event_Composite);

                    req.writeByte(suspendPolicy);
                    req.writeInt(1);
                    req.writeByte(eventKind);
                    req.writeInt(requestId);

                    req.writeRefer(event.thread);
                    event.loc.writeLocation(req);
                    session.send(req.toByteArray());
                    System.out.println("event breakpoint sent.");
                }
            }
        }
    }
}
