#include <stdio.h>
#include <pcap.h>
#include <string.h>

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

int main () {
    char name_buf [100];
    pcap_find_device ("192.168.8.128", name_buf);
    printf ("name %s\n", name_buf);
    return 0;
}