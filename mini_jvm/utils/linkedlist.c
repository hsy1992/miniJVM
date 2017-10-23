//
// Created by gust on 2017/10/23.
//


#include <stdlib.h>
#include <global.h>

#include "linkedlist.h"

/* A doubly-linked list */

struct _ListEntry {
    ListValue data;
    ListEntry *prev;
    ListEntry *next;
};

LinkedList *linkedlist_create() {
    LinkedList *list = jvm_alloc(sizeof(LinkedList));
    list->mNode = jvm_alloc(sizeof(ListEntry));
    return list;
}

void linkedlist_destory(LinkedList *list) {
    _linkedlist_free(list->mNode);
    jvm_free(list);
}

void _linkedlist_free(ListEntry *list) {
    ListEntry *entry;

    entry = list;

    while (entry != NULL) {
        ListEntry *next;

        next = entry->next;

        jvm_free(entry);

        entry = next;
    }
}

ListEntry *linkedlist_push_front(LinkedList *list, ListValue data) {
    ListEntry *mNode = list->mNode;
    ListEntry *newentry;

    if (list == NULL) {
        return NULL;
    }
    newentry = jvm_alloc(sizeof(ListEntry));
    if (newentry == NULL) {
        return NULL;
    }
    newentry->data = data;
    list->count++;
    //
    if (mNode->next == NULL) {  //make a loop
        mNode->prev = mNode->next = newentry;
        newentry->prev = newentry->next = mNode;
    } else {
        ListEntry *oldnext = mNode->next;
        if (oldnext != NULL) {
            oldnext->prev = newentry;
        }
        newentry->prev = mNode;
        newentry->next = oldnext;
        mNode->next = newentry;
    }
    return newentry;
}

ListEntry *linkedlist_push_end(LinkedList *list, ListValue data) {

    ListEntry *mNode = list->mNode;
    ListEntry *newentry;

    if (list == NULL) {
        return NULL;
    }
    newentry = jvm_alloc(sizeof(ListEntry));
    if (newentry == NULL) {
        return NULL;
    }
    newentry->data = data;
    list->count++;
    //
    if (mNode->prev == NULL) {  //make a loop
        mNode->prev = mNode->next = newentry;
        newentry->prev = newentry->next = mNode;
    } else {
        ListEntry *oldprev = mNode->prev;

        if (oldprev) {
            oldprev->next = newentry;
        }
        mNode->prev = newentry;
        newentry->next = mNode;
        newentry->prev = oldprev;
    }
    return newentry;
}

ListEntry *linkedlist_pop_front(LinkedList *list) {
    if (list == NULL) {
        return NULL;
    }
    ListEntry *mNode = list->mNode;
    ListEntry *oldnext = mNode->next;
    if (oldnext) {
        mNode->next = oldnext->next;
        oldnext->prev = oldnext->next = NULL;
        list->count--;
    }
    if (mNode->next) {
        mNode->next->prev = mNode;
    }
    if (mNode->prev == mNode) {  // remove loop
        mNode->next = mNode->prev = NULL;
    }

    return oldnext;
}

ListEntry *linkedlist_pop_end(LinkedList *list) {
    if (list == NULL) {
        return NULL;
    }
    ListEntry *mNode = list->mNode;
    ListEntry *oldprev = mNode->prev;
    if (oldprev) {
        mNode->prev = oldprev->prev;
        oldprev->prev = oldprev->next = NULL;
        list->count--;
    }
    if (mNode->prev) {
        mNode->prev->next = mNode;
    }
    if (mNode->next == mNode) {
        mNode->next = mNode->prev = NULL;
    }

    return oldprev;
}

ListValue linkedlist_data(ListEntry *listentry) {
    if (listentry == NULL) {
        return LINKEDLIST_NULL;
    }

    return listentry->data;
}

void linkedlist_set_data(ListEntry *listentry, ListValue value) {
    if (listentry != NULL) {
        listentry->data = value;
    }
}


ListEntry *linkedlist_prev(ListEntry *listentry) {
    if (listentry == NULL) {
        return NULL;
    }

    return listentry->prev;
}


ListEntry *linkedlist_next(ListEntry *listentry) {
    if (listentry == NULL) {
        return NULL;
    }

    return listentry->next;
}
