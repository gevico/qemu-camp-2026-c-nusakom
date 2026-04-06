#include <stdio.h>

int main() {
    char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
    char destination[101];
    
    int i = 0;
    while (source[i] != '\0' && i < 100) {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
    
    printf("拷贝后的字符串: %s", destination);
    
    return 0;
}