//
// Created by gust on 2017/9/20.
//

#ifndef MINI_JVM_JAVA_NATIVE_IO_H
#define MINI_JVM_JAVA_NATIVE_IO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../utils/utf8_string.h"

#define  SOCK_OP_TYPE_NON_BLOCK   0
#define  SOCK_OP_TYPE_REUSEADDR   1
#define  SOCK_OP_TYPE_RCVBUF   2
#define  SOCK_OP_TYPE_SNDBUF   3

#define  SOCK_OP_VAL_NON_BLOCK   1
#define  SOCK_OP_VAL_BLOCK   0
#define  SOCK_OP_VAL_NON_REUSEADDR   1
#define  SOCK_OP_VAL_REUSEADDR   0

s32 sock_open(Utf8String *ip, s32 port);

s32 sock_send(s32 sockfd, c8 *buf, s32 count);

s32 sock_recv(s32 sockfd, c8 *buf, s32 count);

s32 sock_option(s32 sockfd, s32 opType, s32 opValue);

s32 srv_bind(Utf8String *ip, u16 port);

s32 srv_listen(s32 listenfd);

s32 srv_accept(s32 listenfd);

s32 sock_close(s32 listenfd);


#ifdef __cplusplus
}
#endif


#endif //MINI_JVM_JAVA_NATIVE_IO_H
