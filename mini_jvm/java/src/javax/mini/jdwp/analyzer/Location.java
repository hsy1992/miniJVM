/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.analyzer;

import javax.mini.jdwp.net.ResponsePacket;

/**
 *
 * @author gust
 */
public class Location {

    public byte typeTag;
    public long classID;
    public long methodID;
    public long execIndex;
    
    public void writeLocation(ResponsePacket res){
        res.writeByte(typeTag);
        res.writeRefer(classID);
        res.writeRefer(methodID);
        res.writeLong(execIndex);
        
    }
}
