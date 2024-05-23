#ifndef kNN_h
#define kNN_h


void kNN_classification(DataSet* data_set, int k, double* input, double* output);
void kNN_regression(DataSet* data_set, int k, double* input, double* output);
void weighted_kNN(DataSet* data_set, int k, double* input, double* output);


#endif // kNN_h
