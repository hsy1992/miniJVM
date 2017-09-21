//
// Created by gust on 2017/9/20.
//

#include "jdwp.h"
#include "global.h"
#include "jvm.h"
#include "jvm_util.h"
#include "java_native_jdwp.h"
#include "java_native_io.h"
#include "garbage.h"


//==================================================    server    ==================================================

void jdwp_put_client(ArrayList *clients, JdwpClient *client) {
    arraylist_append(clients, client);
}

void *jdwp_thread_listener(void *para) {
    JdwpServer *srv = (JdwpServer *) para;
    s32 srvsock = srv_bind(srv->ip, srv->port);
    srv->srvsock = srvsock;
    srv_listen(srvsock);

    while (!srv->exit) {
        s32 sockfd = srv_accept(srvsock);
        JdwpClient *client = jvm_alloc(sizeof(JdwpClient));
        client->closed = 0;
        client->conn_first = 1;
        client->sockfd = sockfd;
        jdwp_put_client(srv->clients, client);
    }
    return srv;
}

void *jdwp_thread_dispacher(void *para) {
    JdwpServer *srv = (JdwpServer *) para;
    Runtime runtime;
    runtime_create(&runtime);
    s32 i;
    while (!srv->exit) {
        for (i = 0; i < srv->clients->length; i++) {
            JdwpClient *client = arraylist_get_value(srv->clients, i);
            jdwp_client_process(client, &runtime);
            if (client->closed) {
                arraylist_remove(srv->clients, client);
            }
        }
        threadSleep(20);
    }
    runtime_destory(&runtime);
    return srv;
}

s32 jdwp_start_server() {
    jdwpserver.ip = utf8_create();
    jdwpserver.port = 8000;
    jdwpserver.exit = 0;
    jdwpserver.clients = arraylist_create(0);
    jdwpserver.events = arraylist_create(0);
    jdwpserver.event_sets = hashtable_create(DEFAULT_HASH_FUNC, DEFAULT_HASH_EQUALS_FUNC);

    pthread_create(&jdwpserver.pt_listener, NULL, jdwp_thread_listener, &jdwpserver);
    pthread_create(&jdwpserver.pt_dispacher, NULL, jdwp_thread_dispacher, &jdwpserver);
    return 0;
}

//==================================================    packet    ==================================================



JdwpPacket *jdwppacket_create() {
    JdwpPacket *packet = jvm_alloc(sizeof(JdwpPacket));
    packet->alloc = 32;
    packet->readPos = 11;
    packet->writePos = 11;
    packet->data = jvm_alloc(packet->alloc);
    return packet;
}

JdwpPacket *jdwppacket_create_data(c8 *data, s32 len) {
    JdwpPacket *packet = jvm_alloc(sizeof(JdwpPacket));
    packet->data = data;
    packet->alloc = len;
    return packet;
}

void jdwppacket_destory(JdwpPacket *packet) {
    jvm_free(packet->data);
    jvm_free(packet);
}

c8 jdwppacket_read_byte(JdwpPacket *packet) {
    return packet->data[packet->readPos++];
}

s16 jdwppacket_read_short(JdwpPacket *packet) {
    s16 s = (short) (((packet->data[packet->readPos + 1] & 0xFF) << 0) +
                     ((packet->data[packet->readPos + 0] & 0xFF) << 8));
    packet->readPos += 2;
    return s;
}


s32 jdwppacket_read_int(JdwpPacket *packet) {
    s32 i = ((packet->data[packet->readPos + 3] & 0xFF) << 0)
            + ((packet->data[packet->readPos + 2] & 0xFF) << 8)
            + ((packet->data[packet->readPos + 1] & 0xFF) << 16)
            + ((packet->data[packet->readPos + 0] & 0xFF) << 24);
    packet->readPos += 4;
    return i;
}


s64 jdwppacket_read_long(JdwpPacket *packet) {
    s64 l = (((s64) packet->data[packet->readPos + 7] & 0xFFL) << 0)
            + (((s64) packet->data[packet->readPos + 6] & 0xFFL) << 8)
            + (((s64) packet->data[packet->readPos + 5] & 0xFFL) << 16)
            + (((s64) packet->data[packet->readPos + 4] & 0xFFL) << 24)
            + (((s64) packet->data[packet->readPos + 3] & 0xFFL) << 32)
            + (((s64) packet->data[packet->readPos + 2] & 0xFFL) << 40)
            + (((s64) packet->data[packet->readPos + 1] & 0xFFL) << 48)
            + (((s64) packet->data[packet->readPos + 0] & 0xFFL) << 56);
    packet->readPos += 8;
    return l;
}


Utf8String *jdwppacket_read_utf(JdwpPacket *packet) {
    Utf8String *ustr = utf8_create();
    s32 len = jdwppacket_read_int(packet);
    s32 i;
    for (i = 0; i < len; i++) {
        utf8_pushback(ustr, jdwppacket_read_byte(packet));
    }
    return ustr;
}

__refer jdwppacket_read_refer(JdwpPacket *packet) {
    if (sizeof(__refer) > 4) {
        return (__refer) (long) jdwppacket_read_long(packet);
    } else {
        return (__refer) (long) jdwppacket_read_int(packet);
    }
}

void jdwppacket_ensureCapacity(JdwpPacket *packet, s32 length) {
    s32 newcount = packet->writePos + length;
    if (newcount >= packet->alloc) {
        newcount = newcount > packet->alloc << 1 ? newcount : packet->alloc << 1;
        c8 *tmp = jvm_alloc(newcount);
        memcpy(tmp, packet->data, packet->alloc);
        jvm_free(packet->data);
        packet->data = tmp;
        packet->alloc = newcount;
    }
}

void jdwppacket_write_byte(JdwpPacket *packet, c8 val) {
    jdwppacket_ensureCapacity(packet, 1);
    packet->data[packet->writePos] = val;
    packet->writePos++;
}

void jdwppacket_write_short(JdwpPacket *packet, s16 val) {
    jdwppacket_ensureCapacity(packet, 2);
    packet->data[packet->writePos + 1] = (u8) (val >> 0);
    packet->data[packet->writePos + 0] = (u8) (val >> 8);
    packet->writePos += 2;
}


void jdwppacket_write_int(JdwpPacket *packet, s32 val) {
    jdwppacket_ensureCapacity(packet, 4);
    packet->data[packet->writePos + 3] = (u8) (val >> 0);
    packet->data[packet->writePos + 2] = (u8) (val >> 8);
    packet->data[packet->writePos + 1] = (u8) (val >> 16);
    packet->data[packet->writePos + 0] = (u8) (val >> 24);
    packet->writePos += 4;
}

void jdwppacket_write_long(JdwpPacket *packet, s64 val) {
    jdwppacket_ensureCapacity(packet, 8);
    packet->data[packet->writePos + 7] = (u8) (val >> 0);
    packet->data[packet->writePos + 6] = (u8) (val >> 8);
    packet->data[packet->writePos + 5] = (u8) (val >> 16);
    packet->data[packet->writePos + 4] = (u8) (val >> 24);
    packet->data[packet->writePos + 3] = (u8) (val >> 32);
    packet->data[packet->writePos + 2] = (u8) (val >> 40);
    packet->data[packet->writePos + 1] = (u8) (val >> 48);
    packet->data[packet->writePos + 0] = (u8) (val >> 56);
    packet->writePos += 8;
}

void jdwppacket_write_buf(JdwpPacket *packet, c8 *val, s32 len) {
    jdwppacket_ensureCapacity(packet, len);
    memcpy(packet->data + packet->writePos, val, len);
    packet->writePos += len;
}

void jdwppacket_write_utf(JdwpPacket *packet, Utf8String *val) {
    jdwppacket_write_int(packet, val->length);
    jdwppacket_write_buf(packet, utf8_cstr(val), val->length);
}

void jdwppacket_write_refer(JdwpPacket *packet, __refer val) {
    if (sizeof(__refer) > 4) {
        jdwppacket_write_long(packet, (s64) (long) val);
    } else {
        jdwppacket_write_int(packet, (s32) (long) val);
    }
}

s32 jdwppacket_getbypos(JdwpPacket *packet, s32 pos, s32 n) {
    s32 len = 0;
    s32 last = pos + n;
    s32 i;
    for (i = pos; i < last; i++) {
        len <<= 8;
        len += (u8) packet->data[i];
    }
    return len;
}

s16 jdwppacket_get_cmd_err(JdwpPacket *packet) {
    return (s16) jdwppacket_getbypos(packet, 9, 2);
}

u8 jdwppacket_get_flag(JdwpPacket *packet) {
    return (u8) jdwppacket_getbypos(packet, 8, 1);
}

s32 jdwppacket_get_id(JdwpPacket *packet) {
    return jdwppacket_getbypos(packet, 4, 4);
}

s32 jdwppacket_get_length(JdwpPacket *packet) {
    return jdwppacket_getbypos(packet, 0, 4);
}

void jdwppacket_set_flag(JdwpPacket *packet, u8 flag) {
    s32 pos = 8;
    packet->data[pos++] = flag;
}

void jdwppacket_set_id(JdwpPacket *packet, s32 id) {
    s32 pos = 4;
    packet->data[pos++] = (u8) (id >> 24);
    packet->data[pos++] = (u8) (id >> 16);
    packet->data[pos++] = (u8) (id >> 8);
    packet->data[pos++] = (u8) (id);
}

void jdwppacket_set_length(JdwpPacket *packet, s32 len) {
    s32 pos = 0;
    packet->data[pos++] = (u8) (len >> 24);
    packet->data[pos++] = (u8) (len >> 16);
    packet->data[pos++] = (u8) (len >> 8);
    packet->data[pos++] = (u8) (len);
}

void jdwppacket_set_cmd(JdwpPacket *packet, u8 set, u8 cmd) {
    s32 pos = 9;
    packet->data[pos++] = set;
    packet->data[pos++] = cmd;
}

void jdwppacket_set_err(JdwpPacket *packet, u16 err) {
    s32 pos = 9;
    packet->data[pos++] = (c8) (err >> 8);
    packet->data[pos++] = (c8) (err >> 0);
}

s32 jdwp_read_fully(JdwpClient *client, c8 *buf, s32 need) {
    s32 got = 0, len = 0;
    while ((len = sock_recv(client->sockfd, buf + got, need - got)) != -1) {
        got += len;
        if (got == need)break;
    }
    if (len == -1)return len;
    return got;
}

s32 jdwp_write_fully(JdwpClient *client, c8 *buf, s32 need) {
    s32 sent = 0, len = 0;
    while ((len = sock_send(client->sockfd, buf + sent, need - sent)) != -1) {
        sent += len;
        if (sent == need)break;
    }
    if (len == -1)return len;
    return sent;
}

JdwpPacket *jdwp_readpacket(JdwpClient *client) {
    if (!client->conn_first) {
        c8 buf[4];
        s32 len = jdwp_read_fully(client, (c8 *) &buf, 4);
        s32 pack_len =
                ((buf[0] & 0xFF) << 24) | ((buf[1] & 0xFF) << 16) | ((buf[2] & 0xFF) << 8) | ((buf[3] & 0xFF) << 0);
        JdwpPacket *p = jdwppacket_create();
        jdwppacket_ensureCapacity(p, pack_len);
        jdwppacket_set_length(p, pack_len);
        len = jdwp_read_fully(client, &p->data[4], pack_len - 4);
        p->writePos = pack_len;
        if (len < 0) {
            jdwppacket_destory(p);
            client->closed = 1;
        } else {
            return p;
        }
    } else {//首次连接
        c8 buf[14];
        s32 len = jdwp_read_fully(client, (c8 *) &buf, 14);
        if (len == -1) {
            client->closed = 1;
        }
        len = strlen(JDWP_HANDSHAKE);
        s32 i;
        for (i = 0; i < len; i++) {
            if (JDWP_HANDSHAKE[i] != buf[i]) {
                client->closed = 1;
                return NULL;
            }
        }
        len = jdwp_write_fully(client, JDWP_HANDSHAKE, 14);
        if (len < 0) {
            client->closed = 1;
        }
        client->conn_first = 0;
    }
    return NULL;
}


s32 jdwp_writepacket(JdwpClient *client, JdwpPacket *packet) {
    jdwppacket_set_length(packet, packet->writePos);
    s32 len = jdwp_write_fully(client, packet->data, packet->writePos);
    if (len < 0) {
        client->closed = 1;
        return 1;
    }
    return 0;
}
//==================================================    0    ==================================================


EventSet *jdwp_create_eventset(JdwpPacket *req) {
    EventSet *set = jvm_alloc(sizeof(EventSet));
    set->requestId = jdwp_requestid++;
    set->eventKind = jdwppacket_read_byte(req);
    switch (set->eventKind) {
        case JDWP_EVENTKIND_VM_DISCONNECTED: {
            break;
        }
        case JDWP_EVENTKIND_VM_START: {
            break;
        }
        case JDWP_EVENTKIND_THREAD_DEATH: {
            break;
        }
        case JDWP_EVENTKIND_SINGLE_STEP: {
            break;
        }
        case JDWP_EVENTKIND_BREAKPOINT: {
            break;
        }
        case JDWP_EVENTKIND_FRAME_POP: {
            break;
        }
        case JDWP_EVENTKIND_EXCEPTION: {
            break;
        }
        case JDWP_EVENTKIND_USER_DEFINED: {
            break;
        }
        case JDWP_EVENTKIND_THREAD_START: {
            break;
        }
        case JDWP_EVENTKIND_CLASS_PREPARE: {
            break;
        }
        case JDWP_EVENTKIND_CLASS_UNLOAD: {
            break;
        }
        case JDWP_EVENTKIND_CLASS_LOAD: {
            break;
        }
        case JDWP_EVENTKIND_FIELD_ACCESS: {
            break;
        }
        case JDWP_EVENTKIND_FIELD_MODIFICATION: {
            break;
        }
        case JDWP_EVENTKIND_EXCEPTION_CATCH: {
            break;
        }
        case JDWP_EVENTKIND_METHOD_ENTRY: {
            break;
        }
        case JDWP_EVENTKIND_METHOD_EXIT: {
            break;
        }
        case JDWP_EVENTKIND_METHOD_EXIT_WITH_RETURN_VALUE: {
            break;
        }
        case JDWP_EVENTKIND_VM_DEATH: {
            break;
        }
        default: {
            break;
        }
    }
    return set;
}

void jdwp_eventset_put(EventSet *set) {
    hashtable_put((jdwpserver.event_sets), (__refer) (long) set->requestId, set);
}

EventSet *jdwp_eventset_get(s32 id) {
    return hashtable_get((jdwpserver.event_sets), (__refer) (long) id);
}

s16 jdwp_eventset_process(EventSet *set) {
    if (set) {

    }
    return JDWP_ERROR_NONE;
}

void jdwp_eventset_clear(s32 id) {
    EventSet *set = jdwp_eventset_get(id);
    if (set) {

    }
    hashtable_remove(jdwpserver.event_sets, (__refer) (long) id, 0);
}

void jdwp_event_put(EventComposite *event) {
    arraylist_append(jdwpserver.events, event);
}

EventComposite *jdwp_event_get() {
    return arraylist_pop_front(jdwpserver.events);
}

//==================================================    1    ==================================================
void signatureToName(Utf8String *signature) {
    if (utf8_char_at(signature, 0) == 'L') {
        utf8_substring(signature, 1, signature->length);
    }
    if (utf8_char_at(signature, signature->length - 1) == ';') {
        utf8_substring(signature, 1, signature->length - 1);
    }
}

u8 getClassStatus(Class *clazz) {
    return clazz->status >= CLASS_STATUS_CLINITED ?
           JDWP_CLASS_STATUS_INITIALIZED | JDWP_CLASS_STATUS_PREPARED | JDWP_CLASS_STATUS_VERIFIED
                                                  : (clazz->status >= CLASS_STATUS_PREPARED ?
                                                     JDWP_CLASS_STATUS_PREPARED | JDWP_CLASS_STATUS_VERIFIED
                                                                                            : JDWP_CLASS_STATUS_VERIFIED);
}

CodeAttribute *getCodeAttribute(MethodInfo *method) {
    s32 i;
    CodeAttribute *ca = NULL;
    for (i = 0; i < method->attributes_count; i++) {
        ca = method->attributes[i].converted_code;
        if (ca) {
            break;
        }
    }
    return ca;
}


s32 getClassType(Class *clazz) {
    if (clazz->arr_data_type) {
        return JDWP_TYPETAG_ARRAY;
    } else if (clazz->cff.access_flags & ACC_INTERFACE) {
        return JDWP_TYPETAG_INTERFACE;
    } else {
        return JDWP_TYPETAG_CLASS;
    }
}

c8 getTag(u8 type) {
    char bytes = '0';
    switch (type) {
        case JDWP_TAG_BYTE:
        case JDWP_TAG_BOOLEAN:
            bytes = '1';
            break;
        case JDWP_TAG_SHORT:
        case JDWP_TAG_CHAR:
            bytes = '2';
            break;
        case JDWP_TAG_INT:
        case JDWP_TAG_FLOAT:
            bytes = '4';
            break;
        case JDWP_TAG_LONG:
        case JDWP_TAG_DOUBLE:
            bytes = '8';
            break;
        case JDWP_TAG_ARRAY:
        case JDWP_TAG_OBJECT:
        case JDWP_TAG_STRING:
        case JDWP_TAG_THREAD:
        case JDWP_TAG_THREAD_GROUP:
        case JDWP_TAG_CLASS_LOADER:
        case JDWP_TAG_CLASS_OBJECT:
            bytes = 'R';
            break;
        case JDWP_TAG_VOID:
            bytes = '0';
            break;
    }
    return bytes;
}

void writeValueType(JdwpPacket *res, ValueType *vt) {

    switch (getTag(vt->type)) {
        case '1':
            jdwppacket_write_byte(res, (c8) vt->value);
            break;
        case '2':
            jdwppacket_write_short(res, (short) vt->value);
            break;
        case '4':
            jdwppacket_write_int(res, (int) vt->value);
            break;
        case '8':
            jdwppacket_write_long(res, vt->value);
            break;
        case 'R':
            jdwppacket_write_refer(res, (__refer) (long) vt->value);
            break;
    }
}

void readValueType(JdwpPacket *req, ValueType *vt) {
    vt->type = jdwppacket_read_byte(req);
    switch (getTag(vt->type)) {
        case '1':
            vt->value = jdwppacket_read_byte(req);
            break;
        case '2':
            vt->value = jdwppacket_read_short(req);
            break;
        case '4':
            vt->value = jdwppacket_read_int(req);
            break;
        case '8':
            vt->value = jdwppacket_read_long(req);
            break;
        case 'R':
            vt->value = (s64) (long) jdwppacket_read_refer(req);
            break;
    }
}

void writeLocation(JdwpPacket *res, Location *loc) {
    jdwppacket_write_byte(res, loc->typeTag);
    jdwppacket_write_refer(res, loc->classID);
    jdwppacket_write_refer(res, loc->methodID);
    jdwppacket_write_long(res, loc->execIndex);
}

void readLocation(JdwpPacket *req, Location *loc) {
    loc->typeTag = jdwppacket_read_byte(req);
    loc->classID = jdwppacket_read_refer(req);
    loc->methodID = jdwppacket_read_refer(req);
    loc->execIndex = jdwppacket_read_long(req);
}

s64 getPtrValue(u8 type, u8 *ptr) {
    s64 value;
    switch (getTag(type)) {
        case '1':
            value = getFieldByte(ptr);
            break;
        case '2':
            value = getFieldShort(ptr);
            break;
        case '4':
            value = getFieldInt(ptr);
            break;
        case '8':
            value = getFieldLong(ptr);
            break;
        case 'R':
            value = (s64) (long) getFieldRefer(ptr);
            break;
    }
    return value;
}

s32 getRuntimeDeepth(Runtime *top) {
    s32 deep = 0;
    while (top) {
        deep++;
        top = top->son;
    }
    deep--;//top need not
    return deep;
}

Runtime *getLastSon(Runtime *top) {
    while (top) {
        if (!top->son)return top;
        top = top->son;
    }
    return NULL;
}


void writeArrayRegion(JdwpPacket *res, Instance *arr, s32 firstIndex, s32 length) {
    c8 type = utf8_char_at(arr->mb.clazz->name, 1);
    jdwppacket_write_byte(res, type);
    jdwppacket_write_int(res, length);
    c8 tag = getTag(type);
    s32 i;
    for (i = 0; i < length; i++) {
        switch (tag) {
            case '1':
                jdwppacket_write_byte(res, getFieldByte(&arr->arr_body[firstIndex + i]));
                break;
            case '2':
                jdwppacket_write_short(res, getFieldShort(&arr->arr_body[firstIndex + i * 2]));
                break;
            case '4':
                jdwppacket_write_int(res, getFieldInt(&arr->arr_body[firstIndex + i * 4]));
                break;
            case '8':
                jdwppacket_write_long(res, getFieldLong(&arr->arr_body[firstIndex + i * 8]));
                break;
            case 'R':
                jdwppacket_write_refer(res, getFieldRefer(&arr->arr_body[firstIndex + i * sizeof(__refer)]));
                break;
        }
    }
}

void jdwp_print_packet(JdwpPacket *packet) {
    s32 i;
    for (i = 0; i < packet->writePos; i++) {
        if (i % 5 == 0) { printf("      "); }
        if (i % 10 == 0) { printf("\n"); }
        printf(" %c[%2x]", packet->data[i] < ' ' || packet->data[i] > '~' ? ' ' : packet->data[i],
               (u8) packet->data[i]);
    }
    printf("\n------------------------------\n");
}
//==================================================    2    ==================================================

s32 jdwp_client_process(JdwpClient *client, Runtime *runtime) {
    JdwpPacket *req = jdwp_readpacket(client);
    if (req) {
        u16 cmd = jdwppacket_get_cmd_err(req);
        printf("request %x\n", cmd);
        JdwpPacket *res = jdwppacket_create();
        jdwppacket_set_flag(res, JDWP_PACKET_RESPONSE);
        jdwppacket_set_id(res, jdwppacket_get_id(req));
        switch (cmd) {
//set 1
            case JDWP_CMD_VirtualMachine_Version: {//1.1
                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                Utf8String *ustr = utf8_create();
                utf8_append_c(ustr, "MINI_JVM");
                jdwppacket_write_utf(res, ustr);
                jdwppacket_write_int(res, 0);
                jdwppacket_write_int(res, 1);
                utf8_clear(ustr);
                utf8_append_c(ustr, "0.1");
                jdwppacket_write_utf(res, ustr);
                utf8_clear(ustr);
                utf8_append_c(ustr, "Mini jvm");
                jdwppacket_write_utf(res, ustr);
                jdwp_writepacket(client, res);
                utf8_destory(ustr);
                jdwp_print_packet(req);
                jdwp_print_packet(res);
                break;
            }
            case JDWP_CMD_VirtualMachine_ClassesBySignature: {//1.2
                Utf8String *signature = jdwppacket_read_utf(req);
                jdwp_print_packet(req);
                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                signatureToName(signature);
                Class *cl = classes_get(signature);
                if (cl == NULL) {
                    jdwppacket_write_int(res, 0);
                } else {
                    jdwppacket_write_int(res, 1);
                    jdwppacket_write_byte(res, getClassType(cl));
                    jdwppacket_write_refer(res, cl);
                    jdwppacket_write_int(res, getClassStatus(cl));
                }
                jvm_printf("VirtualMachine_ClassesBySignature:%s ,%lld\n", utf8_cstr(signature), (s64) (long) cl);
                jdwp_writepacket(client, res);
                utf8_destory(signature);
                break;
            }
            case JDWP_CMD_VirtualMachine_AllClasses: {//1.3


                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_int(res, classes->entries);


                HashtableIterator hti;
                hashtable_iterate(classes, &hti);
                for (; hashtable_iter_has_more(&hti);) {
                    Utf8String *k = hashtable_iter_next_key(&hti);
                    Class *cl = hashtable_get(classes, k);

                    jdwppacket_write_byte(res, JDWP_TYPETAG_CLASS);
                    jdwppacket_write_refer(res, cl);
                    jdwppacket_write_utf(res, cl->name);
                    jdwppacket_write_int(res, getClassStatus(cl));
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_AllThreads: {//1.4

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_int(res, thread_list->length);
                s32 i;
                for (i = 0; i < thread_list->length; i++) {
                    Runtime *t = (Runtime *) arraylist_get_value(thread_list, i);
                    jdwppacket_write_refer(res, t);
                    jvm_printf("VirtualMachine_AllThreads: %llx\n", (s64) (long) t);
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_TopLevelThreadGroups: {//1.5

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_int(res, 0);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Dispose: {//1.6

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwp_writepacket(client, res);
                client->closed = 1;
                break;
            }
            case JDWP_CMD_VirtualMachine_IDSizes: {//1.7

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Suspend: {//1.8
                s32 i;
                for (i = 0; i < thread_list->length; i++) {
                    Runtime *t = (Runtime *) arraylist_get_value(thread_list, i);
                    t->threadInfo->suspend_count++;
                    jvm_printf("VirtualMachine_Suspend: %lld\n" + (s64) (long) t);
                }

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Resume: {//1.9
                s32 i;
                for (i = 0; i < thread_list->length; i++) {
                    Runtime *t = (Runtime *) arraylist_get_value(thread_list, i);
                    t->threadInfo->suspend_count--;
                    jvm_printf("VirtualMachine_Resume: %llx\n", (s64) (long) t);
                }

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Exit: {//1.10
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_CreateString: {//1.11
                Utf8String *str = jdwppacket_read_utf(req);//防止回收此处需要
                Instance *jstr = jstring_create(str, runtime);
                utf8_destory(str);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_refer(res, jstr);
                jvm_printf("VirtualMachine_CreateString: %s , rid: %llx\n", utf8_cstr(str), (s64) (long) jstr);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Capabilities: {//1.12

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_byte(res, 0);//canWatchFieldModification
                jdwppacket_write_byte(res, 0);//canWatchFieldAccess
                jdwppacket_write_byte(res, 0);//canGetBytecodes
                jdwppacket_write_byte(res, 0);//canGetSyntheticAttribute
                jdwppacket_write_byte(res, 0);//canGetOwnedMonitorInfo
                jdwppacket_write_byte(res, 0);//canGetCurrentContendedMonitor
                jdwppacket_write_byte(res, 0);//canGetMonitorInfo
                jvm_printf("VirtualMachine_Capabilities");
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_ClassPaths: {//1.13
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_DisposeObjects: {//1.14
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_HoldEvents: {//1.15
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_ReleaseEvents: {//1.16
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_CapabilitiesNew: {//1.17
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_RedefineClasses: {//1.18
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_SetDefaultStratum: {//1.19
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_VirtualMachine_AllClassesWithGeneric: {//1.20
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 2

            case JDWP_CMD_ReferenceType_Signature: {//2.1
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                Utf8String *str = utf8_create_c("L");
                utf8_append(str, ref->name);
                utf8_append_c(str, ";");
                jvm_printf("ReferenceType_Signature:%llx , %s \n", refType, utf8_cstr(str));
                jdwppacket_write_utf(res, str);
                jdwp_writepacket(client, res);
                utf8_destory(str);
                break;
            }
            case JDWP_CMD_ReferenceType_ClassLoader: {//2.2
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_refer(res, 0);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ReferenceType_Modifiers: {//2.3
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ReferenceType_Fields: {//2.4
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                s32 len = ref->fieldPool.field_used;
                jdwppacket_write_int(res, len);
                s32 i;
                for (i = 0; i < len; i++) {
                    //jvm_printf("method[" + i + "]" + ref.methods[i]);
                    jdwppacket_write_refer(res, &ref->fieldPool.field[i]);
                    jdwppacket_write_utf(res, ref->fieldPool.field[i].name);
                    jdwppacket_write_utf(res, ref->fieldPool.field[i].descriptor);
                    jdwppacket_write_int(res, ref->fieldPool.field[i].access_flags);
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ReferenceType_Methods: {//2.5
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                s32 len = ref->methodPool.method_used;
                jdwppacket_write_int(res, len);
                s32 i;
                for (i = 0; i < len; i++) {
                    //jvm_printf("method[" + i + "]" + ref.methods[i]);
                    jdwppacket_write_refer(res, &ref->methodPool.method[i]);
                    jdwppacket_write_utf(res, ref->methodPool.method[i].name);
                    jdwppacket_write_utf(res, ref->methodPool.method[i].descriptor);
                    jdwppacket_write_int(res, ref->methodPool.method[i].access_flags);
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ReferenceType_GetValues: {//2.6
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ReferenceType_SourceFile: {//2.7
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_utf(res, ref->source);
                jvm_printf("ReferenceType_SourceFile:%s\n", utf8_cstr(ref->source));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ReferenceType_NestedTypes: {//2.8
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ReferenceType_Status: {//2.9
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ReferenceType_Interfaces: {//2.10
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                s32 len = ref->interfacePool.clasz_used;
                jdwppacket_write_int(res, len);
                s32 i;
                for (i = 0; i < len; i++) {
                    jdwppacket_write_refer(res, &ref->interfacePool.clasz[i]);
                }
                jvm_printf("ReferenceType_Interfaces:%s\n", utf8_cstr(ref->name));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ReferenceType_ClassObject: {//2.11
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_refer(res, ref);
                jvm_printf("ReferenceType_ClassObject:%s\n", utf8_cstr(ref->name));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ReferenceType_SourceDebugExtension: {//2.12
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ReferenceType_SignatureWithGeneric: {//2.13
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ReferenceType_FieldsWithGeneric: {//2.14
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ReferenceType_MethodsWithGeneric: {//2.15
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 3

            case JDWP_CMD_ClassType_Superclass: {//3.1
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_refer(res, getSuperClass(ref));
                jvm_printf("JDWP_CMD_ClassType_Superclass:%s\n", utf8_cstr(ref->name));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ClassType_SetValues: {//3.2
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ClassType_InvokeMethod: {//3.3
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ClassType_NewInstance: {//3.4
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 4

            case JDWP_CMD_ArrayType_NewInstance: {//4.1
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 5
//set 6

            case JDWP_CMD_Method_LineTable: {//6.1
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);
                MethodInfo *method = jdwppacket_read_refer(req);
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                CodeAttribute *ca = getCodeAttribute(method);
                if (method->access_flags & ACC_NATIVE) {
                    jdwppacket_write_long(res, -1);
                    jdwppacket_write_long(res, -1);
                } else {
                    jdwppacket_write_long(res, (s64) (long) ca->code);
                    jdwppacket_write_long(res, (s64) (long) ca->code + ca->code_length);
                }
                jdwppacket_write_int(res, ca->line_number_table_length);
                s32 i;
                for (i = 0; i < ca->line_number_table_length; i++) {
                    jdwppacket_write_long(res, ca->line_number_table[i].start_pc);
                    jdwppacket_write_int(res, ca->line_number_table[i].line_number);
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_Method_VariableTable: {//6.2
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);
                MethodInfo *method = jdwppacket_read_refer(req);
                CodeAttribute *ca = getCodeAttribute(method);
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, method->para_count);
                jdwppacket_write_int(res, ca->local_var_table_length);
                s32 i;
                for (i = 0; i < ca->local_var_table_length; i++) {
                    LocalVarTable *tab = &ca->local_var_table[i];
                    jdwppacket_write_long(res, tab->start_pc);
                    jdwppacket_write_utf(res, get_utf8_string(ref, tab->name_index));
                    jdwppacket_write_utf(res, get_utf8_string(ref, tab->descriptor_index));
                    jdwppacket_write_int(res, tab->length);
                    jdwppacket_write_int(res, tab->index);
                }
                jvm_printf("Method_VariableTable:\n");
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_Method_Bytecodes: {//6.3
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_Method_IsObsolete: {//6.4
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_Method_VariableTableWithGeneric: {//6.5
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 8
//set 9

            case JDWP_CMD_ObjectReference_ReferenceType: {//9.1
                Instance *obj = (Instance *) jdwppacket_read_refer(req);
                Class *ref = obj->mb.clazz;


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
//                jvm_printf("obj [" + Long.toString(objid, 16) + "]");
//                jvm_printf("obj [" + Long.toString(objid, 16) + "].getClass()=" + obj.getClass());
                jdwppacket_write_byte(res, getClassType(ref));
                jdwppacket_write_refer(res, ref);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ObjectReference_GetValues: {//9.2
                Instance *obj = (Instance *) jdwppacket_read_refer(req);
                Class *ref = obj->mb.clazz;
                s32 fields = jdwppacket_read_int(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, fields);
                s32 i;
                for (i = 0; i < fields; i++) {
                    FieldInfo *fi = jdwppacket_read_refer(req);
                    ValueType vt;
                    vt.type = utf8_char_at(fi->descriptor, 0);
                    u8 *ptr = getFieldPtr_byName(obj, obj->mb.clazz->name, fi->name, fi->descriptor);
                    vt.value = getPtrValue(vt.type, ptr);
                    writeValueType(res, &vt);
                }
                //jvm_printf("ObjectReference_GetValues obj [" + Long.toString(objid, 16) + "]");
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ObjectReference_SetValues: {//9.3
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ObjectReference_MonitorInfo: {//9.5
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ObjectReference_InvokeMethod: {//9.6
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ObjectReference_DisableCollection: {//9.7
                Instance *obj = (Instance *) jdwppacket_read_refer(req);
                Class *ref = obj->mb.clazz;


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
//                jvm_printf("ObjectReference_DisableCollection:" + obj);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ObjectReference_EnableCollection: {//9.8
                Instance *obj = (Instance *) jdwppacket_read_refer(req);
                Class *ref = obj->mb.clazz;


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
//                jvm_printf("ObjectReference_EnableCollection:" + obj);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ObjectReference_IsCollected: {//9.9
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 10

            case JDWP_CMD_StringReference_Value: {//10.1
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 11

            case JDWP_CMD_ThreadReference_Name: {//11.1
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                u8 buf[20];//todo
                memset(&buf, 0, 20);
                sprintf(buf, "%llx", (s64) (long) runtime);
                Utf8String *ustr = utf8_create_c((c8 *) &buf);
                jdwppacket_write_utf(res, ustr);
                utf8_destory(ustr);
                jvm_printf("ThreadReference_Name:%s\n", buf);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Suspend: {//11.2
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);
                runtime->threadInfo->suspend_count++;


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Resume: {//11.3
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);
                runtime->threadInfo->suspend_count--;


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Status: {//11.4
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, JDWP_THREAD_RUNNING);
                jdwppacket_write_int(res, runtime->threadInfo->suspend_count ? JDWP_SUSPEND_STATUS_SUSPENDED : 0);
//                jvm_printf("ThreadReference_Status:" + t.getName() + "," + JdwpThreads.getStatus(t));
                jdwp_writepacket(client, res);
                break;
            }

            case JDWP_CMD_ThreadReference_ThreadGroup: {//11.5
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, 0);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Frames: {//11.6
                Runtime *thread = (Runtime *) jdwppacket_read_refer(req);
                s32 startFrame = jdwppacket_read_int(req);
                s32 length = jdwppacket_read_int(req);
                jvm_printf("ThreadReference_Frames: startFrame=%d, len=%d\n", startFrame, length);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                s32 deepth = getRuntimeDeepth(thread);
                if (length == -1) {//等于-1返回所有剩下的
                    length = deepth - startFrame;
                }
                jdwppacket_write_int(res, length);
                Runtime *r = getLastSon(thread);
                //jvm_printf("deepth:" + runtime.getDeepth());
                s32 i;
                for (i = 0; i < deepth; i++) {
                    if (i >= startFrame && i < startFrame + length) {//返回指定层级的runtimeframe
                        jdwppacket_write_refer(res, r);
                        Location loc;
                        loc.typeTag = getClassType(r->clazz);
                        loc.classID = r->clazz;
                        loc.methodID = r->methodInfo;
                        loc.execIndex = (s64) (long) r->pc - (s64) (long) r->bytecode;
                        writeLocation(res, &loc);
                    }
                    r = r->parent;
                    if (r == NULL) {
                        break;
                    }
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_FrameCount: {//11.7
                Runtime *thread = (Runtime *) jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, getRuntimeDeepth(thread));
                jvm_printf("ThreadReference_FrameCount:%d\n", getRuntimeDeepth(thread));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_OwnedMonitors: {//11.8
                Runtime *thread = (Runtime *) jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, 0);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_CurrentContendedMonitor: {//11.9
                Runtime *therad = (Runtime *) jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_refer(res, NULL);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Stop: {//11.10
                Runtime *thread = (Runtime *) jdwppacket_read_refer(req);
                pthread_cancel(thread->threadInfo->pthread);//todo need release all lock



                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Interrupt: {//11.11
                Runtime *thread = (Runtime *) jdwppacket_read_refer(req);
                //todo


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_SuspendCount: {//11.12
                Runtime *thread = (Runtime *) jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, thread->threadInfo->suspend_count);
                jvm_printf("ThreadReference_SuspendCount:%llx,%d\n", (s64) (long) thread,
                           thread->threadInfo->suspend_count);
                jdwp_writepacket(client, res);
                break;
            }
                //set 12

            case JDWP_CMD_ThreadGroupReference_Name: {//12.1
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ThreadGroupReference_Parent: {//12.2
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
            case JDWP_CMD_ThreadGroupReference_Children: {//12.3
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 13

            case JDWP_CMD_ArrayReference_Length: {//13.1
                Instance *arr = jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, arr->arr_length);
                jvm_printf("ArrayReference_Length:%d\n", arr->arr_length);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ArrayReference_GetValues: {//13.2
                Instance *arr = jdwppacket_read_refer(req);
                s32 firstIndex = jdwppacket_read_int(req);
                s32 length = jdwppacket_read_int(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jvm_printf("ArrayReference_GetValues:%llx\n", arr);
                writeArrayRegion(res, arr, firstIndex, length);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ArrayReference_SetValues: {//13.3
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }

//set 14
            case JDWP_CMD_ClassLoaderReference_VisibleClasses: {//14.1
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 15

            case JDWP_CMD_EventRequest_Set: {//15.1
                EventSet *eventSet = jdwp_create_eventset(req);
                jdwp_eventset_put(eventSet);
                s16 ret = jdwp_eventset_process(eventSet);


                if (ret == JDWP_ERROR_NONE) {
                    jdwppacket_set_err(res, JDWP_ERROR_NONE);
                    jdwppacket_write_int(res, eventSet->requestId);
                } else {
                    jdwppacket_set_err(res, ret);
                }
                jdwp_writepacket(client, res);
                break;
            }//
            case JDWP_CMD_EventRequest_Clear: {//15.2
                u8 eventKind = jdwppacket_read_byte(req);
                s32 requestID = jdwppacket_read_int(req);
                jvm_printf("EventRequest_Clear:eventKind=%d, requestID=%d\n", eventKind, requestID);
                jdwp_eventset_clear(requestID);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_EventRequest_ClearAllBreakpoints: {//15.3`


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
//set 16

            case JDWP_CMD_StackFrame_GetValues: {//16.1
                Runtime *thread = jdwppacket_read_refer(req);
                Runtime *frame = jdwppacket_read_refer(req);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                s32 slots = jdwppacket_read_int(req);
                jdwppacket_write_int(res, slots);
                Long2Double l2d;
                s32 i;
                for (i = 0; i < slots; i++) {
                    s32 slot = jdwppacket_read_int(req);
                    ValueType vt;
                    vt.type = jdwppacket_read_byte(req);
                    if (slot < frame->localvar_count) {
                        switch (getTag(vt.type)) {
                            case 'R':
                                vt.value = (s64) (long) frame->localVariables[slot].refer;
                                break;
                            case '8':
                                l2d.i2l.i1 = frame->localVariables[slot].integer;
                                l2d.i2l.i0 = frame->localVariables[slot + 1].integer;
                                vt.value = l2d.l;
                                break;
                            case '4':
                            case '2':
                            case '1':
                                vt.value = frame->localVariables[slot].integer;
                                break;
                        }
                    }
                    jvm_printf("JDWP_CMD_StackFrame_GetValues,thead=%llx , frame=%llx, val=%llx\n", (s64) (long) thread,
                               (s64) (long) frame, vt.value);
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_StackFrame_SetValues: {//16.2
                Runtime *thread = jdwppacket_read_refer(req);
                Runtime *frame = jdwppacket_read_refer(req);

                s32 slotValues = jdwppacket_read_int(req);
                Long2Double l2d;
                s32 i;
                for (i = 0; i < slotValues; i++) {
                    s32 slot = jdwppacket_read_int(req);
                    ValueType vt;
                    readValueType(req, &vt);
                    if (slot < frame->localvar_count) {
                        switch (getTag(vt.type)) {
                            case 'R':
                                frame->localVariables[slot].refer = (__refer) (long) vt.value;
                                break;
                            case '8':
                                l2d.l = vt.value;
                                frame->localVariables[slot].integer = l2d.i2l.i0;
                                frame->localVariables[slot + 1].integer = l2d.i2l.i1;
                                break;
                            case '4':
                            case '2':
                            case '1':
                                frame->localVariables[slot].integer = (s32) vt.value;
                                break;
                        }
                    }

                    jvm_printf("StackFrame_SetValues,thead=%llx , frame=%llx, val=%llx\n", (s64) (long) thread,
                               (s64) (long) frame, vt.value);
                }


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_StackFrame_ThisObject: {//16.3
                Runtime *thread = jdwppacket_read_refer(req);
                Runtime *frame = jdwppacket_read_refer(req);

                jvm_printf("StackFrame_ThisObject,thead=%llx , runtime=%llx \n", (s64) (long) thread,
                           (s64) (long) frame);


                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_refer(res, frame->methodInfo->access_flags & ACC_STATIC
                                            ? NULL : frame->localVariables[0].refer);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_StackFrame_PopFrames: {//16.4
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 17

            case JDWP_CMD_ClassObjectReference_ReflectedType: {//17.1
                Class *classObject = jdwppacket_read_refer(req);

                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_byte(res, getClassType(classObject));
                jdwppacket_write_refer(res, classObject);
                jvm_printf("ClassObjectReference_ReflectedType:%s\n", utf8_cstr(classObject->name));
                jdwp_writepacket(client, res);
                break;
            }

                //set 64

            case JDWP_CMD_Event_Composite: {
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }


        }


        jdwppacket_destory(req);
        jdwppacket_destory(res);
    }
    return 0;
}

