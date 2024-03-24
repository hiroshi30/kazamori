// ESC - quit app
// c - clear canvas
// 1 - save canvas as 1
// 0 - save canvas as 0

#include <kazamori.h>
#include <print.h>

#include <SDL2/SDL.h>


#define title "app"

#define window_width (16 * 65)
#define window_height (9 * 65)

#define canvas_width (16 * 45)
#define canvas_height window_height

#define pixel_width (16 * 5)
#define pixel_height (9 * 5)


int init();
void quit();
void add(ImageSet* image_set, int index);


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int pixels[canvas_height / pixel_height][canvas_width / pixel_width];


int main(int argc, char* argv[]) {
    if (init() != 0) {
        printf("!!! ERROR in init() !!!\n");
        return 1;
    }
	
    for (int y = 0; y < canvas_height / pixel_height; ++y) {
        for (int x = 0; x < canvas_width / pixel_width; ++x) {
            pixels[y][x] = 0;
        }
    }

    const char* save_file_name = "image_set.txt";
    ImageSet* image_set = ImageSet_construct(0, 3, canvas_height / pixel_height,canvas_width / pixel_width, 2);
    ImageSet_load(image_set, save_file_name);
    ImageSet_print_output(image_set);

    bool run = true;
    bool action = true;
    SDL_Event event;

    bool click_left = false;
    bool click_right = false;

    while (run) {
        while(SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                run = false;
                break;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    run = false;
                    break;
                }
                if (event.key.keysym.sym == SDLK_c) {
                    for (int y = 0; y < canvas_height / pixel_height; ++y) {
                        for (int x = 0; x < canvas_width / pixel_width; ++x) {
                            pixels[y][x] = 0;
                        }
                    }
                    action = true;
                }
                if (event.key.keysym.sym == SDLK_0) {
                    add(image_set, 0);
                }
                if (event.key.keysym.sym == SDLK_1) {
                    add(image_set, 1);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    click_left = true;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    click_right = true;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    click_left = false;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    click_right = false;
                }
            }
        }

        if (click_left) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (x > 0 && x < canvas_width && y > 0 && y < canvas_height) {
                pixels[y / pixel_height][x / pixel_width] = 1;
                action = true;
            }
        }

        if (click_right) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (x > 0 && x < canvas_width && y > 0 && y < canvas_height) {
                pixels[y / pixel_height][x / pixel_width] = 0;
                action = true;
            }
        }


        if (action) {
            action = false;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for (int x = 0; x <= canvas_width; x += pixel_width) {
                SDL_RenderDrawLine(renderer, x, 0, x, canvas_height);
            }
            for (int y = 0; y <= canvas_height; y += pixel_height) {
                SDL_RenderDrawLine(renderer, 0, y, canvas_width, y);
            }

            for (int y = 0; y < canvas_height / pixel_height; ++y) {
                for (int x = 0; x < canvas_width / pixel_width; ++x) {
                    if (pixels[y][x] == 1) {
                        SDL_RenderFillRect(renderer, &(SDL_Rect){
                            x * pixel_width,
                            y * pixel_height,
                            pixel_width,
                            pixel_height
                        });
                    }
                }
            }

            SDL_RenderPresent(renderer);
        }
    }

    ImageSet_save(image_set, save_file_name);
	quit();

	return 0;
}


int init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("!!! ERROR in SDL_Init() !!!\n%s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(title, 100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("!!! ERROR in SDL_CreateWindow() !!!\n%s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("!!! ERROR in SDL_CreateRenderer() !!!\n%s", SDL_GetError());
        return 1;
    }

    return 0;
}


void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void add(ImageSet* image_set, int index) {
    double input[image_set->channels * image_set->height * image_set->width];
    for (int c = 0; c < image_set->channels; ++c) {
        for (int h = 0; h < image_set->height; ++h) {
            for (int w = 0; w < image_set->width; ++w) {
                input[c * image_set->height * image_set->width + h * image_set->width + w] = 255 * pixels[h][w];
            }
        }
    }
    
    double output[image_set->output_length];
    for (int i = 0; i < image_set->output_length; ++i) {
        output[i] = 0;
    }
    output[index] = 1;

    ImageSet_add(image_set, input, output); 
}
