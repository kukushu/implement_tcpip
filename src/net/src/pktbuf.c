#include "pktbuf.h"
#include "mblock.h"

#include <pthread.h>
#include <stdio.h>

static pthread_mutex_t pktbuf_locker;
static mblock_t pktblk_list;
static pktblk_t pktblk_buffer [PKTBUF_BLK_CNT];
static mblock_t pktbuf_list;
static pktbuf_t pktbuf_buffer [PKTBUF_BUF_CNT];

net_err_t pktbuf_init (void) {
    dbg_info (DBG_PKTBUF, "init pktbuf list");

    if (pthread_mutex_init (&pktbuf_locker, NULL) != 0) {
        dbg_info (DBG_PKTBUF, "pthread_mutex_init failed");
        return NET_ERR_PKTBUF;
    }

    mblock_init (&pktblk_list, pktblk_buffer, sizeof (pktblk_t), PKTBUF_BLK_CNT);
    mblock_init (&pktbuf_list, pktbuf_buffer, sizeof (pktbuf_t), PKTBUF_BUF_CNT);
    dbg_info (DBG_PKTBUF, "pktbuf init done");

    return NET_ERR_OK;
}

static pktblk_t * pktblk_alloc () {
    pthread_mutex_lock (&pktbuf_locker);
    pktblk_t * pktblk = mblock_alloc (&pktblk_list);
    pthread_mutex_unlock (&pktbuf_locker);

    if (!pktblk) {
        dbg_error (DBG_PKTBUF, "pktblk_alloc failed");
        return (pktblk_t *) 0;
    }

    pktblk->data = 0;
    pktblk->size = 0;
    nlist_node_init (&pktblk->node);

    return pktblk;
}


pktbuf_t * pktbuf_alloc (int size) {
    pthread_mutex_lock (&pktbuf_locker);
    pktbuf_t * pktbuf = mblock_alloc (&pktbuf_list);
    pthread_mutex_unlock (&pktbuf_locker);

    if (!pktbuf) {
        dbg_error (DBG_PKTBUF, "pktbuf_alloc mblock_alloc failed");
        return (pktbuf_t *) 0;
    }

    pktbuf->total_size = size;
    nlist_init (&pktbuf->blk_list);
    nlist_node_init (&pktbuf->node);

    while (size) {
        int curr_size = size > PKTBUF_BLK_SIZE ? PKTBUF_BLK_SIZE : size;
        size -= curr_size;

        pktblk_t * pktblk = (pktblk_t *) pktblk_alloc ();
        pktblk->size = curr_size;
        pktblk->data = (char *) pktblk->payload + (PKTBUF_BLK_SIZE - curr_size);

        nlist_insert_head (&pktbuf->blk_list, &pktblk->node);
    }

    pktbuf->pos = 0;
    pktbuf->curr_blk = nlist_get_entry (pktblk_t, pktbuf->blk_list.first, node);
    

    return pktbuf;
}

static int curr_blk_remain (pktbuf_t * pktbuf) {
    pktblk_t * pktblk = pktbuf->curr_blk;
    return (int) (pktblk->data - pktbuf->blk_offset - pktblk->payload  + pktblk->size);
}

static pktblk_t * pktblk_next (pktblk_t * pktblk) {
    return nlist_get_entry (pktblk_t, pktblk->node.next, node);
}

void pktbuf_write (pktbuf_t * pktbuf, const char * data, int size) {
    int remain_size = pktbuf->total_size - pktbuf->pos;
    if (remain_size >= size) {
        while (size > 0) {
            int curr_blk_size = curr_blk_remain (pktbuf);
            int curr_copy = size > curr_blk_size ? curr_blk_size : size;
            size -= curr_copy;
            memcpy (pktbuf->curr_blk->data + pktbuf->blk_offset, data, curr_copy);
            pktbuf->blk_offset += curr_copy;
            if (curr_blk_size == size) {
                pktbuf->curr_blk = pktblk_next (pktbuf->curr_blk);
                pktbuf->blk_offset = 0;
            }
            data += curr_copy;
        }
    } else {
        dbg_error (DBG_PKTBUF, "pktbuf_write failed, reamian_size < size");
    }
}

void pktbuf_read (pktbuf_t * pktbuf) {
    int remain_size = pktbuf->total_size;
    while (remain_size) {
        int curr_blk_size = pktbuf->curr_blk->size;
        int curr_read = remain_size > curr_blk_size ? curr_blk_size : remain_size;
        remain_size -= curr_read;
        for (int i = 0; i < curr_read; i ++) {
            printf ("%02x ", pktbuf->curr_blk->data[i]);
            if ((i + 1) % 16 == 0) {
                printf ("\n");
            }
        }
        printf ("\n");
    }
}








