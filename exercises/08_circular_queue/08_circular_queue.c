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

// 初始化队列
void initQueue(Queue *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

// 判断队列是否为空
bool isEmpty(Queue *q) {
    return q->count == 0;
}

// 判断队列是否已满
bool isFull(Queue *q) {
    return q->count == MAX_PEOPLE;
}

// 入队
bool enqueue(Queue *q, People p) {
    if (isFull(q)) {
        return false;
    }
    q->data[q->tail] = p;
    q->tail = (q->tail + 1) % MAX_PEOPLE;
    q->count++;
    return true;
}

// 出队
bool dequeue(Queue *q, People *p) {
    if (isEmpty(q)) {
        return false;
    }
    if (p != NULL) {
        *p = q->data[q->head];
    }
    q->head = (q->head + 1) % MAX_PEOPLE;
    q->count--;
    return true;
}

// 获取队首元素
bool peek(Queue *q, People *p) {
    if (isEmpty(q)) {
        return false;
    }
    *p = q->data[q->head];
    return true;
}

int main() {
    Queue q;
    int total_people=50;
    int report_interval=5;

    initQueue(&q);
    for (int i = 1; i <= total_people; i++) {
        People p = {i};
        enqueue(&q, p);
    }

    int count = 0;
    while (q.count > 1) {
        People cur;
        dequeue(&q, &cur);
        count++;
        if (0 == (count % report_interval)) {
            printf("淘汰: %d\n", cur.id);
        } else {
            enqueue(&q, cur);
        }
    }

    printf("最后剩下的人是: %d\n", q.data[q.head].id);

    return 0;
}
