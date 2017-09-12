/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.egls.jvm;

import java.util.Hashtable;
import java.util.Vector;

/**
 *
 * @author gust
 */
public class JvmThreads {

    /**
     * 存放系统所有线程
     */
    static Hashtable threads = new Hashtable();

    public static void addThread(Thread t) {
        if (t != null) {
            threads.put(t.hashCode(), t);
        }
    }

    public static void removeThread(Thread t) {
        if (t != null) {
            threads.remove(t);
        }
    }

    public static Thread getThread(int hashcode) {
        return (Thread) threads.get(hashcode);
    }

    public static Hashtable getThreads() {
        return threads;
    }

    static public native int getStatus(Thread t);
    static public native int suspendThread(Thread t);
    static public native int resumeThread(Thread t);
    static public native int getSuspendCount(Thread t);
    static public native int getFrameCount(Thread t);
    static public native int stopThread(Thread t,long objid);

}
