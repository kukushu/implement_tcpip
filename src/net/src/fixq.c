#include "fixq.h"
#include "dbg.h"
#include "net_cfg.h"
#include <pthread.h>

net_err_t fixq_init (fixq_t * fixq, void ** buf, int size) {

    fixq->size = size;
    fixq->in = fixq->out = 0;
    fixq->buf = buf;
    pthread_mutex_init (&fixq->mutex, NULL);
    sem_init (&fixq->send_sem, 0, size);
    sem_init (&fixq->recv_sem, 0, 0);

    return NET_ERR_OK;
}
net_err_t fixq_send (fixq_t * fixq, void * msg) {
    if (sem_wait (&fixq->send_sem) != 0) {
        dbg_error (DBG_FIXQ, "fixq_send sem_wait failed");
        return NET_ERR_FIXQ;
    }
    pthread_mutex_lock (&fixq->mutex);
    fixq->buf[fixq->in ++] = msg;
    if (fixq->in >= fixq->size) {
        fixq->in = 0;
    }
    fixq->cnt ++;
    pthread_mutex_unlock (&fixq->mutex);

    sem_post (&fixq->recv_sem);
    return NET_ERR_OK;
}
void * fixq_recv (fixq_t * fixq) {
    if (sem_wait (&fixq->recv_sem) != 0) {
        dbg_error (DBG_FIXQ, "fixq_recv sem_wait failed");
        return (void *) 0;
    }

    pthread_mutex_lock (&fixq->mutex);
    void * msg = fixq->buf[fixq->out ++];
    if (fixq->out >= fixq->size) {
        fixq->out = 0;
    }
    fixq->cnt --;
    pthread_mutex_unlock (&fixq->mutex);

    sem_post (&fixq->send_sem);
    return msg;
}
void fixq_destroy (fixq_t * fixq) {
    sem_destroy (&fixq->send_sem);
    sem_destroy (&fixq->recv_sem);
    pthread_mutex_destroy (&fixq->mutex);
}
int fixq_count (fixq_t * fixq) {
    pthread_mutex_lock (&fixq->mutex);
    int count = fixq->cnt;
    pthread_mutex_unlock (&fixq->mutex);
    return count;
}
