#include <stdio.h>
#include <stdbool.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

struct point {
    int row, col;
} queue[MAX_ROW * MAX_COL];

int head = 0, tail = 0;

void enqueue(struct point p) {
    queue[tail++] = p;
}

struct point dequeue(void) {
    return queue[head++];
}

bool is_empty(void) {
    return head == tail;
}

struct point predecessor[MAX_ROW][MAX_COL] = {
    {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
    {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
    {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
    {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
    {{-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}},
};

void visit(int row, int col, struct point p) {
    struct point visit_p = {row, col};
    maze[row][col] = 2;
    predecessor[row][col] = p;
    enqueue(visit_p);
}

int main(void)
{
    struct point p = {0, 0};
    maze[p.row][p.col] = 2;
    enqueue(p);

    while (!is_empty()) {
        p = dequeue();
        if (p.row == MAX_ROW - 1 && p.col == MAX_COL - 1)
            break;
        if (p.col + 1 < MAX_COL && maze[p.row][p.col + 1] == 0)
            visit(p.row, p.col + 1, p);
        if (p.row + 1 < MAX_ROW && maze[p.row + 1][p.col] == 0)
            visit(p.row + 1, p.col, p);
        if (p.col - 1 >= 0 && maze[p.row][p.col - 1] == 0)
            visit(p.row, p.col - 1, p);
        if (p.row - 1 >= 0 && maze[p.row - 1][p.col] == 0)
            visit(p.row - 1, p.col, p);
    }

    if (p.row == MAX_ROW - 1 && p.col == MAX_COL - 1) {
        printf("(%d, %d)\n", p.row, p.col);
        while (predecessor[p.row][p.col].row != -1) {
            p = predecessor[p.row][p.col];
            printf("(%d, %d)\n", p.row, p.col);
        }
    } else {
        printf("No path!\n");
    }

	return 0;
}