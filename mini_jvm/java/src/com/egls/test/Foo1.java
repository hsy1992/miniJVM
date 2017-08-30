/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.egls.test;

import java.lang.System;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

/**
 *
 * @author gust
 */
public class Foo1 {

    FInterface fi;

    interface FInterface {

        void print();
    }

    class P implements FInterface {

        @Override
        public void print() {
            System.out.println("P.print()");
        }

        public void x1() {
            System.out.println("P.x1()");
        }
    }

    class Q extends P {

        public void print() {
            System.out.println("Q.print()");
        }
    }

    void t1() {
        fi = new Q();
        fi.print();
        ((P) fi).x1();
        int a = Math.abs(-275);
        java.lang.System.out.println("a=" + a);
    }

    void t2() {
        String show = "test hashtable";
        System.out.println(show + ":" + show.hashCode());

        Integer itg = new Integer(0xf8f8f8);
        System.out.println(itg + ":" + itg.hashCode());

        Hashtable htable = new Hashtable();
        htable.put("I", itg);
        htable.put("J", new Long(0x2222222211111111L));
        htable.put("S", new String("SSS"));
        htable.put("L", new Object());
        for (Enumeration e = htable.keys(); e.hasMoreElements();) {
            String s = (String) e.nextElement();
            Object obj = htable.get(s);
            java.lang.System.out.println(s + " : " + obj);
        }
    }

    void t3() {

    }

    void t4() {
        Vector v = new Vector();
        v.addElement(new Object());
        v.addElement(new String("((((((((((("));
        v.addElement(new Double(-5.737373f));
        v.addElement(new Byte((byte) 0xff));
        for (int i = 0; i < v.size(); i++) {
            System.out.println("v[" + i + "]=" + v.elementAt(i));
        }
    }

    void t5() {
        try {
            String s = Double.toString(-5.737373f);
            System.out.println("d=" + s);
            int[] a = new int[10];
            System.out.println("aaa" + 999 + "," + 3.4f + "," + a);
        } catch (Exception e) {
            //System.out.println("error");
        }
    }

    void t6() {
        System.out.println("test start");
        {
            int i1 = 0xffffffff;
            int i2 = 0x80808080;
            System.out.println("iand " + ((i1 & i2) != 0x80808080 ? " error" : " ok"));
            System.out.println("ior " + ((i1 | i2) != 0xffffffff ? " error" : " ok"));
            System.out.println("ixor " + ((i1 ^ i2) != 0x7f7f7f7f ? " error" : " ok"));
            System.out.println("ishl " + ((i1 << 8) != 0xffffff00 ? " error" : " ok"));
            System.out.println("ishr " + ((i1 >> 8) != 0xffffffff ? " error" : " ok"));
            System.out.println("iushr " + ((i1 >>> 8) != 0x00ffffff ? " error" : " ok"));
            i1 = 0x24242424;
            i2 = 0x12121212;
            System.out.println("iadd " + ((i1 + i2) != 0x36363636 ? " error" : " ok"));
            System.out.println("isub " + ((i1 - i2) != 0x12121212 ? " error" : " ok"));
            System.out.println("imul " + ((i1 * i2) != 0x279d1288 ? " error" : " ok"));
            System.out.println("idiv " + ((i1 / i2) != 0x2 ? " error" : " ok"));
            System.out.println("irem " + ((i1 % i2) != 0x0 ? " error" : " ok"));
            System.out.println("ineg " + ((-i1) != 0xdbdbdbdc ? " error" : " ok"));
            int i3 = 100;
            i3 += 1000;
            System.out.println("iinc " + ((i3) != 1100 ? " error" : " ok"));

        }
        {
            long j1 = 0xffffffffffffffffL;
            long j2 = 0x8080808080808080L;
            System.out.println("land " + ((j1 & j2) != 0x8080808080808080L ? " error" : " ok"));
            System.out.println("lor " + ((j1 | j2) != 0xffffffffffffffffL ? " error" : " ok"));
            System.out.println("lxor " + ((j1 ^ j2) != 0x7f7f7f7f7f7f7f7fL ? " error" : " ok"));
            System.out.println("lshl " + ((j1 << 8) != 0xffffffffffffff00L ? " error" : " ok"));
            System.out.println("lshr " + ((j1 >> 8) != 0xffffffffffffffffL ? " error" : " ok"));
            System.out.println("lushr " + ((j1 >>> 8) != 0x00ffffffffffffffL ? " error" : " ok"));
            j1 = 0x2424242424242424L;
            j2 = 0x1212121212121212L;
            System.out.println("ladd " + ((j1 + j2) != 0x3636363636363636L ? " error" : " ok"));
            System.out.println("lsub " + ((j1 - j2) != 0x1212121212121212L ? " error" : " ok"));
            System.out.println("lmul " + ((j1 * j2) != 0x51c73cb2279d1288L ? " error" : " ok"));
            System.out.println("ldiv " + ((j1 / j2) != 0x2L ? " error" : " ok"));
            System.out.println("lrem " + ((j1 % j2) != 0x0L ? " error" : " ok"));
            System.out.println("lneg " + ((-j1) != 0xdbdbdbdbdbdbdbdcL ? " error" : " ok"));
        }

        {
            double d1 = 1.2121212f;
            double d2 = 1.2121212f;
            System.out.println("dadd " + (d1 + d2) + ((d1 + d2) != 2.4242424f ? " error" : " ok"));
            System.out.println("dsub " + (d1 - d2) + ((d1 - d2) != 0.0f ? " error" : " ok"));
            System.out.println("dmul " + (d1 * d2) + ((d1 * d2) != 1.469237920447597f ? " error" : " ok"));
            System.out.println("ddiv " + (d1 / d2) + ((d1 / d2) != 1.0f ? " error" : " ok"));
            System.out.println("drem " + (d1 % d2) + ((d1 % d2) != 0.0f ? " error" : " ok"));
            System.out.println("dneg " + (-d1) + ((-d1) != -1.2121212f ? " error" : " ok"));
            System.out.println("dcmpl " + (d1 > d2) + ((d1 > d2) ? " true" : " false"));
            System.out.println("dcmpg " + (d1 < d2) + ((d1 < d2) ? " true" : " false"));
//            System.out.println("dneg " + Double.toString(-d1) + ((-d1) != -1.2121212f ? " error" : " ok"));
        }
    }

    void t7() {
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                System.out.println("thread here.");
                int j = 0;
                while (true) {
                    try {
                        Thread.sleep(1000);
                        System.out.println(this + "j=" + j++);

                    } catch (InterruptedException ex) {
                    }
                }
            }
        });
        t.start();
        //
        int i = 0;
        while (i++<10) {
            try {
                Thread.sleep(300);
                System.out.println("total mem:" + Runtime.getRuntime().totalMemory()
                        + "   free: " + Runtime.getRuntime().freeMemory());

            } catch (InterruptedException ex) {
            }
        }
    }

    long t81(long l) {
        return l;
    }

    void t8() {
        System.out.println("total mem:" + Runtime.getRuntime().totalMemory()
                + "   free: " + Runtime.getRuntime().freeMemory());
        System.out.println(Long.toString(t81(0x1000000120000002L), 16));
        Long.toString(t81(0x1000000120000002L), 16);
    }

    void t9() {
        double d1 = 1.2121212f;
        double d2 = 1.2121212f;
        double r = d1 + d2;
//        Double.toString(r);
        System.out.println("dadd " + Double.toString(d1 + d2));

    }

    public static void main(String[] agrs) {
        Foo1 f = new Foo1();
        f.t1();
        f.t2();
        f.t3();
        f.t4();
        f.t5();
        f.t6();
        f.t7();
        f.t8();
        f.t9();
        BpDeepTest.main(agrs);
        Foo2.main(agrs);
        Foo3.main(agrs);
    }
}
