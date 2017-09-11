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

    public ResponsePacket() {
        super();
        setFlag(RESPONSE);
    }

    public ResponsePacket(byte[] data) {
        super(data);
    }

    public short getErrorCode() {
        return (short) getVal(10, 2);
    }

    public void setErrorCode(int code) {
        int pos = 9;
        data[pos++] = (byte) (code >>> 8);
        data[pos++] = (byte) (code);
    }
    
    public String toString(){
        return "RES["+getId()+"]"+getErrorCode()+DELIMITER+getLength();
    }
}
