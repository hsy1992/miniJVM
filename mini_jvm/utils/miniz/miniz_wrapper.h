#ifndef __TINY_JVM_MINIZ_WRAPPER_H__
#define __TINY_JVM_MINIZ_WRAPPER_H__

#include <bytebuf.h>

s32 zip_loadfile(char *jarpath, char *filename, ByteBuf *buf);

#endif