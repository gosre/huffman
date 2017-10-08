#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "TreeNode.h"
#include "ByteBuffer.h"
#include <stdlib.h>

/**
 * HuffmanTree - a tree to use as the structure for huffman compression
 * @author Clayton Williams
 * @date September 2016
 **/
class HuffmanTree {
    private:
        TreeNode* root;
    public:
        HuffmanTree();
        ~HuffmanTree();
        void preOrder(TreeNode*);
        void treeToBits(TreeNode*, ByteBuffer&);
        void getNodeCount(TreeNode*, unsigned char&);
        TreeNode* getNextAvailableParent(TreeNode*);
        TreeNode* searchNode(int);
        TreeNode* getRoot() {
            return root;
        }
        void setRoot(TreeNode* root) {
            this->root = root;
        }
};

#endif
