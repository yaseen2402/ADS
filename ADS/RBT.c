#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// Function to check if a file exists
int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file != NULL) {
        fclose(file);
        return 1;
    }
    return 0;
}

void generateRandomNumbersFile(const char* filename, int count) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(1);
    }
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,", rand() % 100); // Random numbers between 0 and 9999
    }
    fclose(file);
    printf("File '%s' created with %d random numbers.\n", filename, count);
}

void generateMixedNumbersFile(const char* filename, int randomCount, int orderedCount) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(1);
    }
    srand(time(NULL));
    // Generate random numbers
    for (int i = 0; i < randomCount; i++) {
        fprintf(file, "%d,", rand() % 100);
    }
    // Generate ordered numbers
    for (int i = 0; i < orderedCount; i++) {
        fprintf(file, "%d,", i);
    }
    fclose(file);
    printf("File '%s' created with %d random and %d ordered numbers.\n", filename, randomCount, orderedCount);
}

void generateIncreasingNumbersFile(const char* filename, int count) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(1);
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,", i);
    }
    fclose(file);
    printf("File '%s' created with %d increasing ordered numbers.\n", filename, count);
}

void generateDecreasingNumbersFile(const char* filename, int count) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(1);
    }
    for (int i = count - 1; i >= 0; i--) {
        fprintf(file, "%d,", i);
    }
    fclose(file);
    printf("File '%s' created with %d decreasing ordered numbers.\n", filename, count);
}


// Red-Black Tree Node Structure
typedef enum { RED, BLACK } Color;

typedef struct Node {
    int data;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
    Color color;
} Node;

Node* NIL;  // Sentinel NIL node for RBT

// Initialize NIL node
void initNIL() {
    NIL = malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NULL;
    NIL->data = 0;
}
// Function to create a new node
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = data;
    newNode->parent = NIL;
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->color = RED;
    return newNode;
}

void freeTreeRecursive(Node* root) {
    if (root == NIL) return;
    
    freeTreeRecursive(root->left);
    freeTreeRecursive(root->right);
    
    // Only free if it's not the NIL sentinel
    if (root != NIL) {
        free(root);
    }
}

void cleanupTree(Node* root) {
    // Free the entire tree
    freeTreeRecursive(root);
    
    // Free the NIL sentinel
    if (NIL) {
        free(NIL);
        NIL = NULL;
    }
}

// Function to perform a left rotate
void leftRotate(Node** root, Node* x) {
    Node* y = x->right;
    x->right = y->left;

    if (y->left != NIL) {
        y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent == NIL) {
        *root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

// Function to perform a right rotate
void rightRotate(Node** root, Node* y) {
    Node* x = y->left;
    y->left = x->right;

    if (x->right != NIL) {
        x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent == NIL) {
        *root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

void fixInsert(Node** root, Node* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node* y = z->parent->parent->right;
            if (y->color == RED) {  // Case 1: Uncle is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {  // Case 2: Triangle
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;  // Case 3: Line
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            Node* y = z->parent->parent->left;
            if (y->color == RED) {  // Case 1: Uncle is RED
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {  // Case 2: Triangle
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;  // Case 3: Line
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }

        // Prevent infinite loop
        if (z == *root) break;
    }
    (*root)->color = BLACK;
}



Node* insert(Node** root, int data) {
    // If tree is empty, create root node
    if (*root == NIL) {
        *root = createNode(data);
        (*root)->color = BLACK;
        return *root;
    }

    Node* z = createNode(data);
    Node* y = NIL;
    Node* x = *root;

    // Find insertion point
    while (x != NIL) {
        y = x;
        if (z->data < x->data) {
            x = x->left;
        } else if (z->data > x->data) {
            x = x->right;
        } else {
            // Duplicate value, do not insert
            free(z);
            return x; // Return existing node if duplicate
        }
    }

    // Link the new node
    z->parent = y;
    if (y == NIL) {
        *root = z;
    } else if (z->data < y->data) {
        y->left = z;
    } else {
        y->right = z;
    }

    // Restore Red-Black Tree properties
    fixInsert(root, z);

    return z; // Return the newly inserted node
}

// Function to search for a node in the RBT
Node* search(Node* root, int data) {
    while (root != NIL && data != root->data) {
        if (data < root->data) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root == NIL ? NULL : root;
}

// Fixup function for Red-Black Tree after deletion
void fixDelete(Node** root, Node* x) {
    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            Node* w = x->parent->right;
            if (w->color == RED) {  // Case 1: Sibling is RED
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {  // Case 2: Sibling's children are BLACK
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {  // Case 3: Sibling's right child is BLACK
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;  // Case 4: Sibling's right child is RED
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
        } else {
            Node* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

// Function to find the minimum node in a subtree
Node* minimum(Node* node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

// Delete function for Red-Black Tree
void deleteNode(Node** root, Node* z) {
    Node* y = z;
    Node* x;
    Color yOriginalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        if (z->parent == NIL) {
            *root = x;
        } else if (z == z->parent->left) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
        x->parent = z->parent;
    } else if (z->right == NIL) {
        x = z->left;
        if (z->parent == NIL) {
            *root = x;
        } else if (z == z->parent->left) {
            z->parent->left = x;
        } else {
            z->parent->right = x;
        }
        x->parent = z->parent;
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            if (y->parent == NIL) {
                *root = x;
            } else if (y == y->parent->left) {
                y->parent->left = x;
            } else {
                y->parent->right = x;
            }
            y->right = z->right;
            y->right->parent = y;
        }
        if (z->parent == NIL) {
            *root = y;
        } else if (z == z->parent->left) {
            z->parent->left = y;
        } else {
            z->parent->right = y;
        }
        y->parent = z->parent;
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK) {
        fixDelete(root, x);
    }

    free(z);
}

void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}


void processFiles(const char* files[], int fileCount) {
    Node* root = NIL;  // Initialize root to NIL

    for (int i = 0; i < fileCount; i++) {
        FILE* file = fopen(files[i], "r");
        if (file == NULL) {
            perror("Error opening file");
            continue;
        }

        printf("\nProcessing file: %s\n", files[i]);

        int number, nodeCount = 0;
        clock_t start, end;

        // Reset root to NIL before processing
        root = NIL;

        // Insertion time
        start = clock();

        // Use a more robust reading method
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file)) {
            char* token = strtok(buffer, ",");
            while (token != NULL) {
                if (sscanf(token, "%d", &number) == 1) {
                    printf("Inserting node with value: %d\n", number);
                    insert(&root, number);
                    nodeCount++;
                } else {
                    fprintf(stderr, "Invalid number format: %s\n", token);
                }
                token = strtok(NULL, ",");
            }
        }

        end = clock();
        printf("Insertion time for %d nodes: %f seconds\n", nodeCount, ((double)(end - start)) / CLOCKS_PER_SEC);

        // Search time for node with value 500
        start = clock();
        Node* foundNode = search(root, 500);
        end = clock();
        if (foundNode != NULL) {
            printf("Node with value 500 found.\n");
        } else {
            printf("Node with value 500 not found.\n");
        }
        printf("Search time for node with value 500: %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

        fclose(file);

        // Cleanup the tree after processing each file
        cleanupTree(root);
    }
}




int main() {
    initNIL();
    const char* files[] = {
        "increasing_numbers.txt",
        "decreasing_numbers.txt",
        "random_numbers.txt",
        "mixed_numbers.txt"
    };

    generateIncreasingNumbersFile(files[0], 1000);
    generateDecreasingNumbersFile(files[1], 1000);
    generateRandomNumbersFile(files[3], 1000);
    generateMixedNumbersFile(files[4], 500, 500);

    processFiles(files, 4);

    return 0;
}