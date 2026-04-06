#include <stdint.h>
#include <stdio.h>
#include <string.h>

#pragma pack(push, 1)

typedef struct {
    uint8_t  version_raw;  /* 高4位主版本，低4位次版本 */
    uint8_t  version_low;  /* 版本字段第2字节（未使用，补齐2字节） */
    uint16_t length_be;    /* 网络序长度 */
    uint8_t  flags_raw;    /* 标志字节 */
} proto_header_raw_t;

typedef struct {
    unsigned int ver_major : 4;
    unsigned int ver_minor : 4;
    uint16_t     length;
    unsigned int flags     : 5;
    unsigned int reserved  : 3;
} proto_header_bits_t;

#pragma pack(pop)

static uint16_t be16_to_cpu(uint16_t be) {
    return (uint16_t)(((be & 0x00FFu) << 8) | ((be & 0xFF00u) >> 8));
}

int main(void) {
    const uint8_t stream[5] = {0x00, 0x03, 0x00, 0x20, 0x00};

    proto_header_raw_t raw = {0};
    memcpy(&raw, stream, sizeof(raw));

    /* 版本：stream[0]=0x00 (主版本高4位), stream[1]=0x03 (次版本低4位) */
    unsigned ver_major = (raw.version_raw >> 4) & 0x0Fu;
    unsigned ver_minor = raw.version_low & 0x0Fu;

    uint16_t length = be16_to_cpu(raw.length_be);
    unsigned flags = (unsigned)(raw.flags_raw & 0x1Fu);

    proto_header_bits_t view = {0};
    view.ver_major = ver_major;
    view.ver_minor = ver_minor;
    view.length    = length;
    view.flags     = flags;

    printf("version:%u.%u, length:%u, flags:0x%02X\n",
           view.ver_major, view.ver_minor, view.length, view.flags & 0xFFu);

    return 0;
}
