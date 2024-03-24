#ifndef FULL_CONNECTED_H
#define FULL_CONNECTED_H


typedef struct FullConnected {
	int length;
	int* layers;

	double** x;
	double*** weights;
	double** biases;
	double** err;
	double*** delta_weights;
	double** delta_biases;

	double learning_rate;
	double momentum;

	ActivationFunction *activation;
	LossFunction *loss;
} FullConnected;

void FullConnected_construct(
	FullConnected* layer,
	int length,
	int layers[],
	ActivationFunction *activation,
	LossFunction *loss
);
void FullConnected_destruct(FullConnected* layer);

void FullConnected_train_construct(FullConnected* layer, double learning_rate, double momentum);
void FullConnected_train_destruct(FullConnected* layer);

void FullConnected_forward(FullConnected* layer, double* input);
void FullConnected_backward(FullConnected* layer, double* output);


#endif // FULL_CONNECTED_H
