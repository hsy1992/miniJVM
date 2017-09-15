/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import javax.mini.jdwp.analyzer.Location;

/**
 *
 * @author gust
 */
public class Event {

    byte eventKind;
    int requestId;

    //VM_START
    long thread;

    //SINGLE_STEP
    //long thread;
    Location loc;

    //BREAKPOINT
    //long thread;
    //Location loc;
    //
    //METHOD_ENTRY
    //long thread;
    //Location loc;
    //
    //METHOD_EXIT
    //long thread;
    //Location loc;
    //
    //METHOD_EXIT_WITH_RETURN_VALUE
    //long thread;
    //Location loc;
    byte tag;
    long value;

    //MONITOR_CONTENDED_ENTER
    //long thread;
    long object;
    //Location loc;

    //MONITOR_CONTENDED_ENTERED
    //long thread;
    //long object;
    //Location loc;
    //
    //MONITOR_WAIT
    //long thread;
    //long object;
    //Location loc;
    long timeout;

    //MONITOR_WAITED
    //long thread;
    //long object;
    //Location loc;
    boolean timed_out;

    //EXCEPTION
    //long thread;
    //long object;
    //Location loc;
    long exception;
    Location catchLoc;

    //THREAD_START
    //long thread;
    //
    //THREAD_DEATH
    //long thread;
    //
    //CLASS_PREPARE
    //long thread;
    byte refTypeTag;
    long typeID;
    String signature;
    int status;

    //CLASS_UNLOAD
    // String signature;
    //
    //FIELD_ACCESS
    //long thread;
    //Location loc;
    //byte refTypeTag;
    //long typeID;
    long fieldID;
    //long object;
    //
    //FIELD_MODIFICATION
    //long thread;
    //Location loc;
    //byte refTypeTag;
    //long typeID;
    //long fieldID;
    //long object;
    //byte tag;
    //long value;
    //
    //VM_DEATH
}
