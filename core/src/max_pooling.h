#ifndef MAX_POOLING_H
#define MAX_POOLING_H


typedef struct MaxPooling {
	const char* file_name;

	int channels;
	int height;
	int width;
	int matrix_h;
	int matrix_w;

	int __output_height__;
	int __output_width__;
	
	double*** input;
	double*** output;
	double*** err;

	int __allocated__;
	int __train_allocated__;
} MaxPooling;


void MaxPooling_allocate(
	MaxPooling* layer,
	const char* file_name,
	int channels,
	int height,
	int width,
	int matrix_h,
	int matrix_w
);
void MaxPooling_deallocate(MaxPooling* layer);

void MaxPooling_train_allocate(MaxPooling* layer);
void MaxPooling_train_deallocate(MaxPooling* layer);

void MaxPooling_forward(MaxPooling* layer, double*** input);
void MaxPooling_backward(MaxPooling* layer, double*** output);

void MaxPooling_load(MaxPooling* layer);
void MaxPooling_save(MaxPooling* layer);


#endif // MAX_POOLING_H
