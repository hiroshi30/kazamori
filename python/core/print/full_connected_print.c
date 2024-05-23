#include <stdio.h>

#include <tab.h>
#include <library.h>
#include <data_set.h>
#include <full_connected.h>


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


void FullConnected_print_info(FullConnected* layer) {
	printf("FullConnected info {\n");

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
		printf("length = %d\n", layer->length);
		printf(TAB);
		printf(TAB);
		printf("layers = {");
		for (int i = 0; i < layer->length; ++i) {
			if (i < layer->length - 1) {
				printf("%d, ", layer->layers[i]);
			} else {
				printf("%d", layer->layers[i]);
			}
		}
		printf("}\n");
		printf(TAB);
		printf(TAB);
		printf("activation = {");
		for (int i = 0; i < layer->length; ++i) {
			if (i < layer->length - 1) {
				if (layer->activations[i] == NULL) {
					printf("NULL, ");
				} else {
					printf("%s, ", layer->activations[i]->name);
				}
			} else {
				if (layer->activations[i] == NULL) {
					printf("NULL");
				} else {
					printf("%s", layer->activations[i]->name);
				}
			}
		}
		printf("}\n");

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


void FullConnected_check(FullConnected* layer, DataSet* data_set) {
	for (int i = 0; i < data_set->length; ++i) {
		FullConnected_forward(layer, data_set->input[i]);
		printf("%d {", i);
		for (int j = 0; j < data_set->output_length; ++j) {
			if (j < data_set->output_length - 1) {
				printf("%lf, ", layer->x[layer->length - 1][j]);
			} else {
				printf("%lf", layer->x[layer->length - 1][j]);
			}
		}
		printf("} -> ");

		printf("{");
		for (int j = 0; j < data_set->output_length; ++j) {
			if (j < data_set->output_length - 1) {
				printf("%lf, ", data_set->output[i][j]);
			} else {
				printf("%lf", data_set->output[i][j]);
			}
		}
		printf("}\n");
	}
}
