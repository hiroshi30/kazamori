#include <GUI.h>

#include <core.h>
#ifdef Debug
#include <print.h>
#endif // Debug


#define window_title "Digit Recognition"

#define canvas_width (20 * 30)
#define canvas_height (20 * 30)

#define pixel_width 30
#define pixel_height 30

#define button_width 230
#define button_height 45

#define padding_x 35
#define padding_y 10

#define buttons_count 4

#define window_width canvas_width + 2 * padding_x + button_width
#define window_height canvas_height

#define pixels_width canvas_width / pixel_width
#define pixels_height canvas_height / pixel_height


int pixels[pixels_height][pixels_width];


void pixels_clear();
void add(ImageSet* image_set, int index);
void allocate_all(
    ImageSet* image_set,
    Convolution* conv1,
    MaxPooling* max_pool1,
    Convolution* conv2,
    MaxPooling* max_pool2,
    FullConnected* full_conn
);
void deallocate_all(
    ImageSet* image_set,
    Convolution* conv1,
    MaxPooling* max_pool1,
    Convolution* conv2,
    MaxPooling* max_pool2,
    FullConnected* full_conn
);
void forward(
    ImageSet* image_set,
    Convolution* conv1,
    MaxPooling* max_pool1,
    Convolution* conv2,
    MaxPooling* max_pool2,
    FullConnected* full_conn
);


int main(int argc, char* argv[]) {
    if (init(window_title, window_width, window_height) != 0) {
        return 1;
    }

    pixels_clear();

    Canvas canvas;
    Canvas_create(&canvas,
        0, 0,
        canvas_width, canvas_height,
        &(SDL_Color){0, 0, 0, 255}
    );

    Button buttons[buttons_count];
    for (int i = 0; i < buttons_count; ++i) {
        Button_create(&buttons[i],
            canvas_width + padding_x,
            (canvas_height - buttons_count * button_height - (buttons_count - 1) * padding_y) / 2 + (button_height + padding_y) * i,
            button_width,
            button_height,
            3,
            20,
            &(SDL_Color){182, 158, 203, 255},
            &(SDL_Color){50, 50, 50, 255},
            25,
            ""
        );
    }
    buttons[0].text = "Save as 0";
    buttons[1].text = "Save as 1";
    buttons[2].text = "Clear";
    buttons[3].text = "Save ImageSet";


    SDL_Color color = {255, 255, 255, 255};


    ImageSet image_set;
    Convolution conv1;
    MaxPooling max_pool1;
    Convolution conv2;
    MaxPooling max_pool2;
    FullConnected full_conn;
    allocate_all(&image_set, &conv1, &max_pool1, &conv2, &max_pool2, &full_conn);
    

    int mouse_x, mouse_y;
    bool mouse_left = false;
    bool mouse_right = false;

    bool need_update = true;
    bool run = true;
    SDL_Event event;
    while (run) {
        if (!mouse_left) {
            for (int i = 0; i < buttons_count; ++i) {
                if (buttons[i].mouse_on == true) {
                    need_update = true;
                    buttons[i].mouse_on = false;
                }
            }
        }

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
            }

            if (event.button.button == SDL_BUTTON_LEFT) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    mouse_left = true;
                    SDL_GetMouseState(&mouse_x, &mouse_y);

                    if (Button_collide(&buttons[0], mouse_x, mouse_y)) {
                        add(&image_set, 0);
                        #ifdef Debug
                        ImageSet_print_output(&image_set);
                        #endif // Debug
                    }
                    else if (Button_collide(&buttons[1], mouse_x, mouse_y)) {
                        add(&image_set, 1);
                        #ifdef Debug
                        ImageSet_print_output(&image_set);
                        #endif // Debug
                    }
                    else if (Button_collide(&buttons[2], mouse_x, mouse_y)) {
                        need_update = true;
                        pixels_clear();
                    }
                    else if (Button_collide(&buttons[3], mouse_x, mouse_y)) {
                        ImageSet_save(&image_set);
                    }
                }
                if (event.type == SDL_MOUSEBUTTONUP) {
                    mouse_left = false;
                }
            }

            if (event.button.button == SDL_BUTTON_RIGHT) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    mouse_right = true;
                }
                if (event.type == SDL_MOUSEBUTTONUP) {
                    mouse_right = false;
                }
            }

            if (mouse_left) {
                need_update = true;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                if (Canvas_collide(&canvas, mouse_x, mouse_y)) {
                    pixels[mouse_y / pixel_height][mouse_x / pixel_width] = 1;
                }
            }

            if (mouse_right) {
                need_update = true;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                if (Canvas_collide(&canvas, mouse_x, mouse_y)) {
                    pixels[mouse_y / pixel_height][mouse_x / pixel_width] = 0;
                }
            }
        }

        if (need_update) {
            need_update = false;

            Canvas_clear(&canvas);
            
            for (int x = 0; x <= pixels_width; ++x) {
                Canvas_draw_line(&canvas, x * pixel_width, 0, x * pixel_width, canvas_height, &color);
            }
            for (int y = 0; y <= pixels_height; ++y) {
                Canvas_draw_line(&canvas, 0, y * pixel_height, canvas_width, y * pixel_height, &color);
            }

            for (int y = 0; y < pixels_height; ++y) {
                for (int x = 0; x < pixels_width; ++x) {
                    if (pixels[y][x] == 1) {
                        Canvas_draw_rect(&canvas, x * pixel_width, y * pixel_height, pixel_width, pixel_height, &color);
                    }
                }
            }

            for (int i = 0; i < buttons_count; ++i) {
                Button_draw(&buttons[i]);
            }

            update();
        }
    }

    deallocate_all(&image_set, &conv1, &max_pool1, &conv2, &max_pool2, &full_conn);

    quit();

    return 0;
}


void pixels_clear() {
    for (int y = 0 ; y < pixels_height; ++y) {
        for (int x = 0; x < pixels_width; ++x) {
            pixels[y][x] = 0;
        }
    }
}


void add(ImageSet* image_set, int index) {
    double data[image_set->channels * image_set->height * image_set->width + image_set->output_length];
    for (int c = 0; c < image_set->channels; ++c) {
        for (int h = 0; h < image_set->height; ++h) {
            for (int w = 0; w < image_set->width; ++w) {
                data[c * image_set->height * image_set->width + h * image_set->width + w] = pixels[h][w];
            }
        }
    }
    
    for (int i = 0; i < image_set->output_length; ++i) {
        data[image_set->channels * image_set->height * image_set->width + i] = 0;
    }
    data[image_set->channels * image_set->height * image_set->width + index] = 1;

    ImageSet_add(image_set, data); 
}


void allocate_all(
    ImageSet* image_set,
    Convolution* conv1,
    MaxPooling* max_pool1,
    Convolution* conv2,
    MaxPooling* max_pool2,
    FullConnected* full_conn
    ) {
    
    image_set->file_name = "DR_ImageSet.txt";
    ImageSet_load(image_set);
    if (image_set->__allocated__ != 1) {
        ImageSet_allocate(image_set, NULL,
            0, // length
            5, // batch_size
            3, // channels
            20, // height
            20, // width
            2 // output_length
            );
    }
    #ifdef Debug
    ImageSet_print_info(image_set);
    #endif // Debug


    conv1->parameters_file_name = "DR_Convolution1.txt";
    conv1->weights_file_name = "DR_Convolution1_Weigths.txt";
    Convolution_load(conv1);
    if (conv1->__allocated__ != 1) {
        Convolution_allocate(conv1, NULL, NULL,
            3, // channels
            20, // height
            20, // width
            5, // matrix channels
            3, // matrix height
            3, // matrix width
            1, // padding
            1, // stride height
            1, // stride width
            &ReLU // activation function
        );
    }
    if (conv1->__train_allocated__ != 1) {
        Convolution_train_allocate(conv1,
            0.09, // learning rate
            0.03, // momentum
            &MSE // loss function
        );
    }
    Convolution_load_weights(conv1);
    #ifdef Debug
    Convolution_print_info(conv1);
    #endif // Debug


    max_pool1->file_name = "DR_MaxPooling1.txt";
    MaxPooling_load(max_pool1);
    if (max_pool1->__allocated__ != 1) {
        MaxPooling_allocate(max_pool1, NULL,
            5, // channels
            20, // height
            20, // width
            2, // matrix height
            2 // matrix width
        );
    }
    if (max_pool1->__train_allocated__ != 1) {
        MaxPooling_train_allocate(max_pool1);
    }
    #ifdef Debug
    MaxPooling_print_info(max_pool1);
    #endif //Debug


    conv2->parameters_file_name = "DR_Convolution2.txt";
    conv2->weights_file_name = "DR_Convolution2_Weights.txt";
    Convolution_load(conv2);
    if (conv2->__allocated__ != 1) {
        Convolution_allocate(conv2, NULL, NULL,
            5, // channels
            10, // height
            10, // width
            7, // matrix channels
            3, // matrix height
            3, // matrix width
            1, // padding
            1, // stride height
            1, // stride width
            &ReLU // activation function
        );
    }
    if (conv2->__train_allocated__ != 1) {
        Convolution_train_allocate(conv2,
            0.03, // learning rate
            0.01, // momentum
            &MSE // loss function
        );
    }
    Convolution_load_weights(conv2);
    #ifdef Debug
    Convolution_print_info(conv2);
    #endif // Debug


    max_pool2->file_name = "DR_MaxPooling2.txt";
    MaxPooling_load(max_pool2);
    if (max_pool2->__allocated__ != 1) {
        MaxPooling_allocate(max_pool2, NULL,
            7, // channels
            10, // height
            10, // width
            2, // matrix height
            2 // matrix width
        );
    }
    if (max_pool2->__train_allocated__ != 1) {
        MaxPooling_train_allocate(max_pool2);
    }
    #ifdef Debug
    MaxPooling_print_info(max_pool2);
    #endif //Debug


    full_conn->parameters_file_name = "DR_FullConnected.txt";
    full_conn->weights_file_name = "DR_FullConnected_Weights.txt";
    FullConnected_load(full_conn);
    if (full_conn->__allocated__ != 1) {
        FullConnected_allocate(full_conn, NULL, NULL,
            4, // length
            (int[]){7 * 5 * 5 , 16, 4, 2}, // layers     
            (ActivationFunction*[]){&ReLU, &Sigmoid, &Sigmoid, NULL} // activation function
        );
    }
    if (full_conn->__train_allocated__ != 1) {
        FullConnected_train_allocate(full_conn,
            0.4, // learning rate
            0.1, // momentum
            &MSE // loss function
        );
    }
    FullConnected_load_weights(full_conn);
    #ifdef Debug
    FullConnected_print_info(full_conn);
    #endif // Debug
}


void deallocate_all(
    ImageSet* image_set,
    Convolution* conv1,
    MaxPooling* max_pool1,
    Convolution* conv2,
    MaxPooling* max_pool2,
    FullConnected* full_conn
    ) {

    FullConnected_train_deallocate(full_conn);
    FullConnected_deallocate(full_conn);

    MaxPooling_train_deallocate(max_pool2);
    MaxPooling_deallocate(max_pool2);

    Convolution_train_deallocate(conv2);
    Convolution_deallocate(conv2);

    MaxPooling_train_deallocate(max_pool1);
    MaxPooling_deallocate(max_pool1);

    Convolution_train_deallocate(conv1);
    Convolution_deallocate(conv1);

    ImageSet_deallocate(image_set);
}


void forward(
    ImageSet* image_set,
    Convolution* conv1,
    MaxPooling* max_pool1,
    Convolution* conv2,
    MaxPooling* max_pool2,
    FullConnected* full_conn
    ) {


}
