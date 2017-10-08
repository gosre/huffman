/**
 * Main class
 * @author Clayton Williams
 * @date September 2016
 */
#include <iostream>
#include "Utilities.h"
#include "Huffman.h"
#include <stdio.h>
#include <sys/types.h>

using namespace std;

/**
 * Main
 **/
int main() {
    /** our huffman program **/
    Huffman huffman;

    /** Read and write file descriptors **/
    int readFile, writeFile;

    Menu menu; //menu class, find it in Utilities.h
	menu.addOption(1, "Compress (Read = uncompressed.txt, Write = compressed.txt)");
	menu.addOption(2, "Decompress (Read = compressed.txt, Write = decompressed.txt)");
	int input = menu.requestInput();
    switch (input) {
        case 1: {
            readFile = open("uncompressed.txt", O_RDONLY);
            writeFile = open("compressed.txt", O_WRONLY | O_CREAT | O_TRUNC);
        }
            break;
        case 2: {
            readFile = open("compressed.txt", O_RDONLY);
            writeFile = open("decompressed.txt", O_WRONLY | O_CREAT | O_TRUNC);
        }
            break;
    }

    if (readFile == -1)
       error("Error opening read file!");

    if (writeFile == -1)
       error("Error opening write file!");

    if (input == 1)
        huffman.compress(readFile, writeFile);
    else
        huffman.decompress(readFile, writeFile);

    /** Close file descriptors **/
    close(readFile);
    close(writeFile);

    exit(0);
	return 0;
}



