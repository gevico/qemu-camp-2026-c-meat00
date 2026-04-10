#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/**
 * URL参数解析器
 * 输入：包含http/https超链接的字符串
 * 输出：解析出所有的key-value键值对，每行显示一个
 */

int parse_url(const char* url) {
    int err = 0;

    // TODO: 在这里添加你的代码
    char *p = strchr(url, '?');
    if (NULL == p) goto exit;

    char key[100];
    char val[100];
    int state = 0;
    int key_i = 0;
    int val_i = 0;
    memset(key, 0, 100);
    memset(val, 0, 100);
    p++;
    while(*p != '\0') {
        if (0 == state) {
            if (*p == '=') {
                key[key_i] = '\0';
                key_i = 0;
                state = 1;
            } else {
                key[key_i++] = *p;
            }
        } else {
            if (*p == '&') {
                val[val_i] = '\0';
                val_i = 0;
                state = 0;
                printf("key = %s, value = %s\n", key, val);
            } else {
                val[val_i++] = *p;
            }
        }
        p++;
    }
    printf("key = %s, value = %s\n", key, val);

exit:
    return err;
}

int main() {
    const char* test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

    printf("Parsing URL: %s\n", test_url);
    printf("Parameters:\n");

    parse_url(test_url);

    return 0;
}
