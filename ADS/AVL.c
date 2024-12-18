#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int max(int a, int b){
    return a>b?a:b;
}

// File generation functions
void generateRandomNumbersFile(const char* filename, int count) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error creating file");
        exit(1);
    }
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d,", rand() % 1000); // Random numbers between 0 and 9999
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
        fprintf(file, "%d,", rand() % 1000);
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

// AVL Tree structure and functions
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int height(Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;
    return newNode;
}

int getBalance(Node* node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));
    return y;
}

Node* insert(Node* node, int data) {
    if (node == NULL) return createNode(data);

    if (data < node->data)
        node->left = insert(node->left, data);
    else if (data > node->data)
        node->right = insert(node->right, data);
    else
        return node;

//    node->height = 1 + (height(node->left) > height(node->right) ? height(node->left) : height(node->right));
    node->height = 1 + max(height(node->left), height(node->right));


    int balance = getBalance(node);

    if (balance > 1 && data < node->left->data)
        return rightRotate(node);

    if (balance < -1 && data > node->right->data)
        return leftRotate(node);

    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* delete(Node* root, int data) {
    if (root == NULL) return root;

    if (data < root->data)
        root->left = delete(root->left, data);
    else if (data > root->data)
        root->right = delete(root->right, data);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
        } else {
            Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = delete(root->right, temp->data);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

Node* search(Node* root, int data) {
    if (root == NULL || root->data == data)
        return root;

    if (data < root->data)
        return search(root->left, data);

    return search(root->right, data);
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
        // note: for random number file value 500 may not be present everytime so please change this value depending on the element you want to delete
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
    //    //note: can comment these once the files are generated
    generateRandomNumbersFile(files[0], 1000);
    generateRandomNumbersFile(files[1], 1000);
    generateRandomNumbersFile(files[2], 1000);
    generateRandomNumbersFile(files[3], 1000);

    processFiles(files, 4);

    return 0;
}
