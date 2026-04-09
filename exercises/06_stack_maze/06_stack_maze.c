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
} Point;

// 定义栈结构
typedef struct {
    Point data[MAX_ROW * MAX_COL];
    int top;
} Stack;

// 栈操作函数
void initStack(Stack* s) {
    s->top = -1;
}

bool isEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, Point p) {
    s->data[++(s->top)] = p;
}

Point pop(Stack* s) {
    return s->data[(s->top)--];
}

Stack path;
int dir[4][2] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}}; // up, left, down, right
//bool finish = false;

bool is_valid(Point *p) {
    return ((p->row >= 0) && (p->col >= 0)
            && (p->row < MAX_ROW) && (p->col < MAX_COL)
            && (maze[p->row][p->col] == 0));
}

bool dfs(Point *p) {
    if (!is_valid(p)) {
        return false;
    }

    maze[p->row][p->col] = 2;
    push(&path, *p);

    if (p->row == (MAX_ROW - 1) && p->col == (MAX_COL - 1)) {
        return true;
    }

    for (int i = 0; i < 4; i++) {
        Point next_p;
        next_p.row = p->row + dir[i][0];
        next_p.col = p->col + dir[i][1];
        if (true == dfs(&next_p)) {
            return true;
        }
    }

    pop(&path);
    maze[p->row][p->col] = 0;

    return false;
}

void print_path(void) {
    while(!isEmpty(&path)) {
        Point p = pop(&path);
        printf("(%d,%d)\n", p.row, p.col);
    }
}

int main(void)
{
	// TODO: 在这里添加你的代码
    initStack(&path);

    Point start = {0, 0};
    if (true == dfs(&start)) {
        print_path();
    } else {
        printf("No path\n");
    }

	return 0;
}
