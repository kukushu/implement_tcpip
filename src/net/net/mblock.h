#ifndef MBLOCK_H
#define MBLOCK_H

#include "net_err.h"
#include "nlist.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct _mblock_t {
    nlist_t free_list;
    void * start;
    pthread_mutex_t mutex;
    sem_t sem;
} mblock_t;

net_err_t mblock_init (mblock_t *mblock, void * mem, int blk_size, int cnt);
void * mblock_alloc (mblock_t * mblock);
int mblock_free_cnt (mblock_t * block);
void mblock_free (mblock_t * mblock, void * block);
void mblock_destory (mblock_t * mblock);







#endif