#include <stdio.h>
#include <stdlib.h>

#include <image_set.h>


void ImageSet_allocate(
		ImageSet* image_set,
		const char* file_name,
		int length,
		int batch_size,
		int channels,
		int height,
		int width,
		int output_length
	) {
	if (file_name) {
		image_set->file_name = file_name;
	}

	image_set->length = 0;
	image_set->allocated_length = length;
	image_set->batch_size = batch_size;
	
	image_set->channels = channels;
	image_set->height = height;
	image_set->width = width;
	image_set->output_length = output_length;

	image_set->input = (double****)malloc(sizeof(double***) * image_set->allocated_length);
	image_set->output = (double**)malloc(sizeof(double*) * image_set->allocated_length);
	for (int i = 0; i < image_set->allocated_length; ++i) {
		image_set->input[i] = (double***)malloc(sizeof(double**) * image_set->channels);
		image_set->output[i] = (double*)malloc(sizeof(double) * image_set->output_length);
		for (int c = 0; c < image_set->channels; ++c) {
			image_set->input[i][c] = (double**)malloc(sizeof(double*) * image_set->height);
			for (int h = 0; h < image_set->height; ++h) {
				image_set->input[i][c][h] = (double*)malloc(sizeof(double) * image_set->width);
			}
		}
	}

	image_set->__allocated__ = 1;
}


void ImageSet_deallocate(ImageSet* image_set) {
	for (int i = 0; i < image_set->allocated_length; ++i) {
		for (int c = 0; c < image_set->channels; ++c) {
			for (int h = 0; h < image_set->height; ++h) {
				free(image_set->input[i][c][h]);
			}
			free(image_set->input[i][c]);
		}
		free(image_set->input[i]);
		free(image_set->output[i]);
	}
	free(image_set->input);
	free(image_set->output);

	image_set->__allocated__ = 0;
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


void ImageSet_add(ImageSet* image_set, double* data) {
	if (image_set->length == image_set->allocated_length) {
		ImageSet_extend(image_set, image_set->batch_size);
	}

	int stride = 0;
	for (int c = 0; c < image_set->channels; ++c) {
		for (int h = 0; h < image_set->height; ++h) {
			for (int w = 0; w < image_set->width; ++w) {
				image_set->input[image_set->length][c][h][w] = data[stride + w];
				++stride;
			}
		}
	}

	for (int o = 0; o < image_set->output_length; ++o) {
		image_set->output[image_set->length][o] = data[stride + o];
		++stride;
	}

	++image_set->length;
}


void ImageSet_load(ImageSet* image_set) {
    FILE* file = fopen(image_set->file_name, "r");
    if (file) {
	    int length, batch_size, channels, height, width, output_length;
	    fscanf(file, "%d %d %d %d %d %d ", &length, &batch_size, &channels, &height, &width, &output_length);

    	if (image_set->__allocated__ == 1) {
    		ImageSet_deallocate(image_set);
    	}
    	ImageSet_allocate(image_set, NULL, length, batch_size, channels, height, width, output_length);

	    for (int i = 0; i < length; ++i) {
	    	for (int c = 0; c < image_set->channels; ++c) {
	    		for (int h = 0; h < image_set->height; ++h) {
	    			for (int w = 0; w < image_set->width; ++w) {
	    				fscanf(file, "%lf ", &image_set->input[i][c][h][w]);
	    			}
	    		}
	    	}
	    	for (int o = 0; o < image_set->output_length; ++o) {
	    		fscanf(file, "%lf ", &image_set->output[i][o]);
	    	}
	    }

	    image_set->length = length;
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in ImageSet_load(): file (%s) doesn't exist !!!\n", image_set->file_name);
	}
	#endif // Debug
    fclose(file);
}


void ImageSet_save(ImageSet* image_set) {
    FILE* file = fopen(image_set->file_name, "w");
    if (file) {
	    fprintf(file, "%d %d %d %d %d %d ", image_set->length, image_set->batch_size, image_set->channels, image_set->height, image_set->width, image_set->output_length);
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
	}
	#ifdef Debug
	else {
		printf("!!! ERROR in ImageSet_save(): file name is %s !!!\n", image_set->file_name);
	}
	#endif // Debug
    fclose(file);
}
