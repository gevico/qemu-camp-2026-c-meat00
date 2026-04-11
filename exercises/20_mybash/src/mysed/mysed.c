#include "mysed.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    // 检查输入参数有效性
    if (!cmd || !old_str || !new_str) {
        return -1;
    }

    // 初始化输出参数
    *old_str = NULL;
    *new_str = NULL;

    // TODO: 在这里添加你的代码
    // 查找第二个'/'的位置
    const char* slash_2nd = strchr(cmd + 2, '/');
    if (slash_2nd == NULL) {
        return -1;  // 格式错误
    }

    // 查找第三个'/'的位置
    const char* slash_3rd = strchr(slash_2nd + 1, '/');
    if (slash_3rd == NULL) {
        return -1;  // 格式错误
    }

    // 计算old_str的长度并分配内存
    int old_len = slash_2nd - (cmd + 2);
    *old_str = (char*)malloc(old_len + 1);
    if (*old_str == NULL) {
        return -1;  // 内存分配失败
    }
    strncpy(*old_str, cmd + 2, old_len);
    (*old_str)[old_len] = '\0';

    // 计算new_str的长度并分配内存
    int new_len = slash_3rd - (slash_2nd + 1);
    *new_str = (char*)malloc(new_len + 1);
    if (*new_str == NULL) {
        free(*old_str);
        *old_str = NULL;
        return -1;  // 内存分配失败
    }
    strncpy(*new_str, slash_2nd + 1, new_len);
    (*new_str)[new_len] = '\0';

    return 0;
}

void replace_first_occurrence(char* str, const char* old, const char* new) {
    // 检查输入参数有效性
    if (!str || !old || !new) {
        return;
    }
    
    // TODO: 在这里添加你的代码
    // 查找旧字符串第一次出现的位置
    char* pos = strstr(str, old);
    if (pos == NULL) {
        return;  // 没有找到，直接返回
    }

    int old_len = strlen(old);
    int new_len = strlen(new);
    int str_len = strlen(str);

    // 如果长度相等，不需要移动
    if (new_len != old_len) {
        int move_len = str_len - (pos - str) - old_len;
        memmove(pos + new_len, pos + old_len, move_len + 1);  // +1 包含结束符'\0'
    }

    // 复制新字符串到目标位置
    strncpy(pos, new, new_len);
}

int __cmd_mysed(const char* rules, const char* str) {
    // 检查输入参数有效性
    if (!rules || !str) {
        fprintf(stderr, "Error: NULL rules or str parameter\n");
        return 1;
    }

    printf("rules: %s\n", rules);
    printf("str: %s\n", str);

    char* old_str = NULL;
    char* new_str = NULL;

    // 解析规则，例如 "s/old/new/"
    if (parse_replace_command(rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }

    if (!old_str || !new_str) {
        fprintf(stderr, "Error: Failed to parse old/new strings from rules\n");
        free(old_str);
        free(new_str);
        return 1;
    }

    // 复制原始字符串，因为我们可能会修改它（避免修改输入参数）
    char line[1024];
    strncpy(line, str, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';  // 确保终止

    // 执行替换：在 line 中把 old_str 替换为 new_str（第一次出现）
    replace_first_occurrence(line, old_str, new_str);

    // 输出结果（建议加上换行，除非 str 本身带了）
    printf("%s\n", line);

    // 释放动态分配的内存
    free(old_str);
    free(new_str);

    return 0;
}
