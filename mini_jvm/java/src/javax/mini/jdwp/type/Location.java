/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.type;

import javax.mini.jdwp.net.JdwpPacket;




/**
 *
 * @author gust
 */
public class Location {

    public byte typeTag;
    public long classID;
    public long methodID;
    public long execIndex;

    public void writeLocation(JdwpPacket res) {
        res.writeByte(typeTag);
        res.writeRefer(classID);
        res.writeRefer(methodID);
        res.writeLong(execIndex);

    }

    public boolean equals(Location other) {
        if (other == null) {
            return false;
        }
        if (other.classID == classID
                && other.methodID == methodID
                && other.execIndex == execIndex) {
            return true;
        }
        return false;
    }

    public String toString() {
        return "Location:"
                + "|" + typeTag
                + "|" + Long.toString(classID, 16)
                + "|" + Long.toString(methodID, 16)
                + "|" + Long.toString(execIndex);
    }
}
