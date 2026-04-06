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

    const char *p = strchr(url, '?');
    if (p == NULL) {
        printf("No parameters found\n");
        return 0;
    }
    p++; // Skip '?'

    char *query = strdup(p);
    if (query == NULL) return -ENOMEM;

    char *pair;
    char *saveptr1, *saveptr2;
    pair = strtok_r(query, "&", &saveptr1);
    while (pair != NULL) {
        char *key = strtok_r(pair, "=", &saveptr2);
        char *value = strtok_r(NULL, "=", &saveptr2);
        if (key && value) {
            printf("%s: %s\n", key, value);
        }
        pair = strtok_r(NULL, "&", &saveptr1);
    }
    free(query);

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