#include "ether.h"

static net_err_t ether_open (netif_t * netif) {

    return NET_ERR_OK;
}

static void ether_close (netif_t * netif) {

}

static net_err_t ether_in (netif_t * netif, pktbuf_t * pktbuf) {
    
    return NET_ERR_OK;
}

static net_err_t ether_out (netif_t * netif, ipaddr_t * ip_addr, pktbuf_t * pktbuf) {

    return NET_ERR_OK;
}

net_err_t ether_init (void) {
    static const link_layer_t link_layer = {
        .type = NETIF_TYPE_ETHER,
        .open = ether_open,
        .close = ether_close,
        .in = ether_in,
        .out = ether_out,
    };
    dbg_info (DBG_ETHER, "ether init");
    net_err_t err = netif_register_layer (NETIF_TYPE_ETHER, &link_layer);
    if (err < 0) {
        dbg_info (DBG_ETHER, "netif_register error");
        return err;
    }
    dbg_info (DBG_ETHER, "ether init done");
    return NET_ERR_OK;
}