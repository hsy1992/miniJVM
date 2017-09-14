/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

import javax.mini.jdwp.net.RequestPacket;

/**
 *
 * @author gust
 */
public class ModExceptionOnly extends Mod {

    long referenceTypeID;
    boolean caught;
    boolean uncaught;

    public ModExceptionOnly(RequestPacket req) {
        referenceTypeID = req.readRefer();
        caught = req.readBoolean();
        uncaught = req.readBoolean();
    }

}
