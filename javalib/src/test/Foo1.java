/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package test;

import com.sun.cldc.i18n.StreamReader;
import com.sun.cldc.i18n.StreamWriter;
import com.sun.cldc.i18n.mini.UTF_8_Reader;
import com.sun.cldc.i18n.mini.UTF_8_Writer;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Vector;
import javax.cldc.io.Connector;
import javax.cldc.io.ContentConnection;
import javax.mini.io.File;
import javax.mini.io.RandomAccessFile;
import javax.mini.net.ServerSocket;
import javax.mini.net.Socket;
import javax.mini.reflect.Method;
import javax.mini.reflect.Reference;
import javax.mini.reflect.StackFrame;
import javax.mini.reflect.vm.RefNative;

/**
 *
 * @author gust
 */
public class Foo1 {

    public FInterface fi = new Q();

    interface FInterface {

        void print();
    }

    interface SubInterface extends FInterface {

    }

    class P implements SubInterface {

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
            System.out.println("dmul " + (d1 * d2) + ((d1 * d2) < 1.46f ? " error" : " ok"));
            System.out.println("ddiv " + (d1 / d2) + ((d1 / d2) != 1.0f ? " error" : " ok"));
            System.out.println("drem " + (d1 % d2) + ((d1 % d2) != 0.0f ? " error" : " ok"));
            System.out.println("dneg " + (-d1) + ((-d1) != -1.2121212f ? " error" : " ok"));
            System.out.println("dcmpl " + (d1 > d2) + ((d1 > d2) ? " true" : " false"));
            System.out.println("dcmpg " + (d1 < d2) + ((d1 < d2) ? " true" : " false"));
//            System.out.println("dneg " + Double.toString(-d1) + ((-d1) != -1.2121212f ? " error" : " ok"));
        }
    }

    void t7() {
        int MAX = 10000;
        int PRINT_COUNT = 10000;
        Thread t = new Thread(new Runnable() {
            List<String> list = new ArrayList();
            Set<String> set = new HashSet();

            @Override
            public void run() {
                try {
                    System.out.println("total mem:" + Runtime.getRuntime().totalMemory()
                            + "   free: " + Runtime.getRuntime().freeMemory());

                } catch (Exception ex) {
                }

                System.out.println("thread here.");
                int j = 0;
                String c = null;
                for (int i = 0; i < MAX; i++) {
                    String a = "abc";
                    String b = "def";
                    c = a + b;
                    list.add(c);
                    set.add(c);
                    if (i % PRINT_COUNT == 0) {
                        System.out.println("thread i=" + i);
                    }
                }
                System.out.println("list.size():" + list.size() + "  ,set.size():" + set.size());
                System.out.println(" thread c=\"" + c + "\"");
            }
        });
        t.start();
        Thread t1 = new Thread(new Runnable() {
            List<String> list = new ArrayList();
            Set<String> set = new HashSet();

            @Override
            public void run() {
                try {
                    System.out.println("total mem:" + Runtime.getRuntime().totalMemory()
                            + "   free: " + Runtime.getRuntime().freeMemory());

                } catch (Exception ex) {
                }

                System.out.println("thread here.");
                int j = 0;
                String c = null;
                for (int i = 0; i < MAX; i++) {
                    String a = "abc";
                    String b = "def";
                    c = a + b;
                    list.add(c);
                    set.add(c);
                    if (i % PRINT_COUNT == 0) {
                        System.out.println("thread i=" + i);
                    }
                }
                System.out.println("list.size():" + list.size() + "  ,set.size():" + set.size());
                System.out.println(" thread c=\"" + c + "\"");
            }
        });
        //t1.start();
//        StackFrame sf = new StackFrame(RefNative.getStackFrame(Thread.currentThread()));
//        long sfid = sf.runtimeId;
//        System.out.println("sfid=" + Long.toString(sfid, 16));
//        while (sf.parent != null) {
//            System.out.println(sf);
//            sf = sf.parent;
//        }
        //
        String c = null;
        for (int i = 0; i < MAX; i++) {
            String a = "abc";
            String b = "def";
            c = a + b;
            if (i % PRINT_COUNT == 0) {
                System.out.println("main i=" + i);
            }
        }
        System.out.println("main c=\"" + c + "\"");
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

    void t10() {
        int i = 0;
        try {
            i = 1;
            if (true) {
                throw new NullPointerException("exception test 1");
            }
            i = 0;
        } catch (Exception e) {
            e.printStackTrace();
            i = 2;
        }
        System.out.println("i=" + i);
    }

    void t11() {
        String a = "0123456789";
        String b = a;
        a = a.substring(a.indexOf('2'), a.indexOf('7'));
//        a = a + "0123456789";
        System.out.println("" + a);
        System.out.println("" + a.charAt(1));
        System.out.println("" + a.indexOf('4', 1));
        String s1 = "";
        String s2 = "";
        System.out.println(s1.equals(s2));
    }

    void t12() {

        new Thread(new Runnable() {

            @Override
            public void run() {
                try {
                    ServerSocket srvsock = (ServerSocket) Connector.open("serversocket://:8080");

                    //建一个线程，过5秒钟关掉自己
                    new Thread(new Runnable() {

                        @Override
                        public void run() {
                            try {
                                int MAX = 3;
                                for (int i = 0; i < MAX; i++) {
                                    System.out.println("server would close at " + (MAX - i) + " second later.");
                                    Thread.sleep(1000);
                                }
                                if (srvsock != null) {
                                    srvsock.close();
                                }
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }

                    }).start();
                    System.out.println("server socket listen...");
                    srvsock.listen();
                    while (true) {
                        try {
                            Socket cltsock;
                            try {
                                cltsock = srvsock.accept();
                            } catch (IOException e) {
                                break;
                            }
                            cltsock.setOption(Socket.OP_TYPE_NON_BLOCK, Socket.OP_VAL_NON_BLOCK);
                            System.out.println("accepted client socket:" + cltsock);
                            byte[] buf = new byte[256];
                            StringBuffer tmps = new StringBuffer();
                            int rlen;
                            while ((rlen = cltsock.read(buf, 0, 256)) != -1) {
                                String s = new String(buf, 0, rlen);
                                tmps.append(s);
                                String s1 = tmps.toString();
                                if (s1.indexOf("\n\n") >= 0 || s1.indexOf("\r\n\r\n") >= 0) {
                                    break;
                                }
                            }
                            //System.out.println("RECV: " + tmps.toString());
                            String sbuf = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\nFor mini_jvm test. ( EGLS Beijing co.,ltd)" + Calendar.getInstance().getTime().toString();
                            int sent = 0;
                            while ((sent) < sbuf.length()) {
                                int wlen = cltsock.write(sbuf.getBytes(), sent, sbuf.length() - sent);
                                if (wlen == -1) {
                                    break;
                                }
                                sent += wlen;
                            }
                            cltsock.close();
                            if (false) {
                                break;
                            }
                        } catch (Exception e) {
                            System.out.println(e);
                        }
                    }
                    srvsock.close();
                } catch (Exception e) {
                    System.out.println(e);
                }
            }
        }).start();

    }

    void t13() {
        try {
            Socket conn = (Socket) Connector.open("socket://127.0.0.1:8080");
            conn.setOption(Socket.OP_TYPE_NON_BLOCK, Socket.OP_VAL_NON_BLOCK);
            String request = "GET / HTTP/1.1\r\n\r\n";
            conn.write(request.getBytes(), 0, request.length());
            byte[] rcvbuf = new byte[256];
            int len = 0;
            int zero = 0;
            while (len != -1) {
                len = conn.read(rcvbuf, 0, 256);
                if (len == 0) {
                    zero++;
                }
                if (zero > 300) {
                    break;
                }
                for (int i = 0; i < len; i++) {
                    System.out.print((char) rcvbuf[i]);
                }

            };
            System.out.print("\n");
            conn.close();
        } catch (Exception e) {

        }
    }

    void t14() {
        String sbuf = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\nFor mini_jvm test. ( EGLS Beijing co.,ltd)" + Calendar.getInstance().getTime().toString();
//        String sbuf = Calendar.getInstance().getTime().toString();
        System.out.println("s=" + sbuf);
    }

    void printString(String s) {
        for (int i = 0; i < s.length(); i++) {
            System.out.print(" " + Integer.toString((int) (s.charAt(i) & 0xffff)));
        }
        System.out.println();
    }

    void printBytes(String s) {
        try {
            byte[] barr = s.getBytes("utf-8");
            for (int i = 0; i < barr.length; i++) {
                System.out.print(" " + Integer.toHexString((int) (barr[i] & 0xff)));
            }
            System.out.println();
        } catch (Exception ex) {
            System.out.println(ex.getMessage());
        }
    }

    void t15() {

        try {
            String s = "这是一个测试";
            printBytes(s);
            printString(s);
            File test = new File("./a.txt");
            StreamWriter writer = new UTF_8_Writer();
            writer.open(test.getOutputStream(false), "utf-8");
            writer.write(s);
            writer.close();

            StreamReader reader = new UTF_8_Reader();
            reader.open(test.getInputStream(), "utf-8");
            char[] buf = new char[100];
            int len = reader.read(buf, 0, 100);
            reader.close();
            String r = new String(buf, 0, len);
            printString(r);

        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
        t15_1();
    }

    void t15_1() {
        File file = new File(".");
        System.out.println("isDir:" + file.isDirectory());
        String[] files = file.list();
        for (int i = 0; i < files.length; i++) {
            System.out.println(files[i]);
        }
    }

    void t16() {
        try {
            File b = new File("./b.txt");
            String r = "这是一个测试";
            printBytes(r);
            printString(r);
            DataOutputStream dos = new DataOutputStream(b.getOutputStream(true));
            dos.writeUTF(r);
            dos.close();
            DataInputStream dis = new DataInputStream(b.getInputStream());
            String s = dis.readUTF();
            printBytes(s);
            printString(s);
            dis.close();
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

    void t17() {
        try {
            RandomAccessFile c = new RandomAccessFile("./c.txt", "rw");
            c.seek(0);
            String r = "这是一个测试";
            printBytes(r);
            printString(r);
            byte[] carr = r.getBytes("utf-8");
            c.write(carr, 0, carr.length);
            c.close();
            RandomAccessFile c1 = new RandomAccessFile("./c.txt", "r");
            c1.seek(0);
            byte[] barr = new byte[256];
            int len;
            len = c1.read(barr, 0, 256);
            System.out.println("len=" + len);
            c1.close();
            String s = new String(barr, 0, len, "utf-8");
            printBytes(s);
            printString(s);
        } catch (IOException ex) {
            System.out.println(ex.getMessage());
        }
    }

    byte[] getViaContentConnection(String url) throws IOException {
        ContentConnection c = null;
        DataInputStream dis = null;
        byte[] data;
        try {
            System.out.println("url:" + url);
            c = (ContentConnection) Connector.open(url);
            int len = (int) c.getLength();
            dis = c.openDataInputStream();
            if (len > 0) {
                data = new byte[len];
                dis.readFully(data);
            } else {
                ByteArrayOutputStream baos = new ByteArrayOutputStream();
                int ch;
                while ((ch = dis.read()) != -1) {

                    baos.write(ch);
                }
                data = baos.toByteArray();
            }
        } finally {
            if (dis != null) {
                dis.close();
            }
            if (c != null) {
                c.close();
            }
        }
        return data;
    }

    void t18() {
        try {
            byte[] data = getViaContentConnection("http://baidu.com/");
            for (int i = 0; i < data.length; i++) {
                System.out.print((char) data[i]);
            }
        } catch (IOException ex) {
//            System.out.println(ex.getMessage());
        }
        System.out.println();
    }

    void t19() {
        System.out.println("fi=" + fi);
        Long[][] a = new Long[3][5];
        System.out.println("arr a:" + a);
        Object[][][] objs = new Object[4][][];
        System.out.println("arr objs:" + objs);
        objs[1] = new Object[2][];
        System.out.println("arr objs[1]:" + objs[1]);
        Short[] c = new Short[5];
        System.out.println("arr c:" + c);
        System.out.println("fi=" + fi);

        List<Integer> list = new ArrayList();
        list.add(1);
        list.add(999);
        for (Iterator it = list.iterator(); it.hasNext();) {
            Object o = it.next();
            System.out.println("list[i]=" + o);
        }
        Integer[] iarr = list.toArray(new Integer[0]);
        for (Integer i : iarr) {
            System.out.println("i=" + i);
        }
        System.out.println("arr type:" + iarr.getClass());

        Map<Long, String> map = new HashMap();
        map.put(3L, "Long");
        map.put(4L, "Float");
        for (Iterator<Long> it = map.keySet().iterator(); it.hasNext();) {
            Long key = it.next();
            String val = map.get(key);
            System.out.println(key + ":" + val);
        }
        List clist = Collections.synchronizedList(new ArrayList());

        Class cla = "".getClass();
        Reference ref = new Reference(RefNative.obj2id(cla));
        System.out.println("ref.name=" + ref.className);
        try {
            System.out.println(new Long(0).getClass().toString());
            String s = (String) cla.newInstance();
            System.out.println(s);
            s += "abcd";
            Method m = ref.getMethod("indexOf", "(Ljava.lang.String;I)I");
            if (m != null) {
                Object result = m.invoke(s, new Object[]{"cd", new Integer(1)});
                System.out.println("reflect invoke result:" + result);
            }
        } catch (InstantiationException ex) {
        } catch (IllegalAccessException ex) {
        }

        Class[] classes = RefNative.getClasses();
        System.out.println("classes.size()=" + classes.length);
//        for (Class cl : classes) {
//            System.out.println("class:" + cl.getName() + " id:" + RefNative.obj2id(cl));
//        }

        System.out.println("fi=" + fi);
        int i = 0;
        while (i++ < 100000) {
            try {
                //Thread.sleep(1000);
                int debug = 1;
                debug++;
                debug++;
                String xa = "a";
                String xb = "b";
                String xc = xa + xb;
                xc = xc.substring(1);
                debug++;
                debug++;
                debug++;
                //t22();
                //System.out.println("sleep 1000");
            } catch (Exception e) {
            }
        }
    }

    public void t20_1() {

        String a = "abc";
        String b = "def";
        String c = a + b;

    }

    public void t20() {
        int i = 0;
        while (i++ < 1000) {
            t20_1();
            int debug = 1;
        }
    }

    public void t21() {
        class A {

            Object v;
        }
        class B {

            Object v;
        }
        class C {

            Object v;
        }
        A a = new A();
        B b = new B();
        C c = new C();
        a.v = b;
        b.v = c;
        c.v = a;
    }

    void t22() {
        long lastms = System.currentTimeMillis();
        Reference r = new Reference(RefNative.obj2id(java.lang.String.class));
        Reference r2 = new Reference(RefNative.obj2id(java.lang.Long.class));
        for (int i = 0; i < 1000; i++) {
            try {
                //System.out.print(" " + (System.currentTimeMillis() - lastms));
                lastms = System.currentTimeMillis();
                if (i % 10 == 0) {
                    //System.out.println();
                }
                String s = "abcd";
                s.indexOf("cd", 1);
                Method m;
                m = r.getMethod("indexOf", new Class[]{java.lang.String.class, java.lang.Integer.class});
                if (m != null) {
                    Object result = m.invoke(s, new Object[]{"cd", 1});
                    //System.out.println("reflect invoke result:" + result);
                }
//                if (RefNative.getGarbageStatus() == 1) {
//                    Object[] objs = RefNative.getGarbageReferedObjs();
//                    for (int n = 0; n < objs.length; n++) {
//                        Object o = objs[n];
//                        if (o != null && o instanceof Reference) {
//                            Method[] mds = ((Reference) objs[n]).getMethods();
//                            System.out.println("Reference[" + Long.toString(RefNative.obj2id(objs[n]), 10) + "]:");
//                            for (int j = 0; j < mds.length; j++) {
//                                Method md = mds[j];
//
//                                if (md == null) {
//                                    System.out.println("Method[" + j + "]:" + md);
//                                } else {
//                                    String[] paras = md.getParameterStrs();
//                                    int k = 0;
//                                    for (String p : paras) {
//                                        System.out.println("Method[" + j + "][" + Long.toString(RefNative.obj2id(md), 16) + "](" + Long.toString(RefNative.obj2id(md), 10) + "):" + md.methodName + " paras[" + k + "]:" + p + "|" + Long.toString(RefNative.obj2id(p), 16));
//                                        k++;
//                                    }
//                                }
//                            }
//                        }
//                    }
//                    int debug = 1;
//                }

                Long lo = new Long(0x1010101020202020L);

                m = r2.getMethod("longValue", new Class[]{});
                if (m != null) {
                    Object result = m.invoke(lo, new Object[]{});
                    //System.out.println("reflect invoke result:" + Long.toString((Long) result, 16));
                }
            } catch (Exception ex) {
            }
        }
    }

    void t23() {
        System.out.println(2);
        for (int i = 3; i < 100000; i += 2) {
            boolean isPrime = true;
            for (int j = 3; j < i / 2; j += 2) {
                if ((i % j) == 0) {
                    isPrime = false;
                    break;
                }
            }
            if (isPrime) {
                //System.out.println(i);
            }
        }
    }

    public static void main() {
        Foo1 f = new Foo1();
        for (int i = 0; i < 1; i++) {
            f.t1();
            f.t2();
            f.t3();
            f.t4();
            f.t5();
            f.t6();
            f.t7();
            f.t8();
            f.t9();
            f.t10();
            f.t11();
            f.t12();
            f.t13();
            f.t14();
//            f.t15();
//            f.t16();
//            f.t17();
            f.t18();
            f.t19();
            f.t20();
            f.t21();
//            f.t23();
        }
    }

    public static void main(String[] agrs) {
        try {
//            BpDeepTest.main(agrs);
            Foo1.main();
//            Foo2.main(agrs);
//            Foo3.main(agrs);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
