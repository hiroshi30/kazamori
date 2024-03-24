#include <stdio.h>

#include <tab.h>
#include <library.h>
#include <convolution.h>


void Convolution_print_output(Convolution* layer) {
	printf("Convolution output {\n");
	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		printf(TAB);
		printf("{\n");

		for (int oh = 0; oh < layer->__output_height; ++oh) {
			printf(TAB);
			printf(TAB);
			printf("{");

			for (int ow = 0; ow < layer->__output_width; ++ow) {
				if (ow < layer->__output_width - 1) {
					printf("%lf, ", layer->output[mc][oh][ow]);
				} else {
					printf("%lf", layer->output[mc][oh][ow]);
				}
			}

			if (oh < layer->__output_height - 1) {
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
