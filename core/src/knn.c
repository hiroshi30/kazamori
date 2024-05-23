#include <math.h>
#include <float.h>

#include <data_set.h>


void kNN_classification(DataSet* data_set, int k, double* input, double* output) {
	double distances[k];
	int outputs[k];
	for (int i = 0; i < k; ++i) {
		distances[i] = DBL_MAX;
		outputs[i] = 0;
	}

	double distance;
	double swap_distances;
	int swap_outputs;
	for (int i = 0; i < data_set->length; ++i) {
		distance = 0;
		for (int j = 0; j < data_set->input_length; ++j) {
			distance += (input[j] - data_set->input[i][j]) * (input[j] - data_set->input[i][j]);
		}
	
		if (distance < distances[k - 1]) {
			distances[k - 1] = distance;

			for (int j = 0; j < data_set->output_length; ++j) {
				if (data_set->output[i][j] == 1) {
					outputs[k - 1] = j;
				}
			}

			for (int j = k - 2; j >= 0; --j) {
				if (distances[j + 1] >= distances[j]) {
					break;
				}
			
				swap_distances = distances[j + 1];
				distances[j + 1] = distances[j];
				distances[j] = swap_distances;

				swap_outputs = outputs[j + 1];
				outputs[j + 1] = outputs[j];
				outputs[j] = swap_outputs;
			}
		}
	}
	
	int counts[data_set->output_length];
	for (int i = 0; i < data_set->output_length; ++i) {
		counts[i] = 0;
	}

	for (int i = 0; i < k; ++i) {
		counts[outputs[i]] += 1;
	}

	int index = 0;
	for (int i = 0; i < data_set->output_length; ++i) {
		output[i] = 0;
		if (counts[i] > counts[index]) {
			index = i;
		}
	}
	output[index] = 1;
}


void kNN_regression(DataSet* data_set, int k, double* input, double* output) {
	double distances[k];
	int outputs[k];
	for (int i = 0; i < k; ++i) {
		distances[i] = DBL_MAX;
		outputs[i] = 0;
	}

	double distance;
	double swap_distances;
	int swap_outputs;
	for (int i = 0; i < data_set->length; ++i) {
		distance = 0;
		for (int j = 0; j < data_set->input_length; ++j) {
			distance += (input[j] - data_set->input[i][j]) * (input[j] - data_set->input[i][j]);
		}
	
		if (distance < distances[k - 1]) {
			distances[k - 1] = distance;
			outputs[k - 1] = i;

			for (int j = k - 2; j >= 0; --j) {
				if (distances[j + 1] >= distances[j]) {
					break;
				}
			
				swap_distances = distances[j + 1];
				distances[j + 1] = distances[j];
				distances[j] = swap_distances;

				swap_outputs = outputs[j + 1];
				outputs[j + 1] = outputs[j];
				outputs[j] = swap_outputs;
			}
		}
	}
	
	for (int i = 0; i < data_set->output_length; ++i) {
		output[i] = 0;
		for (int j = 0; j < k; ++j) {
			output[i] += data_set->output[outputs[j]][i];
		}
		output[i] /= k;
	}
}


void weighted_kNN(DataSet* data_set, int k, double* input, double* output) {
	double distances[k];
	int outputs[k];
	for (int i = 0; i < k; ++i) {
		distances[i] = DBL_MAX;
		outputs[i] = 0;
	}

	double distance;
	double swap_distances;
	int swap_outputs;
	for (int i = 0; i < data_set->length; ++i) {
		distance = 0;
		for (int j = 0; j < data_set->input_length; ++j) {
			distance += (input[j] - data_set->input[i][j]) * (input[j] - data_set->input[i][j]);
		}
	
		if (distance < distances[k - 1]) {
			distances[k - 1] = distance;
			outputs[k - 1] = i;

			for (int j = k - 2; j >= 0; --j) {
				if (distances[j + 1] >= distances[j]) {
					break;
				}
			
				swap_distances = distances[j + 1];
				distances[j + 1] = distances[j];
				distances[j] = swap_distances;

				swap_outputs = outputs[j + 1];
				outputs[j + 1] = outputs[j];
				outputs[j] = swap_outputs;
			}
		}
	}
	
	double sum_distances = 0;
	for (int i = 0; i < k; ++i) {
		distances[i] = 1 / sqrt(distances[i]);
		sum_distances += distances[i];
	}

	for (int i = 0; i < data_set->output_length; ++i) {
		output[i] = 0;
		for (int j = 0; j < k; ++j) {
			output[i] += data_set->output[outputs[j]][i] * distances[j] / sum_distances;
		}
	}
}
