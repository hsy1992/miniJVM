
#include <stdlib.h>
#include <string.h>
#include "utf8_string.h"
#include "../jvm/garbage.h"

Utf8String *utf8_create() {

    int length = 16;
    Utf8String *uni_str;
    /* If the arr_length is not specified, use a sensible default */

    if (length <= 0) {
        length = 16;
    }
    /* Allocate the new uni_string and fill in the obj_fields.  There are
     * initially no entries. */

    uni_str = (Utf8String *) jvm_alloc(sizeof(Utf8String));

    if (uni_str == NULL) {
        return NULL;
    }

    uni_str->_alloced = length;
    uni_str->length = 0;
    uni_str->hash = 0;


    /* Allocate the data array */

    uni_str->data1 = jvm_alloc(length * sizeof(utf8_char));

    if (uni_str->data1 == NULL) {
        jvm_free(uni_str);
        return NULL;
    }
    return uni_str;
}

void utf8_destory(Utf8String *uni_str) {
    if (uni_str != NULL) {
        jvm_free(uni_str->data1);
        jvm_free(uni_str);
    }
}


Utf8String *utf8_create_c(char *str) {
    Utf8String *uni_str = utf8_create();
    utf8_append_c(uni_str, str);
    return uni_str;
}

Utf8String *utf8_create_copy(Utf8String *str) {
    Utf8String *uni_str = utf8_create();
    utf8_append_part(uni_str, str, 0, str->length);
    return uni_str;
}

Utf8String *utf8_create_part(Utf8String *str, int start, int len) {
    Utf8String *uni_str = utf8_create();
    utf8_append_part(uni_str, str, start, len);
    return uni_str;
}

Utf8String *utf8_create_part_c(char *str, int start, int len) {
    Utf8String *uni_str = utf8_create();
    int i;
    for (i = start; i < len; i++) {
        utf8_pushback(uni_str, str[i]);
    }
    return uni_str;
}


void utf8_clear(Utf8String *ustr) {
    /* To clear the list, simply set the arr_length to zero */

    ustr->length = 0;
    ustr->hash = 0;
}

void utf8_append(Utf8String *a1, Utf8String *a2) {
    if (a2 == NULL)return;
    utf8_append_part(a1, a2, 0, a2->length);
}

void utf8_append_part(Utf8String *a1, Utf8String *a2, int start, int len) {
    if (a1 == NULL || a2 == NULL)return;
    int i = 0;
    for (i = start; (i < a2->length) && (i < start + len); i++) {
        utf8_pushback(a1, a2->data1[i]);
    }
}

void utf8_append_c(Utf8String *a1, char *a2) {
    int i = 0;
    for (i = 0;; i++) {
        char ch = a2[i];
        if (ch == 0)break;
        utf8_pushback(a1, ch);
    }
}

void utf8_append_part_c(Utf8String *a1, char *a2, int start, int len) {
    int i = 0;
    for (i = start; i < len; i++) {
        char ch = a2[i];
        if (ch == 0)break;
        utf8_pushback(a1, ch);
    }
}


void utf8_substring(Utf8String *a1, int start, int end) {
    //printf("utf8_substring start=%d,end=%d\n", start, end);
    utf8_remove_range(a1, end, a1->length - end);
    utf8_remove_range(a1, 0, start);
}

char *utf8_cstr(Utf8String *a1) {
    _utf8_space_require(a1, 1);
    a1->data1[a1->length] = 0;
    return (char *) &(a1->data1[0]);
}

int utf8_indexof(Utf8String *a1, Utf8String *a2) {
    return utf8_indexof_pos(a1, a2, 0);
}

int utf8_indexof_pos(Utf8String *a1, Utf8String *a2, int a1_pos) {
    if (a1 == NULL || a2 == NULL)return -1; //无法查找
    if (a1_pos >= a1->length || a1_pos < 0)return -1;
    if (a2->length == 0)return 0; //
    int i = 0;
    for (i = a1_pos; i < a1->length; i++) {
        utf8_char ch = a1->data1[i];
        if (ch == a2->data1[0]) {
            int match = 1; //标识
            int j = 0;
            for (j = 1; j < a2->length; j++) {
                if (i + j >= a1->length)return -1; //超界

                if (a2->data1[j] != a1->data1[i + j]) {//不匹配
                    match = 0;
                    break;
                }
            }
            if (match)return i;
        }
    }
    return -1;
}

int utf8_last_indexof(Utf8String *a1, Utf8String *a2) {
    if (a1 == NULL)return -1;
    return utf8_last_indexof_pos(a1, a2, a1->length - 1);
}

int utf8_last_indexof_pos(Utf8String *a1, Utf8String *a2, int a1_rightpos) {
    if (a1 == NULL || a2 == NULL)return -1; //无法查找
    if (a1_rightpos >= a1->length || a1_rightpos < 0)return -1;
    if (a2->length == 0)return a1->length; //
    int i = 0;
    for (i = a1_rightpos; i > -1; i--) {
        utf8_char ch = a1->data1[i];
        if (ch == a2->data1[0]) {
            int match = 1; //标识
            int j = 1;
            for (j = 1; j < a2->length; j++) {
                if (i + j >= a1->length)return -1; //超界

                if (a2->data1[j] != a1->data1[i + j]) {//不匹配
                    match = 0;
                    break;
                }
            }
            if (match)return i;
        }
    }
    return -1;
}

int utf8_last_indexof_pos_c(Utf8String *a1, char *a2, int a1_rightpos) {
    Utf8String *ua2 = utf8_create_c(a2);
    int index = utf8_last_indexof_pos(a1, ua2, a1_rightpos);
    utf8_destory(ua2);
    return index;
}

int utf8_last_indexof_c(Utf8String *a1, char *a2) {
    Utf8String *ua2 = utf8_create_c(a2);
    int index = utf8_last_indexof(a1, ua2);
    utf8_destory(ua2);
    return index;
}

int utf8_indexof_c(Utf8String *a1, char *a2) {
    utf8_indexof_pos_c(a1, a2, 0);
}

int utf8_indexof_pos_c(Utf8String *a1, char *a2, int a1_pos) {
    Utf8String *ua2 = utf8_create_c(a2);
    int index = utf8_indexof_pos(a1, ua2, a1_pos);
    utf8_destory(ua2);
    return index;
}

void utf8_replace(Utf8String *a1, Utf8String *a2, Utf8String *a3) {
    //printf("utf8_replace,%s,%s,%s\n", utf8_cstr(a1), utf8_cstr(a2), utf8_cstr(a3));
    if (a1 == NULL || a2 == NULL || a3 == NULL)return;
    if (a1->length == 0 || a2->length == 0)return;
    Utf8String *tmps = utf8_create();
    utf8_append(tmps, a1);
    utf8_clear(a1);
    int end = 0, start = 0, len = 0;
    for (;;) {
        end = utf8_indexof_pos(tmps, a2, start);
        //printf("find indexof :%d\n", end);
        if (end == -1) {
            utf8_append_part(a1, tmps, start, tmps->length - start);
            break;
        }
        len = end - start;
        utf8_append_part(a1, tmps, start, len);
        utf8_append(a1, a3);
        start = end + a2->length;
    }
    utf8_destory(tmps);
}

void utf8_replace_c(Utf8String *a1, char *a2, char *a3) {
    Utf8String *ua2 = utf8_create_c(a2);
    Utf8String *ua3 = utf8_create_c(a3);
    utf8_replace(a1, ua2, ua3);
    utf8_destory(ua2);
    utf8_destory(ua3);
}

int utf8_equals(Utf8String *a1, Utf8String *a2) {
    if (a1 == NULL && a2 == NULL)return 1;
    if (a1 == NULL || a2 == NULL)return 0;
    if (_utf8_hashCode(a1) != _utf8_hashCode(a2))return 0;
    if (a1->length != a2->length)return 0;

    int i = 0;
    for (; i < a1->length && i < a2->length; i++) {
        if (a1->data1[i] > a2->data1[i]) {
            return 0;
        } else if (a1->data1[i] < a2->data1[i]) {
            return 0;
        }
    }
    return 1;
}

int utf8_equals_c(Utf8String *a1, char *a2) {
    Utf8String *ua2 = utf8_create_c(a2);
    int index = utf8_equals(a1, ua2);
    utf8_destory(ua2);
    return index;
}

utf8_char utf8_char_at(Utf8String *a1, int pos) {
    return a1->data1[pos];
}

int UNICODE_STR_EQUALS_FUNC(HashtableValue value1, HashtableValue value2) {
    return utf8_equals(value1, value2) == 1;
}

unsigned long UNICODE_STR_HASH_FUNC(HashtableKey kmer) {
    return _utf8_hashCode(kmer);
}


//****************************************************************************
unsigned long _utf8_hashCode(Utf8String *ustr) {
    if (!ustr->hash) {//如果未有赋值，则需计算
        int i = 0;
        for (i; i < ustr->length; i++) {
            ustr->hash = 31 * ustr->hash + ustr->data1[i];
        }
    }
    return ustr->hash;
}

int _utf8_space_require(Utf8String *ustr, int size) {
    if (ustr->length + size > ustr->_alloced) {
        return _utf8_enlarge(ustr);
    }
    return 0;
}

int _utf8_enlarge(Utf8String *ustr) {
    utf8_char *data;
    int newsize;

    /* Double the allocated size */

    newsize = ustr->_alloced * 2;

    /* Reallocate the array to the new size */

    data = jvm_realloc(ustr->data1, sizeof(utf8_char) * newsize);

    if (data == NULL) {
        return 0;
    } else {
        ustr->data1 = data;
        ustr->_alloced = newsize;

        return 1;
    }
}

int utf8_insert(Utf8String *ustr, int index, utf8_char data) {
    /* Sanity check the index */

    if (index < 0 || index > ustr->length) {
        return 0;
    }

    /* Increase the size if necessary */

    if (ustr->length + 1 > ustr->_alloced) {
        if (!_utf8_enlarge(ustr)) {
            return 0;
        }
    }

    /* Move the contents of the array forward from the index
     * onwards */

    memmove(&ustr->data1[index + 1],
            &ustr->data1[index],
            (ustr->length - index) * sizeof(utf8_char));

    /* Insert the new entry at the index */

    ustr->data1[index] = data;
    ++ustr->length;
    ustr->hash = 0;
    return 1;
}

int utf8_pushback(Utf8String *ustr, utf8_char data) {
    return utf8_insert(ustr, ustr->length, data);
}

int utf8_pushfront(Utf8String *ustr, utf8_char data) {
    return utf8_insert(ustr, 0, data);
}

void utf8_remove_range(Utf8String *ustr, int index, int length) {
    /* Check this is a valid range */

    if (index < 0 || length < 0 || index + length > ustr->length) {
        return;
    }

    /* Move back the entries following the range to be removed */

    memmove(&ustr->data1[index],
            &ustr->data1[index + length],
            (ustr->length - (index + length)) * sizeof(utf8_char));

    /* Decrease the counter */

    ustr->length -= length;
    ustr->hash = 0;
}

void utf8_remove(Utf8String *ustr, int index) {
    utf8_remove_range(ustr, index, 1);
}

int utf8_index_of(Utf8String *ustr, utf8_char data) {
    int i;

    for (i = 0; i < ustr->length; ++i) {
        if (ustr->data1[i] == data)
            return i;
    }

    return -1;
}



