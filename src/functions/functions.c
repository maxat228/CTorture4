#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../structures/btree.h"


BTreeNode* createNode() {
    BTreeNode *node = malloc(sizeof(BTreeNode));
    if (node == NULL) return NULL;
    node->num_keys = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) {
        node->children[i] = NULL;
    }
    for (int i = 0; i < MAX_KEYS; i++) {
        node->keys[i][0] = '\0';
        node->values[i] = 0.0;
    }
    return node;
}


double* findNode(BTreeNode *node, const char *key) {
    if (node == NULL) return NULL;
    int i = 0;
    while (i < node->num_keys && strcmp(node->keys[i], key) < 0) {
        i++;
    }
    if (i < node->num_keys && strcmp(node->keys[i], key) == 0) {
        return &node->values[i];
    }
    return findNode(node->children[i], key);
}


void splitChild(BTreeNode* parent, int childIndex) {
    BTreeNode* child = parent->children[childIndex];
    BTreeNode* right = createNode();

    char middleKey[KEY_LEN];
    double middleValue;
    strcpy(middleKey, child->keys[2]);
    middleValue = child->values[2];

    strcpy(right->keys[0], child->keys[3]);
    right->values[0] = child->values[3];
    right->num_keys = 1;

    if (child->children[0] != NULL) {
        right->children[0] = child->children[3];
        right->children[1] = child->children[4];
    }

    child->num_keys = 2;
    child->children[2] = NULL;
    child->children[3] = NULL;
    child->keys[2][0] = '\0';
    child->keys[3][0] = '\0';

    for (int i = parent->num_keys; i > childIndex; i--) {
        parent->children[i + 1] = parent->children[i];
    }

    for (int j = parent->num_keys - 1; j >= childIndex; j--) {
        strcpy(parent->keys[j + 1], parent->keys[j]);
        parent->values[j + 1] = parent->values[j];
    }

    strcpy(parent->keys[childIndex], middleKey);
    parent->values[childIndex] = middleValue;
    parent->num_keys++;

    parent->children[childIndex + 1] = right;
}


BTreeNode* insertRecursive(BTreeNode *node, const char *key, double value) {
    if (node->children[0] == NULL) {
        int i = node->num_keys - 1;
        while (i >= 0 && strcmp(node->keys[i], key) > 0) {
            strcpy(node->keys[i + 1], node->keys[i]);
            node->values[i + 1] = node->values[i];
            i--;
        }
        strcpy(node->keys[i + 1], key);
        node->values[i + 1] = value;
        node->num_keys++;
    } else {
        int i = 0;
        while (i < node->num_keys && strcmp(node->keys[i], key) < 0) {
            i++;
        }
        node->children[i] = insertRecursive(node->children[i], key, value);

        if (node->children[i]->num_keys == 4) {
            splitChild(node, i);
        }
    }
    return node;
}


BTreeNode* insert(BTreeNode *root, const char* key, double value) {
    if (root == NULL) {
        root = createNode();
        strcpy(root->keys[0], key);
        root->values[0] = value;
        root->num_keys = 1;
        return root;
    }

    root = insertRecursive(root, key, value);

    if (root->num_keys == 4) {
        printf("Root overflow, splitting...\n");
        BTreeNode* newROOT = createNode();
        newROOT->children[0] = root;
        printf("Calling splitChild, root keys: ");
        for (int i = 0; i < root->num_keys; i++) printf("%s ", root->keys[i]);
        printf("\n");
        splitChild(newROOT, 0);
        printf("Split done\n");
        root = newROOT;
    }
    return root;
}


void borrowFromLeft(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* leftSibling = node->children[idx - 1];

    for (int i = child->num_keys; i > 0; i--) {
        strcpy(child->keys[i], child->keys[i - 1]);
        child->values[i] = child->values[i - 1];
    }

    strcpy(child->keys[0], node->keys[idx - 1]);
    child->values[0] = node->values[idx - 1];

    strcpy(node->keys[idx - 1], leftSibling->keys[leftSibling->num_keys - 1]);
    node->values[idx - 1] = leftSibling->values[leftSibling->num_keys - 1];

    if (child->children[0] != NULL) {
        for (int i = child->num_keys + 1; i > 0; i--) {
            child->children[i] = child->children[i - 1];
        }
        child->children[0] = leftSibling->children[leftSibling->num_keys];
    }

    child->num_keys++;
    leftSibling->num_keys--;
}


void borrowFromRight(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* rightSibling = node->children[idx + 1];

    strcpy(child->keys[child->num_keys], node->keys[idx]);
    child->values[child->num_keys] = node->values[idx];

    strcpy(node->keys[idx], rightSibling->keys[0]);
    node->values[idx] = rightSibling->values[0];

    if (child->children[0] != NULL) {
        child->children[child->num_keys + 1] = rightSibling->children[0];
    }

    for (int i = 0; i < rightSibling->num_keys - 1; i++) {
        strcpy(rightSibling->keys[i], rightSibling->keys[i + 1]);
        rightSibling->values[i] = rightSibling->values[i + 1];
    }
    if (rightSibling->children[0] != NULL) {
        for (int i = 0; i < rightSibling->num_keys; i++) {
            rightSibling->children[i] = rightSibling->children[i + 1];
        }
    }

    child->num_keys++;
    rightSibling->num_keys--;
}


void merge(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* rightSibling = node->children[idx + 1];

    strcpy(child->keys[child->num_keys], node->keys[idx]);
    child->values[child->num_keys] = node->values[idx];

    for (int i = 0; i < rightSibling->num_keys; i++) {
        strcpy(child->keys[child->num_keys + 1 + i], rightSibling->keys[i]);
        child->values[child->num_keys + 1 + i] = rightSibling->values[i];
    }

    if (child->children[0] != NULL) {
        for (int i = 0; i <= rightSibling->num_keys; i++) {
            child->children[child->num_keys + 1 + i] = rightSibling->children[i];
        }
    }

    child->num_keys += rightSibling->num_keys + 1;
    free(rightSibling);

    for (int i = idx; i < node->num_keys - 1; i++) {
        strcpy(node->keys[i], node->keys[i + 1]);
        node->values[i] = node->values[i + 1];
        node->children[i + 1] = node->children[i + 2];
    }
    node->num_keys--;
}


void fill(BTreeNode* node, int idx) {
    if (idx != 0 && node->children[idx - 1]->num_keys >= 2) {
        borrowFromLeft(node, idx);
    } else if (idx != node->num_keys && node->children[idx + 1]->num_keys >= 2) {
        borrowFromRight(node, idx);
    } else {
        if (idx != node->num_keys) {
            merge(node, idx);
        } else {
            merge(node, idx - 1);
        }
    }
}


BTreeNode* deleteRecursive(BTreeNode* node, const char* key) {
    int idx = 0;
    while (idx < node->num_keys && strcmp(node->keys[idx], key) < 0) {
        idx++;
    }

    if (idx < node->num_keys && strcmp(node->keys[idx], key) == 0) {
        if (node->children[0] == NULL) {
            node->keys[idx][0] = '\0';
            for (int i = idx; i < node->num_keys - 1; i++) {
                strcpy(node->keys[i], node->keys[i + 1]);
                node->values[i] = node->values[i + 1];
            }
            node->num_keys--;
        } else {
            BTreeNode* predNode = node->children[idx];
            while (predNode->children[predNode->num_keys] != NULL) {
                predNode = predNode->children[predNode->num_keys];
            }
            strcpy(node->keys[idx], predNode->keys[predNode->num_keys - 1]);
            node->values[idx] = predNode->values[predNode->num_keys - 1];
            deleteRecursive(node->children[idx], predNode->keys[predNode->num_keys - 1]);
        }
    } else {
        if (node->children[0] == NULL) {
            return node;
        }
        bool isLast = (idx == node->num_keys);
        if (node->children[idx]->num_keys < 2) {
            fill(node, idx);
        }
        if (isLast && idx > node->num_keys) {
            deleteRecursive(node->children[idx - 1], key);
        } else {
            deleteRecursive(node->children[idx], key);
        }
    }
    return node;
}


BTreeNode* deleteKey(BTreeNode* root, const char* key) {
    if (root == NULL) return NULL;

    root = deleteRecursive(root, key);

    if (root->num_keys == 0 && root->children[0] != NULL) {
        BTreeNode* newROOT = root->children[0];
        free(root);
        return newROOT;
    }
    return root;
}


void printTree(BTreeNode* node, int level) {
    if (node == NULL) return;
    for (int i = 0; i <= node->num_keys; i++) {
        if (i < node->num_keys) {
            printTree(node->children[i], level + 1);
            for (int j = 0; j < level; j++) printf("    ");
            printf("%s: %.2f\n", node->keys[i], node->values[i]);
        } else {
            printTree(node->children[i], level + 1);
        }
    }
}


void processFile(const char* inputFile, const char* outputFile) {
    FILE* in = fopen(inputFile, "r");
    FILE* out = fopen(outputFile, "w");
    if (in == NULL || out == NULL) {
        printf("Error opening file\n");
        return;
    }

    BTreeNode* root = NULL;
    char line[256];
    int cmd;
    char key[KEY_LEN];
    double value;

    while (fgets(line, sizeof(line), in)) {
        fprintf(out, "%s", line);
        int n = sscanf(line, "%d %s %lf", &cmd, key, &value);

        switch (cmd) {
            case 1:
                if (n >= 3) {
                    root = insert(root, key, value);
                    fprintf(out, "Inserted: %s = %.2f\n", key, value);
                }
                break;
            case 2:
                if (n >= 2) {
                    root = deleteKey(root, key);
                    fprintf(out, "Deleted: %s\n", key);
                }
                break;
            case 3:
                fprintf(out, "Tree:\n");
                printTree(root, 0);
                break;
            case 4:
                if (n >= 2) {
                    double* res = findNode(root, key);
                    if (res)
                        fprintf(out, "Found: %s = %.2f\n", key, *res);
                    else
                        fprintf(out, "%s not found\n", key);
                }
                break;
        }
        fprintf(out, "\n");
    }

    fclose(in);
    fclose(out);
}