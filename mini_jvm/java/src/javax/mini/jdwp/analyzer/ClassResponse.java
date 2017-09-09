package javax.mini.jdwp.analyzer;

import javax.mini.jdwp.constant.ClassStatus;
import javax.mini.jdwp.constant.SuspendStatus;
import javax.mini.jdwp.constant.ThreadStatus;
import javax.mini.jdwp.constant.TypeTag;
import javax.mini.jdwp.net.Packet;
import javax.mini.jdwp.net.Request;
import javax.mini.jdwp.reflect.FieldType;
import javax.mini.jdwp.reflect.MethodType;
import javax.mini.jdwp.reflect.ObjectType;
import javax.mini.jdwp.reflect.ReferenceType;
import javax.mini.util.ArrayList;
import javax.mini.util.Iterator;
import javax.mini.util.List;

public class ClassResponse {
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
class Response_10_1 extends PacketAnalyzer {

    public Response_10_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        String value = (String) node.getValue().getRealValue();
        objectType.setStringValue(value);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String value = getStr(packet, index, len);
        index += len;
        Node root = new Node(new Node.Descriptor("StringValue:"), new Node.Value(value));

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
class Response_11_1 extends PacketAnalyzer {

    public Response_11_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        String threadName = (String) node.getValue().getRealValue();
        objectType.setThreadName(threadName);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String value = getStr(packet, index, len);
        index += len;
        Node root = new Node(new Node.Descriptor("ThreadName:"), new Node.Value(value));

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
class Response_11_12 extends PacketAnalyzer {

    public Response_11_12() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        Integer suspendCount = (Integer) node.getValue().getRealValue();
        objectType.setSuspendCount(suspendCount);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int suspendCount = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("SuspendCount:"), new Node.Value(new Integer(suspendCount)));

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
class Response_11_4 extends PacketAnalyzer {

    public Response_11_4() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        List nodes = this.getPacketInfo(packet).getChildren();
        Integer threadStatus = (Integer) ((Node) nodes.get(0)).getValue().getRealValue();
        objectType.setThreadStatus(threadStatus);
        Integer suspendStatus = (Integer) ((Node) nodes.get(0)).getValue().getRealValue();
        objectType.setSuspendStatus(suspendStatus);
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        int threadStatus = (int) getVal(packet, index, 4);
        index += 4;
        Node threadStatusInfo = new Node(new Node.Descriptor("ThreadStatus:"), new Node.Value(new Integer(threadStatus), ThreadStatus.asString(threadStatus)));
        root.addChild(threadStatusInfo);

        int suspendStatus = (int) getVal(packet, index, 4);
        index += 4;
        Node suspendStatusInfo = new Node(new Node.Descriptor("SuspendStatus:"), new Node.Value(new Integer(suspendStatus), SuspendStatus.asString(suspendStatus)));
        root.addChild(suspendStatusInfo);

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
class Response_11_5 extends PacketAnalyzer {

    public Response_11_5() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        Long threadGroupID = (Long) node.getValue().getRealValue();
        objectType.setThreadGroupId(threadGroupID);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        long threadGroupID = (int) getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType threadGroupType = ObjectType.getType(threadGroupID);
        Node root = new Node(new Node.Descriptor("ThreadGroupID:", threadGroupType), new Node.Value(new Long(threadGroupID)));

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
class Response_11_6 extends PacketAnalyzer {

    public Response_11_6() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numFrames = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("NumFrames:"), new Node.Value(new Integer(numFrames)));

        for (int i = 0; i < numFrames; i++) {

            long frameId = getVal(packet, index, frameIDSize);
            index += frameIDSize;
            Node frameIdInfo = new Node(new Node.Descriptor("FrameID:"), new Node.Value(new Long(frameId)));
            root.addChild(frameIdInfo);
            Location location = getLocation(packet, index);
            index += locationSize;
            Node locationInfo = new Node(new Node.Descriptor("Location:"), new Node.Value(location));
            root.addChild(locationInfo);
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
class Response_11_7 extends PacketAnalyzer {

    public Response_11_7() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int frameCount = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("FrameCount:"), new Node.Value(new Integer(frameCount)));

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
class Response_11_8 extends PacketAnalyzer {

    public Response_11_8() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numMonitors = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("NumMonitors:"), new Node.Value(new Integer(numMonitors)));

        for (int i = 0; i < numMonitors; i++) {

            Value value = getValue(packet, index);
            index += value.getOffset();

            //TODO - will this work?? The value here should be a Long objectId
            Long monitorObjectId = (Long) value.getValue();
            ObjectType monitorObjectType = ObjectType.getType(monitorObjectId.longValue());
            Node monitorInfo = new Node(new Node.Descriptor("Monitor:", monitorObjectType), new Node.Value(value));

            root.addChild(monitorInfo);
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
//		int numOwnedMonitors = (int) getVal(packet, index, 4);
//		index += 4;
//		colData = new Vector();
//		colData.add("Num Owned Monitors:");
//		colData.add(String.valueOf(numOwnedMonitors));
//		rowData.add(colData);
//
//		colData = new Vector();
//		colData.add("");
//		colData.add("");
//		rowData.add(colData);
//
//		for(int i = 0; i < numOwnedMonitors; i++) {
//			colData = new Vector();
//			colData.add("Monitor:");
//			Value value = getValue(packet, index);
//			colData.add(value.getValue());
//			rowData.add(colData);
//			index += value.getOffset();
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
class Response_11_9 extends PacketAnalyzer {

    public Response_11_9() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        Value value = getValue(packet, index);
        index += value.getOffset();
        Long objectId = (Long) value.getValue();
        ObjectType objectType = ObjectType.getType(objectId);
        Node singleValueInfo = new Node(new Node.Descriptor("ContendedMonitor:", objectType), new Node.Value(value));

        return singleValueInfo;
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
//		colNames.add("Description");
//		colNames.add("Value");
//
//		colData = new Vector();
//		colData.add("Contendent Monitor:");
//		Value value = getValue(packet, index);
//		colData.add(value.getValue());
//		rowData.add(colData);
//		index += value.getOffset();
//
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
class Response_12_1 extends PacketAnalyzer {

    public Response_12_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        String threadGroupName = (String) node.getValue().getRealValue();
        objectType.setThreadGroupName(threadGroupName);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String value = getStr(packet, index, len);
        index += len;
        Node root = new Node(new Node.Descriptor("ThreadGroupName:"), new Node.Value(value));

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
class Response_12_2 extends PacketAnalyzer {

    public Response_12_2() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        Long threadGroupID = (Long) node.getValue().getRealValue();
        objectType.setParentGroupId(threadGroupID);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        long threadGroupID = (int) getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType threadGroupType = ObjectType.getType(threadGroupID);
        Node root = new Node(new Node.Descriptor("ParentThreadGroupID:", threadGroupType), new Node.Value(new Long(threadGroupID)));

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
class Response_12_3 extends PacketAnalyzer {

    public Response_12_3() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        ArrayList childThreads = new ArrayList();
        ArrayList childThreadGroups = new ArrayList();

        ArrayList nodes = this.getPacketInfo(packet).getChildren();

        ArrayList threadNodes = ((Node) nodes.get(0)).getChildren();
        for (Iterator iter = threadNodes.iterator(); iter.hasNext();) {
            Node threadNode = (Node) iter.next();
            Long threadId = (Long) threadNode.getValue().getRealValue();
            childThreads.add(threadId);
        }
        objectType.setChildThreadIds((Long[]) childThreads.toArray(new Long[childThreads.size()]));

        ArrayList threadGroupNodes = ((Node) nodes.get(1)).getChildren();
        for (Iterator iter = threadGroupNodes.iterator(); iter.hasNext();) {
            Node threadGroupNode = (Node) iter.next();
            Long threadGroupId = (Long) threadGroupNode.getValue().getRealValue();
            childThreads.add(threadGroupId);
        }
        objectType.setChildThreadGroupIds((Long[]) childThreadGroups.toArray(new Long[childThreadGroups.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        int numThreads = (int) getVal(packet, index, 4);
        index += 4;
        Node threadsInfo = new Node(new Node.Descriptor("NumThreads:"), new Node.Value(new Integer(numThreads)));

        for (int i = 0; i < numThreads; i++) {
            long threadId = getVal(packet, index, objectIDSize);
            index += objectIDSize;
            ObjectType objectType = ObjectType.getType(threadId);
            Node threadInfo = new Node(new Node.Descriptor("ThreadId:", objectType), new Node.Value(new Long(threadId)));
            threadsInfo.addChild(threadInfo);
        }
        root.addChild(threadsInfo);

        int numThreadGroups = (int) getVal(packet, index, 4);
        index += 4;
        Node threadGroupsInfo = new Node(new Node.Descriptor("NumThreadGroups:"), new Node.Value(new Integer(numThreadGroups)));

        for (int i = 0; i < numThreadGroups; i++) {
            long threadGroupId = getVal(packet, index, objectIDSize);
            index += objectIDSize;
            ObjectType objectType = ObjectType.getType(threadGroupId);
            Node threadGroupInfo = new Node(new Node.Descriptor("ThreadGroupId:", objectType), new Node.Value(new Long(threadGroupId)));
            threadGroupsInfo.addChild(threadGroupInfo);
        }
        root.addChild(threadGroupsInfo);
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
class Response_13_1 extends PacketAnalyzer {

    public Response_13_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        Integer length = (Integer) node.getValue().getRealValue();
        objectType.setSuspendCount(length);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int length = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("Length:"), new Node.Value(new Integer(length)));

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
class Response_13_2 extends PacketAnalyzer {

    public Response_13_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        Value[] values = getArrayRegion(packet, index);
        for (int i = 0; i < values.length; i++) {
            Node valueInfo = new Node(new Node.Descriptor("Value:"), new Node.Value(values[i].getValue()));
            root.addChild(valueInfo);
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
class Response_14_1 extends PacketAnalyzer {

    public Response_14_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numClasses = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("NumClasses:"), new Node.Value(new Integer(numClasses)));

        for (int i = 0; i < numClasses; i++) {

            byte typeTag = packet.getData()[index++];
            Node typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Integer(typeTag), TypeTag.asString(typeTag)));
            root.addChild(typeTagInfo);

            long refTypeId = getVal(packet, index, referenceTypeIDSize);
            index += referenceTypeIDSize;
            ReferenceType refType = ReferenceType.getType(refTypeId);
            Node refTypeInfo = new Node(new Node.Descriptor("RefTypeID:", refType), new Node.Value(new Long(refTypeId)));
            root.addChild(refTypeInfo);
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
class Response_15_1 extends PacketAnalyzer {

    public Response_15_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int requestId = (int) getVal(packet, index, 4);
        index += 4;
        //RequestType objType = RequestType.getType(objTypeId);
        return new Node(new Node.Descriptor("RequestID:"/*, objType*/), new Node.Value(new Integer(requestId)));
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
class Response_16_1 extends PacketAnalyzer {

    public Response_16_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numValues = (int) getVal(packet, index, 4);
        index += 4;
        Node valuesInfo = new Node(new Node.Descriptor("NumValues:"), new Node.Value(new Integer(numValues)));

        for (int i = 0; i < numValues; i++) {
            Value value = getValue(packet, index);
            index += value.getOffset();

            Node singleValueInfo = new Node(new Node.Descriptor("Value:"), new Node.Value(value));
            valuesInfo.addChild(singleValueInfo);
        }
        return valuesInfo;
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
class Response_16_3 extends PacketAnalyzer {

    public Response_16_3() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        Value value = getValue(packet, index);
        index += value.getOffset();
        Long objectId = (Long) value.getValue();
        ObjectType objectType = ObjectType.getType(objectId);
        Node singleValueInfo = new Node(new Node.Descriptor("'This' ObjectID:", objectType), new Node.Value(value));

        return singleValueInfo;
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
//		colData.add("'This' Object:");
//		colData.add(taggedObjectIDToString(packet, index));
//		rowData.add(colData);
//		index += 1 + objectIDSize;
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
class Response_17_1 extends PacketAnalyzer {

    public Response_17_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        ArrayList nodes = getPacketInfo(packet).getChildren();
        Node refTypeIdInfo = (Node) nodes.get(1);
        Long refTypeId = (Long) refTypeIdInfo.getValue().getRealValue();
        objectType.setReferenceTypeId(refTypeId);
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        byte typeTag = packet.getData()[index++];
        Node tagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Integer(typeTag), TypeTag.asString(typeTag)));
        root.addChild(tagInfo);

        long referenceTypeId = (int) getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType refType = ReferenceType.getType(referenceTypeId);
        Node referenceTypeIdInfo = new Node(new Node.Descriptor("RefTypeID:", refType), new Node.Value(new Long(referenceTypeId)));
        root.addChild(referenceTypeIdInfo);

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
class Response_1_1 extends PacketAnalyzer {

    public Response_1_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String desc = getStr(packet, index, len);
        index += len;
        Node descInfo = new Node(new Node.Descriptor("Description:"), new Node.Value(desc));
        root.addChild(descInfo);

        int major = (int) getVal(packet, index, 4);
        index += 4;
        Node majorInfo = new Node(new Node.Descriptor("JDWP Major:"), new Node.Value(new Integer(major)));
        root.addChild(majorInfo);

        int minor = (int) getVal(packet, index, 4);
        index += 4;
        Node minorInfo = new Node(new Node.Descriptor("JDWP Minor:"), new Node.Value(new Integer(minor)));
        root.addChild(minorInfo);

        len = (int) getVal(packet, index, 4);
        index += 4;
        String version = getStr(packet, index, len);
        index += len;
        Node versionInfo = new Node(new Node.Descriptor("VM Version:"), new Node.Value(version));
        root.addChild(versionInfo);

        len = (int) getVal(packet, index, 4);
        index += 4;
        String name = getStr(packet, index, len);
        index += len;
        Node nameInfo = new Node(new Node.Descriptor("VM Name:"), new Node.Value(name));
        root.addChild(nameInfo);

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
class Response_1_11 extends PacketAnalyzer {

    public Response_1_11() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        String strValue = (String) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();

        Node idNode = this.getPacketInfo(packet);
        Long id = (Long) idNode.getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(id);

        objectType.setStringValue(strValue);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        long strId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType objectType = ObjectType.getType(strId);
        Node strInfo = new Node(new Node.Descriptor("StringId:", objectType), new Node.Value(new Long(strId)));

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
class Response_1_12 extends PacketAnalyzer {

    public Response_1_12() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(new Node.Descriptor("VM Abilities:"), null);

        boolean watchFieldModification = getBoolean(packet, index++);
        Node watchFieldModificationInfo = new Node(new Node.Descriptor("WatchFieldModification:"), new Node.Value(new Boolean(watchFieldModification)));
        root.addChild(watchFieldModificationInfo);

        boolean watchFieldAccess = getBoolean(packet, index++);
        Node watchFieldAccessInfo = new Node(new Node.Descriptor("WatchFieldAccess:"), new Node.Value(new Boolean(watchFieldAccess)));
        root.addChild(watchFieldAccessInfo);

        boolean getBytecodes = getBoolean(packet, index++);
        Node getBytecodesInfo = new Node(new Node.Descriptor("GetBytecodes:"), new Node.Value(new Boolean(getBytecodes)));
        root.addChild(getBytecodesInfo);

        boolean getSyntheticAttribute = getBoolean(packet, index++);
        Node getSyntheticAttributeInfo = new Node(new Node.Descriptor("GetSyntheticAttribute:"), new Node.Value(new Boolean(getSyntheticAttribute)));
        root.addChild(getSyntheticAttributeInfo);

        boolean getOwnedMonitor = getBoolean(packet, index++);
        Node getOwnedMonitorInfo = new Node(new Node.Descriptor("GetOwnedMonitor:"), new Node.Value(new Boolean(getOwnedMonitor)));
        root.addChild(getOwnedMonitorInfo);

        boolean getCurrentContendedMonitor = getBoolean(packet, index++);
        Node getCurrentContendedMonitorInfo = new Node(new Node.Descriptor("GetCurrentContendedMonitor:"), new Node.Value(new Boolean(getCurrentContendedMonitor)));
        root.addChild(getCurrentContendedMonitorInfo);

        boolean getMonitor = getBoolean(packet, index++);
        Node getMonitorInfo = new Node(new Node.Descriptor("GetMonitor:"), new Node.Value(new Boolean(getMonitor)));
        root.addChild(getMonitorInfo);

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
class Response_1_13 extends PacketAnalyzer {

    public Response_1_13() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(null, null);

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String basedir = getStr(packet, index, len);
        index += len;
        Node basedirInfo = new Node(new Node.Descriptor("BaseDir:"), new Node.Value(basedir));
        root.addChild(basedirInfo);

        int numClassPaths = (int) getVal(packet, index, 4);
        index += 4;
        Node classPathsInfo = new Node(new Node.Descriptor("NumClassPaths:"), new Node.Value(new Integer(numClassPaths)));

        for (int i = 0; i < numClassPaths; i++) {
            len = (int) getVal(packet, index, 4);
            index += 4;
            String classPath = getStr(packet, index, len);
            index += len;
            Node classPathInfo = new Node(new Node.Descriptor("ClassPath:"), new Node.Value(classPath));
            classPathsInfo.addChild(classPathInfo);
        }
        root.addChild(classPathsInfo);

        int numBootClassPaths = (int) getVal(packet, index, 4);
        index += 4;
        Node bootClassPathsInfo = new Node(new Node.Descriptor("NumBootClassPaths:"), new Node.Value(new Integer(numBootClassPaths)));

        for (int i = 0; i < numBootClassPaths; i++) {
            len = (int) getVal(packet, index, 4);
            index += 4;
            String bootClassPath = getStr(packet, index, len);
            index += len;
            Node bootClassPathInfo = new Node(new Node.Descriptor("BootClassPath:"), new Node.Value(bootClassPath));
            classPathsInfo.addChild(bootClassPathInfo);
        }
        root.addChild(bootClassPathsInfo);

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
class Response_1_17 extends PacketAnalyzer {

    public Response_1_17() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Node root = new Node(new Node.Descriptor("VM Abilities New:"), null);

        boolean watchFieldModification = getBoolean(packet, index++);
        Node watchFieldModificationInfo = new Node(new Node.Descriptor("WatchFieldModification:"), new Node.Value(new Boolean(watchFieldModification)));
        root.addChild(watchFieldModificationInfo);

        boolean watchFieldAccess = getBoolean(packet, index++);
        Node watchFieldAccessInfo = new Node(new Node.Descriptor("WatchFieldAccess:"), new Node.Value(new Boolean(watchFieldAccess)));
        root.addChild(watchFieldAccessInfo);

        boolean getBytecodes = getBoolean(packet, index++);
        Node getBytecodesInfo = new Node(new Node.Descriptor("GetBytecodes:"), new Node.Value(new Boolean(getBytecodes)));
        root.addChild(getBytecodesInfo);

        boolean getSyntheticAttribute = getBoolean(packet, index++);
        Node getSyntheticAttributeInfo = new Node(new Node.Descriptor("GetSyntheticAttribute:"), new Node.Value(new Boolean(getSyntheticAttribute)));
        root.addChild(getSyntheticAttributeInfo);

        boolean getOwnedMonitor = getBoolean(packet, index++);
        Node getOwnedMonitorInfo = new Node(new Node.Descriptor("GetOwnedMonitor:"), new Node.Value(new Boolean(getOwnedMonitor)));
        root.addChild(getOwnedMonitorInfo);

        boolean getCurrentContendedMonitor = getBoolean(packet, index++);
        Node getCurrentContendedMonitorInfo = new Node(new Node.Descriptor("GetCurrentContendedMonitor:"), new Node.Value(new Boolean(getCurrentContendedMonitor)));
        root.addChild(getCurrentContendedMonitorInfo);

        boolean getMonitor = getBoolean(packet, index++);
        Node getMonitorInfo = new Node(new Node.Descriptor("GetMonitor:"), new Node.Value(new Boolean(getMonitor)));
        root.addChild(getMonitorInfo);

        boolean RedefineClasses = getBoolean(packet, index++);
        Node RedefineClassesInfo = new Node(new Node.Descriptor("RedefineClasses:"), new Node.Value(new Boolean(RedefineClasses)));
        root.addChild(RedefineClassesInfo);

        boolean AddMethod = getBoolean(packet, index++);
        Node AddMethodInfo = new Node(new Node.Descriptor("AddMethod:"), new Node.Value(new Boolean(AddMethod)));
        root.addChild(AddMethodInfo);

        boolean UnrestrictedlyRedefineClasses = getBoolean(packet, index++);
        Node UnrestrictedlyRedefineClassesInfo = new Node(new Node.Descriptor("UnrestrictedlyRedefineClasses:"), new Node.Value(new Boolean(UnrestrictedlyRedefineClasses)));
        root.addChild(UnrestrictedlyRedefineClassesInfo);

        boolean PopFrames = getBoolean(packet, index++);
        Node PopFramesInfo = new Node(new Node.Descriptor("PopFrames:"), new Node.Value(new Boolean(PopFrames)));
        root.addChild(PopFramesInfo);

        boolean UseInstanceFilters = getBoolean(packet, index++);
        Node UseInstanceFiltersInfo = new Node(new Node.Descriptor("UseInstanceFilters:"), new Node.Value(new Boolean(UseInstanceFilters)));
        root.addChild(UseInstanceFiltersInfo);

        boolean GetSourceDebugExtension = getBoolean(packet, index++);
        Node GetSourceDebugExtensionInfo = new Node(new Node.Descriptor("GetSourceDebugExtension:"), new Node.Value(new Boolean(GetSourceDebugExtension)));
        root.addChild(GetSourceDebugExtensionInfo);

        boolean RequestVMDeathEvent = getBoolean(packet, index++);
        Node RequestVMDeathEventInfo = new Node(new Node.Descriptor("RequestVMDeathEvent:"), new Node.Value(new Boolean(RequestVMDeathEvent)));
        root.addChild(RequestVMDeathEventInfo);

        boolean SetDefaultStratum = getBoolean(packet, index++);
        Node SetDefaultStratumInfo = new Node(new Node.Descriptor("SetDefaultStratum:"), new Node.Value(new Boolean(SetDefaultStratum)));
        root.addChild(SetDefaultStratumInfo);

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
class Response_1_2 extends PacketAnalyzer {

    public Response_1_2() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        String signature = (String) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();

        ArrayList classesInfo = this.getPacketInfo(packet).getChildren();
        for (int i = 0; i < classesInfo.size(); i++) {
            Node singleClassInfo = (Node) classesInfo.get(i);
            Long refTypeId = (Long) singleClassInfo.getValue().getRealValue();
            ReferenceType refType = ReferenceType.getType(refTypeId);

            refType.setSignature(signature);

            Node typeTag = (Node) singleClassInfo.getChildren().get(0);
            refType.setTypeTag((Byte) typeTag.getValue().getRealValue());

            Node typeStatus = (Node) singleClassInfo.getChildren().get(1);
            refType.setStatus((Integer) typeStatus.getValue().getRealValue());
        }
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numClasses = (int) getVal(packet, index, 4);
        index += 4;
        Node classesInfo = new Node(new Node.Descriptor("NumClasses:"), new Node.Value(new Integer(numClasses)));

        for (int i = 0; i < numClasses; i++) {
            byte typeTag = (byte) (packet.getData()[index++] & 0xff);

            long refTypeId = getVal(packet, index, referenceTypeIDSize);
            index += referenceTypeIDSize;

            int classStatus = (int) getVal(packet, index, 4);
            index += 4;

            ReferenceType referenceType = ReferenceType.getType(refTypeId);
            Node singleClassInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
            classesInfo.addChild(singleClassInfo);

            Node typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Byte(typeTag), TypeTag.asString(typeTag)));
            singleClassInfo.addChild(typeTagInfo);

            Node typeStatusInfo = new Node(new Node.Descriptor("TypeStatus:"), new Node.Value(new Integer(classStatus), ClassStatus.asString(classStatus)));
            singleClassInfo.addChild(typeStatusInfo);
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
class Response_1_20 extends PacketAnalyzer {

    public Response_1_20() {
    }

    public void updateInternalDataModel(Packet packet) {

        List classesInfo = this.getPacketInfo(packet).getChildren();
        for (int i = 0; i < classesInfo.size(); i++) {
            Node singleClassInfo = (Node) classesInfo.get(i);
            Long refTypeId = (Long) singleClassInfo.getValue().getRealValue();
            ReferenceType refType = ReferenceType.getType(refTypeId);

            Node typeTag = (Node) singleClassInfo.getChildren().get(0);
            refType.setTypeTag((Byte) typeTag.getValue().getRealValue());

            Node typeStatus = (Node) singleClassInfo.getChildren().get(1);
            refType.setStatus((Integer) typeStatus.getValue().getRealValue());

            Node sig = (Node) singleClassInfo.getChildren().get(2);
            refType.setSignature((String) sig.getValue().getRealValue());

            Node genSig = (Node) singleClassInfo.getChildren().get(3);
            refType.setSignatureWithGeneric((String) genSig.getValue().getRealValue());
        }
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numClasses = (int) getVal(packet, index, 4);
        index += 4;
        Node classesInfo = new Node(new Node.Descriptor("NumClasses:"), new Node.Value(new Integer(numClasses)));

        for (int i = 0; i < numClasses; i++) {
            byte typeTag = (byte) (packet.getData()[index++] & 0xff);

            long refTypeId = getVal(packet, index, referenceTypeIDSize);
            index += referenceTypeIDSize;

            int len = (int) getVal(packet, index, 4);
            index += 4;
            String signature = getStr(packet, index, len);
            index += len;

            len = (int) getVal(packet, index, 4);
            index += 4;
            String genSignature = getStr(packet, index, len);
            index += len;

            int classStatus = (int) getVal(packet, index, 4);
            index += 4;

            ReferenceType referenceType = ReferenceType.getType(refTypeId);
            Node singleClassInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
            classesInfo.addChild(singleClassInfo);

            Node typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Byte(typeTag), TypeTag.asString(typeTag)));
            singleClassInfo.addChild(typeTagInfo);

            Node typeStatusInfo = new Node(new Node.Descriptor("TypeStatus:"), new Node.Value(new Integer(classStatus), ClassStatus.asString(classStatus)));
            singleClassInfo.addChild(typeStatusInfo);

            Node signatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(signature));
            singleClassInfo.addChild(signatureInfo);

            Node genSignatureInfo = new Node(new Node.Descriptor("GenericSignature:"), new Node.Value(genSignature));
            singleClassInfo.addChild(genSignatureInfo);

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
class Response_1_3 extends PacketAnalyzer {

    public Response_1_3() {
    }

    public void updateInternalDataModel(Packet packet) {

        List classesInfo = this.getPacketInfo(packet).getChildren();
        for (int i = 0; i < classesInfo.size(); i++) {
            Node singleClassInfo = (Node) classesInfo.get(i);
            Long refTypeId = (Long) singleClassInfo.getValue().getRealValue();
            ReferenceType refType = ReferenceType.getType(refTypeId);

            Node typeTag = (Node) singleClassInfo.getChildren().get(0);
            refType.setTypeTag((Byte) typeTag.getValue().getRealValue());

            Node typeStatus = (Node) singleClassInfo.getChildren().get(1);
            refType.setStatus((Integer) typeStatus.getValue().getRealValue());

            Node sig = (Node) singleClassInfo.getChildren().get(2);
            refType.setSignature((String) sig.getValue().getRealValue());
        }
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numClasses = (int) getVal(packet, index, 4);
        index += 4;
        Node classesInfo = new Node(new Node.Descriptor("NumClasses:"), new Node.Value(new Integer(numClasses)));

        for (int i = 0; i < numClasses; i++) {
            byte typeTag = (byte) (packet.getData()[index++] & 0xff);

            long refTypeId = getVal(packet, index, referenceTypeIDSize);
            index += referenceTypeIDSize;

            int len = (int) getVal(packet, index, 4);
            index += 4;
            String signature = getStr(packet, index, len);
            index += len;

            int classStatus = (int) getVal(packet, index, 4);
            index += 4;

            ReferenceType referenceType = ReferenceType.getType(refTypeId);
            Node singleClassInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
            classesInfo.addChild(singleClassInfo);

            Node typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Byte(typeTag), TypeTag.asString(typeTag)));
            singleClassInfo.addChild(typeTagInfo);

            Node typeStatusInfo = new Node(new Node.Descriptor("TypeStatus:"), new Node.Value(new Integer(classStatus), ClassStatus.asString(classStatus)));
            singleClassInfo.addChild(typeStatusInfo);

            Node signatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(signature));
            singleClassInfo.addChild(signatureInfo);

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
class Response_1_4 extends PacketAnalyzer {

    public Response_1_4() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numThreads = (int) getVal(packet, index, 4);
        index += 4;
        Node threadsInfo = new Node(new Node.Descriptor("NumThreads:"), new Node.Value(new Integer(numThreads)));

        for (int i = 0; i < numThreads; i++) {
            long threadId = getVal(packet, index, objectIDSize);
            index += objectIDSize;
            ObjectType objectType = ObjectType.getType(threadId);
            Node threadInfo = new Node(new Node.Descriptor("ThreadId:", objectType), new Node.Value(new Long(threadId)));
            threadsInfo.addChild(threadInfo);
        }

        return threadsInfo;
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
class Response_1_5 extends PacketAnalyzer {

    public Response_1_5() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numThreads = (int) getVal(packet, index, 4);
        index += 4;
        Node threadsInfo = new Node(new Node.Descriptor("NumThreadGroups:"), new Node.Value(new Integer(numThreads)));

        for (int i = 0; i < numThreads; i++) {
            long threadId = getVal(packet, index, objectIDSize);
            index += objectIDSize;
            ObjectType objectType = ObjectType.getType(threadId);
            Node threadInfo = new Node(new Node.Descriptor("ThreadGroupId:", objectType), new Node.Value(new Long(threadId)));
            threadsInfo.addChild(threadInfo);
        }
        return threadsInfo;
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
class Response_1_7 extends PacketAnalyzer {

    public Response_1_7() {
    }

    public void updateInternalDataModel(Packet packet) {
        int[] sizes = this.getSizes(packet);
        PacketAnalyzer.setIDSizes(sizes);
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);

        int fieldSize = (int) getVal(packet, 0, 4);
        Node fieldSizeInfo = new Node(new Node.Descriptor("Field:"), new Node.Value(new Integer(fieldSize)));
        root.addChild(fieldSizeInfo);

        int methSize = (int) getVal(packet, 4, 4);
        Node methSizeInfo = new Node(new Node.Descriptor("Method:"), new Node.Value(new Integer(methSize)));
        root.addChild(methSizeInfo);

        int objSize = (int) getVal(packet, 8, 4);
        Node objSizeInfo = new Node(new Node.Descriptor("Object:"), new Node.Value(new Integer(objSize)));
        root.addChild(objSizeInfo);

        int refSize = (int) getVal(packet, 12, 4);
        Node refSizeInfo = new Node(new Node.Descriptor("RefType:"), new Node.Value(new Integer(refSize)));
        root.addChild(refSizeInfo);

        int frameSize = (int) getVal(packet, 16, 4);
        Node frameSizeInfo = new Node(new Node.Descriptor("Frame:"), new Node.Value(new Integer(frameSize)));
        root.addChild(frameSizeInfo);

        return root;
    }

    private synchronized int[] getSizes(Packet packet) {
        int[] sizes = new int[]{
            (int) getVal(packet, 0, 4),
            (int) getVal(packet, 4, 4),
            (int) getVal(packet, 8, 4),
            (int) getVal(packet, 12, 4),
            (int) getVal(packet, 16, 4),};
        return sizes;
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
class Response_2_1 extends PacketAnalyzer {

    public Response_2_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        String signature = (String) this.getPacketInfo(packet).getValue().getRealValue();
        System.out.println("Setting signature: " + signature + " for refTypeId: " + referenceTypeId);
        referenceType.setSignature(signature);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int len = (int) getVal(packet, index, 4);
        index += 4;
        String signature = getStr(packet, index, len);
        index += len;
        Node node = new Node(new Node.Descriptor("Signature"), new Node.Value(signature));
        return node;
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
class Response_2_10 extends PacketAnalyzer {

    public Response_2_10() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        List interfacesRefTypeIds = new ArrayList();
        List interfacesInfo = this.getPacketInfo(packet).getChildren();

        for (int i = 0; i < interfacesInfo.size(); i++) {
            Node singleInterfaceInfo = (Node) interfacesInfo.get(i);
            Long interfaceRefTypeId = (Long) singleInterfaceInfo.getValue().getRealValue();
            interfacesRefTypeIds.add(interfaceRefTypeId);

            ReferenceType interfaceRefType = ReferenceType.getType(interfaceRefTypeId);
            interfaceRefType.setTypeTag(new Byte(TypeTag.INTERFACE));
        }
        referenceType.setInterfaceTypeIds((Long[]) interfacesRefTypeIds.toArray(new Long[interfacesRefTypeIds.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numClasses = (int) getVal(packet, index, 4);
        index += 4;
        Node interfacesInfo = new Node(new Node.Descriptor("NumInterfaces:"), new Node.Value(new Integer(numClasses)));

        for (int i = 0; i < numClasses; i++) {
            long interfaceRefTypeId = getVal(packet, index, referenceTypeIDSize);
            index += referenceTypeIDSize;

            ReferenceType referenceType = ReferenceType.getType(interfaceRefTypeId);
            Node singleInterfaceInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(interfaceRefTypeId)));
            interfacesInfo.addChild(singleInterfaceInfo);
        }
        return interfacesInfo;
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
class Response_2_11 extends PacketAnalyzer {

    public Response_2_11() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        Long classObjectId = (Long) this.getPacketInfo(packet).getValue().getRealValue();
        referenceType.setClassObjectId(classObjectId);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long classObjectId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        return new Node(new Node.Descriptor("ClassObjectId:"), new Node.Value(new Long(classObjectId)));
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
class Response_2_12 extends PacketAnalyzer {

    public Response_2_12() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        String extension = (String) this.getPacketInfo(packet).getValue().getRealValue();
        referenceType.setSourceDebugExtension(extension);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int len = (int) getVal(packet, index, 4);
        index += 4;

        String extension = getStr(packet, index, len);
        index += len;
        return new Node(new Node.Descriptor("Extension:"), new Node.Value(extension));
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
class Response_2_13 extends PacketAnalyzer {

    public Response_2_13() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());

        List children = this.getPacketInfo(packet).getChildren();
        String signature = (String) ((Node) children.get(0)).getValue().getRealValue();
        String genericSignature = (String) ((Node) children.get(1)).getValue().getRealValue();
        referenceType.setSignature(signature);
        referenceType.setSignatureWithGeneric(genericSignature);
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        int len = (int) getVal(packet, index, 4);
        index += 4;
        String signature = getStr(packet, index, len);
        index += len;
        Node sigNode = new Node(new Node.Descriptor("Signature:"), new Node.Value(signature));
        root.addChild(sigNode);

        len = (int) getVal(packet, index, 4);
        index += 4;
        String genericSignature = getStr(packet, index, len);
        index += len;
        Node genSigNode = new Node(new Node.Descriptor("GenericSignature:"), new Node.Value(genericSignature));
        root.addChild(genSigNode);

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
class Response_2_14 extends PacketAnalyzer {

    public Response_2_14() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        List fields = this.getPacketInfo(packet).getChildren();
        List fieldIDs = new ArrayList();
        for (int i = 0; i < fields.size(); i++) {
            Node child = (Node) fields.get(i);
            Long fieldId = (Long) child.getValue().getRealValue();
            fieldIDs.add(fieldId);

            FieldType fieldType = FieldType.getType(referenceTypeId, fieldId);
            List grandChildren = child.getChildren();
            fieldType.setName((String) ((Node) grandChildren.get(0)).getValue().getRealValue());
            fieldType.setSignature((String) ((Node) grandChildren.get(1)).getValue().getRealValue());
            fieldType.setSignatureWithGeneric((String) ((Node) grandChildren.get(2)).getValue().getRealValue());
            fieldType.setModBits((Integer) ((Node) grandChildren.get(3)).getValue().getRealValue());
        }

        referenceType.setFieldIds((Long[]) fieldIDs.toArray(new Long[fieldIDs.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int numFields = (int) getVal(packet, index, 4);
        index += 4;

        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        Node root = new Node(new Node.Descriptor("NumFields:"), new Node.Value(new Integer(numFields)));

        for (int i = 0; i < numFields; i++) {

            long fieldID = (getVal(packet, index, fieldIDSize));
            index += fieldIDSize;
            FieldType fieldType = FieldType.getType(referenceTypeId, new Long(fieldID));
            Node singleFieldInfo = new Node(new Node.Descriptor("FieldID:", fieldType), new Node.Value(new Long(fieldID)));
            root.addChild(singleFieldInfo);

            int len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleFieldNameInfo = new Node(new Node.Descriptor("Name:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleFieldInfo.addChild(singleFieldNameInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleFieldSignatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleFieldInfo.addChild(singleFieldSignatureInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleFieldGenericSignatureInfo = new Node(new Node.Descriptor("GenericSignature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleFieldInfo.addChild(singleFieldGenericSignatureInfo);

            int modBits = (int) getVal(packet, index, 4);
            index += 4;
            Node singleFieldModBitsInfo = new Node(new Node.Descriptor("ModBits:"), new Node.Value(new Integer(modBits), getModBitsAsStr(modBits)));
            singleFieldInfo.addChild(singleFieldModBitsInfo);
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
class Response_2_15 extends PacketAnalyzer {

    public Response_2_15() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        List methods = this.getPacketInfo(packet).getChildren();
        List methodIDs = new ArrayList();
        for (Iterator iter = methods.iterator(); iter.hasNext();) {
            Node child = (Node) iter.next();
            Long methodId = (Long) child.getValue().getRealValue();
            methodIDs.add(methodId);

            MethodType methodType = MethodType.getType(referenceTypeId, methodId);
            ArrayList grandChildren = child.getChildren();
            methodType.setName((String) ((Node) grandChildren.get(0)).getValue().getRealValue());
            methodType.setSignature((String) ((Node) grandChildren.get(1)).getValue().getRealValue());
            methodType.setSignatureWithGeneric((String) ((Node) grandChildren.get(2)).getValue().getRealValue());
            methodType.setModBits((Integer) ((Node) grandChildren.get(3)).getValue().getRealValue());
        }

        referenceType.setMethodIds((Long[]) methodIDs.toArray(new Long[methodIDs.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();

        int index = 0;
        int numMethods = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("NumMethods:"), new Node.Value(new Integer(numMethods)));

        for (int i = 0; i < numMethods; i++) {

            long methodID = (getVal(packet, index, methodIDSize));
            index += methodIDSize;
            MethodType methodType = MethodType.getType(referenceTypeId, new Long(methodID));
            Node singleMethodInfo = new Node(new Node.Descriptor("MethodID:", methodType), new Node.Value(new Long(methodID)));
            root.addChild(singleMethodInfo);

            int len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleMethodNameInfo = new Node(new Node.Descriptor("Name:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleMethodInfo.addChild(singleMethodNameInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleMethodSignatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleMethodInfo.addChild(singleMethodSignatureInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleMethodGenericSignatureInfo = new Node(new Node.Descriptor("GenericSignature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleMethodInfo.addChild(singleMethodGenericSignatureInfo);

            int modBits = (int) getVal(packet, index, 4);
            index += 4;
            Node singleMethodModBitsInfo = new Node(new Node.Descriptor("ModBits:"), new Node.Value(new Integer(modBits), getModBitsAsStr(modBits)));
            singleMethodInfo.addChild(singleMethodModBitsInfo);
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
class Response_2_2 extends PacketAnalyzer {

    public Response_2_2() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        Long classLoaderID = (Long) this.getPacketInfo(packet).getValue().getRealValue();
        System.out.println("Setting classLoaderID: " + classLoaderID + " for refTypeId: " + referenceTypeId);
        referenceType.setClassLoaderId(classLoaderID);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        long classLoaderID = getVal(packet, index, objectIDSize);
        ObjectType classLoaderType = ObjectType.getType(classLoaderID);
        index += objectIDSize;
        return new Node(new Node.Descriptor("ClassLoaderID:", classLoaderType), new Node.Value(new Long(classLoaderID)));
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
class Response_2_3 extends PacketAnalyzer {

    public Response_2_3() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        Integer modBits = (Integer) this.getPacketInfo(packet).getValue().getRealValue();
        System.out.println("Setting modBits: " + modBits + " for refTypeId: " + referenceTypeId);
        referenceType.setModifiers(modBits);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int modBits = (int) getVal(packet, index, 4);
        index += 4;
        return new Node(new Node.Descriptor("ModBits:"), new Node.Value(new Integer(modBits), getModBitsAsStr(modBits)));
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
class Response_2_4 extends PacketAnalyzer {

    public Response_2_4() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        List fields = this.getPacketInfo(packet).getChildren();
        List fieldIDs = new ArrayList();
        for (int i = 0; i < fields.size(); i++) {
            Node child = (Node) fields.get(i);
            Long fieldId = (Long) child.getValue().getRealValue();
            fieldIDs.add(fieldId);

            FieldType fieldType = FieldType.getType(referenceTypeId, fieldId);
            List grandChildren = child.getChildren();
            fieldType.setName((String) ((Node) grandChildren.get(0)).getValue().getRealValue());
            fieldType.setSignature((String) ((Node) grandChildren.get(1)).getValue().getRealValue());
            fieldType.setModBits((Integer) ((Node) grandChildren.get(2)).getValue().getRealValue());
        }

        referenceType.setFieldIds((Long[]) fieldIDs.toArray(new Long[fieldIDs.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int numFields = (int) getVal(packet, index, 4);
        index += 4;

        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        Node root = new Node(new Node.Descriptor("NumFields:"), new Node.Value(new Integer(numFields)));

        for (int i = 0; i < numFields; i++) {

            long fieldID = (getVal(packet, index, fieldIDSize));
            index += fieldIDSize;
            FieldType fieldType = FieldType.getType(referenceTypeId, new Long(fieldID));
            Node singleFieldInfo = new Node(new Node.Descriptor("FieldID:", fieldType), new Node.Value(new Long(fieldID)));
            root.addChild(singleFieldInfo);

            int len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleFieldNameInfo = new Node(new Node.Descriptor("Name:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleFieldInfo.addChild(singleFieldNameInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleFieldSignatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleFieldInfo.addChild(singleFieldSignatureInfo);

            int modBits = (int) getVal(packet, index, 4);
            index += 4;
            Node singleFieldModBitsInfo = new Node(new Node.Descriptor("ModBits:"), new Node.Value(new Integer(modBits), getModBitsAsStr(modBits)));
            singleFieldInfo.addChild(singleFieldModBitsInfo);
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
class Response_2_5 extends PacketAnalyzer {

    public Response_2_5() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        List methods = this.getPacketInfo(packet).getChildren();
        List methodIDs = new ArrayList();
        for (Iterator iter = methods.iterator(); iter.hasNext();) {
            Node child = (Node) iter.next();
            Long methodId = (Long) child.getValue().getRealValue();
            methodIDs.add(methodId);

            MethodType methodType = MethodType.getType(referenceTypeId, methodId);
            List grandChildren = child.getChildren();
            methodType.setName((String) ((Node) grandChildren.get(0)).getValue().getRealValue());
            methodType.setSignature((String) ((Node) grandChildren.get(1)).getValue().getRealValue());
            methodType.setModBits((Integer) ((Node) grandChildren.get(2)).getValue().getRealValue());
        }

        referenceType.setMethodIds((Long[]) methodIDs.toArray(new Long[methodIDs.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();

        int index = 0;
        int numMethods = (int) getVal(packet, index, 4);
        index += 4;
        Node root = new Node(new Node.Descriptor("NumMethods:"), new Node.Value(new Integer(numMethods)));

        for (int i = 0; i < numMethods; i++) {

            long methodID = (getVal(packet, index, methodIDSize));
            index += methodIDSize;
            MethodType methodType = MethodType.getType(referenceTypeId, new Long(methodID));
            Node singleMethodInfo = new Node(new Node.Descriptor("MethodID:", methodType), new Node.Value(new Long(methodID)));
            root.addChild(singleMethodInfo);

            int len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleMethodNameInfo = new Node(new Node.Descriptor("Name:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleMethodInfo.addChild(singleMethodNameInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node singleMethodSignatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            singleMethodInfo.addChild(singleMethodSignatureInfo);

            int modBits = (int) getVal(packet, index, 4);
            index += 4;
            Node singleMethodModBitsInfo = new Node(new Node.Descriptor("ModBits:"), new Node.Value(new Integer(modBits), getModBitsAsStr(modBits)));
            singleMethodInfo.addChild(singleMethodModBitsInfo);
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
class Response_2_6 extends PacketAnalyzer {

    public Response_2_6() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);

        Node retTypeIdNode = ((Node) requestPacketAnalyzer.getPacketInfo(request).getChildren().get(0));
        Long refTypeId = (Long) retTypeIdNode.getValue().getRealValue();

        List fieldIdNodes = ((Node) requestPacketAnalyzer.getPacketInfo(request).getChildren().get(1)).getChildren();
        List fieldValueNodes = this.getPacketInfo(packet).getChildren();

        for (int i = 0; i < fieldIdNodes.size(); i++) {
            Long fieldId = (Long) ((Node) fieldIdNodes.get(i)).getValue().getRealValue();
            FieldType fieldType = FieldType.getType(refTypeId, fieldId);
            Value value = (Value) ((Node) fieldValueNodes.get(i)).getValue().getRealValue();
            fieldType.setValue(value);
        }
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numValues = (int) getVal(packet, index, 4);
        index += 4;
        Node valuesInfo = new Node(new Node.Descriptor("NumValues:"), new Node.Value(new Integer(numValues)));

        for (int i = 0; i < numValues; i++) {
            Value value = getValue(packet, index);
            index += value.getOffset();
            Node singleValueInfo = new Node(new Node.Descriptor("Value:"), new Node.Value(value));
            valuesInfo.addChild(singleValueInfo);
        }
        return valuesInfo;
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
class Response_2_7 extends PacketAnalyzer {

    public Response_2_7() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        String sourceFile = (String) this.getPacketInfo(packet).getValue().getRealValue();
        referenceType.setSourceFile(sourceFile);
        referenceType = ReferenceType.getType(referenceTypeId.longValue());
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int len = (int) getVal(packet, index, 4);
        index += 4;
        String signature = getStr(packet, index, len);
        index += len;
        return new Node(new Node.Descriptor("Source file:"), new Node.Value(signature));
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
class Response_2_8 extends PacketAnalyzer {

    public Response_2_8() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId);

        List nestedClassesRefTypeIds = new ArrayList();
        List classesInfo = this.getPacketInfo(packet).getChildren();
        for (int i = 0; i < classesInfo.size(); i++) {
            Node singleClassInfo = (Node) classesInfo.get(i);
            Long nestedRefTypeId = (Long) singleClassInfo.getValue().getRealValue();
            nestedClassesRefTypeIds.add(nestedRefTypeId);

            Node typeTag = (Node) singleClassInfo.getChildren().get(0);
            ReferenceType nestedRefType = ReferenceType.getType(nestedRefTypeId);
            nestedRefType.setTypeTag((Byte) typeTag.getValue().getRealValue());
        }
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numClasses = (int) getVal(packet, index, 4);
        index += 4;
        Node classesInfo = new Node(new Node.Descriptor("NumClasses:"), new Node.Value(new Integer(numClasses)));

        for (int i = 0; i < numClasses; i++) {
            byte typeTag = (byte) (packet.getData()[index++] & 0xff);
            long refTypeId = getVal(packet, index, referenceTypeIDSize);
            index += referenceTypeIDSize;

            ReferenceType referenceType = ReferenceType.getType(refTypeId);
            Node singleClassInfo = new Node(new Node.Descriptor("RefTypeID:", referenceType), new Node.Value(new Long(refTypeId)));
            classesInfo.addChild(singleClassInfo);

            Node typeTagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Byte(typeTag), TypeTag.asString(typeTag)));
            singleClassInfo.addChild(typeTagInfo);
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
class Response_2_9 extends PacketAnalyzer {

    public Response_2_9() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        Integer classStatus = (Integer) this.getPacketInfo(packet).getValue().getRealValue();
        referenceType.setStatus(classStatus);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        int classStatus = (int) getVal(packet, index, 4);
        index += 4;
        return new Node(new Node.Descriptor("ClassStatus:"), new Node.Value(new Integer(classStatus), ClassStatus.asString(classStatus)));
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
class Response_3_1 extends PacketAnalyzer {

    public Response_3_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long referenceTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ReferenceType referenceType = ReferenceType.getType(referenceTypeId.longValue());
        Long superID = (Long) this.getPacketInfo(packet).getValue().getRealValue();
        referenceType.setSuperclass(superID);
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
class Response_3_3 extends PacketAnalyzer {

    public Response_3_3() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        Value value = getValue(packet, index);
        index += value.getOffset();
        Node returnInfo = new Node(new Node.Descriptor("ReturnValue:"), new Node.Value(value));
        root.addChild(returnInfo);

        value = getValue(packet, index);
        index += value.getOffset();
        Long exceptionObjectId = (Long) value.getValue();
        ObjectType objectType = ObjectType.getType(exceptionObjectId);
        Node exceptionInfo = new Node(new Node.Descriptor("Exception:", objectType), new Node.Value(value));
        root.addChild(exceptionInfo);

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
class Response_3_4 extends PacketAnalyzer {

    public Response_3_4() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        Value value = getValue(packet, index);
        index += value.getOffset();
        Long newInstanceId = (Long) value.getValue();
        ObjectType objectType = ObjectType.getType(newInstanceId);
        Node returnInfo = new Node(new Node.Descriptor("NewInstanceID:", objectType), new Node.Value(newInstanceId));
        root.addChild(returnInfo);

        value = getValue(packet, index);
        index += value.getOffset();
        Long exceptionObjectId = (Long) value.getValue();
        objectType = ObjectType.getType(exceptionObjectId);
        Node exceptionInfo = new Node(new Node.Descriptor("ExceptionObjectID:", objectType), new Node.Value(exceptionObjectId));
        root.addChild(exceptionInfo);

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
class Response_4_1 extends PacketAnalyzer {

    public Response_4_1() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;
        Value value = getValue(packet, index);
        index += value.getOffset();
        Long objectId = (Long) value.getValue();
        ObjectType objectType = ObjectType.getType(objectId);
        return new Node(new Node.Descriptor("ObjectID:", objectType), new Node.Value(objectId));
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
class Response_6_1 extends PacketAnalyzer {

    public Response_6_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        List requestChildren = requestPacketAnalyzer.getPacketInfo(request).getChildren();

        Long referenceTypeId = (Long) ((Node) requestChildren.get(0)).getValue().getRealValue();
        Long methId = (Long) ((Node) requestChildren.get(1)).getValue().getRealValue();

        MethodType methType = MethodType.getType(referenceTypeId, methId);

        List nodes = this.getPacketInfo(packet).getChildren();

        Long startIndex = (Long) ((Node) nodes.get(0)).getValue().getRealValue();
        methType.setStartCodeIndex(startIndex);
        Long endIndex = (Long) ((Node) nodes.get(1)).getValue().getRealValue();
        methType.setEndCodeIndex(endIndex);

        Node lineInfo = (Node) nodes.get(2);
        Integer numLines = (Integer) lineInfo.getValue().getRealValue();
        methType.setNumLines(numLines);

        List lines = lineInfo.getChildren();
        List lineMappings = new ArrayList();
        for (Iterator iter = lines.iterator(); iter.hasNext();) {
            Node node = (Node) iter.next();
            Long lineCodeIndex = (Long) node.getValue().getRealValue();

            node = (Node) iter.next();
            Integer lineNumber = (Integer) node.getValue().getRealValue();
            MethodType.LineMapping mapping = new MethodType.LineMapping(lineCodeIndex, lineNumber);
            lineMappings.add(mapping);
        }
        methType.setLineMapping((MethodType.LineMapping[]) lineMappings.toArray(new MethodType.LineMapping[lineMappings.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long startIndex = getVal(packet, index, 8);
        index += 8;
        Node startIndexInfo = new Node(new Node.Descriptor("StartIndex:"), new Node.Value(new Long(startIndex)));
        root.addChild(startIndexInfo);

        long endIndex = getVal(packet, index, 8);
        index += 8;
        Node endIndexInfo = new Node(new Node.Descriptor("EndIndex:"), new Node.Value(new Long(endIndex)));
        root.addChild(endIndexInfo);

        int numLines = (int) getVal(packet, index, 4);
        index += 4;
        Node lineInfo = new Node(new Node.Descriptor("NumLines:"), new Node.Value(new Integer(numLines)));
        root.addChild(lineInfo);

        for (int i = 0; i < numLines; i++) {

            long codeIndex = (getVal(packet, index, 8));
            index += 8;
            Node codeIndexInfo = new Node(new Node.Descriptor("LineCodeIndex:"), new Node.Value(new Long(codeIndex)));
            lineInfo.addChild(codeIndexInfo);

            int lineNum = (int) getVal(packet, index, 4);
            index += 4;
            Node lineNumInfo = new Node(new Node.Descriptor("LineNumber:"), new Node.Value(new Integer(lineNum)));
            lineInfo.addChild(lineNumInfo);
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
class Response_6_2 extends PacketAnalyzer {

    public Response_6_2() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        List requestChildren = requestPacketAnalyzer.getPacketInfo(request).getChildren();

        Long referenceTypeId = (Long) ((Node) requestChildren.get(0)).getValue().getRealValue();
        Long methId = (Long) ((Node) requestChildren.get(1)).getValue().getRealValue();

        MethodType methType = MethodType.getType(referenceTypeId, methId);

        List nodes = this.getPacketInfo(packet).getChildren();

        Integer argCnt = (Integer) ((Node) nodes.get(0)).getValue().getRealValue();
        methType.setArgCount(argCnt);

        Node varsInfo = (Node) nodes.get(1);

        List varData = varsInfo.getChildren();
        List variables = new ArrayList();
        for (Iterator iter = varData.iterator(); iter.hasNext();) {
            Node node = (Node) iter.next();
            Long codeIndex = (Long) node.getValue().getRealValue();

            node = (Node) iter.next();
            String name = (String) node.getValue().getRealValue();
            node = (Node) iter.next();
            String signature = (String) node.getValue().getRealValue();

            node = (Node) iter.next();
            Integer lenght = (Integer) node.getValue().getRealValue();
            node = (Node) iter.next();
            Integer frameIndex = (Integer) node.getValue().getRealValue();

            MethodType.Variable variable = new MethodType.Variable(codeIndex, name, signature, null, lenght, frameIndex);
            variables.add(variable);
        }
        methType.setVariables((MethodType.Variable[]) variables.toArray(new MethodType.Variable[variables.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        int argCnt = (int) getVal(packet, index, 4);
        index += 4;
        Node argCntInfo = new Node(new Node.Descriptor("ArgCount:"), new Node.Value(new Integer(argCnt)));
        root.addChild(argCntInfo);

        int numVars = (int) getVal(packet, index, 4);
        index += 4;
        Node varsInfo = new Node(new Node.Descriptor("NumVariables:"), new Node.Value(new Integer(numVars)));
        root.addChild(varsInfo);

        for (int i = 0; i < numVars; i++) {

            long codeIndex = (getVal(packet, index, 8));
            index += 8;
            Node codeIndexInfo = new Node(new Node.Descriptor("CodeIndex:"), new Node.Value(new Long(codeIndex)));
            varsInfo.addChild(codeIndexInfo);

            int len = (int) getVal(packet, index, 4);
            index += 4;
            Node nameInfo = new Node(new Node.Descriptor("Name:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            varsInfo.addChild(nameInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node signatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            varsInfo.addChild(signatureInfo);

            int lenght = (int) getVal(packet, index, 4);
            index += 4;
            Node lenghtInfo = new Node(new Node.Descriptor("Lenght:"), new Node.Value(new Integer(lenght)));
            varsInfo.addChild(lenghtInfo);

            int frameIndex = (int) getVal(packet, index, 4);
            index += 4;
            Node frameIndexInfo = new Node(new Node.Descriptor("FrameIndex:"), new Node.Value(new Integer(frameIndex)));
            varsInfo.addChild(frameIndexInfo);
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
class Response_6_3 extends PacketAnalyzer {

    public Response_6_3() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        List requestChildren = requestPacketAnalyzer.getPacketInfo(request).getChildren();

        Long referenceTypeId = (Long) ((Node) requestChildren.get(0)).getValue().getRealValue();
        Long methId = (Long) ((Node) requestChildren.get(1)).getValue().getRealValue();

        MethodType methType = MethodType.getType(referenceTypeId, methId);

        Node node = this.getPacketInfo(packet);
        Node byteCodeNode = (Node) node.getChildren().get(0);
        byte[] byteCode = (byte[]) byteCodeNode.getValue().getRealValue();
        methType.setByteCode(byteCode);
    }

    public Node getPacketInfo(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        List requestChildren = requestPacketAnalyzer.getPacketInfo(request).getChildren();

        Long referenceTypeId = (Long) ((Node) requestChildren.get(0)).getValue().getRealValue();
        Long methId = (Long) ((Node) requestChildren.get(1)).getValue().getRealValue();

        MethodType methType = MethodType.getType(referenceTypeId, methId);

        int index = 0;
        int numMethodBytes = (int) getVal(packet, index, 4);
        index += 4;
        byte[] methodBytes = new byte[numMethodBytes];
        System.arraycopy(packet.getData(), index, methodBytes, 0, numMethodBytes);
        index += numMethodBytes;

        Node byteInfo = new Node(new Node.Descriptor("Bytes:", methType), new Node.Value(methodBytes, Integer.toString(numMethodBytes)));

        return byteInfo;
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
class Response_6_4 extends PacketAnalyzer {

    public Response_6_4() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        List requestChildren = requestPacketAnalyzer.getPacketInfo(request).getChildren();

        Long referenceTypeId = (Long) ((Node) requestChildren.get(0)).getValue().getRealValue();
        Long methId = (Long) ((Node) requestChildren.get(1)).getValue().getRealValue();

        MethodType methType = MethodType.getType(referenceTypeId, methId);

        Node node = this.getPacketInfo(packet);
        Boolean isObsolete = (Boolean) node.getValue().getRealValue();
        methType.setIsObsolete(isObsolete);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        boolean isObsolete = getBoolean(packet, index++);
        Node root = new Node(new Node.Descriptor("IsObsolete:"), new Node.Value(new Boolean(isObsolete)));

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
class Response_6_5 extends PacketAnalyzer {

    public Response_6_5() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        List requestChildren = requestPacketAnalyzer.getPacketInfo(request).getChildren();

        Long referenceTypeId = (Long) ((Node) requestChildren.get(0)).getValue().getRealValue();
        Long methId = (Long) ((Node) requestChildren.get(1)).getValue().getRealValue();

        MethodType methType = MethodType.getType(referenceTypeId, methId);

        List nodes = this.getPacketInfo(packet).getChildren();

        Integer argCnt = (Integer) ((Node) nodes.get(0)).getValue().getRealValue();
        methType.setArgCount(argCnt);

        Node varsInfo = (Node) nodes.get(1);

        List varData = varsInfo.getChildren();
        List variables = new ArrayList();
        for (Iterator iter = varData.iterator(); iter.hasNext();) {
            Node node = (Node) iter.next();
            Long codeIndex = (Long) node.getValue().getRealValue();

            node = (Node) iter.next();
            String name = (String) node.getValue().getRealValue();
            node = (Node) iter.next();
            String signature = (String) node.getValue().getRealValue();
            node = (Node) iter.next();
            String genericSignature = (String) node.getValue().getRealValue();

            node = (Node) iter.next();
            Integer lenght = (Integer) node.getValue().getRealValue();
            node = (Node) iter.next();
            Integer frameIndex = (Integer) node.getValue().getRealValue();

            MethodType.Variable variable = new MethodType.Variable(codeIndex, name, signature, genericSignature, lenght, frameIndex);
            variables.add(variable);
        }
        methType.setVariables((MethodType.Variable[]) variables.toArray(new MethodType.Variable[variables.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        int argCnt = (int) getVal(packet, index, 4);
        index += 4;
        Node argCntInfo = new Node(new Node.Descriptor("ArgCount:"), new Node.Value(new Integer(argCnt)));
        root.addChild(argCntInfo);

        int numVars = (int) getVal(packet, index, 4);
        index += 4;
        Node varsInfo = new Node(new Node.Descriptor("NumVariables:"), new Node.Value(new Integer(numVars)));
        root.addChild(varsInfo);

        for (int i = 0; i < numVars; i++) {

            long codeIndex = (getVal(packet, index, 8));
            index += 8;
            Node codeIndexInfo = new Node(new Node.Descriptor("CodeIndex:"), new Node.Value(new Long(codeIndex)));
            varsInfo.addChild(codeIndexInfo);

            int len = (int) getVal(packet, index, 4);
            index += 4;
            Node nameInfo = new Node(new Node.Descriptor("Name:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            varsInfo.addChild(nameInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node signatureInfo = new Node(new Node.Descriptor("Signature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            varsInfo.addChild(signatureInfo);

            len = (int) getVal(packet, index, 4);
            index += 4;
            Node genericSignatureInfo = new Node(new Node.Descriptor("GenericSignature:"), new Node.Value(getStr(packet, index, len)));
            index += len;
            varsInfo.addChild(genericSignatureInfo);

            int lenght = (int) getVal(packet, index, 4);
            index += 4;
            Node lenghtInfo = new Node(new Node.Descriptor("Lenght:"), new Node.Value(new Integer(lenght)));
            varsInfo.addChild(lenghtInfo);

            int frameIndex = (int) getVal(packet, index, 4);
            index += 4;
            Node frameIndexInfo = new Node(new Node.Descriptor("FrameIndex:"), new Node.Value(new Integer(frameIndex)));
            varsInfo.addChild(frameIndexInfo);
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
class Response_9_1 extends PacketAnalyzer {

    public Response_9_1() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        List nodes = getPacketInfo(packet).getChildren();
        Node refTypeIdInfo = (Node) nodes.get(1);
        Long refTypeId = (Long) refTypeIdInfo.getValue().getRealValue();
        objectType.setReferenceTypeId(refTypeId);
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        byte typeTag = packet.getData()[index++];
        Node tagInfo = new Node(new Node.Descriptor("TypeTag:"), new Node.Value(new Integer(typeTag), TypeTag.asString(typeTag)));
        root.addChild(tagInfo);

        long referenceTypeId = (int) getVal(packet, index, referenceTypeIDSize);
        index += referenceTypeIDSize;
        ReferenceType refType = ReferenceType.getType(referenceTypeId);
        Node referenceTypeIdInfo = new Node(new Node.Descriptor("RefTypeID:", refType), new Node.Value(new Long(referenceTypeId)));
        root.addChild(referenceTypeIdInfo);

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
class Response_9_2 extends PacketAnalyzer {

    public Response_9_2() {
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        int numValues = (int) getVal(packet, index, 4);
        index += 4;
        Node valuesInfo = new Node(new Node.Descriptor("NumValues:"), new Node.Value(new Integer(numValues)));

        for (int i = 0; i < numValues; i++) {
            Value value = getValue(packet, index);
            index += value.getOffset();

            Node singleValueInfo = new Node(new Node.Descriptor("Value:"), new Node.Value(value));
            valuesInfo.addChild(singleValueInfo);
        }
        return valuesInfo;
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
class Response_9_5 extends PacketAnalyzer {

    public Response_9_5() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        List nodes = this.getPacketInfo(packet).getChildren();

        Long ownerThreadId = (Long) ((Node) nodes.get(0)).getValue().getRealValue();
        objectType.setOwnerThreadId(ownerThreadId);

        Integer entrycount = (Integer) ((Node) nodes.get(1)).getValue().getRealValue();
        objectType.setMonitorEntryCount(entrycount);

        List waitingThreads = ((Node) nodes.get(2)).getChildren();
        List waitingThreadIds = new ArrayList();
        for (Iterator iter = waitingThreads.iterator(); iter.hasNext();) {
            Node waitThreadNode = (Node) iter.next();
            Long id = (Long) waitThreadNode.getValue().getRealValue();
            waitingThreadIds.add(id);
        }
        objectType.setWaitingThreadIds((Long[]) waitingThreadIds.toArray(new Long[waitingThreadIds.size()]));
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        long threadTypeId = getVal(packet, index, objectIDSize);
        index += objectIDSize;
        ObjectType threadType = ObjectType.getType(threadTypeId);
        Node threadInfo = new Node(new Node.Descriptor("ThreadID:", threadType), new Node.Value(new Long(threadTypeId)));
        root.addChild(threadInfo);

        int entryCount = (int) getVal(packet, index, 4);
        index += 4;
        Node entryInfo = new Node(new Node.Descriptor("EntryCount:"), new Node.Value(new Integer(entryCount)));
        root.addChild(entryInfo);

        int numWaiters = (int) getVal(packet, index, 4);
        index += 4;
        Node waitersInfo = new Node(new Node.Descriptor("NumWaitingThreads:"), new Node.Value(new Integer(numWaiters)));
        root.addChild(waitersInfo);

        for (int i = 0; i < numWaiters; i++) {
            long waitingThreadTypeId = getVal(packet, index, objectIDSize);
            index += objectIDSize;
            ObjectType waitingThreadType = ObjectType.getType(waitingThreadTypeId);
            Node waitThreadInfo = new Node(new Node.Descriptor("WaitingThreadID:", waitingThreadType), new Node.Value(new Long(waitingThreadTypeId)));
            waitersInfo.addChild(waitThreadInfo);
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
class Response_9_6 extends PacketAnalyzer {

    public Response_9_6() {
    }

    public Node getPacketInfo(Packet packet) {
        Node root = new Node(null, null);
        int index = 0;

        Value value = getValue(packet, index);
        index += value.getOffset();
        Node returnInfo = new Node(new Node.Descriptor("ReturnValue:"), new Node.Value(value));
        root.addChild(returnInfo);

        value = getValue(packet, index);
        index += value.getOffset();
        Long exceptionObjectId = (Long) value.getValue();
        ObjectType objectType = ObjectType.getType(exceptionObjectId);
        Node exceptionInfo = new Node(new Node.Descriptor("Exception:", objectType), new Node.Value(value));
        root.addChild(exceptionInfo);

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
class Response_9_9 extends PacketAnalyzer {

    public Response_9_9() {
    }

    public void updateInternalDataModel(Packet packet) {
        Request request = Request.getRequest(packet.getId());
        PacketAnalyzer requestPacketAnalyzer = AnalyzerManager.createPacketAnalyzer(request);
        Long objTypeId = (Long) requestPacketAnalyzer.getPacketInfo(request).getValue().getRealValue();
        ObjectType objectType = ObjectType.getType(objTypeId);

        Node node = this.getPacketInfo(packet);
        Boolean isCollected = (Boolean) node.getValue().getRealValue();
        objectType.setIsCollected(isCollected);
    }

    public Node getPacketInfo(Packet packet) {
        int index = 0;

        boolean isObsolete = getBoolean(packet, index++);
        Node root = new Node(new Node.Descriptor("IsCollected:"), new Node.Value(new Boolean(isObsolete)));

        return root;
    }

}

