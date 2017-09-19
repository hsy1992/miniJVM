/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.reflect;

/**
 *
 * @author gust
 */
public class LocalVarTable {
    public long codeIndex;
    public int length;
    public String name;
    public String signature;
    public int slot;
    
    public String toString(){
        return name+"|"+signature+"|start:"+codeIndex+"|len:"+length+"|slot:"+slot;
    }
}
