#ifndef EXMSG_H
#define EXMSG_H

#include "net_err.h"
#include "nlist.h"
#include "netif.h"

typedef struct _func_msg_t {
    
} func_msg_t;

typedef struct _msg_netif_t {
    netif_t * netif;   
} msg_netif_t;

typedef struct _exmsg_t {
    nlist_node_t node;
    enum {
        NET_EXMSG_NETIF_IN,
    } type;

    union {
        msg_netif_t netif;
        func_msg_t * func;
    };

} exmsg_t;

net_err_t exmsg_init (void);
net_err_t exmsg_start (void);
net_err_t exmsg_netif_in (netif_t * netif);

#endif 