/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import javax.mini.jdwp.type.Location;
import javax.mini.jdwp.net.RequestPacket;

/**
 *
 * @author gust
 */
public class ModLocationOnly  extends Mod{

    Location loc=new Location();

    public ModLocationOnly(RequestPacket req) {
        loc.typeTag = req.readByte();
        loc.classID =req.readRefer();
        loc.methodID =req.readRefer();
        loc.execIndex =req.readLong();
        
    }

}
