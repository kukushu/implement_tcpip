#include "mblock.h"
#include "net_err.h"
#include "net_cfg.h"
#include "dbg.h"
#include <stdio.h>

net_err_t mblock_init (mblock_t *mblock, void * mem, int blk_size, int cnt) {
    if (sem_init (&mblock->sem, 0, cnt) == -1) {
        dbg_error (DBG_MBLOCK, "sem init failed");
        return NET_ERR_MBLOCK;
    }

    if (pthread_mutex_init (&mblock->mutex, NULL) != 0) {
        dbg_error (DBG_MBLOCK, "pthread_mutex init failed");
        return NET_ERR_MBLOCK;
    }

    if (blk_size <= sizeof (nlist_node_t)) {
        dbg_error (DBG_MBLOCK, "the block_size is too small");
        return NET_ERR_MBLOCK; 
    }
    
    mblock->start = mem;
    nlist_init (&mblock->free_list);
    for (int i = 0; i < cnt; i ++) {
        nlist_node_init ((nlist_node_t *) ((char *) mem + i * blk_size));
        nlist_insert_head (&mblock->free_list, (nlist_node_t *) ((char *) mem + i * blk_size));
    }
    return NET_ERR_OK;
}

void * mblock_alloc (mblock_t * mblock) {

    if (sem_wait (&mblock->sem) != 0) {
        return (void *) 0;
    } else  {

        pthread_mutex_lock (&mblock->mutex);
        nlist_node_t * node = nlist_remove_first (&mblock->free_list);
        pthread_mutex_unlock (&mblock->mutex);

        return (void *) node;
    }
}

int mblock_free_cnt (mblock_t * mblock) {
    pthread_mutex_lock (&mblock->mutex);
    int count = mblock->free_list.count; 
    pthread_mutex_unlock (&mblock->mutex);
    return count;
}

void mblock_free (mblock_t * mblock, void * block) {
    pthread_mutex_lock (&mblock->mutex);
    nlist_insert_tail (&mblock->free_list, block);
    pthread_mutex_unlock (&mblock->mutex);
}
void mblock_destory (mblock_t * mblock) {
    sem_destroy (&mblock->sem);
    pthread_mutex_destroy (&mblock->mutex);
}