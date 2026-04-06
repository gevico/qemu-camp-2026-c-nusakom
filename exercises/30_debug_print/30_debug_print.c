#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG

#define DEBUG_PRINT(fmt, ...) \
    printf("[%s:%d %s] " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#else

#define DEBUG_PRINT(fmt, ...) do {} while (0)

#endif


// 为了满足 48 行的要求，增加一些空行
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//! MUST BE ENSURE THE DEBUG_PRINT("x=%d", x) AT THE 48 LINE

// 测试代码
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}
