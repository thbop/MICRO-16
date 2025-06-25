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

#include "stdint.h"
#include "string.h"

#include "SDL3/SDL.h"

// If x, print SDL_GetError()
#define MiDi16_ASSERT( x ) \
    if ( ( x ) ) \
        std::cout << SDL_GetError()

// Micro Display 16 namespace
namespace MiDi16 {
    
    // Window class to manage SDL3 windows
    class Window {
    public:
        Window() {}
        // Basic constructor
        Window( const char *title, int width, int height ) {
            // Initialize SDL
            SDL_Init( SDL_INIT_VIDEO );

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

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Event event;

        bool running = true;
    };

    // Pulls basic events like window close
    void Window::PollEvents() {
        while ( SDL_PollEvent( &event ) ) {
            switch ( event.type ) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
            }
        }
    }

    // Simple 32bit RGBA color
    using Color = SDL_Color;

    // Surface class
    class Surface {
    public:
        uint32_t *pixels;

        Surface() {}

        // Allocates the pixel data buffer
        Surface( int width, int height ) : width( width ), height( height ) {
            pixels = new uint32_t[width * height];
        }

        // Deallocates and destroys surface resources
        ~Surface() {
            delete[] pixels;
            if ( texture != NULL )
                SDL_DestroyTexture( texture );
        }

        // Gets the color value at the specified pixel
        Color Get( int x, int y ) const {
            return *(Color*)( pixels + y * width + x );
        }

        // Sets the color value at the specified pixel
        void Set( int x, int y, Color color ) {
            *(Color*)( pixels + ( y * width ) + x ) = color;
        }

        // Blits the Surface to the window
        // MiDi16 only supports one window
        void Blit( Window *window, int x, int y );

        // Blits and scales the surface to fill the window
        void BlitFill( Window *window );

        // Sets all pixels to black
        void Clear() {
            memset( pixels, 0, width * height * sizeof( uint32_t ) );
        }

    private:
        int width, height;
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
                SDL_PIXELFORMAT_ABGR8888,
                SDL_TEXTUREACCESS_STREAMING,
                width, height
            );
            SDL_SetTextureScaleMode( texture, SDL_SCALEMODE_NEAREST );
        }
        
        SDL_UpdateTexture(
            texture, NULL, pixels,
            width * sizeof( uint32_t )
        );
    }

    // Blits the Surface to the window
    // MiDi16 only supports one window
    void Surface::Blit( Window *window, int x, int y ) {
        Update( window );

        SDL_FRect rect = { (float)x, (float)y, (float)width, (float)height };

        SDL_RenderTexture(
            window->GetSDLRenderer(),
            texture, NULL, &rect
        );
    }

    // Blits and scales the surface to fill the window
    void Surface::BlitFill( Window *window ) {
        Update( window );

        SDL_RenderTexture(
            window->GetSDLRenderer(),
            texture, NULL, NULL
        );
    }
}


#endif