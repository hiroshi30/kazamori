#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <library.h>
#include <convolution.h>


void Convolution_allocate(
		Convolution* layer,
		const char* parameters_file_name,
		const char* weights_file_name,
		int channels,
		int height,
		int width,
		int matrix_c,
		int matrix_h,
		int matrix_w,
		int padding,
		int stride_h,
		int stride_w,
		ActivationFunction *activation
	) {

	if (parameters_file_name) {
		layer->parameters_file_name = parameters_file_name;
	}
	if (weights_file_name) {
		layer->weights_file_name = weights_file_name;
	}

	layer->channels = channels;
	layer->height = height;
	layer->width = width;

	layer->matrix_c = matrix_c;
	layer->matrix_h = matrix_h;
	layer->matrix_w = matrix_w;
	
	layer->padding = padding;
	
	layer->stride_h = stride_h;
	layer->stride_w = stride_w;

	layer->activation = activation;

	layer->__input_height__ = layer->height + 2 * layer->padding;
	layer->__input_width__ = layer->width + 2 * layer->padding;
	layer->__output_height__ = 1 + (layer->__input_height__ - layer->matrix_h) / layer->stride_h;
	layer->__output_width__ = 1 + (layer->__input_width__ - layer->matrix_w) / layer->stride_w;

	layer->input = (double***)malloc(sizeof(double**) * layer->channels);
    for (int c = 0; c < layer->channels; ++c) {
        layer->input[c] = (double**)malloc(sizeof(double*) * layer->__input_height__);
        for (int ih = 0; ih < layer->__input_height__; ++ih) {
            layer->input[c][ih] = (double*)calloc(layer->__input_width__, sizeof(double));
        }
    }

    layer->output = (double***)malloc(sizeof(double**) * layer->matrix_c);
    for (int mc = 0; mc < layer->matrix_c; ++mc) {
        layer->output[mc] = (double**)malloc(sizeof(double*) * layer->__output_height__);
        for (int oh = 0; oh < layer->__output_height__; ++oh) {
            layer->output[mc][oh] = (double*)calloc(layer->__output_width__, sizeof(double));
        }
    }

    layer->matrix = (double****)malloc(sizeof(double***) * layer->matrix_c);
    for (int mc = 0; mc < layer->matrix_c; ++mc) {
	    layer->matrix[mc] = (double***)malloc(sizeof(double**) * layer->channels);
	    for (int c = 0; c < layer->channels; ++c) {
	    	layer->matrix[mc][c] = (double**)malloc(sizeof(double*) * layer->matrix_h);
		    for (int mh = 0; mh < layer->matrix_h; ++mh) {
			    layer->matrix[mc][c][mh] = (double*)malloc(sizeof(double) * layer->matrix_w);
			    for (int mw = 0; mw < layer->matrix_w; ++mw) {
			    	layer->matrix[mc][c][mh][mw] = random(-0.25, 0.25);
			    }
		    }
		}
	}

	layer->bias = (double***)malloc(sizeof(double**) * layer->matrix_c);
	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		layer->bias[mc] = (double**)malloc(sizeof(double*) * layer->__output_height__);
		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			layer->bias[mc][oh] = (double*)malloc(sizeof(double) * layer->__output_width__);
			for (int ow = 0; ow < layer->__output_width__; ++ow) {
				layer->bias[mc][oh][ow] = random(-0.25, 0.25);
			}
		}
	}

	layer->__allocated__ = 1;
}


void Convolution_deallocate(Convolution* layer) {
    for (int mc = 0; mc < layer->matrix_c; ++mc) {
		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			free(layer->bias[mc][oh]);
		}
		free(layer->bias[mc]);
	}
	free(layer->bias);
    
    for (int mc = 0; mc < layer->matrix_c; ++mc) {
	    for (int c = 0; c < layer->channels; ++c) {
		    for (int mh = 0; mh < layer->matrix_h; ++mh) {
		    	free(layer->matrix[mc][c][mh]);
		    }
		    free(layer->matrix[mc][c]);
		}
		free(layer->matrix[mc]);
	}
	free(layer->matrix);

    for (int mc = 0; mc < layer->matrix_c; ++mc) {
        for (int oh = 0; oh < layer->__output_height__; ++oh) {
            free(layer->output[mc][oh]);
        }
        free(layer->output[mc]);
    }
    free(layer->output);

	for (int c = 0; c < layer->channels; ++c) {
        for (int ih = 0; ih < layer->__input_height__; ++ih) {
            free(layer->input[c][ih]);
        }
        free(layer->input[c]);
    }
    free(layer->input);

	layer->__allocated__ = 0;
}


void Convolution_train_allocate(Convolution* layer, double learning_rate, double momentum, LossFunction *loss) {
	layer->learning_rate = learning_rate;
	layer->momentum = momentum;

	layer->loss = loss;

	layer->err = (double***)malloc(sizeof(double**) * layer->channels);
	for (int c = 0; c < layer->channels; ++c) {
		layer->err[c] = (double**)malloc(sizeof(double*) * layer->height);
		for (int h = 0; h < layer->height; ++h) {
			layer->err[c][h] = (double*)calloc(layer->width, sizeof(double));
		}
	}

	layer->delta_matrix = (double****)malloc(sizeof(double***) * layer->matrix_c);
    for (int mc = 0; mc < layer->matrix_c; ++mc) {
	    layer->delta_matrix[mc] = (double***)malloc(sizeof(double**) * layer->channels);
	    for (int c = 0; c < layer->channels; ++c) {
	    	layer->delta_matrix[mc][c] = (double**)malloc(sizeof(double*) * layer->matrix_h);
		    for (int mh = 0; mh < layer->matrix_h; ++mh) {
			    layer->delta_matrix[mc][c][mh] = (double*)calloc(layer->matrix_w, sizeof(double));
		    }
		}
	}

	layer->delta_bias = (double***)malloc(sizeof(double**) * layer->matrix_c);
	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		layer->delta_bias[mc] = (double**)malloc(sizeof(double*) * layer->__output_height__);
		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			layer->delta_bias[mc][oh] = (double*)calloc(layer->__output_width__, sizeof(double));
		}
	}

	layer->__train_allocated__ = 1;
}


void Convolution_train_deallocate(Convolution* layer) {
	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			free(layer->delta_bias[mc][oh]);
		}
		free(layer->delta_bias[mc]);
	}
	free(layer->delta_bias);

    for (int mc = 0; mc < layer->matrix_c; ++mc) {
	    for (int c = 0; c < layer->channels; ++c) {
		    for (int mh = 0; mh < layer->matrix_h; ++mh) {
		    	free(layer->delta_matrix[mc][c][mh]);
		    }
		    free(layer->delta_matrix[mc][c]);
		}
		free(layer->delta_matrix[mc]);
	}
	free(layer->delta_matrix);

	for (int c = 0; c < layer->channels; ++c) {
		for (int h = 0; h < layer->height; ++h) {
			free(layer->err[c][h]);
		}
		free(layer->err[c]);
	}
	free(layer->err);

	layer->__train_allocated__ = 0;
}


void Convolution_forward(Convolution* layer, double*** input) {
	// copying input to the layer->input IS NOT EFFICIENT,
	// you can just change pointer layer->input to the input,
	// if you will not change layer->input data and you will not change data in input (ImageSet)
	// you also can even dont save input data and get it only in functions
	// I see problems with converting list[1 * n] to list[channels * height * width]
	// When output from FullConnected layer come to the Convolution input

	for (int c = 0; c < layer->channels; ++c) {
		for (int h = 0; h < layer->height; ++h) {
			for (int w = 0; w < layer->width; ++w) {
				layer->input[c][h + layer->padding][w + layer->padding] = input[c][h][w];
			}
		}
	}

	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			for (int ow = 0; ow < layer->__output_width__; ++ow) {
				layer->output[mc][oh][ow] = layer->bias[mc][oh][ow];
				for (int c = 0; c < layer->channels; ++c) {
					for (int mh = 0; mh < layer->matrix_h; ++mh) {
						for (int mw = 0; mw < layer->matrix_w; ++mw) {
							layer->output[mc][oh][ow] += layer->input[c][oh * layer->stride_h + mh][ow * layer->stride_w + mw] * layer->matrix[mc][c][mh][mw];
						}
					}
				}
				layer->output[mc][oh][ow] = layer->activation->f(layer->output[mc][oh][ow]);
			}
		}
	}
}


void Convolution_backward(Convolution* layer, double*** output) {
	for (int mc = 0; mc < layer->matrix_c; ++mc) {
	    for (int c = 0; c < layer->channels; ++c) {
		    for (int mh = 0; mh < layer->matrix_h; ++mh) {
			    for (int mw = 0; mh < layer->matrix_w; ++mw) {
			    	layer->delta_matrix[mc][c][mh][mw] *= layer->momentum;
			    }
		    }
		}
	}

	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			for (int ow = 0; ow < layer->__output_width__; ++ow) {
				layer->output[mc][oh][ow] = layer->bias[mc][oh][ow];
				for (int c = 0; c < layer->channels; ++c) {
					for (int mh = 0; mh < layer->matrix_h; ++mh) {
						for (int mw = 0; mw < layer->matrix_w; ++mw) {
							layer->delta_matrix[mc][c][mh][mw] += -layer->learning_rate * layer->input[c][oh * layer->stride_h + mh][ow * layer->stride_w + mw] * output[mc][oh][ow];
						}
					}
				}
				layer->delta_bias[mc][oh][ow] = -layer->learning_rate * output[mc][oh][ow] + layer->momentum * layer->delta_bias[mc][oh][ow];
				layer->bias[mc][oh][ow] += layer->delta_bias[mc][oh][ow];
			}
		}
	}

	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		for (int c = 0; c < layer->channels; ++c) {
			for (int mh = 0; mh < layer->matrix_h; ++mh) {
				for (int mw = 0; mw < layer->matrix_w; ++mw) {
					layer->matrix[mc][c][mh][mw] += layer->delta_matrix[mc][c][mh][mw];
				}
			}
		}
	}

	for (int c = 0; c < layer->channels; ++c) {
		for (int h = 0; h < layer->height; ++h) {
			for (int w = 0; w < layer->width; ++w) {
				layer->err[c][h][w] = 0;
			}
		}
	}

	for (int mc = 0; mc < layer->matrix_c; ++mc) {
		for (int oh = 0; oh < layer->__output_height__; ++oh) {
			for (int ow = 0; ow < layer->__output_width__; ++ow) {
				for (int c = 0; c < layer->channels; ++c) {
					for (int mh = 0; mh < layer->matrix_h; ++mh) {
						for (int mw = 0; mw < layer->matrix_w; ++mw) {
							layer->err[c][oh * layer->stride_h + mh][ow * layer->stride_w + mw] += output[mc][oh][ow] * layer->matrix[mc][c][mh][mw]; // * self.activation.df(self.x[mc][hh][ww])
						}
					}
				}
			}
		}
	}
}


void Convolution_load(Convolution* layer) {
	FILE* file = fopen(layer->parameters_file_name, "r");
	if (file) {
		int channels, height, width, matrix_c, matrix_h, matrix_w, padding, stride_h, stride_w;
		fscanf(file, "%d %d %d %d %d %d %d %d %d ",
			&channels,
			&height,
			&width,
			&matrix_c,
			&matrix_h,
			&matrix_w,
			&padding,
			&stride_h,
			&stride_w
		);
		
		char function_name[32];
		fscanf(file, "%s ", &function_name[0]);
		ActivationFunction* activation;
		ActivationFunction_load(&activation, function_name);

		if (layer->__allocated__ == 1) {
			Convolution_deallocate(layer);			
		}
		Convolution_allocate(layer, NULL, NULL, channels, height, width, matrix_c, matrix_h, matrix_w, padding, stride_h, stride_w, activation);

		int __train_construct__;
		fscanf(file, "%d ", &__train_construct__);

		if (__train_construct__ == 1) {
			double learning_rate, momentum;
			fscanf(file, "%lf %lf ", &learning_rate, &momentum);

			fscanf(file, "%s ", &function_name[0]);
			LossFunction* loss;
			LossFunction_load(&loss, function_name);

			if (layer->__train_allocated__ == 1) {
				Convolution_train_deallocate(layer);			
			}
			Convolution_train_allocate(layer, learning_rate, momentum, loss);
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in Convolution_load(): file (%s) doesn't exist !!!\n", layer->parameters_file_name);
	}
	#endif // Debug
	fclose(file);
}


void Convolution_save(Convolution* layer) {
	FILE* file = fopen(layer->parameters_file_name, "w");
	if (file) {
		fprintf(file, "%d %d %d %d %d %d %d %d %d ",
			layer->channels,
			layer->height,
			layer->width,
			layer->matrix_c,
			layer->matrix_h,
			layer->matrix_w,
			layer->padding,
			layer->stride_h,
			layer->stride_w
		);
		fprintf(file, "%s ", layer->activation->name);

		fprintf(file, "%d ", layer->__train_allocated__);
		if (layer->__train_allocated__ == 1) {
			fprintf(file, "%lf %lf ", layer->learning_rate, layer->momentum);
			fprintf(file, "%s ", layer->loss->name);
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in Convolution_save(): file name is %s !!!\n", layer->parameters_file_name);
	}
	#endif // Debug
	fclose(file);
}


void Convolution_load_weights(Convolution* layer) {
	FILE* file = fopen(layer->weights_file_name, "r");
	if (file) {
		for (int mc = 0; mc < layer->matrix_c; ++mc) {
			for (int mh = 0; mh < layer->matrix_h; ++mh) {
				for (int c = 0; c < layer->channels; ++c) {
					for (int mw = 0; mw < layer->matrix_w; ++mw) {
						fscanf(file, "%lf ", &layer->matrix[mc][c][mh][mw]);
					}
				}
			}
		}

		for (int mc = 0; mc < layer->matrix_c; ++mc) {
			for (int oh = 0; oh < layer->__output_height__; ++oh) {
				for (int ow = 0; ow < layer->__output_width__; ++ow) {
					fscanf(file, "%lf ", &layer->bias[mc][oh][ow]);
				}
			}
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in Convolution_load_weights(): file (%s) doesn't exist !!!\n", layer->weights_file_name);
	}
	#endif // Debug
	fclose(file);
}


void Convolution_save_weights(Convolution* layer) {
	FILE* file = fopen(layer->weights_file_name, "w");
	if (file) {
		for (int mc = 0; mc < layer->matrix_c; ++mc) {
			for (int mh = 0; mh < layer->matrix_h; ++mh) {
				for (int c = 0; c < layer->channels; ++c) {
					for (int mw = 0; mw < layer->matrix_w; ++mw) {
						fprintf(file, "%lf ", layer->matrix[mc][c][mh][mw]);
					}
				}
			}
		}

		for (int mc = 0; mc < layer->matrix_c; ++mc) {
			for (int oh = 0; oh < layer->__output_height__; ++oh) {
				for (int ow = 0; ow < layer->__output_width__; ++ow) {
					fprintf(file, "%lf ", layer->bias[mc][oh][ow]);
				}
			}
		}
	}
	#ifdef Debug
	else {
		printf("!!! WARNING in Convolution_save_weights(): file name is %s !!!\n", layer->weights_file_name);
	}
	#endif // Debug
	fclose(file);
}
