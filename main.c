#include <stdio.h>
#include "src/functions/functions.h"

int main() {
    BTreeNode* root = NULL;
    root = insert(root, "qwerty", 3.14);
    root = insert(root, "asdf", 5.0);
    root = insert(root, "dyz", 4.0);
    printf("Before 4th insert, root num_keys=%d\n", root->num_keys);
    root = insert(root, "uiop", 7.7777);
    printf("After 4th insert\n");
    printf("After 4th insert, root num_keys=%d\n", root->num_keys);
    printf("Root key: %s\n", root->keys[0]);
    printf("Left child keys: ");
    for (int i = 0; i < root->children[0]->num_keys; i++) printf("%s ", root->children[0]->keys[i]);
    printf("\nRight child keys: ");
    for (int i = 0; i < root->children[1]->num_keys; i++) printf("%s ", root->children[1]->keys[i]);
    printf("\n");
    printf("Left child children[0]=%p, children[1]=%p, children[2]=%p\n",
       root->children[0]->children[0],
       root->children[0]->children[1],
       root->children[0]->children[2]);
    printTree(root, 0);
    printTree(root, 0);
    return 0;
}