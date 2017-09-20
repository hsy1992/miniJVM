//
// Created by gust on 2017/9/20.
//

#ifndef MINI_JVM_JAVA_NATIVE_IO_H
#define MINI_JVM_JAVA_NATIVE_IO_H


#include "../utils/utf8_string.h"
#include "global.h"

s32 sock_open(Utf8String *ip, s32 port);

s32 sock_send(s32 sockfd, c8 *buf, s32 count);

s32 sock_recv(s32 sockfd, c8 *buf, s32 count);

s32 setOption(s32 sockfd, s32 opType, s32 opValue);

s32 srv_bind(Utf8String *ip, u16 port);

s32 srv_listen(s32 listenfd);

s32 srv_accept(s32 listenfd);

s32 srv_close(s32 listenfd);


#endif //MINI_JVM_JAVA_NATIVE_IO_H
