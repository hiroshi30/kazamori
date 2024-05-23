#include <stdio.h>
#include <stdlib.h>

#include <max_pooling.h>


void MaxPooling_allocate(
        MaxPooling* layer,
        const char* file_name,
        int channels,
        int height,
        int width,
        int matrix_h,
        int matrix_w
    ) {
    
    if (file_name) {
        layer->file_name = file_name;
    }

    layer->channels = channels;
    layer->height = height;
    layer->width = width;
    layer->matrix_h = matrix_h;
    layer->matrix_w = matrix_w;

    layer->__output_height__ = layer->height / layer->matrix_h;
    layer->__output_width__ = layer->width / layer->matrix_w;

    layer->input = (double***)malloc(sizeof(double**) * layer->channels);
    for (int c = 0; c < layer->channels; ++c) {
        layer->input[c] = (double**)malloc(sizeof(double*) * layer->height);
        for (int h = 0; h < layer->height; ++h) {
            layer->input[c][h] = (double*)calloc(layer->width, sizeof(double));
        }
    }

    layer->output = (double***)malloc(sizeof(double**) * layer->channels);
    for (int c = 0; c < layer->channels; ++c) {
        layer->output[c] = (double**)malloc(sizeof(double*) * layer->__output_height__);
        for (int hh = 0; hh < layer->__output_height__; ++hh) {
            layer->output[c][hh] = (double*)calloc(layer->__output_width__, sizeof(double));
        }
    }

    layer->__allocated__ = 1;
}


void MaxPooling_deallocate(MaxPooling* layer) {
    for (int c = 0; c < layer->channels; ++c) {
        for (int hh = 0; hh < layer->__output_height__; ++hh) {
            free(layer->output[c][hh]);
        }
        free(layer->output[c]);
    }
    free(layer->output);

    for (int c = 0; c < layer->channels; ++c) {
        for (int h = 0; h < layer->height; ++h) {
            free(layer->input[c][h]);
        }
        free(layer->input[c]);
    }
    free(layer->input);

    layer->__allocated__ = 0;
}


void MaxPooling_train_allocate(MaxPooling* layer) {
    layer->err = (double***)malloc(sizeof(double**) * layer->channels);
    for (int c = 0; c < layer->channels; ++c) {
        layer->err[c] = (double**)malloc(sizeof(double*) * layer->height);
        for (int h = 0; h < layer->height; ++h) {
            layer->err[c][h] = (double*)calloc(layer->width, sizeof(double));
        }
    }

    layer->__train_allocated__ = 1;
}


void MaxPooling_train_deallocate(MaxPooling* layer) {
    for (int c = 0; c < layer->channels; ++c) {
        for (int h = 0; h < layer->height; ++h) {
            free(layer->err[c][h]);
        }
        free(layer->err[c]);
    }
    free(layer->err);

    layer->__train_allocated__ = 0;
}


void MaxPooling_forward(MaxPooling* layer, double*** input) {
    for (int c = 0; c < layer->channels; ++c) {
        for (int hh = 0; hh < layer->__output_height__; ++hh) {
            for (int ww = 0; ww < layer->__output_width__; ++ww) {
                layer->output[c][hh][ww] = input[c][hh * layer->matrix_h][ww * layer->matrix_w];
                for (int mh = 0; mh < layer->matrix_h; ++mh) {
                    for (int mw = 0; mw < layer->matrix_w; ++mw) {
                        layer->input[c][hh * layer->matrix_h + mh][ww * layer->matrix_w + mw] = input[c][hh * layer->matrix_h + mh][ww * layer->matrix_w + mw];
                        if (layer->input[c][hh * layer->matrix_h + mh][ww * layer->matrix_w + mw] > layer->output[c][hh][ww]) {
                           layer->output[c][hh][ww] = layer->input[c][hh * layer->matrix_h + mh][ww * layer->matrix_w + mw];
                        }
                    }
                }
            }
        }
    }
}


void MaxPooling_backward(MaxPooling* layer, double*** output) {
    for (int c = 0; c < layer->channels; ++c) {
        for (int hh = 0; hh < layer->__output_height__; ++hh) {
            for (int ww = 0; ww < layer->__output_width__; ++ww) {
                for (int mh = 0; mh < layer->matrix_h; ++mh) {
                    for (int mw = 0; mw < layer->matrix_w; ++mw) {
                        layer->err[c][hh * layer->matrix_h + mh][ww * layer->matrix_w + mw] = 0;
                        if (layer->output[c][hh][ww] == layer->input[c][hh * layer->matrix_h + mh][ww * layer->matrix_w + mw]) {
                           layer->err[c][hh * layer->matrix_h + mh][ww * layer->matrix_w + mw] = output[c][hh][ww];
                        }
                    }
                }
            }
        }
    }
}


void MaxPooling_load(MaxPooling* layer) {
    FILE* file = fopen(layer->file_name, "r");
    if (file) {
        int channels, height, width, matrix_h, matrix_w;
        fscanf(file, "%d %d %d %d %d ", &channels, &height, &width, &matrix_h, &matrix_w);

        if (layer->__allocated__ == 1) {
            MaxPooling_deallocate(layer);            
        }
        MaxPooling_allocate(layer, NULL, channels, height, width, matrix_h, matrix_w);

        int train_construct;
        fscanf(file, "%d ", &train_construct);
        if (train_construct == 1) {
            if (layer->__train_allocated__ == 1) {
                MaxPooling_train_deallocate(layer);          
            }
            MaxPooling_train_allocate(layer);
        }
    }
    #ifdef Debug
    else {
        printf("!!! WARNING in MaxPooling_load(): file (%s) doesn't exist !!!\n", layer->file_name);
    }
    #endif // Debug
    fclose(file);
}


void MaxPooling_save(MaxPooling* layer) {
    FILE* file = fopen(layer->file_name, "w");
    if (file) {
        fprintf(file, "%d %d %d %d %d %d ", layer->channels, layer->height, layer->width, layer->matrix_h, layer->matrix_w, layer->__train_allocated__);
    }
    #ifdef Debug
    else {
        printf("!!! WARNING in MaxPooling_save(): file name is %s !!!\n", layer->file_name);
    }
    #endif // Debug
    fclose(file);
}
