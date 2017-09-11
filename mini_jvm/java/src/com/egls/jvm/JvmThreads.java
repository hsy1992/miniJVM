/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.egls.jvm;

import java.util.Vector;

/**
 *
 * @author gust
 */
public class JvmThreads {

    /**
     * 存放系统所有线程
     */
    static Vector threads = new Vector();

    public static void addThread(Thread t) {
        if (t != null) {
            threads.add(t);
        }
    }

    public static void removeThread(Thread t) {
        if (t != null) {
            threads.remove(t);
        }
    }

    public static Vector getThreads() {
        return threads;
    }
}
