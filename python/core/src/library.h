#ifndef LIBRARY_H
#define LIBRARY_H


#define pi 3.14159265359
#define e 2.71828182845


double random(double start, double end);
double random_int(int start, int end);


// Activation Function
typedef struct ActivationFunction {
	const char* name;
	double (*f)(double);
	double (*df)(double);
} ActivationFunction;

extern ActivationFunction Sigmoid;
extern ActivationFunction Tanh;
extern ActivationFunction ReLU;
extern ActivationFunction Leaky_ReLU;

void ActivationFunction_load(ActivationFunction** function, char* name);

void SoftMax_f(int length, double* x, double* y);
double SoftMax_df(double x);


// Loss Function
typedef struct LossFunction {
	const char* name;
	double (*f)(double, double);
	double (*df)(double, double);
} LossFunction;

extern LossFunction MSE;
extern LossFunction MAE;

void LossFunction_load(LossFunction** function, char* name);


#endif // LIBRARY_H
