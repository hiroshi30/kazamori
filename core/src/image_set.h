typedef struct ImageSet {
	int length;
	
	int allocated_length;
	int channels;
	int height;
	int width;
	int output_length;

	double**** input;
	double** output;
} ImageSet;

ImageSet* ImageSet_construct(int length, int channels, int height, int width, int output_length);
void ImageSet_destruct(ImageSet* image_set);

void ImageSet_extend(ImageSet* image_set, int length);
void ImageSet_add(ImageSet* image_set, double input[], double output[]);
void ImageSet_load(ImageSet* image_set, const char* title);
void ImageSet_save(ImageSet* image_set, const char* title);
