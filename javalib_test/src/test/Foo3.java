/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package test;

/**
 *
 * @author gust
 */
public class Foo3 {

    public static void main(String[] args) {
//        M obj = new N();
//        obj.m();
//        System.out.println("over");
        String s=""+'a';
        System.out.println(s);
    }
}

class M {

    public void m() {
        System.out.println("m");
    }
}

class N extends M {

    public void m() {
        super.m();
        System.out.println("n");
    }
}
