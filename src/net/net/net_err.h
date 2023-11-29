#ifndef NET_ERR_H
#define NET_ERR_H

typedef enum _net_err_t {
    NET_ERR_OK = 1,
    NET_ERR_MBLOCK = -1,
    NET_ERR_FIXQ = -2,
    NET_ERR_EXMSG = -3,
    NET_ERR_PKTBUF = -4,
    NET_ERR_NETIF = -5,
    NET_ERR_PARAM = -6,
} net_err_t;



#endif