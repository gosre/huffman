#include "ByteBuffer.h"
/**
 * ByteBuffer.cpp - represents a basic buffer wrapper for read/write capabilities
 * @author Clayton Williams - (Java) May 2011
 * @author Clayton Williams - (Rewrote in C++) September 2016
 * //TODO pre-define bit masks up to index 32
 **/
using namespace std;

/**
 * ByteBuffer
 **/
ByteBuffer::ByteBuffer() {
    initDefaults();
}

/**
 * ByteBuffer
 * @param buffer - the buffer to start with
 * @param length - the length of the buffer
 */
ByteBuffer::ByteBuffer(unsigned char* buffer, int length) {
    this->buffer = buffer;
    this->length = length;
    this->offset = this->bits = this->bitPosition = 0;
}

/**
 * ByteBuffer
 * @param buffer - a vector of unsigned char
 **/
ByteBuffer::ByteBuffer(vector<unsigned char> buffer) {
    this->initDefaults();
    for (int i = 0; i < buffer.size(); i++) {
        writeByte(buffer[i]);
    }
}

/**
 * Initializes defaults of the {ByteBuffer}
 * @return void
 **/
void ByteBuffer::initDefaults() {
    this->length = 16;
    this->buffer = new unsigned char[16];
    this->bits = this->bitPosition = this->offset = 0;
}

/**
 * Takes the given file descriptor and wraps all the bytes inside into this {ByteBuffer}
 * @param fileDescriptor - the file descriptor to read from
 * @param readFile - whether we want to read the file afterwards (it will trim & flip the buffer if so)
 *      if not, then it will keep the offset for writing
 * @return void
 **/
void ByteBuffer::wrapFile(int fileDescriptor, bool readFile) {
    initDefaults();
    unsigned char character;
    int index = 1;
    while ((read(fileDescriptor, &character, 1)) == 1) {
        writeByte(character);
    }
    if (readFile)
        flip(readFile);
}

/**
 * Checks the capacity at a default position increase of 8 bytes
 * @return void
 **/
void ByteBuffer::checkCapacity() {
    checkCapacity(offset + 8);
}

/**
 * Checks the capacity at the given position
 * if the position overflows over the buffer, then a new one is made
 **/
void ByteBuffer::checkCapacity(int position) {
    if (position > getLength()) {
        unsigned char* newBuffer = new unsigned char[this->length = getLength() + 16];
        for (int i = 0; i < getLength(); i++) {
            newBuffer[i] = this->buffer[i];
        }
        delete[] this->buffer;
        this->buffer = newBuffer;
    }
}

/**
 * Writes a bit to the buffer. If the bit appended does not form a full byte chunk,
 *   it will be stored before being placed as a byte
 * @param bit - the bit to write
 * @return void
 **/
void ByteBuffer::writeBit(int bit) {
    this->bits |= (bit == 1) << (7 - bitPosition++);
    if (this->bitPosition == 8) {
        checkCapacity(this->offset);
        this->buffer[this->offset++] = bits;
        this->bits = this->bitPosition = 0;
    }
}

/**
 * Writes a byte to the buffer
 * @param byte - the byte to write
 * @return void
 **/
void ByteBuffer::writeByte(unsigned char byte) {
    writeByte(byte, this->offset);
}

/**
 * Writes a byte to the buffer
 * @param byte - the byte to write
 * @param position - the position to write it at
 * @return void
 **/
void ByteBuffer::writeByte(unsigned char byte, int position) {
    checkCapacity(position);
    if (bitPosition > 0) {
        for (int i = 7; i >= 0; i--) {
            int bit = byte >> i & 1;
            writeBit(bit == 1);
        }
        return;
    }
    this->offset++;
    this->buffer[position] = byte;
}

/**
 * Writes an integer to the buffer (4 bytes, 32 bits) in big endian format
 * @param integer - the integer to write
 * @return void
 **/
void ByteBuffer::writeInt(int integer) {
    writeByte(integer >> 24);
    writeByte(integer >> 16);
    writeByte(integer >> 8);
    writeByte((char) integer);
}

/**
 * Reads a bit from the buffer
 * @return the bit
 **/
unsigned char ByteBuffer::readBit() {
    this->bits = buffer[getOffset()];
    int bit = (bits >> this->bitPosition-- & 1) == 1;
    if (this->bitPosition == -1) {
        this->bits = buffer[++this->offset];
        this->bitPosition = 7;
    }
    return bit;
}

/**
 * Reads a byte from the buffer
 * @return the byte
 **/
unsigned char ByteBuffer::readByte() {
    if (bitPosition < 7) {
        unsigned char byte;
        for (int i = 7; i >= 0; i--) {
            int bit = readBit();
            byte |= (bit == 1) << i;
            //byte |= (readBit() == 1) << i; // why doesn't this one work
        }
        return byte;
    }
    return getRemaining() > 0 ? buffer[this->offset++] : 0;
}

/**
 * Reads an integer from the buffer (4 bytes, 32 bits)
 * @return the integer
 **/
int ByteBuffer::readInt() {
    return (readByte() << 24) + (readByte() << 16) + (readByte() << 8) + readByte();
}

/**
 * Trims the buffer to size and writes garbage bits if bits % 8 != 0
 * This avoids the complication of chunk writing (since operating systems only write in chunks of 8 or 16 bits)
 * @return void
 **/
void ByteBuffer::trim() {
    /** Write out extra garbage bits to fill byte chunk **/
    while (this->bitPosition != 0)
        writeBit(0);
    /** Resize our buffer so that we don't have extra length from capacity checking **/
    unsigned char* newBuffer = new unsigned char[offset];
    for (int i = 0; i < getLength(); i++)
        newBuffer[i] = this->buffer[i];
    delete[] this->buffer;
    this->buffer = newBuffer;
    this->length = offset;
}

/**
 * Flips the buffer to enable new reading/writing
 * @param reading - if we are flipping to read or to write
 * @return void
 **/
void ByteBuffer::flip(bool reading) {
    this->offset = reading ? 0 : getLength();
    this->bitPosition = reading ? 7 : 0;
}

/**
 * Gets the current buffer
 * @return unsigned char[] the buffer
 **/
unsigned char* ByteBuffer::getBuffer() {
    return this->buffer;
}

/**
 * Gets the length of the current buffer (in bytes)
 * @return int - bytes of the buffer
 **/
int ByteBuffer::getLength() {
    return this->length;
}

/**
 * Gets the current offset of the buffer position
 * @return the offset
 **/
int ByteBuffer::getOffset() {
    return this->offset;
}

/**
 * Gets the remaining byte distance between the length and the offset
 * @return int - the remaining bytes
 **/
int ByteBuffer::getRemaining() {
    return this->offset < getLength() ? (getLength() - this->offset) : 0;
}

/**
 * Prints the buffer
 **/
void ByteBuffer::print() {
    cout << "Buffer print out: " << endl;
    for (int i = 0; i < getLength(); i++) {
        unsigned char c = this->buffer[i];
        for (int i2 = 7; i2 >= 0; i2--) {
            cout << ((c >> i2) & 1);
            if (i2 == 0)
                cout << "_";
        }
    }
    if (this->bitPosition > 0) {
        for (int i = 7; i >= 0; i--) {
            if ((7 - bitPosition) < i)
                cout << (int) ((this->bits & (1 << (i))) > 0);
        }
    }
    cout << endl;
}