#include <stdio.h>

#include <tab.h>
#include <max_pooling.h>


void MaxPooling_print_input(MaxPooling* layer) {
	printf("MaxPooling input {\n");
	for (int c = 0; c < layer->channels; ++c) {
		printf(TAB);
		printf("{\n");

		for (int h = 0; h < layer->height; ++h) {
			printf(TAB);
			printf(TAB);
			printf("{");

			for (int w = 0; w < layer->width; ++w) {
				if (w < layer->width - 1) {
					printf("%lf, ", layer->input[c][h][w]);
				} else {
					printf("%lf", layer->input[c][h][w]);
				}
			}

			if (h < layer->height - 1) {
				printf("},\n");
			} else {
				printf("}\n");
			}
		}

		printf(TAB);
		if (c < layer->channels - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	printf("}\n");
}

void MaxPooling_print_output(MaxPooling* layer) {
	printf("MaxPooling output {\n");
	for (int c = 0; c < layer->channels; ++c) {
		printf(TAB);
		printf("{\n");

		for (int hh = 0; hh < layer->height / layer->matrix_h; ++hh) {
			printf(TAB);
			printf(TAB);
			printf("{");

			for (int ww = 0; ww < layer->width / layer->matrix_w; ++ww) {
				if (ww < layer->width / layer->matrix_w - 1) {
					printf("%lf, ", layer->output[c][hh][ww]);
				} else {
					printf("%lf", layer->output[c][hh][ww]);
				}
			}

			if (hh < layer->height / layer->matrix_h - 1) {
				printf("},\n");
			} else {
				printf("}\n");
			}
		}

		printf(TAB);
		if (c < layer->channels - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	printf("}\n");
}

void MaxPooling_print_err(MaxPooling* layer) {
	printf("MaxPooling err {\n");
	for (int c = 0; c < layer->channels; ++c) {
		printf(TAB);
		printf("{\n");

		for (int h = 0; h < layer->height; ++h) {
			printf(TAB);
			printf(TAB);
			printf("{");

			for (int w = 0; w < layer->width; ++w) {
				if (w < layer->width - 1) {
					printf("%lf, ", layer->err[c][h][w]);
				} else {
					printf("%lf", layer->err[c][h][w]);
				}
			}

			if (h < layer->height - 1) {
				printf("},\n");
			} else {
				printf("}\n");
			}
		}

		printf(TAB);
		if (c < layer->channels - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	printf("}\n");
}


void MaxPooling_print_info(MaxPooling* layer) {
	printf("MaxPooling info {\n");

	printf(TAB);
	printf("file_name = %s\n", layer->file_name);

	printf("\n");

	printf(TAB);
	printf("__allocated__ = %d\n", layer->__allocated__);

	if (layer->__allocated__ == 1) {
		printf(TAB);
		printf("{\n");

		printf(TAB);
		printf(TAB);
		printf("input_size = {%d, %d, %d}\n", layer->channels, layer->height, layer->width);
		printf(TAB);
		printf(TAB);
		printf("output_size = {%d, %d, %d}\n", layer->channels, layer->__output_height__, layer->__output_width__);
		printf(TAB);
		printf(TAB);
		printf("matrix_size = {%d, %d}\n", layer->matrix_h, layer->matrix_w);

		printf(TAB);
		printf("}\n");
	}

	printf("\n");
	
	printf(TAB);
	printf("__train_allocated__ = %d\n", layer->__train_allocated__);

	printf("}\n");
}
