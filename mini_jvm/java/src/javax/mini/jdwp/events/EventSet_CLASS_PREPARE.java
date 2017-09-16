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
 *
 * @author gust
 */
public class EventSet_CLASS_PREPARE extends EventSet {

    public EventSet_CLASS_PREPARE(RequestPacket req) {
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
    public void postEvent(Event event,Session session) {
        for (Mod mod : mods) {
            if (mod instanceof ModClassMatch) {
                ModClassMatch m = (ModClassMatch) mod;
                if (event.signature.equals(m.classPattern)) {
                    Class cl=JdwpNative.getClassByName(m.classPattern);
                    Reference ref = new Reference(JdwpNative.referenceId(cl));
                    RequestPacket req = new RequestPacket();
                    req.setId(EventManager.getCommandId());
                    req.setCommandSet(CommandSet.Event);
                    req.setCommand(Command.Event_Composite);
                    if (ref != null) {
                        req.writeByte(suspendPolicy);
                        req.writeInt(1);
                        req.writeByte(eventKind);
                        req.writeInt(requestId);
                        req.writeRefer(event.thread);
                        req.writeByte(TypeTag.CLASS);
                        req.writeRefer(event.typeID);
                        req.writeUTF("L" + event.signature + ";");
                        req.writeInt(ClassStatus.INITIALIZED);
                        session.send(req.toByteArray());
                    }
                }
            }
        }
    }
}
