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
public class ModStep extends Mod {

    long threadId;
    int size;
    int depth;

    public ModStep(RequestPacket req) {
        threadId = req.readRefer();
        size = req.readInt();
        depth = req.readInt();
    }

}
