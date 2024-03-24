#include <stdio.h>
#include <stdlib.h>

#include "image_set.h"


ImageSet* ImageSet_construct(int length, int channels, int height, int width, int output_length) {
	ImageSet* image_set = (ImageSet*)malloc(sizeof(ImageSet));

	image_set->length = 0;
	image_set->allocated_length = length;
	
	image_set->channels = channels;
	image_set->height = height;
	image_set->width = width;
	image_set->output_length = output_length;

	image_set->input = (double****)malloc(sizeof(double***) * image_set->allocated_length);
	for (int i = 0; i < image_set->allocated_length; ++i) {
		image_set->input[i] = (double***)malloc(sizeof(double**) * image_set->channels);
		for (int c = 0; c < image_set->channels; ++c) {
			image_set->input[i][c] = (double**)malloc(sizeof(double*) * image_set->height);
			for (int h = 0; h < image_set->height; ++h) {
				image_set->input[i][c][h] = (double*)malloc(sizeof(double) * image_set->width);
			}
		}
	}

	image_set->output = (double**)malloc(sizeof(double*) * image_set->allocated_length);
	for (int i = 0; i < image_set->allocated_length; ++i) {
		image_set->output[i] = (double*)malloc(sizeof(double) * image_set->output_length);
	}
	
	return image_set;
}

void ImageSet_destruct(ImageSet* image_set) {
	for (int i = 0; i < image_set->allocated_length; ++i) {
		for (int c = 0; c < image_set->channels; ++c) {
			for (int h = 0; h < image_set->height; ++h) {
				free(image_set->input[i][c][h]);
			}
			free(image_set->input[i][c]);
		}
		free(image_set->input[i]);
	}
	free(image_set->input);

	for (int i = 0; i < image_set->allocated_length; ++i) {
		free(image_set->output[i]);
	}
	free(image_set->output);
	
	free(image_set);
}

void ImageSet_extend(ImageSet* image_set, int length) {
	image_set->input = (double****)realloc(image_set->input, sizeof(double***) * (image_set->allocated_length + length));
	image_set->output = (double**)realloc(image_set->output, sizeof(double*) * (image_set->allocated_length + length));
	for (int i = 0; i < length; ++i) {
		image_set->input[image_set->allocated_length + i] = (double***)malloc(sizeof(double**) * image_set->channels);
		image_set->output[image_set->allocated_length + i] = (double*)malloc(sizeof(double) * image_set->output_length);
		for (int c = 0; c < image_set->channels; ++c) {
			image_set->input[image_set->allocated_length + i][c] = (double**)malloc(sizeof(double*) * image_set->height);
			for (int h = 0; h < image_set->height; ++h) {
				image_set->input[image_set->allocated_length + i][c][h] = (double*)malloc(sizeof(double) * image_set->width);
			}
		}
	}

	image_set->allocated_length += length;
}

void ImageSet_add(ImageSet* image_set, double input[], double output[]) {
	if (image_set->length == image_set->allocated_length) {
		ImageSet_extend(image_set, 1);
	}

	for (int c = 0; c < image_set->channels; ++c) {
		for (int h = 0; h < image_set->height; ++h) {
			for (int w = 0; w < image_set->width; ++w) {
				image_set->input[image_set->length][c][h][w] = input[c * image_set->height * image_set->width + h * image_set->width + w];
			}
		}
	}

	for (int o = 0; o < image_set->output_length; ++o) {
		image_set->output[image_set->length][o] = output[o];
	}

	++image_set->length;
}

void ImageSet_load(ImageSet* image_set, const char* title) {
    FILE* file;
    if (file = fopen(title, "r")) {
	    if (file == NULL) {
	        printf("!!! ERROR in file handling for loading ImageSet !!!\n");
	    }

	    int length, channels, height, width, output_length;
	    fscanf(file, "%d %d %d %d %d ", &length, &channels, &height, &width, &output_length);

	    if (channels != image_set->channels) {
	    	printf("!!! ERROR in loading ImageSet: channels (%d) doesn't equal current ImageSet (%d) !!!\n", channels, image_set->channels);
	    }
	    if (height != image_set->height) {
	    	printf("!!! ERROR in loading ImageSet: height (%d) doesn't equal current ImageSet (%d) !!!\n", height, image_set->height);
	    }
	    if (width != image_set->width) {
	    	printf("!!! ERROR in loading ImageSet: width (%d) doesn't equal current ImageSet (%d) !!!\n", width, image_set->width);
	    }
	    if (output_length != image_set->output_length) {
	    	printf("!!! ERROR in loading ImageSet: output_length (%d) doesn't equal current ImageSet (%d) !!!\n", output_length, image_set->output_length);
	    }

	    if (image_set->allocated_length - image_set->length < length) {
	    	ImageSet_extend(image_set, length - (image_set->allocated_length - image_set->length));
	    }

	    for (int i = 0; i < length; ++i) {
	    	for (int c = 0; c < image_set->channels; ++c) {
	    		for (int h = 0; h < image_set->height; ++h) {
	    			for (int w = 0; w < image_set->width; ++w) {
	    				fscanf(file, "%lf ", &image_set->input[image_set->length + i][c][h][w]);
	    			}
	    		}
	    	}
	    	for (int o = 0; o < image_set->output_length; ++o) {
	    		fscanf(file, "%lf ", &image_set->output[image_set->length + i][o]);
	    	}
	    }

	    image_set->length += length;
	    
	    fclose(file);
	} else {
		printf("!!! WARNING in file loading: file with ImageSet data doen't exist !!!\n");
	}
}

void ImageSet_save(ImageSet* image_set, const char* title) {
    FILE* file = fopen(title, "w");
    if (file == NULL) {
        printf("!!! ERROR in file handling for saving ImageSet !!!\n");
    }

    fprintf(file, "%d %d %d %d %d ", image_set->length, image_set->channels, image_set->height, image_set->width, image_set->output_length);
    for (int i = 0; i < image_set->length; ++i) {
    	for (int c = 0; c < image_set->channels; ++c) {
    		for (int h = 0; h < image_set->height; ++h) {
    			for (int w = 0; w < image_set->width; ++w) {
    				fprintf(file, "%lf ", image_set->input[i][c][h][w]);
    			}
    		}
    	}
    	for (int o = 0; o < image_set->output_length; ++o) {
    		fprintf(file, "%lf ", image_set->output[i][o]);
    	}
    }

    fclose(file);
}
