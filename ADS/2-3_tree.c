#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Enum to distinguish node types
typedef enum {
    TWO_NODE,
    THREE_NODE
} NodeType;

// Structure for 2-3 Tree Node
typedef struct Node {
    NodeType type;
    int key1, key2;  // For 3-node, key1 is smaller, key2 is larger
    struct Node *left, *middle, *right;
} Node;

// Forward declaration of the delete function
Node* delete(Node* root, int key);

// Create a new 2-node
Node* createTwoNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->type = TWO_NODE;
    newNode->key1 = key;
    newNode->key2 = 0;
    newNode->left = newNode->middle = newNode->right = NULL;
    return newNode;
}

// Create a new 3-node
Node* createThreeNode(int key1, int key2, Node* left, Node* middle, Node* right) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->type = THREE_NODE;

    // Ensure key1 is smaller
    if (key1 > key2) {
        int temp = key1;
        key1 = key2;
        key2 = temp;
    }

    newNode->key1 = key1;
    newNode->key2 = key2;
    newNode->left = left;
    newNode->middle = middle;
    newNode->right = right;
    return newNode;
}

// Find the minimum key in the subtree
int findMin(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node->key1;
}

// Search in 2-3 Tree, now returns Node* instead of bool
Node* search(Node* root, int key) {
    if (root == NULL) return NULL;

    // For 2-node
    if (root->type == TWO_NODE) {
        if (key == root->key1) return root;
        return key < root->key1 ? search(root->left, key) : search(root->right, key);
    }

    // For 3-node
    if (key == root->key1 || key == root->key2) return root;

    if (key < root->key1) return search(root->left, key);
    if (key > root->key2) return search(root->right, key);
    return search(root->middle, key);
}

// Helper function to borrow a key or merge nodes
Node* balanceAfterDeletion(Node* parent, Node* child, bool isLeft) {
    Node* sibling;

    // Determine which sibling to look at
    if (isLeft) {
        sibling = parent->type == TWO_NODE ? parent->right : parent->middle;
    } else {
        sibling = parent->type == TWO_NODE ? parent->left : parent->middle;
    }

    // If sibling is a 3-node, we can borrow a key
    if (sibling->type == THREE_NODE) {
        // Restructure to balance
        if (parent->type == TWO_NODE) {
            if (isLeft) {
                child->key1 = parent->key1;
                parent->key1 = sibling->key2;
                child->right = sibling->right;
                sibling->type = TWO_NODE;
                sibling->right = NULL;
            } else {
                child->key1 = parent->key1;
                parent->key1 = sibling->key1;
                child->left = sibling->right;
                sibling->type = TWO_NODE;
                sibling->right = sibling->middle;
                sibling->middle = NULL;
            }
        } else {
            // More complex restructuring for 3-node parent
            if (isLeft) {
                child->key1 = parent->key1;
                parent->key1 = sibling->key2;
                child->right = sibling->right;
                sibling->type = TWO_NODE;
                sibling->right = NULL;
            } else {
                child->key1 = parent->key2;
                parent->key2 = sibling->key1;
                child->left = sibling->right;
                sibling->type = TWO_NODE;
                sibling->right = sibling->middle;
                sibling->middle = NULL;
            }
        }
        return parent;
    }

    // If we can't borrow, we need to merge
    // This is a simplification and might need more comprehensive handling
    if (parent->type == TWO_NODE) {
        // Convert to a 3-node if possible
        if (isLeft) {
            parent->type = THREE_NODE;
            parent->key2 = parent->key1;
            parent->key1 = child->key1;
            parent->right = parent->middle;
            parent->middle = parent->left;
            parent->left = child->left;
        } else {
            parent->type = THREE_NODE;
            parent->key2 = child->key1;
            parent->right = child->left;
        }
    }

    return parent;
}

// Delete operation
Node* delete(Node* root, int key) {
    if (root == NULL) return NULL;

    // 2-node case
    if (root->type == TWO_NODE) {
        if (key == root->key1) {
            // If it's a leaf, simply remove
            if (root->left == NULL) {
                free(root);
                return NULL;
            }

            // Find inorder successor
            int successor = findMin(root->right);
            root->key1 = successor;
            root->right = delete(root->right, successor);
        } else if (key < root->key1) {
            root->left = delete(root->left, key);
        } else {
            root->right = delete(root->right, key);
        }
        return root;
    }

    // 3-node case
    if (root->type == THREE_NODE) {
        if (key == root->key1) {
            // If it's a leaf, remove key1
            if (root->left == NULL) {
                root->type = TWO_NODE;
                root->key1 = root->key2;
                return root;
            }

            // Find inorder successor
            int successor = findMin(root->middle);
            root->key1 = successor;
            root->middle = delete(root->middle, successor);
        } else if (key == root->key2) {
            // If it's a leaf, remove key2
            if (root->left == NULL) {
                root->type = TWO_NODE;
                root->key2 = 0;
                return root;
            }

            // Find inorder successor
            int successor = findMin(root->right);
            root->key2 = successor;
            root->right = delete(root->right, successor);
        } else if (key < root->key1) {
            root->left = delete(root->left, key);
        } else {
            root->right = delete(root->right, key);
        }
        return root;
    }

    return root;
}

// Insert into 2-3 Tree (previous implementation remains the same)
Node* insert(Node* root, int key) {
    if (root == NULL) return createTwoNode(key);

    // For 2-node
    if (root->type == TWO_NODE) {
        if (key == root->key1) return root;  // Duplicate not allowed

        if (key < root->key1) {
            if (root->left == NULL) {
                root->left = createTwoNode(key);
                return root;
            }
            root->left = insert(root->left, key);
        } else {
            if (root->right == NULL) {
                root->right = createTwoNode(key);
                return root;
            }
            root->right = insert(root->right, key);
        }
    }

    // For 3-node
    if (root->type == THREE_NODE) {
        if (key == root->key1 || key == root->key2) return root;  // Duplicate not allowed

        if (key < root->key1) {
            if (root->left == NULL) {
                root->left = createTwoNode(key);
                return root;
            }
            root->left = insert(root->left, key);
        } else if (key > root->key2) {
            if (root->right == NULL) {
                root->right = createTwoNode(key);
                return root;
            }
            root->right = insert(root->right, key);
        } else {
            if (root->middle == NULL) {
                root->middle = createTwoNode(key);
                return root;
            }
            root->middle = insert(root->middle, key);
        }
    }

    return root;
}

// Utility function to print tree (in-order traversal)
void inorderTraversal(Node* root) {
    if (root == NULL) return;

    if (root->type == TWO_NODE) {
        inorderTraversal(root->left);
        printf("%d ", root->key1);
        inorderTraversal(root->right);
    } else {
        inorderTraversal(root->left);
        printf("%d ", root->key1);
        inorderTraversal(root->middle);
        printf("%d ", root->key2);
        inorderTraversal(root->right);
    }
}

void processFiles(const char* files[], int fileCount) {
    Node* root = NULL;

    for (int i = 0; i < fileCount; i++) {
        FILE* file = fopen(files[i], "r");
        if (file == NULL) {
            perror("Error opening file");
            return;
        }

        printf("\nProcessing file: %s\n", files[i]);

        int number, nodeCount = 0;
        clock_t start, end;

        // Insertion time
        start = clock();
        while (fscanf(file, "%d,", &number) == 1) {
            root = insert(root, number);
            nodeCount++;
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

        // Deletion time for node with value 500
        start = clock();
        root = delete(root, 500);
        end = clock();
        printf("Deletion time for node with value 500: %f seconds\n", ((double)(end - start)) / CLOCKS_PER_SEC);

        fclose(file);
        root = NULL; // Reset the tree for the next file
    }
}

int main() {

    const char* files[] = {
        "random_numbers.txt", 
        "mixed_numbers.txt", 
        "increasing_numbers.txt", 
        "decreasing_numbers.txt"
    };

    processFiles(files, 4);

    return 0;
}
