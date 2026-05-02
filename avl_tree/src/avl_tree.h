#pragma once

typedef struct AvlTree AvlTree;

// Создаёт пустое АВЛ-дерево.
AvlTree* createTree(void);

// Добавляет элемент в дерево. Возвращает 0 при успехе, -1 при ошибке.
int insert(AvlTree* tree, char* str);

// Удаляет элемент из дерева по ключу.
void delete(AvlTree* tree, const char* key);

// Ищет значение по ключу.
char* find(const AvlTree* tree, const char* key);

// Сохраняет дерево в файл.
void save(AvlTree* tree, char* output);

// Удаляет дерево и освобождает память.
void deleteTree(AvlTree* tree);

// Возвращает количество элементов в дереве.
int getSize(const AvlTree* tree);
