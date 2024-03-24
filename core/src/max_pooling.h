typedef struct MaxPooling {
	int channels;
	int height;
	int width;
	int matrix_h;
	int matrix_w;
	double*** input;
	double*** output;
	double*** err;
} MaxPooling;


MaxPooling* MaxPooling_construct(int channels, int height, int width, int matrix_h, int matrix_w);
void MaxPooling_destruct(MaxPooling* layer);

void MaxPooling_train_construct(MaxPooling* layer);
void MaxPooling_train_destruct(MaxPooling* layer);

void MaxPooling_forward(MaxPooling* layer, double*** input);
void MaxPooling_backward(MaxPooling* layer, double*** output);
