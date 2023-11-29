#ifndef NETIF_PCAP_H
#define NETIF_PCAP_H

#include "net_err.h"
#include "types.h"

typedef struct _pcap_data_t {
    const char * ip;
    const uint8_t * hwaddr;
} pcap_data_t;


#endif