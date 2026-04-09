#include <stdio.h>

int main() {
    char source[] = "Hello, World! This is a test string for my_strcpy function.\n";
    char destination[101];
    int i;

    for (i = 0; i < 100; i++) {
        if (source[i] == '\0') {
            destination[i] = '\0';
            break;
        }
        destination[i] = source[i];
    }
    destination[i] = '\0';

    printf("拷贝后的字符串: %s", destination);

    return 0;
}
