#include <stdio.h>

#include <tab.h>
#include <library.h>
#include <convolution.h>


void Convolution_print_output(Convolution* layer) {
	printf("Convolution output {\n");
	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		printf(TAB);
		printf("{\n");

		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			printf(TAB);
			printf(TAB);
			printf("{");

			for (int ow = 0; ow < layer->__output_width__; ++ow) {
				if (ow < layer->__output_width__ - 1) {
					printf("%lf, ", layer->output[mc][oh][ow]);
				} else {
					printf("%lf", layer->output[mc][oh][ow]);
				}
			}

			if (oh < layer->__output_height__ - 1) {
				printf("},\n");
			} else {
				printf("}\n");
			}
		}

		printf(TAB);
		if (mc < layer->matrix_c - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	printf("}\n");
}


void Convolution_print_info(Convolution* layer) {
	printf("Convolution info {\n");

	printf(TAB);
	printf("parameters_file_name = %s\n", layer->parameters_file_name);
	printf(TAB);
	printf("weights_file_name = %s\n", layer->weights_file_name);

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
		printf("output_size = {%d, %d, %d}\n", layer->matrix_c, layer->__output_height__, layer->__output_width__);
		printf(TAB);
		printf(TAB);
		printf("matrix_size = {%d, %d, %d, %d}\n", layer->matrix_c, layer->channels, layer->matrix_h, layer->matrix_w);
		printf(TAB);
		printf(TAB);
		printf("padding = %d\n", layer->padding);
		printf(TAB);
		printf(TAB);
		printf("stride = %d x %d\n", layer->stride_h, layer->stride_w);
		printf(TAB);
		printf(TAB);
		printf("actication = %s\n", layer->activation->name);

		printf(TAB);
		printf("}\n");
	}

	printf("\n");
	
	printf(TAB);
	printf("__train_allocated__ = %d\n", layer->__train_allocated__);
	
	if (layer->__train_allocated__ == 1) {
		printf(TAB);
		printf("{\n");

		printf(TAB);
		printf(TAB);
		printf("learning_rate = %lf\n", layer->learning_rate);
		printf(TAB);
		printf(TAB);
		printf("momentum = %lf\n", layer->momentum);
		printf(TAB);
		printf(TAB);
		printf("loss = %s\n", layer->loss->name);

		printf(TAB);
		printf("}\n");
	}

	printf("}\n");
}
