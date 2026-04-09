#include <stdio.h>
#include <stdbool.h>

int main() {
    char str[]="Don't ask what your country can do for you, but ask what you can do for your country.";
    int wordCount = 0;

    if (NULL != str) {
        int i = 0;
        bool inword = false;
        while (str[i] != '\0') {
            if (str[i] == ' ' || str[i] == '\t') {
                inword = false;
            } else {
                if (false == inword) {
                    wordCount++;
                    inword = true;
                }
            }
            i++;
        }
    }

    printf("单词数量: %d\n", wordCount);

    return 0;
}
