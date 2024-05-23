#include <stdio.h>
#include <stdlib.h>

#include <data_set.h>


void DataSet_allocate(
		DataSet* data_set,
		const char* file_name,
		int length,
		int batch_size,
		int input_length,
		int output_length
	) {
	if (file_name) {
		data_set->file_name = file_name;
	}

	data_set->length = 0;
	data_set->allocated_length = length;
	data_set->batch_size = batch_size;

	data_set->input_length = input_length;
	data_set->output_length = output_length;

	data_set->input = (double**)malloc(sizeof(double*) * data_set->allocated_length);
	data_set->output = (double**)malloc(sizeof(double*) * data_set->allocated_length);
	for (int i = 0; i < data_set->allocated_length; ++i) {
		data_set->input[i] = (double*)malloc(sizeof(double) * input_length);
		data_set->output[i] = (double*)malloc(sizeof(double) * output_length);
	}

	data_set->__allocated__ = 1;
}


void DataSet_deallocate(DataSet *data_set) {
	for (int i = 0; i < data_set->allocated_length; ++i) {
		free(data_set->input[i]);
		free(data_set->output[i]);
	}
	free(data_set->input);
	free(data_set->output);

	data_set->__allocated__ = 0;
}


void DataSet_extend(DataSet* data_set, int length) {
	data_set->input = (double**)realloc(data_set->input, sizeof(double*) * (data_set->allocated_length + length));
	data_set->output = (double**)realloc(data_set->output, sizeof(double*) * (data_set->allocated_length + length));
	for (int i = 0; i < length; ++i) {
		data_set->input[data_set->allocated_length + i] = (double*)malloc(sizeof(double) * data_set->input_length);
		data_set->output[data_set->allocated_length + i] = (double*)malloc(sizeof(double) * data_set->output_length);
	}
	data_set->allocated_length += length;
}


void DataSet_add(DataSet *data_set, double* data) {
	if (data_set->length == data_set->allocated_length) {
		DataSet_extend(data_set, data_set->batch_size);
	}

	for (int i = 0; i < data_set->input_length; ++i) {
		data_set->input[data_set->length][i] = data[i];
	}
	for (int i = 0; i < data_set->output_length; ++i) {
		data_set->output[data_set->length][i] = data[data_set->input_length + i];
	}
	++data_set->length;
}


void DataSet_clear(DataSet* data_set) {
	data_set->length = 0;
}


void DataSet_load(DataSet* data_set) {
    FILE* file = fopen(data_set->file_name, "r");
    if (file) {
        int length, batch_size, input_length, output_length;
    	fscanf(file, "%d %d %d %d ", &length, &batch_size, &input_length, &output_length);

    	if (data_set->__allocated__ == 1) {
    		DataSet_deallocate(data_set);
    	}
    	DataSet_allocate(data_set, NULL, length, batch_size, input_length, output_length);

	    for (int i = 0; i < length; ++i) {
	    	for (int j = 0; j < data_set->input_length; ++j) {
	    		fscanf(file, "%lf ", &data_set->input[i][j]);
	    	}
	    	for (int j = 0; j < data_set->output_length; ++j) {
	    		fscanf(file, "%lf ", &data_set->output[i][j]);
	    	}
	    }

	    data_set->length = length;
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in DataSet_load(): file (%s) doesn't exist !!!\n", data_set->file_name);
	}
    #endif // Debug
    fclose(file);
}


void DataSet_save(DataSet* data_set) {
    FILE* file = fopen(data_set->file_name, "w");
    if (file) {
	    fprintf(file, "%d %d %d %d ", data_set->length, data_set->batch_size, data_set->input_length, data_set->output_length);
	    for (int i = 0; i < data_set->length; ++i) {
	    	for (int j = 0; j < data_set->input_length; ++j) {
	    		fprintf(file, "%lf ", data_set->input[i][j]);
	    	}
	    	for (int j = 0; j < data_set->output_length; ++j) {
	    		fprintf(file, "%lf ", data_set->output[i][j]);
	    	}
	    }
	}
	#ifdef Debug
    else {
        printf("!!! WARNING in DataSet_save(): file name is %s !!!\n", data_set->file_name);
    }
    #endif // Debug
    fclose(file);
}
