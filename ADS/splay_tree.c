#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Node structure for the splay tree
typedef struct Node {
    int key;
    struct Node *left, *right;
} Node;

// Function to create a new node
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Right rotate function
Node* rightRotate(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

// Left rotate function
Node* leftRotate(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

// Splay function to bring a key to the root
Node* splay(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    // Key lies in the left subtree
    if (key < root->key) {
        if (root->left == NULL) return root;

        // Zig-Zig (Left Left)
        if (key < root->left->key) {
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
        }
        // Zig-Zag (Left Right)
        else if (key > root->left->key) {
            root->left->right = splay(root->left->right, key);
            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        return (root->left == NULL) ? root : rightRotate(root);
    }
    else { // Key lies in the right subtree
        if (root->right == NULL) return root;

        // Zag-Zig (Right Left)
        if (key < root->right->key) {
            root->right->left = splay(root->right->left, key);
            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        // Zag-Zag (Right Right)
        else if (key > root->right->key) {
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
        }

        return (root->right == NULL) ? root : leftRotate(root);
    }
}

// Insert a key into the splay tree
Node* insert(Node* root, int key) {
    if (root == NULL) return createNode(key);

    root = splay(root, key);

    if (root->key == key) return root;

    Node* newNode = createNode(key);

    if (key < root->key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = NULL;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = NULL;
    }

    return newNode;
}

// Delete a key from the splay tree
Node* delete(Node* root, int key) {
    if (root == NULL) return NULL;

    root = splay(root, key);

    if (root->key != key) return root;

    if (root->left == NULL) {
        Node* temp = root->right;
        free(root);
        return temp;
    } else {
        Node* temp = splay(root->left, key);
        temp->right = root->right;
        free(root);
        return temp;
    }
}

// Search for a key in the splay tree
Node* search(Node* root, int key) {
    return splay(root, key);
}

// In-order traversal to display the tree
void inOrder(Node* root) {
    if (root == NULL) return;
    inOrder(root->left);
    printf("%d ", root->key);
    inOrder(root->right);
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
        // note: for random number file value 500 may not be present everytime so please change this value depending on the elemen you want to delete
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
