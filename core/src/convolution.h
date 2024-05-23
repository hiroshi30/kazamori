#ifndef CONVOLUTION_H
#define CONVOLUTION_H


typedef struct Convolution {
	const char* parameters_file_name;
	const char* weights_file_name;

	int channels;
	int height;
	int width;
	int matrix_c;
	int matrix_h;
	int matrix_w;
	int padding;
	int stride_h;
	int stride_w;

	double learning_rate;
	double momentum;

	int __input_height__;
	int __input_width__;
	int __output_height__;
	int __output_width__;

	double*** input;
	double*** output;
	double**** matrix;
	double*** bias;

	double*** err;
	double**** delta_matrix;
	double*** delta_bias;

	ActivationFunction *activation;
	LossFunction *loss;

	int __allocated__;
	int __train_allocated__;
} Convolution;


void Convolution_allocate(
	Convolution* layer,
	const char* parameters_file_name,
	const char* weights_file_name,
	int channels,
	int height,
	int width,
	int matrix_c,
	int matrix_h,
	int matrix_w,
	int padding,
	int stride_h,
	int stride_w,
	ActivationFunction *activation
);
void Convolution_deallocate(Convolution* layer);

void Convolution_train_allocate(Convolution* layer, double learning_rate, double momentum,	LossFunction *loss);
void Convolution_train_deallocate(Convolution* layer);

void Convolution_forward(Convolution* layer, double*** input);
void Convolution_backward(Convolution* layer, double*** output);

void Convolution_load(Convolution* layer);
void Convolution_save(Convolution* layer);
void Convolution_load_weights(Convolution* layer);
void Convolution_save_weights(Convolution* layer);


#endif // CONVOLUTION_H
