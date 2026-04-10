#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char* cmd, char** old_str, char** new_str) {
    if (cmd[0] != 's' || cmd[1] != '/') {
        return -1;
    }

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

int main(int argc, char* argv[]) {
    const char* replcae_rules = "s/unix/linux/";

    char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

    char* old_str = NULL;
    char* new_str = NULL;

    if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
        fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
        return 1;
    }


    replace_first_occurrence(line, old_str, new_str);
    fputs(line, stdout);

    free(old_str);
    free(new_str);
    return 0;
}
