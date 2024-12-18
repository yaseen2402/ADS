#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <time.h>

typedef struct Node{
    int data;
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(int data){
    Node* newNode = (Node*) malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, int data){
    if(root==NULL){
        return createNode(data);
    }
    if(data < root->data){
        root->left = insert(root->left, data);
    }
    else if(data > root->data){
        root->right = insert(root->right, data);
    }
    return root;
}

Node* search(Node* root, int data){
    if( root==NULL || root->data == data){
        return root;
    }
    if(data<root->data){
        return search(root->left, data);
    }
    else if(data>root->data){
        return search(root->right, data);
    }
}

Node* findPredecessor(Node* root){
    root = root->left;
    while(root->right != NULL){
        root = root->right;
    }
    return root;
}

Node* delete(Node* root, int data){

    if(root==NULL){
        return NULL;
    }

    if(root->left == NULL || root->right == NULL){
        free(root);
        return NULL;
    }

    if(data<root->data){
        root->left = delete(root->left, data);
    }
    else if(data>root->data){
        root->right = delete(root->right, data);
    }
    else{
        Node* inorder_pre = findPredecessor(root);
        root->data = inorder_pre->data;
        delete(root->left, inorder_pre->data);
    }

    return root;
}

int main(){

    FILE* file = fopen("input.txt", "r");

    if(file==NULL){
        printf("could not open the file");
        return 1;
    }

    Node* root = NULL;
    int number;

    for(int i=0; i<100 && fscanf(file, "%d,", &number)==1; i++){
        root = insert(root, number);
    }
    fclose(file);

    int searchValue = 50;

    clock_t start, end;
    start = clock();
    Node* found = search(root, searchValue);
    end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (found != NULL) {
        printf("Element %d found in the BST.\n", searchValue);
    } else {
        printf("Element %d not found in the BST.\n", searchValue);
    }
    printf("Time taken to search: %f seconds\n", time_taken);

    return 0;
}