#include "net.h"
#include "net_err.h"
#include "exmsg.h"
#include "pktbuf.h"
#include "ether.h"
#include "netif.h"

net_err_t net_init (void) {
    exmsg_init ();
    pktbuf_init ();
    netif_init ();
    ether_init ();
    

    return NET_ERR_OK;
}
net_err_t net_start (void) {
    exmsg_start ();
    return NET_ERR_OK;
}





