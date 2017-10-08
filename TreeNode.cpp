/**
 * TreeNode.cpp
 * @author Clayton Williams
 */
#include "TreeNode.h"
#include <iostream>

using namespace std;

/**
 * Constructor for TreeNode
 */
TreeNode::TreeNode() {
    this->parent = NULL;
    this->leftChild = NULL;
    this->rightChild = NULL;
    this->character = -1;
    this->weight = 0;
}

/**
 * Sets the character
 * @param character - the character to set
 * @return void
 **/
void TreeNode::setCharacter(int character) {
    this->character = character;
}

/**
 * Gets the data in this node
 * @return int the character
 **/
int TreeNode::getCharacter() {
    return character;
}

/**
 * Sets the weight
 * @param weight - the weight to set
 * @return void
 **/
void TreeNode::setWeight(int weight) {
    this->weight = weight;
}

/**
 * Gets the weight
 * @return int the weight
 **/
int TreeNode::getWeight() {
    return this->weight;
}

/**
 * Prepents a bit recursively to this bit pattern and then all following nodes
 *   this is used in creating a L/R path to use as the compression pair for each character
 * @param bit - the bit to prepend
 * @return void
 **/
void TreeNode::prependBitRecursively(int bit) {
    bits.insert(this->bits.begin(), bit);
    if (leftChild != NULL)
        leftChild->prependBitRecursively(bit);
    if (rightChild != NULL)
        rightChild->prependBitRecursively(bit);
}