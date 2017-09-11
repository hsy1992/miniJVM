/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.net;

/**
 *
 * @author gust
 */
public class RequestPacket extends JdwpPacket {

    public byte getCommandSet() {
        return (byte) getVal(10, 1);
    }

    public byte getCommand() {
        return (byte) getVal(11, 1);
    }
}
