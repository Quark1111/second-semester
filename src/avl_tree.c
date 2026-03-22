#include "avl_tree.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AvlTree* createTree()
{
    AvlTree* tree = malloc(sizeof(AvlTree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

char* find(Node* node, char* key)
{
    if (node == NULL) {
        return NULL;
    }
    int res = strcmp(node->key, key);
    if (res > 0) {
        return find(node->left, key);
    } else if (res < 0){
        return find(node->right, key);
    }
    return node->fullname;
}

static void split(char* str, char** key, char** fullname)
{
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == ':') {
            *key = malloc((i + 1) * sizeof(char));
            *fullname = malloc((len - i) * sizeof(char));
            if (*key != NULL && *fullname != NULL) {
                strncpy(*key, str, i);
                (*key)[i] = '\0';
                strncpy(*fullname, str + i + 1, len - i - 1);
                (*fullname)[len - i - 1] = '\0';
            }
            break;
        }
    }
}

static void saveNode(FILE* file, Node* node) {
    if (node == NULL) {
        return ;
    }
    saveNode(file, node->left);
    fprintf(file, "%s:%s\n", node->key, node->fullname);
    saveNode(file, node->right);
}

void save(AvlTree* tree, char* output)
{
    if (tree == NULL || tree->root == NULL) {
        return ;
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
    } else {
        return node->height;
    }
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

static Node* bigrotateLeft(Node* node)
{
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

static Node* bigrotateRight(Node* node)
{
    node->left = rotateLeft(node->left);
    return rotateRight(node);

}

static Node* balanceNode(Node* node)
{
    int balance = getHeight(node->left) - getHeight(node->right);
    if (balance == 2) {
        if (getHeight(node->left->left) - getHeight(node->left->right) >= 0) {
            return rotateRight(node);
        }
        return bigrotateRight(node);
    } else if (balance == -2) {
        if (getHeight(node->right->left) - getHeight(node->right->right) <= 0) {
            return rotateLeft(node);
        }
        return bigrotateLeft(node);
    }
    return node;
}

static Node* insertNode(Node* node, char* key, char* fullname, bool* unique)
{
    if (node == NULL) {
        Node* newnode = calloc(1, sizeof(Node));
        newnode->left = NULL;
        newnode->right = NULL;
        newnode->height = 0;
        newnode->key = key;
        newnode->fullname = fullname;
        return newnode;
    }
    int res = strcmp(node->key, key);
    if (res > 0) {
        node->left = insertNode(node->left, key, fullname, unique);
    } else if (res < 0) {
        node->right = insertNode(node->right, key, fullname, unique);
    } else {
        *unique = false;
    }
    if (*unique) {
        updateNode(node);
        return balanceNode(node);
    }
    return node;
}

void insert(AvlTree* tree, char* str)
{
    if (tree == NULL) {
        return ;
    }
    char* key = NULL;
    char* fullname = NULL;
    split(str, &key, &fullname);
    bool unique = true;
    if (key == NULL || fullname == NULL) {
        free(key);
        free(fullname);
        return ;
    }
    tree->root = insertNode(tree->root, key, fullname, &unique);
    if (unique) {
        tree->size++;
    } else {
        free(key);
        free(fullname);
    }
}

static Node* deleteNode(Node* node, char* key, bool* unique)
{
    if (node != NULL) {
        int cmp = strcmp(node->key, key);
        if (cmp > 0) {
            node->left = deleteNode(node->left, key, unique);
        } else if (cmp < 0) {
            node->right = deleteNode(node->right, key, unique);
        } else {

            if (node->left == NULL && node->right == NULL) {
                free(node->key);
                free(node->fullname);
                free(node);
                *unique = true;
                return NULL;   
            }

            if (node->left == NULL) {
                Node* newnode = node->right;
                free(node->key);
                free(node->fullname);
                free(node);
                *unique = true;
                return newnode;
            }

            if (node->right == NULL) {
                Node* newnode = node->left;
                free(node->key);
                free(node->fullname);
                free(node);
                *unique = true;
                return newnode;
            }
            
            Node* newnode = node->right;
            while (newnode->left != NULL) {
                newnode = newnode->left;
            }

            char* newKey = malloc(strlen(newnode->key) + 1);
            char* newFullname = malloc(strlen(newnode->fullname) + 1);

            if (newKey == NULL || newFullname == NULL) {
                free(newKey);
                free(newFullname);
                return node;
            }

            free(node->key);
            free(node->fullname);
            node->key = newKey;
            node->fullname = newFullname;
            strcpy(node->key, newnode->key);
            strcpy(node->fullname, newnode->fullname);
            node->right = deleteNode(node->right, node->key, unique);
        }
        updateNode(node);
        return balanceNode(node);
    }
    return node;
}

void delete(AvlTree* tree, char* key)
{
    if (tree == NULL || tree->root == NULL) {
        return ;
    }
    bool unique = false;
    tree->root = deleteNode(tree->root, key, &unique);
    if (unique) {
        tree->size--;
    }
}

static Node* deleteAllNode(Node* node)
{
    if (node == NULL) {
        return NULL;
    }
    node->left = deleteAllNode(node->left);    
    node->right = deleteAllNode(node->right);
    free(node->key);
    free(node->fullname);
    free(node);
    return NULL;

}

void quit(AvlTree* tree)
{
    if (tree == NULL || tree->root == NULL) {
        return;
    }
    deleteAllNode(tree->root);
    free(tree);
}
