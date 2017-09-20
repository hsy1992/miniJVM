//
// Created by gust on 2017/9/20.
//

#include <pthread.h>
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

    while (!exit) {
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
    }
    runtime_destory(&runtime);
    return srv;
}

s32 jdwp_jdwp_server() {
    jdwpserver.ip = utf8_create();
    jdwpserver.port = 8000;
    jdwpserver.exit = 0;
    jdwpserver.clients = arraylist_create(0);
    pthread_create(&jdwpserver.pt_listener, NULL, jdwp_thread_listener, &jdwpserver);
    pthread_create(&jdwpserver.pt_dispacher, NULL, jdwp_thread_dispacher, &jdwpserver);
    return 0;
}

//==================================================    packet    ==================================================



JdwpPacket *jdwppacket_create() {
    JdwpPacket *packet = jvm_alloc(sizeof(JdwpPacket));
    packet->data_len = 32;
    packet->readPos = 11;
    packet->writePos = 11;
    packet->data = jvm_alloc(packet->data_len);
    return packet;
}

JdwpPacket *jdwppacket_create_data(c8 *data, s32 len) {
    JdwpPacket *packet = jvm_alloc(sizeof(JdwpPacket));
    packet->data = data;
    packet->data_len = len;
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
    s64 l = ((packet->data[packet->readPos + 7] & 0xFFL) << 0)
            + ((packet->data[packet->readPos + 6] & 0xFFL) << 8)
            + ((packet->data[packet->readPos + 5] & 0xFFL) << 16)
            + ((packet->data[packet->readPos + 4] & 0xFFL) << 24)
            + ((packet->data[packet->readPos + 3] & 0xFFL) << 32)
            + ((packet->data[packet->readPos + 2] & 0xFFL) << 40)
            + ((packet->data[packet->readPos + 1] & 0xFFL) << 48)
            + ((packet->data[packet->readPos + 0] & 0xFFL) << 56);
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

void jdwppacket_ensureCapacity(JdwpPacket *packet, int length) {
    s32 newcount = packet->writePos + length;
    if (newcount >= packet->data_len) {
        newcount = newcount > packet->data_len << 1 ? newcount : packet->data_len << 1;
        c8 *tmp = jvm_alloc(newcount);
        memcpy(tmp, packet->data, packet->data_len);
        jvm_free(packet->data);
        packet->data = tmp;
    }
}

void jdwppacket_write_byte(JdwpPacket *packet, c8 val) {
    jdwppacket_ensureCapacity(packet, 1);
    packet->data[packet->writePos++] = val;
    packet->writePos++;;
}

void jdwppacket_write_short(JdwpPacket *packet, s16 val) {
    jdwppacket_ensureCapacity(packet, 2);
    packet->data[packet->writePos + 1] = (u8) (val >> 0);
    packet->data[packet->writePos + 0] = (u8) (val >> 8);
    packet->writePos += 2;
}


void jdwppacket_write_int(JdwpPacket *packet, s32 val) {
    jdwppacket_ensureCapacity(packet, 4);
    packet->data[packet->writePos + 1] = (u8) (val >> 0);
    packet->data[packet->writePos + 0] = (u8) (val >> 8);
    packet->data[packet->writePos + 1] = (u8) (val >> 16);
    packet->data[packet->writePos + 0] = (u8) (val >> 24);
    packet->writePos += 4;
}

void jdwppacket_write_long(JdwpPacket *packet, s64 val) {
    jdwppacket_ensureCapacity(packet, 8);
    packet->data[packet->writePos + 1] = (u8) (val >> 0);
    packet->data[packet->writePos + 0] = (u8) (val >> 8);
    packet->data[packet->writePos + 1] = (u8) (val >> 16);
    packet->data[packet->writePos + 0] = (u8) (val >> 24);
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
    int len = 0;
    int last = pos + n;
    for (int i = pos; i < last; i++) {
        len <<= 8;
        len += (u8) packet->data[i];
    }
    return len;
}

s16 jdwppacket_get_cmd_err(JdwpPacket *packet) {
    return (u8) jdwppacket_getbypos(packet, 9, 2);
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
    int pos = 8;
    packet->data[pos++] = flag;
}

void jdwppacket_set_id(JdwpPacket *packet, s32 id) {
    int pos = 4;
    packet->data[pos++] = (u8) (id >> 24);
    packet->data[pos++] = (u8) (id >> 16);
    packet->data[pos++] = (u8) (id >> 8);
    packet->data[pos++] = (u8) (id);
}

void jdwppacket_set_length(JdwpPacket *packet, s32 len) {
    int pos = 0;
    packet->data[pos++] = (u8) (len >> 24);
    packet->data[pos++] = (u8) (len >> 16);
    packet->data[pos++] = (u8) (len >> 8);
    packet->data[pos++] = (u8) (len);
}

void jdwppacket_set_cmd(JdwpPacket *packet, u8 set, u8 cmd) {
    int pos = 9;
    packet->data[pos++] = set;
    packet->data[pos++] = cmd;
}

void jdwppacket_set_err(JdwpPacket *packet, u16 err) {
    int pos = 9;
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
        s32 len = jdwp_read_fully(client, &buf, 4);
        Int2Float i2f;
        s32 pack_len =
                ((buf[0] & 0xFF) << 24) | ((buf[1] & 0xFF) << 16) | ((buf[2] & 0xFF) << 8) | ((buf[3] & 0xFF) << 0);
        JdwpPacket *p = jdwppacket_create();
        jdwppacket_ensureCapacity(p, pack_len);
        jdwppacket_set_length(p, pack_len);
        len = jdwp_read_fully(client, &p->data[p->writePos], pack_len - 4);
        if (len < 0) {
            jdwppacket_destory(p);
            client->closed = 1;
        } else {
            return p;
        }
    } else {//首次连接
        c8 buf[14];
        s32 len = jdwp_read_fully(client, &buf, 14);
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
    }
    return NULL;
}


s32 jdwp_writepacket(JdwpClient *client, JdwpPacket *packet) {
    s32 len = jdwp_write_fully(client, packet->data, packet->writePos);
    if (len < 0) {
        client->closed = 1;
        return 1;
    }
    return 0;
}
//==================================================    0    ==================================================



//==================================================    1    ==================================================
void signatureToName(Utf8String *signature) {
    if (utf8_char_at(signature, 0) == ("L")) {
        utf8_substring(signature, 1, signature->length);
    }
    if (utf8_char_at(signature, signature->length - 1) == ";") {
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
        case Tag.BYTE:
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

void writeValueType(JdwpPacket *res, u8 type, s64 value) {
    switch (getTag(type)) {
        case '1':
            jdwppacket_write_byte(res, (c8) value);
            break;
        case '2':
            jdwppacket_write_short(res, (short) value);
            break;
        case '4':
            jdwppacket_write_int(res, (int) value);
            break;
        case '8':
            jdwppacket_write_long(res, value);
            break;
        case 'R':
            jdwppacket_write_refer(res, value);
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
    Runtime *son = top;
    while (top) {
        if (!top->son)return son;
        top = top->son;
    }
    return NULL;
}
//==================================================    2    ==================================================

s32 jdwp_client_process(JdwpClient *client, Runtime *runtime) {
    JdwpPacket *req = jdwp_readpacket(client);
    if (req) {
        u16 cmd = jdwppacket_get_cmd_err(req);
        switch (cmd) {
//set 1
            case JDWP_CMD_VirtualMachine_Version: {//1.1
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_write_utf(res, "MINI_JVM");
                jdwppacket_write_int(res, 0);
                jdwppacket_write_int(res, 1);
                jdwppacket_write_utf(res, "0.1");
                jdwppacket_write_utf(res, "Mini jvm");
                jdwp_writepacket(client, res);
//                            jvm_printf(res);
                break;
            }
            case JDWP_CMD_VirtualMachine_ClassesBySignature: {//1.2
                Utf8String *signature = jdwppacket_read_utf(req);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                signatureToName(signature);
                Class *cl = classes_get(signature);
                utf8_destory(signature);
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
                break;
            }
            case JDWP_CMD_VirtualMachine_AllClasses: {//1.3

                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_write_int(res, thread_list->length);
                s32 i;
                for (i = 0; i < thread_list->length; i++) {
                    Runtime *t = (Runtime *) arraylist_get_value(thread_list, i);
                    jdwppacket_write_refer(res, t);
                    jvm_printf("VirtualMachine_AllThreads: %lld" + (s64) (long) t);
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_TopLevelThreadGroups: {//1.5
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_write_int(res, 0);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Dispose: {//1.6
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwp_writepacket(client, res);
                client->closed = 1;
                break;
            }
            case JDWP_CMD_VirtualMachine_IDSizes: {//1.7
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwppacket_write_int(res, sizeof(__refer));
                jdwp_writepacket(client, res);
                jvm_printf(res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Suspend: {//1.8
                s32 i;
                for (i = 0; i < thread_list->length; i++) {
                    Runtime *t = (Runtime *) arraylist_get_value(thread_list, i);
                    t->threadInfo->suspend_count++;
                    jvm_printf("VirtualMachine_Suspend: %lld\n" + (s64) (long) t);
                }
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Resume: {//1.9
                s32 i;
                for (i = 0; i < thread_list->length; i++) {
                    Runtime *t = (Runtime *) arraylist_get_value(thread_list, i);
                    t->threadInfo->suspend_count--;
                    jvm_printf("VirtualMachine_Resume: %lld\n" + (s64) (long) t);
                }
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_write_refer(res, jstr);
                jvm_printf("VirtualMachine_CreateString: %s , rid: %llx\n", utf8_cstr(str), (s64) (long) jstr);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_VirtualMachine_Capabilities: {//1.12
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                s32 len = ref->fieldPool.field_used;
                jdwppacket_write_int(res, len);
                for (int i = 0; i < len; i++) {
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                s32 len = ref->methodPool.method_used;
                jdwppacket_write_int(res, len);
                for (int i = 0; i < len; i++) {
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                s32 len = ref->interfacePool.clasz_used;
                jdwppacket_write_int(res, len);
                for (int i = 0; i < len; i++) {
                    jdwppacket_write_refer(res, &ref->interfacePool.clasz[i]);
                }
                jvm_printf("ReferenceType_Interfaces:%s\n", utf8_cstr(ref->name));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ReferenceType_ClassObject: {//2.11
                __refer refType = jdwppacket_read_refer(req);
                Class *ref = (Class *) (refType);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                CodeAttribute *ca = getCodeAttribute(method);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_long(res, (s64) (long) ca->code);
                jdwppacket_write_long(res, (s64) (long) ca->code + ca->code_length);
                jdwppacket_write_int(res, ca->line_number_table_length);
//                jvm_printf("Method_LineTable:" + method.codeStart + "," + method.codeEnd + "," + method.lines +
//                           " ,arrlen=" + method.lineNum.length);
                s32 i;
                for (int i = 0; i < ca->line_number_table_length; i++) {
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

                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                jdwppacket_write_int(res, method->para_count);

                jdwppacket_write_int(res, ca->local_var_table_length);
                for (int i = 0; i < ca->local_var_table_length; i++) {
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                int fields = jdwppacket_read_int(req);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, fields);
                for (int i = 0; i < fields; i++) {
                    FieldInfo *fi = jdwppacket_read_refer(req);
                    u8 type = utf8_char_at(fi->descriptor, 0);
                    u8 *ptr = getFieldPtr_byName(obj, obj->mb.clazz->name, fi->name, fi->descriptor);
                    writeValueType(res, type, getPtrValue(type, ptr));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
//                jvm_printf("ObjectReference_DisableCollection:" + obj);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ObjectReference_EnableCollection: {//9.8
                Instance *obj = (Instance *) jdwppacket_read_refer(req);
                Class *ref = obj->mb.clazz;
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
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
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                u8 buf[20];//todo
                memset(&buf, 0, 20);
                sprintf(buf, "%llx", (s64) (long) runtime);
                Utf8String *ustr = utf8_create_c(&buf);
                jdwppacket_write_utf(res, ustr);
                utf8_destory(ustr);
                jvm_printf("ThreadReference_Name:%s\n", buf);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Suspend: {//11.2
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);
                runtime->threadInfo->suspend_count++;
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Resume: {//11.3
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);
                runtime->threadInfo->suspend_count--;
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Status: {//11.4
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);

                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, JDWP_THREAD_RUNNING);
                jdwppacket_write_int(res, runtime->threadInfo->suspend_count ? JDWP_SUSPEND_STATUS_SUSPENDED : 0);
//                jvm_printf("ThreadReference_Status:" + t.getName() + "," + JdwpThreads.getStatus(t));
                jdwp_writepacket(client, res);
                break;
            }

            case JDWP_CMD_ThreadReference_ThreadGroup: {//11.5
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, 0);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Frames: {//11.6
                Runtime *runtime = (Runtime *) jdwppacket_read_refer(req);
                int startFrame = jdwppacket_read_int(req);
                int length = jdwppacket_read_int(req);
                jvm_printf("ThreadReference_Frames: startFrame=%d, len=%d\n", startFrame, length);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);


                int deepth = getRuntimeDeepth(runtime);
                if (length == -1) {//等于-1返回所有剩下的
                    length = deepth - startFrame;
                }
                jdwppacket_write_int(res, length);
                Runtime *r = getLastSon(runtime);
                //jvm_printf("deepth:" + runtime.getDeepth());
                for (int i = 0; i < deepth; i++) {
                    if (i >= startFrame && i < startFrame + length) {//返回指定层级的runtimeframe
                        jdwppacket_write_refer(res, r);
                        Location loc;
                        loc.typeTag = getClassType(r->clazz);
                        loc.classID = r->clazz;
                        loc.methodID = r->methodInfo;
                        loc.execIndex = (s64) (long) r->pc - (s64) (long) r->bytecode;
                        writeLocation(res, &loc);
                        //jvm_printf(loc);
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
                long thread = jdwppacket_read_refer(req);
                Thread t = (Thread) JdwpNative.referenceObj(thread);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, JdwpThreads.getFrameCount(t));
                jvm_printf("ThreadReference_FrameCount:" + JdwpThreads.getFrameCount(t));
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_OwnedMonitors: {//11.8
                long thread = jdwppacket_read_refer(req);
                Thread t = (Thread) JdwpNative.referenceObj(thread);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_utf(res, t.getName());
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_CurrentContendedMonitor: {//11.9
                long thread = jdwppacket_read_refer(req);
                Thread t = (Thread) JdwpNative.referenceObj(thread);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_refer(res, 0);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Stop: {//11.10
                long thread = jdwppacket_read_refer(req);
                Thread t = (Thread) JdwpNative.referenceObj(thread);
                long objid = jdwppacket_read_refer(req);
                JdwpThreads.stopThread(t, objid);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_Interrupt: {//11.11
                long thread = jdwppacket_read_refer(req);
                Thread t = (Thread) JdwpNative.referenceObj(thread);
                t.interrupt();
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ThreadReference_SuspendCount: {//11.12
                long thread = jdwppacket_read_refer(req);
                Thread t = (Thread) JdwpNative.referenceObj(thread);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, JdwpThreads.getSuspendCount(t));
                jvm_printf("ThreadReference_SuspendCount:" + t.getName() + "," + JdwpThreads.getSuspendCount(t));
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

            case JDWP_CMD_ArrayReference_Length: {
                long arrayObject = jdwppacket_read_refer(req);
                Array
                        arr = new
                Array(arrayObject);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, arr.length);
                jvm_printf("ArrayReference_Length:" + arr.length);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ArrayReference_GetValues: {
                long arrayObject = jdwppacket_read_refer(req);
                int firstIndex = jdwppacket_read_int(req);
                int length = jdwppacket_read_int(req);
                Array
                        arr = new
                Array(arrayObject);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_int(res, arr.length);
                jvm_printf("ArrayReference_GetValues:" + arr);
                arr.write(res, firstIndex, length);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_ArrayReference_SetValues: {
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
                //set 14

            case JDWP_CMD_ClassLoaderReference_VisibleClasses: {
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 15

            case JDWP_CMD_EventRequest_Set: {//15.1
                EventSet eventSet = EventManager.createEventSet(req);
                EventManager.putEventSet(eventSet);
                short ret = eventSet.process();
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                if (ret == JDWP_ERROR_NONE) {
                    jdwppacket_set_err(res, JDWP_ERROR_NONE);
                    jdwppacket_write_int(res, eventSet.getRequestId());
                } else {
                    jdwppacket_set_err(res, ret);
                }
                jdwp_writepacket(client, res);
                break;
            }//
            case JDWP_CMD_EventRequest_Clear: {//15.2
                byte eventKind = jdwppacket_read_byte(req);
                int requestID = jdwppacket_read_int(req);
                jvm_printf("EventRequest_Clear:eventKind=" + eventKind + ", requestID=" + requestID);
                EventSet set = EventManager.getEventSet(requestID);
                if (set != NULL) {
                    set.clear();
                    EventManager.removeEventSet(requestID);
                }
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_EventRequest_ClearAllBreakpoints: {//15.3`
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                Random
                        r = new
                Random();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
//set 16

            case JDWP_CMD_StackFrame_GetValues: {//16.1
                long thread = jdwppacket_read_refer(req);
                long frame = jdwppacket_read_refer(req);
                StackFrame r = new
                StackFrame(frame);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);

                int slots = jdwppacket_read_int(req);
                jdwppacket_write_int(res, slots);
                for (int i = 0; i < slots; i++) {
                    int slot = jdwppacket_read_int(req);
                    byte type = jdwppacket_read_byte(req);
                    ValueType
                            val = new
                    ValueType(type);
                    JdwpNative.getLocalVal(frame, slot, val);
                    val.write(res);
                    jvm_printf("StackFrame_GetValues,thead=" + thread + " , frame=" + frame + ", val=" + val);
                }
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_StackFrame_SetValues: {//16.2
                long thread = jdwppacket_read_refer(req);
                long frame = jdwppacket_read_refer(req);
                int slotValues = jdwppacket_read_int(req);
                for (int i = 0; i < slotValues; i++) {
                    int slot = jdwppacket_read_int(req);
                    ValueType
                            val = new
                    ValueType(req);
                    JdwpNative.setLocalVal(frame, slot, val.type, val.value, val.bytes);
                    jvm_printf("StackFrame_SetValues,thead=" + thread + " , frame=" + frame + ", val=" + val);
                }
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_StackFrame_ThisObject: {//16.3
                long thread = jdwppacket_read_refer(req);
                long frame = jdwppacket_read_refer(req);
                StackFrame r = new
                StackFrame(frame);
                jvm_printf("StackFrame_ThisObject,thead=" + thread + ", r=" + r);

                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_write_refer(res, r.localThis);
                jdwp_writepacket(client, res);
                break;
            }
            case JDWP_CMD_StackFrame_PopFrames: {//16.4
                jvm_printf("%x not support\n", jdwppacket_get_cmd_err(req));
                break;
            }
//set 17

            case JDWP_CMD_ClassObjectReference_ReflectedType: {//17.1
                long classObject = jdwppacket_read_refer(req);
                Instance *obj = JdwpNative.referenceObj(classObject);
                JdwpPacket *res = jdwppacket_create();
                jdwppacket_set_err(res, JDWP_ERROR_NONE);
                jdwppacket_set_id(res, jdwppacket_get_id(req));
                jdwppacket_write_refer(res, JdwpNative.referenceId(obj.getClass()));
                jvm_printf("ClassObjectReference_ReflectedType:" + obj.getClass());
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
    }
    return 0;
}

