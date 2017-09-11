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
public class ResponsePacket extends JdwpPacket {

    public short getErrorCode() {
        return (short) getVal(10, 2);
    }
}
