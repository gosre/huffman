EXECUTABLE=Huffman
  
all: $(EXECUTABLE)

Utilities.o: Utilities.cpp
	g++ -c Utilities.cpp
	
Main.o: Main.cpp
	g++ -c Main.cpp

TreeNode.o: TreeNode.cpp
	g++ -c TreeNode.cpp

Huffman.o: Huffman.cpp
	g++ -c Huffman.cpp

HuffmanTree.o: HuffmanTree.cpp
	g++ -c HuffmanTree.cpp

ByteBuffer.o: ByteBuffer.cpp
	g++ -c ByteBuffer.cpp
	
$(EXECUTABLE): Utilities.o Main.o TreeNode.o Huffman.o HuffmanTree.o ByteBuffer.o
	g++ Utilities.o Main.o TreeNode.o Huffman.o HuffmanTree.o ByteBuffer.o -o $(EXECUTABLE)
	
clean:
	rm -rf *.o $(EXECUTABLE)
