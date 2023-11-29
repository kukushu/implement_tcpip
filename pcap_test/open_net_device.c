#include <stdio.h>
#include <pcap.h>

void printHex(const unsigned char *data, int length) {
    for (int i = 0; i < length; i++) {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

void packetHandler(unsigned char *user_data, const struct pcap_pkthdr *pkthdr, const unsigned char *packet) {
    printf("Received a packet of length: %d\n", pkthdr->len);
    printf("Hexdump of packet data:\n");
    printHex(packet, pkthdr->len);
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];

    // 打开网卡接口（例如 eth0）
    const char *device = "ens33";
    pcap_t *handle = pcap_open_live(device, 65535, 1, 1000, errbuf);
    
    if (handle == NULL) {
        fprintf(stderr, "Error opening device: %s\n", errbuf);
        return 1;
    }

    // 开始捕获数据包，此处只捕获一个数据包
    struct pcap_pkthdr * header;
    const unsigned char *packet;
    int returnValue;

    returnValue = pcap_next_ex(handle, &header, &packet);
    if (returnValue == 1) { // 数据包捕获成功
        packetHandler(NULL, header, packet); // 调用处理函数来处理数据包
    } else if (returnValue == 0) { // 超时，没有数据包到达
        printf("Timeout waiting for a packet\n");
    } else if (returnValue == -1) { // 捕获错误
        fprintf(stderr, "Error capturing a packet: %s\n", pcap_geterr(handle));
    }

    // 关闭捕获会话
    pcap_close(handle);

    return 0;
}
