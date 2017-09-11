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

    public RequestPacket() {
        super();
        setFlag(REQUEST);
    }

    public RequestPacket(byte[] data) {
        super(data);
    }

    public byte getCommandSet() {
        return (byte) getVal(9, 1);
    }

    public byte getCommand() {
        return (byte) getVal(10, 1);
    }

    public void setCommandSet(int cset) {
        int pos = 9;
        data[pos] = (byte) (cset);
    }

    public void setCommand(int cmd) {
        int pos = 10;
        data[pos] = (byte) (cmd);
    }

    public String toString() {
        return "REQ[" + getId() + "]" + getCommandSet() + "." + getCommand() + DELIMITER + getLength();
    }
}
