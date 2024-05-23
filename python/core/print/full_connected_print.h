#ifndef FULL_CONNECTED_PRINT_H
#define FULL_CONNECTED_PRINT_H


void FullConnected_print_weights(FullConnected* layer);
void FullConnected_print_biases(FullConnected* layer);
void FullConnected_print_x(FullConnected* layer);
void FullConnected_print_delta_weights(FullConnected* layer);
void FullConnected_print_delta_biases(FullConnected* layer);
void FullConnected_print_err(FullConnected* layer);

void FullConnected_print(FullConnected* layer);
void FullConnected_print_info(FullConnected* layer);

void FullConnected_check(FullConnected* layer, DataSet* data_set);


#endif // FULL_CONNECTED_PRINT_H
