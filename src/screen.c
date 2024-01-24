#include "stdio.h"
#include "SDL.h"
#include "screen.h"

#define WIDTH 160
#define HEIGHT 144


SDL_Window* window;
SDL_Renderer* renderer;


int SDL_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Could not initalise SDL, Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("gbemu", 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED, 
        WIDTH*4, 
        HEIGHT*4, 
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Failed to initalise window, Error: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        printf("Failed to initalise renderer, Error: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}


void SDL_close(void) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}


void SDL_update(void) {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
}