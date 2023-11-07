#include "net.h"
#include "net_err.h"
#include "exmsg.h"

net_err_t net_init (void) {
    exmsg_init ();
    return NET_ERR_OK;
}
net_err_t net_start (void) {
    exmsg_start ();
    return NET_ERR_OK;
}





