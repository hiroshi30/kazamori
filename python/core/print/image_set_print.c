#include <stdio.h>

#include <tab.h>
#include <image_set.h>


void ImageSet_print_input(ImageSet* image_set) {
	printf("ImageSet (%d/%d) {\n", image_set->length, image_set->allocated_length);
	for (int i = 0; i < image_set->length; ++i) {
		printf(TAB);
		printf("index %d: {\n", i);
		for (int c = 0; c < image_set->channels; ++c) {
			printf(TAB);
			printf(TAB);
			printf("channel %d: {\n", c);
			for (int h = 0; h < image_set->height; ++h) {
				printf(TAB);
				printf(TAB);
				printf(TAB);
				printf("{");
				for (int w = 0; w < image_set->width; ++w) {
					if (w < image_set->width - 1) {
						printf("%lf, ", image_set->input[i][c][h][w]);
					} else {
						printf("%lf", image_set->input[i][c][h][w]);
					}
				}
				if (h < image_set->height - 1) {
					printf("},\n");
				} else {
					printf("}\n");
				}
			}	
			printf(TAB);
			printf(TAB);
			if (c < image_set->channels - 1) {
				printf("},\n");
			} else {
				printf("}\n");
			}
		}
		printf(TAB);
		if (i < image_set->length - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}

	printf("}\n");
}


void ImageSet_print_output(ImageSet* image_set) {
	printf("ImageSet (%d/%d) {\n", image_set->length, image_set->allocated_length);
	for (int i = 0; i < image_set->length; ++i) {
		printf(TAB);
		printf("index %d: {", i);
		for (int o = 0; o < image_set->output_length; ++o) {
			if (o < image_set->output_length - 1) {
				printf("%lf, ", image_set->output[i][o]);
			} else {
				printf("%lf", image_set->output[i][o]);
			}
		}
		if (i < image_set->length - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}

	printf("}\n");
}


void ImageSet_print_info(ImageSet* image_set) {
	printf("ImageSet info {\n");
	
	printf(TAB);
	printf("__allocated__ = %d\n", image_set->__allocated__);
	
	if (image_set->__allocated__ == 1) {
		printf(TAB);
		printf("{\n");

		printf(TAB);
		printf(TAB);
		printf("length = %d\n", image_set->length);
		printf(TAB);
		printf(TAB);
		printf("allocated_length = %d\n", image_set->allocated_length);
		printf(TAB);
		printf(TAB);
		printf("batch_size = %d\n", image_set->batch_size);

		printf(TAB);
		printf(TAB);
		printf("input_size = {%d, %d, %d}\n", image_set->channels, image_set->height, image_set->width);
		printf(TAB);
		printf(TAB);
		printf("output_length = %d\n", image_set->output_length);

		printf(TAB);
		printf("}\n");
	}

	printf("}\n");
}
