

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jvm.h"
#include "garbage.h"


/* parse Interface Pool Class */
static s32 parseIPClass(Class *_this, FILE *fp, s32 index) {
    u8 short_tmp[2];
    ConstantClassRef *ptr = &(_this->interfacePool).clasz[_this->interfacePool.clasz_used];

    ptr->tag = CONSTANT_CLASS;
    ptr->index = index;
    ptr->additional_byte_size = 2;

    fread(short_tmp, 2, 1, fp);
    Short2Char s2c;
    s2c.c1 = short_tmp[0];
    s2c.c0 = short_tmp[1];
    ptr->stringIndex = s2c.s;

    _this->interfacePool.clasz_used++;
}

void printInterfacePool(Class *clazz, InterfacePool *ip) {
    s32 i, j;

    if (ip->clasz_used > 0) {
        printf("Interface Class Pool= \n");
        for (i = 0; i < ip->clasz_used; i++) {
            ConstantUTF8 *ptr = find_constant_utf8(clazz, ip->clasz[i].stringIndex);
            printf(" ip_index[%d], class[%d], tag = %d, size = %d, %d",
                   ip->clasz[i].index, i, ip->clasz[i].tag,
                   ip->clasz[i].additional_byte_size,
                   ip->clasz[i].stringIndex);
            if (ptr != 0) {
                printf(" ");
                printf(" %s\n",utf8_cstr(ptr->ptr));
            } else {
                printf("\n");
            }
        }
    }
}

s32 _parse_interface_pool(Class *_this, FILE *fp, s32 count) {

    s32 size = sizeof(ConstantInterfaceMethodRef) * count;
    _this->interfacePool.clasz = jvm_alloc(size);
    s32 i;
    for (i = 0; i < count; i++)
        parseIPClass(_this, fp, 1);
    return 0;
}
