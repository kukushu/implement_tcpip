#ifndef NBLOCK_H
#define NBLOCK_H

#include "types.h"

typedef struct _nlist_node_t {
    struct _nlist_node_t * pre;
    struct _nlist_node_t * next;
} nlist_node_t;

typedef struct _nlist_t {
    nlist_node_t * first;
    nlist_node_t * last;
    int count;
} nlist_t;

static inline void nlist_node_init (nlist_node_t * node) {
    node->next = node->pre = (nlist_node_t *) 0;
}

static inline nlist_node_t * nlist_node_pre (nlist_node_t * node) {
    return node->pre;
}

static inline nlist_node_t * nlist_node_next (nlist_node_t * node) {
    return node->next;
}

static inline void nlist_node_set_next (nlist_node_t * node, nlist_node_t * next) {
    node->next = next;
    next->pre = node;
}






static inline bool nlist_is_empty (nlist_t * list) {
    return list->count == 0 ? TRUE : FALSE;
}

static inline int nlist_count (nlist_t * list) {
    return list->count;
}

static inline nlist_node_t * nlist_first (nlist_t * list) {
    return list->first;
}

static inline nlist_node_t * nlist_last (nlist_t * list) {
    return list->last;
}

void nlist_init (nlist_t * list);
void nlist_insert_head (nlist_t * list, nlist_node_t * node);
void nlist_insert_tail (nlist_t * list, nlist_node_t * node);
void nlist_insert_after (nlist_t * list, nlist_node_t * pre, nlist_node_t * node);
nlist_node_t * nlist_remove_node (nlist_t * list, nlist_node_t * node);
static inline nlist_node_t * nlist_remove_first (nlist_t * list) {
    nlist_node_t * first = nlist_first (list);
    if (first) {
        nlist_remove_node (list, first);
    }
    return first;
}
static inline nlist_node_t * nlist_remove_last (nlist_t * list) {
    nlist_node_t * last = nlist_last (list);
    if (last) {
        nlist_remove_node (list, last);
    }
    return last;
}


#define nlist_for_each(node_p, list) _nlist_for_each ((node_p), (list))
#define _nlist_for_each(node_p, list) for (node_p = list->first; node_p; node_p = node_p->next)
#define get_nlist_entry(type, pointer, element) ((type *)((char *) pointer - (char *) &(((type *) 0)->element)))




#endif