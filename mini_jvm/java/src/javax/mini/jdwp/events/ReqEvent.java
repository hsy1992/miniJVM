/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.jdwp.events;

/**
 *
 * @author gust
 */
public class ReqEvent {

    private int requestId = EventManager.getRequestId();
    byte eventKind;
    byte suspendPolicy;
    byte kindMod;

    public ReqEvent(byte eventKind, byte suspendPolicy, byte kindMod) {
        this.eventKind = eventKind;
        this.suspendPolicy = suspendPolicy;
        this.kindMod = kindMod;
    }

    /**
     * @return the requestId
     */
    public int getRequestId() {
        return requestId;
    }

}
