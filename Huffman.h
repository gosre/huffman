#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "HuffmanTree.h"
#include "ByteBuffer.h"
#include <vector>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/**
 * Represents the huffman classs
 * @author Clayton Williams
 **/
class Huffman {
    private:

    public:
        Huffman();
        void compress(int readFile, int writeFile);
        void decompress(int readFile, int writeFile);
        std::vector<TreeNode*> sortNodes(std::vector<TreeNode*>);
        void generateBinaryMapping(std::map<int, std::vector<int> >&,TreeNode*);

};

#endif