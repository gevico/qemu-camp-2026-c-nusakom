#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    // 找到 '?' 的位置
    const char* query = strchr(url, '?');
    if (!query) {
        err = -1;
        goto exit;
    }
    query++; // 跳过 '?'

    char key[256], value[256];
    const char* p = query;

    while (*p) {
        // 解析 key
        int ki = 0;
        while (*p && *p != '=' && *p != '&') {
            key[ki++] = *p++;
        }
        key[ki] = '\0';

        // 跳过 '='
        if (*p == '=') p++;

        // 解析 value
        int vi = 0;
        while (*p && *p != '&') {
            // 【修改点】：移除 '+' -> ' ' 的转换逻辑，直接保留原字符
            // 如果题目后续要求支持标准URL解码（%XX），可以在此处添加逻辑
            // 但针对当前测试，必须原样输出 '+'
            value[vi++] = *p++;
        }
        value[vi] = '\0';

        // 按测试要求输出格式
        printf("key = %s, value = %s\n", key, value);

        // 跳过 '&'
        if (*p == '&') p++;
    }

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}