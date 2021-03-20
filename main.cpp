#include <SDL.h>
#include <cstdio>
//#include <SDL_image.h>
#include <SDL_timer.h>
#include <bitset>

// draw a rect
void draw_rect(SDL_Rect rect, SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    SDL_RenderDrawRect(rend, &rect);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderPresent(rend);
}


void draw_surface(uint32_t* image_buffer, uint32_t width, uint32_t height, SDL_Renderer *rend) {
    // creates a surface to load an image into the main memory
    SDL_Surface *surface;

#if IMG_LOAD
    // please provide a path for your image
    surface = IMG_Load("path");
#endif

    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    surface = SDL_CreateRGBSurface(0, width, height, 32,
                                   rmask, gmask, bmask, amask);

    if (surface == NULL) {
        SDL_Log("SDL_CreateRGBSurface() failed: %s", SDL_GetError());
        exit(1);
    }

    surface->pixels = image_buffer;

    // loads image to our graphics hardware memory.
    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);

    if (!tex) {
        SDL_Log("SDL_CreateTextureFromSurface() failed: %s", SDL_GetError());
        exit(1);
    }

    // clears main-memory
    //SDL_FreeSurface(surface);

    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect dest;
    dest.x = 100;
    dest.y = 100;

    // connects our texture with dest to control position
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    // clears the screen
    SDL_RenderClear(rend);
    SDL_RenderCopy(rend, tex, NULL, &dest);
}


// dimentions of a window
const int width = 1024, height = 720;

int main(int argc, char *argv[]) {

    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    // create a window
    SDL_Window *win = SDL_CreateWindow("Video Player",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       width, height, 0);

    // triggers the program that controls
    // your graphics hardware and sets flags
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;

    // creates a renderer to render our images
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);

    // should window be closed
    bool close = false;

    // create starting rect to show later
    SDL_Rect pos;
    pos.x = 100;
    pos.y = 150;
    pos.w = 200;
    pos.h = 200;

    // show rect on set position
    draw_rect(pos, rend);

    // how much should we move rect on click
    int delta = 25;

    uint32_t image[200*200];

    int j = 0;
    for (int i = 0; i < 200 * 200; i++) {
        image[i] = 0xff00ff00;
    }

    draw_surface(image, 200,200 ,rend);

    // loop until not told to close
    while (!close) {
        SDL_Event event;

        // get last event
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                // what should happen on key down
                case SDL_KEYDOWN:
                    switch (event.key.keysym.scancode) {
                        // what should happen on arrow_left
                        case SDL_SCANCODE_LEFT:
                            pos.x -= delta;
                            draw_rect(pos, rend);
                            break;
                        case SDL_SCANCODE_RIGHT:
                            pos.x += delta;
                            draw_rect(pos, rend);
                            break;
                        case SDL_SCANCODE_DOWN:
                            pos.y += delta;
                            draw_rect(pos, rend);
                            break;
                        case SDL_SCANCODE_UP:
                            pos.y -= delta;
                            draw_rect(pos, rend);
                            break;
                        case SDL_SCANCODE_Q:
                            close = true;
                            break;
                        case SDL_SCANCODE_C:
                            // clear the window content
                            SDL_RenderClear(rend);
                            break;
                        default:
                            break;
                    }
                    break;
                    // what happens on closing button click
                case SDL_QUIT: {
                    // handling of close button
                    close = true;
                    break;
                }
            }
        }





        // triggers the double buffers
        // for multiple rendering
        SDL_RenderPresent(rend);

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
    // destroy renderer
    SDL_DestroyRenderer(rend);

    // destroy window
    SDL_DestroyWindow(win);

    // close SDL
    SDL_Quit();


    return 0;
}