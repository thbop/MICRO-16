/*
* Copyright © 2025 Micro-16 Team
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the “Software”), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
* of the Software, and to permit persons to whom the Software is furnished to do
* so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#ifndef OBJECT_HPP
#define OBJECT_HPP

// Object constructor

#include <string>
#include <vector>
#include <unordered_map>

#include "stdio.h"
#include "stdint.h"


#define OBJECT_METADATA_END          0xFF
#define OBJECT_METADATA_ENTRY_LENGTH 3

// Object namespace
namespace obj {

// A byte storage class
class Bytes {
public:
    // Empty constructor
    Bytes() {}

    // Appends a byte to the buffer
    void Append( uint8_t byte ) {
        data.push_back( byte );
        // PrintBytes();
    }

    // Appends a word to the buffer
    void Append( uint16_t word ) {
        data.push_back( (uint8_t)( word & 0xFF ) );
        data.push_back( (uint8_t)( word >> 8 ) );
        // PrintBytes();
    }

    // Appends a string of bytes to the buffer
    void Append( uint8_t *bytes, size_t size ) {
        size_t oldSize = data.size();
        data.resize( oldSize + size );

        memcpy( data.data() + oldSize, bytes, size );
    }

    // Appends a Bytes object to the buffer
    void Append( Bytes *bytes ) {
        Append( (uint8_t*)bytes->buffer(), bytes->size() );
    }

    // Returns the raw byte buffer
    uint8_t *buffer() const {
        return (uint8_t*)data.data();
    }

    // Returns the buffer size
    size_t size() const {
        return data.size();
    }

private:
    std::vector<uint8_t> data;

    #ifdef DEBUG

    // Prints a Bytes object's bytes
    void PrintBytes() {
        std::cout << "Bytes( ";
        for ( auto byte : data )
            printf("%02X ", byte);
        
        std::cout << ")\n";
    }

    #endif
};

// Byte chunk storage class
class Chunk : public Bytes {
public:
    // Chunk type
    enum Type {
        HEADER,   // Basic information about the whole file for the linker
        CODE,     // Code blocks
        DATA,     // Data blocks
    };
    int type = HEADER;

    // Empty constructor
    Chunk() {}

    // Virtual destructor
    virtual ~Chunk() = default;

    // Construct the correct bytes for the particular chunk type
    virtual void Build() {}

};


// Basic information about the whole file for the linker
// Also includes external references
class Header : public Chunk {
public:
    uint16_t origin; // .org

    Header() {
        type = HEADER;
    }

    // Build the header
    void Build() override {
        Append( origin );
    }
};

// Instruction chunk
class Code : public Chunk {
public:
    Code() {
        type = CODE;
    }

    // Adds a label
    void AddLabel() {}

private:
    std::unordered_map<std::string, uint16_t> labels;
};

// Data chunk does nothing special
using Data = Chunk;

// Class to manage the generated object
class Object : public Bytes {
public:
    std::vector<Chunk*> chunks;
    Header *header;
    Code *code;

    // Setup basic object
    Object() {
        header = new Header();
        code   = new Code();

        chunks.push_back( header );
        chunks.push_back( code );
    }

    // Free up chunks when we're done building them
    ~Object() {
        for ( auto chunk : chunks )
            delete chunk;
    }

    // Adds a chunk to the internal list
    void AddChunk( Chunk *chunk ) {
        chunks.push_back( chunk );
    }

    // Construct the chunks into the buffer with the correct metadata
    void Build();

};

// Construct the chunks into the buffer with the correct metadata
void Object::Build() {
    // Each metadata entry is 3 bytes, the first byte gives the length of the
    // metadata section
    // This is kinda like a very basic file system to guide the linker
    Append( (uint8_t)0 ); // Metadata size
    uint16_t pointer = chunks.size() * OBJECT_METADATA_ENTRY_LENGTH + 1;
    for ( Chunk *chunk : chunks ) {
        // Each entry contains the chunk type (u8)
        Append( (uint8_t)chunk->type );
        // And a pointer (u16) to that chunk
        Append( pointer );

        chunk->Build();
        pointer += (uint16_t)chunk->size(); // Increment to the next chunk
    }
    buffer()[0] = (uint8_t)size();

    // Actually add the data
    for ( Chunk *chunk : chunks ) {
        Append( chunk );
    }
}

}

#endif