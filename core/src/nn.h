double FullConnected_calculate_error(FullConnected* layer, DataSet* data_set);
void FullConnected_train(FullConnected* layer, DataSet* data_set, int epochs);
int FullConnected_train_alpha(FullConnected* layer, DataSet* data_set, double alpha);
void FullConnected_check(FullConnected* layer, DataSet* data_set);
