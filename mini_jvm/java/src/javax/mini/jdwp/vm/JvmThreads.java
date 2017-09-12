/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.vm;

import java.util.Hashtable;
import java.util.Vector;

/**
 *
 * @author gust
 */
public class JvmThreads {

    static public native Thread[] getThreads();
    static public native int getStatus(Thread t);
    static public native int suspendThread(Thread t);
    static public native int resumeThread(Thread t);
    static public native int getSuspendCount(Thread t);
    static public native int getFrameCount(Thread t);
    static public native int stopThread(Thread t,long objid);
    static public native long getTopRuntime(Thread t);

}
