#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
    0, 1, 0, 0, 0,
    0, 1, 0, 1, 0,
    0, 0, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
};

int visited[MAX_ROW][MAX_COL] = {0};

int dirs[4][2] = {
    {1, 0},
    {0, 1},
    {-1, 0},
    {0, -1}
};

typedef struct {
    int x, y;
} Point;

Point queue[MAX_ROW * MAX_COL];
int head = 0, tail = 0;

void enqueue(Point p) {
    queue[tail++] = p;
}

Point dequeue() {
    return queue[head++];
}

int is_empty() {
    return head == tail;
}

int solve_maze(int start_x, int start_y) {
    Point start = {start_x, start_y};
    enqueue(start);
    visited[start_x][start_y] = 1;

    while (!is_empty()) {
        Point curr = dequeue();

        if (curr.x == MAX_ROW - 1 && curr.y == MAX_COL - 1)
            return 1;

        for (int i = 0; i < 4; i++) {
            int nx = curr.x + dirs[i][0];
            int ny = curr.y + dirs[i][1];

            if (nx >= 0 && nx < MAX_ROW && ny >= 0 && ny < MAX_COL &&
                maze[nx][ny] == 0 && !visited[nx][ny]) {
                visited[nx][ny] = 1;
                enqueue((Point){nx, ny});
            }
        }
    }
    return 0;
}

int main(void)
{
    if (solve_maze(0, 0)) {
        printf("存在路径！\n\n");
    } else {
        printf("不存在路径！\n\n");
    }

    printf("路径访问标记：\n");
    for (int i = 0; i < MAX_ROW; i++) {
        for (int j = 0; j < MAX_COL; j++) {
            printf("%d ", visited[i][j]);
        }
        printf("\n");
    }

    return 0;
}