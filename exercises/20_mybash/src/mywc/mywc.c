#include "mywc.h"

// 创建哈希表
WordCount **wc_create_hash_table() {
  WordCount **hash_table = calloc(HASH_SIZE, sizeof(WordCount *));
  return hash_table;
}

// 简单的哈希函数
unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % HASH_SIZE;
}

// 检查字符是否构成单词的一部分
bool is_valid_word_char(char c) { return isalpha(c) || c == '\''; }

// 转换为小写
char to_lower(char c) { return tolower(c); }

// 添加单词到哈希表
void add_word(WordCount **hash_table, const char *word) {
    unsigned int index = hash(word);
    WordCount *entry = hash_table[index];

    // TODO: 在这里添加你的代码
    while (entry != NULL) {
        if (strcmp(entry->word, word) == 0) {
            // 找到相同的单词，增加计数
            entry->count++;
            return;
        }
        entry = entry->next;
    }

    // 单词不存在，创建新节点
    WordCount *new_node = (WordCount *)malloc(sizeof(WordCount));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // 复制单词（注意：word是数组，不是指针，所以用strcpy）
    strncpy(new_node->word, word, MAX_WORD_LEN - 1);
    new_node->word[MAX_WORD_LEN - 1] = '\0'; // 确保以空字符结尾

    // 初始化计数
    new_node->count = 1;

    // 使用头插法插入到链表（更高效）
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
}

// 打印单词统计结果
void print_word_counts(WordCount **hash_table) {
    printf("Word Count Statistics:\n");
    printf("======================\n");

    // TODO: 在这里添加你的代码
    int total_words = 0;
    int unique_words = 0;

    // 遍历整个哈希表
    for (int i = 0; i < HASH_SIZE; i++) {
        WordCount *current = hash_table[i];

        // 遍历当前桶的链表
        while (current != NULL) {
            printf("%-20s %d\n", current->word, current->count);
            total_words += current->count;
            unique_words++;
            current = current->next;
        }
    }
}

// 释放哈希表内存
void wc_free_hash_table(WordCount **hash_table) {
  for (int i = 0; i < HASH_SIZE; i++) {
    WordCount *entry = hash_table[i];
    while (entry != NULL) {
      WordCount *temp = entry;
      entry = entry->next;
      free(temp);
    }
  }
  free(hash_table);
}

// 处理文件并统计单词
void process_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  WordCount **hash_table = wc_create_hash_table();
  char word[MAX_WORD_LEN];
  int word_pos = 0;
  int c;

  while ((c = fgetc(file)) != EOF) {
    if (is_valid_word_char(c)) {
      if (word_pos < MAX_WORD_LEN - 1) {
        word[word_pos++] = to_lower(c);
      }
    } else {
      if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
        word_pos = 0;
      }
    }
  }

  // 处理文件末尾的最后一个单词
  if (word_pos > 0) {
    word[word_pos] = '\0';
    add_word(hash_table, word);
  }

  fclose(file);
  print_word_counts(hash_table);
  wc_free_hash_table(hash_table);
}

int __cmd_mywc(const char* filename) {
  process_file(filename);
  return 0;
}
