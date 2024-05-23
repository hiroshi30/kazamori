#include <stdio.h>

#include <tab.h>
#include <data_set.h>


void DataSet_print(DataSet* data_set) {
	printf("DataSet (%d/%d) {\n", data_set->length, data_set->allocated_length);
	for (int i = 0; i < data_set->length; ++i) {
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
		
		if (i < data_set->length - 1) {
			printf("},\n");
		} else {
			printf("}\n");
		}
	}
	printf("}\n");
}


void DataSet_print_info(DataSet* data_set) {
	printf("DataSet info\n");
	
	printf(TAB);
	printf("__allocated__ = %d\n", data_set->__allocated__);
	if (data_set->__allocated__ == 1) {
		printf(TAB);
		printf("length = %d\n", data_set->length);
		printf(TAB);
		printf("allocated_length = %d\n", data_set->allocated_length);
		printf(TAB);
		printf("batch_size = %d\n", data_set->batch_size);
		printf(TAB);
		printf("input_length = %d\n", data_set->input_length);
		printf(TAB);
		printf("output_length = %d\n", data_set->output_length);
	}
}
