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

// 定义坐标结构体
typedef struct {
    int row;
    int col;
    int pre;
} Point;

// 定义队列结构
typedef struct {
    Point data[MAX_ROW * MAX_COL];
    int front;
    int rear;
} Queue;

// 队列操作函数
void initQueue(Queue* q) {
    q->front = 0;
    q->rear = 0;
}

bool isEmpty(Queue* q) {
    return q->front == q->rear;
}

bool isFull(Queue* q) {
    return (q->rear + 1) % (MAX_ROW * MAX_COL) == q->front;
}

void enqueue(Queue* q, Point p) {
    if (!isFull(q)) {
        q->data[q->rear] = p;
        q->rear = (q->rear + 1) % (MAX_ROW * MAX_COL);
    }
}

Point dequeue(Queue* q) {
    Point p = q->data[q->front];
    q->front = (q->front + 1) % (MAX_ROW * MAX_COL);
    return p;
}

Queue path;
int dir[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // up, left, down, right

bool is_valid(Point *p) {
    return ((p->row >= 0) && (p->col >= 0)
            && (p->row < MAX_ROW) && (p->col < MAX_COL)
            && (maze[p->row][p->col] == 0));
}

bool bfs(Point *p) {
    if (!is_valid(p)) {
        return false;
    }

    maze[p->row][p->col] = 2;
    enqueue(&path, *p);

    Point cur_p;
    while (!isEmpty(&path)) {
        cur_p = dequeue(&path);
        if (cur_p.row == (MAX_ROW - 1) && cur_p.col == (MAX_COL - 1)) {
            break;
        }
        for (int i = 0; i < 4; i++) {
            Point next_p;
            next_p.row = cur_p.row + dir[i][0];
            next_p.col = cur_p.col + dir[i][1];
            next_p.pre = path.front - 1;
            if (is_valid(&next_p)) {
                maze[next_p.row][next_p.col] = 2;
                enqueue(&path, next_p);
            }

        }
    }

    if (cur_p.row == (MAX_ROW - 1) && cur_p.col == (MAX_COL - 1)) {
        printf("(%d,%d)\n", cur_p.row, cur_p.col);
        while (cur_p.pre != -1)
        {
            cur_p = path.data[cur_p.pre];
            printf("(%d,%d)\n", cur_p.row, cur_p.col);
        }
        return true;
    }

    return false;
}

int main(void)
{
	// TODO: 在这里添加你的代码
    initQueue(&path);

    Point start = {0, 0, -1};
    if (false == bfs(&start)) {
        printf("No path!\n");
    }

	return 0;
}
