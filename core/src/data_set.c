#include <stdio.h>
#include <stdlib.h>

#include <data_set.h>


void DataSet_construct(DataSet* data_set, int length, int input_length, int output_length) {
	data_set->length = 0;
	data_set->allocated_length = length;

	data_set->input_length = input_length;
	data_set->output_length = output_length;

	data_set->input = (double**)malloc(sizeof(double*) * data_set->allocated_length);
	data_set->output = (double**)malloc(sizeof(double*) * data_set->allocated_length);
	for (int i = 0; i < data_set->allocated_length; ++i) {
		data_set->input[i] = (double*)malloc(sizeof(double) * input_length);
		data_set->output[i] = (double*)malloc(sizeof(double) * output_length);
	}
}

void DataSet_destruct(DataSet *data_set) {
	for (int i = 0; i < data_set->allocated_length; ++i) {
		free(data_set->input[i]);
		free(data_set->output[i]);
	}
	free(data_set->input);
	free(data_set->output);
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

void DataSet_add(DataSet *data_set, double data[]) {
	if (data_set->length == data_set->allocated_length) {
		DataSet_extend(data_set, 1);
	}

	for (int i = 0; i < data_set->input_length; ++i) {
		data_set->input[data_set->length][i] = data[i];
	}

	for (int i = 0; i < data_set->output_length; ++i) {
		data_set->output[data_set->length][i] = data[data_set->input_length + i];
	}

	++data_set->length;
}

void DataSet_load(DataSet* data_set, const char* title) {
    FILE* file;
    if (file = fopen(title, "r")) {
    	#ifdef Debug
	    if (file == NULL) {
	        printf("!!! ERROR in file handling for loading DataSet !!!\n");
	    }
	    #endif // Debug
	    
        int length, input_length, output_length;
    	fscanf(file, "%d %d %d ", &length, &input_length, &output_length);

    	#ifdef Debug
	    if (input_length != data_set->input_length) {
	    	printf("!!! ERROR in loading ImageSet: input_length (%d) doesn't equal current ImageSet (%d) !!!\n", input_length, data_set->input_length);
	    }
	    if (output_length != data_set->output_length) {
	    	printf("!!! ERROR in loading ImageSet: output_length (%d) doesn't equal current ImageSet (%d) !!!\n", output_length, data_set->output_length);
	    }
	    #endif // Debug


	    if (data_set->allocated_length - data_set->length < length) {
	    	DataSet_extend(data_set, length - (data_set->allocated_length - data_set->length));
	    }

	    for (int i = 0; i < length; ++i) {
	    	for (int j = 0; j < data_set->input_length; ++j) {
	    		fscanf(file, "%lf ", &data_set->input[data_set->length + i][j]);
	    	}
	    	for (int j = 0; j < data_set->output_length; ++j) {
	    		fscanf(file, "%lf ", &data_set->output[data_set->length + i][j]);
	    	}
	    }

	    data_set->length += length;
	    
	    fclose(file);
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in file loading: file with DataSet data doen't exist !!!\n");
	}
    #endif // Debug
}

void DataSet_save(DataSet* data_set, const char* title) {
    FILE* file = fopen(title, "w");
	#ifdef Debug
    if (file == NULL) {
        printf("!!! ERROR in file handling for saving DataSet !!!\n");
    }
    #endif // Debug

    fprintf(file, "%d %d %d ", data_set->length, data_set->input_length, data_set->output_length);
    for (int i = 0; i < data_set->length; ++i) {
    	for (int j = 0; j < data_set->input_length; ++j) {
    		fprintf(file, "%lf ", data_set->input[i][j]);
    	}
    	for (int j = 0; j < data_set->output_length; ++j) {
    		fprintf(file, "%lf ", data_set->output[i][j]);
    	}
    }

    fclose(file);
}
