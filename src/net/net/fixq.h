#ifndef FIXQ_H
#define FIXQ_H

#include "net_err.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct _fixq_t {
    int size;
    int in, out, cnt;

    void ** buf;

    pthread_mutex_t mutex;
    sem_t send_sem;
    sem_t recv_sem;
} fixq_t;

net_err_t fixq_init (fixq_t * fixq, void ** buf, int size);
net_err_t fixq_send (fixq_t * fixq, void * msg);
void * fixq_recv (fixq_t * fixq);
void fixq_destroy (fixq_t * fixq);
int fixq_count (fixq_t * fixq);





#endif