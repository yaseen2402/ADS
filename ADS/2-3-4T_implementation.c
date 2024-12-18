#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Enum to distinguish node types
typedef enum {
    TWO_NODE,
    THREE_NODE,
    FOUR_NODE
} NodeType;

// Structure for 2-3-4 Tree Node
typedef struct Node {
    NodeType type;
    int key1, key2, key3;  // Up to 3 keys
    struct Node *child1, *child2, *child3, *child4;  // Up to 4 children
} Node;

// Create a 2-node
Node* createTwoNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->type = TWO_NODE;
    newNode->key1 = key;
    newNode->key2 = newNode->key3 = 0;
    newNode->child1 = newNode->child2 = newNode->child3 = newNode->child4 = NULL;
    return newNode;
}

// Create a 3-node
Node* createThreeNode(int key1, int key2) {
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
    newNode->key3 = 0;
    newNode->child1 = newNode->child2 = newNode->child3 = newNode->child4 = NULL;
    return newNode;
}

// Create a 4-node
Node* createFourNode(int key1, int key2, int key3) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->type = FOUR_NODE;
    
    // Sort keys
    int keys[3] = {key1, key2, key3};
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2 - i; j++) {
            if (keys[j] > keys[j+1]) {
                int temp = keys[j];
                keys[j] = keys[j+1];
                keys[j+1] = temp;
            }
        }
    }
    
    newNode->key1 = keys[0];
    newNode->key2 = keys[1];
    newNode->key3 = keys[2];
    newNode->child1 = newNode->child2 = newNode->child3 = newNode->child4 = NULL;
    return newNode;
}

// Split a 4-node
Node* splitFourNode(Node* node) {
    // Create a new 2-node as parent with middle key
    Node* parent = createTwoNode(node->key2);
    
    // Left child becomes a 2-node with the smallest key
    Node* leftChild = createTwoNode(node->key1);
    leftChild->child1 = node->child1;
    leftChild->child2 = node->child2;
    
    // Right child becomes a 2-node with the largest key
    Node* rightChild = createTwoNode(node->key3);
    rightChild->child1 = node->child3;
    rightChild->child2 = node->child4;
    
    // Connect parent to children
    parent->child1 = leftChild;
    parent->child2 = rightChild;
    
    return parent;
}

// Search in 2-3-4 Tree
Node* search(Node* root, int key) {
    if (root == NULL) return NULL;

    // 2-node case
    if (root->type == TWO_NODE) {
        if (key == root->key1) return root;
        return key < root->key1 ? search(root->child1, key) : search(root->child2, key);
    }
    
    // 3-node case
    if (root->type == THREE_NODE) {
        if (key == root->key1 || key == root->key2) return root;
        
        if (key < root->key1) return search(root->child1, key);
        if (key > root->key2) return search(root->child3, key);
        return search(root->child2, key);
    }
    
    // 4-node case
    if (root->type == FOUR_NODE) {
        if (key == root->key1 || key == root->key2 || key == root->key3) return root;
        
        if (key < root->key1) return search(root->child1, key);
        if (key > root->key3) return search(root->child4, key);
        if (key < root->key2) return search(root->child2, key);
        return search(root->child3, key);
    }
    
    //key not found
    return NULL;
}

// Insertion helper to handle node splitting
Node* insertNonFull(Node* root, int key) {
    // If root is a 4-node, split it first
    if (root->type == FOUR_NODE) {
        root = splitFourNode(root);
    }

    // 2-node case
    if (root->type == TWO_NODE) {
        if (key < root->key1) {
            if (root->child1 == NULL) {
                // Insert at leaf
                if (key < root->key1) {
                    root->key2 = root->key1;
                    root->key1 = key;
                    root->type = THREE_NODE;
                } else {
                    root->key2 = key;
                    root->type = THREE_NODE;
                }
            } else {
                // Recursive insertion
                root->child1 = insertNonFull(root->child1, key);
            }
        } else {
            if (root->child2 == NULL) {
                // Insert at leaf
                root->key2 = key;
                root->type = THREE_NODE;
            } else {
                // Recursive insertion
                root->child2 = insertNonFull(root->child2, key);
            }
        }
    }
    // 3-node case
    else if (root->type == THREE_NODE) {
        if (key < root->key1) {
            if (root->child1 == NULL) {
                // Convert to 4-node
                root->key3 = root->key2;
                root->key2 = root->key1;
                root->key1 = key;
                root->type = FOUR_NODE;
            } else {
                // Recursive insertion
                root->child1 = insertNonFull(root->child1, key);
            }
        } else if (key > root->key2) {
            if (root->child3 == NULL) {
                // Convert to 4-node
                root->key3 = key;
                root->type = FOUR_NODE;
            } else {
                // Recursive insertion
                root->child3 = insertNonFull(root->child3, key);
            }
        } else {
            if (root->child2 == NULL) {
                // Convert to 4-node
                root->key3 = root->key2;
                root->key2 = key;
                root->type = FOUR_NODE;
            } else {
                // Recursive insertion
                root->child2 = insertNonFull(root->child2, key);
            }
        }
    }
    
    return root;
}

// Main insertion function
Node* insert(Node* root, int key) {
    // Empty tree
    if (root == NULL) {
        return createTwoNode(key);
    }
    
    // If root is a 4-node, split it first
    if (root->type == FOUR_NODE) {
        root = splitFourNode(root);
    }
    
    return insertNonFull(root, key);
}

// Find the minimum key in the subtree
int findMin(Node* node) {
    while (node->child1 != NULL) {
        node = node->child1;
    }
    return node->key1;
}

// Delete operation
Node* delete(Node* root, int key) {
    if (root == NULL) return NULL;

    // 2-node case
    if (root->type == TWO_NODE) {
        if (key == root->key1) {
            // If leaf, simply delete
            if (root->child1 == NULL) {
                free(root);
                return NULL;
            }
            
            // Find inorder successor
            int successor = findMin(root->child2);
            root->key1 = successor;
            root->child2 = delete(root->child2, successor);
        } else if (key < root->key1) {
            root->child1 = delete(root->child1, key);
        } else {
            root->child2 = delete(root->child2, key);
        }
        return root;
    }
    
    // 3-node case
    if (root->type == THREE_NODE) {
        // Locate the key and appropriate child
        if (key == root->key1) {
            // If leaf, remove key1
            if (root->child1 == NULL) {
                root->key1 = root->key2;
                root->type = TWO_NODE;
                return root;
            }
            
            // Find inorder successor
            int successor = findMin(root->child2);
            root->key1 = successor;
            root->child2 = delete(root->child2, successor);
        } else if (key == root->key2) {
            // If leaf, remove key2
            if (root->child1 == NULL) {
                root->type = TWO_NODE;
                root->key2 = 0;
                return root;
            }
            
            // Find inorder successor
            int successor = findMin(root->child3);
            root->key2 = successor;
            root->child3 = delete(root->child3, successor);
        } else if (key < root->key1) {
            root->child1 = delete(root->child1, key);
        } else {
            root->child3 = delete(root->child3, key);
        }
        return root;
    }
    
    // 4-node case
    if (root->type == FOUR_NODE) {
        if (key == root->key1) {
            // If leaf, remove key1
            if (root->child1 == NULL) {
                root->key1 = root->key2;
                root->key2 = root->key3;
                root->type = THREE_NODE;
                return root;
            }
            
            // Find inorder successor
            int successor = findMin(root->child2);
            root->key1 = successor;
            root->child2 = delete(root->child2, successor);
        } else if (key == root->key2) {
            // If leaf, remove key2
            if (root->child1 == NULL) {
                root->key2 = root->key3;
                root->type = THREE_NODE;
                return root;
            }
            
            // Find inorder successor
            int successor = findMin(root->child3);
            root->key2 = successor;
            root->child3 = delete(root->child3, successor);
        } else if (key == root->key3) {
            // If leaf, remove key3
            if (root->child1 == NULL) {
                root->type = THREE_NODE;
                root->key3 = 0;
                return root;
            }
            
            // Find inorder successor
            int successor = findMin(root->child4);
            root->key3 = successor;
            root->child4 = delete(root->child4, successor);
        } else if (key < root->key1) {
            root->child1 = delete(root->child1, key);
        } else if (key > root->key3) {
            root->child4 = delete(root->child4, key);
        } else if (key < root->key2) {
            root->child2 = delete(root->child2, key);
        } else {
            root->child3 = delete(root->child3, key);
        }
        return root;
    }
    
    return root;
}

// Utility function to print tree (in-order traversal)
void inorderTraversal(Node* root) {
    if (root == NULL) return;

    switch (root->type) {
        case TWO_NODE:
            if (root->child1) inorderTraversal(root->child1);
            printf("%d ", root->key1);
            if (root->child2) inorderTraversal(root->child2);
            break;
        
        case THREE_NODE:
            if (root->child1) inorderTraversal(root->child1);
            printf("%d ", root->key1);
            if (root->child2) inorderTraversal(root->child2);
            printf("%d ", root->key2);
            if (root->child3) inorderTraversal(root->child3);
            break;
        
        case FOUR_NODE:
            if (root->child1) inorderTraversal(root->child1);
            printf("%d ", root->key1);
            if (root->child2) inorderTraversal(root->child2);
            printf("%d ", root->key2);
            if (root->child3) inorderTraversal(root->child3);
            printf("%d ", root->key3);
            if (root->child4) inorderTraversal(root->child4);
            break;
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