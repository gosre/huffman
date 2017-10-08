#include "Huffman.h"
#include <iostream>
#include <map>

using namespace std;

/**
 * Huffman.cpp
 * @author Clayton Williams
 * @date 9-23-16
 **/

/**
 * Huffman
 **/
Huffman::Huffman() {
    /**
     * Empty
     **/
}

/**
 * Compresses the readable file and writes the new bytes to the writeFile
 * @param readFile - the file (uncompressed) to read
 * @param writeFile - the file (compressed) to write to
 * @return void
 **/
void Huffman::compress(int readFile, int writeFile) {

    /** The characters in our file **/
    vector<char> characters;

    /** Read in file **/
    char character;
    while ((read(readFile, &character, 1)) == 1) {
        characters.push_back(character);
    }

    /** Map for sorting weights <ASCII index, weight> **/
    map<int,int> weightMap;

    /**
     * Assigns weights to all characters that are present in the input
     * key = character code
     * value = weight
     **/
    map<int,int>::iterator it;
    for (int i = 0; i < characters.size(); i++) {
        int c = characters[i];
        it = weightMap.find(c);
        int weight = (it == weightMap.end() ? 0 : it->second);
        weight++;
        if (it == weightMap.end())
            weightMap.insert(pair<int,int>(c, weight));
        else
            it->second = weight;
    }

    vector<TreeNode*> nodes;
    /**
     * Constructs a vector of nodes to begin sorting with
     **/
    it = weightMap.begin();
    for (int index = 0; it != weightMap.end(); ++it, index++) {
        TreeNode* node = new TreeNode();
        node->setCharacter(it->first);
        node->setWeight(it->second);
        nodes.push_back(node);
    }

    //sort the nodes
    nodes = sortNodes(nodes);

    /** Create a huffman tree **/
    HuffmanTree tree;

    while (nodes.size() > 1) {
        TreeNode* parent = new TreeNode();
        parent->setWeight(nodes[0]->getWeight() + nodes[1]->getWeight());
        nodes[0]->prependBitRecursively(0);
        nodes[1]->prependBitRecursively(1);
        parent->leftChild = nodes[0];
        parent->rightChild = nodes[1];
        nodes.erase(nodes.begin(), nodes.begin() + 2);
        nodes.push_back(parent);
        nodes = sortNodes(nodes);
        if (nodes.size() == 1)
            tree.setRoot(parent);
    }

    /**
     * We generate a mapping for each character to the coressponding binary form
     *   so that we don't have to traverse through the tree for each character in our input
     *   Specifically makes a map of each character value with its corresponding bit value (according to the tree layout L/R sequence)
     **/
    map<int, vector<int> > binaryMapping;
    generateBinaryMapping(binaryMapping, tree.getRoot());

    // Create a buffer to store our compressed results in
    ByteBuffer buffer;

    //number of nodes in the tree (can only have a max possible value of a byte since the ascii table is a byte) unsigned char = 1 byte
    unsigned char numNodes = 0;
    tree.getNodeCount(tree.getRoot(), numNodes);
    buffer.writeByte(numNodes);

    //store each node (1 for leaf, 0 for internal) value of 1 must store a char value: pre-order traversal
    tree.treeToBits(tree.getRoot(), buffer);

    //write the character count (using c count for end-of-file indexing)
    buffer.writeInt(characters.size()); //might need a bigger count for large files

    /**
     * Then we can write our bits for each character
     **/
    for (int i = 0; i < characters.size(); i++) {
        char c = characters[i];
        vector<int> bits = binaryMapping.find((int) c)->second;
        for (int i2 = 0; i2 < bits.size(); i2++) {
            buffer.writeBit(bits[i2]);
        }
    }

    buffer.trim();

    //write the buffer to the file
    for (int i = 0; i < buffer.getLength(); i++) {
        write(writeFile, &buffer.getBuffer()[i], 1);
    }

    cout << "Successfully compressed " << characters.size() << " bytes into " << buffer.getLength() << " bytes." << endl;

}

/**
 * Decompress - decompresses a compressed huffman file
 * @param readFile - the file descriptor for reading (compressed file)
 * @param writeFile - the file descriptor for writing (write decompressed to)
 * @return void
 **/
void Huffman::decompress(int readFile, int writeFile) {

    /** Initialize a {ByteBuffer} and wrap the file bytes **/
    ByteBuffer buffer;
    buffer.wrapFile(readFile, true);

    /** Decompress in the same way that we compressed **/
    int numNodes = buffer.readByte();

    /** Create a tree **/
    HuffmanTree tree;

    /** Read each bit, 0 = internal node, 1 = leaf node (then read byte for char value) **/
    TreeNode* previous = NULL;
    for (int nodeIndex = 0; nodeIndex < numNodes; nodeIndex++) {
        int bit = buffer.readBit();
        TreeNode* toInsert = new TreeNode();
        if (bit == 1)
            toInsert->character = buffer.readByte();

        if (tree.getRoot() == NULL) {
            tree.setRoot(toInsert);
            previous = toInsert;
        } else {
            if (previous->leftChild == NULL) { //do left child first b/c of pre-rder traversal
                previous->leftChild = toInsert;
            } else if (previous->rightChild == NULL) {
                previous->rightChild = toInsert;
            }
            toInsert->parent = previous;
            if ((nodeIndex + 1) != numNodes)
                previous = (bit == 0) ? toInsert : tree.getNextAvailableParent(toInsert);
        }
    }

    /** The number of characters we have to find; serves as a pseudo eof in a way **/
    int numChars = buffer.readInt();

    ByteBuffer output;

    /** Find each character as per the L/R bit balance stored in the buffer **/
    for (int charIndex = 0; charIndex < numChars; charIndex++) {
        TreeNode* currentNode = tree.getRoot();
        while (true) {
            if (currentNode->character != -1) {
                output.writeByte(currentNode->character);
                break;
            } else {
                int bit = buffer.readBit();
                currentNode = bit == 0 ? currentNode->leftChild : currentNode->rightChild;
            }
        }
    }

    output.trim();

    //write the buffer to the file
    for (int i = 0; i < output.getLength(); i++) {
        write(writeFile, &output.getBuffer()[i], 1);
    }

    cout << "Successfully decompressed file." << endl;

}

/**
 * Generates the binary mapping (char and the representing bits)
 * @param map<int, vector<int> >
 * @param node - the node to get the bits of (if its a leaf)
 * @return void
 **/
void Huffman::generateBinaryMapping(map<int, vector<int> >& map, TreeNode* node) {
    if (node == NULL) {
        return;
    }
    if (node->getCharacter() != -1) { //leaf node
        map.insert(make_pair(node->getCharacter(), node->bits));
        return;
    }
    generateBinaryMapping(map, node->leftChild);
    generateBinaryMapping(map, node->rightChild);
}

/**
 * Makes a sorted array of TreeNode values depending on the weights
 * To be used in creating the encoding tree (insertion sort least to greatest)
 * @param nodes - the nodes to sort
 * @return vector<{TreeNode}> - a sorted node vector
 **/
vector<TreeNode*> Huffman::sortNodes(vector<TreeNode*> nodes) {
    for (int i = 1, j; i < nodes.size(); i++) {
        TreeNode* temp = nodes[i];
        for (j = i - 1; j >= 0 && temp->getWeight() < nodes[j]->getWeight(); j--) {
            nodes[j + 1] = nodes[j];
        }
        nodes[j + 1] = temp;
    }
    return nodes;
}