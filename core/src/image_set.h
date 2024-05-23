#ifndef IMAGE_SET_H
#define IMAGE_SET_H


typedef struct ImageSet {
	const char* file_name;

	int length;
	int allocated_length;
	int batch_size;

	int channels;
	int height;
	int width;
	int output_length;

	double**** input;
	double** output;

	int __allocated__;
} ImageSet;


void ImageSet_allocate(
	ImageSet* image_set,
	const char* file_name,
	int length,
	int batch_size,
	int channels,
	int height,
	int width,
	int output_length
);
void ImageSet_deallocate(ImageSet* image_set);

void ImageSet_extend(ImageSet* image_set, int length);
void ImageSet_add(ImageSet* image_set, double* data);

void ImageSet_load(ImageSet* image_set);
void ImageSet_save(ImageSet* image_set);


#endif // IMAGE_SET_H
