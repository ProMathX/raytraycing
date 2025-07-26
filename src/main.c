#include "stdio.h"
#include "SDL2/SDL.h"
#include "math.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>

#define WIDTH 2000
#define HEIGHT 1080
#define COLOR_WHITE SDL_MapRGB(surface->format, 255, 255, 255)
#define COLOR_BLACK SDL_MapRGB(surface -> format, 0, 0, 0)
#define COLOR_GRAY SDL_MapRGB(surface -> format, 65,65,65)
#define COLOR_YELLOW SDL_MapRGB(surface -> format, 255, 255, 0)
#define COLOR_RAY_BLUR SDL_MapRGB(surface -> format, 109,104,10)
#define RAYS_NUMBER 350
#define RAY_THICKNESS 3


struct Circle{
    double x;
    double y;
    double radius;
};

struct circle_ray{
    double start_x,start_y;
    double angle;
    double end_x, end_y;

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

void generate_light(struct Circle circle, struct circle_ray rays[RAYS_NUMBER]){

    for(int i = 0; i < RAYS_NUMBER; i++){

        double angle = ((double) i / RAYS_NUMBER)* 2 * M_PI;
        struct circle_ray ray = {circle.x, circle.y, angle};
        rays[i] = ray;

    }

}

void FillRays(SDL_Surface* surface, struct circle_ray rays[RAYS_NUMBER],Uint32 color, Uint32 blur_color,  struct Circle object){

    double radius_sqrt = pow(object.radius, 2);
    for (int i = 0; i < RAYS_NUMBER; i++){
        struct circle_ray ray = rays[i];

        int end_of_screen = 0;
        int object_hit = 0;

        double step = 1;
        double x_draw = ray.start_x;
        double y_draw = ray.start_y;

        while (!end_of_screen && !object_hit){

            x_draw += step * cos(ray.angle);
            y_draw += step * sin(ray.angle);

            // blurring
            const double blur = 2 * RAY_THICKNESS;
            SDL_Rect ray_blur = (SDL_Rect){x_draw, y_draw,blur, RAY_THICKNESS, RAY_THICKNESS};
            SDL_FillRect(surface, &ray_blur, blur_color);

            // drawing the lines
            SDL_Rect ray_point = (SDL_Rect){x_draw,y_draw, RAY_THICKNESS};
            SDL_FillRect(surface, &ray_point, color);


            if(x_draw < 0 || x_draw > WIDTH){
                end_of_screen = 1;
            }

            if(y_draw < 0 || y_draw > HEIGHT){
                end_of_screen = 1;
            }

            // Ray hitting an object?
            double distance_sqrt = pow(x_draw-object.x, 2) + pow(y_draw-object.y, 2);

            if(distance_sqrt < radius_sqrt){

                break;

            }

        }

    }

}

int main(int args, char *argv[]){

    // Initialising the output
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Raytracing", SDL_WINDOWPOS_CENTERED,
                                                        SDL_WINDOWPOS_CENTERED,
                                                        WIDTH,
                                                        HEIGHT,
                                                        0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Rect rect = (SDL_Rect){400,400,400,400};

    SDL_Rect erase_rect = (SDL_Rect){0,0,WIDTH, HEIGHT};

    // The objects
    struct Circle circle = {100,100,80};
    struct Circle shadow_circle= {650,300,150} ;

    // Casting the rays on the circle
    struct circle_ray rays[RAYS_NUMBER];
    generate_light(circle, rays);

    double obstacle_move_y = 4;

    FillCircle(surface, circle, COLOR_WHITE);

    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0 ) {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
                generate_light(circle, rays);

            }
        }
        SDL_FillRect(surface, &erase_rect, COLOR_BLACK) ;
        FillRays(surface, rays, COLOR_YELLOW, COLOR_RAY_BLUR, shadow_circle);

        FillCircle(surface, circle, COLOR_YELLOW);

        FillCircle(surface, shadow_circle, COLOR_WHITE);

        shadow_circle.y += obstacle_move_y;

        if(shadow_circle.y - shadow_circle.radius < 0){
            obstacle_move_y = -obstacle_move_y;
        }

        if(shadow_circle.y + shadow_circle.radius > HEIGHT){
            obstacle_move_y = -obstacle_move_y;
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);

    }

}
