#ifndef IPADDR_H
#define IPADDR_H

#include "types.h"
#include "net_err.h"

#define IPV4_ADDR_SIZE        4

typedef struct _ipaddr_t {
    enum {
        IPADDR_V4,
    } type;
    union {
        uint32_t raw_addr;
        uint8_t  byte_addr [IPV4_ADDR_SIZE];
    };
} ipaddr_t;

void ipaddr_init (ipaddr_t *addr);
net_err_t ipaddr_from_str (ipaddr_t * dest, const char * str);
void ipaddr_copy (ipaddr_t * dest, const ipaddr_t * src);

#endif