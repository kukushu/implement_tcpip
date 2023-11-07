#include "netif_pcap.h"
#include "exmsg.h"
#include "net_err.h"

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

void * recv_thread (void *arg) {
    printf ("recv thread is running\n");
    while (1) {
        exmsg_netif_in ();
        sleep (1);
    }
    return (void *) 0;
}

void * xmit_thread (void *arg) {
    printf ("xmit thread is running\n");
    while (1) {
        sleep (1);
    }
    return (void *) 0;
}

net_err_t netif_pcap_open (void) {
    pthread_t thread_id1;
    pthread_t thread_id2;
    pthread_create (&thread_id1, NULL, recv_thread, NULL);
    pthread_create (&thread_id2, NULL, xmit_thread, NULL);
    return NET_ERR_OK;
}

