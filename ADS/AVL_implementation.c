#include <stdio.h>
#include <stdlib.h>

int max(int a, int b){
    return a > b ? a : b;
}

typedef struct Node{
    int key;
    struct Node* left;
    struct Node* right;
    int height;
} Node;

int getHeight(Node* node){
    if(node==NULL)
        return 0;
    return node->height;    
}

int balanceFactor(Node* node){
    if(node==NULL)
        return 0;

    return getHeight(node->left) - getHeight(node->right);    
}

Node* createNode(int key){
    Node* newNode = (Node*) Malloc(sizeof(Node));
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;

    return newNode;
}

Node* rightRotate(Node* y){
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(getHeight(y->left), getHeight(y->right));
    x->height = 1 + max(getHeight(x->left), getHeight(x->right));

    return x; 
}

//          y                           x
//         / \     right rotation->    / \ 
//        x   T3                     T1   y
//       / \       <-left rotation       / \ 
//      T1  T2                          T2  T3
//


Node* leftRotate(Node* x){
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(getHeight(x->left), getHeight(x->right));
    y->height = 1 + max(getHeight(y->left), getHeight(y->right));

    return y;
}

Node* insert(Node* node, int key){
    if(node==NULL)
        return createNode(key);

    if(key<node->key)
        node->left = insert(node->left, key);  

    else if(key>node->key)
        node->right = insert(node->right, key);
    

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int bf = balanceFactor(node);

    //left heavy
    if(bf>1 && key<node->left->key)
        rightRotate(node);

    //right heavy
    if(bf<-1 && key>node->right->key)
        leftRotate(node);

    //left-right
    if(bf>1 && key>node->left->key){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    //right-left
    if(bf<-1 && key<node->right->key){
        node->right = rightRotate(node->right->key);
        return leftRotate(node);
    }

    return node;
}

Node* findMin(Node* node){
    Node* current = node;
    while(current->left)
        current = current->left;

    return current;    
}

Node* delete(Node* node, int key){
    if(node==NULL)
        return;

    if(key<node->key)
        node->left = delete(node->left, key);
        
    else if(key>node->key)
        node->right = delete(node->right, key);
}