#ifndef LIBRARY_H
#define LIBRARY_H


#define pi 3.14159265359
#define e 2.71828182845


double random(double start, double end);
double random_int(int start, int end);


typedef struct ActivationFunction {
	double (*f)(double);
	double (*df)(double);
} ActivationFunction;


typedef struct LossFunction {
	double (*f)(double, double);
	double (*df)(double, double);
} LossFunction;


extern ActivationFunction Sigmoid;
extern ActivationFunction Tanh;
extern ActivationFunction ReLU;
extern ActivationFunction Leaky_ReLU;

void SoftMax_f(int length, double* x, double* y);
double SoftMax_df(double x);

extern LossFunction MSE;
extern LossFunction MAE;


#endif // LIBRARY_H
