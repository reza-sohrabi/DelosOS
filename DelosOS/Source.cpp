#include<iostream>
#include<vector>
#include <random>
#include<string>
#include <sstream> 
#include <math.h>
#include <iomanip>
#include "Tensor.h"
#include "Tensor.cpp"
using namespace std;


class nnUtil {
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
public:
	virtual Tensor<double>& forward(Tensor<double>& in) {
		return in;
	}
private:

};
class linearLayer : public layer {
public:
	linearLayer() {
	}
	~linearLayer() {
		delete this->weights;
		delete this->bias;
	}
	linearLayer(int n_units, int n_inputs, string activation) {
		this->n_units = n_units;
		this->n_inputs = n_inputs;
		this->activation = activation;
		shape.push_back(n_inputs);
		shape.push_back(n_units);
		weights = new Tensor<double>(shape, true);
		vector<int> bias_shape = { n_units };
		bias = new Tensor<double>(bias_shape, true);
	}
	Tensor<double>* getWeights() {
		return this->weights;
	}
	void setWeights(double* weights, vector<int> shape) {
		this->weights->setLinearData(weights, shape);
	}
	Tensor<double>& activate(Tensor<double>& in) {
		if (this->activation == "none")
			return in;
		Tensor<double> out;
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
		out.setLinearData(out_data, in.getShape());
		return out;
	}
	Tensor<double>& preActivate(Tensor<double>& in) {
		return Tensor<double>::matmul(in, *(this->weights)) + *(this->bias);
	}
	Tensor<double>& forward(Tensor<double>& in) {
		return this->activate(this->preActivate(in));
	}
private:
	int n_units;
	int n_inputs;
	string activation;
	vector<int> shape;
	Tensor<double>* weights;
	Tensor<double>* bias;
};
class model {
public:
private:


};
class optimizer {
public:
private:
};
class SGD : public optimizer {
public:
private:
};
class sequential : public model {
public:
	sequential() {}
	sequential(vector<layer> layers, Tensor<double> input) {
		this->layers = layers;
		this->input = input;
	}

	Tensor<double>& forward(Tensor<double> input) {
		Tensor<double> current_out = input;
		for (vector<layer>::iterator it = this->layers.begin(); it != this->layers.end(); it++) {
			current_out = it->forward(current_out);
		}
		return current_out;

	}
	Tensor<double>& forward() {
		return this->forward(this->input);
	}
private:
	vector<layer> layers;
	Tensor<double> input;
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