#ifndef BTREE_H
#define BTREE_H
#define MAX_KEYS 3
#define MAX_CHILDREN 4
#define KEY_LEN 7

typedef struct BTreeNode {
    char keys[MAX_KEYS][KEY_LEN];
    double values[MAX_KEYS];
    struct BTreeNode* children[MAX_CHILDREN];
    int num_keys;
} BTreeNode;

#endif