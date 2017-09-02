/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javax.mini.eio;

import java.io.IOException;
import javax.cldc.io.Connection;

/**
 *
 * @author gust
 */
public interface ServerSocket extends Connection {

    void listen() throws IOException;

    Socket accept() throws IOException;
}
