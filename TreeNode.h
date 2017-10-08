/**
 * TreeNode.h
 * @author Clayton Williams
 */

#ifndef TREENODE_H
#define TREENODE_H

#include <vector>

/**
 * Represents a node in a tree
 **/
class TreeNode {
	private:
		TreeNode *parent;
		TreeNode *leftChild;
		TreeNode *rightChild;
		int character, weight;
		std::vector<int> bits;
	public:
	    friend class HuffmanTree;
	    friend class Huffman;
		TreeNode();
		int getCharacter();
		int getWeight();
		void setCharacter(int);
		void setWeight(int);
		void prependBitRecursively(int);
};

#endif
