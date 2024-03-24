#include <stdlib.h>

#include "max_pooling.h"


MaxPooling* MaxPooling_construct(int channels, int height, int width, int matrix_h, int matrix_w) {
    MaxPooling* layer = (MaxPooling*)malloc(sizeof(MaxPooling));

    layer->channels = channels;
    layer->height = height;
    layer->width = width;
    layer->matrix_h = matrix_h;
    layer->matrix_w = matrix_w;

    layer->input = (double***)malloc(sizeof(double**) * layer->channels);
    for (int c = 0; c < layer->channels; ++c) {
        layer->input[c] = (double**)malloc(sizeof(double*) * layer->height);
        for (int h = 0; h < layer->height; ++h) {
            layer->input[c][h] = (double*)malloc(sizeof(double) * layer->width);
        }
    }

    layer->output = (double***)malloc(sizeof(double**) * layer->channels);
    for (int c = 0; c < layer->channels; ++c) {
        layer->output[c] = (double**)malloc(sizeof(double*) * (layer->height / layer->matrix_h));
        for (int h = 0; h < layer->height / layer->matrix_h; ++h) {
            layer->output[c][h] = (double*)malloc(sizeof(double) * (layer->width / layer->matrix_w));
        }
    }
    return layer;
}

void MaxPooling_destruct(MaxPooling* layer) {
    for (int c = 0; c < layer->channels; ++c) {
        for (int h = 0; h < layer->height; ++h) {
            free(layer->input[c][h]);
        }
        free(layer->input[c]);
    }
    free(layer->input);

    for (int c = 0; c < layer->channels; ++c) {
        for (int h = 0; h < layer->height / layer->matrix_h; ++h) {
            free(layer->output[c][h]);
        }
        free(layer->output[c]);
    }
    free(layer->output);

    free(layer);
}

void MaxPooling_train_construct(MaxPooling* layer) {
    layer->err = (double***)malloc(sizeof(double**) * layer->channels);
    for (int c = 0; c < layer->channels; ++c) {
        layer->err[c] = (double**)malloc(sizeof(double*) * layer->height);
        for (int h = 0; h < layer->height; ++h) {
            layer->err[c][h] = (double*)malloc(sizeof(double) * layer->width);
        }
    }
}

void MaxPooling_train_destruct(MaxPooling* layer) {
    for (int c = 0; c < layer->channels; ++c) {
        for (int h = 0; h < layer->height; ++h) {
            free(layer->err[c][h]);
        }
        free(layer->err[c]);
    }
    free(layer->err);
}

void MaxPooling_forward(MaxPooling* layer, double*** input) {
    for (int c = 0; c < layer->channels; ++c) {
        for (int hh = 0; hh < layer->height / layer->matrix_h; ++hh) {
            for (int ww = 0; ww < layer->width / layer->matrix_w; ++ww) {
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
        for (int hh = 0; hh < layer->height / layer->matrix_h; ++hh) {
            for (int ww = 0; ww < layer->width / layer->matrix_w; ++ww) {
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
