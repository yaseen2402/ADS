#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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


typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

Node* search(Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (data < root->data) {
        return search(root->left, data);
    } else {
        return search(root->right, data);
    }
}

Node* findPredecessor(Node* root) {
    root = root->left;
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

Node* delete(Node* root, int data) {
    if (root == NULL) {
        return NULL;
    }

    if (data < root->data) {
        root->left = delete(root->left, data);
    } else if (data > root->data) {
        root->right = delete(root->right, data);
    } else {
        if (root->left == NULL || root->right == NULL) {
            Node* temp = root;
            if (root->left != NULL) {
                root = root->left;
            } else if (root->right != NULL) {
                root = root->right;
            }
            free(temp);
        } else {
            Node* inorder_pre = findPredecessor(root);
            root->data = inorder_pre->data;
            root->left = delete(root->left, inorder_pre->data);
        }
    }
    return root;
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
    
    generateRandomNumbersFile(files[0], 1000);
    generateMixedNumbersFile(files[1], 500, 500);
    generateIncreasingNumbersFile(files[2], 1000);
    generateDecreasingNumbersFile(files[3], 1000);

    processFiles(files, 4);

    return 0;
}
