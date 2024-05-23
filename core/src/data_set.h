#ifndef DATA_SET_H
#define DATA_SET_H


typedef struct DataSet {
	const char* file_name;

	int length;
	int allocated_length;
	int batch_size;

	int input_length;
	int output_length;
	
	double** input;
	double** output;
	
	int __allocated__;
} DataSet;


void DataSet_allocate(
	DataSet* data_set,
	const char* file_name,
	int length,
	int batch_size,
	int input_length,
	int output_length
);
void DataSet_deallocate(DataSet* data_set);

void DataSet_extend(DataSet* data_set, int length);
void DataSet_add(DataSet* data_set, double* data);
void DataSet_clear(DataSet* data_set);

void DataSet_load(DataSet* data_set);
void DataSet_save(DataSet* data_set);


#endif // DATA_SET_H
