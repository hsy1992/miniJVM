#ifndef __TINY_JVM_MINIZ_WRAPPER_H__
#define __TINY_JVM_MINIZ_WRAPPER_H__

#include "bytebuf.h"

s32 zip_loadfile(char *jarpath, char *filename, ByteBuf *buf);

s32 zip_savefile(char *jarpath, char *filename, ByteBuf *buf);

s32 zip_savefile_mem(char *jarpath, char *filename, char *buf, int size);

#endif
