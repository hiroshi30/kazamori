#include <stdio.h>

#include "tab.h"
#include "library.h"
#include "full_connected.h"


void FullConnected_print_weights(FullConnected* layer) {
	printf("FullConnected weights {\n");
	for (int i = 0; i < layer->length - 1; ++i) {
		printf(TAB);
		printf("{\n");
		for (int j = 0; j < layer->layers[i]; ++j) {
			printf(TAB);
			printf(TAB);
			printf("{");
			for (int l = 0; l < layer->layers[i + 1]; ++l) {
				if (l < layer->layers[i + 1] - 1) {
					printf("%lf, ", layer->weights[i][j][l]);
				} else {
					printf("%lf", layer->weights[i][j][l]);
				}
			}

			if (j < layer->layers[i] - 1) {
				printf("},\n");
			} else {
				printf("}\n");
			}
		}

		printf(TAB);
		if (i < layer->length - 2) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}

	printf("}\n");
}

void FullConnected_print_biases(FullConnected* layer) {
	printf("FullConnected biases {\n");
	for (int i = 0; i < layer->length - 1; ++i) {
		printf(TAB);
		printf("{");
		for (int j = 0; j < layer->layers[i + 1]; ++j) {
			if (j < layer->layers[i + 1] - 1) {
				printf("%lf, ", layer->biases[i][j]);
			} else {
				printf("%lf", layer->biases[i][j]);
			}
		}

		if (i < layer->length - 2) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}

	printf("}\n");
}

void FullConnected_print_x(FullConnected* layer) {
	printf("FullConnected x {\n");
	for (int i = 0; i < layer->length; ++i) {
		printf(TAB);
		printf("{");
		for (int j = 0; j < layer->layers[i]; ++j) {
			if (j < layer->layers[i] - 1) {
				printf("%lf, ", layer->x[i][j]);
			} else {
				printf("%lf", layer->x[i][j]);
			}
		}

		if (i < layer->length - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	
	printf("}\n");
}


void FullConnected_print_delta_weights(FullConnected* layer) {
	printf("FullConnected delta_weights {\n");
	for (int i = 0; i < layer->length - 1; ++i) {
		printf(TAB);
		printf("{\n");
		for (int j = 0; j < layer->layers[i]; ++j) {
			printf(TAB);
			printf(TAB);
			printf("{");
			for (int l = 0; l < layer->layers[i + 1]; ++l) {
				if (l < layer->layers[i + 1] - 1) {
					printf("%lf, ", layer->delta_weights[i][j][l]);
				} else {
					printf("%lf", layer->delta_weights[i][j][l]);
				}
			}

			if (j < layer->layers[i] - 1) {
				printf("},\n");
			} else {
				printf("}\n");
			}
		}

		printf(TAB);
		if (i < layer->length - 2) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}

	printf("}\n");
}

void FullConnected_print_delta_biases(FullConnected* layer) {
	printf("FullConnected delta_biases {\n");
	for (int i = 0; i < layer->length - 1; ++i) {
		printf(TAB);
		printf("{");
		for (int j = 0; j < layer->layers[i + 1]; ++j) {
			if (j < layer->layers[i + 1] - 1) {
				printf("%lf, ", layer->delta_biases[i][j]);
			} else {
				printf("%lf", layer->delta_biases[i][j]);
			}
		}

		if (i < layer->length - 2) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}

	printf("}\n");
}

void FullConnected_print_err(FullConnected* layer) {
	printf("FullConnected err {\n");
	for (int i = 0; i < layer->length; ++i) {
		printf(TAB);
		printf("{");
		for (int j = 0; j < layer->layers[i]; ++j) {
			if (j < layer->layers[i] - 1) {
				printf("%lf, ", layer->err[i][j]);
			} else {
				printf("%lf", layer->err[i][j]);
			}
		}

		if (i < layer->length - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	
	printf("}\n");
}

void FullConnected_print(FullConnected* layer) {
    FullConnected_print_x(layer);
    FullConnected_print_delta_weights(layer);
    FullConnected_print_weights(layer);
    FullConnected_print_delta_biases(layer);
    FullConnected_print_biases(layer);
    FullConnected_print_err(layer);
}
