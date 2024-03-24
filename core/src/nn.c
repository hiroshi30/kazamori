#include <stdio.h>
#include <math.h>

#include "library.h"
#include "data_set.h"
#include "full_connected.h"


double FullConnected_calculate_error(FullConnected* layer, DataSet* data_set) {
	double error = 0;
	for (int i = 0; i < data_set->length; ++i) {
		FullConnected_forward(layer, data_set->input[i]);
		for (int j = 0; j < data_set->output_length; ++j) {
			error += fabs(data_set->output[i][j] - layer->x[layer->length - 1][j]);
		}
		error /= data_set->output_length;
	}
	error /= data_set->length;

	return error;
}

void FullConnected_train(FullConnected* layer, DataSet* data_set, int epochs) {
	int index;
	for (int i = 0; i < epochs; ++i) {
		index = random_int(0, data_set->length - 1);
		FullConnected_forward(layer, data_set->input[index]);
		FullConnected_backward(layer, data_set->output[index]);
	}
}

int FullConnected_train_alpha(FullConnected* layer, DataSet* data_set, double alpha) {
	int step = 500;
	int epochs = 0;
	double error = FullConnected_calculate_error(layer, data_set);
	while (error > alpha) {
		FullConnected_train(layer, data_set, step);
		epochs += step;
		error = FullConnected_calculate_error(layer, data_set);
		printf("error %lf\n", error);
	}

	return epochs;
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
