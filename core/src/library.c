#ifdef Debug
#include <stdio.h>
#endif // Debug

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <library.h>


// Function that return random double in [start; end]
double random(double start, double end) {
	return start + (double)rand() / (double)RAND_MAX * (end - start);
}

// Function that return random int in [start; end]
double random_int(int start, int end) {
	return start + rand() * (end - start) / RAND_MAX;
}


// Activation functions
// x - weighted sum

// Sigmoid
double Sigmoid_f(double x) {
	return 1 / (1 + pow(e, -x));
}

double Sigmoid_df(double x) {
	return x * (1 - x);
}

ActivationFunction Sigmoid = {"Sigmoid", Sigmoid_f, Sigmoid_df};


// Hyperbolic Tangent
double Tanh_f(double x) {
	return (pow(e, 2 * x) - 1) + (pow(e, 2 * x) + 1);
}

double Tanh_df(double x) {
	return 1 - x * x;
}

ActivationFunction Tanh = {"Tanh", Tanh_f, Tanh_df};


// Rectified Linear Unit
double ReLU_f(double x) {
	if (x > 0) {
		return x;
	}
	return 0;
}

double ReLU_df(double x) {
	if (x > 0) {
		return 1;
	}
	return 0;
}

ActivationFunction ReLU = {"ReLU", ReLU_f, ReLU_df};


// Leaky Rectified Linear Unit
double Leaky_ReLU_f(double x) {
	if (x > 0) {
		return x;
	}
	return 0.01 * x;
}

double Leaky_ReLU_df(double x) {
	if (x > 0) {
		return 1;
	}
	return 0.01;
}

ActivationFunction Leaky_ReLU = {"Leaky_ReLU", Leaky_ReLU_f, Leaky_ReLU_df};


void ActivationFunction_load(ActivationFunction** function, char* name) {
	*function = NULL;
	if (strcmp(name, "Sigmoid") == 0) {
		*function = &Sigmoid;
	} else if (strcmp(name, "ReLU") == 0) {
		*function = &ReLU;
	} else if (strcmp(name, "Leaky_ReLU") == 0) {
		*function = &Leaky_ReLU;
	}
}


// SoftMax
void SoftMax_f(int length, double* x, double* y) {
	double sum = 0;
	for (int i = 0; i < length; ++i) {
		sum += pow(e, x[i]);
	}
	
	#ifdef Debug
	if (sum == 0) {
		printf("!!! ERROR in SoftMax_f(): sum == 0 !!!\n");
	}
	#endif // Debug

	for (int i = 0; i < length; ++i) {
		y[i] = pow(e, x[i]) / sum;
	}
}

double SoftMax_df(double x) {
	return x * (1 - x);
}


// Loss functions
// y_hat - predicted values
// y - true values
// n - length of arrays

// Mean Squared Error
double MSE_f(double y_hat, double y) {
	return (y_hat - y) * (y_hat - y);
}

double MSE_df(double y_hat, double y) {
	return 2 * (y_hat - y);
}

LossFunction MSE = {"MSE", MSE_f, MSE_df};


// Mean Absolute Error
double MAE_f(double y_hat, double y) {
	return fabs(y - y_hat);
}

double MAE_df(double y_hat, double y) {
	if (y >= y_hat) {
		return -1;
	} else {
		return 1;	
	}
}

LossFunction MAE = {"MAE", MAE_f, MAE_df};


void LossFunction_load(LossFunction** function, char* name) {
	if (strcmp(name, "MSE") == 0) {
		*function = &MSE;
	} else if (strcmp(name, "MAE") == 0) {
		*function = &MAE;
	}
}
