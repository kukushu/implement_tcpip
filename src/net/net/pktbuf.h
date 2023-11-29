#ifndef PKTBUF_H
#define PKTBUF_H

#include "types.h"
#include "net_err.h"
#include "net_cfg.h"
#include "nlist.h"

typedef struct _pktblk_t {
    nlist_node_t node;
    int size;
    uint8_t * data;
    uint8_t payload[PKTBUF_BLK_SIZE];
} pktblk_t;

typedef struct _pktbuf_t { 
    int total_size;
    nlist_t blk_list;
    nlist_node_t node; 

    int pos;
    pktblk_t * curr_blk;
    uint8_t blk_offset;
} pktbuf_t;

net_err_t pktbuf_init (void);
pktbuf_t * pktbuf_alloc (int size);
void pktbuf_write (pktbuf_t * pktbuf, const char * data, int size);
void pktbuf_read (pktbuf_t * pktbuf);

#endif 