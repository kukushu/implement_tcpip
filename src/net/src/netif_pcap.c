#include "netif_pcap.h"
#include "exmsg.h"
#include "net_err.h"
#include "ether.h"
#include "netif.h"
#include "fixq.h"

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <pcap.h>

void * recv_thread (void *arg) {
    printf ("recv thread is running...\n");

    netif_t * netif = (netif_t *) arg;
    pcap_t * pcap = (pcap_t *) netif->pcap_handle;
     
    int i = 2;
    while (i --) {
        struct pcap_pkthdr * header;
        const unsigned char *packet;

        if (pcap_next_ex(pcap, &header, &packet) != 1) {
            continue;
        }

        pktbuf_t * pktbuf = pktbuf_alloc (header->len);
        if (pktbuf == (pktbuf_t *) 0) {
            dbg_error (DBG_PKTBUF, "pktbuf_alloc failed");
            continue;
        } 

        pktbuf_write (pktbuf, packet, header->len);

        net_err_t err = fixq_send (&netif->in_q, pktbuf);
        printf ("send a message\n");

        exmsg_netif_in (netif);
    }
    printf ("the end of recv thread....\n") ;
    return (void *) 0;
}

void * xmit_thread (void *arg) {
    printf ("xmit thread is running\n");
    while (1) {
        sleep (1);
    }
    return (void *) 0;
}

int pcap_find_device(const char* ip, char* name_buf) {
    struct in_addr dest_ip;

    inet_pton(AF_INET, ip, &dest_ip);

    // 获取所有的接口列表
    char err_buf[PCAP_ERRBUF_SIZE];
    pcap_if_t* pcap_if_list = NULL;
    int err = pcap_findalldevs(&pcap_if_list, err_buf);
    if (err < 0) {
        pcap_freealldevs(pcap_if_list);
        return -1;
    }

    // 遍历列表
    pcap_if_t* item;
    for (item = pcap_if_list; item != NULL; item = item->next) {
        if (item->addresses == NULL) {
            continue;
        }

        // 查找地址
        for (struct pcap_addr* pcap_addr = item->addresses; pcap_addr != NULL; pcap_addr = pcap_addr->next) {
            // 检查ipv4地址类型
            struct sockaddr* sock_addr = pcap_addr->addr;
            if (sock_addr->sa_family != AF_INET) {
                continue;
            }

            // 地址相同则返回
            struct sockaddr_in* curr_addr = ((struct sockaddr_in*)sock_addr);
            if (curr_addr->sin_addr.s_addr == dest_ip.s_addr) {
                strcpy(name_buf, item->name);
                pcap_freealldevs(pcap_if_list);
                return 0;
            }
        }
    }

    pcap_freealldevs(pcap_if_list);
    return -1;
}

net_err_t netif_pcap_open (struct _netif_t * netif, void * ops_data) {
    pcap_data_t * dev_data = (pcap_data_t *) ops_data;
    char name_buf [100];
    if ( pcap_find_device (dev_data->ip, name_buf) == -1) {
        dbg_error (DBG_NETIF, "couldn't find device by ip");
        return NET_ERR_NETIF;
    }
    
    char errbuf [PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_live (name_buf, 65535, 1, 1000, errbuf);
    
    if (handle == NULL) {
        dbg_error (DBG_NETIF, "Error opening device: %s\n", errbuf);
        return NET_ERR_NETIF;
    }

    netif->pcap_handle = handle;
    netif->type = NETIF_TYPE_ETHER;
    netif->mtu = ETH_MTU;
    netif_set_hwaddr (netif, dev_data->hwaddr, NETIF_HWADDR_SIZE);



    pthread_t thread_id1;
    pthread_t thread_id2;
    pthread_create (&thread_id1, NULL, recv_thread, netif);
    pthread_create (&thread_id2, NULL, xmit_thread, netif);
    return NET_ERR_OK;
}


static void netif_pcap_close (struct _netif_t * netif) {

}

static net_err_t netif_pcap_xmit (struct _netif_t * netif) {

    return NET_ERR_OK;
}




const netif_ops_t netdev_ops = {
    .open = netif_pcap_open,
    .close = netif_pcap_close,
    .xmit = netif_pcap_xmit,
};