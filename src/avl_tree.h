typedef struct Node {
    struct Node* left;
    struct Node* right;
    int height;
    char* key;
    char* fullname;
} Node;

typedef struct AvlTree {
    Node* root;
    int size;
} AvlTree;

AvlTree* createTree();
void insert(AvlTree* tree, char* str);
void delete(AvlTree* tree, char* key);
char* find(Node* node, char* key);
void save(AvlTree* tree, char* output);
void quit(AvlTree* tree);