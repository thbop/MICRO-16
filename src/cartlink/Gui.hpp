/*
* Copyright © 2025 Micro-16 Team
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the “Software”), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
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

#ifndef GUI_HPP
#define GUI_HPP

#define GUI_FONT              "resources/font.png"
#define GUI_FONT_GLYPH_WIDTH  3
#define GUI_FONT_GLYPH_HEIGHT 5

// A little software-rendered gui library (without a proper name)

#include <vector>
#include "../MiDi16/MicroDisplay16.hpp"

// GUI namespace
namespace gui {

// Basic GUI element
class Element {
public:
    MiDi16::Window *window;
    MiDi16::Surface *screen;

    MiDi16::Rect rect;

    // Constructor
    Element(
        MiDi16::Window *window,
        MiDi16::Surface *screen,
        MiDi16::Rect rect
    )
        : window( window ), screen( screen ), rect( rect ) {}

    // Virtual destructor
    virtual ~Element() = default;

    // Called once per frame to update the element
    virtual void Run() {}

};

// Text box from scratch
class TextBox : public Element {
public:
    // Constructor
    TextBox(
        MiDi16::Window *window,
        MiDi16::Surface *screen,
        MiDi16::Rect rect
    )
    : Element( window, screen, rect ) {
        lines.push_back( "" );
        font = new MiDi16::Font(
            "resources/font.png",
            GUI_FONT_GLYPH_WIDTH,
            GUI_FONT_GLYPH_HEIGHT
        );
        font->Render( "" );

        renderedText = new MiDi16::Surface( rect.w, rect.h );
    }

    // Destructor
    ~TextBox() {
        delete font;
        delete renderedText;
    }

    // Returns the textbox text
    std::string text() const {
        std::string text;
        for ( auto line : lines )
            text += line + '\n';
        
        return text;
    }

    // Runs the textbox
    void Run() override;

private:
    std::vector<std::string> lines;
    MiDi16::Surface *renderedText;
    MiDi16::Font *font;
    int cursorX = 0;
    int cursorY = 0;

    // Super optimized filler
    // Clears just one line
    void ClearLine( int line ) {
        int lineSize = renderedText->surface->pitch *
                        ( GUI_FONT_GLYPH_HEIGHT + 1 );
        memset(
            (uint8_t*)renderedText->surface->pixels + lineSize * line,
            0, lineSize
        );
    }

};

// Runs the textbox
void TextBox::Run() {
    bool update = false;
    
    // Text input
    window->StartTextInput();
    std::string textInput = window->GetTextInput();
    if ( textInput.size() ) {
        lines[cursorY] += textInput;
        update = true;
        window->StopTextInput();
    }
    // Backspace
    if (
        window->IsKeyPressed( MiDi16::KEY_BACKSPACE ) &&
        lines[cursorY].size()
    ) {
        lines[cursorY].pop_back();
        update = true;
    }

    if ( update ) {
        font->Render( lines[cursorY].c_str() );
        ClearLine( cursorY );
        renderedText->Blit( font, 0, cursorY * ( GUI_FONT_GLYPH_HEIGHT + 1 ) );
    }

    screen->Blit( renderedText, rect.x, rect.y );

}
    
};


#endif