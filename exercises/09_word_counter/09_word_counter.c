#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;
    bool inWord = false;

    for (int i = 0; str[i] != '\0'; i++) {
        if (isalnum(str[i]) || str[i] == '\'') {
            if (!inWord) {
                wordCount++;
                inWord = true;
            }
        } else {
            inWord = false;
        }
    }
    
    printf("单词数量: %d\n", wordCount);
    
    return 0;
}