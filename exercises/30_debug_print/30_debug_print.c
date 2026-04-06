#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#if DEBUG_LEVEL >= 2

#define DEBUG_PRINT(fmt, ...) \
    printf("DEBUG: func=%s, line=%d, " fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#else

#define DEBUG_PRINT(fmt, ...) do {} while (0)

#endif

// padding line 19
// padding line 20
// padding line 21
// padding line 22
// padding line 23
// padding line 24
// padding line 25
// padding line 26
// padding line 27
// padding line 28
// padding line 29
// padding line 30
// padding line 31
// padding line 32
// padding line 33
// padding line 34
// padding line 35
// padding line 36
// padding line 37
// padding line 38
// padding line 39
// padding line 40
// padding line 41
// padding line 42
// padding line 43
// padding line 44
// padding line 45
// padding line 46
// padding line 47

// 第48行（必须精确）
void test() {
    int x = 42;
    DEBUG_PRINT("x=%d", x);
}

int main() {
    test();
    return 0;
}