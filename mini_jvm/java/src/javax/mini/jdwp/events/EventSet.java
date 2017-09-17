/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import javax.mini.jdwp.net.RequestPacket;
import javax.mini.jdwp.net.Session;

/**
 *
 * @author gust
 */
abstract public class EventSet {

    public int requestId = EventManager.getRequestId();
    public byte eventKind;
    public byte suspendPolicy;
    public byte kindMod;
    public int modifiers;
    Mod[] mods;

    public EventSet(RequestPacket req, byte eventKind) {
        this.eventKind = eventKind;
        suspendPolicy = req.readByte();
        modifiers = req.readInt();
        mods = new Mod[modifiers];
        System.out.print("EventRequest_Set:reqEventId:" + getRequestId() + ", kind=" + eventKind + ", modifiers=" + modifiers);
        for (int i = 0; i < modifiers; i++) {
            byte modi = req.readByte();
            System.out.println(",mod=" + modi + "\n");
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

    /**
     * 向debug客户端发送虚拟机的事件
     *
     * @param event
     */
    abstract public void postEvent(Event event, Session session);

    /**
     * 对jvm进行设置set
     *
     * @param event
     */
    abstract public short process();

    /**
     * 清除jvm set的内容
     *
     * @return
     */
    abstract public short clear();
//    {
//        for (Mod mod : mods) {
//            switch (eventKind) {
//                case EventKind.VM_DISCONNECTED: {
//                    break;
//                }
//                case EventKind.VM_START: {
//                    break;
//                }
//                case EventKind.THREAD_DEATH: {
//                    break;
//                }
//                case EventKind.SINGLE_STEP: {
//                    break;
//                }
//                case EventKind.BREAKPOINT: {
//                    for (Mod m : mods) {
//                        if (m instanceof ModLocationOnly) {
//                            ModLocationOnly modLoc = (ModLocationOnly) m;
//                            JdwpNative.setBreakPoint(modLoc.loc.typeTag, modLoc.loc.classID, modLoc.loc.methodID, modLoc.loc.execIndex);
//                        }
//                    }
//                    break;
//                }
//                case EventKind.FRAME_POP: {
//                    break;
//                }
//                case EventKind.EXCEPTION: {
//                    break;
//                }
//                case EventKind.USER_DEFINED: {
//                    break;
//                }
//                case EventKind.THREAD_START: {
//                    break;
//                }
//                case EventKind.CLASS_PREPARE: {
////                    Reference ref = JdwpManager.getReference(refId);
////                    RequestPacket resEvent = new RequestPacket();
////                    resEvent.setId(requestId);
////                    resEvent.setCommandSet(CommandSet.Event);
////                    resEvent.setCommand(Command.Event_Composite);
////                    if (ref != null) {
////                        resEvent.writeByte(0);
////                        resEvent.writeInt(1);
////                        resEvent.writeByte(eventKind);
////                        resEvent.writeInt(requestId);
////                        resEvent.writeRefer(JdwpNative.referenceId(Thread.currentThread()));
////                        resEvent.writeByte(TypeTag.CLASS);
////                        resEvent.writeRefer(ref.classId);
////                        resEvent.writeUTF("L" + ref.className + ";");
////                        resEvent.writeInt(ClassStatus.INITIALIZED);
////                    }
//
//                    break;
//                }
//                case EventKind.CLASS_UNLOAD: {
//                    break;
//                }
//                case EventKind.CLASS_LOAD: {
//                    break;
//                }
//                case EventKind.FIELD_ACCESS: {
//                    break;
//                }
//                case EventKind.FIELD_MODIFICATION: {
//                    break;
//                }
//                case EventKind.EXCEPTION_CATCH: {
//                    break;
//                }
//                case EventKind.METHOD_ENTRY: {
//                    break;
//                }
//                case EventKind.METHOD_EXIT: {
//                    break;
//                }
//                case EventKind.VM_DEATH: {
//                    break;
//                }
//
//                default: {
//                    break;
//                }
//            }
//        }
//        return Error.NONE;
//    }

}
