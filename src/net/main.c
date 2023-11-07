#include <stdio.h>

#include "fixq.h"
#include "nlist.h"
#include "dbg.h"
#include "mblock.h"
#include "net.h"

//definitions for test
#define NUM_NODES 10

net_err_t netdev_init (void) {
    netif_pcap_open ();
    return NET_ERR_OK;
}

void fixq_test ();
void mblock_test ();
void dbg_test ();
void nlist_test ();
void basic_test ();

int main () {
    basic_test ();

    net_init ();    
    net_start ();

    net_err_t err = netdev_init ();
    while (1) {
        sleep (10);
    }

    return 0;
}

void basic_test () {
    //nlist_test ();
    //dbg_test ();
    //mblock_test ();
    //fixq_test ();

}

void fixq_test () {
    fixq_t fixq;
    typedef struct {
        int a;
        char c;
        char cc;
    } msg_t;
    msg_t * buffer[5];
    fixq_init (&fixq, (void **) buffer, 5);
    printf ("%d\n", fixq_count (&fixq));
    msg_t msg = {3, 'c', 'c'};
    fixq_send (&fixq, &msg);
    printf ("%d\n", fixq_count (&fixq));
    msg_t * msg_recv = (msg_t *) fixq_recv (&fixq);
    if (msg_recv == &msg) {
        printf ("msg_recv == &msg\n");
    }
}

void mblock_test () {
    mblock_t mblock;
    static u_int8_t buffer[5][32];

    mblock_init (&mblock, buffer, 32, 5);

    void * temp[5];
    for (int i = 0; i < 5; i ++) {
        temp[i] = mblock_alloc (&mblock);
        printf ("block: %p, free_count: %d\n", temp[i], mblock_free_cnt (&mblock));
    }

    for (int i = 0; i < 5; i ++) {
        mblock_free (&mblock, temp[i]);
        printf ("free count: %d\n", mblock_free_cnt (&mblock));
    }
    mblock_destory (&mblock);
}
void dbg_test () {
    dbg_error (DBG_LEVEL_ERROR, "dbg_test");
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