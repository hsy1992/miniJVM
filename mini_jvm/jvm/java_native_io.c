//
// Created by gust on 2017/9/1.
//

#include "jvm.h"
#include "java_native_std.h"
#include "garbage.h"
#include <sys/stat.h>

#ifndef _CYGWIN_CONFIG_H
#ifndef __WIN32__
#define __WIN32__
#endif
#endif
#define   err printf

#ifdef __cplusplus
extern "C" {
#endif


#include <errno.h>
#include <signal.h>

#ifdef __WIN32__

#include <winsock2.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#define SHUT_RDWR SD_BOTH
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#else
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#define closesocket close
#endif

#ifdef __IPHONE_NA
#include <netdb.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <unistd.h>
#endif
#ifdef __ANDROID__
#include <netdb.h>
#include <linux/in.h>
#include <sys/endian.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>

//#include <android/log.h>
//
//#define  LOG_TAG    "gstlout"
//#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif
#define  SOCK_OP_TYPE_NON_BLOCK   0
#define  SOCK_OP_TYPE_REUSEADDR   1
#define  SOCK_OP_TYPE_RCVBUF   2
#define  SOCK_OP_TYPE_SNDBUF   3

#define  SOCK_OP_VAL_NON_BLOCK   1
#define  SOCK_OP_VAL_BLOCK   0
#define  SOCK_OP_VAL_NON_REUSEADDR   1
#define  SOCK_OP_VAL_REUSEADDR   0

void _on_sock_sig(s32 signo) {
}

//=================================  socket  ====================================
s32 setOption(s32 sockfd, s32 opType, s32 opValue) {
    switch (opType) {
        case SOCK_OP_TYPE_NON_BLOCK: {//阻塞设置
#ifdef __WIN32__
            unsigned long ul = 1;
            if (!opValue) {
                ul = 0;
            }
            s32 ret = ioctlsocket(sockfd, FIONBIO, (unsigned long *) &ul);
            if (ret == SOCKET_ERROR)err("set socket non_block error.");
#else
            if (opValue) {
                fcntl(sockfd, F_SETFL, O_NONBLOCK);
            } else {
                //fcntl(sockfd, F_SETFL, O_BLOCK);
            }
#endif
            break;
        }
        case SOCK_OP_TYPE_REUSEADDR: {//阻塞设置
            s32 x = 1;
            s32 ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &x, sizeof(x));
#ifndef __WIN32__
#define SOCKET_ERROR -1
#endif
            return -2;

        }
        case SOCK_OP_TYPE_RCVBUF: {//缓冲区设置
            int nRecvBuf = opValue;//设置为 opValue K
            s32 ret = setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (const char *) &nRecvBuf, sizeof(int));
#ifndef __WIN32__
#define SOCKET_ERROR -1
#endif
            return -3;
        }
        case SOCK_OP_TYPE_SNDBUF: {//缓冲区设置
            s32 nRecvBuf = opValue;//设置为 opValue K
            s32 ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (const char *) &nRecvBuf, sizeof(int));
#ifndef __WIN32__
#define SOCKET_ERROR -1
#endif
            return -4;
        }
    }
    return 0;
}

s32 sock_recv(s32 sockfd, c8 *buf, s32 count) {

    s32 len = recv(sockfd, buf, count, 0);

    if (len == 0) {//如果是正常断开，返回-1
        len = -1;
    } else if (len == -1) {//如果发生错误
#ifdef __WIN32__
        if (WSAEWOULDBLOCK == WSAGetLastError()) {//但是如果是非阻塞端口，说明连接仍正常
            //printf("sc send error client time = %f ;\n", (f64)clock());
            len = 0;
        }
#else
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            len = 0;
        }
#endif
#ifdef  __ANDROID__
        //            LOGE("recv error no: %x\n", errno);
//            __android_log_write(ANDROID_LOG_ERROR, "socket errno", strerror(errno));
#endif
        len = -1;
    }
    return len;
}


s32 sock_send(s32 sockfd, c8 *buf, s32 count) {
    s32 len = send(sockfd, buf, count, 0);

    if (len == 0) {//如果是正常断开，返回-1
        len = -1;
    } else if (len == -1) {//如果发生错误
#ifdef __WIN32__
        if (WSAEWOULDBLOCK == WSAGetLastError()) {//但是如果是非阻塞端口，说明连接仍正常
            //printf("sc send error server time = %f ;\n", (f64)clock());
            len = 0;
        }
#else
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
            len = 0;
        }
#endif
#ifdef  __ANDROID__
        //            LOGE("send error no: %x\n", errno);
//            __android_log_write(ANDROID_LOG_ERROR, "socket errno", strerror(errno));
#endif
        len = -1;
    }
    return len;
}

s32 sock_open(Utf8String *ip, s32 port) {
    s32 sockfd;
    struct sockaddr_in inet_addr; /* connector's address information */

#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1, 1), &wsaData);
#endif  /*  WIN32  */
#ifdef  __ANDROID__
    //LOGE("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
#endif

    struct hostent *host;
    if ((host = gethostbyname(utf8_cstr(ip))) == NULL) { /* get the host info */
        err("get host by name error");
        //exit(1);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        err(strerror(errno));
        err("socket init error");

        //exit(1);
    }
    s32 x = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *) &x, sizeof(x)) == -1) {
        err("socket reuseaddr error");
    }

    memset((char *) &inet_addr, 0, sizeof(inet_addr));
    inet_addr.sin_family = AF_INET; /* host byte order */
    inet_addr.sin_port = htons((u16) port); /* short, network byte order */

#ifdef  __ANDROID__
    //LOGE("inet_addr.sin_port     : %x\n",inet_addr.sin_port);
        //LOGE("htons((u16)port)     : %x\n",htons((u16)10086));
        //u32 hosttmp = (((u32)221) << 24) | (((u32)179) << 16) | (((u32)222) << 8) | ((u32)66);
        //LOGE("hosttmp     : %x\n",hosttmp);
        //inet_addr.sin_addr.s_addr =  htonl(0x0a000202ul);//
        //inet_addr.sin_addr.s_addr =  htonl(0x7f000001ul);//
        inet_addr.sin_addr.s_addr = *((u32 *) (host->h_addr)); //htonl(hosttmp); //
        //LOGE("host->h_addr: %x\n",htonl(*((u32 *)(host->h_addr))));
        //LOGE("htonl(0x0a000202ul): %x\n",htonl(0x0a000202ul));
        //LOGE("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
#else
    inet_addr.sin_addr = *((struct in_addr *) host->h_addr);
    memset(&(inet_addr.sin_zero), 0, sizeof((inet_addr.sin_zero))); /* zero the rest of the struct */
#endif
    if (connect(sockfd, (struct sockaddr *) &inet_addr, sizeof(inet_addr)) == -1) {
#ifdef  __ANDROID__
        //            LOGE("connect error no: %x\n", errno);
//            __android_log_write(ANDROID_LOG_ERROR, "socket errno", strerror(errno));
#endif
        err("socket connect error");
        //exit(1);
    }
#ifndef WIN32
    signal(SIGPIPE, _on_sock_sig);
#endif
    return sockfd;
}



//=================================  serversocket  ====================================

s32 srv_bind(Utf8String *ip, u16 port) {

    struct sockaddr_in server_addr;
    s32 listenfd;
#ifdef WIN32
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(1, 1), &wsadata) == SOCKET_ERROR) {
        err("Error creating serversocket.");
    }
#endif

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        err("Error opening serversocket.");
    }

    struct hostent *host;

    memset((char *) &server_addr, 0, sizeof(server_addr));//清0
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (ip->length != 0) {//如果指定了ip
        if ((host = gethostbyname(utf8_cstr(ip))) == NULL) { /* get the host info */
            err("get host by name error");
//exit(1);
        }
#ifdef __WIN32__
        server_addr.sin_addr = *((struct in_addr *) host->h_addr);
#else
        server_addr.sin_addr.s_addr = htonl(*((u32 *) (host->h_addr))); //htonl(hosttmp); //
#endif
    } else {
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }

    s32 on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));
    if ((bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr))) < 0) {
        err("Error binding serversocket.");
    }
    return listenfd;
}


s32 srv_listen(s32 listenfd) {
    u16 MAX_LISTEN = 64;
    if ((listen(listenfd, MAX_LISTEN)) < 0) {
        err("Error listening on serversocket.");
    }
}

s32 srv_accept(s32 listenfd) {
    struct sockaddr_in clt_addr;
    memset(&clt_addr, 0, sizeof(clt_addr)); //清0
    s32 clt_addr_length = sizeof(clt_addr);
    s32 clt_socket_fd = accept(listenfd, (struct sockaddr *) &clt_addr, (s32 *) &clt_addr_length);
    if (clt_socket_fd == -1) {
        if (errno != EINTR) {
            err("Error accepting on serversocket.");
        }
    }

    return clt_socket_fd;
}

s32 srv_close(s32 listenfd) {
    if (listenfd) {
        closesocket(listenfd);
        listenfd = 0;
        //#ifdef WIN32
        //            WSACancelBlockingCall();
        //            WSACleanup();
        //#endif
    }

}


//=================================  native  ====================================



s32 javax_mini_net_socket_Protocol_open0(Runtime *runtime, Class *clazz) {
    StackFrame *stack = runtime->stack;
    Instance *jbyte_arr = (Instance *) (runtime->localVariables + 0)->refer;
    s32 port = (runtime->localVariables + 1)->integer;
    s32 mode = (runtime->localVariables + 2)->integer;
    Utf8String *ip = utf8_create_part_c(jbyte_arr->arr_body, 0, jbyte_arr->arr_length);

    s32 sockfd = sock_open(ip, port);
#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_open0  \n");
#endif
    push_int(stack, sockfd);
    return 0;
}

s32 javax_mini_net_socket_Protocol_readBuf(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    Instance *jbyte_arr = (Instance *) (runtime->localVariables + 1)->refer;
    s32 offset = (runtime->localVariables + 2)->integer;
    s32 count = (runtime->localVariables + 3)->integer;

    s32 len = sock_recv(sockfd, jbyte_arr->arr_body + offset, count);
    push_int(runtime->stack, len);
#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_readBuf  \n");
#endif
    return 0;
}

s32 javax_mini_net_socket_Protocol_readByte(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    u8 b = 0;
    s32 len = sock_recv(sockfd, &b, 1);
    if (len < 0) {
        push_int(runtime->stack, -1);
    } else {
        push_int(runtime->stack, b);

    }

#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_readByte  \n");
#endif
    return 0;
}


s32 javax_mini_net_socket_Protocol_writeBuf(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    Instance *jbyte_arr = (Instance *) (runtime->localVariables + 1)->refer;
    s32 offset = (runtime->localVariables + 2)->integer;
    s32 count = (runtime->localVariables + 3)->integer;

    s32 len = sock_send(sockfd, jbyte_arr->arr_body + offset, count);

    push_int(runtime->stack, len);
#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_writeBuf  \n");
#endif
    return 0;
}

s32 javax_mini_net_socket_Protocol_writeByte(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    s32 val = (runtime->localVariables + 1)->integer;
    u8 b = (u8) val;
    s32 len = sock_send(sockfd, &b, 1);
#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_writeByte  \n");
#endif
    push_int(runtime->stack, len);
    return 0;
}

s32 javax_mini_net_socket_Protocol_available0(Runtime *runtime, Class *clazz) {
//    Instance *tmps = (Instance *) (runtime->localVariables + 0)->refer;
#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_available0  \n");
#endif
    push_int(runtime->stack, 0);
    return 0;
}

s32 javax_mini_net_socket_Protocol_close0(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    if (sockfd) {
        shutdown(sockfd, SHUT_RDWR);
        closesocket(sockfd);
#ifdef __WIN32__
        //            WSACleanup();
#endif
        sockfd = 0;
    }
#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_close0  \n");
#endif
    return 0;
}

s32 javax_mini_net_socket_Protocol_setOption0(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    s32 type = (runtime->localVariables + 1)->integer;
    s32 val = (runtime->localVariables + 2)->integer;
    s32 ret = 0;
    if (sockfd) {
        ret = setOption(sockfd, type, val);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_net_socket_Protocol_setOption0  \n");
#endif
    return 0;
}


s32 javax_mini_net_serversocket_Protocol_open0(Runtime *runtime, Class *clazz) {
    Instance *jbyte_arr = (runtime->localVariables + 0)->refer;
    s32 port = (runtime->localVariables + 1)->integer;
    Utf8String *ip = utf8_create_part_c(jbyte_arr->arr_body, 0, jbyte_arr->arr_length);
    s32 ret = 0;
    ret = srv_bind(ip, port);
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_net_serversocket_Protocol_open0  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_listen0(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    s32 ret = 0;
    if (sockfd) {
        ret = srv_listen(sockfd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_net_serversocket_Protocol_listen0  \n");
#endif
    return 0;
}


s32 javax_mini_net_serversocket_Protocol_accept0(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    s32 ret = 0;
    if (sockfd) {
        ret = srv_accept(sockfd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_net_serversocket_Protocol_accept0  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_close0(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    s32 ret = 0;
    if (sockfd) {
        ret = srv_close(sockfd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_net_serversocket_Protocol_close0  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_registerCleanup(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    s32 ret = 0;
    if (sockfd) {

    }

#if _JVM_DEBUG
    printf("javax_mini_net_serversocket_Protocol_registerCleanup  \n");
#endif
    return 0;
}

s32 javax_mini_net_serversocket_Protocol_finalize(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    s32 ret = 0;
    if (sockfd) {

    }

#if _JVM_DEBUG
    printf("javax_mini_net_serversocket_Protocol_finalize  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_openFile(Runtime *runtime, Class *clazz) {
    Instance *name_arr = (runtime->localVariables + 0)->refer;
    Instance *mode_arr = (runtime->localVariables + 1)->refer;
    if (name_arr) {
        Utf8String *filepath = utf8_create_part_c(name_arr->arr_body, 0, name_arr->arr_length);
        Utf8String *mode = utf8_create_part_c(mode_arr->arr_body, 0, mode_arr->arr_length);
        FILE *fd = fopen(utf8_cstr(filepath), utf8_cstr(mode));
        push_long(runtime->stack, (s64) (long) fd);
        utf8_destory(filepath);
        utf8_destory(mode);
    } else {
        push_long(runtime->stack, 0);
    }

#if _JVM_DEBUG
    printf("javax_mini_io_File_openFile  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_closeFile(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer fd = l2d.r;
    s32 ret = -1;
    if (fd) {
        ret = fclose(fd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_io_File_closeFile  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_read0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer fd = l2d.r;
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

#if _JVM_DEBUG
    printf("javax_mini_io_File_read0  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_write0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer fd = l2d.r;
    u8 byte = (u8) (runtime->localVariables + 2)->integer;
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

#if _JVM_DEBUG
    printf("javax_mini_io_File_write0  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_readbuf(Runtime *runtime, Class *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    __refer fd = l2d.r;
    Instance *bytes_arr = (runtime->localVariables + pos++)->refer;
    s32 offset = (runtime->localVariables + pos++)->integer;
    s32 len = (runtime->localVariables + pos++)->integer;
    s32 ret = -1;
    if (fd && bytes_arr) {
        ret = fread(bytes_arr->arr_body + offset, len, 1, fd);
        if (ret != len) {
            push_int(runtime->stack, -1);
        } else {
            push_int(runtime->stack, ret);
        }
    } else {
        push_int(runtime->stack, ret);
    }

#if _JVM_DEBUG
    printf("javax_mini_io_File_readbuf  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_writebuf(Runtime *runtime, Class *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    __refer fd = l2d.r;
    Instance *bytes_arr = (runtime->localVariables + pos++)->refer;
    s32 offset = (runtime->localVariables + pos++)->integer;
    s32 len = (runtime->localVariables + pos++)->integer;
    s32 ret = -1;
    if (fd && bytes_arr) {
        ret = fwrite(bytes_arr->arr_body + offset, len, 1, fd);
        if (ret != len) {
            push_int(runtime->stack, -1);
        } else {
            push_int(runtime->stack, ret);
        }
    } else {
        push_int(runtime->stack, ret);
    }

#if _JVM_DEBUG
    printf("javax_mini_io_File_writebuf  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_seek0(Runtime *runtime, Class *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    __refer fd = l2d.r;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    s64 filepos = l2d.l;
    s32 ret = -1;
    if (fd) {
        ret = fseek(fd, (long) filepos, SEEK_SET);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_io_File_seek0  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_setLength0(Runtime *runtime, Class *clazz) {
    s32 pos = 0;
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    __refer fd = l2d.r;
    l2d.i2l.i1 = (runtime->localVariables + pos++)->integer;
    l2d.i2l.i0 = (runtime->localVariables + pos++)->integer;
    s64 filelen = l2d.l;
    s32 ret = 0;
    if (fd) {
        ret = ftruncate((s32) (long) fd, filelen);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_io_File_setLength0  \n");
#endif
    return 0;
}


s32 javax_mini_io_File_flush0(Runtime *runtime, Class *clazz) {
    Long2Double l2d;
    l2d.i2l.i1 = (runtime->localVariables + 0)->integer;
    l2d.i2l.i0 = (runtime->localVariables + 1)->integer;
    __refer fd = l2d.r;
    s32 ret = -1;
    if (fd) {
        ret = fflush(fd);
    }
    push_int(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_io_File_flush0  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_loadFD(Runtime *runtime, Class *clazz) {
    Instance *name_arr = (runtime->localVariables + 0)->refer;
    Instance *fd = (runtime->localVariables + 1)->refer;
    s32 ret = -1;
    if (name_arr) {
        Utf8String *filepath = utf8_create_part_c(name_arr->arr_body, 0, name_arr->arr_length);
        struct stat buf;
        ret = stat(utf8_cstr(filepath), &buf);
        s32 a=S_ISDIR(buf.st_mode);
        if (ret == 0) {
            c8 *className = "javax/mini/io/FileDescriptor";
            u8 *ptr;
            ptr = getFieldPtr_byName(fd, className, "st_dev", "I");
            setFieldInt(ptr, buf.st_dev);
            ptr = getFieldPtr_byName(fd, className, "st_ino", "S");
            setFieldShort(ptr, buf.st_ino);
            ptr = getFieldPtr_byName(fd, className, "st_mode", "S");
            setFieldShort(ptr, buf.st_mode);
            ptr = getFieldPtr_byName(fd, className, "st_nlink", "S");
            setFieldShort(ptr, buf.st_nlink);
            ptr = getFieldPtr_byName(fd, className, "st_uid", "S");
            setFieldShort(ptr, buf.st_uid);
            ptr = getFieldPtr_byName(fd, className, "st_gid", "S");
            setFieldShort(ptr, buf.st_gid);
            ptr = getFieldPtr_byName(fd, className, "st_rdev", "S");
            setFieldShort(ptr, buf.st_rdev);
            ptr = getFieldPtr_byName(fd, className, "st_size", "J");
            setFieldLong(ptr, buf.st_size);
            ptr = getFieldPtr_byName(fd, className, "st_atime", "J");
            setFieldLong(ptr, buf.st_atime);
            ptr = getFieldPtr_byName(fd, className, "st_mtime", "J");
            setFieldLong(ptr, buf.st_mtime);
            ptr = getFieldPtr_byName(fd, className, "st_ctime", "J");
            setFieldLong(ptr, buf.st_ctime);
        }
        utf8_destory(filepath);
    }
    push_long(runtime->stack, ret);
#if _JVM_DEBUG
    printf("javax_mini_io_File_loadFD  \n");
#endif
    return 0;
}

s32 javax_mini_io_File_listDir(Runtime *runtime, Class *clazz) {
    Instance *name_arr = (runtime->localVariables + 0)->refer;
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
                utf8_destory(ustr);
                arraylist_append(files, jstr);
            }
            (void) closedir(dirp); //关闭目录

            s32 i;
            Long2Double l2d;
            Instance *jarr = jarray_create(files->length, DATATYPE_REFERENCE);
            garbage_refer(jarr, NULL);
            for (i = 0; i < files->length; i++) {
                l2d.r = arraylist_get_value(files, i);
                jarray_set_field(jarr, i, &l2d, data_type_bytes[DATATYPE_REFERENCE]);
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
#if _JVM_DEBUG
    printf("javax_mini_io_File_listDir  \n");
#endif
    return 0;
}

static java_native_method method_net_table[] = {
        {"javax/mini/net/protocol/socket/Protocol",       "open0",           javax_mini_net_socket_Protocol_open0},
        {"javax/mini/net/protocol/socket/Protocol",       "readBuf",         javax_mini_net_socket_Protocol_readBuf},
        {"javax/mini/net/protocol/socket/Protocol",       "readByte",        javax_mini_net_socket_Protocol_readByte},
        {"javax/mini/net/protocol/socket/Protocol",       "writeBuf",        javax_mini_net_socket_Protocol_writeBuf},
        {"javax/mini/net/protocol/socket/Protocol",       "writeByte",       javax_mini_net_socket_Protocol_writeByte},
        {"javax/mini/net/protocol/socket/Protocol",       "available0",      javax_mini_net_socket_Protocol_available0},
        {"javax/mini/net/protocol/socket/Protocol",       "close0",          javax_mini_net_socket_Protocol_close0},
        {"javax/mini/net/protocol/socket/Protocol",       "setOption0",      javax_mini_net_socket_Protocol_setOption0},
        {"javax/mini/net/protocol/serversocket/Protocol", "accept0",         javax_mini_net_serversocket_Protocol_accept0},
        {"javax/mini/net/protocol/serversocket/Protocol", "close0",          javax_mini_net_serversocket_Protocol_close0},
        {"javax/mini/net/protocol/serversocket/Protocol", "listen0",         javax_mini_net_serversocket_Protocol_listen0},
        {"javax/mini/net/protocol/serversocket/Protocol", "open0",           javax_mini_net_serversocket_Protocol_open0},
        {"javax/mini/net/protocol/serversocket/Protocol", "registerCleanup", javax_mini_net_serversocket_Protocol_registerCleanup},
        {"javax/mini/net/protocol/serversocket/Protocol", "finalize",        javax_mini_net_serversocket_Protocol_finalize},
        {"javax/mini/io/File",                            "openFile",        javax_mini_io_File_openFile},
        {"javax/mini/io/File",                            "closeFile",       javax_mini_io_File_closeFile},
        {"javax/mini/io/File",                            "read0",           javax_mini_io_File_read0},
        {"javax/mini/io/File",                            "write0",          javax_mini_io_File_write0},
        {"javax/mini/io/File",                            "readbuf",         javax_mini_io_File_readbuf},
        {"javax/mini/io/File",                            "writebuf",        javax_mini_io_File_writebuf},
        {"javax/mini/io/File",                            "seek0",           javax_mini_io_File_seek0},
        {"javax/mini/io/File",                            "setLength0",      javax_mini_io_File_setLength0},
        {"javax/mini/io/File",                            "flush0",          javax_mini_io_File_flush0},
        {"javax/mini/io/File",                            "loadFD",          javax_mini_io_File_loadFD},
        {"javax/mini/io/File",                            "listDir",         javax_mini_io_File_listDir},

};


void reg_net_native_lib() {
    native_reg_lib(&(method_net_table[0]), sizeof(method_net_table) / sizeof(java_native_method));
}


#ifdef __cplusplus
}
#endif