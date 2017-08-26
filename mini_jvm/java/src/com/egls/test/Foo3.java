/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.egls.test;

/**
 *
 * @author gust
 */
public class Foo3 {

    public static void main(String args[]) {
        int r = 0;
        Foo3 f = new Foo3();
        for (int i = 0; i < 1000; i++) {
            f.t1();
        }
        System.out.println("r=" + r);
    }

    public int t1() {
        int r = 0;
        int i = 0;
        math mymath = new math();
        for (i = 100; i <= 999; i++) {
            if (mymath.shuixianhua(i) == true) {
                //System.out.println(i);
                r += i;
            }
        }
        return r;
    }
}

class math {

    public int f(int x) {
        if (x == 1 || x == 2) {
            return 1;
        } else {
            return f(x - 1) + f(x - 2);
        }
    }

    public boolean iszhishu(int x) {
        for (int i = 2; i <= x / 2; i++) {
            if (x % i == 0) {
                return false;
            }
        }
        return true;
    }

    public boolean shuixianhua(int x) {
        int i = 0, j = 0, k = 0;
        i = x / 100;
        j = (x % 100) / 10;
        k = x % 10;
        if (x == i * i * i + j * j * j + k * k * k) {
            return true;
        } else {
            return false;
        }

    }
}
