/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import javax.mini.jdwp.JdwpManager;
import javax.mini.jdwp.constant.ClassStatus;
import javax.mini.jdwp.constant.Command;
import javax.mini.jdwp.constant.CommandSet;
import javax.mini.jdwp.constant.EventKind;
import javax.mini.jdwp.constant.TypeTag;
import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.reflect.Reference;
import javax.mini.jdwp.vm.JdwpNative;

/**
 *
 * @author gust
 */
public class EventSet {

    private int requestId = EventManager.getRequestId();
    //private int eventId=EventManager.getCommandId();
    byte eventKind;
    byte suspendPolicy;
    byte kindMod;
    int modifiers;
    Mod[] mods;

    public EventSet(RequestPacket req) {
        eventKind = req.readByte();
        suspendPolicy = req.readByte();
        modifiers = req.readInt();
        mods = new Mod[modifiers];
        System.out.println("EventRequest_Set:reqEventId:" + getRequestId() + ", kind=" + eventKind + ", susp=" + suspendPolicy);
        for (int i = 0; i < modifiers; i++) {
            byte modi = req.readByte();
            System.out.println("EventRequest_Set:mod=" + modi);
            Mod mod = null;
            switch (modi) {
                case 1:
                    mod = new ModCount(req);
                    break;
                case 2:
                    mod = new ModConditional(req);
                    break;
                case 3:
                    mod = new ModThreadOnly(req);
                    break;
                case 4:
                    mod = new ModClassOnly(req);
                    break;
                case 5:
                    mod = new ModClassMatch(req);
                    break;
                case 6:
                    mod = new ModClassExclude(req);
                    break;
                case 7:
                    mod = new ModLocationOnly(req);
                    break;
                case 8:
                    mod = new ModExceptionOnly(req);
                    break;
                case 9:
                    mod = new ModFieldOnly(req);
                    break;
                case 10:
                    mod = new ModStep(req);
                    break;
                case 11:
                    mod = new ModInstanceOnly(req);
                    break;
                case 12:
                    mod = new ModSourceNameMatch(req);
                    break;
            }
            mods[i] = mod;
        }
    }

    /**
     * @return the requestId
     */
    public int getRequestId() {
        return requestId;
    }

    public void process() {
        for (Mod mod : mods) {
            switch (eventKind) {
                case EventKind.VM_DISCONNECTED: {
                    break;
                }
                case EventKind.VM_START: {
                    break;
                }
                case EventKind.THREAD_DEATH: {
                    break;
                }
                case EventKind.SINGLE_STEP: {
                    break;
                }
                case EventKind.BREAKPOINT: {
                    break;
                }
                case EventKind.FRAME_POP: {
                    break;
                }
                case EventKind.EXCEPTION: {
                    break;
                }
                case EventKind.USER_DEFINED: {
                    break;
                }
                case EventKind.THREAD_START: {
                    break;
                }
                case EventKind.CLASS_PREPARE: {
//                    Reference ref = JdwpManager.getReference(refId);
//                    RequestPacket resEvent = new RequestPacket();
//                    resEvent.setId(requestId);
//                    resEvent.setCommandSet(CommandSet.Event);
//                    resEvent.setCommand(Command.Event_Composite);
//                    if (ref != null) {
//                        resEvent.writeByte(0);
//                        resEvent.writeInt(1);
//                        resEvent.writeByte(eventKind);
//                        resEvent.writeInt(requestId);
//                        resEvent.writeRefer(JdwpNative.referenceId(Thread.currentThread()));
//                        resEvent.writeByte(TypeTag.CLASS);
//                        resEvent.writeRefer(ref.classId);
//                        resEvent.writeUTF("L" + ref.className + ";");
//                        resEvent.writeInt(ClassStatus.INITIALIZED);
//                    }

                    break;
                }
                case EventKind.CLASS_UNLOAD: {
                    break;
                }
                case EventKind.CLASS_LOAD: {
                    break;
                }
                case EventKind.FIELD_ACCESS: {
                    break;
                }
                case EventKind.FIELD_MODIFICATION: {
                    break;
                }
                case EventKind.EXCEPTION_CATCH: {
                    break;
                }
                case EventKind.METHOD_ENTRY: {
                    break;
                }
                case EventKind.METHOD_EXIT: {
                    break;
                }
                case EventKind.VM_DEATH: {
                    break;
                }

                default: {
                    break;
                }
            }
        }
    }

}
