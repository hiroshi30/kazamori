#ifndef FULL_CONNECTED_H
#define FULL_CONNECTED_H


typedef struct FullConnected {
	const char* parameters_file_name;
	const char* weights_file_name;

	int length;
	int* layers;
	double learning_rate;
	double momentum;

	double** x;
	double*** weights;
	double** biases;
	double** err;
	double*** delta_weights;
	double** delta_biases;

	ActivationFunction** activations;
	LossFunction* loss;

	int __allocated__;
	int __train_allocated__;
} FullConnected;


void FullConnected_allocate(
	FullConnected* layer, 
	const char* parameters_file_name,
	const char* weights_file_name,
	int length,
	int layers[],
	ActivationFunction** activations
);
void FullConnected_deallocate(FullConnected* layer);

void FullConnected_train_allocate(FullConnected* layer, double learning_rate, double momentum, LossFunction *loss);
void FullConnected_train_deallocate(FullConnected* layer);

void FullConnected_forward(FullConnected* layer, double* input);
void FullConnected_backward(FullConnected* layer, double* output);

void FullConnected_load(FullConnected* layer);
void FullConnected_save(FullConnected* layer);
void FullConnected_load_weights(FullConnected* layer);
void FullConnected_save_weights(FullConnected* layer);

double FullConnected_calculate_error(FullConnected* layer, DataSet* data_set);
void FullConnected_train(FullConnected* layer, DataSet* data_set, int epochs);
void FullConnected_train_threshold(FullConnected* layer, DataSet* data_set, double threshold);


#endif // FULL_CONNECTED_H
