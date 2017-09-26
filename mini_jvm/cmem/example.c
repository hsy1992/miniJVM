#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include "memleak.h"
#include "../utils/utf8_string.h"

int main1() {
    dbg_init(10);
    dbg_catch_sigsegv();

    char *cs = "This is a test";
    Utf8String *utf = utf8_create();
    utf8_append_c(utf, cs);
    utf8_equals_c(utf, "here you are.");
    utf8_replace_c(utf, "is", "was");
    if (utf8_last_indexof_c(utf, "/") != utf->length - 1)utf8_append_c(utf, "/");

    utf8_destory(utf);

    dbg_mem_stat();
    dbg_heap_dump("");
    return 0;
}
