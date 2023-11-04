#include "nlist.h"

void nlist_init (nlist_t * list) {
    list->first = list->last = (nlist_node_t *) 0;
    list->count = 0;
}
void nlist_insert_head (nlist_t * list, nlist_node_t * node) {
    if (0 == list->count) {
        node->pre = (nlist_node_t *) 0;
        node->next = (nlist_node_t *) 0;
        list->first = node;
        list->last = node;
    } else {
        node->pre = (nlist_node_t *) 0;
        node->next = list->first;
        list->first->pre = node;
        list->first = node;
    }
    list->count ++;
    return ;
}
void nlist_insert_tail (nlist_t * list, nlist_node_t * node) {
    if (0 == list->count) {
        node->pre = (nlist_node_t *) 0;
        node->next = (nlist_node_t *) 0;
        list->first = node;
        list->last = node;
    } else {
        node->next = (nlist_node_t *) 0;
        node->pre = list->last;
        list->last->next = node;
        list->last = node;
    }
    list->count ++;
    return ;
}
void nlist_insert_after (nlist_t * list, nlist_node_t * pre, nlist_node_t * node) {
    if (nlist_is_empty (list)) {
        nlist_insert_head (list, node);
        return ;
    } else if (list->last == pre) {
        nlist_insert_tail (list, node);
        return ;
    }
    node->pre = pre;
    node->next = pre->next;
    pre->next->pre = node;
    pre->next->pre = node;
    return ;
}