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

#ifndef MICRODISPLAY16_HPP
#define MICRODISPLAY16_HPP

#include <iostream>
#include <vector>

#include "stdint.h"
#include "string.h"

#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"

// If x, print SDL_GetError()
#define MiDi16_ASSERT( x ) \
    if ( ( x ) ) \
        std::cout << SDL_GetError()

// Micro Display 16 namespace
namespace MiDi16 {

// Keyboard enums
enum Key {
    KEY_A = SDL_SCANCODE_A,
    KEY_B = SDL_SCANCODE_B,
    KEY_C = SDL_SCANCODE_C,
    KEY_D = SDL_SCANCODE_D,
    KEY_E = SDL_SCANCODE_E,
    KEY_F = SDL_SCANCODE_F,
    KEY_G = SDL_SCANCODE_G,
    KEY_H = SDL_SCANCODE_H,
    KEY_I = SDL_SCANCODE_I,
    KEY_J = SDL_SCANCODE_J,
    KEY_K = SDL_SCANCODE_K,
    KEY_L = SDL_SCANCODE_L,
    KEY_M = SDL_SCANCODE_M,
    KEY_N = SDL_SCANCODE_N,
    KEY_O = SDL_SCANCODE_O,
    KEY_P = SDL_SCANCODE_P,
    KEY_Q = SDL_SCANCODE_Q,
    KEY_R = SDL_SCANCODE_R,
    KEY_S = SDL_SCANCODE_S,
    KEY_T = SDL_SCANCODE_T,
    KEY_U = SDL_SCANCODE_U,
    KEY_V = SDL_SCANCODE_V,
    KEY_W = SDL_SCANCODE_W,
    KEY_X = SDL_SCANCODE_X,
    KEY_Y = SDL_SCANCODE_Y,
    KEY_Z = SDL_SCANCODE_Z,
    KEY_1 = SDL_SCANCODE_1,
    KEY_2 = SDL_SCANCODE_2,
    KEY_3 = SDL_SCANCODE_3,
    KEY_4 = SDL_SCANCODE_4,
    KEY_5 = SDL_SCANCODE_5,
    KEY_6 = SDL_SCANCODE_6,
    KEY_7 = SDL_SCANCODE_7,
    KEY_8 = SDL_SCANCODE_8,
    KEY_9 = SDL_SCANCODE_9,
    KEY_0 = SDL_SCANCODE_0,
    KEY_ENTER = SDL_SCANCODE_RETURN,
    KEY_ESC = SDL_SCANCODE_ESCAPE,
    KEY_BACKSPACE = SDL_SCANCODE_BACKSPACE,
    KEY_TAB = SDL_SCANCODE_TAB,
    KEY_SPACE = SDL_SCANCODE_SPACE,
    KEY_MINUS = SDL_SCANCODE_MINUS,
    KEY_EQUALS = SDL_SCANCODE_EQUALS,
    KEY_LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET,
    KEY_RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET,
    KEY_BACKSLASH = SDL_SCANCODE_BACKSLASH,
    KEY_SEMICOLON = SDL_SCANCODE_SEMICOLON,
    KEY_APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
    KEY_GRAVE = SDL_SCANCODE_GRAVE,
    KEY_COMMA = SDL_SCANCODE_COMMA,
    KEY_PERIOD = SDL_SCANCODE_PERIOD,
    KEY_SLASH = SDL_SCANCODE_SLASH,
    KEY_CAPSLOCK = SDL_SCANCODE_CAPSLOCK,
    KEY_F1 = SDL_SCANCODE_F1,
    KEY_F2 = SDL_SCANCODE_F2,
    KEY_F3 = SDL_SCANCODE_F3,
    KEY_F4 = SDL_SCANCODE_F4,
    KEY_F5 = SDL_SCANCODE_F5,
    KEY_F6 = SDL_SCANCODE_F6,
    KEY_F7 = SDL_SCANCODE_F7,
    KEY_F8 = SDL_SCANCODE_F8,
    KEY_F9 = SDL_SCANCODE_F9,
    KEY_F10 = SDL_SCANCODE_F10,
    KEY_F11 = SDL_SCANCODE_F11,
    KEY_F12 = SDL_SCANCODE_F12,
    KEY_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
    KEY_SCROLLOCK = SDL_SCANCODE_SCROLLLOCK,
    KEY_PAUSE = SDL_SCANCODE_PAUSE,
    KEY_INSERT = SDL_SCANCODE_INSERT,
    KEY_HOME = SDL_SCANCODE_HOME,
    KEY_PAGEUP = SDL_SCANCODE_PAGEUP,
    KEY_DELETE = SDL_SCANCODE_DELETE,
    KEY_END = SDL_SCANCODE_END,
    KEY_PAGEDOWN = SDL_SCANCODE_PAGEDOWN,
    KEY_RIGHT = SDL_SCANCODE_RIGHT,
    KEY_LEFT = SDL_SCANCODE_LEFT,
    KEY_DOWN = SDL_SCANCODE_DOWN,
    KEY_UP = SDL_SCANCODE_UP,
};
// Fancy SDL_Rect wrapping
// TODO: Make fancy class (if needed)
using Rect = SDL_Rect;

// Window class to manage SDL3 windows
class Window {
public:
    Window() {}
    // Basic constructor
    Window( const char *title, int width, int height ) {
        // Initialize SDL
        MiDi16_ASSERT( !SDL_Init( SDL_INIT_VIDEO ) );
        // Create window
        window = SDL_CreateWindow(
            title, width, height,
            SDL_WINDOW_HIGH_PIXEL_DENSITY
        );
        MiDi16_ASSERT( window == NULL );
        // Create renderer
        renderer = SDL_CreateRenderer( window, NULL );
        MiDi16_ASSERT( renderer == NULL );
    }
    // Basic destructor
    ~Window() {
        SDL_DestroyRenderer( renderer );
        SDL_DestroyWindow( window );
    }
    // Returns the SDL_Renderer
    SDL_Renderer *GetSDLRenderer() const {
        return renderer;
    }
    // Returns if the window should be open
    bool IsRunning() const {
        return running;
    }
    // Pulls basic events like window close
    void PollEvents();
    // Updates the window
    void Flip() {
        SDL_RenderPresent( renderer );
    }
    // Checks if a key is down
    bool IsKeyDown( int scancode ) {
        const bool *keys = SDL_GetKeyboardState( NULL );
        return keys[scancode];
    }
    // Checks if a key is pressed
    bool IsKeyPressed( int scancode ) {
        for ( auto it : pressedKeys ) {
            if ( it == scancode )
                return true;
        }
        return false;
    }
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    std::vector<int> pressedKeys;
    bool running = true;
};
// Pulls basic events like window close
void Window::PollEvents() {
    pressedKeys.clear();
    while ( SDL_PollEvent( &event ) ) {
        switch ( event.type ) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                // Push pressed key to the list
                if ( !event.key.repeat )
                    pressedKeys.push_back( event.key.scancode );
                break;
        }
    }
}
// Simple 32bit RGBA color
using Color = SDL_Color;
// Surface class
class Surface {
public:
    SDL_Surface *surface;
    // Empty constructor
    Surface() {}
    // SDL_Surface constructor
    Surface( SDL_Surface *surface ) : surface( surface ) {}
    // Allocates the pixel data buffer
    Surface( int width, int height ) {
        surface = SDL_CreateSurface(
            width, height,
            SDL_PIXELFORMAT_XBGR8888
        );
    }
    // Construct a surface from an image
    Surface ( const char *imageFile ) {
        // Load and convert image to the correct format
        surface = IMG_Load( imageFile );
        if ( surface->format != SDL_PIXELFORMAT_XBGR8888 ) {
            SDL_Surface *convertedSurface =
                SDL_ConvertSurface( surface, SDL_PIXELFORMAT_XBGR8888 );
            SDL_DestroySurface( surface );
            surface = convertedSurface;
        }
    }
    // Deallocates and destroys surface resources
    ~Surface() {
        if ( surface != NULL )
            SDL_DestroySurface( surface );
        if ( texture != NULL )
            SDL_DestroyTexture( texture );
    }
    // Returns the width of the surface
    int width() const {
        return surface->w;
    }
    // Returns the height of the surface
    int height() const {
        return surface->h;
    }
    // Gets the color value at the specified pixel
    Color Get( int x, int y ) const {
        return *(Color*)( (uint32_t*)surface->pixels + y * width() + x );
    }
    // Sets the color value at the specified pixel
    void Set( int x, int y, Color color ) {
        *(Color*)( (uint32_t*)surface->pixels + y * width() + x ) = color;
    }
    // Blits the Surface to the window
    // MiDi16 only supports one window
    void Blit( Window *window, int x, int y );
    // Blits another Surface onto this surface
    void Blit( Surface *surface, Rect srcRect, Rect dstRect );
    // Blits another Surface onto this surface
    void Blit( Surface *surface, int x, int y );
    // Blits and scales the surface to fill the window
    void BlitFill( Window *window );
    // Sets all pixels to black
    void Clear() {
        memset( surface->pixels, 0, surface->pitch * height() );
    }
private:
    SDL_Texture *texture = NULL;
    // Updates the surface to get it ready for rendering
    void Update( Window *window );
};
// Updates the surface to get it ready for rendering
void Surface::Update( Window *window ) {
    // Create the texture if necessary
    if ( texture == NULL ) {
        texture = SDL_CreateTexture(
            window->GetSDLRenderer(),
            surface->format,
            SDL_TEXTUREACCESS_STREAMING,
            width(), height()
        );
        SDL_SetTextureScaleMode( texture, SDL_SCALEMODE_NEAREST );
    }
    
    SDL_UpdateTexture( texture, NULL, surface->pixels, surface->pitch );
}
// Blits the Surface to the window
// MiDi16 only supports one window
void Surface::Blit( Window *window, int x, int y ) {
    Update( window );
    SDL_FRect rect = { (float)x, (float)y, (float)width(), (float)height() };
    SDL_RenderTexture(
        window->GetSDLRenderer(),
        texture, NULL, &rect
    );
}
// Blits another Surface onto this surface
void Surface::Blit( Surface *src, Rect srcRect, Rect dstRect ) {
    SDL_BlitSurface( src->surface, &srcRect, surface, &dstRect );
}
// Blits another Surface onto this surface
void Surface::Blit( Surface *src, int x, int y ) {
    SDL_Rect
        srcRect = { 0, 0, src->width(), src->height() },
        dstRect = { x, y, srcRect.w, srcRect.h };
    SDL_BlitSurface( src->surface, &srcRect, surface, &dstRect );
}
// Blits and scales the surface to fill the window
void Surface::BlitFill( Window *window ) {
    Update( window );
    SDL_RenderTexture(
        window->GetSDLRenderer(),
        texture, NULL, NULL
    );
}
class Font : public Surface {
public:
    // Empty constructor
    Font() {}
    // Constructor
    // `fontFile` must be an image with a set of glyphs in ASCII order
    // (starting with space and ending with ~) on a single line without
    // padding.
    Font(
        const char *fontFile,
        int glyphWidth,
        int glyphHeight
    ) : Surface( fontFile ),
        glyphWidth( glyphWidth ),
        glyphHeight( glyphHeight ) {
        
        glyphs = surface;
        surface = nullptr;
    }
    // Renders text to the font surface
    // Ensure that the text is null-terminated
    void Render( const char *text );
    
private:
    int
        glyphWidth,
        glyphHeight;
    
    SDL_Surface *glyphs;

    // Gets a character's source rectangle
    Rect SampleChar( char character ) {
        return {
            ( character - ' ' ) * glyphWidth, 0,
            glyphWidth, glyphHeight
        };
    }
};
// Renders text to the font surface
// Ensure that the text is null-terminated
void Font::Render( const char *text ) {
    // Clear the last render
    if ( surface != nullptr )
        SDL_DestroySurface( surface );
    
    int stringLength = (int)strlen( text );
    int width = stringLength * ( glyphWidth + 1 );
    surface = SDL_CreateSurface( width, glyphHeight, glyphs->format );
    
    SDL_Rect
        srcRect,
        dstRect = { 0, 0, glyphWidth, glyphHeight };

    for ( int i = 0; i < stringLength; i++ ) {
        srcRect = SampleChar( text[i] );
        SDL_BlitSurface( glyphs, &srcRect, surface, &dstRect );

        dstRect.x += glyphWidth + 1;
    }
}

}


#endif