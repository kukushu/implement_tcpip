#include "ipaddr.h"

void ipaddr_init (ipaddr_t *addr) {
    addr->raw_addr = 0;
}

net_err_t ipaddr_from_str (ipaddr_t * dest, const char * str) {
// 必要的参数检查
    if (!dest || !str) {
        return NET_ERR_PARAM;
    }

    // 初始值清空
    dest->raw_addr = 0;

    // 不断扫描各字节串，直到字符串结束
    char c;
    uint8_t * p = dest->byte_addr;
    uint8_t sub_addr = 0;
    while ((c = *str++) != '\0') {
        // 如果是数字字符，转换成数字并合并进去
        if ((c >= '0') && (c <= '9')) {
            // 数字字符转换为数字，再并入计算
            sub_addr = sub_addr * 10 + c - '0';
        } else if (c == '.') {
            // . 分隔符，进入下一个地址符，并重新计算
            *p++ = sub_addr;
            sub_addr = 0;
        } else {
            // 格式错误，包含非数字和.字符
            return NET_ERR_PARAM;
        }
    }

    // 写入最后计算的值
    *p++ = sub_addr;
    return NET_ERR_OK;
}

void ipaddr_copy (ipaddr_t * dest, const ipaddr_t * src) {
    dest->raw_addr = src->raw_addr;
}