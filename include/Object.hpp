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

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

#include "stdio.h"
#include "stdint.h"

#include "Bytes.hpp"

#ifdef PARSER_HPP
using namespace lex;
#endif


#define OBJECT_METADATA_END          0xFF
#define OBJECT_METADATA_ENTRY_LENGTH 3

// Object namespace
namespace obj {


// Byte chunk storage class
class Chunk : public Bytes {
public:
    // Chunk type
    enum Type {
        HEADER,   // Basic information about the whole file for the linker
        CODE,     // Code blocks
    };
    int type = HEADER;

    // Empty constructor
    Chunk() {}

    // Virtual destructor
    virtual ~Chunk() = default;

    // Construct the correct bytes for the particular chunk type
    virtual void Dump() {}

    // Load the chunk from raw bytes
    virtual void Load() {}

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

    // Dump the actual binary data
    void Dump() override;

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

// Dump the actual binary data
void Label::Dump() {
    if ( isGlobalLabel ) {
        Append( (uint16_t)0x0000 ); // Size of label section

        // Dump and append external labels
        for ( auto it : subLabels ) {
            if ( it.second->external ) {
                it.second->Dump();
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
                    *(uint16_t*)( &code->buffer()[im.position] ) = position;
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

    // Dump the header
    void Dump() override {
        Append( origin );

        // External labels
        labels.Dump();
        Append( &labels );
    }

    // Loads the raw header bytes
    void Load() override {
        origin = *(uint16_t*)buffer();

        // External labels: TODO
    }

    #ifdef PARSER_HPP
    // Returns a pointer to a label or sub-label from that label's token
    Label *GetLabelFromToken( token::Label *token );
    #endif
};

#ifdef PARSER_HPP
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
#endif

// Instruction chunk
class Code : public Chunk {
public:
    Code() {
        type = CODE;
    }
};

// Class to manage the generated object
class Object : public Bytes {
public:
    std::vector<Chunk*> chunks;
    Header *header;
    Code *code;

    // Empty constructor
    Object() {}

    // Loads the object file upon initialization
    Object( const std::string &objectFileName ) {
        Load( objectFileName );
    }

    // Free up chunks when we're done Dumping them
    ~Object() {
        for ( auto chunk : chunks )
            delete chunk;
    }

    // Adds a chunk to the internal list
    void AddChunk( Chunk *chunk ) {
        chunks.push_back( chunk );
    }

    // Construct the chunks into the buffer with the correct metadata
    void Dump();

    // Construct object chunks from raw binary data (opposite of Dump)
    void Load( const std::string &objectFileName );

private:
    // Just loads raw bytes from the supplied file into the object's bytes
    // buffer
    void LoadFromFile( const std::string &objectFileName );

    int GetChunkEntryPos( int chunkEntry ) {
        return chunkEntry * OBJECT_METADATA_ENTRY_LENGTH + 1;
    }

    // Parses a chunk entry and copies the appropriate bytes into the chunk,
    // calls Chunk::Load(), and adds the chunk
    void LoadChunk( Chunk *chunk, int chunkEntry );

};

// Construct the chunks into the buffer with the correct metadata
void Object::Dump() {
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

        chunk->Dump();
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

// Just loads raw bytes from the supplied file into the object's bytes
// buffer
void Object::LoadFromFile( const std::string &objectFileName ) {
    std::ifstream file( objectFileName, std::ios_base::binary );

    if ( !file.is_open() ) {
        std::cout << "Could not find object file \"" << objectFileName
            << "\"!\n";
        return;
    }

    // Calculate file size
    std::streamsize fileSize = file.tellg();
    file.seekg( 0, std::ios::end );
    fileSize = file.tellg() - fileSize;
    file.seekg( 0, std::ios::beg );

    // Resize and copy the bytes into the buffer
    resize( fileSize );
    file.read( (char*)buffer(), fileSize );

    file.close();
}

// Parses a chunk entry and copies the appropriate bytes into the chunk,
// calls Chunk::Load(), and adds the chunk
void Object::LoadChunk( Chunk *chunk, int chunkEntry ) {
    // Calculate chunk count
    int chunkCount = ( buffer()[0] - 1 ) / OBJECT_METADATA_ENTRY_LENGTH;
    int entryPos = GetChunkEntryPos( chunkEntry );

    if ( chunkEntry > chunkCount )
        return;

    // Fetch start
    uint16_t start = *(uint16_t*)( buffer() + entryPos + 1 );

    // Calculate end
    uint16_t end;
    if ( chunkEntry == chunkCount - 1 ) // If last chunk, end = EOF
        end = (uint16_t)size();
    else                                // else, end = beginning of next chunk
        end = *(uint16_t*)(
            buffer() + entryPos + OBJECT_METADATA_ENTRY_LENGTH + 1
        );

    // Do the actual byte loading
    chunk->Append( buffer() + start, end - start );
    
    chunk->Load();
    AddChunk( chunk );
}

// Construct object chunks from raw binary data (opposite of Dump)
void Object::Load( const std::string &objectFileName ) {
    LoadFromFile( objectFileName );

    // Calculate chunk count
    int chunkCount = ( buffer()[0] - 1 ) / OBJECT_METADATA_ENTRY_LENGTH;

    // Iterate through chunk entries and add chunks
    for ( int i = 0; i < chunkCount; i++ ) {
        switch ( buffer()[GetChunkEntryPos( i )] ) {
            case Chunk::HEADER: {
                header = new Header();
                LoadChunk( header, i );
                break;
            }
            case Chunk::CODE: {
                code = new Code();
                LoadChunk( code, i );
                break;
            }
            default:
                std::cout << "Invalid chunk type \""
                    << buffer()[GetChunkEntryPos( i )] << "\"!\n";
                break;
        }
    }
}

}

#endif