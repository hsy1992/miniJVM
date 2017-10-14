//
// Created by gust on 2017/8/9.
//
#include "jvm.h"

#if !__JVM_OS_WIN__

#include <sys/time.h>
#include "jvm_util.h"
#include <time.h>
#include <unistd.h>
#if __JVM_OS_MINGW__
#include <pthread_time.h>
#endif //__JVM_OS_MINGW__

s64 currentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((s64) tv.tv_sec) * 1000 + tv.tv_usec / 1000;
}

s64 nanoTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    if (!NANO_START) {
        NANO_START = ((s64) tv.tv_sec) * 1000000000;
    }
    s64 v = (((s64) tv.tv_sec) * 1000000 + tv.tv_usec) * 1000;
    return v - NANO_START;
}

s32 threadSleep(s64 ms) {
    //wait time
    struct timespec req;
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000;
    //if notify or notifyall ,the thread is active again, rem record remain wait time
    struct timespec rem;
    rem.tv_sec = 0;
    rem.tv_nsec = 0;
    nanosleep(&req, &rem);
    return rem.tv_sec * 1000 + rem.tv_nsec / 1000000;
}

//s32 threadSleep(s64 ms) {
//    struct timeval delay;
//    delay.tv_sec = ms / 1000;
//    delay.tv_usec = (ms % 1000) * 1000; //  ms
//    select(0, NULL, NULL, NULL, &delay);
//    return 0;
//}

#endif