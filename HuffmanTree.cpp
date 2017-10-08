#include "HuffmanTree.h"
#include "TreeNode.h"
#include "Utilities.h"
using namespace std;

/**
 * Constructor
 */
HuffmanTree::HuffmanTree() {
	root = NULL;
}

/**
 * Destructor
 */
HuffmanTree::~HuffmanTree() {
    /** Empty **/
}

/**
 * A pre order tree walk, left nodes -> right nodes
 * @param node - the node to print
 * @return void
 */
void HuffmanTree::preOrder(TreeNode* node) {
	if (node != NULL) {
		cout << (int)node->character << ", ";
		preOrder(node->leftChild);
		preOrder(node->rightChild);
	}
}

/**
 * Stores the current tree into a bit header for huffman compression
 * @param node - the node
 * @param bits - the bit array
 * @return void
 **/
void HuffmanTree::treeToBits(TreeNode* node, ByteBuffer& buffer) {
    if (node != NULL) {
        buffer.writeBit(node->getCharacter() == -1 ? 0 : 1);
        if (node->getCharacter() != -1) { //leaf node
            buffer.writeByte((int) node->getCharacter());
        }
		treeToBits(node->leftChild, buffer);
		treeToBits(node->rightChild, buffer);
	}
}

/**
 * Gets the node count in the tree
 * @param node - the node
 * @param numNodes - the node count
 * @return void
 **/
void HuffmanTree::getNodeCount(TreeNode* node, unsigned char& numNodes) {
    if (node != NULL) {
        numNodes++;
		getNodeCount(node->leftChild, numNodes);
		getNodeCount(node->rightChild, numNodes);
	}
}

/**
 * Used for re-building tree from stored preorder traversal
 * Gets the next available parent with a free (NULL) left or right node
 * @param node - the node to check
 * @return {TreeNode} - the node with an available (NULL) left or right child
 **/
TreeNode* HuffmanTree::getNextAvailableParent(TreeNode* node) {
    if (node != NULL && node->character == -1 && (node->leftChild == NULL || node->rightChild == NULL))
        return node;
    return getNextAvailableParent(node->parent);
}
