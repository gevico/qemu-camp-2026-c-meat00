#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 16 LRU 缓存淘汰算法（哈希表 + 双向链表）
 *  - put(k,v)、get(k) 均为 O(1)
 *  - 容量满时淘汰最久未使用（LRU）的元素
 */

typedef struct LRUNode {
    int key;
    int value;
    struct LRUNode* prev;
    struct LRUNode* next;
} LRUNode;

typedef struct HashEntry {
    int key;
    LRUNode* node;
    struct HashEntry* next;
} HashEntry;

typedef struct {
    int capacity;
    int size;
    LRUNode* head; /* 最近使用（MRU） */
    LRUNode* tail; /* 最久未使用（LRU） */
    /* 简单链式哈希表 */
    size_t bucket_count;
    HashEntry** buckets;
} LRUCache;

static unsigned hash_int(int key, size_t bucket_count) {
    return (unsigned)(key * 31) % bucket_count;
}

static HashEntry* hash_find(LRUCache* c, int key, HashEntry*** pprev_next) {
    size_t idx = hash_int(key, c->bucket_count);
    HashEntry* entry = c->buckets[idx];
    HashEntry* prev = NULL;
    while (entry) {
        if (entry->key == key) {
            *pprev_next = prev;
            return entry;
        }
        prev = entry;
        entry = entry->next;
    }
    *pprev_next = prev;
    return NULL;
}

static void list_add_to_head(LRUCache* c, LRUNode* node) {
    node->next = c->head;
    node->prev = NULL;
    if (c->head) {
        c->head->prev = node;
    }
    c->head = node;
    if (!c->tail) {
        c->tail = node;
    }
}

static void list_remove(LRUCache* c, LRUNode* node) {
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        c->head = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        c->tail = node->prev;
    }
}

static void list_move_to_head(LRUCache* c, LRUNode* node) {
    if (node == c->head) return;
    list_remove(c, node);
    list_add_to_head(c, node);
}

static LRUNode* list_pop_tail(LRUCache* c) {
    if (!c->tail) return NULL;
    LRUNode* node = c->tail;
    list_remove(c, node);
    return node;
}

/* LRU 接口实现 */
static LRUCache* lru_create(int capacity) {
    LRUCache* c = (LRUCache*)malloc(sizeof(LRUCache));
    if (!c) return NULL;
    c->capacity = capacity;
    c->size = 0;
    c->head = NULL;
    c->tail = NULL;
    c->bucket_count = capacity * 2;
    c->buckets = (HashEntry**)calloc(c->bucket_count, sizeof(HashEntry*));
    if (!c->buckets) {
        free(c);
        return NULL;
    }
    return c;
}

static void lru_free(LRUCache* c) {
    if (!c) return;
    LRUNode* p = c->head;
    while (p) {
        LRUNode* next = p->next;
        free(p);
        p = next;
    }
    free(c->buckets);
    free(c);
}

static int lru_get(LRUCache* c, int key, int* out_value) {
    HashEntry** prev_next = NULL;
    HashEntry* entry = hash_find(c, key, &prev_next);
    if (!entry) return 0;
    *out_value = entry->node->value;
    list_move_to_head(c, entry->node);
    return 1;
}

static void lru_put(LRUCache* c, int key, int value) {
    HashEntry** prev_next = NULL;
    HashEntry* entry = hash_find(c, key, &prev_next);
    if (entry) {
        entry->node->value = value;
        list_move_to_head(c, entry->node);
        return;
    }
    LRUNode* node = (LRUNode*)malloc(sizeof(LRUNode));
    if (!node) return;
    node->key = key;
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    list_add_to_head(c, node);
    size_t idx = hash_int(key, c->bucket_count);
    HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
    if (!new_entry) {
        free(node);
        return;
    }
    new_entry->key = key;
    new_entry->node = node;
    new_entry->next = c->buckets[idx];
    c->buckets[idx] = new_entry;
    c->size++;
    if ((int)c->size > c->capacity) {
        LRUNode* tail = list_pop_tail(c);
        if (tail) {
            HashEntry** pprev = NULL;
            HashEntry* he = hash_find(c, tail->key, &pprev);
            if (he) {
                if (pprev) {
                    pprev[0] = he->next;
                } else {
                    size_t i = hash_int(tail->key, c->bucket_count);
                    c->buckets[i] = he->next;
                }
                free(he);
            }
            free(tail);
            c->size--;
        }
    }
}

/* 打印当前缓存内容（从头到尾） */
static void lru_print(LRUCache* c) {
    LRUNode* p = c->head;
    int first = 1;
    while (p) {
        if (!first) printf(", ");
        first = 0;
        printf("%d:%d", p->key, p->value);
        p = p->next;
    }
    printf("\n");
}

int main(void) {
    /* 容量 3：put(1,1), put(2,2), put(3,3), put(4,4), get(2), put(5,5) */
    LRUCache* c = lru_create(3);
    if (!c) {
        fprintf(stderr, "创建 LRU 失败\n");
        return 1;
    }

    lru_put(c, 1, 1); /* 缓存：1 */
    lru_put(c, 2, 2); /* 缓存：2,1 */
    lru_put(c, 3, 3); /* 缓存：3,2,1 (满) */
    lru_put(c, 4, 4); /* 淘汰 LRU(1)，缓存：4,3,2 */

    int val;
    if (lru_get(c, 2, &val)) {
        /* 访问 2：缓存：2,4,3 */
        (void)val; /* 演示无需使用 */
    }

    lru_put(c, 5, 5); /* 淘汰 LRU(3)，缓存：5,2,4 */

    /* 期望最终键集合：{2,4,5}，顺序无关。此处按最近->最久打印：5:5, 2:2, 4:4 */
    lru_print(c);

    lru_free(c);
    return 0;
}
