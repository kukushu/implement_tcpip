#include <pcap.h>
#include <stdio.h>
typedef unsigned char u_char;

void packet_handler(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    printf("Packet capture length: %d\n", pkthdr->caplen);
    printf("Packet total length: %d\n", pkthdr->len);
}

int main() {
    pcap_if_t *alldevs;
    pcap_if_t *device;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    // 获取设备列表
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return(2);
    }

    // 遍历设备列表并选择第一个设备
    for(device = alldevs; device != NULL; device = device->next) {
        printf("Device found: %s\n", device->name);
    }
    if (alldevs == NULL) {
        fprintf(stderr, "No devices found! Make sure libpcap is properly installed.\n");
        return(2);
    }

    // 使用列表中的第一个设备进行捕获
    handle = pcap_open_live(alldevs->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", alldevs->name, errbuf);
        pcap_freealldevs(alldevs); // 清理设备列表
        return(2);
    }
    
    // 抓取包
    pcap_loop(handle, 10, packet_handler, NULL);
    
    // 关闭抓包设备
    pcap_close(handle);
    
    // 清理设备列表
    pcap_freealldevs(alldevs);

    return(0);
}

