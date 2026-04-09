#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[20];
    int score;
} Student;

void insertion_sort(Student students[], int n) {
    Student swap_stu;
    int i, j;
    for (i = 1; i < n; i++) {
        j = i;
        memcpy(swap_stu.name, students[i].name, 20);
        swap_stu.score = students[i].score;
        while ((j > 0) && (swap_stu.score > students[j-1].score)) {
            memcpy(students[j].name, students[j-1].name, 20);
            students[j].score = students[j-1].score;
            j--;
        }
        memcpy(students[j].name, swap_stu.name, 20);
        students[j].score = swap_stu.score;
    }
}

int main(void) {
    FILE *file;
    Student students[50];
    int n = 0;
    
    // 打开文件（从命令行参数获取文件名）
    file = fopen("01_students.txt", "r");
    if (!file) {
        printf("错误：无法打开文件 01_students.txt\n");
        return 1;
    }
    
    // 从文件读取学生信息
    while (n < 50 && fscanf(file, "%s %d", students[n].name, &students[n].score) == 2) {
        n++;
    }
    fclose(file);
    
    if (n == 0) {
        printf("文件中没有学生信息\n");
        return 1;
    }
    
    insertion_sort(students, n);
    
    printf("\n按成绩从高到低排序后的学生信息:\n");
    for (int i = 0; i < n; i++) {
        printf("%s %d\n", students[i].name, students[i].score);
    }
    
    return 0;
}

