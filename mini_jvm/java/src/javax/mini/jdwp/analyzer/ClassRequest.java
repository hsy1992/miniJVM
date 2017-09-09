package javax.mini.jdwp.analyzer;

import javax.mini.jdwp.constant.ClassStatus;
import javax.mini.jdwp.constant.EventKind;
import javax.mini.jdwp.constant.Step;
import javax.mini.jdwp.constant.SuspendPolicy;
import javax.mini.jdwp.constant.Tag;
import javax.mini.jdwp.constant.TypeTag;
import javax.mini.jdwp.net.Packet;
import javax.mini.jdwp.reflect.FieldType;
import javax.mini.jdwp.reflect.MethodType;
import javax.mini.jdwp.reflect.ObjectType;
import javax.mini.jdwp.reflect.ReferenceType;
import javax.mini.util.Iterator;
import javax.mini.util.List;

public class ClassRequest {
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_10_1 extends PacketAnalyzer {

    public Request_10_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("StrObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_1 extends PacketAnalyzer {

    public Request_11_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_10 extends PacketAnalyzer {

    public Request_11_10() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        Node threadIdInfo = new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
        root.addChild(threadIdInfo);

        long exceptionTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType exceptionType = ObjectType.getType(exceptionTypeId);
        Node exceptionIdInfo = new Node(new Node.Descriptor("AsyncExceptionObjectID:", exceptionType), new Node.Value(new Long(exceptionTypeId)));
        root.addChild(exceptionIdInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_11 extends PacketAnalyzer {

    public Request_11_11() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_12 extends PacketAnalyzer {

    public Request_11_12() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_2 extends PacketAnalyzer {

    public Request_11_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_3 extends PacketAnalyzer {

    public Request_11_3() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_4 extends PacketAnalyzer {

    public Request_11_4() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_5 extends PacketAnalyzer {

    public Request_11_5() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_6 extends PacketAnalyzer {

    public Request_11_6() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        Node threadIdInfo = new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
        root.addChild(threadIdInfo);

        int startFrame = (int) getVal(packet, index, 4);
        index += 4;
        Node startFrameInfo = new Node(new Node.Descriptor("StartFrame:"), new Node.Value(new Integer(startFrame)));
        root.addChild(startFrameInfo);

        int lenght = (int) getVal(packet, index, 4);
        index += 4;
        Node lenghtInfo = new Node(new Node.Descriptor("Lenght:"), new Node.Value(new Integer(lenght)));
        root.addChild(lenghtInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_7 extends PacketAnalyzer {

    public Request_11_7() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_8 extends PacketAnalyzer {

    public Request_11_8() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_11_9 extends PacketAnalyzer {

    public Request_11_9() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_12_1 extends PacketAnalyzer {

    public Request_12_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadGroupObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_12_2 extends PacketAnalyzer {

    public Request_12_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadGroupObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_12_3 extends PacketAnalyzer {

    public Request_12_3() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ThreadGroupObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_13_1 extends PacketAnalyzer {

    public Request_13_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ArrayObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_13_2 extends PacketAnalyzer {

    public Request_13_2() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        Node arrayInfo = new Node(new Node.Descriptor("ArrayObjectID:", objType), new Node.Value(new Long(objTypeId)));
        root.addChild(arrayInfo);

        int startIndex = (int) getVal(packet, index, 4);
        index += 4;
        Node startIndexInfo = new Node(new Node.Descriptor("FirstIndex:"), new Node.Value(new Integer(startIndex)));
        root.addChild(startIndexInfo);

        int lenght = (int) getVal(packet, index, 4);
        index += 4;
        Node lenghtInfo = new Node(new Node.Descriptor("Length:"), new Node.Value(new Integer(lenght)));
        root.addChild(lenghtInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_13_3 extends PacketAnalyzer {

    public Request_13_3() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        Node arrayInfo = new Node(new Node.Descriptor("ArrayObjectID:", objType), new Node.Value(new Long(objTypeId)));
        root.addChild(arrayInfo);

        int startIndex = (int) getVal(packet, index, 4);
        index += 4;
        Node startIndexInfo = new Node(new Node.Descriptor("FirstIndex:"), new Node.Value(new Integer(startIndex)));
        root.addChild(startIndexInfo);

        int lenght = (int) getVal(packet, index, 4);
        index += 4;
        Node lenghtInfo = new Node(new Node.Descriptor("Length:"), new Node.Value(new Integer(lenght)));
        root.addChild(lenghtInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_14_1 extends PacketAnalyzer {

    public Request_14_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ClassLoaderObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_15_1 extends PacketAnalyzer {

    /**
     * Creates a new instance of Request_15_1
     */
    public Request_15_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        byte eventKind = packet.getData()[index++];
        Node root = new Node(new Node.Descriptor("EventKind:"), new Node.Value(new Byte(eventKind), EventKind.asString(eventKind)));

        byte suspend = packet.getData()[index++];
        Node suspendInfo = new Node(new Node.Descriptor("SuspendPolicy:"), new Node.Value(new Byte(suspend), SuspendPolicy.asString(suspend)));
        root.addChild(suspendInfo);

        int numModifiers = (int) getVal(packet, index, 4);
        index += 4;
        Node modifiersInfo = new Node(new Node.Descriptor("NumModifiers:"), new Node.Value(new Integer(numModifiers)));
        root.addChild(modifiersInfo);

        for (int i = 0; i < numModifiers; i++) {
            int mod = packet.getData()[index++];
            switch (mod) {
                case 1:
                    int count = (int) getVal(packet, index, 4);
                    index += 4;
                    Node countInfo = new Node(new Node.Descriptor("Count:"), new Node.Value(new Integer(count)));
                    root.addChild(countInfo);
                    break;
                case 2:
                    int exprId = (int) getVal(packet, index, 4);
                    index += 4;
                    Node exprIdInfo = new Node(new Node.Descriptor("Conditional-ExprID:"), new Node.Value(new Integer(exprId)));
                    root.addChild(exprIdInfo);
                    break;
                case 3:
                    long objTypeId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    ObjectType objType = ObjectType.getType(objTypeId);
                    Node threadIdInfo = new Node(new Node.Descriptor("ThreadOnly-ThreadID:", objType), new Node.Value(new Long(objTypeId)));
                    root.addChild(threadIdInfo);
                    break;
                case 4:
                    long refId = getVal(packet, index, referenceTypeIDSize);
                    index += referenceTypeIDSize;
                    ReferenceType refType = ReferenceType.getType(refId);
                    Node refIdInfo = new Node(new Node.Descriptor("ClassOnly-RefTypeID:", refType), new Node.Value(new Long(refId)));
                    root.addChild(refIdInfo);
                    break;
                case 5:
                    int len = (int) getVal(packet, index, 4);
                    index += 4;
                    String classMatch = getStr(packet, index, len);
                    index += len;
                    Node matchInfo = new Node(new Node.Descriptor("ClassInclude-Pattern:"), new Node.Value(classMatch));
                    root.addChild(matchInfo);
                    break;
                case 6:
                    len = (int) getVal(packet, index, 4);
                    index += 4;
                    String classExclude = getStr(packet, index, len);
                    index += len;
                    Node excludeInfo = new Node(new Node.Descriptor("ClassExclude-Pattern:"), new Node.Value(classExclude));
                    root.addChild(excludeInfo);
                    break;
                case 7:
                    Location location = getLocation(packet, index);
                    index += locationSize;
                    Node locationInfo = location.toNode();
                    root.addChild(locationInfo);
                    break;
                case 8:
                    refId = getVal(packet, index, referenceTypeIDSize);
                    index += referenceTypeIDSize;
                    boolean reportCaught = getBoolean(packet, index++);
                    boolean reportUncaught = getBoolean(packet, index++);
                    String desc = (reportCaught ? "Caught=YES" : "Caught=NO") + "/"
                            + (reportUncaught ? "Uncaught=YES" : "Uncaught=NO");
                    refType = ReferenceType.getType(refId);
                    Node exceptionInfo = new Node(new Node.Descriptor("Exception:", refType), new Node.Value(desc));
                    root.addChild(exceptionInfo);
                    break;
                case 9:
                    refId = getVal(packet, index, referenceTypeIDSize);
                    index += referenceTypeIDSize;
                    long fieldId = getVal(packet, index, fieldIDSize);
                    index += fieldIDSize;
                    FieldType fieldType = FieldType.getType(refId, fieldId);
                    Node fieldOnlyInfo = new Node(new Node.Descriptor("FieldOnlyID:", fieldType), new Node.Value(new Long(fieldId)));
                    root.addChild(fieldOnlyInfo);
                    break;
                case 10:
                    long threadId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    int stepSize = (int) getVal(packet, index, 4);
                    index += 4;
                    int stepDepth = (int) getVal(packet, index, 4);
                    index += 4;
                    desc = "ThreadID:" + threadId + "," + Step.sizeAsString(stepSize)
                            + "," + Step.depthAsString(stepDepth);
                    ObjectType threadType = ObjectType.getType(threadId);
                    Node stepInfo = new Node(new Node.Descriptor("Step:", threadType), new Node.Value(desc));
                    root.addChild(stepInfo);
                    break;
                case 11:
                    long objId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    ObjectType instance = ObjectType.getType(objId);
                    Node instanceInfo = new Node(new Node.Descriptor("InstanceOnly-ObjID:", instance), new Node.Value(new Long(objId)));
                    root.addChild(instanceInfo);
                    break;

                //this should NEVER happen
                default:
                    Node errInfo = new Node(new Node.Descriptor("ERR: Unknown modifier " + mod), null);
                    root.addChild(errInfo);
                    break;
            }
        }

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_15_2 extends PacketAnalyzer {

    public Request_15_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        byte eventKind = packet.getData()[index++];
        Node eventKindInfo = new Node(new Node.Descriptor("EventKind:"), new Node.Value(new Byte(eventKind), EventKind.asString(eventKind)));
        root.addChild(eventKindInfo);

        int requestId = (int) getVal(packet, index, 4);
        index += 4;
        Node requestInfo = new Node(new Node.Descriptor("RequestID:"), new Node.Value(new Integer(requestId)));
        root.addChild(requestInfo);

        return root;
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_16_1 extends PacketAnalyzer {

    public Request_16_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        long threadId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(threadId);
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:", objType), new Node.Value(new Long(threadId)));
        root.addChild(threadInfo);

        long frameId = getVal(packet, index, frameIDSize);
        index += frameIDSize;
        Node frameInfo = new Node(new Node.Descriptor("FrameID:"), new Node.Value(new Long(frameId)));
        root.addChild(frameInfo);

        int numVars = (int) getVal(packet, index, 4);
        index += 4;
        Node variablesInfo = new Node(new Node.Descriptor("NumVariables:"), new Node.Value(new Integer(numVars)));
        root.addChild(variablesInfo);

        for (int i = 0; i < numVars; i++) {
            int vfi = (int) getVal(packet, index, 4);
            index += 4;
            Node vfiInfo = new Node(new Node.Descriptor("VariableFrameIndex:"), new Node.Value(new Integer(vfi)));
            variablesInfo.addChild(vfiInfo);

            byte variableTag = packet.getData()[index++];
            Node variableTagInfo = new Node(new Node.Descriptor("VariableTag:"), new Node.Value(new Byte(variableTag), Tag.asString(variableTag)));
            variablesInfo.addChild(variableTagInfo);
        }

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_16_2 extends PacketAnalyzer {

    public Request_16_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        long threadId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:"), new Node.Value(new Long(threadId)));
        root.addChild(threadInfo);

        long frameId = getVal(packet, index, frameIDSize);
        index += frameIDSize;
        Node frameInfo = new Node(new Node.Descriptor("FrameID:"), new Node.Value(new Long(frameId)));
        root.addChild(frameInfo);

        int numValues = (int) getVal(packet, index, 4);
        index += 4;
        Node valuesInfo = new Node(new Node.Descriptor("NumValues:"), new Node.Value(new Integer(numValues)));
        root.addChild(valuesInfo);

        for (int i = 0; i < numValues; i++) {
            int slotId = (int) getVal(packet, index, 4);
            index += 4;
            Node slotInfo = new Node(new Node.Descriptor("SlotID:"), new Node.Value(new Integer(slotId)));
            valuesInfo.addChild(slotInfo);

            Value value = getValue(packet, index);
            index += value.getOffset();
            Node valueInfo = new Node(new Node.Descriptor("Value:"), new Node.Value(value));
            valuesInfo.addChild(valueInfo);
        }

        return root;
    }

//    public Component getDetailedView(Packet packet) {
//		int index = 0;
//		JPanel panel = new JPanel();
//		panel.setLayout(new BorderLayout());
//
//		Vector colData;
//		Vector rowData = new Vector();
//		Vector colNames = new Vector();
//
//		colNames.add("Description");
//		colNames.add("Value");
//		
//		colData = new Vector();
//		colData.add("Thread ID:");
//		colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//		rowData.add(colData);
//        index += objectIDSize;
//        
//		colData = new Vector();
//		colData.add("Frame ID:");
//		colData.add(String.valueOf(getVal(packet, index, frameIDSize)));
//		rowData.add(colData);
//        index += frameIDSize;
//        
//		int numSlots = (int) getVal(packet, index, 4);
//		index += 4;
//		colData = new Vector();
//		colData.add("Num Values:");
//		colData.add(String.valueOf(numSlots));
//		rowData.add(colData);
//
//		if (numSlots > 0) {
//			colData = new Vector();
//			colData.add("");
//			colData.add("");
//			rowData.add(colData);
//		}
//		
//		for(int i = 0; i < numSlots; i++) {
//			colData = new Vector();
//			colData.add("Slot ID:");
//			colData.add(String.valueOf(getVal(packet, index, 4)));
//			rowData.add(colData);
//	        index += 4;
//
//			Value value = getValue(packet, index);
//            colData = new Vector();
//			colData.add("Value:");
//			colData.add(value.getValue());
//			rowData.add(colData);
//            index += value.getOffset();
//		}
//    	
//		panel.add(new JScrollPane(new JTable(rowData, colNames)), BorderLayout.CENTER);
//		return panel;
//    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_16_3 extends PacketAnalyzer {

    public Request_16_3() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        long threadId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:"), new Node.Value(new Long(threadId)));
        root.addChild(threadInfo);

        long frameId = getVal(packet, index, frameIDSize);
        index += frameIDSize;
        Node frameInfo = new Node(new Node.Descriptor("FrameID:"), new Node.Value(new Long(frameId)));
        root.addChild(frameInfo);

        return root;
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_16_4 extends PacketAnalyzer {

    public Request_16_4() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        long threadId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(threadId);
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:", objType), new Node.Value(new Long(threadId)));
        root.addChild(threadInfo);

        long frameId = getVal(packet, index, frameIDSize);
        index += frameIDSize;
        Node frameInfo = new Node(new Node.Descriptor("FrameID:"), new Node.Value(new Long(frameId)));
        root.addChild(frameInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_17_1 extends PacketAnalyzer {

    public Request_17_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ClassObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_1_10 extends PacketAnalyzer {

    public Request_1_10() {
    }

    public Node getPacketInfo(Packet packet) {
        int exitCode = (int) getVal(packet, 0, 4);
        Node fieldSizeInfo = new Node(new Node.Descriptor("ExitCode:"), new Node.Value(new Integer(exitCode)));
        return fieldSizeInfo;
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_1_11 extends PacketAnalyzer {

    public Request_1_11() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int len = (int) getVal(packet, index, 4);
        index += 4;
        String str = getStr(packet, index, len);
        index += len;
        Node strInfo = new Node(new Node.Descriptor("StringValue:"), new Node.Value(str));
        return strInfo;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_1_14 extends PacketAnalyzer {

    public Request_1_14() {
    }

    public void updateInternalDataModel(Packet packet) {
        List children = this.getPacketInfo(packet).getChildren();
        for (Iterator iter = children.iterator(); iter.hasNext();) {
            Node child = (Node) iter.next();
            Long objectId = (Long) child.getValue().getRealValue();
            ObjectType object = ObjectType.getType(objectId);
            object.setDisposeRequested(new Boolean(true));
            iter.next(); //skip refcount
        }
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int numObjects = (int) getVal(packet, index, 4);
        index += 4;
        Node objectsInfo = new Node(new Node.Descriptor("NumObjects:"), new Node.Value(new Integer(numObjects)));

        for (int i = 0; i < numObjects; i++) {
            long objectId = getVal(packet, index, objectIDSize);
            index += objectIDSize;
            ObjectType objectType = ObjectType.getType(objectId);
            Node objectInfo = new Node(new Node.Descriptor("ObjectId:", objectType), new Node.Value(new Long(objectId)));
            objectsInfo.addChild(objectInfo);

            int refCount = (int) getVal(packet, index, 4);
            index += 4;
            Node refCountInfo = new Node(new Node.Descriptor("ReferenceCount:"), new Node.Value(new Long(refCount)));
            objectsInfo.addChild(refCountInfo);
        }

        return objectsInfo;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_1_18 extends PacketAnalyzer {

    public Request_1_18() {
    }

    public void updateInternalDataModel(Packet packet) {
        List classes = this.getPacketInfo(packet).getChildren();
        for (Iterator iter = classes.iterator(); iter.hasNext();) {
            Node refTypeInfo = (Node) iter.next();
            ReferenceType refType = (ReferenceType) refTypeInfo.getDescriptor().getAssociatedType();

            byte[] classBytes = (byte[]) ((Node) refTypeInfo.getChildren().get(0)).getValue().getRealValue();
            refType.setClassBytes(classBytes);
        }
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numClasses = (int) getVal(packet, index, 4);
        index += 4;
        Node classesInfo = new Node(new Node.Descriptor("NumClasses:"), new Node.Value(new Integer(numClasses)));

        for (int i = 0; i < numClasses; i++) {
            long referenceTypeID = getVal(packet, index, referenceTypeIDSize);
            index += referenceTypeIDSize;
            ReferenceType refType = ReferenceType.getType(referenceTypeID);
            Node refTypeInfo = new Node(new Node.Descriptor("RefTypeID:", refType), new Node.Value(new Long(referenceTypeID)));
            classesInfo.addChild(refTypeInfo);

            int numBytes = (int) getVal(packet, index, 4);
            index += 4;
            byte[] classBytes = new byte[numBytes];
            System.arraycopy(packet.getData(), index, classBytes, 0, numBytes);
            index += numBytes;
            Node byteInfo = new Node(new Node.Descriptor("ClassBytes:"), new Node.Value(classBytes, Integer.toString(numBytes)));
            refTypeInfo.addChild(byteInfo);
        }

        return classesInfo;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_1_19 extends PacketAnalyzer {

    public Request_1_19() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String value = getStr(packet, index, len);
        index += len;
        Node root = new Node(new Node.Descriptor("StratumID:"), new Node.Value(value));

        return root;
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_1_2 extends PacketAnalyzer {

    public Request_1_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String desc = getStr(packet, index, len);
        index += len;
        Node root = new Node(new Node.Descriptor("JNISignature:"), new Node.Value(desc));

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_1 extends PacketAnalyzer {

    public Request_2_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_10 extends PacketAnalyzer {

    public Request_2_10() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_11 extends PacketAnalyzer {

    public Request_2_11() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_12 extends PacketAnalyzer {

    public Request_2_12() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_13 extends PacketAnalyzer {

    public Request_2_13() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_14 extends PacketAnalyzer {

    public Request_2_14() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_15 extends PacketAnalyzer {

    public Request_2_15() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_2 extends PacketAnalyzer {

    public Request_2_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_3 extends PacketAnalyzer {

    public Request_2_3() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_4 extends PacketAnalyzer {

    public Request_2_4() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_5 extends PacketAnalyzer {

    public Request_2_5() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_6 extends PacketAnalyzer {

    public Request_2_6() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node refInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(refInfo);

        int numFields = (int) getVal(packet, index, 4);
        index += 4;
        Node fieldsInfo = new Node(new Node.Descriptor("NumFields:"), new Node.Value(new Integer(numFields)));
        root.addChild(fieldsInfo);

        for (int i = 0; i < numFields; i++) {
            long fieldID = (getVal(packet, index, fieldIDSize));
            index += fieldIDSize;
            FieldType fieldType = FieldType.getType(refTypeId, fieldID);
            Node singleFieldInfo = new Node(new Node.Descriptor("FieldID:", fieldType), new Node.Value(new Long(fieldID)));
            fieldsInfo.addChild(singleFieldInfo);
        }
        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_7 extends PacketAnalyzer {

    public Request_2_7() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_8 extends PacketAnalyzer {

    public Request_2_8() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_2_9 extends PacketAnalyzer {

    public Request_2_9() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_3_1 extends PacketAnalyzer {

    public Request_3_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        return new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
    }

}

/*
/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_3_2 extends PacketAnalyzer {

    public Request_3_2() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node refInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(refInfo);

        int numFields = (int) getVal(packet, index, 4);
        index += 4;
        Node valuesInfo = new Node(new Node.Descriptor("NumValues:"), new Node.Value(new Integer(numFields)));
        root.addChild(valuesInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_3_3 extends PacketAnalyzer {

    public Request_3_3() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node refInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(refInfo);

        long threadId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objectType = ObjectType.getType(threadId);
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:", objectType), new Node.Value(new Long(threadId)));
        root.addChild(threadInfo);

        long methodId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methodType = MethodType.getType(refTypeId, methodId);
        Node methodInfo = new Node(new Node.Descriptor("MethodID:", methodType), new Node.Value(new Long(methodId)));
        root.addChild(methodInfo);

        int numargs = (int) getVal(packet, index, 4);
        index += 4;

        Node argsInfo = new Node(new Node.Descriptor("NumArgs:"), new Node.Value(new Integer(numargs)));
        root.addChild(argsInfo);

        for (int i = 0; i < numargs; i++) {
            Value value = getValue(packet, index);
            index += value.getOffset();
            Node singleArgInfo = new Node(new Node.Descriptor("ArgValue:"), new Node.Value(value));
            argsInfo.addChild(singleArgInfo);
        }

        int options = (int) getVal(packet, index, 4);
        index += 4;
        Node optionsInfo = new Node(new Node.Descriptor("Options:"), new Node.Value(new Integer(options), getInvokeOptionsAsStr(options)));
        root.addChild(optionsInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_3_4 extends PacketAnalyzer {

    public Request_3_4() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node refInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(refInfo);

        long threadId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objectType = ObjectType.getType(threadId);
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:", objectType), new Node.Value(new Long(threadId)));
        root.addChild(threadInfo);

        long methodId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methodType = MethodType.getType(refTypeId, methodId);
        Node methodInfo = new Node(new Node.Descriptor("MethodID:", methodType), new Node.Value(new Long(methodId)));
        root.addChild(methodInfo);

        int numargs = (int) getVal(packet, index, 4);
        index += 4;

        Node argsInfo = new Node(new Node.Descriptor("NumArgs:"), new Node.Value(new Integer(numargs)));
        root.addChild(argsInfo);

        for (int i = 0; i < numargs; i++) {
            Value value = getValue(packet, index);
            index += value.getOffset();
            Node singleArgInfo = new Node(new Node.Descriptor("ArgValue:"), new Node.Value(value));
            argsInfo.addChild(singleArgInfo);
        }

        int options = (int) getVal(packet, index, 4);
        index += 4;
        Node optionsInfo = new Node(new Node.Descriptor("Options:"), new Node.Value(new Integer(options), getInvokeOptionsAsStr(options)));
        root.addChild(optionsInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_4_1 extends PacketAnalyzer {

    public Request_4_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        index += referenceTypeIDSize;
        Node newArrayInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));

        int lenght = (int) getVal(packet, index, 4);
        index += 4;
        Node lenghtInfo = new Node(new Node.Descriptor("ArrayLenght:"), new Node.Value(new Integer(lenght)));
        newArrayInfo.addChild(lenghtInfo);

        return newArrayInfo;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author martin ryzl, karel herink
 */
class Request_64_100 extends PacketAnalyzer {

    //public static final int[] WTK = new int[] {8, 4, 4, 4, 4};
    /**
     * Creates a new instance of CDAnalyzer
     */
    public Request_64_100() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        byte suspend = packet.getData()[index++];
        Node suspendInfo = new Node(new Node.Descriptor("SuspendPolicy:"), new Node.Value(new Byte(suspend), SuspendPolicy.asString(suspend)));
        root.addChild(suspendInfo);

        int numEvents = (int) getVal(packet, index, 4);
        index += 4;
        Node eventsInfo = new Node(new Node.Descriptor("NumEvents:"), new Node.Value(new Integer(numEvents)));
        root.addChild(eventsInfo);

        for (int i = 0; i < numEvents; i++) {
            byte eventKind = packet.getData()[index++];

            Node eventInfo = new Node(new Node.Descriptor("EventKind:"), new Node.Value(new Byte(eventKind), EventKind.asString(eventKind)));
            eventsInfo.addChild(eventInfo);

            int requestId = (int) getVal(packet, index, 4);
            index += 4;

            switch (eventKind) {
                case EventKind.VM_START:
//					fallthrough
                case EventKind.THREAD_DEATH:
//					fallthrough
                case EventKind.THREAD_START:
                    long threadId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    ObjectType thread = ObjectType.getType(threadId);
                    Node threadInfo = new Node(new Node.Descriptor("ThreadID:", thread), new Node.Value(new Long(threadId)));
                    eventInfo.addChild(threadInfo);
                    break;

                case EventKind.SINGLE_STEP:
//					fallthrough
                case EventKind.METHOD_ENTRY:
//					fallthrough
                case EventKind.METHOD_EXIT:
//					fallthrough
                case EventKind.BREAKPOINT:
                    threadId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    thread = ObjectType.getType(threadId);
                    threadInfo = new Node(new Node.Descriptor("ThreadID:", thread), new Node.Value(new Long(threadId)));
                    eventInfo.addChild(threadInfo);

                    Location location = getLocation(packet, index);
                    index += locationSize;
                    Node locationInfo = location.toNode();
                    eventInfo.addChild(locationInfo);
                    break;

                case EventKind.EXCEPTION:
                    threadId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    thread = ObjectType.getType(threadId);
                    threadInfo = new Node(new Node.Descriptor("ThreadID:", thread), new Node.Value(new Long(threadId)));
                    eventInfo.addChild(threadInfo);

                    Location locationThrown = getLocation(packet, index);
                    locationThrown.setDecription("LocationThrown:");
                    index += locationSize;
                    Node locationThrownInfo = locationThrown.toNode();
                    eventInfo.addChild(locationThrownInfo);

                    Value value = getValue(packet, index);
                    index += value.getOffset();
                    Long objectId = (Long) value.getValue();
                    ObjectType objectType = ObjectType.getType(objectId);
                    Node singleValueInfo = new Node(new Node.Descriptor("ExceptionObjectID:", objectType), new Node.Value(value));
                    eventInfo.addChild(singleValueInfo);

                    Location locationCaught = getLocation(packet, index);
                    locationCaught.setDecription("LocationCaught");
                    index += locationSize;
                    Node locationCaughtInfo = locationCaught.toNode();
                    eventInfo.addChild(locationCaughtInfo);
                    break;

                case EventKind.CLASS_PREPARE:
                    threadId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    thread = ObjectType.getType(threadId);
                    threadInfo = new Node(new Node.Descriptor("LoaderThreadID:", thread), new Node.Value(new Long(threadId)));
                    eventInfo.addChild(threadInfo);

                    byte typeTag = packet.getData()[index++];
                    Node typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Byte(typeTag), TypeTag.asString(typeTag)));
                    eventInfo.addChild(typeTagInfo);

                    long refId = getVal(packet, index, referenceTypeIDSize);
                    index += referenceTypeIDSize;
                    ReferenceType refType = ReferenceType.getType(refId);
                    Node refIdInfo = new Node(new Node.Descriptor("ReferenceID:", refType), new Node.Value(new Long(refId)));
                    eventInfo.addChild(refIdInfo);

                    int sigLen = (int) getVal(packet, index, 4);
                    index += 4;
                    String signature = getStr(packet, index, sigLen);
                    index += sigLen;
                    Node signatureInfo = new Node(new Node.Descriptor("RefTypeSignature:"), new Node.Value(signature));
                    eventInfo.addChild(signatureInfo);

                    int classStatus = (int) getVal(packet, index, 4);
                    index += 4;
                    Node classStatusInfo = new Node(new Node.Descriptor("Status:"), new Node.Value(new Integer(classStatus), ClassStatus.asString(classStatus)));
                    eventInfo.addChild(classStatusInfo);
                    break;

                //TODO test this case
                case EventKind.CLASS_UNLOAD:
                    sigLen = (int) getVal(packet, index, 4);
                    index += 4;
                    signature = getStr(packet, index, sigLen);
                    index += sigLen;
                    signatureInfo = new Node(new Node.Descriptor("RefTypeSignature:"), new Node.Value(signature));
                    eventInfo.addChild(signatureInfo);
                    break;

                //TODO test this case
                case EventKind.FIELD_ACCESS:
                    threadId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    thread = ObjectType.getType(threadId);
                    threadInfo = new Node(new Node.Descriptor("AccessorThreadID:", thread), new Node.Value(new Long(threadId)));
                    eventInfo.addChild(threadInfo);

                    location = getLocation(packet, index);
                    location.setDecription("AccessLocation:");
                    index += locationSize;
                    locationInfo = location.toNode();
                    eventInfo.addChild(locationInfo);

                    typeTag = packet.getData()[index++];
                    typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Byte(typeTag), TypeTag.asString(typeTag)));
                    eventInfo.addChild(typeTagInfo);

                    refId = getVal(packet, index, referenceTypeIDSize);
                    index += referenceTypeIDSize;
                    refType = ReferenceType.getType(refId);
                    refIdInfo = new Node(new Node.Descriptor("ReferenceID:", refType), new Node.Value(new Long(refId)));
                    eventInfo.addChild(refIdInfo);

                    long fieldId = getVal(packet, index, fieldIDSize);
                    index += fieldIDSize;
                    FieldType fieldType = FieldType.getType(refId, fieldId);
                    Node fieldIdInfo = new Node(new Node.Descriptor("AccessedFieldID:", fieldType), new Node.Value(new Long(fieldId)));
                    eventInfo.addChild(fieldIdInfo);

                    value = getValue(packet, index);
                    index += value.getOffset();
                    objectId = (Long) value.getValue();
                    objectType = ObjectType.getType(objectId);
                    singleValueInfo = new Node(new Node.Descriptor("AccessedObjectID:", objectType), new Node.Value(value));
                    eventInfo.addChild(singleValueInfo);
                    break;

                case EventKind.FIELD_MODIFICATION:
                    threadId = getVal(packet, index, objectIDSize);
                    index += objectIDSize;
                    thread = ObjectType.getType(threadId);
                    threadInfo = new Node(new Node.Descriptor("AccessorThreadID:", thread), new Node.Value(new Long(threadId)));
                    eventInfo.addChild(threadInfo);

                    location = getLocation(packet, index);
                    location.setDecription("AccessLocation:");
                    index += locationSize;
                    locationInfo = location.toNode();
                    eventInfo.addChild(locationInfo);

                    typeTag = packet.getData()[index++];
                    typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Byte(typeTag), TypeTag.asString(typeTag)));
                    eventInfo.addChild(typeTagInfo);

                    refId = getVal(packet, index, referenceTypeIDSize);
                    index += referenceTypeIDSize;
                    refType = ReferenceType.getType(refId);
                    refIdInfo = new Node(new Node.Descriptor("ReferenceID:", refType), new Node.Value(new Long(refId)));
                    eventInfo.addChild(refIdInfo);

                    fieldId = getVal(packet, index, fieldIDSize);
                    index += fieldIDSize;
                    fieldType = FieldType.getType(refId, fieldId);
                    fieldIdInfo = new Node(new Node.Descriptor("AccessedFieldID:", fieldType), new Node.Value(new Long(fieldId)));
                    eventInfo.addChild(fieldIdInfo);

                    value = getValue(packet, index);
                    index += value.getOffset();
                    objectId = (Long) value.getValue();
                    objectType = ObjectType.getType(objectId);
                    singleValueInfo = new Node(new Node.Descriptor("AccessedObjectID:", objectType), new Node.Value(value));
                    eventInfo.addChild(singleValueInfo);

                    value = getValue(packet, index);
                    index += value.getOffset();
                    objectId = (Long) value.getValue();
                    objectType = ObjectType.getType(objectId);
                    singleValueInfo = new Node(new Node.Descriptor("ValueSet:", objectType), new Node.Value(value));
                    eventInfo.addChild(singleValueInfo);
                    break;

                case EventKind.VM_DEATH:
                    //TODO : all streams are closed after this so stop reading
                    break;

                default:
                    Node errInfo = new Node(new Node.Descriptor("ERR: Unknown eventKind " + eventKind), null);
                    eventInfo.addChild(errInfo);
                    break;
            }

        }

        return root;
    }

//	public Component getDetailedView(Packet packet) {
//		int index = 0;
//		JPanel panel = new JPanel();
//		panel.setLayout(new BorderLayout());
//
//		Vector colData;
//		Vector rowData = new Vector();
//		Vector colNames = new Vector();
//
//		colNames.add("Event Kind");
//		colNames.add("Description");
//		colNames.add("Value");
//
//		colData = new Vector();
//		colData.add("");
//		colData.add("Suspend policy: ");
//		colData.add(SuspendPolicy.asString(packet.getData()[index++]));
//		rowData.add(colData);
//
//		int numEvents = (int) getVal(packet, 1, 4);
//		index += 4;		
//		colData = new Vector();
//		colData.add("");
//		colData.add("Number of events: ");
//		colData.add(String.valueOf(numEvents));
//		rowData.add(colData);
//
//		colData = new Vector();
//		colData.add("");
//		colData.add("");
//		colData.add("");
//		rowData.add(colData);
//
//		for (int i = 0; i < numEvents; i++) {
//			byte command = packet.getData()[index++];
//
//			colData = new Vector();
//			colData.add(EventKind.asString(command));
//			colData.add("");
//			colData.add("");
//			rowData.add(colData);
//
//			colData = new Vector();
//			colData.add("");
//			colData.add("Request ID:");
//			colData.add(String.valueOf(getVal(packet, index, 4)));
//			rowData.add(colData);
//			index += 4;
//
//			switch (command) {
//				
//				case EventKind.VM_START:
////					fallthrough
//				case EventKind.THREAD_DEATH:
////					fallthrough
//				case EventKind.THREAD_START:
//					colData = new Vector();
//					colData.add("");
//					colData.add("ThreadID:");
//					colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//					rowData.add(colData);
//					index += objectIDSize;
//					break;
//
//
//				case EventKind.SINGLE_STEP:
//					colData = new Vector();
//					colData.add("");
//					colData.add("ThreadID:");
//					colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//					rowData.add(colData);
//					index += objectIDSize;
//					
//					colData = new Vector();
//					colData.add("");
//					colData.add("Step location:");
//					colData.add(getLocation(packet, index));
//					rowData.add(colData);
//					index += locationSize;
//					break;
//
//				case EventKind.METHOD_ENTRY:
////					fallthrough
//				case EventKind.METHOD_EXIT:
////					fallthrough
//				case EventKind.BREAKPOINT:
//					colData = new Vector();
//					colData.add("");
//					colData.add("ThreadID:");
//					colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//					colData.add("");
//					rowData.add(colData);
//					index += objectIDSize;
//					
//					colData = new Vector();
//					colData.add("");
//					colData.add("Location:");
//					colData.add(getLocation(packet, index));
//					rowData.add(colData);
//					index += locationSize;
//					break;
//
//				case EventKind.EXCEPTION:
//					colData = new Vector();
//					colData.add("");
//					colData.add("ThreadID:");
//					colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//					rowData.add(colData);
//					index += objectIDSize;
//					
//					colData = new Vector();
//					colData.add("");
//					colData.add("Throw location:");
//					colData.add(getLocation(packet, index));
//					rowData.add(colData);
//					index += locationSize;
//					
//					colData = new Vector();
//					colData.add("");
//					colData.add("Exception Object ID:");
//					colData.add(taggedObjectIDToString(packet, index));
//					rowData.add(colData);
//					index += 1 + objectIDSize;
//					
//					colData = new Vector();
//					colData.add("");
//					colData.add("Catch location:");
//					colData.add(getLocation(packet, index));
//					rowData.add(colData);
//					index += locationSize;
//					
//					//ps.println("EXCEPTION: reqID = " + getVal(packet, index, 4));
//					//index += objectIDSize + locationSize + objectIDSize + 1;
//					
//					break;
//					
//				case EventKind.CLASS_PREPARE:
//					colData = new Vector();
//					colData.add("");
//					colData.add("Loader ThreadID:");
//					colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//					rowData.add(colData);
//					index += objectIDSize;
//
//					colData = new Vector();
//					colData.add("");
//					colData.add("Ref TypeTag:");
//					colData.add(TypeTag.asString(packet.getData()[index++]));
//					rowData.add(colData);
//
//					colData = new Vector();
//					colData.add("");
//					colData.add("Ref Type ID:");
//					colData.add(String.valueOf(getVal(packet, index, referenceTypeIDSize)));
//					rowData.add(colData);
//					index += referenceTypeIDSize;
//
//					int sigLen = (int) getVal(packet, index, 4);
//					index += 4;
//					colData = new Vector();
//					colData.add("");
//					colData.add("RefType signature:");
//					colData.add(getStr(packet, index, sigLen));
//					rowData.add(colData);
//					index += sigLen;
//
//					colData = new Vector();
//					colData.add("");
//					colData.add("Status:");
//					colData.add(ClassStatus.asString((int) getVal(packet, index, 4)));
//					rowData.add(colData);
//					index += 4;
//					break;
//
//					//TODO test this case
//				case EventKind.CLASS_UNLOAD:
//					int sigLen2 = (int) getVal(packet, index, 4);
//					index += 4;
//					colData = new Vector();
//					colData.add("");
//					colData.add("RefType signature:");
//					colData.add(getStr(packet, index, sigLen2));
//					rowData.add(colData);
//					index += sigLen2;
//					break;
//
//					//TODO test this case
//				case EventKind.FIELD_ACCESS:
//					colData = new Vector();
//					colData.add("");
//					colData.add("Accessing ThreadID:");
//					colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//					rowData.add(colData);
//					index += objectIDSize;
//
//					colData = new Vector();
//					colData.add("");
//					colData.add("Access location:");
//					colData.add(getLocation(packet, index));
//					rowData.add(colData);
//					index += locationSize;
//
//					colData = new Vector();
//					colData.add("");
//					colData.add("Ref TypeTag:");
//					colData.add(TypeTag.asString(packet.getData()[index++]));
//					rowData.add(colData);
//
//					colData = new Vector();
//					colData.add("");
//					colData.add("Ref Type ID:");
//					colData.add(String.valueOf(getVal(packet, index, referenceTypeIDSize)));
//					rowData.add(colData);
//					index += referenceTypeIDSize;
//
//					colData = new Vector();
//					colData.add("");
//					colData.add("Accessed Field ID:");
//					colData.add(String.valueOf(getVal(packet, index, fieldIDSize)));
//					rowData.add(colData);
//					index += fieldIDSize;
//
//					//taggedObjectIDToString
//					colData = new Vector();
//					colData.add("");
//					colData.add("Accessed Object ID:");
//					colData.add(taggedObjectIDToString(packet, index));
//					rowData.add(colData);
//					index += 1 + objectIDSize;
//					break;
//
//				case EventKind.FIELD_MODIFICATION:
//					colData = new Vector();
//					colData.add("");
//					colData.add("Accessing ThreadID:");
//					colData.add(String.valueOf(getVal(packet, index, objectIDSize)));
//					rowData.add(colData);
//					index += objectIDSize;
//	
//					colData = new Vector();
//					colData.add("");
//					colData.add("Access location:");
//					colData.add(getLocation(packet, index));
//					rowData.add(colData);
//					index += locationSize;
//	
//					colData = new Vector();
//					colData.add("");
//					colData.add("Ref TypeTag:");
//					colData.add(TypeTag.asString(packet.getData()[index++]));
//					rowData.add(colData);
//	
//					colData = new Vector();
//					colData.add("");
//					colData.add("Ref Type ID:");
//					colData.add(String.valueOf(getVal(packet, index, referenceTypeIDSize)));
//					rowData.add(colData);
//					index += referenceTypeIDSize;
//	
//					colData = new Vector();
//					colData.add("");
//					colData.add("Accessed Field ID:");
//					colData.add(String.valueOf(getVal(packet, index, fieldIDSize)));
//					rowData.add(colData);
//					index += fieldIDSize;
//	
//					//taggedObjectIDToString
//					colData = new Vector();
//					colData.add("");
//					colData.add("Accessed Object ID:");
//					colData.add(taggedObjectIDToString(packet, index));
//					rowData.add(colData);
//					index += 1 + objectIDSize;
//
//					byte valueType = packet.getData()[index++];
//					String valueValue = "";
//					switch (valueType) {
//						case Tag.VOID: // VOID
//							valueValue = Tag.asString(valueType);
//							break;
//						case Tag.BYTE: // Byte
//						case Tag.BOOLEAN: // Boolean
//							valueValue = Tag.asString(valueType) + ":" + packet.getData()[index++];
//							break;
//						case Tag.CHAR: // Char
//						case Tag.SHORT: // Short
//							valueValue = Tag.asString(valueType) + ":" + getVal(packet, index, 2);
//							index += 2;
//							break;
//						case Tag.FLOAT: // Float
//						case Tag.INT: // Int
//							valueValue = Tag.asString(valueType) + ":" + getVal(packet, index, 4);
//							index += 4;
//							break;
//						case Tag.DOUBLE: // Double
//						case Tag.LONG: // Long
//							valueValue = Tag.asString(valueType) + ":" + getVal(packet, index, 8);
//							index += 8;
//							break;
//						case Tag.ARRAY: // Array
//						case Tag.OBJECT: // Object
//						case Tag.STRING: // String
//						case Tag.THREAD: // Thread
//						case Tag.THREAD_GROUP: // ThreadGroup
//						case Tag.CLASS_LOADER: // ClassLoader
//						case Tag.CLASS_OBJECT: // ClassObject
//							valueValue = Tag.asString(valueType) + ":" + getVal(packet, index, objectIDSize);
//							index += objectIDSize;
//							break;
//						default:
//							valueValue = "Value type: " + Tag.asString(valueType);
//					}
//					colData = new Vector();
//					colData.add("");
//					colData.add("Value set:");
//					colData.add(valueValue);
//					rowData.add(colData);
//					break;
//					
//				case EventKind.VM_DEATH:
//					break;
//
//				default:
//					throw new RuntimeException("unimplemented command = " + command);
//			}
//		}
//		panel.add(new JScrollPane(new JTable(rowData, colNames)), BorderLayout.CENTER);
//		return panel;
//	}
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_6_1 extends PacketAnalyzer {

    public Request_6_1() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node info = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(info);

        long methId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methType = MethodType.getType(refTypeId, methId);
        Node methInfo = new Node(new Node.Descriptor("MethodID:", methType), new Node.Value(new Long(methId)));
        root.addChild(methInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_6_2 extends PacketAnalyzer {

    public Request_6_2() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node info = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(info);

        long methId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methType = MethodType.getType(refTypeId, methId);
        Node methInfo = new Node(new Node.Descriptor("MethodID:", methType), new Node.Value(new Long(methId)));
        root.addChild(methInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_6_3 extends PacketAnalyzer {

    public Request_6_3() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node info = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(info);

        long methId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methType = MethodType.getType(refTypeId, methId);
        Node methInfo = new Node(new Node.Descriptor("MethodID:", methType), new Node.Value(new Long(methId)));
        root.addChild(methInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_6_4 extends PacketAnalyzer {

    public Request_6_4() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node info = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(info);

        long methId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methType = MethodType.getType(refTypeId, methId);
        Node methInfo = new Node(new Node.Descriptor("MethodID:", methType), new Node.Value(new Long(methId)));
        root.addChild(methInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_6_5 extends PacketAnalyzer {

    public Request_6_5() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node info = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(info);

        long methId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methType = MethodType.getType(refTypeId, methId);
        Node methInfo = new Node(new Node.Descriptor("MethodID:", methType), new Node.Value(new Long(methId)));
        root.addChild(methInfo);

        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_1 extends PacketAnalyzer {

    public Request_9_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_2 extends PacketAnalyzer {

    public Request_9_2() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objectType = ObjectType.getType(objTypeId);
        Node objInfo = new Node(new Node.Descriptor("ObjTypeId:", objectType), new Node.Value(new Long(objTypeId)));
        root.addChild(objInfo);

        int numFields = (int) getVal(packet, index, 4);
        index += 4;
        Node fieldsInfo = new Node(new Node.Descriptor("NumFields:"), new Node.Value(new Integer(numFields)));
        root.addChild(fieldsInfo);

        for (int i = 0; i < numFields; i++) {
            long fieldID = (getVal(packet, index, fieldIDSize));
            index += fieldIDSize;
            Node singleFieldInfo = new Node(new Node.Descriptor("FieldID:"), new Node.Value(new Long(fieldID)));
            fieldsInfo.addChild(singleFieldInfo);
        }
        return root;
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_3 extends PacketAnalyzer {

    public Request_9_3() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long objectTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objectType = ObjectType.getType(objectTypeId);
        Node objInfo = new Node(new Node.Descriptor("ObjTypeId:", objectType), new Node.Value(new Long(objectTypeId)));
        root.addChild(objInfo);

        int numFields = (int) getVal(packet, index, 4);
        index += 4;
        Node valuesInfo = new Node(new Node.Descriptor("NumValues:"), new Node.Value(new Integer(numFields)));
        root.addChild(valuesInfo);

        //TODO : VALUE + updateInternalDataModel
        return root;

    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_5 extends PacketAnalyzer {

    public Request_9_5() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_6 extends PacketAnalyzer {

    public Request_9_6() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objectType = ObjectType.getType(objTypeId);
        Node objInfo = new Node(new Node.Descriptor("RefTypeID:", objectType), new Node.Value(new Long(objTypeId)));
        root.addChild(objInfo);

        long threadId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType threadType = ObjectType.getType(threadId);
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:", threadType), new Node.Value(new Long(threadId)));
        root.addChild(threadInfo);

        long refTypeId = getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType referenceType = ReferenceType.getType(refTypeId);
        Node refInfo = new Node(new Node.Descriptor("ClassID:", referenceType), new Node.Value(new Long(refTypeId)));
        root.addChild(refInfo);

        long methodId = getVal(packet, index, methodIDSize);
        index += methodIDSize;
        MethodType methodType = MethodType.getType(refTypeId, methodId);
        Node methodInfo = new Node(new Node.Descriptor("MethodID:", methodType), new Node.Value(new Long(methodId)));
        root.addChild(methodInfo);

        int numargs = (int) getVal(packet, index, 4);
        index += 4;

        Node argsInfo = new Node(new Node.Descriptor("NumArgs:"), new Node.Value(new Integer(numargs)));
        root.addChild(argsInfo);

        for (int i = 0; i < numargs; i++) {
            Value value = getValue(packet, index);
            index += value.getOffset();
            Node singleArgInfo = new Node(new Node.Descriptor("ArgValue:"), new Node.Value(value));
            argsInfo.addChild(singleArgInfo);
        }

        int options = (int) getVal(packet, index, 4);
        index += 4;
        Node optionsInfo = new Node(new Node.Descriptor("Options:"), new Node.Value(new Integer(options), getInvokeOptionsAsStr(options)));
        root.addChild(optionsInfo);

        return root;
    }
}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_7 extends PacketAnalyzer {

    public Request_9_7() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_8 extends PacketAnalyzer {

    public Request_9_8() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

/*
 * The contents of this file are subject to the terms of the Common Development
 * and Distribution License (the License). You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at http://www.netbeans.org/cddl.html
 * or http://www.netbeans.org/cddl.txt.
 *
 * When distributing Covered Code, include this CDDL Header Notice in each file
 * and include the License file at http://www.netbeans.org/cddl.txt.
 * If applicable, add the following below the CDDL Header, with the fields
 * enclosed by brackets [] replaced by your own identifying information:
 * "Portions Copyrighted [year] [name of copyright owner]"
 *
 * The Original Software is NetBeans. The Initial Developer of the Original
 * Software is Sun Microsystems, Inc. Portions Copyright 1997-2006 Sun
 * Microsystems, Inc. All Rights Reserved.
 */
/**
 * @author karel herink
 */
class Request_9_9 extends PacketAnalyzer {

    public Request_9_9() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long objTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objType = ObjectType.getType(objTypeId);
        return new Node(new Node.Descriptor("ObjectID:", objType), new Node.Value(new Long(objTypeId)));
    }

}

