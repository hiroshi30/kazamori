#ifdef Debug
#include <stdio.h>
#endif // Debug

#include <SDL.h>
#include <SDL_ttf.h>


SDL_Window* window;
SDL_Renderer* renderer;


int init(const char* window_title, int window_width, int window_height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        #ifdef Debug
        printf("!!! ERROR in SDL_Init(): %s !!!\n", SDL_GetError());
        #endif // Debug

        return 1;
    }

    window = SDL_CreateWindow(window_title, 100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        #ifdef Debug
        printf("!!! ERROR in SDL_CreateWindow(): %s !!!\n", SDL_GetError());
        #endif // Debug
        
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        #ifdef Debug
        printf("!!! ERROR in SDL_CreateRenderer(): %s !!!\n", SDL_GetError());
        #endif // Debug
        
        return 1;
    }

    TTF_Init();

    return 0;
}


void quit() {
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void update() {
    SDL_RenderPresent(renderer);
}
