#include<iostream>
#include<vector>
#include <random>
#include<string>
#include <sstream> 
#include <math.h>
using namespace std;
template<class type> class Tensor {
public:
	Tensor() {

	}
	Tensor(vector<int> shape, bool random_initialization) {
		this->rank = shape.size();
		this->shape = shape;
		int size = 1;
		for (vector<int>::iterator it = shape.begin(); it != shape.end(); it++) {
			size *= *it;
		}
		this->size = size;
		if (random_initialization && (is_same< type, double>::value)) {
			data = new type[this->size];
			default_random_engine generator;
			normal_distribution<double> distribution(0, 1);
			for (int i = 0; i < size; i++) {
				this->data[i] = distribution(generator);
			}
		}

	}
	/*~Tensor() {
		delete[] this->data;
	}*/
	int getRank() {
		return this->rank;
	}
	int getSize() {
		return this->size;
	}
	vector<int> getShape() {
		return this->shape;
	}
	type* getLinearData() {
		return this->data;
	}
	void setLinearData(type* new_data , vector<int> shape) {
		delete[] this->data;
		int size=1;
		for (vector<int>::iterator it = shape.begin(); it != shape.end(); it++) {
			size *= *it;
		}
		this->size = size;
		this->rank = shape.size();
		this->shape = shape;
		this->data = new_data;
	}
	type* getDataFrom(string pattern) {
		//TODO
		return this->data;
	}

	string toString() {
		
		int counter = 0;
		vector<string> vec;
		string row = "[";
		for (int j = 1; j <= this->size; j++) {
			if (j%this->shape[this->rank - 1] != 0) {
				row.append(to_string(this->data[j-1]) + ", ");
			}
			else {
				row.append(to_string(this->data[j-1]) + "]");
					vec.push_back(row);
				row = "[";
			}
		}
		vector<string>* vecs = new vector<string>[this->rank];
		vecs[this->rank - 1] = vec;
		for (int i = this->rank - 1; i > 0; i--) {
			row = "[";
			for (int j = 1; j <= vecs[i].size(); j++) {
				if (j%this->shape[i-1] != 0) {
					row.append( " "+vecs[i].at(j-1)+ ",\n");
				}
				else {
					row.append(vecs[i].at(j-1) + "]");
					vecs[i-1].push_back(row);
					row = "[";
				}

			}
		}
		

		
		return vecs[0].at(0);

	}
	static Tensor<double>* matmul(Tensor<double> arg1, Tensor<double> arg2) {
		try
		{
			if ((arg1.getRank() > 2) || (arg2.getRank() > 2) || (arg1.getShape().at(1) != arg2.getShape().at(0))) {
				throw 0;
			}
		}
		catch (int e)
		{
			cout << "Tensors not compatible for matrix multiplication" << '\n';
		}
		cout << arg2.toString()<<endl << arg2.getSize();
		int out_size = arg1.getShape().at(0)*arg2.getShape().at(1);
		vector<int> out_shape{ arg1.getShape().at(0),arg2.getShape().at(1) };
		double* out_data = new double[out_size];
		int out_index = 0;
		double sum = 0.0;
		int rows = arg1.getShape().at(0);
		int columns = arg1.getShape().at(1);
		int columns_out = arg2.getShape().at(1);
		double* mat1 = arg1.getLinearData();
		double* mat2 = arg2.getLinearData();
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns_out; j++) {
				for (int k = 0; k < columns; k++)
					sum += mat1[i*columns + k] * mat2[j + k*columns_out];	
				out_data[out_index] = sum;
				out_index++;
				sum = 0.0;
			}
		}
		Tensor<double>* out = new Tensor<double>;
		out->setLinearData(out_data, out_shape);
		return out;		
	}
protected:
	type * data;
	int rank;
	int size;
	vector<int> shape;
	static string to_string(type const& value) {
		ostringstream sstr;
		sstr << value;
		return sstr.str();
	}

};
class layer {
public:
private:

};
class util {
public:
	static double sigmoid(double in) {
		return 1.0 / (1 + exp(-in));
	}
	static double tanh(double in) {
		return (exp(in)- exp(-in)) / (exp(in) + exp(-in));
	}
	static double relu(double in) {
		return max(0.0, in);
	}
};
class linearLayer: public layer{
public:
	linearLayer() {
	}
	~linearLayer() {
		delete this->weights;
	}
	linearLayer(int n_units, int n_inputs, string activation) {
		this->n_units = n_units;
		this->n_inputs = n_inputs;
		this->activation = activation;
		shape.push_back(n_inputs);
		shape.push_back(n_units);
		weights = new Tensor<double> (shape, true);
	}
	Tensor<double>* getWeights() {
		return this->weights;
	}
	void setWeights(double* weights, vector<int> shape) {
		this->weights->setLinearData(weights, shape);
	}
	Tensor<double>* activate(Tensor<double>* in) {
		if(this->activation=="none")
			return in;
		Tensor<double>* out = new Tensor<double>;
		double* out_data = new double[in->getSize()];
		double* in_data = in->getLinearData();
		for (int i = 0; i < in->getSize(); i++) {
			if (this->activation == "sigmoid") {
				out_data[i] = util::sigmoid(in_data[i]);
			}
			else if (this->activation == "relu") {
				out_data[i] = util::relu(in_data[i]);
			}
			else if (this->activation == "tanh") {
				out_data[i] = util::tanh(in_data[i]);
			}
		}
		out->setLinearData(out_data, in->getShape());
		return out;
	}
	Tensor<double>* preActivate(Tensor<double> in) {
		return Tensor<double>::matmul(in, *(this->weights));
	}
	Tensor<double>* forward(Tensor<double> in) {
		return this->activate(this->preActivate(in));
	}
private:
	int n_units;
	int n_inputs;
	string activation;
	vector<int> shape;
	Tensor<double>* weights;
};
void test() {
	vector<int> s{2,3};
	vector<int> ss{3,2};
	Tensor<double> T1(s, true);
	Tensor<double> T2(ss, true);
	cout << T1.toString()<<endl;
	cout << T2.toString() << endl;
	cout << Tensor<double>::matmul(T1,T2)->toString();
}
int main() {
	

	test();
	char c = getchar();
	return 0;

}