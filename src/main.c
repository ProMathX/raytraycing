#include "stdio.h"
#include "SDL2/SDL.h"
#include "math.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600
#define COLOR_WHITE SDL_MapRGB(surface->format, 255, 255, 255)

struct Circle{
    double x;
    double y;
    double radius;
};

void FillCircle(SDL_Surface* surface, struct Circle circle, Uint32 color){

    double radius_sqrt = pow(circle.radius, 2);

    for (double x = circle.x - circle.radius; x <= circle.x +  circle.radius; x++) {

        for (double y =  circle.y -  circle.radius; y <= circle.y + circle.radius; y++ ) {

            double distance_sqrt = pow(x - circle.x, 2) + pow(y - circle.y, 2);

            if(distance_sqrt < radius_sqrt){

                SDL_Rect pixel= (SDL_Rect){x,y,1,1};

                SDL_FillRect(surface, &pixel, color);
            }
        }
    }

}


int main(int args, char *argv[]){

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED,
                                                        SDL_WINDOWPOS_CENTERED,
                                                        WIDTH,
                                                        HEIGHT,
                                                        0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Rect rect = (SDL_Rect){200,200,200,200};
    //SDL_FillRect(surface, &rect, COLOR_WHITE);

    struct Circle circle = {200,200,80};

    FillCircle(surface, circle, COLOR_WHITE);

    SDL_UpdateWindowSurface(window);


    while (expression) {

        FillCircle(surface, circle, COLOR_WHITE);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(5000);

    }

}
