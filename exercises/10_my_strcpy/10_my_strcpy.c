#include <stdio.h>

int main() {
    char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
    char destination[101];
    
    char *src = source;
    char *dest = destination;
    while ((*dest++ = *src++) != '\0')
        ;
    
    printf("拷贝后的字符串: %s", destination);
    
    return 0;
}