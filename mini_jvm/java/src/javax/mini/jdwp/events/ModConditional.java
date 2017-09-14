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
public class ModConditional  extends Mod{

    int exprID;

    public ModConditional(RequestPacket req) {
        exprID = req.readInt();
    }

}
