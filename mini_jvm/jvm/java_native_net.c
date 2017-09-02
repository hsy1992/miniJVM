//
// Created by gust on 2017/9/1.
//

#include "jvm.h"
#include "java_native.h"

#ifndef USE_BSD_SOCKET
#define USE_BSD_SOCKET 1
#endif
#define   err printf

#ifdef __cplusplus
extern "C" {
#endif


#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

#include <errno.h>
#include <signal.h>

#ifdef __WIN32__
#include <winsock2.h>
#include <fcntl.h>
#define SHUT_RDWR SD_BOTH
#define SHUT_RD SD_RECEIVE
#define SHUT_WR SD_SEND
#else

#include <sys/types.h>
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

void _on_sock_sig(s32 signo) {
}

s32 javax_mini_eio_socket_Protocol_open0(Runtime *runtime, Class *clazz) {
    StackFrame *stack = runtime->stack;
    Instance *jbyte_arr = (Instance *) (runtime->localVariables + 0)->refer;
    s32 port = (runtime->localVariables + 1)->integer;
    s32 mode = (runtime->localVariables + 2)->integer;

    s32 sockfd;
    struct sockaddr_in inet_addr; /* connector's address information */

#ifdef WIN32
    WSADATA wsaData;
        WSAStartup(MAKEWORD(1, 1), &wsaData);
#endif  /*  WIN32  */
#ifdef  __ANDROID__
    //LOGE("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n");
#endif
    Utf8String *ip = utf8_create_part_c(jbyte_arr->arr_body, 0, jbyte_arr->arr_length);
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
#if _JVM_DEBUG
    printf("javax_mini_eio_socket_Protocol_open0  \n");
#endif
    return 0;
}

s32 javax_mini_eio_socket_Protocol_readBuf(Runtime *runtime, Class *clazz) {
    s32 sockfd = (runtime->localVariables + 0)->integer;
    Instance *jbyte_arr = (Instance *) (runtime->localVariables + 1)->refer;
    s32 offset = (runtime->localVariables + 2)->integer;
    s32 buf_len = (runtime->localVariables + 3)->integer;

    s32 len = recv(sockfd, jbyte_arr->arr_body + offset, buf_len, 0);

    if (len == 0) {//如果是正常断开，返回-1
        len = -1;
    } else if (len == -1) {//如果发生错误
#ifdef __WIN32__
        if (WSAEWOULDBLOCK == WSAGetLastError()) {//但是如果是非阻塞端口，说明连接仍正常
                //printf("sc send error client time = %f ;\n", (f64)clock());
                len= 0;
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
    push_int(runtime->stack, len);
#if _JVM_DEBUG
    printf("javax_mini_eio_socket_Protocol_readBuf  \n");
#endif
    return 0;
}

s32 javax_mini_eio_socket_Protocol_readByte(Runtime *runtime, Class *clazz) {
//    Instance *tmps = (Instance *) (runtime->localVariables + 0)->refer;
#if _JVM_DEBUG
    printf("javax_mini_eio_socket_Protocol_readByte  \n");
#endif
    return 0;
}

s32 javax_mini_eio_socket_Protocol_writeBuf(Runtime *runtime, Class *clazz) {
    StackFrame *stack = runtime->stack;
    s32 sockfd = (runtime->localVariables + 0)->integer;
    Instance *jbyte_arr = (Instance *) (runtime->localVariables + 1)->refer;
    s32 offset = (runtime->localVariables + 2)->integer;
    s32 buf_len = (runtime->localVariables + 3)->integer;

    s32 len = send(sockfd, jbyte_arr->arr_body + offset, buf_len, 0);

    if (len == 0) {//如果是正常断开，返回-1
        len = -1;
    } else if (len == -1) {//如果发生错误
#ifdef __WIN32__
        if (WSAEWOULDBLOCK == WSAGetLastError()) {//但是如果是非阻塞端口，说明连接仍正常
                //printf("sc send error server time = %f ;\n", (f64)clock());
                len= 0;
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
    push_int(runtime->stack, len);
#if _JVM_DEBUG
    printf("javax_mini_eio_socket_Protocol_writeBuf  \n");
#endif
    return 0;
}

s32 javax_mini_eio_socket_Protocol_writeByte(Runtime *runtime, Class *clazz) {
    StackFrame *stack = runtime->stack;
//    Instance *tmps = (Instance *) (runtime->localVariables + 0)->refer;
#if _JVM_DEBUG
    printf("javax_mini_eio_socket_Protocol_writeByte  \n");
#endif
    return 0;
}

s32 javax_mini_eio_socket_Protocol_available0(Runtime *runtime, Class *clazz) {
    StackFrame *stack = runtime->stack;
//    Instance *tmps = (Instance *) (runtime->localVariables + 0)->refer;
#if _JVM_DEBUG
    printf("javax_mini_eio_socket_Protocol_available0  \n");
#endif
    return 0;
}

s32 javax_mini_eio_socket_Protocol_close0(Runtime *runtime, Class *clazz) {
    StackFrame *stack = runtime->stack;
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
    printf("javax_mini_eio_socket_Protocol_close0  \n");
#endif
    return 0;
}

static java_native_method method_net_table[] = {
        {"javax/mini/eio/socket/Protocol", "open0",      javax_mini_eio_socket_Protocol_open0},
        {"javax/mini/eio/socket/Protocol", "readBuf",    javax_mini_eio_socket_Protocol_readBuf},
        {"javax/mini/eio/socket/Protocol", "readByte",   javax_mini_eio_socket_Protocol_readByte},
        {"javax/mini/eio/socket/Protocol", "writeBuf",   javax_mini_eio_socket_Protocol_writeBuf},
        {"javax/mini/eio/socket/Protocol", "writeByte",  javax_mini_eio_socket_Protocol_writeByte},
        {"javax/mini/eio/socket/Protocol", "available0", javax_mini_eio_socket_Protocol_available0},
        {"javax/mini/eio/socket/Protocol", "close0",     javax_mini_eio_socket_Protocol_close0},

};


void reg_net_native_lib() {
    native_reg_lib(&(method_net_table[0]), sizeof(method_net_table) / sizeof(java_native_method));
}


#ifdef __cplusplus
}
#endif