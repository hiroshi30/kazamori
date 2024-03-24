#ifndef DATA_SET_H
#define DATA_SET_H


typedef struct DataSet {
	int length;
	int allocated_length;

	int input_length;
	int output_length;
	
	double** input;
	double** output;
} DataSet;

void DataSet_construct(DataSet* data_set, int length, int input_length, int output_length);
void DataSet_destruct(DataSet* data_set);

void DataSet_extend(DataSet* data_set, int length);
void DataSet_add(DataSet* data_set, double data[]);
void DataSet_load(DataSet* data_set, const char* title);
void DataSet_save(DataSet* data_set, const char* title);


#endif // DATA_SET_H
