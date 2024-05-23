#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <library.h>
#include <data_set.h>
#include <full_connected.h>


void FullConnected_allocate(
		FullConnected* layer, 
		const char* parameters_file_name,
		const char* weights_file_name,
		int length,
		int layers[],
		ActivationFunction** activations
	) {

	if (parameters_file_name) {
		layer->parameters_file_name = parameters_file_name;
	}
	if (weights_file_name) {
		layer->weights_file_name = weights_file_name;
	}

	layer->length = length;
	layer->layers = (int*)malloc(sizeof(int) * layer->length);
	for (int i = 0; i < layer->length; ++i) {
		layer->layers[i] = layers[i];
	}

	layer->activations = (ActivationFunction**)malloc(sizeof(ActivationFunction*) * layer->length);
	for (int i = 0; i < layer->length; ++i) {
		layer->activations[i] = activations[i];
	}

	layer->x = (double**)malloc(sizeof(double*) * layer->length);
	for (int i = 0; i < layer->length; ++i) {
		layer->x[i] = (double*)calloc(layer->layers[i], sizeof(double));
	}

	layer->weights = (double***)malloc(sizeof(double**) * (layer->length - 1));
	for (int i = 0; i < layer->length - 1; ++i) {
		layer->weights[i] = (double**)malloc(sizeof(double*) * layer->layers[i]);
		for (int j = 0; j < layer->layers[i]; ++j) {
			layer->weights[i][j] = (double*)malloc(sizeof(double) * layer->layers[i + 1]);
			for (int l = 0; l < layer->layers[i + 1]; ++l) {
				layer->weights[i][j][l] = random(-0.3, 0.3);
			}
		}
	}

	layer->biases = (double**)malloc(sizeof(double*) * (layer->length - 1));
	for (int i = 0; i < layer->length - 1; ++i) {
		layer->biases[i] = (double*)malloc(sizeof(double) * layer->layers[i + 1]);
		for (int l = 0; l < layer->layers[i + 1]; ++l) {
			layer->biases[i][l] = random(-0.3, 0.3);
		}
	}

	layer->__allocated__ = 1;
}


void FullConnected_deallocate(FullConnected* layer) {
	for (int i = 0; i < layer->length - 1; ++i) {
		free(layer->biases[i]);
	}
	free(layer->biases);

	for (int i = 0; i < layer->length - 1; ++i) {
		for (int j = 0; j < layer->layers[i]; ++j) {
			free(layer->weights[i][j]);
		}
		free(layer->weights[i]);
	}
	free(layer->weights);

	for (int i = 0; i < layer->length; ++i) {
		free(layer->x[i]);
	}
	free(layer->x);

	free(layer->activations);

	free(layer->layers);
	
	layer->__allocated__ = 0;
}


void FullConnected_train_allocate(FullConnected* layer, double learning_rate, double momentum, LossFunction* loss) {
	layer->learning_rate = learning_rate;
	layer->momentum = momentum;

	layer->loss = loss;

	layer->err = (double**)malloc(sizeof(double*) * layer->length);
	for (int i = 0; i < layer->length; ++i) {
		layer->err[i] = (double*)calloc(layer->layers[i], sizeof(double));
	}

	layer->delta_weights = (double***)malloc(sizeof(double**) * (layer->length - 1));
	for (int i = 0; i < layer->length - 1; ++i) {
		layer->delta_weights[i] = (double**)malloc(sizeof(double*) * layer->layers[i]);
		for (int j = 0; j < layer->layers[i]; ++j) {
			layer->delta_weights[i][j] = (double*)calloc(layer->layers[i + 1], sizeof(double));
		}
	}

	layer->delta_biases = (double**)malloc(sizeof(double*) * (layer->length - 1));
	for (int i = 0; i < layer->length - 1; ++i) {
		layer->delta_biases[i] = (double*)calloc(layer->layers[i + 1], sizeof(double));
	}

	layer->__train_allocated__ = 1;
}


void FullConnected_train_deallocate(FullConnected* layer) {
	for (int i = 0; i < layer->length - 1; ++i) {
		free(layer->delta_biases[i]);
	}
	free(layer->delta_biases);

	for (int i = 0; i < layer->length - 1; ++i) {
		for (int j = 0; j < layer->layers[i]; ++j) {
			free(layer->delta_weights[i][j]);
		}
		free(layer->delta_weights[i]);
	}
	free(layer->delta_weights);

	for (int i = 0; i < layer->length; ++i) {
		free(layer->err[i]);
	}
	free(layer->err);

	layer->__train_allocated__ = 0;
}


void FullConnected_forward(FullConnected* layer, double* input) {
	for (int j = 0; j < layer->layers[0]; ++j) {
		layer->x[0][j] = input[j];
	}

	for (int i = 0; i < layer->length - 1; ++i) {
		for (int l = 0; l < layer->layers[i + 1]; ++l) {
			layer->x[i + 1][l] = layer->biases[i][l];
			for (int j = 0; j < layer->layers[i]; ++j) {
				layer->x[i + 1][l] += layer->weights[i][j][l] * layer->x[i][j];
			}
			if (i + 1 < layer->length - 1) {
				layer->x[i + 1][l] = layer->activations[i + 1]->f(layer->x[i + 1][l]);
			}
		}
	}
	SoftMax_f(layer->layers[layer->length - 1], layer->x[layer->length - 1], layer->x[layer->length - 1]);
}


void FullConnected_backward(FullConnected* layer, double* output) {
	for (int j = 0; j < layer->layers[layer->length - 1]; ++j) {
		layer->err[layer->length - 1][j] = layer->loss->df(layer->x[layer->length - 1][j], output[j]) * SoftMax_df(layer->x[layer->length - 1][j]);
	}

	for (int i = layer->length - 2; i > -1; --i) {
		for (int j = 0; j < layer->layers[i]; ++j) {
			layer->err[i][j] = 0;
			for (int l = 0; l < layer->layers[i + 1]; ++l) {
				layer->err[i][j] += layer->err[i + 1][l] * layer->weights[i][j][l] * layer->activations[i]->df(layer->x[i][j]);
			}
		}
	}

	for (int i = 0; i < layer->length - 1; ++i) {
		for (int l = 0; l < layer->layers[i + 1]; ++l) {
			for (int j = 0; j < layer->layers[i]; ++j) {
                layer->delta_weights[i][j][l] = -layer->learning_rate * layer->x[i][j] * layer->err[i + 1][l] + layer->momentum * layer->delta_weights[i][j][l];
                layer->weights[i][j][l] += layer->delta_weights[i][j][l];
			}
            layer->delta_biases[i][l] = -layer->learning_rate * layer->err[i + 1][l] + layer->momentum * layer->delta_biases[i][l];
            layer->biases[i][l] += layer->delta_biases[i][l];
		}
	}
}


void FullConnected_load(FullConnected* layer) {
	FILE* file = fopen(layer->parameters_file_name, "r");
	if (file) {
		int length;
		fscanf(file, "%d ", &length);

		int layers[length];
		for (int i = 0; i < length; ++i) {
			fscanf(file, "%d ", &layers[i]);
		}
		
		char function_name[32];
		ActivationFunction* activations[length];
		for (int i = 0; i < length; ++i) {
			fscanf(file, "%s ", &function_name[0]);
			ActivationFunction_load(&activations[i], function_name);
		}

		if (layer->__allocated__ == 1) {
			FullConnected_deallocate(layer);			
		}
		FullConnected_allocate(layer, NULL, NULL, length, layers, activations);

		int __train_construct__;
		fscanf(file, "%d ", &__train_construct__);
		
		if (__train_construct__ == 1) {
			double learning_rate, momentum;
			fscanf(file, "%lf %lf ", &learning_rate, &momentum);

			fscanf(file, "%s ", &function_name[0]);
			LossFunction* loss;
			LossFunction_load(&loss, function_name);

			if (layer->__train_allocated__ == 1) {
				FullConnected_train_deallocate(layer);			
			}
			FullConnected_train_allocate(layer, learning_rate, momentum, loss);
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in FullConnected_load(): file (%s) doesn't exist !!!\n", layer->parameters_file_name);
	}
	#endif // Debug
	fclose(file);
}


void FullConnected_save(FullConnected* layer) {
	FILE* file = fopen(layer->parameters_file_name, "w");
	if (file) {
		fprintf(file, "%d ", layer->length);
		for (int i = 0; i < layer->length; ++i) {
			fprintf(file, "%d ", layer->layers[i]);
		}

		for (int i = 0; i < layer->length; ++i) {
			if (layer->activations[i] == NULL) {
				fprintf(file, "NULL ");
			} else {
				fprintf(file, "%s ", layer->activations[i]->name);
			}	
		}

		fprintf(file, "%d ", layer->__train_allocated__);
		if (layer->__train_allocated__ == 1) {
			fprintf(file, "%lf %lf ", layer->learning_rate, layer->momentum);
			fprintf(file, "%s ", layer->loss->name);
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in FullConnected_save(): file name is %s !!!\n", layer->parameters_file_name);
	}
	#endif // Debug
	fclose(file);
}


void FullConnected_load_weights(FullConnected* layer) {
	FILE* file = fopen(layer->weights_file_name, "r");
	if (file) {
		for (int i = 0; i < layer->length - 1; ++i) {
			for (int j = 0; j < layer->layers[i]; ++j) {
				for (int l = 0; l < layer->layers[i + 1]; ++l) {
					fscanf(file, "%lf ", &layer->weights[i][j][l]);
				}
			}
		}

		for (int i = 0; i < layer->length - 1; ++i) {
			for (int l = 0; l < layer->layers[i + 1]; ++l) {
				fscanf(file, "%lf ", &layer->biases[i][l]);
			}
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in FullConnected_load_weights(): file (%s) doesn't exist !!!\n", layer->weights_file_name);
	}
	#endif // Debug
	fclose(file);
}


void FullConnected_save_weights(FullConnected* layer) {
	FILE* file = fopen(layer->weights_file_name, "w");
	if (file) {
		for (int i = 0; i < layer->length - 1; ++i) {
			for (int j = 0; j < layer->layers[i]; ++j) {
				for (int l = 0; l < layer->layers[i + 1]; ++l) {
					fprintf(file, "%lf ", layer->weights[i][j][l]);
				}
			}
		}

		for (int i = 0; i < layer->length - 1; ++i) {
			for (int l = 0; l < layer->layers[i + 1]; ++l) {
				fprintf(file, "%lf ", layer->biases[i][l]);
			}
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in FullConnected_save_weights(): file name is %s !!!\n", layer->weights_file_name);
	}
	#endif // Debug
	fclose(file);
}


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


void FullConnected_train_threshold(FullConnected* layer, DataSet* data_set, double threshold) {
	int step = 500;
	int epochs = 0;
	double error = FullConnected_calculate_error(layer, data_set);
	while (error > threshold) {
		FullConnected_train(layer, data_set, step);
		epochs += step;
		error = FullConnected_calculate_error(layer, data_set);
		#ifdef Debug
		printf("error %lf\n", error);
		#endif // Debug
	}
	#ifdef Debug
	printf("epochs %d\n", epochs);
	#endif // Debug
}
