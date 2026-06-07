#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "../structures/btree.h"

#ifdef __cplusplus
extern "C" {
#endif

    BTreeNode* createNode();
    double* findNode(BTreeNode *node, const char *key);
    void splitChild(BTreeNode* parent, int childIndex);
    BTreeNode* insertRecursive(BTreeNode *node, const char *key, double value);
    BTreeNode* insert(BTreeNode *root, const char* key, double value);
    void borrowFromLeft(BTreeNode* node, int idx);
    void borrowFromRight(BTreeNode* node, int idx);
    void merge(BTreeNode* node, int idx);
    void fill(BTreeNode* node, int idx);
    BTreeNode* deleteRecursive(BTreeNode* node, const char* key);
    BTreeNode* deleteKey(BTreeNode* root, const char* key);
    void printTree(BTreeNode* node, int level);
    void processFile(const char* inputFile, const char* outputFile);

#ifdef __cplusplus
}
#endif

#endif