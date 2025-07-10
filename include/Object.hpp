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

#include "../btp6kasm/lexer.hpp"
using namespace lex;


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

    // Appends a null-terminated string to the buffer
    void Append( std::string &str ) {
        Append( (uint8_t*)str.c_str(), str.size() + 1 );
    }

    // Returns the raw byte buffer
    uint8_t *buffer() const {
        return (uint8_t*)data.data();
    }

    // Returns the buffer size
    size_t size() const {
        return data.size();
    }

    #ifdef DEBUG

    // Prints a Bytes object's bytes
    void PrintBytes() {
        std::cout << "Bytes( ";
        for ( auto byte : data )
            printf("%02X ", byte);
        
        std::cout << ")\n";
    }

    #endif

private:
    std::vector<uint8_t> data;
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

// Instruction labal data - data that an instruction sends to a label
struct ImLabelData {
    enum Type {
        OFFSET,
        ABSOLUTE,
    };
    uint8_t type;
    uint16_t position; // Immediate position
};

// Labels are very similar to chunks, but are not the same
class Label : public Chunk {
public:
    // Label specific data
    std::string value = "";
    uint16_t position = 0x0000;
    bool external = false; // sub-labels cannot be external

    // Constructors
    Label() {
        isGlobalLabel = true;
    }
    Label( std::string &label )
        : value( label ) {}

    // Free children
    ~Label() {
        for ( auto it : subLabels )
            delete it.second;
    }

    // Getter + setter
    Label *operator[]( std::string label ) {
        // If we are requesting the global label
        if ( label == "" )
            return this;

        // Ensure a label exists
        try {
            return subLabels.at( label );
        }
        catch ( ... ) {
            subLabels[label] = new Label( label );

        
            return subLabels.at( label );
        }
    }

    // Build the actual binary data
    void Build() override;

    // Append immediate label data
    void AppendIm( ImLabelData &data ) {
        imData.push_back( data );
    }

    // Insert actual label offsets/adresses into the parsed code
    void CorrectImmediates( uint16_t origin, Bytes *code );

private:
    bool isGlobalLabel = false;
    bool isSubLabel    = false;

    // sub-labels cannot have sub-labels
    std::unordered_map<std::string, Label*> subLabels;
    std::vector<ImLabelData> imData;
};

// Build the actual binary data
void Label::Build() {
    if ( isGlobalLabel ) {
        Append( (uint16_t)0x0000 ); // Size of label section

        // Build and append external labels
        for ( auto it : subLabels ) {
            if ( it.second->external ) {
                it.second->Build();
                Append( it.second );
            }
        }

        *(uint16_t*)buffer() = (uint16_t)size();
    }
    else {
        // Individual label entry
        Append( value );    // Label string
        Append( position ); // Label binary position
    }
}

// Insert actual label offsets/adresses into the parsed code
void Label::CorrectImmediates( uint16_t origin, Bytes *code ) {
    // Iterate through children
    for ( auto it : subLabels )
        it.second->CorrectImmediates( origin, code );
    
    if ( !isGlobalLabel ) {
        for ( auto im : imData ) {
            switch ( im.type ) {
                case ImLabelData::OFFSET:
                    code->buffer()[im.position] =
                        (int8_t)( position - im.position - 1 );
                    break;
                case ImLabelData::ABSOLUTE:
                    *(uint16_t*)( &code->buffer()[im.position] )
                        = origin + position;
                    break;
            }
        }
    }
}


// Basic information about the whole file for the linker
// Also includes external references
class Header : public Chunk {
public:
    uint16_t origin;     // .org

    Label labels;
    std::string labelScope = ""; // Global (ik this is bad)

    Header() {
        type = HEADER;
    }

    // Build the header
    void Build() override {
        Append( origin );

        // Labels
        labels.Build();
        Append( &labels );
    }

    // Returns a pointer to a label or sub-label from that label's token
    Label *GetLabelFromToken( token::Label *token );
};

// Returns a pointer to a label or sub-label from that label's token
Label *Header::GetLabelFromToken( token::Label *token ) {
    Label *label;

    if ( token->value[0] == '.' ) {
        label =
            ( *labels[labelScope] )
            [token->value];
    }
    else {
        label = labels[token->value];
        labelScope = label->value;
    }

    return label;
}

// Instruction chunk
class Code : public Chunk {
public:
    Code() {
        type = CODE;
    }
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

    // And correct immedate labels - this has to be run after the code chunk
    // has been generated
    header->labels.CorrectImmediates( header->origin, code );

    // Actually add the data
    for ( Chunk *chunk : chunks ) {
        Append( chunk );
    }
}

}

#endif