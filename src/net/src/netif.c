#include "netif.h"
#include "net_cfg.h"
#include "ipaddr.h"
#include "mblock.h"

#include <string.h>

static netif_t netif_buffer [NETIF_DEV_CNT];
static mblock_t netif_mblock;
static nlist_t netif_list;
static const link_layer_t * link_layers [NETIF_TYPE_SIZE];




net_err_t netif_init (void) {
    dbg_info (DBG_NETIF, "init netif");
    nlist_init (&netif_list);
    mblock_init (&netif_mblock, netif_buffer, sizeof (netif_t), NETIF_DEV_CNT);
}

net_err_t netif_register_layer (netif_type_t type, const link_layer_t * layer) {
    link_layers [type] = layer;
    return NET_ERR_OK;
}

netif_t * netif_open (const char * dev_name, const netif_ops_t * ops, void * ops_data) {
    netif_t * netif = (netif_t *) mblock_alloc (&netif_mblock);
    
    ipaddr_init (&netif->ipaddr);
    ipaddr_init (&netif->netmask);
    ipaddr_init (&netif->gateway);
    netif->mtu = 0;
    netif->type = NETIF_TYPE_NONE;

    strncpy (netif->name, dev_name, NETIF_NAME_SIZE); 
    netif->name [NETIF_NAME_SIZE - 1] = '\0';
    netif->driver_ops = ops;
    netif->pcap_handle = ops_data;

    // 初始化接收队列
    net_err_t err = fixq_init(&netif->in_q, netif->in_q_buf, NETIF_INQ_SIZE);
    if (err < 0) {
        dbg_error(DBG_NETIF, "netif in_q init error, err: %d", err);
        return (netif_t *)0;
    }

    // 初始化发送队列
    err = fixq_init(&netif->out_q, netif->out_q_buf, NETIF_OUTQ_SIZE);
    if (err < 0) {
        dbg_error(DBG_NETIF, "netif out_q init error, err: %d", err);
        fixq_destroy(&netif->in_q);
        return (netif_t *)0;
    }

    err = netif->driver_ops->open (netif, ops_data);

    netif->state = NETIF_OPENED;
    netif->link_layer = link_layers [netif->type];
    nlist_insert_tail (&netif_list, &netif->node);
    
    return netif;
}

net_err_t netif_set_hwaddr (netif_t * netif, const uint8_t * hwaddr, int len) {
    memcpy (netif->hwaddr.addr, hwaddr, len);
    netif->hwaddr.len = len;
    return NET_ERR_OK;
}

ipaddr_t * ipaddr_get_any (void) {
    static ipaddr_t ipaddr_any = { .raw_addr = 0 };
    return &ipaddr_any;
}

net_err_t netif_set_addr(netif_t* netif, ipaddr_t* ip, ipaddr_t* netmask, ipaddr_t* gateway) {
    ipaddr_copy(&netif->ipaddr, ip ? ip : ipaddr_get_any());
    ipaddr_copy(&netif->netmask, netmask ? netmask : ipaddr_get_any());
    ipaddr_copy(&netif->gateway, gateway ? gateway : ipaddr_get_any());
    return NET_ERR_OK;
}

net_err_t netif_set_active (netif_t * netif) {
    net_err_t err = netif->link_layer->open (netif);
    if (err < 0) {
        dbg_info (DBG_NETIF, "active error");
        return err;
    }

    netif->state = NETIF_ACTIVE;

    return NET_ERR_OK;
}


