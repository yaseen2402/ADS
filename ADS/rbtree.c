#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILE_COUNT 4

// Structure for a Red-Black Tree Node
typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    Color color;
    struct Node *left, *right, *parent;
} Node;

typedef struct RedBlackTree {
    Node *root;
    Node *NIL; // Sentinel node for NIL
} RedBlackTree;

//Function prototypes
Node* createNode(int data, Color color, Node* NIL);
RedBlackTree* initializeTree();
void leftRotate(RedBlackTree *tree, Node *x);
void rightRotate(RedBlackTree *tree, Node *y);
void insertFixup(RedBlackTree *tree, Node *z);
void insert(RedBlackTree *tree, int data);
void transplant(RedBlackTree *tree, Node *u, Node *v);
Node* minimum(Node *node, Node* NIL);
void deleteFixup(RedBlackTree *tree, Node *x);
void deleteNode(RedBlackTree *tree, Node *z);
Node* search(RedBlackTree *tree, Node *node, int data);
void generateFiles();
void performOperations(const char *filename, RedBlackTree *tree);


// Main function
int main() {
    generateFiles();
    RedBlackTree *tree = initializeTree();

    const char *files[FILE_COUNT] = {"increasing.txt", "decreasing.txt", "mixed.txt", "random.txt"};
    for (int i = 0; i < FILE_COUNT; i++) {
        printf("\nProcessing file: %s\n", files[i]);
        performOperations(files[i], tree);
    }

    return 0;
}

// Create a new node
Node* createNode(int data, Color color, Node* NIL) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = color;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->parent = NIL;
    return newNode;
}

// Initialize the Red-Black Tree
RedBlackTree* initializeTree() {
    RedBlackTree *tree = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    tree->NIL = createNode(0, BLACK, NULL);
    tree->root = tree->NIL;
    return tree;
}

// Left rotate
void leftRotate(RedBlackTree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != tree->NIL)
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == tree->NIL)
        tree->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Right rotate
void rightRotate(RedBlackTree *tree, Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right != tree->NIL)
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == tree->NIL)
        tree->root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    x->right = y;
    y->parent = x;
}

// Insert fixup
void insertFixup(RedBlackTree *tree, Node *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            //y is uncle 
            Node *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

// Insert a node
void insert(RedBlackTree *tree, int data) {
    Node *z = createNode(data, RED, tree->NIL);
    Node *y = tree->NIL;
    Node *x = tree->root;

    while (x != tree->NIL) {
        y = x;
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == tree->NIL)
        tree->root = z;
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;

    insertFixup(tree, z);
}

// Transplant nodes
void transplant(RedBlackTree *tree, Node *u, Node *v) {
    if (u->parent == tree->NIL)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

// Find the minimum node
Node* minimum(Node *node, Node* NIL) {
    while (node->left != NIL)
        node = node->left;
    return node;
}

// Delete fixup
void deleteFixup(RedBlackTree *tree, Node *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Delete a node
void deleteNode(RedBlackTree *tree, Node *z) {
    Node *y = z;
    Node *x;
    Color yOriginalColor = y->color;

    if (z->left == tree->NIL) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == tree->NIL) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = minimum(z->right, tree->NIL);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK)
        deleteFixup(tree, x);
}

// Search for a node
Node* search(RedBlackTree *tree, Node *node, int data) {
    if (node == tree->NIL || data == node->data)
        return node;
    if (data < node->data)
        return search(tree, node->left, data);
    else
        return search(tree, node->right, data);
}

// Generate files
void generateFiles() {
    FILE *f;

    // Increasing
    f = fopen("increasing.txt", "w");
    for (int i = 1; i <= 1000; i++) {
        fprintf(f, "%d\n", i);
    }
    fclose(f);

    // Decreasing
    f = fopen("decreasing.txt", "w");
    for (int i = 1000; i >= 1; i--) {
        fprintf(f, "%d\n", i);
    }
    fclose(f);

    // Mixed
    f = fopen("mixed.txt", "w");
    for (int i = 1; i <= 500; i++) {
        fprintf(f, "%d\n", i);
    }
    for (int i = 100; i > 500; i--) {
        fprintf(f, "%d\n", i);
    }
    fclose(f);

    // Random
    f = fopen("random.txt", "w");
    srand(time(NULL));
    for (int i = 1; i <= 1000; i++) {
        fprintf(f, "%d\n", rand() % 100 + 1);
    }
    fclose(f);
}

// Perform operations
void performOperations(const char *filename, RedBlackTree *tree) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Measure insertion time
    clock_t start, end;
    int num;
    start = clock();
    while (fscanf(f, "%d", &num) != EOF) {
        insert(tree, num);
    }
    end = clock();
    printf("Insertion time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    fclose(f);

    // Measure search time
    start = clock();
    Node *result = search(tree, tree->root, 50);
    end = clock();
    printf("Search time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);

    // Measure deletion time
    if (result != tree->NIL) {
        start = clock();
        deleteNode(tree, result);
        end = clock();
        printf("Deletion time: %lf seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    } else {
        printf("Node 50 not found for deletion.\n");
    }
}
