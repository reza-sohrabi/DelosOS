#include<iostream>
#include<vector>
#include <random>
#include<string>
#include <sstream> 
#include <math.h>
#include <iomanip>
#include "Tensor.h"
#include "Tensor.cpp" // included to resolve linkage issues of template classes
using namespace std;


class nnUtil {
	/*A class that contains util functions such as activations for neural networks*/

public:
	static double sigmoid(double in) {
		return 1.0 / (1 + exp(-in));
	}
	static double tanh(double in) {
		return (exp(in) - exp(-in)) / (exp(in) + exp(-in));
	}
	static double relu(double in) {
		return max(0.0, in);
	}
};
class layer {
	/*Abstract class for layers in a neural networks*/
public:
	virtual Tensor<double>& forward(Tensor<double>& in) = 0; // virtual method that given an input should give the output of the layer
private:

};
class convLayer : public layer {
	/*A class that implements the convolutional layer in a neural network*/
public:
	convLayer() {
	}
	convLayer(int n_filters, int kernel_size[], vector<int> input_shape, string activation) {
		/*Constructor that initializes weights and bias*/
		this->n_filters = n_filters;
		this->input_shape = input_shape;
		this->activation = activation;
		shape.push_back(kernel_size[0]);
		shape.push_back(kernel_size[1]);
		shape.push_back(*input_shape.end());
		shape.push_back(n_filters);
		weights = new Tensor<double>(shape, true);
		vector<int> bias_shape = { n_filters };
		bias = new Tensor<double>(bias_shape, true);
	}
	~convLayer() {
		delete[] this->weights;
		delete[] this->bias;
	}
private:
	int n_filters;// number of hidden units
	vector<int> input_shape;// input shape
	string activation;// type of activation
	vector<int> shape; // shape of the weights
	Tensor<double>* weights;// pointer to the weights
	Tensor<double>* bias;// pointer to the bias
};
class linearLayer : public layer {
	/*A class that implements the dense layer in a neural network*/
public:
	linearLayer() {
	}
	linearLayer(int n_units, int n_inputs, string activation) {
		/*Constructor that initializes weights and bias*/
		this->n_units = n_units;
		this->n_inputs = n_inputs;
		this->activation = activation;
		shape.push_back(n_inputs);
		shape.push_back(n_units);
		weights = new Tensor<double>(shape, true);
		vector<int> bias_shape = { n_units };
		bias = new Tensor<double>(bias_shape, true);
	}
	~linearLayer() {
		delete[] this->weights;
		delete[] this->bias;
	}
	Tensor<double>* getWeights() {
		/*Method that returns the pointer to the weights*/
		return this->weights;
	}
	Tensor<double>* getBias() {
		/*Method that returns the pointer to the bias*/
		return this->bias;
	}
	void setWeights(double* weights, vector<int> shape) {
		/*Method that sets the pointer to the weights and other Tensor properties */
		this->weights->setLinearData(weights, shape);
	}
	void setbias(double* weights, vector<int> shape) {
		/*Method that sets the pointer to the bias and other Tensor properties */
		this->bias->setLinearData(weights, shape);
	}
	Tensor<double>& activate(Tensor<double>& in) {
		/*Method that performs activation if there is one associated with the layer*/
		if (this->activation == "none")
			return in;
		Tensor<double> * out = new Tensor<double>;
		double* out_data = new double[in.getSize()];
		double* in_data = in.getLinearData();
		for (int i = 0; i < in.getSize(); i++) {
			if (this->activation == "sigmoid") {
				out_data[i] = nnUtil::sigmoid(in_data[i]);
			}
			else if (this->activation == "relu") {
				out_data[i] = nnUtil::relu(in_data[i]);
			}
			else if (this->activation == "tanh") {
				out_data[i] = nnUtil::tanh(in_data[i]);
			}
		}
		out->setLinearData(out_data, in.getShape());
		return *out;
	}
	Tensor<double>& preActivate(Tensor<double>& in) {
		/*Method that performs weights multiplication plus bias to the input*/
		return Tensor<double>::matmul(in, *(this->weights)) + *(this->bias);
	}
	Tensor<double>& forward(Tensor<double>& in) {
		/*Method that implements forward functionality in linearLayer*/
		return this->activate(this->preActivate(in));
	}
private:
	int n_units;// number of hidden units
	int n_inputs;// input dimensionality
	string activation;// type of activation
	vector<int> shape; // shape of the weights
	Tensor<double>* weights;// pointer to the weights
	Tensor<double>* bias;// pointer to the bias
};
class model {
	/*Base class for neural network model*/
public:
protected:
	vector<Tensor<double>> layer_outputs;

};
class optimizer {
	/*Base class for optimizers*/
public:
private:
};
class SGD : public optimizer {
	/*stochastic gradient descent optimizer*/
public:
private:
};
class sequential : public model {
	/*Class that implements a sequential model*/
public:
	sequential() {}
	sequential(vector<layer*> layers, Tensor<double> input) {
		/*Constructor that sets layers and input Tensor*/
		this->layers = layers;
		this->input = input;
	}

	Tensor<double>& forward(Tensor<double> input) {
		/*Method that performs forward in a model if the input has not been set*/
		Tensor<double> current_out = input;
		for (vector<layer*>::iterator it = this->layers.begin(); it != this->layers.end(); it++) {
			current_out = (*it)->forward(current_out);
			this->layer_outputs.push_back(current_out);
		}
		return current_out;

	}
	Tensor<double>& forward() {
		/*Method that performs forward in a model if the input has been set*/
		return this->forward(this->input);
	}
	void add(layer* new_layer) {
		/*Method that adds a new layer*/
		this->layers.push_back(new_layer);
	}
private:
	vector<layer*> layers;// layers in the model
	Tensor<double> input;// input Tensor to the model
	
};
void test_matmul() {
	vector<int> s{ 2,3,3 };
	vector<int> ss{ 3,2 };
	Tensor<double> T1(s, true);
	Tensor<double> T2(ss, true);
	cout << T1.toString() << endl;
	cout << T2.toString() << endl;
	cout << Tensor<double>::matmul(T1, T2).toString();
}
void test_add() {
	vector<int> s{ 1,3,3 };
	vector<int> ss{ 2,3,3 };
	Tensor<double> T1(s, true);
	Tensor<double> T2(ss, true);
	cout << T1.toString() << endl<<endl;
	cout << T2.toString() << endl<<endl;
	Tensor<double> res;
	res = (T1 + T2);
	cout << res.toString();
}
int main() {

	/*test_matmul();*/
	test_add();
	char c = getchar();
	return 0;

}