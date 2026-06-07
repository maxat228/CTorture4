#include <gtest/gtest.h>
extern "C" {
#include "src/functions/functions.h"
}

TEST(BTreeTest, CreateNode) {
    BTreeNode* node = createNode();
    ASSERT_NE(node, nullptr);
    EXPECT_EQ(node->num_keys, 0);
    free(node);
}

TEST(BTreeTest, InsertSingle) {
    BTreeNode* root = insert(NULL, "test", 1.0);
    EXPECT_EQ(root->num_keys, 1);
    EXPECT_STREQ(root->keys[0], "test");
    EXPECT_DOUBLE_EQ(root->values[0], 1.0);
    free(root);
}

TEST(BTreeTest, InsertMultiple) {
    BTreeNode* root = NULL;
    root = insert(root, "b", 2.0);
    root = insert(root, "a", 1.0);
    root = insert(root, "c", 3.0);
    EXPECT_EQ(root->num_keys, 3);
    EXPECT_STREQ(root->keys[0], "a");
    EXPECT_STREQ(root->keys[1], "b");
    EXPECT_STREQ(root->keys[2], "c");
    free(root);
}

TEST(BTreeTest, FindNode) {
    BTreeNode* root = NULL;
    root = insert(root, "key1", 10.0);
    root = insert(root, "key2", 20.0);
    double* val = findNode(root, "key1");
    ASSERT_NE(val, nullptr);
    EXPECT_DOUBLE_EQ(*val, 10.0);
    val = findNode(root, "key3");
    EXPECT_EQ(val, nullptr);
    free(root);
}

TEST(BTreeTest, DeleteLeaf) {
    BTreeNode* root = NULL;
    root = insert(root, "a", 1.0);
    root = insert(root, "b", 2.0);
    root = deleteKey(root, "a");
    EXPECT_EQ(root->num_keys, 1);
    EXPECT_STREQ(root->keys[0], "b");
    free(root);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}