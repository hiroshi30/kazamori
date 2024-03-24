#include <stdio.h>

#include <tab.h>
#include <data_set.h>


void DataSet_print(DataSet* data_set) {
	printf("DataSet (%d/%d) {\n", data_set->length, data_set->allocated_length);
	for (int i = 0; i < data_set->allocated_length; ++i) {
		printf(TAB);
		printf("index %d: {", i);

		printf(" {");
		for (int j = 0; j < data_set->input_length; ++j) {
			if (j < data_set->input_length - 1) {
				printf("%lf, ", data_set->input[i][j]);
			} else {
				printf("%lf", data_set->input[i][j]);
			}
		}
		printf("}, ");

		printf("{");
		for (int j = 0; j < data_set->output_length; ++j) {
			if (j < data_set->output_length - 1) {
				printf("%lf, ", data_set->output[i][j]);
			} else {
				printf("%lf", data_set->output[i][j]);
			}
		}

		printf("} ");
		
		if (i < data_set->allocated_length - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	printf("}\n");
}
