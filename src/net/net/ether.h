#ifndef ETHER_H
#define ETHER_H

#include "net_err.h"
#include "netif.h"

#define ETH_MTU 1500;

net_err_t ether_init (void);

#endif