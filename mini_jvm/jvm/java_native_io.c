//
// Created by gust on 2017/9/1.
//

#include "jvm.h"
#include "java_native_std.h"
#include "garbage.h"
#include "java_native_io.h"
#include "jvm_util.h"
#include "../utils/miniz/miniz_wrapper.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>

#define   err jvm_printf

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>   // NULL and possibly memcpy, memset

#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__

#include <WinSock2.h>
#include <Ws2tcpip.h>

typedef int socklen_t;
#pragma comment(lib, "Ws2_32.lib")
#define SHUT_RDWR SD_BOTH
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#else

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define INVALID_SOCKET    -1
#define SOCKET_ERROR      -1
#define closesocket(fd)   close(fd)
#endif

#if __JVM_OS_VS__
#include "../utils/dirent_win.h"
#else

#include <dirent.h>
#include <unistd.h>

#endif

#include <errno.h>


//=================================  socket  ====================================
s32 sock_option(s32 sockfd, s32 opType, s32 opValue) {
    s32 ret = 0;
    switch (opType) {
        case SOCK_OP_TYPE_NON_BLOCK: {//阻塞设置

#if __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__ || __JVM_OS_VS__
#if __JVM_OS_CYGWIN__
            __ms_u_long ul = 1;
//fix cygwin bug ,cygwin FIONBIO = 0x8008667E
#undef FIONBIO
#define FIONBIO 0x8004667E
#else
            u_long ul = 1;
#endif
            if (!opValue) {
                ul = 0;
            }
            //jvm_printf(" FIONBIO:%x\n", FIONBIO);
            ret = ioctlsocket(sockfd, FIONBIO, &ul);
            if (ret == SOCKET_ERROR) {
                err("set socket non_block error: %s\n", strerror(errno));
                s32 ec = WSAGetLastError();
                //jvm_printf(" error code:%d\n", ec);
            }
#else
            if (opValue) {
                s32 flags = fcntl(sockfd, F_GETFL, 0);
                ret = fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
                if (ret) {
                    err("set socket non_block error.\n");
                    //printf("errno.%02d is: %s\n", errno, strerror(errno));
                }
            } else {
                //fcntl(sockfd, F_SETFL, O_BLOCK);
            }
#endif
            break;
        }
        case SOCK_OP_TYPE_REUSEADDR: {//
            s32 x = 1;
            ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &x, sizeof(x));
            break;
        }
        case SOCK_OP_TYPE_RCVBUF: {//缓冲区设置
            int nRecvBuf = opValue;//设置为 opValue K
            ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char *) &nRecvBuf, sizeof(int));
            break;
        }
        case SOCK_OP_TYPE_SNDBUF: {//缓冲区设置
            s32 nRecvBuf = opValue;//设置为 opValue K
            ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (const char *) &nRecvBuf, sizeof(int));
            break;
        }
    }
    return ret;
}

s32 sock_recv(s32 sockfd, c8 *buf, s32 count) {
    s32 len = (s32) recv(sockfd, buf, count, 0);

    if (len == 0) {//如果是正常断开，返回-1
        len = -1;
    } else if (len == -1) {//如果发生错误
        len = -1;
#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
        if (WSAEWOULDBLOCK == WSAGetLastError()) {//但是如果是非阻塞端口，说明连接仍正常
            //jvm_printf("sc send error client time = %f ;\n", (f64)clock());
            len = 0;
        }
#else
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            len = 0;
        }
#endif
    }
    return len;
}


s32 sock_send(s32 sockfd, c8 *buf, s32 count) {
    s32 len = (s32) send(sockfd, buf, count, 0);

    if (len == 0) {//如果是正常断开，返回-1
        len = -1;
    } else if (len == -1) {//如果发生错误
#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
        if (WSAEWOULDBLOCK == WSAGetLastError()) {//但是如果是非阻塞端口，说明连接仍正常
            //jvm_printf("sc send error server time = %f ;\n", (f64)clock());
            len = 0;
        }
#else
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            len = 0;
        }
#endif

    }
    return len;
}

s32 sock_open(Utf8String *ip, s32 port) {
    s32 sockfd = -1;

#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1, 1), &wsaData);
#endif  /*  WIN32  */

    struct hostent *host;
    if ((host = gethostbyname(utf8_cstr(ip))) == NULL) { /* get the host info */
        err("get host by name error: %s\n", strerror(errno));
        sockfd = -1;
    } else {

        if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
            err(strerror(errno));
            err("socket init error: %s\n", strerror(errno));
            sockfd = -1;
        } else {
            s32 x = 1;
            if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &x, sizeof(x)) == -1) {
                err("socket reuseaddr error: %s\n", strerror(errno));
                sockfd = -1;
            } else {
                struct sockaddr_in sock_addr; /* connector's address information */
                memset((char *) &sock_addr, 0, sizeof(sock_addr));
                sock_addr.sin_family = AF_INET; /* host byte order */
                sock_addr.sin_port = htons((u16) port); /* short, network byte order */
#if __JVM_OS_MAC__ || __JVM_OS_LINUX__
                sock_addr.sin_addr = *((struct in_addr *) host->h_addr_list[0]);
#else
                sock_addr.sin_addr = *((struct in_addr *) host->h_addr);
#endif
                memset(&(sock_addr.sin_zero), 0, sizeof((sock_addr.sin_zero))); /* zero the rest of the struct */
                if (connect(sockfd, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) == -1) {
                    err("socket connect error: %s\n", strerror(errno));
                    sockfd = -1;
                }
            }
        }
    }
    return sockfd;
}

//=================================  serversocket  ====================================

s32 srv_bind(Utf8String *ip, u16 port) {

    struct sockaddr_in server_addr;
    s32 listenfd = -1;
#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(1, 1), &wsadata) == SOCKET_ERROR) {
        err("Error creating serversocket: %s\n", strerror(errno));
    }
#endif

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("Error opening serversocket: %s\n", strerror(errno));
        listenfd = -1;
    } else {

        struct hostent *host;

        memset((char *) &server_addr, 0, sizeof(server_addr));//清0
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        if (ip->length) {//如果指定了ip
            if ((host = gethostbyname(utf8_cstr(ip))) == NULL) { /* get the host info */
                err("get host by name error: %s\n", strerror(errno));
            }
#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
            server_addr.sin_addr = *((struct in_addr *) host->h_addr);
#else
            //server_addr.sin_len = sizeof(struct sockaddr_in);
            server_addr.sin_addr = *((struct in_addr *) host->h_addr_list[0]);
#endif
        } else {
            server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        s32 on = 1;
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
        if ((bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
            err("Error binding serversocket: %s\n", strerror(errno));
            closesocket(listenfd);
            listenfd = -1;
        }
    }
    return listenfd;
}


s32 srv_listen(s32 listenfd) {
    u16 MAX_LISTEN = 64;
    if ((listen(listenfd, MAX_LISTEN)) < 0) {
        err("Error listening on serversocket: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

s32 srv_accept(s32 listenfd) {
    struct sockaddr_in clt_addr;
    memset(&clt_addr, 0, sizeof(clt_addr)); //清0
    s32 clt_addr_length = sizeof(clt_addr);
    s32 clt_socket_fd = accept(listenfd, (struct sockaddr *) &clt_addr, (socklen_t *) &clt_addr_length);
    if (clt_socket_fd == -1) {
        if (errno != EINTR) {
            err("Error accepting on serversocket: %s\n", strerror(errno));
        }
    }

    return clt_socket_fd;
}

s32 sock_close(s32 listenfd) {
    if (listenfd) {
        shutdown(listenfd, SHUT_RDWR);
        closesocket(listenfd);
#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
        WSACancelBlockingCall();
        WSACleanup();
#endif
    }
    return 0;
}


//=================================  native  ====================================

s32 isDir(Utf8String *path) {
    struct stat buf;
    stat(utf8_cstr(path), &buf);
    s32 a = S_ISDIR(buf.st_mode);
    return a;
}

Utf8String *getTmpDir() {
    Utf8String *tmps = utf8_create();
#if __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__ || __JVM_OS_VS__
    c8 buf[128];
    s32 len = GetTempPath(128, buf);
    utf8_append_data(tmps, buf, len);
#else

#ifndef P_tmpdir
#define P_tmpdir "/tmp"
#endif
    utf8_append_c(tmps, P_tmpdir);
#endif
    return tmps;
}


s32 javax_mini_net_socket_Protocol_open0(Runtime *runtime, JClass *clazz) {
    RuntimeStack *stack = runtime->stack;
    Instance *jbyte_arr = (Instance *) localvar_getRefer(runtime->localvar, 0);
    s32 port = localvar_getInt(runtime->localvar, 1);
    s32 mode = localvar_getInt(runtime->localvar, 2);
    Utf8String *ip = utf8_create_part_c(jbyte_arr->arr_body, 0, jbyte_arr->arr_length);

    jthread_block_enter(runtime);
    s32 sockfd = sock_open(ip, port);
    jthread_block_exit(runtime);
    utf8_destory(ip);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_open0  \n");
#endif
    push_int(stack, sockfd);
    return 0;
}

s32 javax_mini_net_socket_Protocol_readBuf(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    Instance *jbyte_arr = (Instance *) localvar_getRefer(runtime->localvar, 1);
    s32 offset = localvar_getInt(runtime->localvar, 2);
    s32 count = localvar_getInt(runtime->localvar, 3);

    jthread_block_enter(runtime);
    s32 len = sock_recv(sockfd, jbyte_arr->arr_body + offset, count);
    jthread_block_exit(runtime);
    push_int(runtime->stack, len);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_readBuf  \n");
#endif
    return 0;
}

s32 javax_mini_net_socket_Protocol_readByte(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    c8 b = 0;
    jthread_block_enter(runtime);
    s32 len = sock_recv(sockfd, &b, 1);
    jthread_block_exit(runtime);
    if (len < 0) {
        push_int(runtime->stack, -1);
    } else {
        push_int(runtime->stack, (u8) b);

    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_readByte  \n");
#endif
    return 0;
}


s32 javax_mini_net_socket_Protocol_writeBuf(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    Instance *jbyte_arr = (Instance *) localvar_getRefer(runtime->localvar, 1);
    s32 offset = localvar_getInt(runtime->localvar, 2);
    s32 count = localvar_getInt(runtime->localvar, 3);

    jthread_block_enter(runtime);
    s32 len = sock_send(sockfd, jbyte_arr->arr_body + offset, count);
    jthread_block_exit(runtime);

    push_int(runtime->stack, len);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_writeBuf  \n");
#endif
    return 0;
}

s32 javax_mini_net_socket_Protocol_writeByte(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    s32 val = localvar_getInt(runtime->localvar, 1);
    c8 b = (u8) val;
    jthread_block_enter(runtime);
    s32 len = sock_send(sockfd, &b, 1);
    jthread_block_exit(runtime);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_writeByte  \n");
#endif
    push_int(runtime->stack, len);
    return 0;
}

s32 javax_mini_net_socket_Protocol_available0(Runtime *runtime, JClass *clazz) {
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_available0  \n");
#endif
    push_int(runtime->stack, 0);
    return 0;
}

s32 javax_mini_net_socket_Protocol_close0(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    sock_close(sockfd);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_close0  \n");
#endif
    return 0;
}

s32 javax_mini_net_socket_Protocol_setOption0(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    s32 type = localvar_getInt(runtime->localvar, 1);
    s32 val = localvar_getInt(runtime->localvar, 2);
    s32 ret = 0;
    if (sockfd) {
        ret = sock_option(sockfd, type, val);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_socket_Protocol_setOption0  \n");
#endif
    return 0;
}


s32 javax_mini_net_serversocket_Protocol_open0(Runtime *runtime, JClass *clazz) {
    Instance *jbyte_arr = localvar_getRefer(runtime->localvar, 0);
    s32 port = localvar_getInt(runtime->localvar, 1);
    Utf8String *ip = utf8_create_part_c(jbyte_arr->arr_body, 0, jbyte_arr->arr_length);
    s32 sockfd = 0;
    jthread_block_enter(runtime);
    sockfd = srv_bind(ip, port);
    jthread_block_exit(runtime);
    push_int(runtime->stack, sockfd);
    utf8_destory(ip);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_serversocket_Protocol_open0  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_listen0(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    s32 ret = 0;
    if (sockfd) {
        jthread_block_enter(runtime);
        ret = srv_listen(sockfd);
        jthread_block_exit(runtime);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_serversocket_Protocol_listen0  \n");
#endif
    return 0;
}


s32 javax_mini_net_serversocket_Protocol_accept0(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    s32 ret = 0;
    if (sockfd) {

        jthread_block_enter(runtime);
        ret = srv_accept(sockfd);
        jthread_block_exit(runtime);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_serversocket_Protocol_accept0  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_close0(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    s32 ret = 0;
    if (sockfd) {
        ret = sock_close(sockfd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_serversocket_Protocol_close0  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_registerCleanup(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    s32 ret = 0;
    if (sockfd) {

    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_serversocket_Protocol_registerCleanup  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_finalize(Runtime *runtime, JClass *clazz) {
    s32 sockfd = localvar_getInt(runtime->localvar, 0);
    s32 ret = 0;
    if (sockfd) {

    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_net_serversocket_Protocol_finalize  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_openFile(Runtime *runtime, JClass *clazz) {
    Instance *name_arr = localvar_getRefer(runtime->localvar, 0);
    Instance *mode_arr = localvar_getRefer(runtime->localvar, 1);
    if (name_arr) {
        FILE *fd = fopen(name_arr->arr_body, mode_arr->arr_body);
        push_long(runtime->stack, (s64) (intptr_t) fd);
    } else {
        push_long(runtime->stack, 0);
    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_openFile  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_closeFile(Runtime *runtime, JClass *clazz) {
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, 0);
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    s32 ret = -1;
    if (fd) {
        ret = fclose(fd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_closeFile  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_read0(Runtime *runtime, JClass *clazz) {
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, 0);
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    s32 ret = -1;
    if (fd) {
        ret = fgetc(fd);
        if (ret == EOF) {
            push_int(runtime->stack, -1);
        } else {
            push_int(runtime->stack, ret);
        }
    } else {
        push_int(runtime->stack, ret);
    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_read0  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_write0(Runtime *runtime, JClass *clazz) {
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, 0);
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    u8 byte = (u8) localvar_getInt(runtime->localvar, 2);
    s32 ret = -1;
    if (fd) {
        ret = fputc(byte, fd);
        if (ret == EOF) {
            push_int(runtime->stack, -1);
        } else {
            push_int(runtime->stack, byte);
        }
    } else {
        push_int(runtime->stack, ret);
    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_write0  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_readbuf(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    Instance *bytes_arr = localvar_getRefer(runtime->localvar, pos++);
    s32 offset = localvar_getInt(runtime->localvar, pos++);
    s32 len = localvar_getInt(runtime->localvar, pos++);
    s32 ret = -1;
    if (fd && bytes_arr) {
        ret = (s32) fread(bytes_arr->arr_body + offset, 1, len, fd);
    }
    if (ret == 0) {
        ret = -1;
    }
    push_int(runtime->stack, ret);

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_readbuf  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_writebuf(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    Instance *bytes_arr = localvar_getRefer(runtime->localvar, pos++);
    s32 offset = localvar_getInt(runtime->localvar, pos++);
    s32 len = localvar_getInt(runtime->localvar, pos++);
    s32 ret = -1;
    if (fd && bytes_arr) {
        ret = (s32) fwrite(bytes_arr->arr_body + offset, 1, len, fd);
        if (ret == 0) {
            ret = -1;
        }
        push_int(runtime->stack, ret);
    } else {
        push_int(runtime->stack, ret);
    }

#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_writebuf  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_seek0(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    s64 filepos = l2d.l;
    s32 ret = -1;
    if (fd) {
        ret = fseek(fd, (long) filepos, SEEK_SET);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_seek0  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_available0(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FILE *fd = (FILE *) (intptr_t) l2d.l;

    s32 cur = 0, end = 0;
    if (fd) {
        cur = ftell(fd);
        fseek(fd, (long) 0, SEEK_END);
        end = ftell(fd);
        fseek(fd, (long) cur, SEEK_SET);
    }
    push_int(runtime->stack, end - cur);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_available0  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_setLength0(Runtime *runtime, JClass *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    l2d.l = localvar_getLong(runtime->localvar, pos);
    pos += 2;
    s64 filelen = l2d.l;
    s32 ret = 0;
    if (fd) {
#ifndef __C99
        ret = ftruncate((s32) (intptr_t) fd, filelen);
#endif
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_setLength0  \n");
#endif
    return 0;
}


s32 org_mini_fs_InnerFile_flush0(Runtime *runtime, JClass *clazz) {
    Long2Double l2d;
    l2d.l = localvar_getLong(runtime->localvar, 0);
    FILE *fd = (FILE *) (intptr_t) l2d.l;
    s32 ret = -1;
    if (fd) {
        ret = fflush(fd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_flush0  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_loadFS(Runtime *runtime, JClass *clazz) {
    Instance *name_arr = localvar_getRefer(runtime->localvar, 0);
    Instance *fd = localvar_getRefer(runtime->localvar, 1);
    s32 ret = -1;
    if (name_arr) {
        Utf8String *filepath = utf8_create_part_c(name_arr->arr_body, 0, name_arr->arr_length);
        struct stat buf;
        ret = stat(utf8_cstr(filepath), &buf);
        s32 a = S_ISDIR(buf.st_mode);
        if (ret == 0) {
            c8 *className = "org/mini/fs/InnerFileStat";
            c8 *ptr;
            ptr = getFieldPtr_byName_c(fd, className, "st_dev", "I");
            setFieldInt(ptr, buf.st_dev);
            ptr = getFieldPtr_byName_c(fd, className, "st_ino", "S");
            setFieldShort(ptr, buf.st_ino);
            ptr = getFieldPtr_byName_c(fd, className, "st_mode", "S");
            setFieldShort(ptr, buf.st_mode);
            ptr = getFieldPtr_byName_c(fd, className, "st_nlink", "S");
            setFieldShort(ptr, buf.st_nlink);
            ptr = getFieldPtr_byName_c(fd, className, "st_uid", "S");
            setFieldShort(ptr, buf.st_uid);
            ptr = getFieldPtr_byName_c(fd, className, "st_gid", "S");
            setFieldShort(ptr, buf.st_gid);
            ptr = getFieldPtr_byName_c(fd, className, "st_rdev", "S");
            setFieldShort(ptr, buf.st_rdev);
            ptr = getFieldPtr_byName_c(fd, className, "st_size", "J");
            setFieldLong(ptr, buf.st_size);
            ptr = getFieldPtr_byName_c(fd, className, "st_atime", "J");
            setFieldLong(ptr, buf.st_atime);
            ptr = getFieldPtr_byName_c(fd, className, "st_mtime", "J");
            setFieldLong(ptr, buf.st_mtime);
            ptr = getFieldPtr_byName_c(fd, className, "st_ctime", "J");
            setFieldLong(ptr, buf.st_ctime);
            ptr = getFieldPtr_byName_c(fd, className, "exists", "Z");
            setFieldByte(ptr, 1);
        }
        utf8_destory(filepath);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_loadFD  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_listDir(Runtime *runtime, JClass *clazz) {
    Instance *name_arr = localvar_getRefer(runtime->localvar, 0);
    if (name_arr) {
        Utf8String *filepath = utf8_create_part_c(name_arr->arr_body, 0, name_arr->arr_length);

        ArrayList *files = arraylist_create(0);
        DIR *dirp;
        struct dirent *dp;
        dirp = opendir(utf8_cstr(filepath)); //打开目录指针
        if (dirp) {
            while ((dp = readdir(dirp)) != NULL) { //通过目录指针读目录
                Utf8String *ustr = utf8_create_c(dp->d_name);
                Instance *jstr = jstring_create(ustr, runtime);
                gc_refer_hold(jstr);
                utf8_destory(ustr);
                arraylist_push_back(files, jstr);
            }
            (void) closedir(dirp); //关闭目录

            s32 i;
            Utf8String *ustr = utf8_create_c(STR_CLASS_JAVA_LANG_STRING);
            Instance *jarr = jarray_create_by_type_name(runtime, files->length, ustr);
            utf8_destory(ustr);
            for (i = 0; i < files->length; i++) {
                __refer ref = arraylist_get_value(files, i);
                gc_refer_release(ref);
                jarray_set_field(jarr, i, (intptr_t) ref);
            }
            push_ref(runtime->stack, jarr);
        } else {
            push_ref(runtime->stack, NULL);
        }
        arraylist_destory(files);
        utf8_destory(filepath);
    } else {
        push_ref(runtime->stack, NULL);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_listDir  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_getcwd(Runtime *runtime, JClass *clazz) {
    Instance *path_arr = localvar_getRefer(runtime->localvar, 0);
    if (path_arr) {
        __refer ret = getcwd(path_arr->arr_body, path_arr->arr_length);
        push_int(runtime->stack, ret == path_arr->arr_body ? 0 : -1);
    } else {
        push_int(runtime->stack, -1);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_getcwd  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_chmod(Runtime *runtime, JClass *clazz) {
    Instance *path_arr = localvar_getRefer(runtime->localvar, 0);
    s32 mode = localvar_getInt(runtime->localvar, 1);
    if (path_arr) {
        s32 ret = chmod(path_arr->arr_body, mode);
        push_int(runtime->stack, ret);
    } else {
        push_int(runtime->stack, -1);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_fullpath  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_rename0(Runtime *runtime, JClass *clazz) {
    Instance *old_arr = localvar_getRefer(runtime->localvar, 0);
    Instance *new_arr = localvar_getRefer(runtime->localvar, 1);
    if (old_arr && new_arr) {
        s32 ret = rename(old_arr->arr_body, new_arr->arr_body);
        push_int(runtime->stack, ret);
    } else {
        push_int(runtime->stack, -1);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_rename0  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_getTmpDir(Runtime *runtime, JClass *clazz) {
    Utf8String *tdir = getTmpDir();
    if (tdir) {
        Instance *jstr = jstring_create(tdir, runtime);
        utf8_destory(tdir);
        push_ref(runtime->stack, jstr);
    } else {
        push_ref(runtime->stack, NULL);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_getTmpDir  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_mkdir0(Runtime *runtime, JClass *clazz) {
    Instance *path_arr = localvar_getRefer(runtime->localvar, 0);
    s32 ret = -1;
    if (path_arr) {
#if __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__ || __JVM_OS_VS__
        ret = mkdir(path_arr->arr_body);
#else
        ret = mkdir(path_arr->arr_body, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
        push_int(runtime->stack, ret);
    } else {
        push_int(runtime->stack, ret);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_mkdir  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_getOS(Runtime *runtime, JClass *clazz) {
#if __JVM_OS_VS__ || __JVM_OS_MINGW__ || __JVM_OS_CYGWIN__
    push_int(runtime->stack, 1);
#else
    push_int(runtime->stack, 0);
#endif
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_getOS  \n");
#endif
    return 0;
}

s32 org_mini_fs_InnerFile_delete0(Runtime *runtime, JClass *clazz) {
    Instance *path_arr = localvar_getRefer(runtime->localvar, 0);
    s32 ret = -1;
    if (path_arr) {
        struct stat buf;
        stat(path_arr->arr_body, &buf);
        s32 a = S_ISDIR(buf.st_mode);
        if (a) {
            ret = rmdir(path_arr->arr_body);
        } else {
            ret = remove(path_arr->arr_body);
        }
        push_int(runtime->stack, ret);
    } else {
        push_int(runtime->stack, ret);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("org_mini_fs_InnerFile_delete0  \n");
#endif
    return 0;
}

s32 javax_mini_zip_ZipFile_getEntry0(Runtime *runtime, JClass *clazz) {
    Instance *zip_path_arr = localvar_getRefer(runtime->localvar, 0);
    Instance *name_arr = localvar_getRefer(runtime->localvar, 1);
    s32 ret = -1;
    if (zip_path_arr && name_arr) {
        ByteBuf *buf = bytebuf_create(0);
        zip_loadfile(zip_path_arr->arr_body, name_arr->arr_body, buf);
        if (buf->wp) {
            Instance *arr = jarray_create_by_type_index(runtime, buf->wp, DATATYPE_BYTE);
            memmove(arr->arr_body, buf->buf, buf->wp);
            push_ref(runtime->stack, arr);
            ret = 0;
        }
        bytebuf_destory(buf);
    }
    if (ret) {
        push_ref(runtime->stack, NULL);
    }
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_zip_ZipFile_getEntry0  \n");
#endif
    return 0;
}

s32 javax_mini_zip_ZipFile_putEntry0(Runtime *runtime, JClass *clazz) {
    Instance *zip_path_arr = localvar_getRefer(runtime->localvar, 0);
    Instance *name_arr = localvar_getRefer(runtime->localvar, 1);
    Instance *content_arr = localvar_getRefer(runtime->localvar, 2);
    s32 ret = -1;
    if (zip_path_arr && name_arr && content_arr) {
        zip_savefile_mem(zip_path_arr->arr_body, name_arr->arr_body, content_arr->arr_body, content_arr->arr_length);
        ret = 0;
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG_BYTECODE_DETAIL > 5
    invoke_deepth(runtime);
    jvm_printf("javax_mini_zip_ZipFile_putEntry0  \n");
#endif
    return 0;
}

static java_native_method method_net_table[] = {
        {"javax/mini/net/protocol/socket/Protocol",       "open0",           "([BII)I",                          javax_mini_net_socket_Protocol_open0},
        {"javax/mini/net/protocol/socket/Protocol",       "readBuf",         "(I[BII)I",                         javax_mini_net_socket_Protocol_readBuf},
        {"javax/mini/net/protocol/socket/Protocol",       "readByte",        "(I)I",                             javax_mini_net_socket_Protocol_readByte},
        {"javax/mini/net/protocol/socket/Protocol",       "writeBuf",        "(I[BII)I",                         javax_mini_net_socket_Protocol_writeBuf},
        {"javax/mini/net/protocol/socket/Protocol",       "writeByte",       "(II)I",                            javax_mini_net_socket_Protocol_writeByte},
        {"javax/mini/net/protocol/socket/Protocol",       "available0",      "(I)I",                             javax_mini_net_socket_Protocol_available0},
        {"javax/mini/net/protocol/socket/Protocol",       "close0",          "(I)V",                             javax_mini_net_socket_Protocol_close0},
        {"javax/mini/net/protocol/socket/Protocol",       "setOption0",      "(III)I",                           javax_mini_net_socket_Protocol_setOption0},
        {"javax/mini/net/protocol/serversocket/Protocol", "accept0",         "(I)I",                             javax_mini_net_serversocket_Protocol_accept0},
        {"javax/mini/net/protocol/serversocket/Protocol", "close0",          "(I)V",                             javax_mini_net_serversocket_Protocol_close0},
        {"javax/mini/net/protocol/serversocket/Protocol", "listen0",         "(I)I",                             javax_mini_net_serversocket_Protocol_listen0},
        {"javax/mini/net/protocol/serversocket/Protocol", "open0",           "([BI)I",                           javax_mini_net_serversocket_Protocol_open0},
        {"javax/mini/net/protocol/serversocket/Protocol", "registerCleanup", "",                                 javax_mini_net_serversocket_Protocol_registerCleanup},
        {"javax/mini/net/protocol/serversocket/Protocol", "finalize",        "",                                 javax_mini_net_serversocket_Protocol_finalize},
        {"org/mini/fs/InnerFile",                         "openFile",        "([B[B)J",                          org_mini_fs_InnerFile_openFile},
        {"org/mini/fs/InnerFile",                         "closeFile",       "(J)I",                             org_mini_fs_InnerFile_closeFile},
        {"org/mini/fs/InnerFile",                         "read0",           "(J)I",                             org_mini_fs_InnerFile_read0},
        {"org/mini/fs/InnerFile",                         "write0",          "(JI)I",                            org_mini_fs_InnerFile_write0},
        {"org/mini/fs/InnerFile",                         "readbuf",         "(J[BII)I",                         org_mini_fs_InnerFile_readbuf},
        {"org/mini/fs/InnerFile",                         "writebuf",        "(J[BII)I",                         org_mini_fs_InnerFile_writebuf},
        {"org/mini/fs/InnerFile",                         "seek0",           "(JJ)I",                            org_mini_fs_InnerFile_seek0},
        {"org/mini/fs/InnerFile",                         "available0",      "(J)I",                             org_mini_fs_InnerFile_available0},
        {"org/mini/fs/InnerFile",                         "setLength0",      "(JJ)I",                            org_mini_fs_InnerFile_setLength0},
        {"org/mini/fs/InnerFile",                         "flush0",          "(J)I",                             org_mini_fs_InnerFile_flush0},
        {"org/mini/fs/InnerFile",                         "loadFS",          "([BLorg/mini/fs/InnerFileStat;)I", org_mini_fs_InnerFile_loadFS},
        {"org/mini/fs/InnerFile",                         "listDir",         "([B)[Ljava/lang/String;",          org_mini_fs_InnerFile_listDir},
        {"org/mini/fs/InnerFile",                         "getcwd",          "([B)I",                            org_mini_fs_InnerFile_getcwd},
        {"org/mini/fs/InnerFile",                         "chmod",           "([BI)I",                           org_mini_fs_InnerFile_chmod},
        {"org/mini/fs/InnerFile",                         "mkdir0",          "([B)I",                            org_mini_fs_InnerFile_mkdir0},
        {"org/mini/fs/InnerFile",                         "getOS",           "()I",                              org_mini_fs_InnerFile_getOS},
        {"org/mini/fs/InnerFile",                         "delete0",         "([B)I",                            org_mini_fs_InnerFile_delete0},
        {"org/mini/fs/InnerFile",                         "rename0",         "([B[B)I",                          org_mini_fs_InnerFile_rename0},
        {"org/mini/fs/InnerFile",                         "getTmpDir",       "()Ljava/lang/String;",             org_mini_fs_InnerFile_getTmpDir},
        {"javax/mini/zip/Zip",                            "getEntry0",       "([B[B)[B",                         javax_mini_zip_ZipFile_getEntry0},
        {"javax/mini/zip/Zip",                            "putEntry0",       "([B[B[B)I",                        javax_mini_zip_ZipFile_putEntry0},

};


void reg_net_native_lib() {
    native_reg_lib(&(method_net_table[0]), sizeof(method_net_table) / sizeof(java_native_method));
}


#ifdef __cplusplus
}
#endif
