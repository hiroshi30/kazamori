#include <math.h>

#include <GUI.h>

#include <core.h>
#ifdef Debug
#include <print.h>
#endif // Debug


#define window_title "MulticlassClassification"

#define canvas_width (16 * 60)
#define canvas_height (9 * 60)

#define button_width 230
#define button_height 45

#define padding_x 35
#define padding_y 10

#define buttons_count 9

#define window_width canvas_width + 2 * padding_x + button_width
#define window_height canvas_height


int pixels[canvas_height][canvas_width];


void pixels_clear();


int main(int argc, char* argv[]) {
    if (init(window_title, window_width, window_height) != 0) {
        return 1;
    }

    pixels_clear();

    Canvas canvas;
    Canvas_create(&canvas,
        0, 0,
        canvas_width, canvas_height,
        &(SDL_Color){255, 255, 255, 255}
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
    buttons[0].text = "Next type";
    buttons[1].text = "Previous type";
    buttons[2].text = "Clear";
    buttons[3].text = "FNN";
    buttons[4].text = "kNN Classification";
    buttons[5].text = "kNN Regression";
    buttons[6].text = "Weighted kNN";
    buttons[7].text = "Save DataSet";
    buttons[8].text = "Save FNN";


    int types_count = 4;
    SDL_Color border_color = {50, 50, 50, 255};
    SDL_Color colors[] = {
        {182, 158, 203, 255},
        {171, 215, 168, 255},
        {241, 237, 130, 255},
        {154, 218, 244, 255}
    };
    SDL_Color background_colors[1 + types_count];
    background_colors[0] = (SDL_Color){255, 255, 255, 255};
    for (int i = 0; i < types_count; ++i) {
        background_colors[1 + i].r = fmin(1.3 * colors[i].r, 255);
        background_colors[1 + i].g = fmin(1.3 * colors[i].g, 255);
        background_colors[1 + i].b = fmin(1.3 * colors[i].b, 255);
        background_colors[1 + i].a = 255;
    }


    // DataSet
    DataSet data_set;
    data_set.file_name = "SVM_DataSet.txt";
    DataSet_load(&data_set);
    if (data_set.__allocated__ != 1) {
        DataSet_allocate(&data_set, NULL, 0, 5, 2, types_count);
    }
    #ifdef Debug
    DataSet_print(&data_set);
    #endif // Debug


    // FullConnected
    FullConnected layer;
    layer.parameters_file_name = "SVM_FullConnected.txt";
    layer.weights_file_name = "SVM_Weights.txt";
    FullConnected_load(&layer);
    if (layer.__allocated__ != 1) {
        FullConnected_allocate(&layer, NULL, NULL, 4, (int[]){2, 5, 2, types_count},
            (ActivationFunction*[]){&Sigmoid, &Sigmoid, &Sigmoid, NULL}
        );
    }
    if (layer.__train_allocated__ != 1) {
        FullConnected_train_allocate(&layer, 0.15, 0.03, &MSE);
    }
    FullConnected_load_weights(&layer);
    #ifdef Debug
    FullConnected_print_info(&layer);
    #endif // Debug


    // kNN
    const int k = 5;


    int type = 0;

    int mouse_x, mouse_y;

    bool need_update = true;
    bool run = true;
    SDL_Event event;
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
            }

            if (event.button.button == SDL_BUTTON_LEFT) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    SDL_GetMouseState(&mouse_x, &mouse_y);

                    if (Canvas_collide(&canvas, mouse_x, mouse_y)) {
                        need_update = true;
                        double point[2 + types_count];
                        point[0] = (double)mouse_y / canvas.height;
                        point[1] = (double)mouse_x / canvas.width;
                        for (int i = 0; i < types_count; ++i) {
                            point[2 + i] = 0;
                            if (type == i) {
                                point[2 + i] = 1;
                            }
                        }
                        DataSet_add(&data_set, point);
                        #ifdef Debug
                        DataSet_print(&data_set);
                        #endif // Debug
                    }

                    else if (Button_collide(&buttons[0], mouse_x, mouse_y)) {
                        need_update = true;
                        type += 1;
                        if (type >= types_count) {
                            type = 0;
                        }
                    }

                    else if (Button_collide(&buttons[1], mouse_x, mouse_y)) {
                        need_update = true;
                        type -= 1;
                        if (type < 0) {
                            type = types_count - 1;
                        }
                    }

                    else if (Button_collide(&buttons[2], mouse_x, mouse_y)) {
                        need_update = true;
                        pixels_clear();
                        Canvas_clear(&canvas);
                        DataSet_clear(&data_set);
                    }

                    else if (Button_collide(&buttons[3], mouse_x, mouse_y)) {
                        need_update = true;
                        pixels_clear();
                        Canvas_clear(&canvas);
                        FullConnected_train_threshold(&layer, &data_set, 0.0001);
                        for (int y = 0; y < canvas.height; ++y) {
                            for (int x = 0; x < canvas.width; ++x) {
                                FullConnected_forward(&layer, (double[]){(double)y / canvas.height, (double)x / canvas.width});
                                for (int i = 0; i < types_count; ++i) {
                                    if (layer.x[layer.length - 1][i] * types_count >= 1) {
                                        pixels[y][x] = i + 1;
                                    }
                                }
                            }
                        }
                        #ifdef Debug
                        FullConnected_check(&layer, &data_set);
                        FullConnected_print(&layer);
                        #endif // Debug
                    }

                    else if (Button_collide(&buttons[4], mouse_x, mouse_y)) {
                        need_update = true;
                        pixels_clear();
                        Canvas_clear(&canvas);
                        double output[types_count];
                        for (int y = 0; y < canvas.height; ++y) {
                            for (int x = 0; x < canvas.width; ++x) {
                                kNN_classification(&data_set, k, (double[]){(double)y / canvas.height, (double)x / canvas.width}, output);
                                for (int i = 0; i < types_count; ++i) {
                                    if (output[i] * types_count >= 1) {
                                        pixels[y][x] = i + 1;
                                    }
                                }
                            }
                        }
                    }

                    else if (Button_collide(&buttons[5], mouse_x, mouse_y)) {
                        need_update = true;
                        pixels_clear();
                        Canvas_clear(&canvas);
                        double output[types_count];
                        for (int y = 0; y < canvas.height; ++y) {
                            for (int x = 0; x < canvas.width; ++x) {
                                kNN_regression(&data_set, k, (double[]){(double)y / canvas.height, (double)x / canvas.width}, output);
                                for (int i = 0; i < types_count; ++i) {
                                    if (output[i] * types_count >= 1) {
                                        pixels[y][x] = i + 1;
                                    }
                                }
                            }
                        }
                    }

                    else if (Button_collide(&buttons[6], mouse_x, mouse_y)) {
                        need_update = true;
                        pixels_clear();
                        Canvas_clear(&canvas);
                        double output[types_count];
                        for (int y = 0; y < canvas.height; ++y) {
                            for (int x = 0; x < canvas.width; ++x) {
                                weighted_kNN(&data_set, k, (double[]){(double)y / canvas.height, (double)x / canvas.width}, output);
                                for (int i = 0; i < types_count; ++i) {
                                    if (output[i] * types_count >= 1) {
                                        pixels[y][x] = i + 1;
                                    }
                                }
                            }
                        }
                    }

                    else if (Button_collide(&buttons[7], mouse_x, mouse_y)) {
                        DataSet_save(&data_set);
                    }

                    else if (Button_collide(&buttons[8], mouse_x, mouse_y)) {
                        FullConnected_save(&layer);
                        FullConnected_save_weights(&layer);
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONUP) {
                    need_update = true;
                    canvas.mouse_on = false;
                    for (int i = 0; i < buttons_count; ++i) {
                        buttons[i].mouse_on = false;
                    }
                }
            }
        }

        if (need_update) {
            need_update = false;

            for (int y = 0; y < canvas.height; ++y) {
                for (int x = 0; x < canvas.width; ++x) {
                    Canvas_draw_point(&canvas, x, y, &background_colors[pixels[y][x]]);
                }
            }

            for (int i = 0; i < data_set.length; ++i) {
                Canvas_draw_circle(&canvas,
                    (int)(canvas.width * data_set.input[i][1]),
                    (int)(canvas.height * data_set.input[i][0]),
                    15,
                    &border_color
                );
                for (int j = 0; j < types_count; ++j) {
                    if (data_set.output[i][j] == 1) {
                        Canvas_draw_circle(&canvas,
                            (int)(canvas.width * data_set.input[i][1]),
                            (int)(canvas.height * data_set.input[i][0]),
                            15 - 2,
                            &colors[j]
                        );
                        break;
                    }
                }
            }
            
            for (int i = 0; i < buttons_count; ++i) {
                Button_draw(&buttons[i]);
            }

            Canvas_draw_vignette(&canvas, 10, &colors[type]);

            update();
        }
    }
    
    FullConnected_train_deallocate(&layer);
    FullConnected_deallocate(&layer);

    DataSet_deallocate(&data_set);

    quit();

    return 0;
}


void pixels_clear() {
    for (int y = 0 ; y < canvas_height; ++y) {
        for (int x = 0; x < canvas_width; ++x) {
            pixels[y][x] = 0;
        }
    }
}
