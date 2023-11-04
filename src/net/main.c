#include <stdio.h>

#include "nlist.h"

//definitions for test
#define NUM_NODES 10

void nlist_test ();
void basic_test ();

int main () {
    basic_test ();
    return 0;
}

void basic_test () {
    nlist_test ();
    
}
void nlist_test () {
    typedef struct _tnode_t {
        int id;
        nlist_node_t node;
    } tnode_t;

    nlist_t list;
    nlist_init (&list);
    printf ("just after nlist init\n");

    tnode_t nodes[NUM_NODES];
    for (int i = 0; i < NUM_NODES; i ++) {
        nodes[i].id = i;
    }
    for (int i = 0; i < NUM_NODES; i ++) {
        nlist_node_init (&nodes[i].node);
    }

    nlist_insert_head (&list, &nodes[1].node);
    nlist_insert_head (&list, &nodes[0].node);
    nlist_insert_after (&list, &nodes[1].node, &nodes[2].node);
    nlist_insert_tail (&list, &nodes[3].node);

    nlist_node_t * p;
    nlist_for_each (p, &list) {
        printf ("%d\n", (get_nlist_entry (tnode_t, p, node))->id); 
    }
    printf ("count: %d\n", list.count);
}