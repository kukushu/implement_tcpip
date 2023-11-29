#include "exmsg.h"
#include "netif.h"
#include "fixq.h"
#include "net_err.h"
#include "net_cfg.h"
#include "mblock.h"
#include "dbg.h"

#include <stdio.h>
#include <pthread.h>

static void * msg_tbl[EXMSG_MSG_CNT];
static fixq_t msg_queue;

static exmsg_t msg_buffer[EXMSG_MSG_CNT];
static mblock_t msg_block;

net_err_t exmsg_init (void) {
    dbg_info (DBG_MSG, "exmsg_init");
    net_err_t err = fixq_init (&msg_queue, msg_tbl, EXMSG_MSG_CNT);
    if (err < 0) {
        dbg_error (DBG_MSG, "fixq init failed");
        return err;
    }
    err = mblock_init (&msg_block, msg_buffer, sizeof (exmsg_t), EXMSG_MSG_CNT);
    if (err < 0) {
        dbg_error (DBG_MSG, "mblock init failed");
        return err;
    }
    dbg_info (DBG_MSG, "exmsg init done");

    return NET_ERR_OK;
}

void printHex(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}



static void * work_thread (void * arg) {
    dbg_info (DBG_MSG, "exmsg work thread is running");

    while (1) {
        exmsg_t * msg = (exmsg_t *) fixq_recv (&msg_queue);
        
        printf ("******************************\n");
        pktbuf_read ((pktbuf_t *) fixq_recv (&msg->netif.netif->in_q));

        mblock_free (&msg_block, msg);
    }
    return (void *) 0;
}

net_err_t exmsg_netif_in (netif_t * netif) {
    exmsg_t * msg = mblock_alloc (&msg_block);

    msg->type = NET_EXMSG_NETIF_IN;
    msg->netif.netif = netif;

    net_err_t err = fixq_send (&msg_queue, msg);

    return NET_ERR_OK;
}

net_err_t exmsg_start (void) {
    pthread_t thread_id;
    if (pthread_create (&thread_id, NULL, work_thread, (void *) 0) != 0) {
        dbg_error (DBG_EXMSG, "pthread_create() failed");
        return NET_ERR_EXMSG;
    }

    return NET_ERR_OK;
}


