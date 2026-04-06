#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
    int id;
} People;

typedef struct {
    People data[MAX_PEOPLE];
    int head;
    int tail;
    int count;
} Queue;

int main() {
    Queue q;
    int total_people = 50;
    int report_interval = 5;

    q.head = 0;
    q.tail = 0;
    q.count = 0;

    // 初始化队列
    for (int i = 1; i <= total_people; i++) {
        q.data[q.tail].id = i;
        q.tail = (q.tail + 1) % MAX_PEOPLE;
        q.count++;
    }

    int counter = 0;
    while (q.count > 1) {
        People p = q.data[q.head];
        q.head = (q.head + 1) % MAX_PEOPLE;
        q.count--;
        
        counter++;
        if (counter == report_interval) {
            // 报数到间隔，出列（不放回）
            counter = 0;
        } else {
            // 未到间隔，放回队尾
            q.data[q.tail] = p;
            q.tail = (q.tail + 1) % MAX_PEOPLE;
            q.count++;
        }
    }
    
    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}