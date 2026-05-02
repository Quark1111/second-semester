#include "avl_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    struct Node* left;
    struct Node* right;
    int height;
    char* key;
    char* fullName;
} Node;

struct AvlTree {
    Node* root;
    int size;
};

AvlTree* createTree(void)
{
    return calloc(1, sizeof(AvlTree));
}

char* find(const AvlTree* tree, const char* key)
{
    Node* node = tree->root;
    while (node != NULL) {
        int result = strcmp(node->key, key);
        if (result > 0) {
            node = node->left;
        } else if (result < 0) {
            node = node->right;
        } else {
            return node->fullName;
        }
    }
    return NULL;
}

static void split(char* str, char** key, char** fullName)
{
    size_t length = strlen(str);
    for (size_t i = 0; i < length; i++) {
        if (str[i] == ':') {
            *key = malloc((i + 1) * sizeof(char));
            *fullName = malloc((length - i) * sizeof(char));
            if (*key != NULL && *fullName != NULL) {
                strncpy(*key, str, i);
                (*key)[i] = '\0';
                strncpy(*fullName, str + i + 1, length - i - 1);
                (*fullName)[length - i - 1] = '\0';
            }
            break;
        }
    }
}

int getSize(const AvlTree* tree)
{
    if (tree == NULL) {
        return 0;
    }
    return tree->size;
}

static void saveNode(FILE* file, Node* node)
{
    if (node == NULL) {
        return;
    }
    saveNode(file, node->left);
    fprintf(file, "%s:%s\n", node->key, node->fullName);
    saveNode(file, node->right);
}

void save(AvlTree* tree, char* output)
{
    if (tree == NULL || tree->root == NULL) {
        return;
    }
    FILE* file = fopen(output, "w");
    if (file == NULL) {
        return;
    }
    saveNode(file, tree->root);
    fclose(file);
}

static int getHeight(Node* node)
{
    if (node == NULL) {
        return -1;
    }
    return node->height;
}

static void updateNode(Node* node)
{
    int left = getHeight(node->left);
    int right = getHeight(node->right);
    node->height = (left > right ? left : right) + 1;
}

static Node* rotateLeft(Node* node)
{
    Node* rightNode = node->right;
    node->right = rightNode->left;
    rightNode->left = node;
    updateNode(node);
    updateNode(rightNode);
    return rightNode;
}

static Node* rotateRight(Node* node)
{
    Node* leftNode = node->left;
    node->left = leftNode->right;
    leftNode->right = node;
    updateNode(node);
    updateNode(leftNode);
    return leftNode;
}

static Node* bigRotateLeft(Node* node)
{
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

static Node* bigRotateRight(Node* node)
{
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

static Node* balanceNode(Node* node)
{
    if (node == NULL) {
        return NULL;
    }
    int balance = getHeight(node->left) - getHeight(node->right);
    if (balance == 2) {
        if (getHeight(node->left->left) - getHeight(node->left->right) >= 0) {
            return rotateRight(node);
        }
        return bigRotateRight(node);
    } else if (balance == -2) {
        if (getHeight(node->right->left) - getHeight(node->right->right) <= 0) {
            return rotateLeft(node);
        }
        return bigRotateLeft(node);
    }
    return node;
}

static Node* insertNode(Node* node, const char* key, char* fullName, bool* unique)
{
    if (node == NULL) {
        Node* newNode = calloc(1, sizeof(Node));
        newNode->key = (char*)key;
        newNode->fullName = fullName;
        return newNode;
    }
    int result = strcmp(node->key, key);
    if (result > 0) {
        node->left = insertNode(node->left, key, fullName, unique);
    } else if (result < 0) {
        node->right = insertNode(node->right, key, fullName, unique);
    } else {
        *unique = false;
    }
    if (*unique) {
        updateNode(node);
        return balanceNode(node);
    }
    return node;
}

int insert(AvlTree* tree, char* str)
{
    if (tree == NULL) {
        return -1;
    }
    char* key = NULL;
    char* fullName = NULL;
    split(str, &key, &fullName);
    bool unique = true;
    if (key == NULL || fullName == NULL) {
        free(key);
        free(fullName);
        return -1;
    }
    tree->root = insertNode(tree->root, key, fullName, &unique);
    if (unique) {
        tree->size++;
        return 0;
    }
    free(key);
    free(fullName);
    return -1;
}

static Node* deleteNode(Node* node, const char* key, bool* unique)
{
    if (node == NULL) {
        return NULL;
    }
    int compare = strcmp(node->key, key);
    if (compare > 0) {
        node->left = deleteNode(node->left, key, unique);
    } else if (compare < 0) {
        node->right = deleteNode(node->right, key, unique);
    } else {
        if (node->left == NULL && node->right == NULL) {
            free(node->key);
            free(node->fullName);
            free(node);
            *unique = true;
            return NULL;
        }
        if (node->left == NULL) {
            Node* newNode = node->right;
            free(node->key);
            free(node->fullName);
            free(node);
            *unique = true;
            return newNode;
        }
        if (node->right == NULL) {
            Node* newNode = node->left;
            free(node->key);
            free(node->fullName);
            free(node);
            *unique = true;
            return newNode;
        }
        Node* newNode = node->right;
        while (newNode->left != NULL) {
            newNode = newNode->left;
        }
        size_t keyLength = strlen(newNode->key) + 1;
        size_t nameLength = strlen(newNode->fullName) + 1;
        char* newKey = malloc(keyLength);
        char* newFullName = malloc(nameLength);
        if (newKey == NULL || newFullName == NULL) {
            free(newKey);
            free(newFullName);
            return node;
        }
        free(node->key);
        free(node->fullName);
        node->key = newKey;
        node->fullName = newFullName;
        memcpy(node->key, newNode->key, keyLength);
        memcpy(node->fullName, newNode->fullName, nameLength);
        node->right = deleteNode(node->right, node->key, unique);
    }
    updateNode(node);
    return balanceNode(node);
}

void delete(AvlTree* tree, const char* key)
{
    if (tree == NULL || tree->root == NULL) {
        return;
    }
    bool unique = false;
    tree->root = deleteNode(tree->root, key, &unique);
    if (unique) {
        tree->size--;
    }
}

static Node* deleteAllNodes(Node* node)
{
    if (node == NULL) {
        return NULL;
    }
    node->left = deleteAllNodes(node->left);
    node->right = deleteAllNodes(node->right);
    free(node->key);
    free(node->fullName);
    free(node);
    return NULL;
}

void deleteTree(AvlTree* tree)
{
    if (tree == NULL || tree->root == NULL) {
        return;
    }
    deleteAllNodes(tree->root);
    free(tree);
}
