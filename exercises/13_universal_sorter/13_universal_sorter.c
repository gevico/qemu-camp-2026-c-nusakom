#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*CompareFunc)(const void *, const void *);

int compareInt(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int compareFloat(const void *a, const void *b) {
    float diff = (*(float*)a - *(float*)b);
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

int compareString(const void *a, const void *b) {
    return strcmp(*(char**)a, *(char**)b);
}

void universal_sort(void *base, int n, int size, int (*compare)(const void *, const void *)) {
    char *p = (char *)base;
    char *temp = (char *)malloc(size);
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (compare(p + j * size, p + (j + 1) * size) > 0) {
                memcpy(temp, p + j * size, size);
                memcpy(p + j * size, p + (j + 1) * size, size);
                memcpy(p + (j + 1) * size, temp, size);
            }
        }
    }
    free(temp);
}

void processFile(const char *filename) {
    FILE *fin = fopen(filename, "r");
    if (!fin) {
        printf("错误: 无法打开文件 %s\n", filename);
        return;
    }

    int choice, n;
    if (fscanf(fin, "%d", &choice) != 1 || fscanf(fin, "%d", &n) != 1) {
        printf("错误: 文件 %s 格式不正确\n", filename);
        fclose(fin);
        return;
    }

    if (n > 20) n = 20;  // 最多支持20个元素

    printf("=== 处理数据来自: %s ===\n", filename);

    switch (choice) {
        case 1: {
            int arr[20];
            for (int i = 0; i < n; i++) fscanf(fin, "%d", &arr[i]);
            universal_sort(arr, n, sizeof(int), compareInt);
            printf("整数排序结果: ");
            for (int i = 0; i < n; i++) printf("%d ", arr[i]);
            printf("\n");
            break;
        }
        case 2: {
            float arr[20];
            for (int i = 0; i < n; i++) fscanf(fin, "%f", &arr[i]);
            universal_sort(arr, n, sizeof(float), compareFloat);
            printf("浮点数排序结果: ");
            for (int i = 0; i < n; i++) printf("%.2f ", arr[i]);
            printf("\n");
            break;
        }
        default:
            printf("错误: 未知类型 %d\n", choice);
            break;
    }

    fclose(fin);
}

int main() {
    processFile("int_sort.txt");
    processFile("float_sort.txt");

    return 0;
}