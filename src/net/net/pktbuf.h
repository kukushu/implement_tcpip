#ifndef PKTBUF_H
#define PKTBUF_H

#include "types.h"
#include "net_err.h"
#include "net_cfg.h"
#include "nlist.h"

typedef struct _pktblk_t {
    nlist_node_t node;
    int size;
    u_int8_t * data;
    u_int8_t payload[PKTBUF_BLK_SIZE];
} pktblk_t;

typedef struct _pktbuf_t { 
    int total_size;
    nlist_t blk_list;
    nlist_node_t node; 
} pktbuf_t;

net_err_t pktbuf_init (void);



#endif 