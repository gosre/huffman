#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <vector>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/**
 * Represents a wrapper to a byte buffer (in c++'s case, an unsigned char).
 * Reads and writes various data types from and to the buffer
 * @author Clayton Williams - September 2016
 **/
class ByteBuffer {
    private:
        unsigned char* buffer;
        int offset, length;
        int bitPosition; //the bit position (incase we have to read/write bits instead of bytes)
        unsigned char bits;

        void checkCapacity();
        void checkCapacity(int);
    public:
        ByteBuffer();
        ByteBuffer(unsigned char*, int);
        ByteBuffer(std::vector<unsigned char>);
        void wrapFile(int, bool readFile = true);
        void initDefaults();

        /** Outputstreams **/
        void writeBit(int);
        void writeByte(unsigned char, int);
        void writeByte(unsigned char);
        void writeInt(int);
        /** Inputstreams **/
        unsigned char readBit();
        unsigned char readByte();
        int readInt();
        /** Getters and setters **/
        int getLength();
        int getOffset();
        int getRemaining();
        void trim();
        void flip(bool trim = false);
        void print();
        unsigned char* getBuffer();

};

#endif